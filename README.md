# kaldifeat

Wrap kaldi's feature computations to Python with PyTorch support.

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
