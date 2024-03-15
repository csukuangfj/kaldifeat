#!/usr/bin/env python3

# Copyright      2021-2022  Xiaomi Corporation (authors: Fangjun Kuang)

import pickle
from pathlib import Path

import torch
from utils import get_devices, read_ark_txt, read_wave

import kaldifeat

cur_dir = Path(__file__).resolve().parent


def test_pitch_default():
    print("=====test_pitch_default=====")
    filename = cur_dir / "test_data/test.wav"
    wave = read_wave(filename)
    gt = read_ark_txt(cur_dir / "test_data/test-pitch.txt")

    cpu_features = None
    for device in get_devices():
        print("device", device)
        opts = kaldifeat.PitchOptions()
        opts.device = device
        opts.frame_opts.dither = 0
        pitch = kaldifeat.Pitch(opts)

        features = pitch(wave)
        assert features.device.type == "cpu"
        assert torch.allclose(features, gt, rtol=1e-4)
        if cpu_features is None:
            cpu_features = features

        features = pitch(wave.to(device))
        assert features.device == device
        assert torch.allclose(features.cpu(), gt, rtol=1e-4)

if __name__ == "__main__":
    test_pitch_default()
