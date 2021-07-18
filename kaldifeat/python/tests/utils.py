# Copyright      2021  Xiaomi Corporation (authors: Fangjun Kuang)

from typing import List

import numpy as np
import soundfile as sf
import torch


def read_wave(filename) -> torch.Tensor:
    """Read a wave file and return it as a 1-D tensor.

    Note:
      You don't need to scale it to [-32768, 32767].
      We use scaling here to follow the approach in Kaldi.

    Args:
      filename:
        Filename of a sound file.
    Returns:
      Return a 1-D tensor containing audio samples.
    """
    with sf.SoundFile(filename) as sf_desc:
        sampling_rate = sf_desc.samplerate
        assert sampling_rate == 16000
        data = sf_desc.read(dtype=np.float32, always_2d=False)
    data *= 32768
    return torch.from_numpy(data)


def read_ark_txt(filename) -> torch.Tensor:
    #  test_data_dir = cur_dir / "test_data"
    #  filename = test_data_dir / filename
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


def get_devices() -> List[torch.device]:
    ans = [torch.device("cpu")]
    if torch.cuda.is_available():
        ans.append(torch.device("cuda", 0))
        if torch.cuda.device_count() > 1:
            ans.append(torch.device("cuda", 1))
    return ans
