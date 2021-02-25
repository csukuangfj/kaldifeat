# Copyright (c) 2020 Fangjun Kuang (csukuangfj@gmail.com)
# See ../LICENSE for clarification regarding multiple authors

# PYTHON_EXECUTABLE is set by cmake/pybind11.cmake
message(STATUS "Python executable: ${PYTHON_EXECUTABLE}")
execute_process(
  COMMAND "${PYTHON_EXECUTABLE}" -c "import os; import torch; print(os.path.dirname(torch.__file__))"
  OUTPUT_STRIP_TRAILING_WHITESPACE
  OUTPUT_VARIABLE TORCH_DIR
)

list(APPEND CMAKE_PREFIX_PATH "${TORCH_DIR}")
find_package(Torch REQUIRED)

# set the global CMAKE_CXX_FLAGS so that
# kaldifeat uses the same ABI flag as PyTorch
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")

execute_process(
  COMMAND "${PYTHON_EXECUTABLE}" -c "import torch; print(torch.__version__)"
  OUTPUT_STRIP_TRAILING_WHITESPACE
  OUTPUT_VARIABLE TORCH_VERSION
)

message(STATUS "PyTorch version: ${TORCH_VERSION}")

# Solve the following error for NVCC:
#   unknown option `-Wall`
#
# It contains only some -Wno-* flags, so it is OK
# to set them to empty
set_property(TARGET torch_cuda
  PROPERTY
    INTERFACE_COMPILE_OPTIONS ""
)
set_property(TARGET torch_cpu
  PROPERTY
    INTERFACE_COMPILE_OPTIONS ""
)
