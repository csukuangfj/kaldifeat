#!/usr/bin/env python3

# Copyright      2021  Xiaomi Corporation (authors: Fangjun Kuang)

from pathlib import Path

import torch
from utils import get_devices, read_ark_txt, read_wave

import kaldifeat

cur_dir = Path(__file__).resolve().parent


def test_plp_default():
    print("=====test_plp_default=====")
    for device in get_devices():
        print("device", device)
        opts = kaldifeat.PlpOptions()
        opts.frame_opts.dither = 0
        opts.device = device
        plp = kaldifeat.Plp(opts)
        filename = cur_dir / "test_data/test.wav"
        wave = read_wave(filename).to(device)

        features = plp(wave)
        gt = read_ark_txt(cur_dir / "test_data/test-plp.txt")
        assert torch.allclose(features.cpu(), gt, rtol=1e-1)


def test_plp_no_snip_edges():
    print("=====test_plp_no_snip_edges=====")
    for device in get_devices():
        print("device", device)
        opts = kaldifeat.PlpOptions()
        opts.device = device
        opts.frame_opts.dither = 0
        opts.frame_opts.snip_edges = False

        plp = kaldifeat.Plp(opts)
        filename = cur_dir / "test_data/test.wav"
        wave = read_wave(filename).to(device)

        features = plp(wave)
        gt = read_ark_txt(cur_dir / "test_data/test-plp-no-snip-edges.txt")
        assert torch.allclose(features.cpu(), gt, atol=1e-1)


def test_plp_htk_10_ceps():
    print("=====test_plp_htk_10_ceps=====")
    for device in get_devices():
        print("device", device)
        opts = kaldifeat.PlpOptions()
        opts.device = device
        opts.htk_compat = True
        opts.num_ceps = 10
        opts.frame_opts.dither = 0

        plp = kaldifeat.Plp(opts)
        filename = cur_dir / "test_data/test.wav"
        wave = read_wave(filename).to(device)

        features = plp(wave)
        gt = read_ark_txt(cur_dir / "test_data/test-plp-htk-10-ceps.txt")
        assert torch.allclose(features.cpu(), gt, atol=1e-1)


if __name__ == "__main__":
    test_plp_default()
    test_plp_no_snip_edges()
    test_plp_htk_10_ceps()
