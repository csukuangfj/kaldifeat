# Copyright (c)  2024             (authors: Feiteng Li)


import _kaldifeat

from .offline_feature import OfflineFeature


class Pitch(OfflineFeature):
    def __init__(self, opts: _kaldifeat.FbankOptions):
        super().__init__(opts)
        self.computer = _kaldifeat.Pitch(opts)

