// kaldifeat/csrc/online-feature-test.h
//
// Copyright (c)  2022  Xiaomi Corporation (authors: Fangjun Kuang)

#include "kaldifeat/csrc/online-feature.h"

#include "gtest/gtest.h"

namespace kaldifeat {

TEST(RecyclingVector, TestUnlimited) {
  RecyclingVector v(-1);
  constexpr int32_t N = 100;
  for (int32_t i = 0; i != N; ++i) {
    torch::Tensor t = torch::tensor({i, i + 1, i + 2});
    v.PushBack(t);
  }
  ASSERT_EQ(v.Size(), N);

  for (int32_t i = 0; i != N; ++i) {
    torch::Tensor t = v.At(i);
    torch::Tensor expected = torch::tensor({i, i + 1, i + 2});
    EXPECT_TRUE(t.equal(expected));
  }
}

TEST(RecyclingVector, Testlimited) {
  constexpr int32_t K = 3;
  constexpr int32_t N = 10;
  RecyclingVector v(K);
  for (int32_t i = 0; i != N; ++i) {
    torch::Tensor t = torch::tensor({i, i + 1, i + 2});
    v.PushBack(t);
  }

  ASSERT_EQ(v.Size(), N);

  for (int32_t i = 0; i < N - K; ++i) {
    ASSERT_DEATH(v.At(i), "");
  }

  for (int32_t i = N - K; i != N; ++i) {
    torch::Tensor t = v.At(i);
    torch::Tensor expected = torch::tensor({i, i + 1, i + 2});
    EXPECT_TRUE(t.equal(expected));
  }
}

}  // namespace kaldifeat
