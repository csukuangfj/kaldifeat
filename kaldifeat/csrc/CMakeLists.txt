set(kaldifeat_srcs
  feature-fbank.cc
  feature-window.cc
  mel-computations.cc
)

add_library(kaldifeat_core SHARED ${kaldifeat_srcs})
target_link_libraries(kaldifeat_core PUBLIC ${TORCH_LIBRARIES})

add_executable(test_kaldifeat test_kaldifeat.cc)
target_link_libraries(test_kaldifeat PRIVATE kaldifeat_core)
