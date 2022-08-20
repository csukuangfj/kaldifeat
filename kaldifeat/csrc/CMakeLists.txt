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

install(TARGETS kaldifeat_core
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
install(FILES ${all_headers}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/kaldifeat/csrc
)
