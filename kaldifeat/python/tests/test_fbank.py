#!/usr/bin/env python3

# Copyright      2021  Xiaomi Corporation (authors: Fangjun Kuang)

import numpy as np
import soundfile as sf
import torch

import kaldifeat


def read_wave(filename) -> torch.Tensor:
    """Read a wave file and return it as a 1-D tensor.

    Note:
      You don't need to scale it to [-32768, 32767].
      We use scaling here to follow the approach in Kaldi.

    Args:
      filename:
        Filename of a sound file.
    Returns:
      Return a 1-D tensor containing audio samples.
    """
    with sf.SoundFile(filename) as sf_desc:
        sampling_rate = sf_desc.samplerate
        assert sampling_rate == 16000
        data = sf_desc.read(dtype=np.float32, always_2d=False)
    data *= 32768
    return torch.from_numpy(data)


def test_fbank():
    device = torch.device("cpu")
    #  if torch.cuda.is_available():
    #      device = torch.device("cuda", 0)

    wave0 = read_wave("test_data/test.wav")
    wave1 = read_wave("test_data/test2.wav")

    wave0 = wave0.to(device)
    wave1 = wave1.to(device)

    opts = kaldifeat.FbankOptions()
    opts.frame_opts.dither = 0
    opts.device = device

    fbank = kaldifeat.Fbank(opts)

    # We can compute fbank features in batches
    features = fbank([wave0, wave1])
    assert isinstance(features, list), f"{type(features)}"
    assert len(features) == 2

    # We can also compute fbank features for a single wave
    features0 = fbank(wave0)
    features1 = fbank(wave1)

    assert torch.allclose(features[0], features0)
    assert torch.allclose(features[1], features1)

    # To compute fbank features for only a specified frame
    audio_frames = fbank.convert_samples_to_frames(wave0)
    feature_frame_1 = fbank.compute(audio_frames[1:2])
    feature_frame_10 = fbank.compute(audio_frames[10:11])

    assert torch.allclose(features0[1], feature_frame_1)
    assert torch.allclose(features0[10], feature_frame_10)


if __name__ == "__main__":
    test_fbank()
