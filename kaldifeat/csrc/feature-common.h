// kaldifeat/csrc/feature-common.h
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

// This file is copied/modified from kaldi/src/feat/feature-common.h

#ifndef KALDIFEAT_CSRC_FEATURE_COMMON_H_
#define KALDIFEAT_CSRC_FEATURE_COMMON_H_

#include "kaldifeat/csrc/feature-window.h"

namespace kaldifeat {

template <class F>
class OfflineFeatureTpl {
 public:
  using Options = typename F::Options;

  // Note: feature_window_function_ is the windowing function, which initialized
  // using the options class, that we cache at this level.
  OfflineFeatureTpl(const Options &opts)
      : computer_(opts),
        feature_window_function_(computer_.GetFrameOptions()) {}

  /**
     Computes the features for one file (one sequence of features).
     This is the newer interface where you specify the sample frequency
     of the input waveform.
       @param [in] wave   The input waveform
       @param [in] sample_freq  The sampling frequency with which
                                'wave' was sampled.
                                if sample_freq is higher than the frequency
                                specified in the config, we will downsample
                                the waveform, but if lower, it's an error.
     @param [in] vtln_warp  The VTLN warping factor (will normally
                            be 1.0)
     @param [out]  output  The matrix of features, where the row-index
                           is the frame index.
  */
  torch::Tensor ComputeFeatures(const torch::Tensor &wave, float vtln_warp);

  int32_t Dim() const { return computer_.Dim(); }

  // Copy constructor.
  OfflineFeatureTpl(const OfflineFeatureTpl<F> &) = delete;
  OfflineFeatureTpl<F> &operator=(const OfflineFeatureTpl<F> &) = delete;

 private:
  F computer_;
  FeatureWindowFunction feature_window_function_;
};

}  // namespace kaldifeat

#include "kaldifeat/csrc/feature-common-inl.h"

#endif  // KALDIFEAT_CSRC_FEATURE_COMMON_H_
