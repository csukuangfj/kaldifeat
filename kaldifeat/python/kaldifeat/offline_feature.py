# Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)

from typing import List, Optional, Union

import _kaldifeat
import torch
import torch.nn as nn


class OfflineFeature(nn.Module):
    """Offline feature is a base class of other feature computers,
    e.g., Fbank, Mfcc.

    This class has two fields:

        (1) opts. It contains the options for the feature computer.
        (2) computer. The actual feature computer. It should be
            instantiated by subclasses.
    """

    def __init__(self, opts):
        super().__init__()

        self.opts = opts

        # self.computer is expected to be set by subclasses
        self.computer = None

    def forward(
        self,
        waves: Union[torch.Tensor, List[torch.Tensor]],
        vtln_warp: float = 1.0,
        chunk_size: Optional[int] = None,
    ) -> Union[torch.Tensor, List[torch.Tensor]]:
        """Compute the features of a single waveform or
        a list of waveforms.

        Args:
          waves:
            A single 1-D tensor or a  list of 1-D tensors. Each tensor contains
            audio samples of a sound file. To get a result compatible with
            Kaldi, you should scale the samples to [-32768, 32767] before
            calling this function. Note: You are not required to scale them if
            you don't care about the compatibility with Kaldi.
          vtln_warp
            The VTLN warping factor that the user wants to be applied when
            computing features for this utterance.  Will normally be 1.0,
            meaning no warping is to be done.  The value will be ignored for
            feature types that don't support VLTN, such as spectrogram features.
          chunk_size:
            It specifies the number of frames for each computation.
            If None, it computes features at once (requiring more memory for
            long waves). If not None, each computation takes this number of
            frames (requiring less memory).
        Returns:
          Return a list of 2-D tensors containing the features if the
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

        features = self.compute(strided, vtln_warp)

        if is_list:
            return list(features.split(num_frames_per_wave))
        else:
            return features

    def compute(
        self,
        x: torch.Tensor,
        vtln_warp: float = 1.0,
        chunk_size: Optional[int] = None,
    ) -> torch.Tensor:
        """Compute features given a 2-D tensor containing
        frames data. Each row is a frame of size frame_lens, specified
        in the options.
        Args:
          x:
            A 2-D tensor.
          vtln_warp
            The VTLN warping factor that the user wants to be applied when
            computing features for this utterance.  Will normally be 1.0,
            meaning no warping is to be done.  The value will be ignored for
            feature types that don't support VLTN, such as spectrogram features.
          chunk_size:
            It specifies the number of frames for each computation.
            If None, it computes features at once (requiring more memory for
            long waves). If not None, each computation takes this number of
            frames (requiring less memory).
        Returns:
          Return a 2-D tensor with as many rows as the input tensor. Its
          number of columns is the number mel bins.
        """
        assert x.ndim == 2
        assert x.dtype == torch.float32
        if chunk_size is None:
            features = self.computer.compute_features(x, vtln_warp)
        else:
            assert chunk_size > 0
            num_chunks = x.size(0) // chunk_size
            end = 0
            features = []
            for i in range(num_chunks):
                start = i * chunk_size
                end = start + chunk_size
                this_chunk = self.computer.compute_features(
                    x[start:end], vtln_warp
                )
                features.append(this_chunk)
            if end < x.size(0):
                last_chunk = self.compute_features(x[end:], vtln_warp)
                features.append(last_chunk)
            features = torch.cat(features, dim=0)

        return features

    def convert_samples_to_frames(self, wave: torch.Tensor) -> torch.Tensor:
        """Convert a 1-D tensor containing audio samples to a 2-D
        tensor where each row is a frame of samples of size frame length
        specified in the options.

        Args:
          waves:
            A 1-D tensor.
        Returns:
          Return a 2-D tensor.
        """
        assert wave.ndim == 1
        assert wave.dtype == torch.float32
        return _kaldifeat.get_strided(wave, self.opts.frame_opts)
