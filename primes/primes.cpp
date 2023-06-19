#include "primes.h"

bool is_prime(int n) {
  if (n < 11) {
    switch (n) {
    case 2:
    case 3:
    case 5:
    case 7:
      return true;
    }
    return false;
  }
  
  for (int i = 3;; i += 2) {
    auto reminder = n % i;
    if (reminder == 0) {
      return false;
    }
    auto q = n / i;
    if (q <= i) {
      return true;
    }
  }
}

std::vector<int> primes(int max) {
  std::vector<int> result;

  if (max < 2) {
    return result;
  }
  result.push_back(2);
  for (int i = 3; i <= max; i += 2) {
    if (is_prime(i))
      result.push_back(i);
  }
  return result;
}

std::vector<std::pair<int, int>> prime_twins(int max) {
  std::vector<std::pair<int, int>> result;
  for (int i = 3; i <= max; i += 2) {
    if (is_prime(i) && is_prime(i + 2))
      result.emplace_back(i, i + 2);
  }
  return result;
}
