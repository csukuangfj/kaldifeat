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

  const FrameExtractionOptions &frame_opts =
      computer_.GetFrameOptions().frame_opts;

  torch::Tensor strided_input = GetStrided(wave, frame_opts);

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

  if (frame_opts.preemph_coeff != 0.0f) {
    KALDIFEAT_ASSERT(frame_opts.preemph_coeff >= 0.0f &&
                     frame_opts.preemph_coeff <= 1.0f);

    // right = strided_input[:, 1:]
    torch::Tensor right = strided_input.index(
        {"...", torch::indexing::Slice(1, torch::indexing::None,
                                       torch::indexing::None)});

    // current = strided_input[:, 0:-1]
    torch::Tensor current =
        strided_input.index({"...", torch::indexing::Slice(0, -1, 1)});

    // strided_input[1:, :] =
    //   strided_input[:, 1:] - preemph_coeff * strided_input[:, 0:-1]
    strided_input.index(
        {"...", torch::indexing::Slice(1, torch::indexing::None,
                                       torch::indexing::None)}) =
        right - frame_opts.preemph_coeff * current;

    strided_input.index({"...", 0}) *= frame_opts.preemph_coeff;
  }

  strided_input = feature_window_function_.Apply(strided_input);

#if 0
  Vector<BaseFloat> window;  // windowed waveform.
  bool use_raw_log_energy = computer_.NeedRawLogEnergy();
  for (int32 r = 0; r < rows_out; r++) {  // r is frame index.
    BaseFloat raw_log_energy = 0.0;
    ExtractWindow(0, wave, r, computer_.GetFrameOptions(),
                  feature_window_function_, &window,
                  (use_raw_log_energy ? &raw_log_energy : NULL));

    SubVector<BaseFloat> output_row(*output, r);
    computer_.Compute(raw_log_energy, vtln_warp, &window, &output_row);
  }
#endif
}

}  // namespace kaldifeat

#endif  // KALDIFEAT_CSRC_FEATURE_COMMON_INL_H_
