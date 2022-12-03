// kaldifeat/python/csrc/feature-spectrogram.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifeat/python/csrc/feature-spectrogram.h"

#include <memory>
#include <string>

#include "kaldifeat/csrc/feature-spectrogram.h"
#include "kaldifeat/python/csrc/utils.h"

namespace kaldifeat {

static void PybindSpectrogramOptions(py::module &m) {
  using PyClass = SpectrogramOptions;
  py::class_<PyClass>(m, "SpectrogramOptions")
      .def(py::init([](const FrameExtractionOptions &frame_opts =
                           FrameExtractionOptions(),
                       float energy_floor = 0.0, bool raw_energy = true,
                       bool return_raw_fft = false,
                       py::object device = py::str(
                           "cpu")) -> std::unique_ptr<SpectrogramOptions> {
             auto opts = std::make_unique<SpectrogramOptions>();
             opts->frame_opts = frame_opts;
             opts->energy_floor = energy_floor;
             opts->raw_energy = raw_energy;
             opts->return_raw_fft = return_raw_fft;

             std::string s = static_cast<py::str>(device);
             opts->device = torch::Device(s);

             return opts;
           }),
           py::arg("frame_opts") = FrameExtractionOptions(),
           py::arg("energy_floor") = 0.0, py::arg("raw_energy") = true,
           py::arg("return_raw_fft") = false,
           py::arg("device") = py::str("cpu"))
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
      .def_static("from_dict",
                  [](py::dict dict) -> PyClass {
                    return SpectrogramOptionsFromDict(dict);
                  })
      .def(py::pickle(
          [](const PyClass &self) -> py::dict { return AsDict(self); },
          [](py::dict dict) -> PyClass {
            return SpectrogramOptionsFromDict(dict);
          }));
}

static void PybindSpectrogram(py::module &m) {
  using PyClass = Spectrogram;
  py::class_<PyClass>(m, "Spectrogram")
      .def(py::init<const SpectrogramOptions &>(), py::arg("opts"))
      .def("dim", &PyClass::Dim)
      .def_property_readonly("options", &PyClass::GetOptions)
      .def("compute_features", &PyClass::ComputeFeatures, py::arg("wave"),
           py::arg("vtln_warp"), py::call_guard<py::gil_scoped_release>())
      .def(py::pickle(
          [](const PyClass &self) -> py::dict {
            return AsDict(self.GetOptions());
          },
          [](py::dict dict) -> std::unique_ptr<PyClass> {
            return std::make_unique<PyClass>(SpectrogramOptionsFromDict(dict));
          }));
}

void PybindFeatureSpectrogram(py::module &m) {
  PybindSpectrogramOptions(m);
  PybindSpectrogram(m);
}

}  // namespace kaldifeat
