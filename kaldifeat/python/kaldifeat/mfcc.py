# Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)


import _kaldifeat

from .offline_feature import OfflineFeature
from .online_feature import OnlineFeature


class Mfcc(OfflineFeature):
    def __init__(self, opts: _kaldifeat.MfccOptions):
        super().__init__(opts)
        self.computer = _kaldifeat.Mfcc(opts)


class OnlineMfcc(OnlineFeature):
    def __init__(self, opts: _kaldifeat.MfccOptions):
        super().__init__(opts)
        self.computer = _kaldifeat.OnlineMfcc(opts)

    def __setstate__(self, state):
        self.opts = _kaldifeat.MfccOptions.from_dict(state)
        self.computer = _kaldifeat.OnlineMfcc(self.opts)
