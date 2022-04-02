#!/usr/bin/env python3

# Copyright      2021  Xiaomi Corporation (authors: Fangjun Kuang)

import pickle
from pathlib import Path

import torch
from utils import get_devices, read_ark_txt, read_wave

import kaldifeat

cur_dir = Path(__file__).resolve().parent


def test_online_mfcc(
    opts: kaldifeat.MfccOptions,
    wave: torch.Tensor,
    cpu_features: torch.Tensor,
):
    """
    Args:
      opts:
        The options to create the online mfcc extractor.
      wave:
        The input 1-D waveform.
      cpu_features:
        The groud truth features that are computed offline
    """
    online_mfcc = kaldifeat.OnlineMfcc(opts)

    num_processed_frames = 0
    i = 0  # current sample index to feed
    while not online_mfcc.is_last_frame(num_processed_frames - 1):
        while num_processed_frames < online_mfcc.num_frames_ready:
            # There are new frames to be processed
            frame = online_mfcc.get_frame(num_processed_frames)
            assert torch.allclose(
                frame.squeeze(0), cpu_features[num_processed_frames], atol=1e-3
            )
            num_processed_frames += 1

        # Simulate streaming . Send a random number of audio samples
        # to the extractor
        num_samples = torch.randint(300, 1000, (1,)).item()

        samples = wave[i : (i + num_samples)]  # noqa
        i += num_samples
        if len(samples) == 0:
            online_mfcc.input_finished()
            continue

        online_mfcc.accept_waveform(16000, samples)

    assert num_processed_frames == online_mfcc.num_frames_ready
    assert num_processed_frames == cpu_features.size(0)


def test_mfcc_default():
    print("=====test_mfcc_default=====")
    filename = cur_dir / "test_data/test.wav"
    wave = read_wave(filename)
    gt = read_ark_txt(cur_dir / "test_data/test-mfcc.txt")

    cpu_features = None
    for device in get_devices():
        print("device", device)
        opts = kaldifeat.MfccOptions()
        opts.device = device
        opts.frame_opts.dither = 0
        mfcc = kaldifeat.Mfcc(opts)

        features = mfcc(wave.to(device))
        if device.type == "cpu":
            cpu_features = features

        assert torch.allclose(features.cpu(), gt, atol=1e-1)

    opts = kaldifeat.MfccOptions()
    opts.frame_opts.dither = 0

    test_online_mfcc(opts, wave, cpu_features)


def test_mfcc_no_snip_edges():
    print("=====test_mfcc_no_snip_edges=====")
    filename = cur_dir / "test_data/test.wav"
    wave = read_wave(filename)
    gt = read_ark_txt(cur_dir / "test_data/test-mfcc-no-snip-edges.txt")

    cpu_features = None
    for device in get_devices():
        print("device", device)
        opts = kaldifeat.MfccOptions()
        opts.device = device
        opts.frame_opts.dither = 0
        opts.frame_opts.snip_edges = False

        mfcc = kaldifeat.Mfcc(opts)

        features = mfcc(wave.to(device))
        if device.type == "cpu":
            cpu_features = features

        assert torch.allclose(features.cpu(), gt, rtol=1e-1)

    opts = kaldifeat.MfccOptions()
    opts.frame_opts.dither = 0
    opts.frame_opts.snip_edges = False

    test_online_mfcc(opts, wave, cpu_features)


def test_pickle():
    for device in get_devices():
        opts = kaldifeat.MfccOptions()
        opts.device = device
        opts.frame_opts.dither = 0
        opts.frame_opts.snip_edges = False

        mfcc = kaldifeat.Mfcc(opts)
        data = pickle.dumps(mfcc)
        mfcc2 = pickle.loads(data)

        assert str(mfcc.opts) == str(mfcc2.opts)

    opts = kaldifeat.MfccOptions()
    opts.frame_opts.dither = 0
    opts.frame_opts.snip_edges = False

    mfcc = kaldifeat.OnlineMfcc(opts)
    data = pickle.dumps(mfcc)
    mfcc2 = pickle.loads(data)

    assert str(mfcc.opts) == str(mfcc2.opts)


if __name__ == "__main__":
    test_mfcc_default()
    test_mfcc_no_snip_edges()
    test_pickle()
