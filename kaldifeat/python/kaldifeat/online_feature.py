# Copyright (c)  2022  Xiaomi Corporation (authors: Fangjun Kuang)

from typing import List

import torch


class OnlineFeature(object):
    """Offline feature is a base class of other feature computers,
    e.g., Fbank, Mfcc.

    This class has two fields:

        (1) opts. It contains the options for the feature computer.
        (2) computer. The actual feature computer. It should be
            instantiated by subclasses.

    Caution:
      It supports only CPU at present.
    """

    def __init__(self, opts):
        assert opts.device.type == "cpu"

        self.opts = opts

        # self.computer is expected to be set by subclasses
        self.computer = None

    @property
    def num_frames_ready(self) -> int:
        """Return the number of ready frames.

        It can be updated by :method:`accept_waveform`.

        Note:
          If you set ``opts.frame_opts.max_feature_vectors``, then
          the valid frame indexes are in the range.
          ``[num_frames_ready - max_feature_vectors, num_frames_ready)``

          If you leave ``opts.frame_opts.max_feature_vectors`` to its default
          value, then the range is ``[0, num_frames_ready)``
        """
        return self.computer.num_frames_ready

    def is_last_frame(self, frame: int) -> bool:
        """Return True if the given frame is the last frame."""
        return self.computer.is_last_frame(frame)

    def get_frame(self, frame: int) -> torch.Tensor:
        """Get the frame by its index.
        Args:
          frame:
            The frame index. If ``opts.frame_opts.max_feature_vectors`` is
            -1, then its valid values are in the range
            ``[0, num_frames_ready)``. Otherwise, the range is
            ``[num_frames_ready - max_feature_vectors, num_frames_ready)``.
        Returns:
          Return a 2-D tensor with shape ``(1, feature_dim)``
        """
        return self.computer.get_frame(frame)

    def get_frames(self, frames: List[int]) -> List[torch.Tensor]:
        """Get frames at the given frame indexes.
        Args:
          frames:
            Frames whose indexes are in this list are returned.
        Returns:
          Return a list of feature frames at the given indexes.
        """
        return self.computer.get_frames(frames)

    def accept_waveform(
        self, sampling_rate: float, waveform: torch.Tensor
    ) -> None:
        """Send audio samples to the extractor.
        Args:
          sampling_rate:
            The sampling rate of the given audio samples. It has to be equal
            to ``opts.frame_opts.samp_freq``.
          waveform:
            A 1-D tensor of shape (num_samples,). Its dtype is torch.float32
            and has to be on CPU.
        """
        self.computer.accept_waveform(sampling_rate, waveform)

    def input_finished(self) -> None:
        """Tell the extractor that no more audio samples will be available.
        After calling this function, you cannot invoke ``accept_waveform``
        again.
        """
        self.computer.input_finished()

    @property
    def dim(self) -> int:
        """Return the feature dimension of this extractor"""
        return self.computer.dim

    @property
    def frame_shift_in_seconds(self) -> int:
        """Return frame shift in seconds of this extractor"""
        return self.computer.frame_shift_in_seconds

    def __getstate__(self):
        return self.opts.as_dict()
