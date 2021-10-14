// kaldifeat/python/csrc/feature-window.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifeat/python/csrc/feature-window.h"

#include <string>

#include "kaldifeat/csrc/feature-window.h"
#include "kaldifeat/python/csrc/utils.h"

namespace kaldifeat {

static void PybindFrameExtractionOptions(py::module &m) {
  py::class_<FrameExtractionOptions>(m, "FrameExtractionOptions")
      .def(py::init<>())
      .def_readwrite("samp_freq", &FrameExtractionOptions::samp_freq)
      .def_readwrite("frame_shift_ms", &FrameExtractionOptions::frame_shift_ms)
      .def_readwrite("frame_length_ms",
                     &FrameExtractionOptions::frame_length_ms)
      .def_readwrite("dither", &FrameExtractionOptions::dither)
      .def_readwrite("preemph_coeff", &FrameExtractionOptions::preemph_coeff)
      .def_readwrite("remove_dc_offset",
                     &FrameExtractionOptions::remove_dc_offset)
      .def_readwrite("window_type", &FrameExtractionOptions::window_type)
      .def_readwrite("round_to_power_of_two",
                     &FrameExtractionOptions::round_to_power_of_two)
      .def_readwrite("blackman_coeff", &FrameExtractionOptions::blackman_coeff)
      .def_readwrite("snip_edges", &FrameExtractionOptions::snip_edges)
      .def("as_dict",
           [](const FrameExtractionOptions &self) -> py::dict {
             return AsDict(self);
           })
      .def_static("from_dict",
                  [](py::dict dict) -> FrameExtractionOptions {
                    return FrameExtractionOptionsFromDict(dict);
                  })
#if 0
      .def_readwrite("allow_downsample",
                     &FrameExtractionOptions::allow_downsample)
      .def_readwrite("allow_upsample", &FrameExtractionOptions::allow_upsample)
      .def_readwrite("max_feature_vectors",
                     &FrameExtractionOptions::max_feature_vectors)
#endif
      .def("__str__", [](const FrameExtractionOptions &self) -> std::string {
        return self.ToString();
      });

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
