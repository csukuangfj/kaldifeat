#!/usr/bin/env python3
#
# Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

from pathlib import Path
cur_dir = Path(__file__).resolve().parent
kaldi_feat_dir = cur_dir.parent.parent.parent

import sys
sys.path.insert(0, f'{kaldi_feat_dir}/build/lib')

import torch
import numpy as np
import soundfile as sf
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


def main():
    test_data_dir = cur_dir / 'test_data'
    filename = test_data_dir / 'abc.wav'
    with sf.SoundFile(filename) as sf_desc:
        sampling_rate = sf_desc.samplerate
        assert sampling_rate == 16000
        data = sf_desc.read(dtype=np.float32, always_2d=False)
    data *= 32768
    tensor = torch.from_numpy(data)
    ans, elapsed_seconds = _kaldifeat.test_default_parameters(tensor)
    expected = read_ark_txt()
    assert torch.allclose(ans, expected, rtol=1e-3)
    print('elapsed seconds:', elapsed_seconds)


if __name__ == '__main__':
    main()
