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
}

}  // namespace kaldifeat
