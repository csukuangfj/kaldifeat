// kaldifeat/python/csrc/feature-fbank.h
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#ifndef KALDIFEAT_PYTHON_CSRC_FEATURE_FBANK_H_
#define KALDIFEAT_PYTHON_CSRC_FEATURE_FBANK_H_

#include "kaldifeat/python/csrc/kaldifeat.h"

namespace kaldifeat {

void PybindFeatureFbank(py::module &m);

}  // namespace kaldifeat

#endif  // KALDIFEAT_PYTHON_CSRC_FEATURE_FBANK_H_
