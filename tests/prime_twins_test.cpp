#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "primes.h"

using namespace testing;

TEST(prime_twins, basic)
{
  EXPECT_THAT(prime_twins(31), ElementsAre(Pair(3, 5), Pair(5, 7), Pair(11, 13),
                                           Pair(17, 19), Pair(29, 31)));
}


