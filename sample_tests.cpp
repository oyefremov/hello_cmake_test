#include <gtest/gtest.h>
#include "header.h"

TEST(maxPoints, sample_test) {
    Solution s;
    vector<vector<int>> points = {{1, 1}, {2, 2}, {3, 3}};
    
    EXPECT_EQ(3, s.maxPoints(points));
}

TEST(maxPoints, sample_test_2) {
    Solution s;
    /* 1 2 3 4 5
    1  .     .
    2      .
    3    .     .
    4  .
    5
    */
    vector<vector<int>> points = { {1, 1}, {3, 2}, {5, 3}, {4, 1}, {2, 3}, {1, 4} };

    EXPECT_EQ(4, s.maxPoints(points));
}

TEST(maxPoints, sample_test_3) {
    Solution s;
    vector<vector<int>> points = { {0, 0}, {2, 2}, {-1, -1} };

    EXPECT_EQ(3, s.maxPoints(points));
}
