// kaldifeat/csrc/feature-spectrogram.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

// This file is copied/modified from kaldi/src/feat/feature-spectrogram.cc

#include "kaldifeat/csrc/feature-spectrogram.h"

namespace kaldifeat {

std::ostream &operator<<(std::ostream &os, const SpectrogramOptions &opts) {
  os << opts.ToString();
  return os;
}

SpectrogramComputer::SpectrogramComputer(const SpectrogramOptions &opts)
    : opts_(opts) {
  if (opts.energy_floor > 0.0) log_energy_floor_ = logf(opts.energy_floor);
}

// ans.shape [signal_frame.size(0), this->Dim()]
torch::Tensor SpectrogramComputer::Compute(torch::Tensor signal_raw_log_energy,
                                           float vtln_warp,
                                           const torch::Tensor &signal_frame) {
  KALDIFEAT_ASSERT(signal_frame.dim() == 2);

  KALDIFEAT_ASSERT(signal_frame.size(1) == opts_.frame_opts.PaddedWindowSize());

  // torch.finfo(torch.float32).eps
  constexpr float kEps = 1.1920928955078125e-07f;

  // Compute energy after window function (not the raw one).
  if (!opts_.raw_energy) {
    signal_raw_log_energy =
        torch::clamp_min(signal_frame.pow(2).sum(1), kEps).log();
  }

  // note spectrum is in magnitude, not power, because of `abs()`
#if defined(KALDIFEAT_HAS_FFT_NAMESPACE)
  // signal_frame shape: [x, 512]
  // spectrum shape [x, 257
  torch::Tensor spectrum = torch::fft::rfft(signal_frame).abs();
#else
  // signal_frame shape [x, 512]
  // real_imag shape [x, 257, 2],
  //   where [..., 0] is the real part
  //         [..., 1] is the imaginary part
  torch::Tensor real_imag = torch::rfft(signal_frame, 1);
  torch::Tensor real = real_imag.index({"...", 0});
  torch::Tensor imag = real_imag.index({"...", 1});
  torch::Tensor spectrum = (real.square() + imag.square()).sqrt();
#endif

  if (opts_.return_raw_fft) {
    KALDIFEAT_ERR << "return raw fft is not supported yet";
  }

  // compute power spectrum
  spectrum = spectrum.pow(2);

  // NOTE: take the log
  spectrum = torch::clamp_min(spectrum, kEps).log();

  if (opts_.energy_floor > 0.0f) {
    signal_raw_log_energy =
        torch::clamp_min(signal_raw_log_energy, log_energy_floor_);
  }

  // The zeroth spectrogram component is always set to the signal energy,
  // instead of the square of the constant component of the signal.
  //
  // spectrum[:,0] = signal_raw_log_energy
  spectrum.index({"...", 0}) = signal_raw_log_energy;

  return spectrum;
}

}  // namespace kaldifeat
