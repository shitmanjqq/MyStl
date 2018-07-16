#ifndef PAIR_H_
#define PAIR_H_

#include <type_traits>

namespace MyStl {

  struct piecewise_construct_t {};

  constexpr const piecewise_construct_t piecewise_construct = piecewise_construct_t{};

  template <typename ... TS>
  struct tuple;

  template <std::size_t ... Index>
  struct index_tuple;

  template <typename T1, typename T2>
  struct pair {
    using first_type = T1;
    using second_type = T2;
    T1 first;
    T2 second;

    constexpr pair()
      : first{}, second{} {}

    constexpr pair(const pair &) = default;
    constexpr pair(pair &&) = default;

    template <typename MT1, typename MT2,
              typename = typename std::enable_if<std::__and_<std::is_convertible<MT1, T1>,
                                                             std::is_convertible<MT2, T2>>::value>::type>
    constexpr pair(MT1 &&t1, MT2 &&t2)
      : first{std::forward<MT1>(t1)}, second{std::forward<MT2>(t2)} {}

    template <typename MT1, typename MT2,
              typename = typename std::enable_if<std::__and_<std::is_convertible<const MT1 &, T1>,
                                                             std::is_convertible<const MT2 &, T2>>::value>::type>
    constexpr pair(const pair<MT1, MT2> &p)
      : first{p.first}, second{p.second} {}

    template <typename MT1, typename MT2,
              typename = typename std::enable_if<std::__and_<std::is_convertible<MT1 &&, T1>,
                                                             std::is_convertible<MT2 &&, T2>>::value>::type>
    constexpr pair(pair<MT1, MT2> &&p)
      : first{std::move<MT1>(p.first)}, second{std::move<MT2>(p.second)} {}

    template <typename ... Args1, typename ... Args2>
    constexpr pair(piecewise_construct_t, tuple<Args1 ...>, tuple<Args2 ...>);

    pair &operator=(const pair &p) {
      first = p.first;
      second = p.second;
      return *this;
    }

    pair &operator=(pair &&p) {
      first = std::move(p.first);
      second = std::move(p.second);
      return *this;
    }

    template <typename MT1, typename MT2,
              typename = typename std::enable_if<std::__and_<std::is_convertible<const MT1 &, T1>,
                                                             std::is_convertible<const MT2 &, T2>>::value>::type>
    pair &operator=(const pair<MT1, MT2> &p) {
      first = p.first;
      second = p.second;
      return *this;
    }

    template <typename MT1, typename MT2,
              typename = typename std::enable_if<std::__and_<std::is_convertible<MT1 &&, T1>,
                                                             std::is_convertible<MT2 &&, T2>>::value>::type>
    pair &operator=(pair<MT1, MT2> &&p) {
      first = std::move(p.first);
      second = std::move(p.second);
      return *this;
    }

   private:
    template <typename ... Args1, std::size_t ... Idx1,
              typename ... Args2, std::size_t ... Idx2>
    constexpr pair(tuple<Args1 ...> &, tuple<Args2 ...> &,
                   index_tuple<Idx1 ...>, index_tuple<Idx2 ...>);
  };

}

#endif