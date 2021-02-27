// kaldifeat/python/csrc/kaldifeat.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifeat/python/csrc/kaldifeat.h"

#include <chrono>

#include "kaldifeat/csrc/feature-fbank.h"
#include "torch/torch.h"

namespace kaldifeat {

PYBIND11_MODULE(_kaldifeat, m) {
  m.doc() = "Python wrapper for kaldifeat";

  // It verifies that the reimplementation produces the same output
  // as kaldi using default paremters with dither disabled.
  m.def("test_default_parameters",
        [](const torch::Tensor &tensor) -> std::pair<torch::Tensor, double> {
          FbankOptions fbank_opts;
          fbank_opts.frame_opts.dither = 0.0f;

          Fbank fbank(fbank_opts);
          float vtln_warp = 1.0f;

          std::chrono::steady_clock::time_point begin =
              std::chrono::steady_clock::now();

          torch::Tensor ans = fbank.ComputeFeatures(tensor, vtln_warp);
          std::chrono::steady_clock::time_point end =
              std::chrono::steady_clock::now();
          double elapsed_seconds =
              std::chrono::duration_cast<std::chrono::microseconds>(end - begin)
                  .count() /
              1000000.;

          return std::make_pair(ans, elapsed_seconds);
        });
}

}  // namespace kaldifeat
