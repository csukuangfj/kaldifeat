# Copyright (c)  2021  Xiaomi Corporation (author: Fangjun Kuang)
if (CMAKE_VERSION VERSION_GREATER_EQUAL "4.0.0")
  set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
endif()

cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(kaldifeat)

# remember to change the version in
# scripts/conda/kaldifeat/meta.yaml
# scripts/conda-cpu/kaldifeat/meta.yaml
set(kaldifeat_VERSION "1.25.5")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(BUILD_RPATH_USE_ORIGIN TRUE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)


if(NOT APPLE)
  set(kaldifeat_rpath_origin "$ORIGIN")
else()
  set(kaldifeat_rpath_origin "@loader_path")
endif()

set(CMAKE_INSTALL_RPATH ${kaldifeat_rpath_origin})
set(CMAKE_BUILD_RPATH ${kaldifeat_rpath_origin})


if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "No CMAKE_BUILD_TYPE given, default to Release")
  set(CMAKE_BUILD_TYPE Release)
endif()

if (NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17 CACHE STRING "The C++ version to be used.")
endif()
message(STATUS "C++ Standard version: ${CMAKE_CXX_STANDARD}")
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

option(BUILD_SHARED_LIBS "Whether to build shared libraries" ON)
option(kaldifeat_BUILD_TESTS "Whether to build tests or not" OFF)
option(kaldifeat_BUILD_PYMODULE "Whether to build python module or not" ON)

message(STATUS "BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS}")

if(BUILD_SHARED_LIBS AND MSVC)
  set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

if(kaldifeat_BUILD_PYMODULE)
  include(pybind11)
endif()
# to prevent cmake from trying to link with system installed mkl since we not directly use it
# mkl libraries should be linked with pytorch already
# ref: https://github.com/pytorch/pytorch/blob/master/cmake/public/mkl.cmake
set(CMAKE_DISABLE_FIND_PACKAGE_MKL TRUE)
include(torch)

if(kaldifeat_BUILD_TESTS)
  include(googletest)
  enable_testing()
endif()


if(WIN32)
  # disable various warnings for MSVC
  # 4624: destructor was implicitly defined as deleted because a base class destructor is inaccessible or deleted
  set(disabled_warnings
      /wd4624
  )
  message(STATUS "Disabled warnings: ${disabled_warnings}")
  foreach(w IN LISTS disabled_warnings)
    string(APPEND CMAKE_CXX_FLAGS " ${w} ")
  endforeach()
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")

message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}")

add_subdirectory(kaldifeat)

# TORCH_VERSION is defined in cmake/torch.cmake
configure_file(
  ${PROJECT_SOURCE_DIR}/kaldifeat/python/kaldifeat/torch_version.py.in
  ${PROJECT_SOURCE_DIR}/kaldifeat/python/kaldifeat/torch_version.py @ONLY
)

configure_file(
  ${PROJECT_SOURCE_DIR}/cmake/kaldifeatConfigVersion.cmake.in
  ${PROJECT_BINARY_DIR}/kaldifeatConfigVersion.cmake
  @ONLY
)

configure_file(
  ${PROJECT_SOURCE_DIR}/cmake/kaldifeatConfig.cmake.in
  ${PROJECT_BINARY_DIR}/kaldifeatConfig.cmake
  @ONLY
)

install(FILES
  ${PROJECT_BINARY_DIR}/kaldifeatConfigVersion.cmake
  ${PROJECT_BINARY_DIR}/kaldifeatConfig.cmake
  DESTINATION share/cmake/kaldifeat
)

install(FILES
  ${PROJECT_SOURCE_DIR}/kaldifeat/python/kaldifeat/torch_version.py
  DESTINATION ./
)
