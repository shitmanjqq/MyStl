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
#include <memory>
// using namespace std;
#include "pair.h"
#include "tuple.h"
#include "smart_pointer.h"

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
        return *this;
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

struct D {
    using pointer = const Foo *;

    void operator()(pointer p) {
        delete[] p;
    }
};

std::unique_ptr<int> func() {
    std::unique_ptr<int> pi{new int};

    return pi;
}

int main()
{
    // std::default_delete<A> da;
    // std::default_delete<const B> db;
    std::cout << std::boolalpha;

    // std::cout << MyStl::is_more_cv_pointer<A *, B *>::value << std::endl;
    // std::cout << MyStl::is_convertible<B, A>::value << std::endl;
    // std::cout << MyStl::is_convertible<MyStl::remove_pointer<int *>::type, MyStl::remove_pointer<const int *>::type>::value << std::endl;
    // std::cout << MyStl::is_pointer<int *const>::value << std::endl;

    // MyStl::tell_type<MyStl::remove_pointer<int *>::type> a;
    // std::cout << std::is_convertible<B &, A &>::value << std::endl;

    // MyStl::unique_ptr<Foo [], D> up{new Foo[10]};
// // std::unique_ptr<int> up = new int; // ERROR
// std::unique_ptr<int> up(new int); // OK
    // up.release();
    // MyStl::unique_ptr<Foo> pf(new Foo);
    // MyStl::unique_ptr<Foo> pf1(new Foo);
    // pf = std::move(pf1);
    // pf = new Foo;
    // pf = nullptr;
    // pf.reset();
    // std::unique_ptr<Foo> sf(new Foo);
    // sf = nullptr;
    // up.reset(new const Foo[3]);
    // auto r = func();

    for (int i = 0; i < 10; ++i) {
        Foo f;
    }
    
    std::cout << "end of main" << std::endl;
}


