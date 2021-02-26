#!/usr/bin/env python3
#
# Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

import sys

sys.path.insert(0, '/root/fangjun/open-source/kaldifeat/build/lib')

import torch
import numpy as np
import soundfile as sf
import _kaldifeat


def main():
    # sox -n -r 16000 -b 16 abc.wav synth 1 sine 100
    filename = '/root/fangjun/open-source/kaldi/src/featbin/abc.wav'
    with sf.SoundFile(filename) as sf_desc:
        sampling_rate = sf_desc.samplerate
        assert sampling_rate == 16000
        a = sf_desc.read(dtype=np.float32, always_2d=False)
        a *= 32768
        tensor = torch.from_numpy(a)
        ans = _kaldifeat.test(tensor)
        #  torch.set_printoptions(profile="full")
        #  print(ans.shape)
        #  print(ans)


if __name__ == '__main__':
    main()
