#ifndef UTILITY_TYPE_TRAITS_H_
#define UTILITY_TYPE_TRAITS_H_

#include <type_traits>

namespace MyStl {

  template <typename>
  struct tell_type;

  template <std::size_t>
  struct tell_compile_const;

#if __GNUC__ < 7
  template <typename T>
  T *address_of(T &t) noexcept {
    return reinterpret_cast<T *>(
      &(const_cast<char &>(reinterpret_cast<const volatile char &>(t))));
  }
#else
  template <typename T>
  constexpr T *address_of(T &t) noexcept {
    return __builtin_addressof(t);
  }
#endif

  template <typename T>
  inline T *addressof(T &t) noexcept {
    return address_of(t);
  }

  template <typename T>
  inline T *addressof(const T &&t) = delete;

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
  struct remove_reference {
    using type = T;
  };

  template <typename T>
  struct remove_reference<T &> {
    using type = T;
  };

  template <typename T>
  struct remove_reference<T &&> {
    using type = T;
  };

  template <typename T>
  struct is_lvalue_reference : false_type {};

  template <typename T>
  struct is_lvalue_reference<T &> : true_type {};

  template <typename T>
  struct is_rvalue_reference : false_type {};

  template <typename T>
  struct is_rvalue_reference<T &&> : true_type {};

  template <typename T>
  struct is_reference : OR<is_lvalue_reference<T>,
                           is_rvalue_reference<T>>::type {};

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

  template <typename T>
  struct is_array_and_known_bound : false_type {};

  template <typename T, std::size_t N>
  struct is_array_and_known_bound<T[N]> : true_type {};

  template <typename T>
  struct is_array_and_unknown_bound : false_type {};

  template <typename T>
  struct is_array_and_unknown_bound<T[]> : true_type {};

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

  // template 
  template <typename T>
  struct remove_extent {
    using type = T;
  };

  template <typename T>
  struct remove_extent<T[]> {
    using type = T;
  };

  template <typename T, std::size_t N>
  struct remove_extent<T[N]> {
    using type = T;
  };

  template <typename T>
  struct remove_all_extents {
    using type = T;
  };

  template <typename T>
  struct remove_all_extents<T[]> : remove_all_extents<T> {};

  template <typename T, std::size_t N>
  struct remove_all_extents<T[N]> : remove_all_extents<T> {};

  struct is_default_constructible_helper {
    template <typename T, typename = decltype(T{})>
    static true_type test(int);

    template <typename>
    static false_type test(...);
  };

  template <typename T, bool = is_array_and_unknown_bound<T>::value>
  struct is_default_constructible_array : is_default_constructible_helper {
    using type = decltype(test<typename remove_all_extents<T>::type>(0));
  };

  template <typename T>
  struct is_default_constructible_array<T, true> {
    using type = false_type;
  };

  template <typename T, bool = is_void<T>::value>
  struct is_default_constructible_imple {
    using type = typename is_default_constructible_array<T>::type;
  };

  template <typename T>
  struct is_default_constructible_imple<T, true> {
    using type = false_type;
  };

  template <typename T>
  struct is_default_constructible : is_default_constructible_imple<T>::type {};

  struct is_implicitly_default_constructible_helper {
    template <typename T>
    static void helper(const T &);

    template <typename T, typename = decltype(helper<T>({}))>
    static true_type test(int);

    template <typename>
    static false_type test(...);
  };

  template <typename T>
  struct is_implicitly_default_constructible_impl : is_implicitly_default_constructible_helper {
    using type = decltype(test<T>(0));
  };

  template <typename T>
  struct is_implicitly_default_constructible : AND<is_default_constructible<T>, typename is_implicitly_default_constructible_impl<T>::type> {};

  template <typename T>
  constexpr typename remove_reference<T>::type &&
  move(T &&t) {
    return static_cast<typename remove_reference<T>::type &&>(t);
  }

  template <typename T>
  constexpr T &&
  forward(typename remove_reference<T>::type &t) {
    return static_cast<T &&>(t);
  }

  template <typename T>
  constexpr T &&
  forward(typename remove_reference<T>::type &&t) {
    static_assert(!is_lvalue_reference<T>::value, "trans rvalue to lvalue");
    return static_cast<T &&>(t);
  }

  template <typename T>
  struct is_mem_obj_pointer_impl : false_type {};

  template <typename T, typename Class>
  struct is_mem_obj_pointer_impl<T Class::*>
    : bool_constant<!is_function<T>::value> {};

  template <typename T>
  struct is_mem_obj_pointer
    : is_mem_obj_pointer_impl<typename remove_cv<T>::type> {};

  template <typename T>
  struct is_mem_func_pointer_impl : false_type {};

  template <typename T, typename Class>
  struct is_mem_func_pointer_impl<T Class::*>
    : bool_constant<is_function<T>::value> {};

  template <typename T>
  struct is_mem_func_pointer
    : is_mem_func_pointer_impl<typename remove_cv<T>::type> {};

  template <typename PMObj, typename Obj, typename ... Args>
  struct result_of_mem_obj {
    using result_type = decltype((std::declval<Obj>().*(std::declval<PMObj>()))(std::declval<Args>() ...));
  };

  template <bool IsMemFn, bool IsMemObj, typename CallObj, typename ... Args>
  struct result_of_impl;

  template <typename CallObj, typename ... Args>
  struct result_of_impl<true, false, CallObj, Args ...> {
    // using result_type = typename result_of_mem_obj<CallObj, Args ...>::result_type;
  };

  template <typename CallObj, typename ... Args>
  struct result_of_impl<false, true, CallObj, Args ...> {
    using result_type = typename result_of_mem_obj<CallObj, Args ...>::result_type;
  };

  template <typename CallObj, typename ... Args>
  struct result_of_impl<false, false, CallObj, Args ...> {
    // no need to dereference any possible reference_wrapper arguments,
    // because they can be implicitly converted to the underly type
    using result_type = decltype(std::declval<CallObj>()(std::declval<Args>() ...));
  };

  template <typename T>
  struct result_of;

  template <typename Func, typename ... Args>
  struct result_of<Func(Args ...)>
    : result_of_impl<is_mem_func_pointer<typename remove_cv<Func>::type>::value,
                     is_mem_obj_pointer<typename remove_cv<Func>::type>::value,
                     Func, Args ...>
  {};

  template <typename T>
  class reference_wrapper {
    T *pt_;

   public:
    reference_wrapper(T &t)
      : pt_{address_of(t)} {}

    reference_wrapper(T &&) = delete;
    reference_wrapper(const reference_wrapper &) = default;

    reference_wrapper &operator=(const reference_wrapper &r) = default;

    T &get() const noexcept {
      return *pt_;
    }

    operator T&() const noexcept {
      return this->get();
    }

    template <typename ... Args>
    typename result_of<T &(Args ...)>::type
    operator()(Args &&... args) const {
      return invoke(get(), std::forward<Args>(args) ...);
    }
  };

}

#endif