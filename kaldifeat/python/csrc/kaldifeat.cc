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

  m.def("test", [](const torch::Tensor &tensor) -> torch::Tensor {
    FbankOptions fbank_opts;
    fbank_opts.frame_opts.dither = 0.0f;

    Fbank fbank(fbank_opts);
    float vtln_warp = 1.0f;

    std::chrono::steady_clock::time_point begin =
        std::chrono::steady_clock::now();

    torch::Tensor ans = fbank.ComputeFeatures(tensor, vtln_warp);
    std::chrono::steady_clock::time_point end =
        std::chrono::steady_clock::now();
    std::cout << "Time difference = "
              << std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                       begin)
                         .count() /
                     1000000.
              << "[s]" << std::endl;

    return ans;
  });
}

}  // namespace kaldifeat
