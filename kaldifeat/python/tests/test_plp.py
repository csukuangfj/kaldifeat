#!/usr/bin/env python3

# Copyright      2021  Xiaomi Corporation (authors: Fangjun Kuang)

import pickle
from pathlib import Path

import torch
from utils import get_devices, read_ark_txt, read_wave

import kaldifeat

cur_dir = Path(__file__).resolve().parent


def test_online_plp(
    opts: kaldifeat.PlpOptions,
    wave: torch.Tensor,
    cpu_features: torch.Tensor,
):
    """
    Args:
      opts:
        The options to create the online plp extractor.
      wave:
        The input 1-D waveform.
      cpu_features:
        The groud truth features that are computed offline
    """
    online_plp = kaldifeat.OnlinePlp(opts)

    num_processed_frames = 0
    i = 0  # current sample index to feed
    while not online_plp.is_last_frame(num_processed_frames - 1):
        while num_processed_frames < online_plp.num_frames_ready:
            # There are new frames to be processed
            frame = online_plp.get_frame(num_processed_frames)
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
            online_plp.input_finished()
            continue

        online_plp.accept_waveform(16000, samples)

    assert num_processed_frames == online_plp.num_frames_ready
    assert num_processed_frames == cpu_features.size(0)


def test_plp_default():
    print("=====test_plp_default=====")
    filename = cur_dir / "test_data/test.wav"
    wave = read_wave(filename)
    gt = read_ark_txt(cur_dir / "test_data/test-plp.txt")

    cpu_features = None
    for device in get_devices():
        print("device", device)
        opts = kaldifeat.PlpOptions()
        opts.frame_opts.dither = 0
        opts.device = device
        plp = kaldifeat.Plp(opts)

        features = plp(wave.to(device))
        if device.type == "cpu":
            cpu_features = features

        assert torch.allclose(features.cpu(), gt, rtol=1e-1)

    opts = kaldifeat.PlpOptions()
    opts.frame_opts.dither = 0

    test_online_plp(opts, wave, cpu_features)


def test_plp_no_snip_edges():
    print("=====test_plp_no_snip_edges=====")
    filename = cur_dir / "test_data/test.wav"
    wave = read_wave(filename)
    gt = read_ark_txt(cur_dir / "test_data/test-plp-no-snip-edges.txt")

    cpu_features = None
    for device in get_devices():
        print("device", device)
        opts = kaldifeat.PlpOptions()
        opts.device = device
        opts.frame_opts.dither = 0
        opts.frame_opts.snip_edges = False

        plp = kaldifeat.Plp(opts)

        features = plp(wave.to(device))
        if device.type == "cpu":
            cpu_features = features
        assert torch.allclose(features.cpu(), gt, atol=1e-1)

    opts = kaldifeat.PlpOptions()
    opts.frame_opts.dither = 0
    opts.frame_opts.snip_edges = False

    test_online_plp(opts, wave, cpu_features)


def test_plp_htk_10_ceps():
    print("=====test_plp_htk_10_ceps=====")
    filename = cur_dir / "test_data/test.wav"
    wave = read_wave(filename)
    gt = read_ark_txt(cur_dir / "test_data/test-plp-htk-10-ceps.txt")

    cpu_features = None
    for device in get_devices():
        print("device", device)
        opts = kaldifeat.PlpOptions()
        opts.device = device
        opts.htk_compat = True
        opts.num_ceps = 10
        opts.frame_opts.dither = 0

        plp = kaldifeat.Plp(opts)

        features = plp(wave.to(device))
        if device.type == "cpu":
            cpu_features = features
        assert torch.allclose(features.cpu(), gt, atol=1e-1)

    opts = kaldifeat.PlpOptions()
    opts.htk_compat = True
    opts.num_ceps = 10
    opts.frame_opts.dither = 0

    test_online_plp(opts, wave, cpu_features)


def test_pickle():
    for device in get_devices():
        opts = kaldifeat.PlpOptions()
        opts.device = device
        opts.frame_opts.dither = 0
        opts.frame_opts.snip_edges = False

        plp = kaldifeat.Plp(opts)
        data = pickle.dumps(plp)
        plp2 = pickle.loads(data)

        assert str(plp.opts) == str(plp2.opts)

    opts = kaldifeat.PlpOptions()
    opts.frame_opts.dither = 0
    opts.frame_opts.snip_edges = False

    plp = kaldifeat.OnlinePlp(opts)
    data = pickle.dumps(plp)
    plp2 = pickle.loads(data)

    assert str(plp.opts) == str(plp2.opts)


if __name__ == "__main__":
    test_plp_default()
    test_plp_no_snip_edges()
    test_plp_htk_10_ceps()
    test_pickle()
