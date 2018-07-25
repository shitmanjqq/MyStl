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
    // explicit A(int) {}
    // A(int, int, int) {}

    template <typename T>
    A(std::initializer_list<T>) {}
    // A() = default;
    // template <typename T>
    // A(std::initializer_list<T>) {}
    // operator char() {
    //     return 'c';
    // }
    // operator std::string &() {
    //     return s;
    // }
    // operator int &() {
    //     return i;
    // }

    // A() = default;
    // template <typename T>
    // A(std::initializer_list<T>) {
    //     std::cout << "1111111111" << std::endl;
    // }
};

// void func(std::tuple<int>);
// A func() {
//     return {1, 2, 'c'};
// }

template <typename ... TS>
struct tuple_cat_type;

template <typename ... TS1, typename ... TS2, typename ... TPS>
struct tuple_cat_type <std::tuple<TS1 ...>, std::tuple<TS2 ...>, TPS ...>
{
    using type = typename tuple_cat_type<std::tuple<TS1 ...>, typename tuple_cat_type<std::tuple<TS2 ...>, TPS ...>::type>::type;
};

template <typename ... TS1, typename ... TS2>
struct tuple_cat_type <std::tuple<TS1 ...>, std::tuple<TS2 ...>>
{
    using type = std::tuple<TS1 ..., TS2 ...>;
};

template <typename ... TS>
struct make_first_tuple_index
{
    using type = std::_Index_tuple<>;
};

template <typename ... TS, typename ... TPS>
struct make_first_tuple_index<std::tuple<TS ...>, TPS ...> {
    using type = typename std::_Build_index_tuple<sizeof...(TS)>::__type;
};

template <typename Res, std::size_t CurrIdx, std::size_t UpperBound, typename = typename std::enable_if<(CurrIdx == UpperBound)>::type, typename ... TPS>
Res tuple_cat_helper(std::_Index_tuple<>, TPS &&... tps) {
    return Res{std::forward<TPS>(tps) ...};
}

template <typename Res, std::size_t CurrIdx, std::size_t UpperBound, typename = typename std::enable_if<(CurrIdx < UpperBound)>::type, std::size_t ... Idx, typename TP, typename ... TPS>
Res tuple_cat_helper(std::_Index_tuple<Idx ...>, TP &&tp, TPS &&... tps) {
    using FirstIndex = typename make_first_tuple_index<typename std::remove_reference<TPS>::type ...>::type;
    return tuple_cat_helper<Res, CurrIdx + 1, UpperBound>(FirstIndex{}, std::forward<TPS>(tps) ..., std::get<Idx>(std::forward<TP>(tp))...);
}


template <typename ... TPS>
typename tuple_cat_type<typename std::remove_reference<TPS>::type ...>::type
tuple_cat(TPS &&... tps) {
    using ResType = typename tuple_cat_type<typename std::remove_reference<TPS>::type ...>::type;
    using FirstIndex = typename make_first_tuple_index<typename std::remove_reference<TPS>::type ...>::type;
    // MyStl::tell_type<FirstIndex> a;
    return tuple_cat_helper<ResType, 0, sizeof...(TPS)>(FirstIndex{}, std::forward<TPS>(tps) ...);
}

// const static int ggi = 3;

const int iii = 3;

struct TT {
    constexpr static int i = 3;
};

int main()
{
    std::string s = "hello world";
    // auto f1 = MyStl::make_tuple(s, 23);
    // auto f2 = MyStl::make_tuple(2.3);
    // auto f3 = MyStl::make_tuple(f1);
    // MyStl::tell_type<decltype(MyStl::make_tuple(f2))> a;

    // auto f1 = std::make_tuple(s, 23);
    // auto f2 = std::make_tuple(2.3);
    // auto f3 = std::make_tuple(f2);

    std::tuple<int> t1{3};
    // std::tuple<std::tuple<int>> t2{t1};

    auto t3 = t1;

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


