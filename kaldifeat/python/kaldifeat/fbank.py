# Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

from typing import List, Union

import _kaldifeat
import torch
import torch.nn as nn


class Fbank(nn.Module):
    def __init__(self, opts: _kaldifeat.FbankOptions):
        super().__init__()

        self.opts = opts
        self.computer = _kaldifeat.Fbank(opts)

    def forward(
        self, waves: Union[torch.Tensor, List[torch.Tensor]]
    ) -> Union[torch.Tensor, List[torch.Tensor]]:
        """Compute the fbank features of a single waveform or
        a list of waveforms.

        Args:
          waves:
            A single 1-D tensor or a  list of 1-D tensors. Each tensor contains
            audio samples of a soundfile. To get a result compatible with Kaldi,
            you should scale the samples to [-32768, 32767] before calling this
            function. Note: You are not required to scale them if you don't care
            about the compatibility with Kaldi.
        Returns:
          Return a list of 2-D tensors containing the fbank features if the
          input is a list of 1-D tensors. The returned list has as many elements
          as the input list.
          Return a single 2-D tensor if the input is a single tensor.
        """
        if isinstance(waves, list):
            is_list = True
        else:
            waves = [waves]
            is_list = False

        num_frames_per_wave = [
            _kaldifeat.num_frames(w.numel(), self.opts.frame_opts)
            for w in waves
        ]

        strided = [self.convert_samples_to_frames(w) for w in waves]
        strided = torch.cat(strided, dim=0)

        features = self.compute(strided)

        if is_list:
            return list(features.split(num_frames_per_wave))
        else:
            return features

    def compute(self, x: torch.Tensor) -> torch.Tensor:
        """Compute fbank features given a 2-D tensor containing
        frames data. Each row is a frame of size frame_lens, specified
        in the fbank options.
        Args:
          x:
            A 2-D tensor.
        Returns:
          Return a 2-D tensor with as many rows as the input tensor. Its
          number of columns is the number mel bins.
        """
        features = _kaldifeat.compute_fbank_feats(x, self.computer)
        return features

    def convert_samples_to_frames(self, wave: torch.Tensor) -> torch.Tensor:
        """Convert a 1-D tensor containing audio samples to a 2-D
        tensor where each row is a frame of samples of size frame length
        specified in the fbank options.

        Args:
          waves:
            A 1-D tensor.
        Returns:
          Return a 2-D tensor.
        """
        return _kaldifeat.get_strided(wave, self.opts.frame_opts)
