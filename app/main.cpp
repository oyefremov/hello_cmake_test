#include "primes.h"
#include <iomanip>
#include <iostream>

int main() {

  int step = 200;
  int line = step;
  for (auto [a, b] : prime_twins(10000)) {
    if (a > line) {
      line += step;
      std::cout << "\n";
    }
    std::cout << a << "-" << b << " ";
  }
  std::cout << "\n";
}
