// kaldifeat/python/csrc/kaldifeat.cc
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifeat/python/csrc/kaldifeat.h"

#include "kaldifeat/csrc/feature-fbank.h"
#include "kaldifeat/python/csrc/feature-fbank.h"
#include "kaldifeat/python/csrc/feature-mfcc.h"
#include "kaldifeat/python/csrc/feature-plp.h"
#include "kaldifeat/python/csrc/feature-spectrogram.h"
#include "kaldifeat/python/csrc/feature-window.h"
#include "kaldifeat/python/csrc/mel-computations.h"
#include "torch/torch.h"

namespace kaldifeat {

PYBIND11_MODULE(_kaldifeat, m) {
  m.doc() = "Python wrapper for kaldifeat";

  PybindFeatureWindow(m);
  PybindMelComputations(m);
  PybindFeatureFbank(m);
  PybindFeatureMfcc(m);
  PybindFeaturePlp(m);
  PybindFeatureSpectrogram(m);
}

}  // namespace kaldifeat
