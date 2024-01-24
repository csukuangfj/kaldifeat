// kaldifeat/python/csrc/pitch-functions.cc
//
// Copyright (c)  2024             (authors: Feiteng Li)

#include "kaldifeat/python/csrc/pitch-functions.h"

#include <memory>
#include <string>

#include "kaldifeat/csrc/pitch-functions.h"
#include "kaldifeat/python/csrc/utils.h"

namespace kaldifeat {

static void PybindPitchOptions(py::module &m) {
  using PyClass = PitchExtractionOptions;
  py::class_<PyClass>(m, "PitchOptions")
      .def(py::init<>())
      .def(py::init([](float samp_freq = 16000,
                       float frame_shift_ms = 10.0, float frame_length_ms = 25.0, float preemph_coeff = 0.0,
                       float min_f0 = 50, float max_f0 = 400, float soft_min_f0 = 10.0,
                       float penalty_factor = 0.1, float lowpass_cutoff = 1000, float resample_freq = 4000,
                       float delta_pitch = 0.005, float nccf_ballast = 7000,
                       int32_t lowpass_filter_width = 1, int32_t upsample_filter_width = 5,
                       int32_t max_frames_latency = 0, int32_t frames_per_chunk = 0,
                       bool simulate_first_pass_online = false, int32_t recompute_frame = 500,
                       bool nccf_ballast_online = false, bool snip_edges = true,
                       py::object device =
                           py::str("cpu")) -> std::unique_ptr<PitchExtractionOptions> {
             auto opts = std::make_unique<PitchExtractionOptions>();
             opts->samp_freq = samp_freq;
              opts->frame_shift_ms = frame_shift_ms;
              opts->frame_length_ms = frame_length_ms;
              opts->preemph_coeff = preemph_coeff;
              opts->min_f0 = min_f0;
              opts->max_f0 = max_f0;
              opts->soft_min_f0 = soft_min_f0;
              opts->penalty_factor = penalty_factor;
              opts->lowpass_cutoff = lowpass_cutoff;
              opts->resample_freq = resample_freq;
              opts->delta_pitch = delta_pitch;
              opts->nccf_ballast = nccf_ballast;
              opts->lowpass_filter_width = lowpass_filter_width;
              opts->upsample_filter_width = upsample_filter_width;
              opts->max_frames_latency = max_frames_latency;
              opts->frames_per_chunk = frames_per_chunk;
              opts->simulate_first_pass_online = simulate_first_pass_online;
              opts->recompute_frame = recompute_frame;
              opts->nccf_ballast_online = nccf_ballast_online;
              opts->snip_edges = snip_edges;

             std::string s = static_cast<py::str>(device);
             opts->device = torch::Device(s);

             return opts;
           }),
          py::arg("samp_freq") = 16000, py::arg("frame_shift_ms") = 10.0,
          py::arg("frame_length_ms") = 25.0, py::arg("preemph_coeff") = 0.0,
          py::arg("min_f0") = 50, py::arg("max_f0") = 400, py::arg("soft_min_f0") = 10.0,
          py::arg("penalty_factor") = 0.1, py::arg("lowpass_cutoff") = 1000, py::arg("resample_freq") = 4000,
          py::arg("delta_pitch") = 0.005, py::arg("nccf_ballast") = 7000,
          py::arg("lowpass_filter_width") = 1, py::arg("upsample_filter_width") = 5,
          py::arg("max_frames_latency") = 0, py::arg("frames_per_chunk") = 0,
          py::arg("simulate_first_pass_online") = false, py::arg("recompute_frame") = 500,
          py::arg("nccf_ballast_online") = false, py::arg("snip_edges") = true,
          py::arg("device") = py::str("cpu"))

      .def_readwrite("samp_freq", &PyClass::samp_freq)
      .def_readwrite("frame_shift_ms", &PyClass::frame_shift_ms)
      .def_readwrite("frame_length_ms", &PyClass::frame_length_ms)
      .def_readwrite("preemph_coeff", &PyClass::preemph_coeff)
      .def_readwrite("min_f0", &PyClass::min_f0)
      .def_readwrite("max_f0", &PyClass::max_f0)
      .def_readwrite("soft_min_f0", &PyClass::soft_min_f0)
      .def_readwrite("penalty_factor", &PyClass::penalty_factor)
      .def_readwrite("lowpass_cutoff", &PyClass::lowpass_cutoff)
      .def_readwrite("resample_freq", &PyClass::resample_freq)
      .def_readwrite("delta_pitch", &PyClass::delta_pitch)
      .def_readwrite("nccf_ballast", &PyClass::nccf_ballast)
      .def_readwrite("lowpass_filter_width", &PyClass::lowpass_filter_width)
      .def_readwrite("upsample_filter_width", &PyClass::upsample_filter_width)
      .def_readwrite("max_frames_latency", &PyClass::max_frames_latency)
      .def_readwrite("frames_per_chunk", &PyClass::frames_per_chunk)
      .def_readwrite("simulate_first_pass_online",
                     &PyClass::simulate_first_pass_online)
      .def_readwrite("recompute_frame", &PyClass::recompute_frame)
      .def_readwrite("nccf_ballast_online", &PyClass::nccf_ballast_online)
      .def_readwrite("snip_edges", &PyClass::snip_edges)
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
          [](py::dict dict) -> PyClass { return PitchOptionsFromDict(dict); })
      .def(py::pickle(
          [](const PyClass &self) -> py::dict { return AsDict(self); },
          [](py::dict dict) -> PyClass { return PitchOptionsFromDict(dict); }));
}

static void PybindPitch(py::module &m) {
  using PyClass = Pitch;
  py::class_<PyClass>(m, "Pitch")
      .def(py::init<const PitchOptions &>(), py::arg("opts"))
      .def("dim", &PyClass::Dim)
      .def_property_readonly("options", &PyClass::GetOptions)
      .def("compute_features", &PyClass::ComputeFeatures, py::arg("wave"),
           py::arg("vtln_warp"), py::call_guard<py::gil_scoped_release>())
      .def(py::pickle(
          [](const PyClass &self) -> py::dict {
            return AsDict(self.GetOptions());
          },
          [](py::dict dict) -> std::unique_ptr<PyClass> {
            return std::make_unique<PyClass>(PitchOptionsFromDict(dict));
          }));
}

void PybindPitchFunctions(py::module &m) {
  PybindPitchOptions(m);
  PybindPitch(m);
}

}  // namespace kaldifeat
