// kaldifeat/python/csrc/feature-fbank.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifeat/python/csrc/feature-fbank.h"

#include "kaldifeat/csrc/feature-fbank.h"

namespace kaldifeat {

void PybindFbankOptions(py::module &m) {
  py::class_<FbankOptions>(m, "FbankOptions")
      .def(py::init<>())
      .def_readwrite("frame_opts", &FbankOptions::frame_opts)
      .def_readwrite("mel_opts", &FbankOptions::mel_opts)
      .def_readwrite("use_energy", &FbankOptions::use_energy)
      .def_readwrite("energy_floor", &FbankOptions::energy_floor)
      .def_readwrite("raw_energy", &FbankOptions::raw_energy)
      .def_readwrite("htk_compat", &FbankOptions::htk_compat)
      .def_readwrite("use_log_fbank", &FbankOptions::use_log_fbank)
      .def_readwrite("use_power", &FbankOptions::use_power)
      .def("__str__", [](const FbankOptions &self) -> std::string {
        return self.ToString();
      });
}

}  // namespace kaldifeat
