#include "gtest/gtest.h"
#include "matr.h"

TEST(std_matrmul, best) {
  std::vector<std::vector<int>> a = {{2, 3}, {4, 5}, {3, 4}, {5, 6}};
  std::vector<std::vector<int>> b = {{3, 4, 2, 5}, {6, 4, 3, 2}};
  std::vector<std::vector<int>> c = {{24, 20, 13, 16}, {42, 36, 23, 30}, {33, 28, 18, 23}, {51, 44, 28, 37}};
  std::vector<std::vector<int>> t(4, std::vector<int>(4, 0));
  std_matrmul(a, b, t, 4, 2, 2, 4);
  ASSERT_EQ(c, t);
}

TEST(winograd, best) {
  std::vector<std::vector<int>> a = {{2, 3}, {4, 5}, {3, 4}, {5, 6}};
  std::vector<std::vector<int>> b = {{3, 4, 2, 5}, {6, 4, 3, 2}};
  std::vector<std::vector<int>> c = {{24, 20, 13, 16}, {42, 36, 23, 30}, {33, 28, 18, 23}, {51, 44, 28, 37}};
  std::vector<std::vector<int>> t(4, std::vector<int>(4, 0));
  winograd(a, b, t, 4, 2, 2, 4);
  ASSERT_EQ(c, t);
}

TEST(winograd, worst) {
  std::vector<std::vector<int>> a = {{2, 3, 3}, {4, 5, 5}, {3, 4, 4}, {5, 6, 6}};
  std::vector<std::vector<int>> b = {{3, 4, 2, 5}, {6, 4, 3, 2}, {7, 3, 2, 1}};
  std::vector<std::vector<int>> c = {{45, 29, 19, 19}, {77, 51, 33, 35}, {61, 40, 26, 27}, {93, 62, 40, 43}};
  std::vector<std::vector<int>> t(4, std::vector<int>(4, 0));
  winograd(a, b, t, 4, 3, 3, 4);
  ASSERT_EQ(c, t);
}

TEST(winograd_optimized, best) {
  std::vector<std::vector<int>> a = {{2, 3}, {4, 5}, {3, 4}, {5, 6}};
  std::vector<std::vector<int>> b = {{3, 4, 2, 5}, {6, 4, 3, 2}};
  std::vector<std::vector<int>> c = {{24, 20, 13, 16}, {42, 36, 23, 30}, {33, 28, 18, 23}, {51, 44, 28, 37}};
  std::vector<std::vector<int>> t(4, std::vector<int>(4, 0));
  winograd_optimized(a, b, t, 4, 2, 2, 4);
  ASSERT_EQ(c, t);
}

TEST(winograd_optimized, worst) {
  std::vector<std::vector<int>> a = {{2, 3, 3}, {4, 5, 5}, {3, 4, 4}, {5, 6, 6}};
  std::vector<std::vector<int>> b = {{3, 4, 2, 5}, {6, 4, 3, 2}, {7, 3, 2, 1}};
  std::vector<std::vector<int>> c = {{45, 29, 19, 19}, {77, 51, 33, 35}, {61, 40, 26, 27}, {93, 62, 40, 43}};
  std::vector<std::vector<int>> t(4, std::vector<int>(4, 0));
  winograd_optimized(a, b, t, 4, 3, 3, 4);
  ASSERT_EQ(c, t);
}

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
