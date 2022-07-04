// kaldifeat/python/csrc/online-feature.cc
//
// Copyright (c)  2022  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifeat/python/csrc/online-feature.h"

#include <string>

#include "kaldifeat/csrc/online-feature.h"
namespace kaldifeat {

template <typename C>
void PybindOnlineFeatureTpl(py::module &m, const std::string &class_name,
                            const std::string &class_help_doc = "") {
  using PyClass = OnlineGenericBaseFeature<C>;
  using Options = typename C::Options;
  py::class_<PyClass>(m, class_name.c_str(), class_help_doc.c_str())
      .def(py::init<const Options &>(), py::arg("opts"))
      .def_property_readonly("dim", &PyClass::Dim)
      .def_property_readonly("frame_shift_in_seconds",
                             &PyClass::FrameShiftInSeconds)
      .def_property_readonly("num_frames_ready", &PyClass::NumFramesReady)
      .def("is_last_frame", &PyClass::IsLastFrame, py::arg("frame"))
      .def("get_frame", &PyClass::GetFrame, py::arg("frame"))
      .def("get_frames", &PyClass::GetFrames, py::arg("frames"),
           py::call_guard<py::gil_scoped_release>())
      .def("accept_waveform", &PyClass::AcceptWaveform,
           py::arg("sampling_rate"), py::arg("waveform"),
           py::call_guard<py::gil_scoped_release>())
      .def("input_finished", &PyClass::InputFinished);
}

void PybindOnlineFeature(py::module &m) {
  PybindOnlineFeatureTpl<Mfcc>(m, "OnlineMfcc");
  PybindOnlineFeatureTpl<Fbank>(m, "OnlineFbank");
  PybindOnlineFeatureTpl<Plp>(m, "OnlinePlp");
}

}  // namespace kaldifeat
