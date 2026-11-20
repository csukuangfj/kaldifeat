# Copyright (c)  2021  Xiaomi Corporation (author: Fangjun Kuang)

set(kaldifeat_srcs
  feature-fbank.cc
  feature-functions.cc
  feature-mfcc.cc
  feature-plp.cc
  feature-spectrogram.cc
  feature-window.cc
  matrix-functions.cc
  mel-computations.cc
  online-feature.cc
  whisper-fbank.cc
)

add_library(kaldifeat_core ${kaldifeat_srcs})
target_link_libraries(kaldifeat_core PUBLIC ${TORCH_LIBRARIES})

# On a ROCm PyTorch, TORCH_LIBRARIES pulls in the static libkineto.a whose ROCm
# profiler needs the HIP runtime; link it so downstream executables resolve it
# (kaldifeat_TORCH_IS_ROCM / kaldifeat_HIP_RUNTIME_LIB set in cmake/torch.cmake).
if(kaldifeat_TORCH_IS_ROCM)
  target_link_libraries(kaldifeat_core PUBLIC ${kaldifeat_HIP_RUNTIME_LIB})
endif()

target_compile_definitions(kaldifeat_core PUBLIC KALDIFEAT_TORCH_VERSION_MAJOR=${KALDIFEAT_TORCH_VERSION_MAJOR})
target_compile_definitions(kaldifeat_core PUBLIC KALDIFEAT_TORCH_VERSION_MINOR=${KALDIFEAT_TORCH_VERSION_MINOR})

if(APPLE)
  execute_process(
    COMMAND "${PYTHON_EXECUTABLE}" -c "from distutils.sysconfig import get_python_lib; print(get_python_lib())"
    OUTPUT_STRIP_TRAILING_WHITESPACE
    OUTPUT_VARIABLE PYTHON_SITE_PACKAGE_DIR
  )
  message(STATUS "PYTHON_SITE_PACKAGE_DIR: ${PYTHON_SITE_PACKAGE_DIR}")
  target_link_libraries(kaldifeat_core PUBLIC "-L ${PYTHON_SITE_PACKAGE_DIR}/../..")
endif()

add_executable(test_kaldifeat test_kaldifeat.cc)
target_link_libraries(test_kaldifeat PRIVATE kaldifeat_core)

function(kaldifeat_add_test source)
  get_filename_component(name ${source} NAME_WE)
  add_executable(${name} "${source}" $<TARGET_OBJECTS:kaldifeat_core_obj>)
  target_link_libraries(${name}
    PRIVATE
      ${TORCH_LIBRARIES}
      gtest
      gtest_main
  )
  if(kaldifeat_TORCH_IS_ROCM)
    target_link_libraries(${name} PRIVATE ${kaldifeat_HIP_RUNTIME_LIB})
    if(kaldifeat_ROCPROFILER_SDK_LIB)
      target_link_libraries(${name} PRIVATE ${kaldifeat_ROCPROFILER_SDK_LIB})
    endif()
  endif()

  # NOTE: We set the working directory here so that
  # it works also on windows. The reason is that
  # the required DLLs are inside ${TORCH_DIR}/lib
  # and they can be found by the exe if the current
  # working directory is ${TORCH_DIR}\lib
  add_test(NAME "Test.${name}"
    COMMAND
    $<TARGET_FILE:${name}>
    WORKING_DIRECTORY ${TORCH_DIR}/lib
  )
endfunction()

if(kaldifeat_BUILD_TESTS)
  # Source-level core for the unit-test executables. On a ROCm PyTorch, linking
  # a test exe against the SHARED libkaldifeat_core.so makes the ROCm profiler
  # (librocprofiler-sdk, pulled in by torch's static libkineto.a) recursively
  # dlopen that .so during its own one-time static init, which aborts with
  # __gnu_cxx::recursive_init_error before any test runs. The python module is
  # unaffected (it is imported into an already-initialized process). Compiling
  # the core sources into each test exe avoids the startup dlopen while leaving
  # the shared kaldifeat_core (used by the python module and install) unchanged.
  # On CUDA/CPU builds this is an equivalent static link.
  add_library(kaldifeat_core_obj OBJECT ${kaldifeat_srcs})
  target_compile_definitions(kaldifeat_core_obj PUBLIC
    KALDIFEAT_TORCH_VERSION_MAJOR=${KALDIFEAT_TORCH_VERSION_MAJOR}
    KALDIFEAT_TORCH_VERSION_MINOR=${KALDIFEAT_TORCH_VERSION_MINOR})
  target_include_directories(kaldifeat_core_obj PUBLIC ${TORCH_INCLUDE_DIRS})

  # please sort the source files alphabetically
  set(test_srcs
    feature-window-test.cc
    online-feature-test.cc
  )

  foreach(source IN LISTS test_srcs)
    kaldifeat_add_test(${source})
  endforeach()
endif()

file(MAKE_DIRECTORY
  DESTINATION
    ${PROJECT_BINARY_DIR}/include/kaldifeat/csrc
)

file(GLOB_RECURSE all_headers *.h)
message(STATUS "All headers: ${all_headers}")

file(COPY
  ${all_headers}
  DESTINATION
    ${PROJECT_BINARY_DIR}/include/kaldifeat/csrc
)
if(BUILD_SHARED_LIBS AND WIN32)
  install(TARGETS kaldifeat_core
    DESTINATION ../
  )
endif()
install(TARGETS kaldifeat_core
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(FILES ${all_headers}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/kaldifeat/csrc
)
