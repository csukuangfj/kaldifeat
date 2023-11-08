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

#ifndef KALDIFEAT_CSRC_WHISPER_FBANK_H_
#define KALDIFEAT_CSRC_WHISPER_FBANK_H_

#include <string>
#include <vector>

#include "kaldifeat/csrc/feature-common.h"
#include "kaldifeat/csrc/feature-window.h"
#include "kaldifeat/csrc/mel-computations.h"

namespace kaldifeat {

struct WhisperFbankOptions {
  FrameExtractionOptions frame_opts;

  torch::Device device{"cpu"};
  std::string ToString() const {
    std::ostringstream os;
    os << "WhisperFbankOptions(";
    os << "frame_opts=" << frame_opts.ToString() << ", ";
    os << "device=\"" << device << "\")";
    return os.str();
  }
};

class WhisperFbankComputer {
 public:
  // note: Only frame_opts.device is used. All other fields from frame_opts
  // are ignored
  explicit WhisperFbankComputer(const WhisperFbankOptions &opts = {});

  int32_t Dim() const { return 80; }

  const FrameExtractionOptions &GetFrameOptions() const {
    return opts_.frame_opts;
  }

  const WhisperFbankOptions &GetOptions() const { return opts_; }

  torch::Tensor Compute(torch::Tensor /*signal_raw_log_energy*/,
                        float /*vtln_warp*/, const torch::Tensor &signal_frame);

  // if true, compute log_energy_pre_window but after dithering and dc removal
  bool NeedRawLogEnergy() const { return false; }
  using Options = WhisperFbankOptions;

 private:
  WhisperFbankOptions opts_;
  MelBanks mel_banks_;
};

using WhisperFbank = OfflineFeatureTpl<WhisperFbankComputer>;

}  // namespace kaldifeat

#endif  // KALDIFEAT_CSRC_WHISPER_FBANK_H_
