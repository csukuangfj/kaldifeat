// kaldifeat/python/csrc/feature-window.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifeat/python/csrc/feature-window.h"

#include <memory>
#include <string>

#include "kaldifeat/csrc/feature-window.h"
#include "kaldifeat/python/csrc/utils.h"

namespace kaldifeat {

static void PybindFrameExtractionOptions(py::module &m) {
  using PyClass = FrameExtractionOptions;
  py::class_<PyClass>(m, "FrameExtractionOptions")
      .def(
          py::init([](float samp_freq = 16000, float frame_shift_ms = 10.0f,
                      float frame_length_ms = 25.0f, float dither = 1.0f,
                      float preemph_coeff = 0.97f, bool remove_dc_offset = true,
                      const std::string &window_type = "povey",
                      bool round_to_power_of_two = true,
                      float blackman_coeff = 0.42f, bool snip_edges = true,
                      int32_t max_feature_vectors =
                          -1) -> std::unique_ptr<FrameExtractionOptions> {
            auto opts = std::make_unique<FrameExtractionOptions>();

            opts->samp_freq = samp_freq;
            opts->frame_shift_ms = frame_shift_ms;
            opts->frame_length_ms = frame_length_ms;
            opts->dither = dither;
            opts->preemph_coeff = preemph_coeff;
            opts->remove_dc_offset = remove_dc_offset;
            opts->window_type = window_type;
            opts->round_to_power_of_two = round_to_power_of_two;
            opts->blackman_coeff = blackman_coeff;
            opts->snip_edges = snip_edges;
            opts->max_feature_vectors = max_feature_vectors;

            return opts;
          }),
          py::arg("samp_freq") = 16000, py::arg("frame_shift_ms") = 10.0f,
          py::arg("frame_length_ms") = 25.0f, py::arg("dither") = 1.0f,
          py::arg("preemph_coeff") = 0.97f, py::arg("remove_dc_offset") = true,
          py::arg("window_type") = "povey",
          py::arg("round_to_power_of_two") = true,
          py::arg("blackman_coeff") = 0.42f, py::arg("snip_edges") = true,
          py::arg("max_feature_vectors") = -1)
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
