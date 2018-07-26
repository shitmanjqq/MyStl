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

struct A {
    virtual ~A() {}
};

struct B : A {
    ~B() {
        // std::cout << "~B" << std::endl;
    }
};

template <typename T>
struct Test {
    // using type = T *;
    // void func(T *);
};

struct AA {
    using pointer = AA *;
    ~AA() {
        std::cout << "~AA" << std::endl;
    }
};

struct BB {
  private:
    BB();
};

struct C {
    AA &&ra;
};

int main()
{
    // std::default_delete<A> da;
    std::default_delete<const B> db;
    std::cout << std::boolalpha;

    std::cout << std::is_convertible<B &, A &>::value << std::endl;

    // MyStl::unique_ptr_impl<AA, BB> a;
    // std::cout << MyStl::is_default_constructible<BB>::value << std::endl;

    // C c{AA()};
    // std::cout << "end of main" << std::endl;

    MyStl::unique_ptr<AA> pa{new AA};

    std::cout << MyStl::is_reference<int &>::value << std::endl;
    // auto pa1 = pa;
    // MyStl::tell_type<decltype(BB{})> b;
    // MyStl::tell_type<decltype(BB())> b;
}


