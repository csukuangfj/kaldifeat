# Copyright (c) 2020 Fangjun Kuang (csukuangfj@gmail.com)
# See ../LICENSE for clarification regarding multiple authors

# PYTHON_EXECUTABLE is set by cmake/pybind11.cmake
message(STATUS "Python executable: ${PYTHON_EXECUTABLE}")
execute_process(
  COMMAND "${PYTHON_EXECUTABLE}" -c "import os; import torch; print(os.path.dirname(torch.__file__))"
  OUTPUT_STRIP_TRAILING_WHITESPACE
  OUTPUT_VARIABLE TORCH_DIR
)
message(STATUS "TORCH_DIR: ${TORCH_DIR}")

list(APPEND CMAKE_PREFIX_PATH "${TORCH_DIR}")
find_package(Torch REQUIRED)

# set the global CMAKE_CXX_FLAGS so that
# kaldifeat uses the same ABI flag as PyTorch.
# On Windows with clang (not MSVC) TORCH_CXX_FLAGS and the torch/caffe2 imported
# targets' INTERFACE_COMPILE_OPTIONS may contain MSVC-only slash options
# (/permissive-, /EHsc, /bigobj) that clang rejects in GCC-compat mode. Strip
# them from both places.
if(WIN32 AND NOT MSVC)
  # TORCH_CXX_FLAGS may contain MSVC-only slash options (/permissive-, /EHsc,
  # /bigobj) that clang rejects in GCC-compat mode. Strip items starting with /
  # before appending to CMAKE_CXX_FLAGS.
  string(REGEX REPLACE "(^|;| )/[^ ;]+" "" _torch_cxx_flags_filtered "${TORCH_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${_torch_cxx_flags_filtered}")
  # Caffe2Targets.cmake also bakes these MSVC flags into INTERFACE_COMPILE_OPTIONS
  # via generator expressions containing /permissive-, /EHsc, /bigobj, /Z7. The
  # generator expressions have nested ';' separators that cmake splits incorrectly
  # when reading back via get_target_property, making surgical item-level filtering
  # unreliable. Instead clear INTERFACE_COMPILE_OPTIONS on the imported torch
  # targets (clang rejects the MSVC content) and rely on CMAKE_CXX_FLAGS (set
  # above) and INTERFACE_COMPILE_DEFINITIONS for all needed defines. The defines
  # (-D__HIP_PLATFORM_AMD__=1, -DUSE_ROCM, etc.) are already in TORCH_CXX_FLAGS
  # and will be present via CMAKE_CXX_FLAGS after the filter above.
  foreach(_tgt c10 c10_hip torch_cpu torch_cpu_library torch_hip torch_hip_library
               torch torch_library headeronly)
    if(TARGET ${_tgt})
      set_target_properties(${_tgt} PROPERTIES INTERFACE_COMPILE_OPTIONS "")
    endif()
  endforeach()
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TORCH_CXX_FLAGS}")
endif()

# Ensure we use at least the C++ standard that torch requires. Newer torch
# versions (2.13+) set CXX_STANDARD 20 on the torch interface target; building
# with a lower standard produces compilation errors in torch's own headers.
get_target_property(_torch_cxx_std torch CXX_STANDARD)
if(_torch_cxx_std AND CMAKE_CXX_STANDARD AND CMAKE_CXX_STANDARD LESS _torch_cxx_std)
  message(STATUS "Raising CMAKE_CXX_STANDARD from ${CMAKE_CXX_STANDARD} to ${_torch_cxx_std} (torch requirement)")
  set(CMAKE_CXX_STANDARD ${_torch_cxx_std} CACHE STRING "The C++ version to be used." FORCE)
elseif(_torch_cxx_std AND NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD ${_torch_cxx_std} CACHE STRING "The C++ version to be used." FORCE)
endif()

execute_process(
  COMMAND "${PYTHON_EXECUTABLE}" -c "import torch; print(torch.__version__)"
  OUTPUT_STRIP_TRAILING_WHITESPACE
  OUTPUT_VARIABLE TORCH_VERSION
)

message(STATUS "PyTorch version: ${TORCH_VERSION}")

# A ROCm (AMD GPU) PyTorch reports a non-empty torch.version.hip. On such a
# build TORCH_LIBRARIES drags in the static libkineto.a, whose ROCm profiler
# references the HIP runtime (hipRuntimeGetVersion in libamdhip64.so). The
# shared torch libraries do not re-export that symbol, so any executable that
# links kaldifeat_core needs the HIP runtime on its own link line. Detect ROCm
# and expose the runtime so csrc/CMakeLists.txt can link it. The device string
# stays "cuda" as usual; this only fixes linking, not the GPU code path.
execute_process(
  COMMAND "${PYTHON_EXECUTABLE}" -c "import torch; print(torch.version.hip or '')"
  OUTPUT_STRIP_TRAILING_WHITESPACE
  OUTPUT_VARIABLE TORCH_HIP_VERSION
)
if(TORCH_HIP_VERSION)
  message(STATUS "PyTorch is a ROCm build (HIP ${TORCH_HIP_VERSION})")
  set(kaldifeat_TORCH_IS_ROCM TRUE)
  find_library(kaldifeat_HIP_RUNTIME_LIB amdhip64
    HINTS ENV ROCM_PATH /opt/rocm
    PATH_SUFFIXES lib
  )
  if(NOT kaldifeat_HIP_RUNTIME_LIB)
    message(FATAL_ERROR "ROCm PyTorch detected but libamdhip64 was not found; set ROCM_PATH")
  endif()
  message(STATUS "HIP runtime library: ${kaldifeat_HIP_RUNTIME_LIB}")

  # libkineto.a's RocprofLogger references librocprofiler-sdk. The shared
  # libkaldifeat_core.so resolves it transitively, but a unit-test executable
  # that compiles the core sources in directly needs it on its own link line.
  find_library(kaldifeat_ROCPROFILER_SDK_LIB rocprofiler-sdk
    HINTS ENV ROCM_PATH /opt/rocm
    PATH_SUFFIXES lib
  )
endif()

execute_process(
  COMMAND "${PYTHON_EXECUTABLE}" -c "import torch; print(torch.__version__.split('.')[0])"
  OUTPUT_STRIP_TRAILING_WHITESPACE
  OUTPUT_VARIABLE KALDIFEAT_TORCH_VERSION_MAJOR
)

execute_process(
  COMMAND "${PYTHON_EXECUTABLE}" -c "import torch; print(torch.__version__.split('.')[1])"
  OUTPUT_STRIP_TRAILING_WHITESPACE
  OUTPUT_VARIABLE KALDIFEAT_TORCH_VERSION_MINOR
)
