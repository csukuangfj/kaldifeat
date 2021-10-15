// kaldifeat/python/csrc/utils.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifeat/python/csrc/utils.h"

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

#undef FROM_DICT
#undef AS_DICT

}  // namespace kaldifeat
