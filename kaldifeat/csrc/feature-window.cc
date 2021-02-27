// kaldifeat/csrc/feature-window.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

// This file is copied/modified from kaldi/src/feat/feature-window.cc

#include "kaldifeat/csrc/feature-window.h"

#include <cmath>

#include "torch/torch.h"

#ifndef M_2PI
#define M_2PI 6.283185307179586476925286766559005
#endif

namespace kaldifeat {

std::ostream &operator<<(std::ostream &os, const FrameExtractionOptions &opts) {
  os << opts.ToString();
  return os;
}

FeatureWindowFunction::FeatureWindowFunction(
    const FrameExtractionOptions &opts) {
  int32_t frame_length = opts.WindowSize();
  KALDIFEAT_ASSERT(frame_length > 0);

  window = torch::empty({frame_length}, torch::kFloat32);
  float *window_data = window.data_ptr<float>();

  double a = M_2PI / (frame_length - 1);
  for (int32_t i = 0; i < frame_length; i++) {
    double i_fl = static_cast<double>(i);
    if (opts.window_type == "hanning") {
      window_data[i] = 0.5 - 0.5 * cos(a * i_fl);
    } else if (opts.window_type == "sine") {
      // when you are checking ws wikipedia, please
      // note that 0.5 * a = M_PI/(frame_length-1)
      window_data[i] = sin(0.5 * a * i_fl);
    } else if (opts.window_type == "hamming") {
      window_data[i] = 0.54 - 0.46 * cos(a * i_fl);
    } else if (opts.window_type ==
               "povey") {  // like hamming but goes to zero at edges.
      window_data[i] = pow(0.5 - 0.5 * cos(a * i_fl), 0.85);
    } else if (opts.window_type == "rectangular") {
      window_data[i] = 1.0;
    } else if (opts.window_type == "blackman") {
      window_data[i] = opts.blackman_coeff - 0.5 * cos(a * i_fl) +
                       (0.5 - opts.blackman_coeff) * cos(2 * a * i_fl);
    } else {
      KALDIFEAT_ERR << "Invalid window type " << opts.window_type;
    }
  }

  window = window.unsqueeze(0);
}

void FeatureWindowFunction::Apply(torch::Tensor *wave) const {
  KALDIFEAT_ASSERT(wave->dim() == 2);
  KALDIFEAT_ASSERT(wave->sizes()[1] == wave->sizes()[1]);
  wave->mul_(window);
}

static int64_t FirstSampleOfFrame(int32_t frame,
                                  const FrameExtractionOptions &opts) {
  int64_t frame_shift = opts.WindowShift();
  if (opts.snip_edges) {
    return frame * frame_shift;
  } else {
    int64_t midpoint_of_frame = frame_shift * frame + frame_shift / 2,
            beginning_of_frame = midpoint_of_frame - opts.WindowSize() / 2;
    return beginning_of_frame;
  }
}

int32_t NumFrames(int64_t num_samples, const FrameExtractionOptions &opts,
                  bool flush /*= true*/) {
  int64_t frame_shift = opts.WindowShift();
  int64_t frame_length = opts.WindowSize();
  if (opts.snip_edges) {
    // with --snip-edges=true (the default), we use a HTK-like approach to
    // determining the number of frames-- all frames have to fit completely into
    // the waveform, and the first frame begins at sample zero.
    if (num_samples < frame_length)
      return 0;
    else
      return (1 + ((num_samples - frame_length) / frame_shift));
    // You can understand the expression above as follows: 'num_samples -
    // frame_length' is how much room we have to shift the frame within the
    // waveform; 'frame_shift' is how much we shift it each time; and the ratio
    // is how many times we can shift it (integer arithmetic rounds down).
  } else {
    // if --snip-edges=false, the number of frames is determined by rounding the
    // (file-length / frame-shift) to the nearest integer.  The point of this
    // formula is to make the number of frames an obvious and predictable
    // function of the frame shift and signal length, which makes many
    // segmentation-related questions simpler.
    //
    // Because integer division in C++ rounds toward zero, we add (half the
    // frame-shift minus epsilon) before dividing, to have the effect of
    // rounding towards the closest integer.
    int32_t num_frames = (num_samples + (frame_shift / 2)) / frame_shift;

    if (flush) return num_frames;

    // note: 'end' always means the last plus one, i.e. one past the last.
    int64_t end_sample_of_last_frame =
        FirstSampleOfFrame(num_frames - 1, opts) + frame_length;

    // the following code is optimized more for clarity than efficiency.
    // If flush == false, we can't output frames that extend past the end
    // of the signal.
    while (num_frames > 0 && end_sample_of_last_frame > num_samples) {
      num_frames--;
      end_sample_of_last_frame -= frame_shift;
    }
    return num_frames;
  }
}

torch::Tensor GetStrided(const torch::Tensor &wave,
                         const FrameExtractionOptions &opts) {
  KALDIFEAT_ASSERT(wave.dim() == 1);

  std::vector<int64_t> strides = {opts.WindowShift() * wave.strides()[0],
                                  wave.strides()[0]};

  KALDIFEAT_ASSERT(opts.snip_edges == true);  // FIXME(fangjun)

  int64_t num_samples = wave.sizes()[0];
  int32_t num_frames = NumFrames(num_samples, opts);
  std::vector<int64_t> sizes = {num_frames, opts.WindowSize()};

  return wave.as_strided(sizes, strides);
}

torch::Tensor Dither(const torch::Tensor &wave, float dither_value) {
  if (dither_value == 0.0f) wave;

  torch::Tensor rand_gauss = torch::randn_like(wave);
#if 1
  return wave + rand_gauss * dither_value;
#else
  // use in-place version of wave and change its to pointer type
  wave_->add_(rand_gauss, dither_value);
#endif
}

void Preemphasize(float preemph_coeff, torch::Tensor *wave) {
  if (preemph_coeff == 0.0f) return;

  KALDIFEAT_ASSERT(preemph_coeff >= 0.0f && preemph_coeff <= 1.0f);

  // right = wave[:, 1:]
  torch::Tensor right =
      wave->index({"...", torch::indexing::Slice(1, torch::indexing::None,
                                                 torch::indexing::None)});

  // current = wave[:, 0:-1]
  torch::Tensor current = wave->index(
      {"...", torch::indexing::Slice(0, -1, torch::indexing::None)});

  // wave[1:, :] = wave[:, 1:] - preemph_coeff * wave[:, 0:-1]
  wave->index({"...", torch::indexing::Slice(1, torch::indexing::None,
                                             torch::indexing::None)}) =
      right - preemph_coeff * current;

  wave->index({"...", 0}) *= 1 - preemph_coeff;
}

}  // namespace kaldifeat
