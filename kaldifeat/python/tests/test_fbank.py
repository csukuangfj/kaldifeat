#!/usr/bin/env python3

# Copyright      2021-2022  Xiaomi Corporation (authors: Fangjun Kuang)

import pickle
from pathlib import Path

import torch
from utils import get_devices, read_ark_txt, read_wave

import kaldifeat

cur_dir = Path(__file__).resolve().parent


def test_online_fbank(
    opts: kaldifeat.FbankOptions,
    wave: torch.Tensor,
    cpu_features: torch.Tensor,
):
    """
    Args:
      opts:
        The options to create the online fbank extractor.
      wave:
        The input 1-D waveform.
      cpu_features:
        The groud truth features that are computed offline
    """
    online_fbank = kaldifeat.OnlineFbank(opts)

    num_processed_frames = 0
    i = 0  # current sample index to feed
    while not online_fbank.is_last_frame(num_processed_frames - 1):
        while num_processed_frames < online_fbank.num_frames_ready:
            # There are new frames to be processed
            frame = online_fbank.get_frame(num_processed_frames)
            assert torch.allclose(
                frame.squeeze(0), cpu_features[num_processed_frames]
            )
            num_processed_frames += 1

        # Simulate streaming . Send a random number of audio samples
        # to the extractor
        num_samples = torch.randint(300, 1000, (1,)).item()

        samples = wave[i : (i + num_samples)]  # noqa
        i += num_samples
        if len(samples) == 0:
            online_fbank.input_finished()
            continue

        online_fbank.accept_waveform(16000, samples)

    assert num_processed_frames == online_fbank.num_frames_ready
    assert num_processed_frames == cpu_features.size(0)


def test_fbank_default():
    print("=====test_fbank_default=====")
    filename = cur_dir / "test_data/test.wav"
    wave = read_wave(filename)
    gt = read_ark_txt(cur_dir / "test_data/test.txt")

    cpu_features = None
    for device in get_devices():
        print("device", device)
        opts = kaldifeat.FbankOptions()
        opts.device = device
        opts.frame_opts.dither = 0
        fbank = kaldifeat.Fbank(opts)

        features = fbank(wave)
        assert features.device.type == "cpu"
        assert torch.allclose(features, gt, rtol=1e-1)
        if cpu_features is None:
            cpu_features = features

        features = fbank(wave.to(device))
        assert features.device == device
        assert torch.allclose(features.cpu(), gt, rtol=1e-1)

    # Now for online fbank
    opts = kaldifeat.FbankOptions()
    opts.frame_opts.dither = 0
    opts.frame_opts.max_feature_vectors = 100

    test_online_fbank(opts, wave, cpu_features)


def test_fbank_htk():
    print("=====test_fbank_htk=====")
    filename = cur_dir / "test_data/test.wav"
    wave = read_wave(filename)
    gt = read_ark_txt(cur_dir / "test_data/test-htk.txt")

    cpu_features = None
    for device in get_devices():
        print("device", device)
        opts = kaldifeat.FbankOptions()
        opts.device = device
        opts.frame_opts.dither = 0
        opts.use_energy = True
        opts.htk_compat = True

        fbank = kaldifeat.Fbank(opts)

        features = fbank(wave)
        assert features.device.type == "cpu"
        assert torch.allclose(features, gt, rtol=1e-1)
        if cpu_features is None:
            cpu_features = features

        features = fbank(wave.to(device))
        assert features.device == device
        assert torch.allclose(features.cpu(), gt, rtol=1e-1)

    opts = kaldifeat.FbankOptions()
    opts.frame_opts.dither = 0
    opts.use_energy = True
    opts.htk_compat = True

    test_online_fbank(opts, wave, cpu_features)


def test_fbank_with_energy():
    print("=====test_fbank_with_energy=====")
    filename = cur_dir / "test_data/test.wav"
    wave = read_wave(filename)
    gt = read_ark_txt(cur_dir / "test_data/test-with-energy.txt")

    cpu_features = None
    for device in get_devices():
        print("device", device)
        opts = kaldifeat.FbankOptions()
        opts.device = device
        opts.frame_opts.dither = 0
        opts.use_energy = True

        fbank = kaldifeat.Fbank(opts)

        features = fbank(wave)
        assert torch.allclose(features, gt, rtol=1e-1)
        assert features.device.type == "cpu"
        if cpu_features is None:
            cpu_features = features

        features = fbank(wave.to(device))
        assert features.device == device
        assert torch.allclose(features.cpu(), gt, rtol=1e-1)

    opts = kaldifeat.FbankOptions()
    opts.frame_opts.dither = 0
    opts.use_energy = True

    test_online_fbank(opts, wave, cpu_features)


def test_fbank_40_bins():
    print("=====test_fbank_40_bins=====")
    filename = cur_dir / "test_data/test.wav"
    wave = read_wave(filename)
    gt = read_ark_txt(cur_dir / "test_data/test-40.txt")

    cpu_features = None
    for device in get_devices():
        print("device", device)
        opts = kaldifeat.FbankOptions()
        opts.device = device
        opts.frame_opts.dither = 0
        opts.mel_opts.num_bins = 40

        fbank = kaldifeat.Fbank(opts)

        features = fbank(wave)
        assert features.device.type == "cpu"
        assert torch.allclose(features, gt, rtol=1e-1)
        if cpu_features is None:
            cpu_features = features

        features = fbank(wave.to(device))
        assert features.device == device
        assert torch.allclose(features.cpu(), gt, rtol=1e-1)

    opts = kaldifeat.FbankOptions()
    opts.frame_opts.dither = 0
    opts.mel_opts.num_bins = 40

    test_online_fbank(opts, wave, cpu_features)


def test_fbank_40_bins_no_snip_edges():
    print("=====test_fbank_40_bins_no_snip_edges=====")
    filename = cur_dir / "test_data/test.wav"
    wave = read_wave(filename)
    gt = read_ark_txt(cur_dir / "test_data/test-40-no-snip-edges.txt")

    cpu_features = None
    for device in get_devices():
        print("device", device)
        opts = kaldifeat.FbankOptions()
        opts.device = device
        opts.frame_opts.dither = 0
        opts.mel_opts.num_bins = 40
        opts.frame_opts.snip_edges = False

        fbank = kaldifeat.Fbank(opts)

        features = fbank(wave)
        assert features.device.type == "cpu"
        assert torch.allclose(features, gt, rtol=1e-1)
        if cpu_features is None:
            cpu_features = features

        features = fbank(wave.to(device))
        assert features.device == device
        assert torch.allclose(features.cpu(), gt, rtol=1e-1)

    opts = kaldifeat.FbankOptions()
    opts.frame_opts.dither = 0
    opts.mel_opts.num_bins = 40
    opts.frame_opts.snip_edges = False

    test_online_fbank(opts, wave, cpu_features)


def test_fbank_chunk():
    print("=====test_fbank_chunk=====")

    filename = cur_dir / "test_data/test-1hour.wav"
    if filename.is_file() is False:
        print(
            f"Please execute {cur_dir}/test_data/run.sh "
            f"to generate {filename} before running tis test"
        )
        return

    for device in get_devices():
        print("device", device)
        opts = kaldifeat.FbankOptions()
        opts.device = device
        opts.frame_opts.dither = 0
        opts.mel_opts.num_bins = 40
        opts.frame_opts.snip_edges = False

        fbank = kaldifeat.Fbank(opts)
        wave = read_wave(filename)

        # You can use
        #
        #  $ watch -n 0.2 free -m
        #
        # to view memory consumption
        #
        # 100 frames per chunk
        features1 = fbank(wave, chunk_size=100 * 10)
        features2 = fbank(wave)
        assert torch.allclose(features1, features2)
        assert features1.device == features2.device
        assert features1.device.type == "cpu"

        if device.type == "cuda":
            wave = wave.to(device)
            features1 = fbank(wave, chunk_size=100 * 10)
            features2 = fbank(wave)
            assert torch.allclose(features1, features2)
            assert features1.device == features2.device
            assert features1.device == device


def test_fbank_batch():
    print("=====test_fbank_batch=====")
    for device in get_devices():
        print("device", device)
        wave0 = read_wave(cur_dir / "test_data/test.wav")
        wave1 = read_wave(cur_dir / "test_data/test2.wav")

        opts = kaldifeat.FbankOptions()
        opts.device = device
        opts.frame_opts.dither = 0
        fbank = kaldifeat.Fbank(opts)

        features = fbank([wave0, wave1], chunk_size=10)

        features0 = fbank(wave0)
        features1 = fbank(wave1)

        assert torch.allclose(features[0], features0)
        assert torch.allclose(features[1], features1)

        if device.type == "cuda":
            wave0 = wave0.to(device)
            wave1 = wave1.to(device)

            features = fbank([wave0, wave1], chunk_size=10)

            features0 = fbank(wave0)
            features1 = fbank(wave1)

            assert torch.allclose(features[0], features0)
            assert torch.allclose(features[1], features1)


def test_pickle():
    for device in get_devices():
        opts = kaldifeat.FbankOptions()
        opts.use_energy = True
        opts.use_power = False
        opts.device = device

        fbank = kaldifeat.Fbank(opts)
        data = pickle.dumps(fbank)
        fbank2 = pickle.loads(data)

        assert str(fbank.opts) == str(fbank2.opts)

    opts = kaldifeat.FbankOptions()
    opts.use_energy = True
    opts.use_power = False

    fbank = kaldifeat.OnlineFbank(opts)
    data = pickle.dumps(fbank)
    fbank2 = pickle.loads(data)

    assert str(fbank.opts) == str(fbank2.opts)


if __name__ == "__main__":
    test_fbank_default()
    test_fbank_htk()
    test_fbank_with_energy()
    test_fbank_40_bins()
    test_fbank_40_bins_no_snip_edges()
    test_fbank_chunk()
    test_fbank_batch()
    test_pickle()
