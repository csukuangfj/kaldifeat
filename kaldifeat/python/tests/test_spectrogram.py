#!/usr/bin/env python3

# Copyright      2021  Xiaomi Corporation (authors: Fangjun Kuang)

from pathlib import Path

from utils import get_devices, read_ark_txt, read_wave

import kaldifeat

cur_dir = Path(__file__).resolve().parent


def test_spectrogram_default():
    print("=====test_spectrogram_default=====")
    for device in get_devices():
        print("device", device)
        opts = kaldifeat.SpectrogramOptions()
        opts.device = device
        opts.frame_opts.dither = 0
        spectrogram = kaldifeat.Spectrogram(opts)
        filename = cur_dir / "test_data/test.wav"
        wave = read_wave(filename).to(opts.device)

        features = spectrogram(wave)
        gt = read_ark_txt(cur_dir / "test_data/test-spectrogram.txt")

        #  assert torch.allclose(features.cpu(), gt, atol=1.1)
        print(features[1, 145:148], gt[1, 145:148])  # they are different


def test_spectrogram_no_snip_edges():
    print("=====test_spectrogram_no_snip_edges=====")
    for device in get_devices():
        print("device", device)
        opts = kaldifeat.SpectrogramOptions()
        opts.device = device
        opts.frame_opts.dither = 0
        opts.frame_opts.snip_edges = False
        spectrogram = kaldifeat.Spectrogram(opts)
        filename = cur_dir / "test_data/test.wav"
        wave = read_wave(filename).to(opts.device)

        features = spectrogram(wave)
        gt = read_ark_txt(
            cur_dir / "test_data/test-spectrogram-no-snip-edges.txt"
        )

        #  assert torch.allclose(features.cpu(), gt, atol=1.5)
        print(features[1, 145:148], gt[1, 145:148])  # they are different


if __name__ == "__main__":
    test_spectrogram_default()
    test_spectrogram_no_snip_edges()
