#ifndef UTILITY_TYPE_TRAITS_H_
#define UTILITY_TYPE_TRAITS_H_

#include <cstddef>
#include <type_traits>

namespace MyStl {

  template <typename T, T N>
  struct type_constant {
    constexpr static const T value = N;
  };

  template <std::size_t N>
  using integer_constant = type_constant<std::size_t, N>;

  template <bool N>
  using bool_constant = type_constant<bool, N>;

  using true_type = bool_constant<true>;
  using false_type = bool_constant<false>;

  template <bool V, typename TrueType, typename FalseType>
  struct conditional {
    using type = TrueType;
  };

  template <typename TrueType, typename FalseType>
  struct conditional<false, TrueType, FalseType> {
    using type = FalseType;
  };

  template <typename ...>
  struct AND;

  template <typename T, typename ... TS>
  struct AND<T, TS ...> : conditional<T::value, AND<TS ...>, T>::type {};

  template <>
  struct AND<> : true_type {};

  template <typename T>
  struct AND<T> : T {};

  template <typename ...>
  struct OR;

  template <typename T, typename ... TS>
  struct OR<T, TS ...> : conditional<T::value, T, OR<TS ...>>::type {};

  template <typename T>
  struct OR<T> : T {};

  template <>
  struct OR<> : false_type {};

  template <bool, typename = void>
  struct enable_if {};

  template <typename T>
  struct enable_if<true, T> {
    using type = T;
  };

  template <typename ... Conds>
  using Require = typename enable_if<AND<Conds ...>::value>::type;

  template <typename T>
  struct remove_const {
    using type = T;
  };

  template <typename T>
  struct remove_const<const T> {
    using type = T;
  };

  template <typename T>
  struct remove_volatile {
    using type = T;
  };

  template <typename T>
  struct remove_volatile<volatile T> {
    using type = T;
  };

  template <typename T>
  struct remove_cv {
    using type = typename remove_const<typename remove_volatile<T>::type>::type;
  };

  template <typename T>
  struct is_void_helper : false_type {};

  template <>
  struct is_void_helper<void> : true_type {};

  template <typename T>
  struct is_void : is_void_helper<typename remove_cv<T>::type> {};

  template <typename T>
  struct is_function : false_type {};

#define IS_FUNCTION_TYPE(cv_append, ref_append) \
template <typename Res, typename ... Args> \
struct is_function<Res(Args ...) cv_append ref_append> : true_type {}; \
\
template <typename Res, typename ... Args> \
struct is_function<Res(Args ......) cv_append ref_append> : true_type {}

  IS_FUNCTION_TYPE(,);
  IS_FUNCTION_TYPE(const,);
  IS_FUNCTION_TYPE(const volatile,);
  IS_FUNCTION_TYPE(volatile,);
  IS_FUNCTION_TYPE(, &);
  IS_FUNCTION_TYPE(const, &);
  IS_FUNCTION_TYPE(const volatile, &);
  IS_FUNCTION_TYPE(volatile, &);
  IS_FUNCTION_TYPE(, &&);
  IS_FUNCTION_TYPE(const, &&);
  IS_FUNCTION_TYPE(const volatile, &&);
  IS_FUNCTION_TYPE(volatile, &&);

  template <typename T>
  struct is_array : false_type {};

  template <typename T>
  struct is_array<T[]> : true_type {};

  template <typename T, std::size_t N>
  struct is_array<T [N]> : true_type {};

  template <typename From, typename To,
            bool = OR<is_void<To>, is_function<To>, is_array<To>>::value>
  struct is_convertible_helper {
    using type = is_void<To>;
  };

  template <typename From, typename To>
  struct is_convertible_helper<From, To, false> {
   private:
    static void helper(To &);

    template <typename MFrom, typename = decltype(helper(std::declval<MFrom>()))>
    static true_type test(int);

    template <typename>
    static false_type test(...);

   public:
    using type = decltype(test<From>(0));
  };

  template <typename From, typename To>
  struct is_convertible : is_convertible_helper<From, To>::type {};

  template <typename T>
  struct is_empty : bool_constant<__is_empty(T)> {};

  template <typename T>
  struct is_final : bool_constant<__is_final(T)> {};

}

#endif