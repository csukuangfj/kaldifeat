// kaldifeat/python/csrc/utils.h
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#ifndef KALDIFEAT_PYTHON_CSRC_UTILS_H_
#define KALDIFEAT_PYTHON_CSRC_UTILS_H_

#include "kaldifeat/csrc/feature-window.h"
#include "kaldifeat/python/csrc/kaldifeat.h"

namespace kaldifeat {

FrameExtractionOptions FrameExtractionOptionsFromDict(py::dict dict);
py::dict AsDict(const FrameExtractionOptions &opts);

}  // namespace kaldifeat

#endif  // KALDIFEAT_PYTHON_CSRC_UTILS_H_
