set(kaldifeat_srcs
  feature-window.cc
  mel-computations.cc
)

add_library(kaldifeat_core SHARED ${kaldifeat_srcs})
target_link_libraries(kaldifeat_core PUBLIC ${TORCH_LIBRARIES})
