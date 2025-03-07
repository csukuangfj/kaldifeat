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

export PATH=$PYTHON_INSTALL_DIR/bin:$PATH
export LD_LIBRARY_PATH=$PYTHON_INSTALL_DIR/lib:$LD_LIBRARY_PATH
ls -lh $PYTHON_INSTALL_DIR/lib/

python3 --version
which python3

python3 -m pip install scikit-build
python3 -m pip install -U pip cmake
python3 -m pip install wheel twine typing_extensions
python3 -m pip install bs4 requests tqdm auditwheel

echo "Installing torch"
python3 -m pip install -qq torch==$TORCH_VERSION || \
python3 -m pip install -qq torch==$TORCH_VERSION -f https://download.pytorch.org/whl/torch_stable.html || \
python3 -m pip install -qq torch==$TORCH_VERSION -f https://download.pytorch.org/whl/torch/

rm -rf ~/.cache/pip
yum clean all

cd /var/www

export CMAKE_CUDA_COMPILER_LAUNCHER=
export KALDIFEAT_CMAKE_ARGS=" -DPYTHON_EXECUTABLE=$PYTHON_INSTALL_DIR/bin/python3 "
export KALDIFEAT_MAKE_ARGS=" -j "

nvcc --version || true
rm -rf /usr/local/cuda*
nvcc --version || true

if [[ x"$IS_2_28" == x"1" ]]; then
  plat=manylinux_2_28_aarch64
else
  plat=manylinux_2_17_aarch64
fi

python3 setup.py bdist_wheel

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
