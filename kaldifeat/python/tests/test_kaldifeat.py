#!/usr/bin/env python3
#
# Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

from pathlib import Path
cur_dir = Path(__file__).resolve().parent
kaldi_feat_dir = cur_dir.parent.parent.parent

from typing import List

import sys
sys.path.insert(0, f'{kaldi_feat_dir}/build/lib')

import numpy as np
import soundfile as sf
import torch

import _kaldifeat


def read_ark_txt() -> torch.Tensor:
    test_data_dir = cur_dir / 'test_data'
    filename = test_data_dir / 'abc.txt'
    features = []
    with open(filename) as f:
        for line in f:
            if '[' in line: continue
            line = line.strip('').split()
            data = [float(d) for d in line if d != ']']
            features.append(data)
    ans = torch.tensor(features)
    return ans


def parse_str(s) -> torch.Tensor:
    '''
    Args:
      s:
        It consists of several lines. Each line contains several numbers
        separated by spaces.
    '''
    ans = []
    for line in s.strip().split('\n'):
        data = [float(d) for d in line.strip().split()]
        ans.append(data)
    return torch.tensor(ans)


def read_wave() -> torch.Tensor:
    test_data_dir = cur_dir / 'test_data'
    filename = test_data_dir / 'abc.wav'
    with sf.SoundFile(filename) as sf_desc:
        sampling_rate = sf_desc.samplerate
        assert sampling_rate == 16000
        data = sf_desc.read(dtype=np.float32, always_2d=False)
    data *= 32768
    return torch.from_numpy(data)


def test_and_benchmark_default_parameters():
    devices = [torch.device('cpu')]
    if torch.cuda.is_available():
        devices.append(torch.device('cuda', 0))

    for device in devices:
        fbank_opts = _kaldifeat.FbankOptions()
        fbank_opts.frame_opts.dither = 0
        fbank_opts.set_device(device)
        fbank = _kaldifeat.Fbank(fbank_opts)

        data = read_wave().to(device)

        ans, elapsed_seconds = _kaldifeat._compute_with_elapsed_time(
            data, fbank)

        expected = read_ark_txt()
        assert torch.allclose(ans.cpu(), expected, rtol=1e-3)
        print(f'elapsed seconds {device}:', elapsed_seconds)


def test_use_energy_htk_compat_true():
    devices = [torch.device('cpu')]
    if torch.cuda.is_available():
        devices.append(torch.device('cuda', 0))

    for device in devices:
        fbank_opts = _kaldifeat.FbankOptions()
        fbank_opts.frame_opts.dither = 0
        fbank_opts.set_device(device)
        fbank_opts.use_energy = True
        fbank_opts.htk_compat = True
        fbank = _kaldifeat.Fbank(fbank_opts)

        data = read_wave().to(device)

        ans = _kaldifeat.compute(data, fbank)

        # ./compute-fbank-feats --dither=0 --use-energy=1 --htk-compat=1 scp:abc.scp ark,t:abc.txt
        # the first 3 rows are:
        expected_str = '''
            15.576 21.93211 25.55334 24.08283 15.93041 12.47176 10.47909 9.024426 7.899537 6.935482 6.21563 6.035741 6.140291 5.94696 6.146772 6.860236 6.702379 7.087324 6.929666 7.66336 7.935287 8.405977 8.309303 25.38995
            15.5755 21.93212 25.55334 24.08282 15.93044 12.47107 10.47753 9.026523 7.901362 6.939464 6.189109 5.926141 5.678882 5.553694 6.006057 6.066478 6.500169 7.277717 7.248817 7.699819 7.990362 8.033764 8.220113 25.38996
            15.57543 21.93211 25.55334 24.08282 15.93052 12.47129 10.4782 9.028108 7.90429 6.946663 6.310408 5.903729 5.777827 6.027511 6.000434 6.190129 5.968217 6.455313 7.450428 7.993948 8.512851 8.341401 8.14073 25.38995
        '''
        expected = parse_str(expected_str)
        assert torch.allclose(ans[:3, :].cpu(), expected, rtol=1e-3)


def test_use_energy_htk_compat_false():
    devices = [torch.device('cpu')]
    if torch.cuda.is_available():
        devices.append(torch.device('cuda', 0))

    for device in devices:
        fbank_opts = _kaldifeat.FbankOptions()
        fbank_opts.frame_opts.dither = 0
        fbank_opts.use_energy = True
        fbank_opts.htk_compat = False
        fbank_opts.set_device(device)
        fbank = _kaldifeat.Fbank(fbank_opts)

        data = read_wave().to(device)

        ans = _kaldifeat.compute(data, fbank)

        # ./compute-fbank-feats --dither=0 --use-energy=1 --htk-compat=0 scp:abc.scp ark,t:abc.txt
        # the first 3 rows are:
        expected_str = '''
          25.38995 15.576 21.93211 25.55334 24.08283 15.93041 12.47176 10.47909 9.024426 7.899537 6.935482 6.21563 6.035741 6.140291 5.94696 6.146772 6.860236 6.702379 7.087324 6.929666 7.66336 7.935287 8.405977 8.309303 
          25.38996 15.5755 21.93212 25.55334 24.08282 15.93044 12.47107 10.47753 9.026523 7.901362 6.939464 6.189109 5.926141 5.678882 5.553694 6.006057 6.066478 6.500169 7.277717 7.248817 7.699819 7.990362 8.033764 8.220113 
          25.38995 15.57543 21.93211 25.55334 24.08282 15.93052 12.47129 10.4782 9.028108 7.90429 6.946663 6.310408 5.903729 5.777827 6.027511 6.000434 6.190129 5.968217 6.455313 7.450428 7.993948 8.512851 8.341401 8.14073 
        '''
        expected = parse_str(expected_str)
        assert torch.allclose(ans[:3, :].cpu(), expected, rtol=1e-3)


def test_compute_batch():
    devices = [torch.device('cpu')]
    if torch.cuda.is_available():
        devices.append(torch.device('cuda', 0))

    for device in devices:
        data1 = read_wave().to(device)
        data2 = read_wave().to(device)

        fbank_opts = _kaldifeat.FbankOptions()
        fbank_opts.frame_opts.dither = 0
        fbank_opts.set_device(device)
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

            features = _kaldifeat.compute(strided, fbank)
            feature1 = features[:num_frames[0]]
            feature2 = features[num_frames[0]:]
            return [feature1, feature2]

        feature1, feature2 = impl([data1, data2])
        assert torch.allclose(feature1, feature2)


def main():
    test_and_benchmark_default_parameters()
    test_use_energy_htk_compat_true()
    test_use_energy_htk_compat_false()

    test_compute_batch()


if __name__ == '__main__':
    main()
