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
#include <utility>
#include <tuple>
// using namespace std;
#include "pair.h"
#include "tuple.h"

class Foo {
  public:
    Foo (MyStl::tuple<int, float>) {
        std::cout << "Foo::Foo(tuple)" << std::endl;
    }
    template <typename... Args>
    Foo (Args... args) {
        std::cout << "Foo::Foo(args...)" << std::endl;
    }
};

// template <int, typename T>
// void func(T);

// template <typename T>
// void func<0, T>(T);

int main()
{
    // create tuple t:
    MyStl::tuple<int,float> t(1,2.22f);

    // pass the tuple as a whole to the constructor of Foo:
    MyStl::pair<int,int> p1 (42, 23);

    // pass the elements of the tuple to the constructor of Foo:
    MyStl::pair<int,Foo> p2 (MyStl::piecewise_construct, MyStl::make_tuple(42), t);

    // std::cout << (MyStl::get<1>(std::move(t)) = 3) << std::endl;

    std::cout << (p1 == p1) << std::endl;
    
    // int i = 0;
    // MyStl::pair<int, int &> p{23, i};
    // MyStl::get<0>(p) = 3;
    // MyStl::get<1>(p) = 13;
    // std::cout << i << std::endl;
    // constexpr MyStl::pair<int, int> cp{12, 3};

    // MyStl::tell_compile_const<MyStl::get<0>(cp)> a;

    // MyStl::get<0>(std::move(p)) = 3;
    // MyStl::get<1>(static_cast<decltype(p) const &&>(p)) = 23;
    // std::cout << i << std::endl;
    // int i;
    // std::pair<void(), int> p;
    // std::forward<int &>(std::move(i));
    // std::pair<int, int &> sp{23, i};
    // std::get<1>(static_cast<decltype(sp) const &&>(sp)) = 23;
}

