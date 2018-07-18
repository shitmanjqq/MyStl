#include <iostream>
#include "pair.h"
#include <utility>
#include <tuple>
#include "type_traits.h"
#include "tuple.h"

struct Base {
  Base() {
    std::cout << "Base::default" << std::endl;
  };
  Base(const Base &) {
    std::cout << "Base::copy" << std::endl;
  }
  Base(Base &&) {
    std::cout << "Base::move" << std::endl;
  };
};

struct Derived : Base {
  Derived() {
    std::cout << "Derived::default" << std::endl;
  };
  Derived(const Derived &) {
    std::cout << "Derived::copy" << std::endl;
  }
  Derived(Derived &&) {
    std::cout << "Derived::move" << std::endl;
  };
};

// struct A;
// void helper(A);

// struct A final {
//   A(int = 3) {}
//   virtual ~A() {}

//   // template <typename T, typename = decltype(helper(std::declval<int>()))>
//   // void test(T);
// };

struct Test {
  Test() {
    std::cout << "Test::default" << std::endl;
  }

  Test(const Test &) {
    std::cout << "Test::copy" << std::endl;
  }

  Test(Test &&) {
    std::cout << "Test::move" << std::endl;
  }

  Test &operator=(const Test &) {
    std::cout << "Test::copy =" << std::endl;
  }

  Test &operator=(Test &&) {
    std::cout << "Test::move =" << std::endl;
  }
};

int main() {
  std::cout << std::boolalpha;

  Derived d;
  std::cout << "std" << std::endl;
  std::_Head_base<0, Base> sh{std::move(d)};
  std::cout << "MyStl" << std::endl;
  MyStl::Head_base<0, Base> mh{std::move(d)};

  // std::cout << MyStl::is_convertible<const A &, A>::value << std::endl;
  // std::cout << std::is_convertible<volatile A &, A>::value << std::endl;

  // std::tuple<const A> t1;
  // std::tuple<A> t2{t1};
  // Base &&rt = Derived{};
  // Derived d;

  // constexpr Derived cd = std::move(d);

  // std::tuple<Base &&>{Base{}};
  // std::tuple<Base>{std::tuple<Base &&>{Base{}}};
  // int arr[3];
  // std::tuple<int [3]> t;
  // std::get<0>(t)[0] = 3;

  // std::cout << __is_empty(A) << std::endl;
  // std::cout << __is_final(A) << std::endl;

  // A a{2};
  // const A &ra = 3;
  // a.test(3);
  // std::cout << MyStl::is_void<void volatile >::value << std::endl;
  // std::cout << MyStl::OR<B, B, B, A>::value << std::endl;
  // MyStl::pair<int, float> p1;
  // std::cout << "Value-initialized: "
  //           << p1.first << ", " << p1.second << '\n';

  // MyStl::pair<int, double> p2(42, 0.123);
  // std::cout << "Initialized with two values: "
  //           << p2.first << ", " << p2.second << '\n';

  // MyStl::pair<char, int> p4(p2);
  // std::cout << "Implicitly converted: "
  //           << p4.first << ", " << p4.second << '\n';
  // auto cp = std::move(p);

  // std::get<0>(std::tuple<int>{}) = 3;
  // std::tuple<int, int, int> ti;
  // std::tuple<A, A> ta{ti};
  // MyStl::Require<A, A, A, B> a;
  // A a;
  // test(a, a);
  // B b;
  // test(a, b);
}