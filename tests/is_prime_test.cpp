#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "primes.h"

using namespace testing;

TEST(is_prime, basic) {
  for (int p : {2, 3, 5, 7}) {
    EXPECT_TRUE(is_prime(p)) << "p=" << p;
  }
}

TEST(is_prime, negative) {
  for (int composite : {0, 1, 4, 9, 27, 81, 100}) {
    EXPECT_FALSE(is_prime(composite)) << "composite=" << composite;
  }
}
