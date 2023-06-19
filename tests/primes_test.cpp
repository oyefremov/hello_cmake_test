#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "primes.h"

using namespace testing;

TEST(primes, basic)
{
    EXPECT_THAT(primes(10), ElementsAre(2, 3, 5, 7));
}

TEST(primes, 100)
{
    EXPECT_THAT(primes(100), SizeIs(25));
}

TEST(primes, 1000)
{
    EXPECT_THAT(primes(1000), SizeIs(168));
}
