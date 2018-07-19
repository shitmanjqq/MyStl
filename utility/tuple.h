#ifndef UTILITY_TUPLE_H_
#define UTILITY_TUPLE_H_

#include "type_traits.h"

namespace MyStl {

  template <typename ... TS>
  struct tuple;

  template <typename T>
  struct is_empty_and_not_tuple : is_empty<T> {};

  template <typename ... TS>
  struct is_empty_and_not_tuple<tuple<TS ...>> : false_type {};

  template <std::size_t Idx, typename Head, bool = conditional<__is_final(Head), false_type,
                                                               is_empty_and_not_tuple<Head>>::type::value>
  struct Head_base : Head {
    constexpr static Head &
    get_head(Head_base &h) {
      return h;
    }

    constexpr static const Head &
    get_head(const Head_base &h) {
      return h;
    }

    constexpr Head_base() : Head{} {}
    constexpr Head_base(const Head_base &) = default;
    constexpr Head_base(Head_base &&) = default;

    constexpr Head_base(const Head &head)
      : Head{head} {}

    template <typename MHead>
    constexpr Head_base(MHead &&head)
      : Head{std::forward<MHead>(head)} {}
  };

  template <std::size_t Idx, typename Head>
  struct Head_base<Idx, Head, false> {
    constexpr static Head &
    get_head(Head_base &h) {
      return h.head_;
    }

    constexpr static const Head &
    get_head(const Head_base &h) {
      return h.head_;
    }

    constexpr Head_base() : head_{} {}
    constexpr Head_base(const Head_base &) = default;
    constexpr Head_base(Head_base &&) = default;

    constexpr Head_base(const Head &head)
      : head_{head} {}

    template <typename MHead>
    constexpr Head_base(MHead &&head)
      : head_{std::forward<MHead>(head)} {}
  
   private:
    Head head_;
  };

  template <std::size_t Idx, typename Head, typename ... Tails>
  struct tuple_impl : tuple_impl<Idx + 1, Tails ...>
                    , private Head_base<Idx, Head>
  {
    using Inherited = tuple_impl<Idx + 1, Tails ...>;
    using Base = Head_base<Idx, Head>;

    constexpr static Head &
    get_head(tuple_impl &t) noexcept {
      return Base::get_head(t);
    }

    constexpr static const Head &
    get_head(const tuple_impl &t) noexcept {
      return Base::get_head(t);
    }

    constexpr static Inherited &
    get_tails(tuple_impl &t) noexcept {
      return t;
    }

    constexpr static const Inherited &
    get_tails(const tuple_impl &t) noexcept {
      return t;
    }

    constexpr tuple_impl() = default;
    constexpr tuple_impl(const tuple_impl &) = default;
    constexpr tuple_impl(tuple_impl &&) = default;

    constexpr tuple_impl(const Head &head, const Tails &... tails)
      : Inherited{tails ...}, Base{head} {}

    template <typename MHead, typename ... MTails, typename = typename enable_if<sizeof...(Tails) == sizeof...(MTails)>::type>
    constexpr tuple_impl(MHead &&head, MTails &&... tails)
      : Inherited{std::forward<MTails>(tails) ...}, Base{std::forward<MHead>(head)} {}

    template <typename ... Elems, typename = typename enable_if<sizeof...(Tails) + 1 == sizeof...(Elems)>::type>
    constexpr tuple_impl(const tuple_impl<Idx, Elems ...> &t) // use copy
      : Inherited{tuple_impl<Idx, Elems ...>::get_tails(t)}
      , Base{tuple_impl<Idx, Elems ...>::get_head(t)} {}

    template <typename MHead, typename ... MTails, typename = typename enable_if<sizeof...(Tails) == sizeof...(MTails)>::type>
    constexpr tuple_impl(tuple_impl<Idx, MHead, MTails ...> &&t)
      : Inherited{std::move(tuple_impl<Idx, MHead, MTails ...>::get_tails(t))}
      , Base{std::forward<MHead>(tuple_impl<Idx, MHead, MTails ...>::get_head(t))} {}

  };

  template <std::size_t Idx, typename Head>
  struct tuple_impl<Idx, Head> : private Head_base<Idx, Head> {
    using Base = Head_base<Idx, Head>;

    constexpr static Head &
    get_head(tuple_impl &t) {
      return Base::get_head(t);
    }

    constexpr static const Head &
    get_head(const tuple_impl &t) {
      return Base::get_head(t);
    }

    constexpr tuple_impl() = default;
    constexpr tuple_impl(const tuple_impl &) = default;
    constexpr tuple_impl(tuple_impl &&) = default;

    constexpr tuple_impl(const Head &head)
      : Base{head} {}

    constexpr tuple_impl(Head &&head)
      : Base{std::move(head)} {}

    template <typename MHead>
    constexpr tuple_impl(MHead &&head)
      : Base{std::forward<MHead>(head)} {}

    template <typename MHead>
    constexpr tuple_impl(const tuple_impl<Idx, MHead> &t)
      : Base{tuple_impl<Idx, MHead>::get_head(t)} {}

    template <typename MHead>
    constexpr tuple_impl(tuple_impl<Idx, MHead> &&t)
      : Base{std::forward<MHead>(tuple_impl<Idx, MHead>::get_head(t))} {}
  };

  template <typename ... TS>
  struct tuple : tuple_impl<0, TS ...> {
    using Inherited = tuple_impl<0, TS ...>;
  
    constexpr tuple() = default;
    constexpr tuple(const tuple &) = default;
    constexpr tuple(tuple &&) = default;

    template <typename ... Elems, typename = typename enable_if<sizeof...(TS) == sizeof...(Elems)>::type>
    constexpr tuple(Elems &&... elems)
      : Inherited{std::forward<Elems>(elems) ...} {}

    template <typename ... Elems, typename = typename enable_if<sizeof...(TS) == sizeof...(Elems)>::type>
    constexpr tuple(const tuple<Elems ...> &t)
      : Inherited{static_cast<const tuple_impl<0, Elems ...> &>(t)} {}

    template <typename ... Elems, typename = typename enable_if<sizeof...(TS) == sizeof...(Elems)>::type>
    constexpr tuple(tuple<Elems ...> &&t)
      : Inherited{static_cast<tuple_impl<0, Elems ...> &&>(t)} {}
  };

  template <typename T>
  struct tuple_size;

  template <typename ... TS>
  struct tuple_size<tuple<TS ...>> : integer_constant<sizeof...(TS)> {};

  template <std::size_t Idx, typename T>
  struct tuple_elem;

  template <std::size_t Idx, typename Head, typename ... Tails>
  struct tuple_elem<Idx, tuple<Head, Tails ...>> : tuple_elem<Idx - 1, tuple<Tails ...>> {};

  template <typename Head, typename ... Tails>
  struct tuple_elem<0, tuple<Head, Tails ...>> {
    using type = Head;
  };

  template <std::size_t Idx>
  struct tuple_elem<Idx, tuple<>> {
    static_assert(Idx < 0, "tuple index is out of range");
  };

  template <std::size_t ... Idxs>
  struct index_tuple {};

  template <typename T>
  struct index_tuple_impl;

  template <std::size_t ... Idxs>
  struct index_tuple_impl<index_tuple<Idxs ...>> {
    using type = index_tuple<Idxs ..., sizeof...(Idxs)>;
  };

  template <std::size_t N>
  struct make_index_tuple {
    using type = typename index_tuple_impl<typename make_index_tuple<N - 1>::type>::type;
  };

  template <>
  struct make_index_tuple<0> {
    using type = index_tuple<>;
  };

  template <std::size_t N>
  struct build_index_tuple {
    template <std::size_t ... NS>
    static index_tuple<NS ..., sizeof...(NS)>
    helper(index_tuple<NS ...>);

    using type = decltype(helper((typename build_index_tuple<N - 1>::type){}));
  };

  template <>
  struct build_index_tuple<0> {
    static index_tuple<> helper(index_tuple<>);

    using type = index_tuple<>;
  };

  template <typename IndexTuple1, typename IndexTuple2>
  struct index_tuple_cat;

  template <std::size_t ... NS1, std::size_t ... NS2>
  struct index_tuple_cat<index_tuple<NS1 ...>, index_tuple<NS2 ...>> {
    using type = index_tuple<NS1 ..., NS2 + sizeof...(NS1) ...>;
  };

  template <std::size_t N>
  struct build_index_tuple_v2 {
    using type = typename index_tuple_cat<typename build_index_tuple_v2<N / 2>::type,
                                          typename build_index_tuple_v2<N - N / 2>::type>::type;
  };

  template <>
  struct build_index_tuple_v2<1> {
    using type = index_tuple<0>;
  };

  template <>
  struct build_index_tuple_v2<0> {
    using type = index_tuple<>;
  };

  template <std::size_t Idx, typename Head, typename ... Tails>
  constexpr Head &get_helper(tuple_impl<Idx, Head, Tails ...> &t) {
    return tuple_impl<Idx, Head, Tails ...>::get_head(t);
  }

  template <std::size_t Idx, typename Head, typename ... Tails>
  constexpr const Head &get_helper(const tuple_impl<Idx, Head, Tails ...> &t) {
    return tuple_impl<Idx, Head, Tails ...>::get_head(t);
  }
  

  template <std::size_t Idx, typename ... Elems>
  constexpr typename tuple_elem<Idx, tuple<Elems ...>>::type &
  get(tuple<Elems ...> &t) {
    return get_helper<Idx>(t);
  }

  template <std::size_t Idx, typename ... Elems>
  constexpr const typename tuple_elem<Idx, tuple<Elems ...>>::type &
  get(const tuple<Elems ...> &t) {
    return get_helper<Idx>(t);
  }

  template <std::size_t Idx, typename ... Elems>
  constexpr typename tuple_elem<Idx, tuple<Elems ...>>::type &&
  get(tuple<Elems ...> &&t) {
    using elem_type = typename tuple_elem<Idx, tuple<Elems ...>>::type;
    return std::forward<elem_type>(get_helper<Idx>(t));
  }

  template <typename ... TS>
  constexpr tuple<typename std::decay<TS>::type ...> make_tuple(TS &&... elems) {
    return tuple<typename std::decay<TS>::type ...>{std::forward<TS>(elems) ...};
  }
}

#endif
