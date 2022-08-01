add_definitions(-DTORCH_API_INCLUDE_EXTENSION_H)
pybind11_add_module(_kaldifeat
  feature-fbank.cc
  feature-mfcc.cc
  feature-plp.cc
  feature-spectrogram.cc
  feature-window.cc
  kaldifeat.cc
  mel-computations.cc
  online-feature.cc
  utils.cc
)

if(APPLE)
  execute_process(
    COMMAND "${PYTHON_EXECUTABLE}" -c "from distutils.sysconfig import get_python_lib; print(get_python_lib())"
    OUTPUT_STRIP_TRAILING_WHITESPACE
    OUTPUT_VARIABLE PYTHON_SITE_PACKAGE_DIR
  )
  message(STATUS "PYTHON_SITE_PACKAGE_DIR: ${PYTHON_SITE_PACKAGE_DIR}")
  target_link_libraries(_kaldifeat PRIVATE "-Wl,-rpath,${PYTHON_SITE_PACKAGE_DIR}")
endif()

if(NOT WIN32)
  target_link_libraries(_kaldifeat PRIVATE "-Wl,-rpath,${kaldifeat_rpath_origin}/kaldifeat/${CMAKE_INSTALL_LIBDIR}")
endif()

target_link_libraries(_kaldifeat PRIVATE kaldifeat_core)
if(UNIX AND NOT APPLE)
  target_link_libraries(_kaldifeat PUBLIC ${TORCH_DIR}/lib/libtorch_python.so)
  target_link_libraries(_kaldifeat PUBLIC ${PYTHON_LIBRARY})
elseif(WIN32)
  target_link_libraries(_kaldifeat PUBLIC ${TORCH_DIR}/lib/torch_python.lib)
  target_link_libraries(_kaldifeat PUBLIC ${PYTHON_LIBRARIES})
endif()

install(TARGETS _kaldifeat
  DESTINATION ../
)
