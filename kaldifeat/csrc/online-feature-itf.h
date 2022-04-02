// kaldifeat/csrc/online-feature-itf.h
//
// Copyright (c)  2022  Xiaomi Corporation (authors: Fangjun Kuang)

// This file is copied/modified from kaldi/src/itf/online-feature-itf.h

#ifndef KALDIFEAT_CSRC_ONLINE_FEATURE_ITF_H_
#define KALDIFEAT_CSRC_ONLINE_FEATURE_ITF_H_

#include <utility>
#include <vector>

#include "torch/script.h"

namespace kaldifeat {

class OnlineFeatureInterface {
 public:
  virtual ~OnlineFeatureInterface() = default;

  virtual int32_t Dim() const = 0;  /// returns the feature dimension.
                                    //
  // Returns frame shift in seconds.  Helps to estimate duration from frame
  // counts.
  virtual float FrameShiftInSeconds() const = 0;

  /// Returns the total number of frames, since the start of the utterance, that
  /// are now available.  In an online-decoding context, this will likely
  /// increase with time as more data becomes available.
  virtual int32_t NumFramesReady() const = 0;

  /// Returns true if this is the last frame.  Frame indices are zero-based, so
  /// the first frame is zero.  IsLastFrame(-1) will return false, unless the
  /// file is empty (which is a case that I'm not sure all the code will handle,
  /// so be careful).  This function may return false for some frame if we
  /// haven't yet decided to terminate decoding, but later true if we decide to
  /// terminate decoding.  This function exists mainly to correctly handle end
  /// effects in feature extraction, and is not a mechanism to determine how
  /// many frames are in the decodable object (as it used to be, and for
  /// backward compatibility, still is, in the Decodable interface).
  virtual bool IsLastFrame(int32_t frame) const = 0;

  /// Gets the feature vector for this frame.  Before calling this for a given
  /// frame, it is assumed that you called NumFramesReady() and it returned a
  /// number greater than "frame".  Otherwise this call will likely crash with
  /// an assert failure.  This function is not declared const, in case there is
  /// some kind of caching going on, but most of the time it shouldn't modify
  /// the class.
  ///
  /// The returned tensor has shape (1, Dim()).
  virtual torch::Tensor GetFrame(int32_t frame) = 0;

  /// This is like GetFrame() but for a collection of frames.  There is a
  /// default implementation that just gets the frames one by one, but it
  /// may be overridden for efficiency by child classes (since sometimes
  /// it's more efficient to do things in a batch).
  ///
  /// The returned tensor has shape (frames.size(), Dim()).
  virtual std::vector<torch::Tensor> GetFrames(
      const std::vector<int32_t> &frames) {
    std::vector<torch::Tensor> features;
    features.reserve(frames.size());

    for (auto i : frames) {
      torch::Tensor f = GetFrame(i);
      features.push_back(std::move(f));
    }
    return features;
#if 0
    return torch::cat(features, /*dim*/ 0);
#endif
  }

  /// This would be called from the application, when you get more wave data.
  /// Note: the sampling_rate is typically only provided so the code can assert
  /// that it matches the sampling rate expected in the options.
  virtual void AcceptWaveform(float sampling_rate,
                              const torch::Tensor &waveform) = 0;

  /// InputFinished() tells the class you won't be providing any
  /// more waveform.  This will help flush out the last few frames
  /// of delta or LDA features (it will typically affect the return value
  /// of IsLastFrame.
  virtual void InputFinished() = 0;
};

}  // namespace kaldifeat

#endif  // KALDIFEAT_CSRC_ONLINE_FEATURE_ITF_H_
