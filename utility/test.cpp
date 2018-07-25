/* The following code example is taken from the book
 * "The C++ Standard Library - A Tutorial and Reference, 2nd Edition"
 * by Nicolai M. Josuttis, Addison-Wesley, 2012
 *
 * (C) Copyright Nicolai M. Josuttis 2012.
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 */
#include <iostream>
#include <algorithm>
#include <utility>
#include <tuple>
#include <functional>
#include <vector>
// using namespace std;
#include "pair.h"
#include "tuple.h"

int gi = 3;

class Foo
{
  public:
    Foo(int i = 23) : i_{i} {
        std::cout << "default" << std::endl;
    }

    Foo(const Foo &f) : i_{f.i_} {
        std::cout << "copy" << std::endl;
    }

    Foo(Foo &&f) {
        MyStl::swap(i_, f.i_);
        std::cout << "move" << std::endl;
    }

    Foo &operator=(Foo &&f) {
        MyStl::swap(i_, f.i_);
        std::cout << "move =" << std::endl;
    }

    friend std::ostream &operator<<(std::ostream &os, const Foo &f) {
        return os << f.i_;
    }

    ~Foo() {
        std::cout << "~Foo" << std::endl;
    }

  private:
    int i_;
};

struct A {};
struct B : A {};

int main()
{
    std::cout << std::boolalpha;
    std::string s = "hello world";
    auto f1 = MyStl::make_tuple(s);
    auto f2 = MyStl::make_tuple(2.3);
    auto f3 = MyStl::make_tuple(f1, f2);
    // std::cout << MyStl::get<0>(MyStl::get<0>(f3)) << std::endl;
    // std::cout << MyStl::get<1>(MyStl::get<0>(f3)) << std::endl;
    // std::cout << MyStl::get<0>(MyStl::get<1>(f3)) << std::endl;
    // MyStl::tell_type<decltype(MyStl::make_tuple(f2))> a;

    // std::cout << MyStl::get<0>(f1) << std::endl;
    // auto f4 = std::move(f1);
    // std::cout << MyStl::get<0>(f1) << std::endl;
    // std::cout << MyStl::get<0>(f4) << std::endl;

    // std::cout << MyStl::is_base_of<A, const A>::value << std::endl;
    // auto f1 = std::make_tuple(s, 23);
    // auto f2 = std::make_tuple(2.3);
    // auto f3 = std::make_tuple(f2);

    // MyStl::tuple<int> t1{3};
    // std::tuple<std::tuple<int>> t2{t1};

    // auto t3 = t1;
    // std::cout << MyStl::get<0>(t3) << std::endl;

    // std::cout << MyStl::if_copy_constructed<A, const A &, int>::value << std::endl;

    // MyStl::tell_type<decltype(f3)> a;
    // std::cout << MyStl::get<1>(f1) << std::endl;
    // auto f3 = MyStl::tuple_cat(std::move(f1), f2);
    // std::cout << MyStl::get<1>(f1) << std::endl;

    // std::cout << MyStl::get<1>(f3) << std::endl;
    // auto f4 = MyStl::tuple_cat(f1, f2, std::move(f3));
    // std::cout << MyStl::get<1>(f3) << std::endl;
    // std::cout << 
    // MyStl::tell_type<MyStl::remove_cv_and_reference_t<const volatile int &&>> a;

    // TT::i = 3;
    // int i;
    // const int j = i;
    // MyStl::tell_type<tuple_cat_type<std::tuple<A>, std::tuple<int, double>, std::tuple<A, A, A>, std::tuple<int, float, double>, std::tuple<A, float, double>>::type> a;
    // auto f4 = ::tuple_cat(f1, f2, f3);
    // std::cout << std::tuple_size<decltype(f4)>::value << std::endl;

    // MyStl::tell_type<MyStl::tuple_cat_type<MyStl::tuple<int &, std::string>, MyStl::tuple<double, char, A>, MyStl::tuple<std::string, MyStl::tuple<int>>>::type> a;
    // const int i = 3;
    // MyStl::tell_compile_const<i> a;
    // MyStl::tell_type<std::_Build_index_tuple<10>::__type> a;
}


