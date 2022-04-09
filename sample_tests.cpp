#include "header.h"

#include <gtest/gtest.h>

TEST(product, simple) {
    Solution s;
    auto res = s.productExceptSelf({1, 2, 3, 4});
    EXPECT_EQ(res, vector<int>({24, 12, 8, 6}));
}

TEST(product, simple2) {
    Solution s;
    auto res = s.productExceptSelf({1, 2, 3, 4, 5});
    EXPECT_EQ(res, vector<int>({120, 60, 40, 30, 24}));
}

TEST(product, simple3) {
    Solution s;
    auto res = s.productExceptSelf({1, 2, 3, 4, 5, 6});
    EXPECT_EQ(res, vector<int>({720, 360, 240, 180, 144, 120}));
}