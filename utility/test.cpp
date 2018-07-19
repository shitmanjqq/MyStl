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
// #include <utility>
// #include <tuple>
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

int main()
{
    // create tuple t:
    MyStl::tuple<int,float> t(1,2.22f);

    // pass the tuple as a whole to the constructor of Foo:
    MyStl::pair<int,Foo> p1 (42, t);

    // pass the elements of the tuple to the constructor of Foo:
    MyStl::pair<int,Foo> p2 (MyStl::piecewise_construct, MyStl::make_tuple(42), t);
}

