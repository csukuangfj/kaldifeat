// kaldifeat/python/csrc/feature-plp.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifeat/python/csrc/feature-plp.h"

#include <string>

#include "kaldifeat/csrc/feature-plp.h"
#include "kaldifeat/python/csrc/utils.h"

namespace kaldifeat {

void PybindPlpOptions(py::module &m) {
  using PyClass = PlpOptions;
  py::class_<PyClass>(m, "PlpOptions")
      .def(py::init<>())
      .def_readwrite("frame_opts", &PyClass::frame_opts)
      .def_readwrite("mel_opts", &PyClass::mel_opts)
      .def_readwrite("lpc_order", &PyClass::lpc_order)
      .def_readwrite("num_ceps", &PyClass::num_ceps)
      .def_readwrite("use_energy", &PyClass::use_energy)
      .def_readwrite("energy_floor", &PyClass::energy_floor)
      .def_readwrite("raw_energy", &PyClass::raw_energy)
      .def_readwrite("compress_factor", &PyClass::compress_factor)
      .def_readwrite("cepstral_lifter", &PyClass::cepstral_lifter)
      .def_readwrite("cepstral_scale", &PyClass::cepstral_scale)
      .def_readwrite("htk_compat", &PyClass::htk_compat)
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
        return PlpOptionsFromDict(dict);
      });
}

static void PybindPlp(py::module &m) {
  using PyClass = Plp;
  py::class_<PyClass>(m, "Plp")
      .def(py::init<const PlpOptions &>(), py::arg("opts"))
      .def("dim", &PyClass::Dim)
      .def_property_readonly("options", &PyClass::GetOptions)
      .def("compute_features", &PyClass::ComputeFeatures, py::arg("wave"),
           py::arg("vtln_warp"));
}

void PybindFeaturePlp(py::module &m) {
  PybindPlpOptions(m);
  PybindPlp(m);
}

}  // namespace kaldifeat
