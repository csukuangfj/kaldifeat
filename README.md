# kaldifeat

Feature extraction compatible with kaldi using PyTorch, supporting
CUDA, batch process, and autograd.

See [colab notebook][1] for example usage.

[1]: https://colab.research.google.com/drive/16LXJ5QlqO0mQRYK4q1NxOBdM2yL0CYNJ#scrollTo=dTYi4IVJro0j

# Installation

## From PyPi with pip

If you install `kaldifeat` using `pip`, it will also install
PyTorch 1.8.1. If this is not what you want, please install `kaldifeat`
from source (see below).

```bash
pip install kaldifeat
```

## From source

The following are the commands to compile `kaldifeat` from source.
We assume that you have installed `cmake` and PyTorch.
cmake 3.11 is known to work. Other cmake versions may also work.
PyTorch 1.8.1 is known to work. Other PyTorch versions may also work.

```bash
mkdir /some/path
git clone https://github.com/csukuangfj/kaldifeat.git
cd kaldifeat
python setup.py install
```

To test whether `kaldifeat` was installed successfully, you can run:
```
python3 -c "import kaldifeat; print(kaldifeat.__version__)"
```

## Usage

Please refer to <https://kaldifeat.readthedocs.io/en/latest/usage.html>
for how to use `kaldifeat`.
