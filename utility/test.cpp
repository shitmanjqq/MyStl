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


int main()
{
    std::cout << std::boolalpha;
    // int i = 1;
    // auto pm = MyStl::make_pair(MyStl::ref(i), MyStl::ref(i));
    // auto ps = std::make_pair(std::ref(i), std::ref(i));
    // MyStl::tell_type<std::__decay_and_strip<const std::reference_wrapper<int const> *>::__type> m1;

    // MyStl::tell_type<MyStl::decay_and_unwrap_ref<const MyStl::reference_wrapper<int const> *>::type> m2;

    // MyStl::tell_type<decltype(pm)> m1;
    // MyStl::tell_type<decltype(ps)> m2;

    // ++pm.first;
    // ++pm.second;
    // std::cout << i << std::endl;

    auto t1 = MyStl::make_tuple(3, 2.3, 'c');
    auto t2 = t1;

    MyStl::get<0>(t2) = 23;
    std::cout << (t1 != t2) << std::endl;
    // MyStl::tell_type<decltype(t1)> a;
}


