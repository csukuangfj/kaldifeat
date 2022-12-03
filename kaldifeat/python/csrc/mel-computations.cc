// kaldifeat/python/csrc/mel-computations.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifeat/python/csrc/mel-computations.h"

#include <memory>
#include <string>

#include "kaldifeat/csrc/mel-computations.h"
#include "kaldifeat/python/csrc/utils.h"

namespace kaldifeat {

static void PybindMelBanksOptions(py::module &m) {
  using PyClass = MelBanksOptions;
  py::class_<PyClass>(m, "MelBanksOptions")
      .def(py::init(
               [](int32_t num_bins = 25, float low_freq = 20,
                  float high_freq = 0, float vtln_low = 100,
                  float vtln_high = -500,
                  bool debug_mel = false) -> std::unique_ptr<MelBanksOptions> {
                 auto opts = std::make_unique<MelBanksOptions>();

                 opts->num_bins = num_bins;
                 opts->low_freq = low_freq;
                 opts->high_freq = high_freq;
                 opts->vtln_low = vtln_low;
                 opts->vtln_high = vtln_high;

                 return opts;
               }),
           py::arg("num_bins") = 25, py::arg("low_freq") = 20,
           py::arg("high_freq") = 0, py::arg("vtln_low") = 100,
           py::arg("vtln_high") = -500, py::arg("debug_mel") = false)
      .def_readwrite("num_bins", &PyClass::num_bins)
      .def_readwrite("low_freq", &PyClass::low_freq)
      .def_readwrite("high_freq", &PyClass::high_freq)
      .def_readwrite("vtln_low", &PyClass::vtln_low)
      .def_readwrite("vtln_high", &PyClass::vtln_high)
      .def_readwrite("debug_mel", &PyClass::debug_mel)
      .def_readwrite("htk_mode", &PyClass::htk_mode)
      .def("__str__",
           [](const PyClass &self) -> std::string { return self.ToString(); })
      .def("as_dict",
           [](const PyClass &self) -> py::dict { return AsDict(self); })
      .def_static("from_dict",
                  [](py::dict dict) -> PyClass {
                    return MelBanksOptionsFromDict(dict);
                  })
      .def(py::pickle(
          [](const PyClass &self) -> py::dict { return AsDict(self); },
          [](py::dict dict) -> PyClass {
            return MelBanksOptionsFromDict(dict);
          }));
}

void PybindMelComputations(py::module &m) { PybindMelBanksOptions(m); }

}  // namespace kaldifeat
