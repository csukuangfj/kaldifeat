// kaldifeat/csrc/feature-common-inl.h
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

// This file is copied/modified from kaldi/src/feat/feature-common-inl.h

#ifndef KALDIFEAT_CSRC_FEATURE_COMMON_INL_H_
#define KALDIFEAT_CSRC_FEATURE_COMMON_INL_H_

#include "kaldifeat/csrc/feature-window.h"

namespace kaldifeat {

template <class F>
torch::Tensor OfflineFeatureTpl<F>::ComputeFeatures(const torch::Tensor &wave,
                                                    float vtln_warp) {
  KALDIFEAT_ASSERT(wave.dim() == 1);
  int32_t rows_out = NumFrames(wave.sizes()[0], computer_.GetFrameOptions());
  int32_t cols_out = computer_.Dim();

  const FrameExtractionOptions &frame_opts = computer_.GetFrameOptions();

  // TODO(fangjun): avoid clone
  torch::Tensor strided_input = GetStrided(wave, frame_opts).clone();

  if (frame_opts.dither != 0)
    strided_input = Dither(strided_input, frame_opts.dither);

  if (frame_opts.remove_dc_offset) {
    torch::Tensor row_means = strided_input.mean(1).unsqueeze(1);
    strided_input -= row_means;
  }

  bool use_raw_log_energy = computer_.NeedRawLogEnergy();
  torch::Tensor log_energy_pre_window;

  // torch.finfo(torch.float32).eps
  constexpr float kEps = 1.1920928955078125e-07f;

  if (use_raw_log_energy) {
    log_energy_pre_window =
        torch::clamp_min(strided_input.pow(2).sum(1), kEps).log();
  }

  if (frame_opts.preemph_coeff != 0.0f)
    Preemphasize(frame_opts.preemph_coeff, &strided_input);

  feature_window_function_.Apply(&strided_input);

  int32_t padding = frame_opts.PaddedWindowSize() - strided_input.sizes()[1];

  if (padding > 0) {
    strided_input = torch::nn::functional::pad(
        strided_input, torch::nn::functional::PadFuncOptions({0, padding})
                           .mode(torch::kConstant)
                           .value(0));
  }

  return computer_.Compute(log_energy_pre_window, vtln_warp, strided_input);
}

}  // namespace kaldifeat

#endif  // KALDIFEAT_CSRC_FEATURE_COMMON_INL_H_
