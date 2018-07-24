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

std::string s;
struct A {
    operator std::string &() {
        return s;
    }
    // operator int &() {
    //     return i;
    // }

    // A() = default;
    // template <typename T>
    // A(std::initializer_list<T>) {
    //     std::cout << "1111111111" << std::endl;
    // }
};


int main()
{
    std::cout << std::boolalpha;
    
    std::string s = "hello";

    auto t1 = MyStl::make_tuple(s);
    MyStl::get<0>(t1) = "world";
    std::cout << s << std::endl;

    auto t2 = MyStl::make_tuple(MyStl::ref(s));
    MyStl::get<0>(t2) = "world";
    std::cout << s << std::endl;

    // std::string &rs1{A{}};
    // std::string &rs2(A{});
    // std::string &rs3 = A();

    // auto t2 = MyStl::make_tuple(MyStl::ref(s));

    // std::string &rs = MyStl::ref(s);
    // MyStl::tell_type<decltype(MyStl::ref(s))> a;


    // MyStl::Head_base<0, std::string &> h{MyStl::ref(s)};
    // std::string &&rs{std::forward<MyStl::reference_wrapper<std::string>>(MyStl::ref(s))};
    // auto t2 = std::make_tuple(std::ref(s));

    // MyStl::tell_type<decltype(std::forward<MyStl::reference_wrapper<std::string>>(MyStl::ref(s)))> a;
    // decltype({std::forward<MyStl::reference_wrapper<std::string>>(MyStl::ref(s))})
    // int i;
    // int &ri{std::move(i)};
    // A a;
    // A &&ra{std::forward<MyStl::reference_wrapper<A>>(a)};

    // A &ra1 = std::forward<MyStl::reference_wrapper<A>>(a);

    // MyStl::tell_type<decltype(std::forward<MyStl::reference_wrapper<std::string>>(MyStl::ref(s)).operator std::string &())> a;
    // int i;
    // A a;
    // int &ri{std::forward<A>(a)};
    // int &ri{std::forward<int>(i)};
    // MyStl::tell_type<MyStl::decay_and_unwrap_ref<MyStl::
    // MyStl::get<0>(t2) = "world";
    // std::cout << s << std::endl;
}


