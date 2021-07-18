// kaldifeat/python/csrc/mel-computations.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifeat/python/csrc/mel-computations.h"

#include <string>

#include "kaldifeat/csrc/mel-computations.h"

namespace kaldifeat {

static void PybindMelBanksOptions(py::module &m) {
  using PyClass = MelBanksOptions;
  py::class_<PyClass>(m, "MelBanksOptions")
      .def(py::init<>())
      .def_readwrite("num_bins", &PyClass::num_bins)
      .def_readwrite("low_freq", &PyClass::low_freq)
      .def_readwrite("high_freq", &PyClass::high_freq)
      .def_readwrite("vtln_low", &PyClass::vtln_low)
      .def_readwrite("vtln_high", &PyClass::vtln_high)
      .def_readwrite("debug_mel", &PyClass::debug_mel)
      .def_readwrite("htk_mode", &PyClass::htk_mode)
      .def("__str__",
           [](const PyClass &self) -> std::string { return self.ToString(); });
}

void PybindMelComputations(py::module &m) { PybindMelBanksOptions(m); }

}  // namespace kaldifeat
