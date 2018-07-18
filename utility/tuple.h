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

    constexpr Head_base() = default;
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

    constexpr Head_base() = default;
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
      return Base::get(t);
    }

    constexpr static const Head &
    get_head(const tuple_impl &t) noexcept {
      return Base::get(t);
    }

    constexpr static Inherited &
    get_tail(tuple_impl &t) noexcept {
      return t;
    }

    constexpr static const Inherited &
    get_tail(const tuple_impl &t) noexcept {
      return t;
    }

    constexpr tuple_impl() = default;
    constexpr tuple_impl(const tuple_impl &) = default;

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

}

#endif
