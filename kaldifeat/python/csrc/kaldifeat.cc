// kaldifeat/python/csrc/kaldifeat.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifeat/python/csrc/kaldifeat.h"

#include <chrono>

#include "kaldifeat/csrc/feature-fbank.h"
#include "kaldifeat/python/csrc/feature-fbank.h"
#include "kaldifeat/python/csrc/feature-window.h"
#include "kaldifeat/python/csrc/mel-computations.h"
#include "torch/torch.h"

namespace kaldifeat {

static torch::Tensor Compute(const torch::Tensor &wave,
                             const FbankOptions &fbank_opts) {
  // TODO(fangjun): wrap Fbank to Python

  Fbank fbank(fbank_opts);
  float vtln_warp = 1.0f;

  torch::Tensor ans = fbank.ComputeFeatures(wave, vtln_warp);
  return ans;
}

PYBIND11_MODULE(_kaldifeat, m) {
  m.doc() = "Python wrapper for kaldifeat";

  PybindFrameExtractionOptions(m);
  PybindMelBanksOptions(m);
  PybindFbankOptions(m);

  m.def("compute", &Compute, py::arg("wave"), py::arg("fbank_opts"));

  // It verifies that the reimplementation produces the same output
  // as kaldi using default parameters with dither disabled.
  m.def(
      "_compute_with_elapsed_time",  // for benchmark only
      [](const torch::Tensor &wave,
         const FbankOptions &fbank_opts) -> std::pair<torch::Tensor, double> {
        std::chrono::steady_clock::time_point begin =
            std::chrono::steady_clock::now();

        torch::Tensor ans = Compute(wave, fbank_opts);

        std::chrono::steady_clock::time_point end =
            std::chrono::steady_clock::now();

        double elapsed_seconds =
            std::chrono::duration_cast<std::chrono::microseconds>(end - begin)
                .count() /
            1000000.;

        return std::make_pair(ans, elapsed_seconds);
      },
      py::arg("wave"), py::arg("fbank_opts"));
}

}  // namespace kaldifeat
