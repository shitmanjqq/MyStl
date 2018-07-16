#include <iostream>
#include "pair.h"
#include <utility>

struct Test {
  constexpr Test() = default;
  Test(const Test &) {
    std::cout << "Test::copy" << std::endl;
  }
  Test(Test &&) {
    std::cout << "Test::move" << std::endl;
  }
};

struct Derived : Test {
  constexpr Derived() = default;
  Derived(const Derived &) {
    std::cout << "Derived::copy" << std::endl;
  }
  Derived(Derived &&) {
    std::cout << "Derived::move" << std::endl;
  }
};

int main() {
  MyStl::pair<int, float> p1;
  std::cout << "Value-initialized: "
            << p1.first << ", " << p1.second << '\n';

  MyStl::pair<int, double> p2(42, 0.123);
  std::cout << "Initialized with two values: "
            << p2.first << ", " << p2.second << '\n';

  MyStl::pair<char, int> p4(p2);
  std::cout << "Implicitly converted: "
            << p4.first << ", " << p4.second << '\n';
  // auto cp = std::move(p);
}