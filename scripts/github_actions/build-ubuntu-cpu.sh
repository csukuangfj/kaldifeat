#!/usr/bin/env bash
#
set -ex

if [ -z $PYTHON_VERSION ]; then
  echo "Please set the environment variable PYTHON_VERSION"
  echo "Example: export PYTHON_VERSION=3.8"
  # Valid values: 3.8, 3.9, 3.10, 3.11
  exit 1
fi

if [ -z $TORCH_VERSION ]; then
  echo "Please set the environment variable TORCH_VERSION"
  echo "Example: export TORCH_VERSION=1.10.0"
  exit 1
fi

if [ -z "$PYTHON_INSTALL_DIR" ]; then
  echo "Please set the environment variable PYTHON_INSTALL_DIR"
  echo "Example: export PYTHON_INSTALL_DIR=/opt/python/cp310-cp310"
  exit 1
fi

export PATH=$PYTHON_INSTALL_DIR/bin:$PATH
export LD_LIBRARY_PATH=$PYTHON_INSTALL_DIR/lib:$LD_LIBRARY_PATH
ls -lh $PYTHON_INSTALL_DIR/lib/

python3 --version
which python3

python3 -m pip install scikit-build
# The old pinned manylinux images contain CPython 3.12.0rc1 as cp312. Its re
# engine has a possessive-quantifier bug (fixed in 3.11.5 and 3.12.0 final,
# see https://github.com/python/cpython/pull/107795), but packaging >= 26
# (vendored by pip >= 26) guards only CPython < 3.11.5 and then its
# possessive-quantifier version regex fails to match on such interpreters,
# breaking pip and tools using packaging (e.g. auditwheel) with
# InvalidVersion errors. Constrain pip and packaging below 26 there.
if python3 -c 'import sys; sys.exit(0 if sys.version_info[:2] == (3, 12) and sys.version_info.releaselevel != "final" else 1)'; then
  echo "pip<26" > /tmp/pip-constraint.txt
  echo "packaging<26" >> /tmp/pip-constraint.txt
  export PIP_CONSTRAINT=/tmp/pip-constraint.txt
fi
python3 -m pip install -U pip cmake
python3 -m pip install wheel twine typing_extensions
python3 -m pip install bs4 requests tqdm auditwheel

echo "Installing torch"
python3 -m pip install -qq torch==$TORCH_VERSION+cpu -f https://download.pytorch.org/whl/torch_stable.html || \
python3 -m pip install -qq torch==$TORCH_VERSION+cpu -f https://download.pytorch.org/whl/torch/

# Prefer the libraries bundled inside the installed torch wheel. The
# manylinux2_28 builder images have an Arm Compute Library build (e.g.
# /acl/build) in LD_LIBRARY_PATH which they use to build pytorch itself; it
# shadows the ACL bundled inside the torch wheel and breaks `import torch`
# for torch versions built against a different ACL ABI (e.g. 2.4/2.5).
TORCH_LIB_DIR=$(python3 -c "import sysconfig; print(sysconfig.get_paths()['purelib'] + '/torch/lib')")
export LD_LIBRARY_PATH=$TORCH_LIB_DIR:$LD_LIBRARY_PATH

rm -rf ~/.cache/pip
yum clean all

cd /var/www

export CMAKE_CUDA_COMPILER_LAUNCHER=
export KALDIFEAT_CMAKE_ARGS=" -DPYTHON_EXECUTABLE=$PYTHON_INSTALL_DIR/bin/python3 "
export KALDIFEAT_MAKE_ARGS=" -j "

nvcc --version || true
rm -rf /usr/local/cuda*
nvcc --version || true

python3 setup.py bdist_wheel
if [[ x"$IS_2_28" == x"1" ]]; then
  plat=manylinux_2_28_x86_64
else
  plat=manylinux_2_17_x86_64
fi

auditwheel --verbose repair \
  --exclude libc10.so \
  --exclude libc10_cuda.so \
  --exclude libcuda.so.1 \
  --exclude libcudart.so.${CUDA_VERSION} \
  --exclude libnvToolsExt.so.1 \
  --exclude libnvrtc.so.${CUDA_VERSION} \
  --exclude libtorch.so \
  --exclude libtorch_cpu.so \
  --exclude libtorch_cuda.so \
  --exclude libtorch_python.so \
  \
  --exclude libcudnn.so.8 \
  --exclude libcublas.so.11 \
  --exclude libcublasLt.so.11 \
  --exclude libcudart.so.11.0 \
  --exclude libnvrtc.so.11.2 \
  --exclude libtorch_cuda_cu.so \
  --exclude libtorch_cuda_cpp.so \
  --plat $plat \
  dist/*.whl

ls -lh  /var/www
