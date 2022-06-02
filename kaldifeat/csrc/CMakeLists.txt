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
)

add_library(kaldifeat_core ${kaldifeat_srcs})
target_link_libraries(kaldifeat_core PUBLIC ${TORCH_LIBRARIES})

if(EXISTS /usr/local/miniconda/envs/kaldifeat/lib/libmkl_intel_ilp64.dylib)
  message(STATUS "Linking kaldifeat_core with /usr/local/miniconda/envs/kaldifeat/lib/libmkl_intel_ilp64.dylib")
  target_link_libraries(kaldifeat_core PUBLIC "/usr/local/miniconda/envs/kaldifeat/lib/libmkl_intel_ilp64.dylib")
  target_link_libraries(kaldifeat_core PUBLIC "-L/usr/local/miniconda/envs/kaldifeat/lib")
else()
  message(STATUS "Not linking kaldifeat_core with /usr/local/miniconda/envs/kaldifeat/lib/libmkl_intel_ilp64.dylib")
endif()

target_compile_definitions(kaldifeat_core PUBLIC KALDIFEAT_TORCH_VERSION_MAJOR=${KALDIFEAT_TORCH_VERSION_MAJOR})
target_compile_definitions(kaldifeat_core PUBLIC KALDIFEAT_TORCH_VERSION_MINOR=${KALDIFEAT_TORCH_VERSION_MINOR})

add_executable(test_kaldifeat test_kaldifeat.cc)
target_link_libraries(test_kaldifeat PRIVATE kaldifeat_core)

function(kaldifeat_add_test source)
  get_filename_component(name ${source} NAME_WE)
  add_executable(${name} "${source}")
  target_link_libraries(${name}
    PRIVATE
      kaldifeat_core
      gtest
      gtest_main
  )

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
  # please sort the source files alphabetically
  set(test_srcs
    feature-window-test.cc
    online-feature-test.cc
  )

  foreach(source IN LISTS test_srcs)
    kaldifeat_add_test(${source})
  endforeach()

endif()
