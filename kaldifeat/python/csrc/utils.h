// kaldifeat/python/csrc/utils.h
//
// Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

#ifndef KALDIFEAT_PYTHON_CSRC_UTILS_H_
#define KALDIFEAT_PYTHON_CSRC_UTILS_H_

#include "kaldifeat/csrc/feature-fbank.h"
#include "kaldifeat/csrc/feature-mfcc.h"
#include "kaldifeat/csrc/feature-plp.h"
#include "kaldifeat/csrc/feature-spectrogram.h"
#include "kaldifeat/csrc/feature-window.h"
#include "kaldifeat/csrc/mel-computations.h"
#include "kaldifeat/python/csrc/kaldifeat.h"

/*
 * This file contains code about `from_dict` and
 * `to_dict` for various options in kaldifeat.
 *
 * Regarding `from_dict`, users don't need to provide
 * all the fields in the options. If some fields
 * are not provided, it just uses the default one.
 *
 * If the provided dict in `from_dict` is empty,
 * all fields use their default values.
 */

namespace kaldifeat {

FrameExtractionOptions FrameExtractionOptionsFromDict(py::dict dict);
py::dict AsDict(const FrameExtractionOptions &opts);

MelBanksOptions MelBanksOptionsFromDict(py::dict dict);
py::dict AsDict(const MelBanksOptions &opts);

FbankOptions FbankOptionsFromDict(py::dict dict);
py::dict AsDict(const FbankOptions &opts);

MfccOptions MfccOptionsFromDict(py::dict dict);
py::dict AsDict(const MfccOptions &opts);

SpectrogramOptions SpectrogramOptionsFromDict(py::dict dict);
py::dict AsDict(const SpectrogramOptions &opts);

PlpOptions PlpOptionsFromDict(py::dict dict);
py::dict AsDict(const PlpOptions &opts);

}  // namespace kaldifeat

#endif  // KALDIFEAT_PYTHON_CSRC_UTILS_H_
