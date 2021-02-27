// kaldifeat/python/csrc/feature-fbank.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifeat/python/csrc/feature-fbank.h"

#include "kaldifeat/csrc/feature-fbank.h"

namespace kaldifeat {

void PybindFbankOptions(py::module &m) {
  py::class_<FbankOptions>(m, "FbankOptions")
      .def(py::init<>())
      .def_readwrite("frame_opts", &FbankOptions::frame_opts)
      .def_readwrite("mel_opts", &FbankOptions::mel_opts)
      .def_readwrite("use_energy", &FbankOptions::use_energy)
      .def_readwrite("energy_floor", &FbankOptions::energy_floor)
      .def_readwrite("raw_energy", &FbankOptions::raw_energy)
      .def_readwrite("htk_compat", &FbankOptions::htk_compat)
      .def_readwrite("use_log_fbank", &FbankOptions::use_log_fbank)
      .def_readwrite("use_power", &FbankOptions::use_power)
      .def("set_device",
           [](FbankOptions *fbank_opts, py::object device) {
             std::string device_type =
                 static_cast<py::str>(device.attr("type"));
             KALDIFEAT_ASSERT(device_type == "cpu" || device_type == "cuda")
                 << "Unsupported device type: " << device_type;

             auto index_attr = static_cast<py::object>(device.attr("index"));
             int32_t device_index = 0;
             if (!index_attr.is_none())
               device_index = static_cast<py::int_>(index_attr);
             if (device_type == "cpu")
               fbank_opts->device = torch::Device("cpu");
             else
               fbank_opts->device = torch::Device(torch::kCUDA, device_index);
           })
      .def("__str__", [](const FbankOptions &self) -> std::string {
        return self.ToString();
      });

  py::class_<Fbank>(m, "Fbank")
      .def(py::init<const FbankOptions &>(), py::arg("opts"))
      .def("dim", &Fbank::Dim)
      .def("options", &Fbank::GetOptions,
           py::return_value_policy::reference_internal)
      .def("compute_features", &Fbank::ComputeFeatures, py::arg("wave"),
           py::arg("vtln_warp"));
}

}  // namespace kaldifeat
