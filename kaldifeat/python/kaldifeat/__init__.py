import torch

from .torch_version import kaldifeat_torch_version

if torch.__version__.split("+")[0] != kaldifeat_torch_version.split("+")[0]:
    raise ImportError(
        f"kaldifeat was built using PyTorch {kaldifeat_torch_version}\n"
        f"But you are using PyTorch {torch.__version__} to run it"
    )

from _kaldifeat import (
    FbankOptions,
    FrameExtractionOptions,
    MelBanksOptions,
    MfccOptions,
    PlpOptions,
    SpectrogramOptions,
)

from .fbank import Fbank, OnlineFbank
from .mfcc import Mfcc, OnlineMfcc
from .offline_feature import OfflineFeature
from .online_feature import OnlineFeature
from .plp import OnlinePlp, Plp
from .spectrogram import Spectrogram

from pathlib import Path as _Path

cmake_prefix_path = _Path(__file__).parent / "share" / "cmake"
del _Path
