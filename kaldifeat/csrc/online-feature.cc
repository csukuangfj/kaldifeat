// kaldifeat/csrc/online-feature.cc
//
// Copyright (c)  2022  Xiaomi Corporation (authors: Fangjun Kuang)

// This file is copied/modified from kaldi/src/feat/online-feature.cc

#include "kaldifeat/csrc/online-feature.h"

#include "kaldifeat/csrc/feature-window.h"
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

template <class C>
OnlineGenericBaseFeature<C>::OnlineGenericBaseFeature(
    const typename C::Options &opts)
    : computer_(opts),
      window_function_(opts.frame_opts, opts.device),
      features_(opts.frame_opts.max_feature_vectors),
      input_finished_(false),
      waveform_offset_(0) {}

template <class C>
void OnlineGenericBaseFeature<C>::AcceptWaveform(
    float sampling_rate, const torch::Tensor &original_waveform) {
  if (original_waveform.numel() == 0) return;  // Nothing to do.

  KALDIFEAT_ASSERT(original_waveform.dim() == 1);
  KALDIFEAT_ASSERT(sampling_rate == computer_.GetFrameOptions().samp_freq);

  if (input_finished_)
    KALDIFEAT_ERR << "AcceptWaveform called after InputFinished() was called.";

  if (waveform_remainder_.numel() == 0) {
    waveform_remainder_ = original_waveform;
  } else {
    waveform_remainder_ =
        torch::cat({waveform_remainder_, original_waveform}, /*dim*/ 0);
  }

  ComputeFeatures();
}

template <class C>
void OnlineGenericBaseFeature<C>::InputFinished() {
  input_finished_ = true;
  ComputeFeatures();
}

template <class C>
void OnlineGenericBaseFeature<C>::ComputeFeatures() {
  const FrameExtractionOptions &frame_opts = computer_.GetFrameOptions();

  int64_t num_samples_total = waveform_offset_ + waveform_remainder_.numel();
  int32_t num_frames_old = features_.Size();
  int32_t num_frames_new =
      NumFrames(num_samples_total, frame_opts, input_finished_);

  KALDIFEAT_ASSERT(num_frames_new >= num_frames_old);

  // note: this online feature-extraction code does not support VTLN.
  float vtln_warp = 1.0;

  for (int32_t frame = num_frames_old; frame < num_frames_new; ++frame) {
    torch::Tensor window =
        ExtractWindow(waveform_offset_, waveform_remainder_, frame, frame_opts);

    // TODO(fangjun): We can compute all feature frames at once
    torch::Tensor this_feature =
        computer_.ComputeFeatures(window.unsqueeze(0), vtln_warp);
    features_.PushBack(this_feature);
  }

  // OK, we will now discard any portion of the signal that will not be
  // necessary to compute frames in the future.
  int64_t first_sample_of_next_frame =
      FirstSampleOfFrame(num_frames_new, frame_opts);
  int32_t samples_to_discard = first_sample_of_next_frame - waveform_offset_;
  if (samples_to_discard > 0) {
    // discard the leftmost part of the waveform that we no longer need.
    int32_t new_num_samples = waveform_remainder_.numel() - samples_to_discard;
    if (new_num_samples <= 0) {
      // odd, but we'll try to handle it.
      waveform_offset_ += waveform_remainder_.numel();
      waveform_remainder_.resize_({0});
    } else {
      using torch::indexing::None;
      using torch::indexing::Slice;

      waveform_remainder_ =
          waveform_remainder_.index({Slice(samples_to_discard, None)});

      waveform_offset_ += samples_to_discard;
    }
  }
}

// instantiate the templates defined here for MFCC, PLP and filterbank classes.
template class OnlineGenericBaseFeature<Mfcc>;
template class OnlineGenericBaseFeature<Plp>;
template class OnlineGenericBaseFeature<Fbank>;

}  // namespace kaldifeat
