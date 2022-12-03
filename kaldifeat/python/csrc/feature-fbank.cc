// kaldifeat/python/csrc/feature-fbank.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifeat/python/csrc/feature-fbank.h"

#include <memory>
#include <string>

#include "kaldifeat/csrc/feature-fbank.h"
#include "kaldifeat/python/csrc/utils.h"

namespace kaldifeat {

static void PybindFbankOptions(py::module &m) {
  using PyClass = FbankOptions;
  py::class_<PyClass>(m, "FbankOptions")
      .def(py::init<>())
      .def(py::init([](const MelBanksOptions &mel_opts,
                       const FrameExtractionOptions &frame_opts =
                           FrameExtractionOptions(),
                       bool use_energy = false, float energy_floor = 0.0f,
                       bool raw_energy = true, bool htk_compat = false,
                       bool use_log_fbank = true, bool use_power = true,
                       py::object device =
                           py::str("cpu")) -> std::unique_ptr<FbankOptions> {
             auto opts = std::make_unique<FbankOptions>();
             opts->frame_opts = frame_opts;
             opts->mel_opts = mel_opts;
             opts->use_energy = use_energy;
             opts->energy_floor = energy_floor;
             opts->raw_energy = raw_energy;
             opts->htk_compat = htk_compat;
             opts->use_log_fbank = use_log_fbank;
             opts->use_power = use_power;

             std::string s = static_cast<py::str>(device);
             opts->device = torch::Device(s);

             return opts;
           }),
           py::arg("mel_opts"),
           py::arg("frame_opts") = FrameExtractionOptions(),
           py::arg("use_energy") = false, py::arg("energy_floor") = 0.0f,
           py::arg("raw_energy") = true, py::arg("htk_compat") = false,
           py::arg("use_log_fbank") = true, py::arg("use_power") = true,
           py::arg("device") = py::str("cpu"))
      .def_readwrite("frame_opts", &PyClass::frame_opts)
      .def_readwrite("mel_opts", &PyClass::mel_opts)
      .def_readwrite("use_energy", &PyClass::use_energy)
      .def_readwrite("energy_floor", &PyClass::energy_floor)
      .def_readwrite("raw_energy", &PyClass::raw_energy)
      .def_readwrite("htk_compat", &PyClass::htk_compat)
      .def_readwrite("use_log_fbank", &PyClass::use_log_fbank)
      .def_readwrite("use_power", &PyClass::use_power)
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
      .def_static(
          "from_dict",
          [](py::dict dict) -> PyClass { return FbankOptionsFromDict(dict); })
      .def(py::pickle(
          [](const PyClass &self) -> py::dict { return AsDict(self); },
          [](py::dict dict) -> PyClass { return FbankOptionsFromDict(dict); }));
}

static void PybindFbank(py::module &m) {
  using PyClass = Fbank;
  py::class_<PyClass>(m, "Fbank")
      .def(py::init<const FbankOptions &>(), py::arg("opts"))
      .def("dim", &PyClass::Dim)
      .def_property_readonly("options", &PyClass::GetOptions)
      .def("compute_features", &PyClass::ComputeFeatures, py::arg("wave"),
           py::arg("vtln_warp"), py::call_guard<py::gil_scoped_release>())
      .def(py::pickle(
          [](const PyClass &self) -> py::dict {
            return AsDict(self.GetOptions());
          },
          [](py::dict dict) -> std::unique_ptr<PyClass> {
            return std::make_unique<PyClass>(FbankOptionsFromDict(dict));
          }));
}

void PybindFeatureFbank(py::module &m) {
  PybindFbankOptions(m);
  PybindFbank(m);
}

}  // namespace kaldifeat
