add_definitions(-DTORCH_API_INCLUDE_EXTENSION_H)
pybind11_add_module(_kaldifeat
  feature-fbank.cc
  feature-mfcc.cc
  feature-window.cc
  kaldifeat.cc
  mel-computations.cc
)
target_link_libraries(_kaldifeat PRIVATE kaldifeat_core)
target_link_libraries(_kaldifeat PRIVATE ${TORCH_DIR}/lib/libtorch_python.so)
