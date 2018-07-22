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

struct A {
    int i;
};

struct B {
    int j;
};

struct C : A, B {
    int k;
};

struct Test {
    int i;
    void *func(int, double, A, B) const {}
};

// Test func(int, double, A, B);

// void gunc(B);

int main()
{
    // const int i = 3;
    // std::reference_wrapper<int > rr;
    // C c;
    // std::reference_wrapper<B> rr{c};
    // std::cout << &c << std::endl;
    // B &rb = c;
    // std::cout << &rb << std::endl;
    // std::cout << &(rr.get()) << std::endl;
    std::cout << std::boolalpha;
    MyStl::tell_type<typename MyStl::result_of<decltype(&Test::func)(const Test, const int, std::reference_wrapper<double>, volatile A, const B &)>::result_type> a;
    // MyStl::tell_type<decltype(std::declval<decltype(gunc)>()(std::declval<const volatile B&>()))> b;
    // MyStl::tell_type<decltype(std::declval<const volatile B&>())> c;
    MyStl::tell_type<typename std::result_of<decltype(&Test::func)(const Test, const int, std::reference_wrapper<double>, volatile A, const B &)>::type> b;
    // MyStl::tell_type<typename std::__result_of_impl<false, false, decltype(func), int, double, A, B>::type> b;
    // std::cout << MyStl::is_mem_func_pointer<decltype(&Test::func) *>::value << std::endl;
    // std::cout << MyStl::is_mem_func_pointer<decltype(&Test::i)>::value << std::endl;
    // std::cout << MyStl::is_mem_obj_pointer<decltype(&Test::func)>::value << std::endl;
    // std::cout << MyStl::is_mem_obj_pointer<decltype(&Test::i)>::value << std::endl;
}


