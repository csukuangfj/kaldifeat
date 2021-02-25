// kaldifeat/csrc/feature-fbank.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

// This file is copied/modified from kaldi/src/feat/feature-fbank.cc

#include "kaldifeat/csrc/feature-fbank.h"

#include <cmath>

#include "torch/fft.h"
#include "torch/torch.h"

namespace kaldifeat {

FbankComputer::FbankComputer(const FbankOptions &opts) : opts_(opts) {
  if (opts.energy_floor > 0.0f) log_energy_floor_ = logf(opts.energy_floor);

  // We'll definitely need the filterbanks info for VTLN warping factor 1.0.
  // [note: this call caches it.]
  GetMelBanks(1.0f);
}

FbankComputer::FbankComputer(const FbankComputer &other)
    : opts_(other.opts_),
      log_energy_floor_(other.log_energy_floor_),
      mel_banks_(other.mel_banks_) {
  for (auto iter = mel_banks_.begin(); iter != mel_banks_.end(); ++iter)
    iter->second = new MelBanks(*(iter->second));
}

FbankComputer::~FbankComputer() {
  for (auto iter = mel_banks_.begin(); iter != mel_banks_.end(); ++iter)
    delete iter->second;
}

const MelBanks *FbankComputer::GetMelBanks(float vtln_warp) {
  MelBanks *this_mel_banks = nullptr;

  // std::map<float, MelBanks *>::iterator iter = mel_banks_.find(vtln_warp);
  auto iter = mel_banks_.find(vtln_warp);
  if (iter == mel_banks_.end()) {
    this_mel_banks = new MelBanks(opts_.mel_opts, opts_.frame_opts, vtln_warp);
    mel_banks_[vtln_warp] = this_mel_banks;
  } else {
    this_mel_banks = iter->second;
  }
  return this_mel_banks;
}

// ans.shape [signal_frame.sizes()[0], this->Dim()]
torch::Tensor FbankComputer::Compute(torch::Tensor signal_raw_log_energy,
                                     float vtln_warp,
                                     const torch::Tensor &signal_frame) {
  const MelBanks &mel_banks = *(GetMelBanks(vtln_warp));

  KALDIFEAT_ASSERT(signal_frame.dim() == 2);

  KALDIFEAT_ASSERT(signal_frame.sizes()[1] ==
                   opts_.frame_opts.PaddedWindowSize());

  // torch.finfo(torch.float32).eps
  constexpr float kEps = 1.1920928955078125e-07f;

  // Compute energy after window function (not the raw one).
  if (opts_.use_energy && !opts_.raw_energy) {
    signal_raw_log_energy =
        torch::clamp_min(signal_frame.pow(2).sum(1), kEps).log();
  }

  // note spectrum is in magnitude, not power, because of `abs()`
  torch::Tensor spectrum = torch::fft::rfft(signal_frame).abs();

  // Use power instead of magnitude if requested.
  if (opts_.use_power) spectrum = spectrum.pow(2);

#if 0
  int32_t mel_offset = ((opts_.use_energy && !opts_.htk_compat) ? 1 : 0);
  SubVector<float> mel_energies(*feature, mel_offset, opts_.mel_opts.num_bins);
#endif

  torch::Tensor mel_energies = mel_banks.Compute(spectrum);
  if (opts_.use_log_fbank) {
    // Avoid log of zero (which should be prevented anyway by dithering).
    mel_energies = torch::clamp_min(mel_energies, kEps).log();
  }

  // Copy energy as first value (or the last, if htk_compat == true).
  if (opts_.use_energy) {
#if 0
    if (opts_.energy_floor > 0.0 && signal_raw_log_energy < log_energy_floor_) {
      signal_raw_log_energy = log_energy_floor_;
    }
#endif
    int32_t energy_index = opts_.htk_compat ? opts_.mel_opts.num_bins : 0;
    energy_index = 0;  // TODO(fangjun): fix it

    mel_energies.index({"...", energy_index}) = signal_raw_log_energy;
  }

  return mel_energies;
}

}  // namespace kaldifeat
