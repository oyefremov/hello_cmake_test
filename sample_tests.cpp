#include <gtest/gtest.h>

TEST(sample_tests, sample_test) {
    EXPECT_EQ(1, 1);
}

TEST(sample_tests, failed_test) {
    EXPECT_EQ(1, 2);
}