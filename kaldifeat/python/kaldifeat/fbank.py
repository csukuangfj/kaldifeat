# Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)


import _kaldifeat

from .offline_feature import OfflineFeature
from .online_feature import OnlineFeature


class Fbank(OfflineFeature):
    def __init__(self, opts: _kaldifeat.FbankOptions):
        super().__init__(opts)
        self.computer = _kaldifeat.Fbank(opts)


class OnlineFbank(OnlineFeature):
    def __init__(self, opts: _kaldifeat.FbankOptions):
        super().__init__(opts)
        self.computer = _kaldifeat.OnlineFbank(opts)

    def __setstate__(self, state):
        self.opts = _kaldifeat.FbankOptions.from_dict(state)
        self.computer = _kaldifeat.OnlineFbank(self.opts)
