# Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)


import _kaldifeat

from .offline_feature import OfflineFeature


class Plp(OfflineFeature):
    def __init__(self, opts: _kaldifeat.PlpOptions):
        super().__init__(opts)
        self.computer = _kaldifeat.Plp(opts)
