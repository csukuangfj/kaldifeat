// kaldifeat/python/csrc/mel-computations.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifeat/csrc/mel-computations.h"

#include "kaldifeat/python/csrc/feature-window.h"

namespace kaldifeat {

void PybindMelBanksOptions(py::module &m) {
  py::class_<MelBanksOptions>(m, "MelBanksOptions")
      .def(py::init<>())
      .def_readwrite("num_bins", &MelBanksOptions::num_bins)
      .def_readwrite("low_freq", &MelBanksOptions::low_freq)
      .def_readwrite("high_freq", &MelBanksOptions::high_freq)
      .def_readwrite("vtln_low", &MelBanksOptions::vtln_low)
      .def_readwrite("vtln_high", &MelBanksOptions::vtln_high)
      .def_readwrite("debug_mel", &MelBanksOptions::debug_mel)
      .def_readwrite("htk_mode", &MelBanksOptions::htk_mode)
      .def("__str__", [](const MelBanksOptions &self) -> std::string {
        return self.ToString();
      });
  ;
}

}  // namespace kaldifeat
