// kaldifeat/csrc/feature-window.h
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

// This file is copied/modified from kaldi/src/feat/feature-window.h

#include "kaldifeat/csrc/log.h"
#include "torch/torch.h"

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
  bool allow_downsample = false;
  bool allow_upsample = false;
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
};

struct FeatureWindowFunction {
  FeatureWindowFunction() = default;
  explicit FeatureWindowFunction(const FrameExtractionOptions &opts);
  torch::Tensor window;
};

}  // namespace kaldifeat

#endif  // KALDIFEAT_CSRC_FEATURE_WINDOW_H_
