# Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)


import _kaldifeat

from .offline_feature import OfflineFeature


class Spectrogram(OfflineFeature):
    def __init__(self, opts: _kaldifeat.SpectrogramOptions):
        super().__init__(opts)
        self.computer = _kaldifeat.Spectrogram(opts)
