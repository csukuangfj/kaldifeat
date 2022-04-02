# Copyright (c)  2021  Xiaomi Corporation (authors: Fangjun Kuang)


import _kaldifeat

from .offline_feature import OfflineFeature
from .online_feature import OnlineFeature


class Plp(OfflineFeature):
    def __init__(self, opts: _kaldifeat.PlpOptions):
        super().__init__(opts)
        self.computer = _kaldifeat.Plp(opts)


class OnlinePlp(OnlineFeature):
    def __init__(self, opts: _kaldifeat.PlpOptions):
        super().__init__(opts)
        self.computer = _kaldifeat.OnlinePlp(opts)

    def __setstate__(self, state):
        self.opts = _kaldifeat.PlpOptions.from_dict(state)
        self.computer = _kaldifeat.OnlinePlp(self.opts)
