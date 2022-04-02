// kaldifeat/csrc/online-feature.h
//
// Copyright (c)  2022  Xiaomi Corporation (authors: Fangjun Kuang)

// This file is copied/modified from kaldi/src/feat/online-feature.h

#ifndef KALDIFEAT_CSRC_ONLINE_FEATURE_H_
#define KALDIFEAT_CSRC_ONLINE_FEATURE_H_

#include <deque>

#include "kaldifeat/csrc/feature-fbank.h"
#include "kaldifeat/csrc/feature-mfcc.h"
#include "kaldifeat/csrc/feature-plp.h"
#include "kaldifeat/csrc/feature-window.h"
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

/// This is a templated class for online feature extraction;
/// it's templated on a class like MfccComputer or PlpComputer
/// that does the basic feature extraction.
template <class C>
class OnlineGenericBaseFeature : public OnlineFeatureInterface {
 public:
  // Constructor from options class
  explicit OnlineGenericBaseFeature(const typename C::Options &opts);

  int32_t Dim() const override { return computer_.Dim(); }

  float FrameShiftInSeconds() const override {
    return computer_.GetFrameOptions().frame_shift_ms / 1000.0f;
  }

  int32_t NumFramesReady() const override { return features_.Size(); }

  // Note: IsLastFrame() will only ever return true if you have called
  // InputFinished() (and this frame is the last frame).
  bool IsLastFrame(int32_t frame) const override {
    return input_finished_ && frame == NumFramesReady() - 1;
  }

  torch::Tensor GetFrame(int32_t frame) override { return features_.At(frame); }

  // This would be called from the application, when you get
  // more wave data.  Note: the sampling_rate is only provided so
  // the code can assert that it matches the sampling rate
  // expected in the options.
  void AcceptWaveform(float sampling_rate,
                      const torch::Tensor &waveform) override;

  // InputFinished() tells the class you won't be providing any
  // more waveform.  This will help flush out the last frame or two
  // of features, in the case where snip-edges == false; it also
  // affects the return value of IsLastFrame().
  void InputFinished() override;

 private:
  // This function computes any additional feature frames that it is possible to
  // compute from 'waveform_remainder_', which at this point may contain more
  // than just a remainder-sized quantity (because AcceptWaveform() appends to
  // waveform_remainder_ before calling this function).  It adds these feature
  // frames to features_, and shifts off any now-unneeded samples of input from
  // waveform_remainder_ while incrementing waveform_offset_ by the same amount.
  void ComputeFeatures();

  C computer_;  // class that does the MFCC or PLP or filterbank computation

  FeatureWindowFunction window_function_;

  // features_ is the Mfcc or Plp or Fbank features that we have already
  // computed.

  RecyclingVector features_;

  // True if the user has called "InputFinished()"
  bool input_finished_;

  // waveform_offset_ is the number of samples of waveform that we have
  // already discarded, i.e. that were prior to 'waveform_remainder_'.
  int64_t waveform_offset_;

  // waveform_remainder_ is a short piece of waveform that we may need to keep
  // after extracting all the whole frames we can (whatever length of feature
  // will be required for the next phase of computation).
  // It is a 1-D tensor
  torch::Tensor waveform_remainder_;
};

using OnlineMfcc = OnlineGenericBaseFeature<Mfcc>;
using OnlinePlp = OnlineGenericBaseFeature<Plp>;
using OnlineFbank = OnlineGenericBaseFeature<Fbank>;

}  // namespace kaldifeat

#endif  // KALDIFEAT_CSRC_ONLINE_FEATURE_H_
