// kaldifeat/csrc/online-feature.h
//
// Copyright (c)  2022  Xiaomi Corporation (authors: Fangjun Kuang)

// This file is copied/modified from kaldi/src/feat/online-feature.h

#ifndef KALDIFEAT_CSRC_ONLINE_FEATURE_H_
#define KALDIFEAT_CSRC_ONLINE_FEATURE_H_

#include <deque>

#include "kaldifeat/csrc/online-feature-itf.h"

namespace kaldifeat {

/// This class serves as a storage for feature vectors with an option to limit
/// the memory usage by removing old elements. The deleted frames indices are
/// "remembered" so that regardless of the MAX_ITEMS setting, the user always
/// provides the indices as if no deletion was being performed.
/// This is useful when processing very long recordings which would otherwise
/// cause the memory to eventually blow up when the features are not being
/// removed.
class RecyclingVector {
 public:
  /// By default it does not remove any elements.
  explicit RecyclingVector(int32_t items_to_hold = -1);

  ~RecyclingVector() = default;
  RecyclingVector(const RecyclingVector &) = delete;
  RecyclingVector &operator=(const RecyclingVector &) = delete;

  torch::Tensor At(int32_t index) const;

  void PushBack(torch::Tensor item);

  /// This method returns the size as if no "recycling" had happened,
  /// i.e. equivalent to the number of times the PushBack method has been
  /// called.
  int32_t Size() const;

 private:
  std::deque<torch::Tensor> items_;
  int32_t items_to_hold_;
  int32_t first_available_index_;
};

}  // namespace kaldifeat

#endif  // KALDIFEAT_CSRC_ONLINE_FEATURE_H_
