#!/usr/bin/env python3
#
# Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

import sys
from pathlib import Path
from typing import List

cur_dir = Path(__file__).resolve().parent
kaldi_feat_dir = cur_dir.parent.parent.parent


sys.path.insert(0, f"{kaldi_feat_dir}/build/lib")

import _kaldifeat
import numpy as np
import soundfile as sf
import torch


def read_ark_txt(filename) -> torch.Tensor:
    test_data_dir = cur_dir / "test_data"
    filename = test_data_dir / filename
    features = []
    with open(filename) as f:
        for line in f:
            if "[" in line:
                continue
            line = line.strip("").split()
            data = [float(d) for d in line if d != "]"]
            features.append(data)
    ans = torch.tensor(features)
    return ans


def read_wave() -> torch.Tensor:
    test_data_dir = cur_dir / "test_data"
    filename = test_data_dir / "test.wav"
    with sf.SoundFile(filename) as sf_desc:
        sampling_rate = sf_desc.samplerate
        assert sampling_rate == 16000
        data = sf_desc.read(dtype=np.float32, always_2d=False)
    data *= 32768
    return torch.from_numpy(data)


def test_and_benchmark_default_parameters():
    devices = [torch.device("cpu")]
    if torch.cuda.is_available():
        devices.append(torch.device("cuda", 0))

    for device in devices:
        fbank_opts = _kaldifeat.FbankOptions()
        fbank_opts.frame_opts.dither = 0
        fbank_opts.device = device
        fbank = _kaldifeat.Fbank(fbank_opts)

        data = read_wave().to(device)

        ans, elapsed_seconds = _kaldifeat._compute_with_elapsed_time(
            data, fbank
        )

        expected = read_ark_txt("test.txt")
        assert torch.allclose(ans.cpu(), expected, rtol=1e-2)
        print(f"elapsed seconds {device}:", elapsed_seconds)


def test_use_energy_htk_compat_true():
    devices = [torch.device("cpu")]
    if torch.cuda.is_available():
        devices.append(torch.device("cuda", 0))

    for device in devices:
        fbank_opts = _kaldifeat.FbankOptions()
        fbank_opts.frame_opts.dither = 0
        fbank_opts.device = device
        fbank_opts.use_energy = True
        fbank_opts.htk_compat = True
        fbank = _kaldifeat.Fbank(fbank_opts)

        data = read_wave().to(device)

        ans = _kaldifeat.compute_fbank_feats(data, fbank)

        expected = read_ark_txt("test-htk.txt")
        assert torch.allclose(ans.cpu(), expected, rtol=1e-2)


def test_use_energy_htk_compat_false():
    devices = [torch.device("cpu")]
    if torch.cuda.is_available():
        devices.append(torch.device("cuda", 0))

    for device in devices:
        fbank_opts = _kaldifeat.FbankOptions()
        fbank_opts.frame_opts.dither = 0
        fbank_opts.use_energy = True
        fbank_opts.htk_compat = False
        fbank_opts.device = device
        fbank = _kaldifeat.Fbank(fbank_opts)

        data = read_wave().to(device)

        ans = _kaldifeat.compute_fbank_feats(data, fbank)

        expected = read_ark_txt("test-with-energy.txt")
        assert torch.allclose(ans.cpu(), expected, rtol=1e-2)


def test_40_mel():
    devices = [torch.device("cpu")]
    if torch.cuda.is_available():
        devices.append(torch.device("cuda", 0))

    for device in devices:
        fbank_opts = _kaldifeat.FbankOptions()
        fbank_opts.frame_opts.dither = 0
        fbank_opts.mel_opts.num_bins = 40
        fbank_opts.device = device
        fbank = _kaldifeat.Fbank(fbank_opts)

        data = read_wave().to(device)

        ans = _kaldifeat.compute_fbank_feats(data, fbank)

        expected = read_ark_txt("test-40.txt")
        assert torch.allclose(ans.cpu(), expected, rtol=1e-1)


def test_40_mel_no_snip_edges():
    devices = [torch.device("cpu")]
    if torch.cuda.is_available():
        devices.append(torch.device("cuda", 0))

    for device in devices:
        fbank_opts = _kaldifeat.FbankOptions()
        fbank_opts.frame_opts.snip_edges = False
        fbank_opts.frame_opts.dither = 0
        fbank_opts.mel_opts.num_bins = 40
        fbank_opts.device = device
        fbank = _kaldifeat.Fbank(fbank_opts)

        data = read_wave().to(device)

        ans = _kaldifeat.compute_fbank_feats(data, fbank)

        expected = read_ark_txt("test-40-no-snip-edges.txt")
        assert torch.allclose(ans.cpu(), expected, rtol=1e-2)


def test_compute_batch():
    devices = [torch.device("cpu")]
    if torch.cuda.is_available():
        devices.append(torch.device("cuda", 0))

    for device in devices:
        data1 = read_wave().to(device)
        data2 = read_wave().to(device)

        fbank_opts = _kaldifeat.FbankOptions()
        fbank_opts.frame_opts.dither = 0
        fbank_opts.frame_opts.snip_edges = False
        fbank_opts.device = device
        fbank = _kaldifeat.Fbank(fbank_opts)

        def impl(waves: List[torch.Tensor]) -> List[torch.Tensor]:
            num_frames = [
                _kaldifeat.num_frames(w.numel(), fbank_opts.frame_opts)
                for w in waves
            ]

            strided = [
                _kaldifeat.get_strided(w, fbank_opts.frame_opts) for w in waves
            ]
            strided = torch.cat(strided, dim=0)

            features = _kaldifeat.compute_fbank_feats(strided, fbank).split(
                num_frames
            )

            return features

        feature1, feature2 = impl([data1, data2])
        assert torch.allclose(feature1, feature2)


def main():
    test_and_benchmark_default_parameters()
    test_use_energy_htk_compat_true()
    test_use_energy_htk_compat_false()
    test_40_mel()
    test_40_mel_no_snip_edges()

    test_compute_batch()


if __name__ == "__main__":
    main()
