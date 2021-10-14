add_definitions(-DTORCH_API_INCLUDE_EXTENSION_H)
pybind11_add_module(_kaldifeat
  feature-fbank.cc
  feature-mfcc.cc
  feature-plp.cc
  feature-spectrogram.cc
  feature-window.cc
  kaldifeat.cc
  mel-computations.cc
  utils.cc
)
target_link_libraries(_kaldifeat PRIVATE kaldifeat_core)
if(UNIX AND NOT APPLE)
  target_link_libraries(_kaldifeat PUBLIC ${TORCH_DIR}/lib/libtorch_python.so)
  target_link_libraries(_kaldifeat PUBLIC ${PYTHON_LIBRARY})
endif()
