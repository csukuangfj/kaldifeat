// kaldifeat/csrc/feature-window.h
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

// This file is copied/modified from kaldi/src/feat/feature-window.h

#include <string>

#include "kaldifeat/csrc/log.h"
#include "torch/all.h"
#include "torch/script.h"

#ifndef KALDIFEAT_CSRC_FEATURE_WINDOW_H_
#define KALDIFEAT_CSRC_FEATURE_WINDOW_H_

namespace kaldifeat {

inline int32_t RoundUpToNearestPowerOfTwo(int32_t n) {
  // copied from kaldi/src/base/kaldi-math.cc
  KALDIFEAT_ASSERT(n > 0);
  n--;
  n |= n >> 1;
  n |= n >> 2;
  n |= n >> 4;
  n |= n >> 8;
  n |= n >> 16;
  return n + 1;
}

struct FrameExtractionOptions {
  float samp_freq = 16000;
  float frame_shift_ms = 10.0f;   // in milliseconds.
  float frame_length_ms = 25.0f;  // in milliseconds.
  float dither = 1.0f;            // Amount of dithering, 0.0 means no dither.
  float preemph_coeff = 0.97f;    // Preemphasis coefficient.
  bool remove_dc_offset = true;   // Subtract mean of wave before FFT.
  std::string window_type = "povey";  // e.g. Hamming window
  // May be "hamming", "rectangular", "povey", "hanning", "sine", "blackman"
  // "povey" is a window I made to be similar to Hamming but to go to zero at
  // the edges, it's pow((0.5 - 0.5*cos(n/N*2*pi)), 0.85) I just don't think the
  // Hamming window makes sense as a windowing function.
  bool round_to_power_of_two = true;
  float blackman_coeff = 0.42f;
  bool snip_edges = true;
  // bool allow_downsample = false;
  // bool allow_upsample = false;

  // Used for streaming feature extraction. It indicates the number
  // of feature frames to keep in the recycling vector. -1 means to
  // keep all feature frames.
  int32_t max_feature_vectors = -1;

  int32_t WindowShift() const {
    return static_cast<int32_t>(samp_freq * 0.001f * frame_shift_ms);
  }
  int32_t WindowSize() const {
    return static_cast<int32_t>(samp_freq * 0.001f * frame_length_ms);
  }
  int32_t PaddedWindowSize() const {
    return (round_to_power_of_two ? RoundUpToNearestPowerOfTwo(WindowSize())
                                  : WindowSize());
  }
  std::string ToString() const {
    std::ostringstream os;
    os << "FrameExtractionOptions(";
    os << "samp_freq=" << samp_freq << ", ";
    os << "frame_shift_ms=" << frame_shift_ms << ", ";
    os << "frame_length_ms=" << frame_length_ms << ", ";
    os << "dither=" << dither << ", ";
    os << "preemph_coeff=" << preemph_coeff << ", ";
    os << "remove_dc_offset=" << (remove_dc_offset ? "True" : "False") << ", ";
    os << "window_type=" << '"' << window_type << '"' << ", ";
    os << "round_to_power_of_two=" << (round_to_power_of_two ? "True" : "False")
       << ", ";
    os << "blackman_coeff=" << blackman_coeff << ", ";
    os << "snip_edges=" << (snip_edges ? "True" : "False") << ", ";
    os << "max_feature_vectors=" << max_feature_vectors << ")";

    return os.str();
  }
};

std::ostream &operator<<(std::ostream &os, const FrameExtractionOptions &opts);

class FeatureWindowFunction {
 public:
  FeatureWindowFunction() = default;
  FeatureWindowFunction(const FrameExtractionOptions &opts,
                        torch::Device device);
  torch::Tensor Apply(const torch::Tensor &wave) const;

 private:
  torch::Tensor window;
};

/**
   This function returns the number of frames that we can extract from a wave
   file with the given number of samples in it (assumed to have the same
   sampling rate as specified in 'opts').

      @param [in] num_samples  The number of samples in the wave file.
      @param [in] opts     The frame-extraction options class

      @param [in] flush   True if we are asserting that this number of samples
   is 'all there is', false if we expecting more data to possibly come in.  This
   only makes a difference to the answer
   if opts.snips_edges== false.  For offline feature extraction you always want
   flush == true.  In an online-decoding context, once you know (or decide) that
   no more data is coming in, you'd call it with flush == true at the end to
   flush out any remaining data.
*/
int32_t NumFrames(int64_t num_samples, const FrameExtractionOptions &opts,
                  bool flush = true);

int64_t FirstSampleOfFrame(int32_t frame, const FrameExtractionOptions &opts);

// return a 2-d tensor with shape [num_frames, opts.WindowSize()]
//
// Suppose the wave contains samples [a, b, c, d, e, f],
// windows size is 4 and window shift is 2
//
// if opt.snip_edges is true, it returns:
//   a b c d
//   c d e f
//
// if opt.snip_edges is false, it returns
//   a a b c
//   b c d e
//   d e f f
// (Note, it use reflections at the end. That is
//  abcdef is reflected to fedcba|abcdef|fedcba)
torch::Tensor GetStrided(const torch::Tensor &wave,
                         const FrameExtractionOptions &opts);

torch::Tensor Dither(const torch::Tensor &wave, float dither_value);

torch::Tensor Preemphasize(float preemph_coeff, const torch::Tensor &wave);

/*
  ExtractWindow() extracts "frame_length" samples from the given waveform.
  Note: This function only extracts "frame_length" samples
  from the input waveform, without any further processing.

  @param [in] sample_offset  If 'wave' is not the entire waveform, but
                   part of it to the left has been discarded, then the
                   number of samples prior to 'wave' that we have
                   already discarded.  Set this to zero if you are
                   processing the entire waveform in one piece, or
                   if you get 'no matching function' compilation
                   errors when updating the code.
  @param [in] wave  The waveform
  @param [in] f     The frame index to be extracted, with
                    0 <= f < NumFrames(sample_offset + wave.numel(), opts, true)
  @param [in] opts  The options class to be used
  @return  Return a tensor containing "frame_length" samples extracted from
           `wave`, without any further processing. Its shape is
           (1, frame_length).
*/
torch::Tensor ExtractWindow(int64_t sample_offset, const torch::Tensor &wave,
                            int32_t f, const FrameExtractionOptions &opts);

}  // namespace kaldifeat

#endif  // KALDIFEAT_CSRC_FEATURE_WINDOW_H_
