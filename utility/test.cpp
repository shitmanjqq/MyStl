#include <iostream>
#include "pair.h"
#include <utility>
#include <tuple>
#include "type_traits.h"
#include "tuple.h"

template <typename>
struct tell_type;

template <std::size_t>
struct tell_compile_const;

struct Base {
  Base() {
    std::cout << "Base::default" << std::endl;
  }
  Base(const Base &) {
    std::cout << "Base::copy" << std::endl;
  }
  Base(Base &&) {
    std::cout << "Base::move" << std::endl;
  }

  void func() const && {}
};

struct Derived : Base {
  Derived() {
    std::cout << "Derived::default" << std::endl;
  }
  Derived(const Derived &d) : Base{d} {
    std::cout << "Derived::copy" << std::endl;
  }
  Derived(Derived &&d) : Base{std::move(d)} {
    std::cout << "Derived::move" << std::endl;
  }
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
  explicit Test() {
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

  Test(int) {
    std::cout << "------------" << std::endl;
  }

  // // Base b;
  int i;
};

template <int ... NS>
struct index {};

template <int N>
struct make_index {
  template <int ... NS>
  static index<NS ..., sizeof...(NS)>
  test(index<NS ...>);

  using type = decltype(test((typename make_index<N - 1>::type){}));
};

template <>
struct make_index<0> {
  static index<> test(index<>);

  using type = index<>;
};

// template <int ... NS>
// index<NS ...> test(index<NS ...>);

// template <int ... NS>
// auto test(index<NS ...>) -> decltype(test(index<NS ..., sizeof...(NS)>));

struct A {
  constexpr A(int i = 3) : i{i} {}
  constexpr operator int() {
    return i;
  }

  void func();

  int i;
};

A ga{33};

int main() {
  std::cout << std::boolalpha;

  Test t;
  Base b;
  Derived d;
  int i;

  std::cout << "std" << std::endl;
  // std::_Tuple_impl<0, const Test, Base, Derived, int> st;
  // std::cout << "MyStl" << std::endl;
  // MyStl::tuple_impl<0, const Test, Base, Derived, int> mt;

  // tell_compile_const<MyStl::get<1>(MyStl::tuple<int, A &, int>{1, ga, 3})> ma;
  // tell_compile_const<std::get<1>(std::tuple<int, A &, int>{1, ga, 3})> sa;
  // tell_compile_const<std::get<1>(std::tuple<int, A, int>{1, 23, 3})> a;

  const std::tuple<int, A &, int> cst{1, ga, 3};
  std::get<1>(cst).func();
  const MyStl::tuple<int, A &, int> cmt{1, ga, 3};
  MyStl::get<1>(cmt).func();
  // constexpr A a{};
  std::get<1>(std::tuple<int, A &, int>{1, ga, 3}) = A{};
  MyStl::get<1>(MyStl::tuple<int, A &, int>{1, ga, 3}) = A{};

  // func({});
  // std::cout << std::__is_implicitly_default_constructible<A>::value << std::endl;
  // std::cout << MyStl::is_implicitly_default_constructible<A>::value << std::endl;
  // decltype(void()) ii;
  // const Test ct;

  // MyStl::tuple<int, Derived, int> st;
  // static_cast<std::_Tuple_impl<0, Test, Base, Test> &>(st);
  // MyStl::tuple_impl<0, Test, Base, Test> ff = st;

  // tell_type<MyStl::tuple_elem<0, MyStl::tuple<const int &&>>::type> a0;
  // tell_type<MyStl::tuple_elem<1, decltype(st)>::type> a1;
  // tell_type<MyStl::tuple_elem<2, decltype(st)>::type> a2;
  // MyStl::tuple_elem<3, decltype(st)>::type a3;

  // tell_type<MyStl::build_index_tuple_v2<10>::type> a;

  // MyStl::remove_all_extents<int [][1][2][3]>::type ii;
  // using func = int();
  // std::cout << MyStl::is_default_constructible<Test [2]>::value << std::endl;
  // std::cout << std::is_default_constructible<Test []>::value << std::endl;

  // int arr[];
  // int arr[10];
  // std::tuple<int [10]> st{arr};
  // Derived cd = d;

  // sh._M_head(sh).func();
  // mh.get_head(mh);

  // Base & rb = sh._M_head(sh);
  // mh.get_head(mh);

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