// kaldifeat/python/csrc/whisper-fbank.h
//
// Copyright (c)  2023  Xiaomi Corporation (authors: Fangjun Kuang)

#ifndef KALDIFEAT_PYTHON_CSRC_WHISPER_FBANK_H_
#define KALDIFEAT_PYTHON_CSRC_WHISPER_FBANK_H_

#include "kaldifeat/python/csrc/kaldifeat.h"

namespace kaldifeat {

void PybindWhisperFbank(py::module &m);

}  // namespace kaldifeat

#endif  // KALDIFEAT_PYTHON_CSRC_WHISPER_FBANK_H_
