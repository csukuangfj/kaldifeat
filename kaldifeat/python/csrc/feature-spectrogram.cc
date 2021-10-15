// kaldifeat/python/csrc/feature-spectrogram.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifeat/python/csrc/feature-spectrogram.h"

#include <string>

#include "kaldifeat/csrc/feature-spectrogram.h"
#include "kaldifeat/python/csrc/utils.h"

namespace kaldifeat {

static void PybindSpectrogramOptions(py::module &m) {
  using PyClass = SpectrogramOptions;
  py::class_<PyClass>(m, "SpectrogramOptions")
      .def(py::init<>())
      .def_readwrite("frame_opts", &PyClass::frame_opts)
      .def_readwrite("energy_floor", &PyClass::energy_floor)
      .def_readwrite("raw_energy", &PyClass::raw_energy)
      // .def_readwrite("return_raw_fft", &PyClass::return_raw_fft) // not
      // implemented yet
      .def_property(
          "device",
          [](const PyClass &self) -> py::object {
            py::object ans = py::module_::import("torch").attr("device");
            return ans(self.device.str());
          },
          [](PyClass &self, py::object obj) -> void {
            std::string s = static_cast<py::str>(obj);
            self.device = torch::Device(s);
          })
      .def("__str__",
           [](const PyClass &self) -> std::string { return self.ToString(); })
      .def("as_dict",
           [](const PyClass &self) -> py::dict { return AsDict(self); })
      .def_static("from_dict", [](py::dict dict) -> PyClass {
        return SpectrogramOptionsFromDict(dict);
      });
}

static void PybindSpectrogram(py::module &m) {
  using PyClass = Spectrogram;
  py::class_<PyClass>(m, "Spectrogram")
      .def(py::init<const SpectrogramOptions &>(), py::arg("opts"))
      .def("dim", &PyClass::Dim)
      .def_property_readonly("options", &PyClass::GetOptions)
      .def("compute_features", &PyClass::ComputeFeatures, py::arg("wave"),
           py::arg("vtln_warp"));
}

void PybindFeatureSpectrogram(py::module &m) {
  PybindSpectrogramOptions(m);
  PybindSpectrogram(m);
}

}  // namespace kaldifeat
