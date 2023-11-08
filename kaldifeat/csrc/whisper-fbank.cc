/**
 * Copyright (c)  2023  Xiaomi Corporation (authors: Fangjun Kuang)
 *
 * See LICENSE for clarification regarding multiple authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "kaldifeat/csrc/whisper-fbank.h"

#include <cmath>
#include <vector>

#include "kaldifeat/csrc/mel-computations.h"
#include "kaldifeat/csrc/whisper-mel-bank.h"

#ifndef M_2PI
#define M_2PI 6.283185307179586476925286766559005
#endif

namespace kaldifeat {

WhisperFbankComputer::WhisperFbankComputer(const WhisperFbankOptions &opts)
    : opts_(opts),
      mel_banks_(kWhisperMelArray, kWhisperMelRows, kWhisperMelCols,
                 opts.device) {
  opts_.frame_opts.samp_freq = 16000;
  opts_.frame_opts.frame_shift_ms = 10;
  opts_.frame_opts.frame_length_ms = 25;
  opts_.frame_opts.dither = 0;
  opts_.frame_opts.preemph_coeff = 0;
  opts_.frame_opts.remove_dc_offset = false;
  opts_.frame_opts.window_type = "hann";
  opts_.frame_opts.round_to_power_of_two = false;
  opts_.frame_opts.snip_edges = false;
}

torch::Tensor WhisperFbankComputer::Compute(
    torch::Tensor /*signal_raw_log_energy*/, float /*vtln_warp*/,
    const torch::Tensor &signal_frame) {
  KALDIFEAT_ASSERT(signal_frame.dim() == 2);
  KALDIFEAT_ASSERT(signal_frame.size(1) == opts_.frame_opts.PaddedWindowSize());

  // note spectrum is in magnitude, not power, because of `abs()`
#if defined(KALDIFEAT_HAS_FFT_NAMESPACE)
  // signal_frame shape: [x, 512]
  // power shape [x, 257]
  torch::Tensor power = torch::fft::rfft(signal_frame).abs().pow(2);
#else
  // signal_frame shape [x, 512]
  // real_imag shape [x, 257, 2],
  //   where [..., 0] is the real part
  //         [..., 1] is the imaginary part
  torch::Tensor real_imag = torch::rfft(signal_frame, 1);
  torch::Tensor real = real_imag.index({"...", 0});
  torch::Tensor imag = real_imag.index({"...", 1});
  torch::Tensor power = (real.square() + imag.square());
#endif

  torch::Tensor mel_energies = mel_banks_.Compute(power);
  torch::Tensor log_spec = torch::clamp_min(mel_energies, 1e-10).log10();
  log_spec = torch::maximum(log_spec, log_spec.max() - 8.0);
  torch::Tensor mel = (log_spec + 4.0) / 4.0;

  return mel;
}

}  // namespace kaldifeat
