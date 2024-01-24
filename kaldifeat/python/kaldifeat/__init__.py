import torch

from .torch_version import kaldifeat_torch_version

if torch.__version__.split("+")[0] != kaldifeat_torch_version.split("+")[0]:
    raise ImportError(
        f"kaldifeat was built using PyTorch {kaldifeat_torch_version}\n"
        f"But you are using PyTorch {torch.__version__} to run it"
    )

from pathlib import Path as _Path

from _kaldifeat import (
    FbankOptions,
    FrameExtractionOptions,
    MelBanksOptions,
    MfccOptions,
    PitchOptions,
    PlpOptions,
    SpectrogramOptions,
    WhisperFbankOptions,
    num_frames,
)

from .fbank import Fbank, OnlineFbank
from .mfcc import Mfcc, OnlineMfcc
from .offline_feature import OfflineFeature
from .online_feature import OnlineFeature
from .pitch import Pitch
from .plp import OnlinePlp, Plp
from .spectrogram import Spectrogram
from .whisper_fbank import WhisperFbank

cmake_prefix_path = _Path(__file__).parent / "share" / "cmake"
del _Path

__version__ = '1.25.4.dev20240124+cpu.torch2.1.1'
__version__ = '1.25.4.dev20240124+cpu.torch2.1.1'
__version__ = '1.25.4.dev20240124+cpu.torch2.1.1'
__version__ = '1.25.4.dev20240124+cpu.torch2.1.1'
__version__ = '1.25.4.dev20240124+cpu.torch2.1.1'
__version__ = '1.25.4.dev20240124+cpu.torch2.1.1'
__version__ = '1.25.4.dev20240124+cpu.torch2.1.1'
__version__ = '1.25.4.dev20240124+cpu.torch2.1.1'
__version__ = '1.25.4.dev20240124+cpu.torch2.1.1'
__version__ = '1.25.4.dev20240124+cpu.torch2.1.1'
