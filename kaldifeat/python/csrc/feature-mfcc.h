// kaldifeat/python/csrc/feature-mfcc.h
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#ifndef KALDIFEAT_PYTHON_CSRC_FEATURE_MFCC_H_
#define KALDIFEAT_PYTHON_CSRC_FEATURE_MFCC_H_

#include "kaldifeat/python/csrc/kaldifeat.h"

namespace kaldifeat {

void PybindFeatureMfcc(py::module &m);

}  // namespace kaldifeat

#endif  // KALDIFEAT_PYTHON_CSRC_FEATURE_MFCC_H_
