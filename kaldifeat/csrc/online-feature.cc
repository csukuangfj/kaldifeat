// kaldifeat/csrc/online-feature.cc
//
// Copyright (c)  2022  Xiaomi Corporation (authors: Fangjun Kuang)

// This file is copied/modified from kaldi/src/feat/online-feature.cc

#include "kaldifeat/csrc/online-feature.h"

#include "kaldifeat/csrc/log.h"

namespace kaldifeat {

RecyclingVector::RecyclingVector(int32_t items_to_hold)
    : items_to_hold_(items_to_hold == 0 ? -1 : items_to_hold),
      first_available_index_(0) {}

torch::Tensor RecyclingVector::At(int32_t index) const {
  if (index < first_available_index_) {
    KALDIFEAT_ERR << "Attempted to retrieve feature vector that was "
                     "already removed by the RecyclingVector (index = "
                  << index << "; "
                  << "first_available_index = " << first_available_index_
                  << "; "
                  << "size = " << Size() << ")";
  }
  // 'at' does size checking.
  return items_.at(index - first_available_index_);
}

void RecyclingVector::PushBack(torch::Tensor item) {
  // Note: -1 is a larger number when treated as unsigned
  if (items_.size() == static_cast<size_t>(items_to_hold_)) {
    items_.pop_front();
    ++first_available_index_;
  }
  items_.push_back(item);
}

int32_t RecyclingVector::Size() const {
  return first_available_index_ + static_cast<int32_t>(items_.size());
}

}  // namespace kaldifeat
