// kaldifeat/csrc/feature-fbank.h
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

// This file is copied/modified from kaldi/src/feat/feature-fbank.h

#ifndef KALDIFEAT_CSRC_FEATURE_FBANK_H_
#define KALDIFEAT_CSRC_FEATURE_FBANK_H_

#include <map>

#include "kaldifeat/csrc/feature-window.h"
#include "kaldifeat/csrc/mel-computations.h"

namespace kaldifeat {

struct FbankOptions {
  FrameExtractionOptions frame_opts;
  MelBanksOptions mel_opts;
  // append an extra dimension with energy to the filter banks
  bool use_energy = false;
  float energy_floor = 0.0f;

  // If true, compute energy before preemphasis and windowing
  bool raw_energy = true;
  // If true, put energy last (if using energy)
  bool htk_compat = false;
  // if true (default), produce log-filterbank, else linear
  bool use_log_fbank = true;

  // if true (default), use power in filterbank
  // analysis, else magnitude.
  bool use_power = true;

  FbankOptions() { mel_opts.num_bins = 23; }
};

class FbankComputer {
 public:
  explicit FbankComputer(const FbankOptions &opts);
  ~FbankComputer();

  FbankComputer &operator=(const FbankComputer &) = delete;

  FbankComputer(const FbankComputer &other);

  int32_t Dim() const {
    return opts_.mel_opts.num_bins + (opts_.use_energy ? 1 : 0);
  }

  bool NeedRawLogEnergy() const { return opts_.use_energy && opts_.raw_energy; }

  const FrameExtractionOptions &GetFrameOptions() const {
    return opts_.frame_opts;
  }

  torch::Tensor Compute(torch::Tensor signal_raw_log_energy, float vtln_warp,
                        const torch::Tensor &signal_frame);

 private:
  const MelBanks *GetMelBanks(float vtln_warp);

  FbankOptions opts_;
  float log_energy_floor_;
  std::map<float, MelBanks *> mel_banks_;  // float is VTLN coefficient.
};

}  // namespace kaldifeat

#endif  // KALDIFEAT_CSRC_FEATURE_FBANK_H_
