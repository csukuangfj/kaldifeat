// kaldifeat/python/csrc/feature-window.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifeat/python/csrc/feature-window.h"

#include <string>

#include "kaldifeat/csrc/feature-window.h"
#include "kaldifeat/python/csrc/utils.h"

namespace kaldifeat {

static void PybindFrameExtractionOptions(py::module &m) {
  using PyClass = FrameExtractionOptions;
  py::class_<PyClass>(m, "FrameExtractionOptions")
      .def(py::init<>())
      .def_readwrite("samp_freq", &PyClass::samp_freq)
      .def_readwrite("frame_shift_ms", &PyClass::frame_shift_ms)
      .def_readwrite("frame_length_ms", &PyClass::frame_length_ms)
      .def_readwrite("dither", &PyClass::dither)
      .def_readwrite("preemph_coeff", &PyClass::preemph_coeff)
      .def_readwrite("remove_dc_offset", &PyClass::remove_dc_offset)
      .def_readwrite("window_type", &PyClass::window_type)
      .def_readwrite("round_to_power_of_two", &PyClass::round_to_power_of_two)
      .def_readwrite("blackman_coeff", &PyClass::blackman_coeff)
      .def_readwrite("snip_edges", &PyClass::snip_edges)
      .def_readwrite("max_feature_vectors", &PyClass::max_feature_vectors)
      .def("as_dict",
           [](const PyClass &self) -> py::dict { return AsDict(self); })
      .def_static("from_dict",
                  [](py::dict dict) -> PyClass {
                    return FrameExtractionOptionsFromDict(dict);
                  })
#if 0
      .def_readwrite("allow_downsample",
                     &PyClass::allow_downsample)
      .def_readwrite("allow_upsample", &PyClass::allow_upsample)
#endif
      .def("__str__",
           [](const PyClass &self) -> std::string { return self.ToString(); })
      .def(py::pickle(
          [](const PyClass &self) -> py::dict { return AsDict(self); },
          [](py::dict dict) -> PyClass {
            return FrameExtractionOptionsFromDict(dict);
          }));

  m.def("num_frames", &NumFrames, py::arg("num_samples"), py::arg("opts"),
        py::arg("flush") = true);

  m.def("get_strided", &GetStrided, py::arg("wave"), py::arg("opts"));
}

void PybindFeatureWindow(py::module &m) {
  PybindFrameExtractionOptions(m);

  m.def("num_frames", &NumFrames, py::arg("num_samples"), py::arg("opts"),
        py::arg("flush") = true);

  m.def("get_strided", &GetStrided, py::arg("wave"), py::arg("opts"));
}

}  // namespace kaldifeat
