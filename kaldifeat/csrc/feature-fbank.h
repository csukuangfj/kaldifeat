// kaldifeat/csrc/feature-fbank.h
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

// This file is copied/modified from kaldi/src/feat/feature-fbank.h

#ifndef KALDIFEAT_CSRC_FEATURE_FBANK_H_
#define KALDIFEAT_CSRC_FEATURE_FBANK_H_

#include <map>
#include <string>

#include "kaldifeat/csrc/feature-common.h"
#include "kaldifeat/csrc/feature-window.h"
#include "kaldifeat/csrc/mel-computations.h"

namespace kaldifeat {

struct FbankOptions {
  FrameExtractionOptions frame_opts;
  MelBanksOptions mel_opts;
  // append an extra dimension with energy to the filter banks
  bool use_energy = false;
  float energy_floor = 0.0f;  // active iff use_energy==true

  // If true, compute log_energy before preemphasis and windowing
  // If false, compute log_energy after preemphasis ans windowing
  bool raw_energy = true;  // active iff use_energy==true

  // If true, put energy last (if using energy)
  // If false, put energy first
  bool htk_compat = false;  // active iff use_energy==true

  // if true (default), produce log-filterbank, else linear
  bool use_log_fbank = true;

  // if true (default), use power in filterbank
  // analysis, else magnitude.
  bool use_power = true;

  torch::Device device{"cpu"};

  FbankOptions() { mel_opts.num_bins = 23; }

  std::string ToString() const {
    std::ostringstream os;
    os << "FbankOptions(";

    os << "frame_opts=" << frame_opts.ToString() << ", ";
    os << "mel_opts=" << mel_opts.ToString() << ", ";

    os << "use_energy=" << (use_energy ? "True" : "False") << ", ";
    os << "energy_floor=" << energy_floor << ", ";
    os << "raw_energy=" << (raw_energy ? "True" : "False") << ", ";
    os << "htk_compat=" << (htk_compat ? "True" : "False") << ", ";
    os << "use_log_fbank=" << (use_log_fbank ? "True" : "False") << ", ";
    os << "use_power=" << (use_power ? "True" : "False") << ", ";
    os << "device=\"" << device << "\")";
    return os.str();
  }
};

std::ostream &operator<<(std::ostream &os, const FbankOptions &opts);

class FbankComputer {
 public:
  using Options = FbankOptions;

  explicit FbankComputer(const FbankOptions &opts);
  ~FbankComputer();

  FbankComputer &operator=(const FbankComputer &) = delete;
  FbankComputer(const FbankComputer &) = delete;

  int32_t Dim() const {
    return opts_.mel_opts.num_bins + (opts_.use_energy ? 1 : 0);
  }

  // if true, compute log_energy_pre_window but after dithering and dc removal
  bool NeedRawLogEnergy() const { return opts_.use_energy && opts_.raw_energy; }

  const FrameExtractionOptions &GetFrameOptions() const {
    return opts_.frame_opts;
  }

  const FbankOptions &GetOptions() const { return opts_; }

  // signal_raw_log_energy is log_energy_pre_window, which is not empty
  // iff NeedRawLogEnergy() returns true.
  torch::Tensor Compute(torch::Tensor signal_raw_log_energy, float vtln_warp,
                        const torch::Tensor &signal_frame);

 private:
  const MelBanks *GetMelBanks(float vtln_warp);

  FbankOptions opts_;
  float log_energy_floor_;
  std::map<float, MelBanks *> mel_banks_;  // float is VTLN coefficient.
};

using Fbank = OfflineFeatureTpl<FbankComputer>;

}  // namespace kaldifeat

#endif  // KALDIFEAT_CSRC_FEATURE_FBANK_H_
