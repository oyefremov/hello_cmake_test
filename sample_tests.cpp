#include <gtest/gtest.h>
#include "header.h"

TEST(median, sample_test) {
    MedianFinder mf;
    mf.addNum(1);
    mf.addNum(2);
    mf.addNum(3);
    mf.addNum(4);

    EXPECT_EQ(2.5, mf.findMedian());
}

TEST(median, sample_test2) {
    MedianFinder mf;
    mf.addNum(1);
    mf.addNum(2);

    EXPECT_EQ(1.5, mf.findMedian());
}

TEST(median, sample_test3) {
    MedianFinder mf;
    mf.addNum(1);
    mf.addNum(2);
    mf.addNum(3);

    EXPECT_EQ(2, mf.findMedian());
}

TEST(median, in_histogram){
    MedianFinder mf;
    for (int i=-10; i<=50; i++) {
        for (int j=0; j<=10; j++) {
            mf.addNum(i);
        }
    }
    EXPECT_EQ((-10+50)/2, mf.findMedian());
}

TEST(median, in_negative){
    MedianFinder mf;
    for (int i=-110; i<=50; i++) {
        for (int j=0; j<=10; j++) {
            mf.addNum(i);
        }
    }
    EXPECT_EQ((-110+50)/2, mf.findMedian());
}

TEST(median, in_negative_simple) {
    MedianFinder mf;
    for (int i = -20; i <= 10; i++) {
        mf.addNum(i);
    }
    EXPECT_EQ((-20 + 10) / 2, mf.findMedian());
}

TEST(median, in_negative_simple_2) {
    MedianFinder mf;
    for (int i = -21; i <= 10; i++) {
        mf.addNum(i);
    }
    EXPECT_EQ((-21 + 10) / 2.0, mf.findMedian());
}

TEST(median, in_large){
    MedianFinder mf;
    for (int i=-10; i<=1000; i++) {
        for (int j=0; j<=10; j++) {
            mf.addNum(i);
        }
    }
    EXPECT_EQ((-10+1000)/2, mf.findMedian());
}
