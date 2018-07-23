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

struct D {
    void operator()(int) {}
    void func() & {}
};

struct Test {
    D i;
    void *func(int, double, A, B) const {}
};

// Test func(int, double, A, B);

// void gunc(B);

// template <typename T>
// using void_t = void;


// template <typename T>
// struct F;

// template <typename T, typename = F<T &>>
// struct Mem;

// template <typename T, typename U>
// struct Mem<T, F<U>> {
//     U u;
// };

// template <typename T>
// struct Mem<T, void> {};

// struct F {
//     explicit F(int i) : i{i} {}

//     template <typename T>
//     F(T &&i) : i{i} {}

//     int i;
// };

int add(int a, int b) {
    return a + b;
}

int main()
{
    // auto pmf = &Test::func;
    // MyStl::reference_wrapper<decltype(&Test::func)> f{pmf};
    // Test t;
    // f(Test{}, 2, 2.3, A{}, B{});

    // std::reference_wrapper<decltype(&Test::func)> sf{pmf};
    // sf(Test{}, 2, 2.3, A{}, B{});
    // std::cout << f(2, 3) << std::endl;
    // const int i = 3;
    // std::reference_wrapper<int > rr;
    // C c;
    // std::reference_wrapper<B> rr{c};
    // std::cout << &c << std::endl;
    // B &rb = c;
    // std::cout << &rb << std::endl;
    // std::cout << &(rr.get()) << std::endl;
    // std::cout << std::boolalpha;
    // MyStl::tell_type<typename MyStl::result_of<decltype(&Test::func) &(const MyStl::reference_wrapper<Test>, const int, std::reference_wrapper<double>, volatile A, const B &)>::type> a;
    // MyStl::tell_type<typename std::result_of<decltype(&Test::func) &(std::reference_wrapper<Test>, const int, std::reference_wrapper<double>, volatile A, const B &)>::type> b;

    // MyStl::tell_type<std::result_of<decltype(&Test::i)(const std::reference_wrapper<Test>)>::type> a;
    // MyStl::tell_type<typename MyStl::result_of<decltype(&Test::i)(const MyStl::reference_wrapper<Test>)>::result_type> b;

    // MyStl::tell_type<std::__inv_unwrap<const std::reference_wrapper<Test>>::type> i;

    // auto pmf = &Test::i;
    // auto f = std::ref(pmf);
    // Test t;
    // f(t).func();
    // Mem<int> m;
    // int i;
    // F f(i);
    // F cf{f};
    int i = 3;
    int *pi = &i;
    auto f = MyStl::ref(pi);
    auto cf = MyStl::cref(f);

    // auto sf = std::ref(pi);
    // auto csf = std::cref(sf);
    // *(f.get()) = 23;
    // MyStl::tell_type<decltype(f)> a;
    // MyStl::cref(i);
}


