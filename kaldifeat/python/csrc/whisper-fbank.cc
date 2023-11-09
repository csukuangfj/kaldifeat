// kaldifeat/python/csrc/whisper-fbank.cc
//
// Copyright (c)  2023  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifeat/python/csrc/whisper-fbank.h"

#include <memory>
#include <string>

#include "kaldifeat/csrc/whisper-fbank.h"
#include "kaldifeat/python/csrc/utils.h"

namespace kaldifeat {

static void PybindWhisperFbankOptions(py::module *m) {
  using PyClass = WhisperFbankOptions;
  py::class_<PyClass>(*m, "WhisperFbankOptions")
      .def(py::init<>())
      .def(py::init([](const FrameExtractionOptions &frame_opts =
                           FrameExtractionOptions(),
                       int32_t num_mels = 80,
                       py::object device = py::str(
                           "cpu")) -> std::unique_ptr<WhisperFbankOptions> {
             auto opts = std::make_unique<WhisperFbankOptions>();
             opts->frame_opts = frame_opts;
             opts->num_mels = num_mels;

             std::string s = static_cast<py::str>(device);
             opts->device = torch::Device(s);

             return opts;
           }),
           py::arg("frame_opts") = FrameExtractionOptions(),
           py::arg("num_mels") = 80, py::arg("device") = py::str("cpu"))
      .def_readwrite("frame_opts", &PyClass::frame_opts)
      .def_readwrite("num_mels", &PyClass::num_mels)
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
                    return WhisperFbankOptionsFromDict(dict);
                  })
      .def(py::pickle(
          [](const PyClass &self) -> py::dict { return AsDict(self); },
          [](py::dict dict) -> PyClass {
            return WhisperFbankOptionsFromDict(dict);
          }));
}

static void PybindWhisperFbankImpl(py::module *m) {
  using PyClass = WhisperFbank;
  py::class_<PyClass>(*m, "WhisperFbank")
      .def(py::init<const WhisperFbankOptions &>(), py::arg("opts"))
      .def("dim", &PyClass::Dim)
      .def_property_readonly("options", &PyClass::GetOptions)
      .def("compute_features", &PyClass::ComputeFeatures, py::arg("wave"),
           py::arg("vtln_warp"), py::call_guard<py::gil_scoped_release>())
      .def(py::pickle(
          [](const PyClass &self) -> py::dict {
            return AsDict(self.GetOptions());
          },
          [](py::dict dict) -> std::unique_ptr<PyClass> {
            return std::make_unique<PyClass>(WhisperFbankOptionsFromDict(dict));
          }));
}

void PybindWhisperFbank(py::module *m) {
  PybindWhisperFbankOptions(m);
  PybindWhisperFbankImpl(m);
}

}  // namespace kaldifeat
