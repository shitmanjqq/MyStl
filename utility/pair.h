#ifndef UTILITY_PAIR_H_
#define UTILITY_PAIR_H_

#include "type_traits.h"
#include "function/functional.h"

namespace MyStl {

  struct piecewise_construct_t {};

  constexpr piecewise_construct_t piecewise_construct;

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
      first = std::forward<MT1>(p.first);
      second = std::forward<MT2>(p.second);
      return *this;
    }

    template <typename MT1, typename MT2>
    bool operator==(const pair<MT1, MT2> &p) {
      if (this == &p) {
        return true;
      }
      return first == p.first && second == p.second;
    }

    template <typename MT1, typename MT2>
    bool operator!=(const pair<MT1, MT2> &p) {
      if (this == &p) {
        return false;
      }
      return !(*this == p);
    }

    template <typename MT1, typename MT2>
    bool operator<(const pair<MT1, MT2> &p) {
      if (this == &p) {
        return false;
      }
      return first < p.first || (!(p.first < first) && second < p.second);
    }

    template <typename MT1, typename MT2>
    bool operator>(const pair<MT1, MT2> &p) {
      return p < *this;
    }

    template <typename MT1, typename MT2>
    bool operator<=(const pair<MT1, MT2> &p) {
      return !(*this > p);
    }

    template <typename MT1, typename MT2>
    bool operator>=(const pair<MT1, MT2> &p) {
      return !(*this < p);
    }

    template <typename MT1, typename MT2>
    void swap(pair<MT1, MT2> &p) {
      swap(first, p.first);
      swap(second, p.second);
    }

   private:
    template <typename ... Args1, std::size_t ... Idx1,
              typename ... Args2, std::size_t ... Idx2>
    constexpr pair(tuple<Args1 ...> &, tuple<Args2 ...> &,
                   index_tuple<Idx1 ...>, index_tuple<Idx2 ...>);
  };

  // template <typename T1, typename T2>
  // pair<typename std::decay<T1>::type, typename std::decay<T2>::type>
  // make_pair(T1 &&t1, T2 &&t2) {
  //   using pair_type = pair<typename std::decay<T1>::type, typename std::decay<T2>::type>;
  //   return pair_type{std::forward<T1>(t1), std::forward<T2>(t2)};
  // }

  template <typename T1, typename T2>
  pair<typename decay_and_unwrap_ref<T1>::type, typename decay_and_unwrap_ref<T2>::type>
  make_pair(T1 &&t1, T2 &&t2) {
    using pair_type = pair<typename decay_and_unwrap_ref<T1>::type, typename decay_and_unwrap_ref<T2>::type>;
    return pair_type{std::forward<T1>(t1), std::forward<T2>(t2)};
  }

  template <typename T>
  struct tuple_size;

  template <typename T1, typename T2>
  struct tuple_size<pair<T1, T2>> : integer_constant<2> {};

  template <std::size_t Idx, typename T>
  struct tuple_elem;

  template <std::size_t Idx, typename T1, typename T2>
  struct tuple_elem<Idx, pair<T1, T2>>;

  template <typename T1, typename T2>
  struct tuple_elem<0, pair<T1, T2>> {
    using type = T1;
  };

  template <typename T1, typename T2>
  struct tuple_elem<1, pair<T1, T2>> {
    using type = T2;
  };

  template <std::size_t N>
  struct pair_get;

  template <>
  struct pair_get<0> {
    template <typename T1, typename T2>
    constexpr static T1 &
    get(pair<T1, T2> &p) {
      return p.first;
    }

    template <typename T1, typename T2>
    constexpr static const T1 &
    get(const pair<T1, T2> &p) {
      return p.first;
    }
  };

  template <>
  struct pair_get<1> {
    template <typename T1, typename T2>
    constexpr static T2 &
    get(pair<T1, T2> &p) {
      return p.second;
    }

    template <typename T1, typename T2>
    constexpr static const T2 &
    get(const pair<T1, T2> &p) {
      return p.second;
    }
  };

  template <std::size_t Idx, typename T1, typename T2>
  constexpr typename tuple_elem<Idx, pair<T1, T2>>::type &
  get(pair<T1, T2> &p) {
    return pair_get<Idx>::get(p);
  }

  template <std::size_t Idx, typename T1, typename T2>
  constexpr const typename tuple_elem<Idx, pair<T1, T2>>::type &
  get(const pair<T1, T2> &p) {
    return pair_get<Idx>::get(p);
  }

  template <std::size_t Idx, typename T1, typename T2>
  constexpr typename tuple_elem<Idx, pair<T1, T2>>::type &&
  get(pair<T1, T2> &&p) {
    return static_cast<typename tuple_elem<Idx, pair<T1, T2>>::type &&>(pair_get<Idx>::get(p));
  }

  template <typename T1, typename T2, typename T3, typename T4>
  void swap(pair<T1, T2> &p1, pair<T3, T4> &p2) {
    p1.swap(p2);
  }

  template <typename T1, typename T2>
  std::ostream &operator<<(std::ostream &os, const pair<T1, T2> &p) {
    return os << p.first << ", " << p.second;
  }

}

#endif