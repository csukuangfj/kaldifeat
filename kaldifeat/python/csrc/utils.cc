// kaldifeat/python/csrc/utils.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifeat/python/csrc/utils.h"

#include <string>

#include "kaldifeat/csrc/feature-window.h"

#define FROM_DICT(type, key)         \
  if (dict.contains(#key)) {         \
    opts.key = py::type(dict[#key]); \
  }

#define AS_DICT(key) dict[#key] = opts.key

namespace kaldifeat {

FrameExtractionOptions FrameExtractionOptionsFromDict(py::dict dict) {
  FrameExtractionOptions opts;

  FROM_DICT(float_, samp_freq);
  FROM_DICT(float_, frame_shift_ms);
  FROM_DICT(float_, frame_length_ms);
  FROM_DICT(float_, dither);
  FROM_DICT(float_, preemph_coeff);
  FROM_DICT(bool_, remove_dc_offset);
  FROM_DICT(str, window_type);
  FROM_DICT(bool_, round_to_power_of_two);
  FROM_DICT(float_, blackman_coeff);
  FROM_DICT(bool_, snip_edges);

  return opts;
}

py::dict AsDict(const FrameExtractionOptions &opts) {
  py::dict dict;

  AS_DICT(samp_freq);
  AS_DICT(frame_shift_ms);
  AS_DICT(frame_length_ms);
  AS_DICT(dither);
  AS_DICT(preemph_coeff);
  AS_DICT(remove_dc_offset);
  AS_DICT(window_type);
  AS_DICT(round_to_power_of_two);
  AS_DICT(blackman_coeff);
  AS_DICT(snip_edges);

  return dict;
}

MelBanksOptions MelBanksOptionsFromDict(py::dict dict) {
  MelBanksOptions opts;

  FROM_DICT(int_, num_bins);
  FROM_DICT(float_, low_freq);
  FROM_DICT(float_, high_freq);
  FROM_DICT(float_, vtln_low);
  FROM_DICT(float_, vtln_high);
  FROM_DICT(bool_, debug_mel);
  FROM_DICT(bool_, htk_mode);

  return opts;
}
py::dict AsDict(const MelBanksOptions &opts) {
  py::dict dict;

  AS_DICT(num_bins);
  AS_DICT(low_freq);
  AS_DICT(high_freq);
  AS_DICT(vtln_low);
  AS_DICT(vtln_high);
  AS_DICT(debug_mel);
  AS_DICT(htk_mode);

  return dict;
}

FbankOptions FbankOptionsFromDict(py::dict dict) {
  FbankOptions opts;

  if (dict.contains("frame_opts")) {
    opts.frame_opts = FrameExtractionOptionsFromDict(dict["frame_opts"]);
  }

  if (dict.contains("mel_opts")) {
    opts.mel_opts = MelBanksOptionsFromDict(dict["mel_opts"]);
  }

  FROM_DICT(bool_, use_energy);
  FROM_DICT(float_, energy_floor);
  FROM_DICT(bool_, raw_energy);
  FROM_DICT(bool_, htk_compat);
  FROM_DICT(bool_, use_log_fbank);
  FROM_DICT(bool_, use_power);

  if (dict.contains("device")) {
    opts.device = torch::Device(std::string(py::str(dict["device"])));
  }

  return opts;
}

py::dict AsDict(const FbankOptions &opts) {
  py::dict dict;

  dict["frame_opts"] = AsDict(opts.frame_opts);
  dict["mel_opts"] = AsDict(opts.mel_opts);
  AS_DICT(use_energy);
  AS_DICT(energy_floor);
  AS_DICT(raw_energy);
  AS_DICT(htk_compat);
  AS_DICT(use_log_fbank);
  AS_DICT(use_power);

  auto torch_device = py::module_::import("torch").attr("device");
  dict["device"] = torch_device(opts.device.str());

  return dict;
}

MfccOptions MfccOptionsFromDict(py::dict dict) {
  MfccOptions opts;

  if (dict.contains("frame_opts")) {
    opts.frame_opts = FrameExtractionOptionsFromDict(dict["frame_opts"]);
  }

  if (dict.contains("mel_opts")) {
    opts.mel_opts = MelBanksOptionsFromDict(dict["mel_opts"]);
  }

  FROM_DICT(int_, num_ceps);
  FROM_DICT(bool_, use_energy);
  FROM_DICT(float_, energy_floor);
  FROM_DICT(bool_, raw_energy);
  FROM_DICT(float_, cepstral_lifter);
  FROM_DICT(bool_, htk_compat);

  if (dict.contains("device")) {
    opts.device = torch::Device(std::string(py::str(dict["device"])));
  }

  return opts;
}

py::dict AsDict(const MfccOptions &opts) {
  py::dict dict;

  dict["frame_opts"] = AsDict(opts.frame_opts);
  dict["mel_opts"] = AsDict(opts.mel_opts);

  AS_DICT(num_ceps);
  AS_DICT(use_energy);
  AS_DICT(energy_floor);
  AS_DICT(raw_energy);
  AS_DICT(cepstral_lifter);
  AS_DICT(htk_compat);

  auto torch_device = py::module_::import("torch").attr("device");
  dict["device"] = torch_device(opts.device.str());

  return dict;
}

SpectrogramOptions SpectrogramOptionsFromDict(py::dict dict) {
  SpectrogramOptions opts;

  if (dict.contains("frame_opts")) {
    opts.frame_opts = FrameExtractionOptionsFromDict(dict["frame_opts"]);
  }

  FROM_DICT(float_, energy_floor);
  FROM_DICT(bool_, raw_energy);
  // FROM_DICT(bool_, return_raw_fft);

  if (dict.contains("device")) {
    opts.device = torch::Device(std::string(py::str(dict["device"])));
  }

  return opts;
}

py::dict AsDict(const SpectrogramOptions &opts) {
  py::dict dict;

  dict["frame_opts"] = AsDict(opts.frame_opts);

  AS_DICT(energy_floor);
  AS_DICT(raw_energy);

  auto torch_device = py::module_::import("torch").attr("device");
  dict["device"] = torch_device(opts.device.str());

  return dict;
}

PlpOptions PlpOptionsFromDict(py::dict dict) {
  PlpOptions opts;

  if (dict.contains("frame_opts")) {
    opts.frame_opts = FrameExtractionOptionsFromDict(dict["frame_opts"]);
  }

  if (dict.contains("mel_opts")) {
    opts.mel_opts = MelBanksOptionsFromDict(dict["mel_opts"]);
  }

  FROM_DICT(int_, lpc_order);
  FROM_DICT(int_, num_ceps);
  FROM_DICT(bool_, use_energy);
  FROM_DICT(float_, energy_floor);
  FROM_DICT(bool_, raw_energy);
  FROM_DICT(float_, compress_factor);
  FROM_DICT(int_, cepstral_lifter);
  FROM_DICT(float_, cepstral_scale);
  FROM_DICT(bool_, htk_compat);

  if (dict.contains("device")) {
    opts.device = torch::Device(std::string(py::str(dict["device"])));
  }

  return opts;
}

py::dict AsDict(const PlpOptions &opts) {
  py::dict dict;

  dict["frame_opts"] = AsDict(opts.frame_opts);
  dict["mel_opts"] = AsDict(opts.mel_opts);

  AS_DICT(lpc_order);
  AS_DICT(num_ceps);
  AS_DICT(use_energy);
  AS_DICT(energy_floor);
  AS_DICT(raw_energy);
  AS_DICT(compress_factor);
  AS_DICT(cepstral_lifter);
  AS_DICT(cepstral_scale);
  AS_DICT(htk_compat);

  auto torch_device = py::module_::import("torch").attr("device");
  dict["device"] = torch_device(opts.device.str());

  return dict;
}

#undef FROM_DICT
#undef AS_DICT

}  // namespace kaldifeat
