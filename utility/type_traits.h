#ifndef UTILITY_TYPE_TRAITS_H_
#define UTILITY_TYPE_TRAITS_H_

#include <type_traits>
#include <iostream>

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

  template <typename T>
  using false_assert = false_type;

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

  template <typename T>
  struct NOT : bool_constant<!T::value> {};

  template <bool, typename = void>
  struct enable_if {};

  template <typename T>
  struct enable_if<true, T> {
    using type = T;
  };

  template <bool V>
  using enable_if_t = typename enable_if<V>::type;

  template <typename T>
  using void_t = void;

  template <typename ... Conds>
  using Require = typename enable_if<AND<Conds ...>::value>::type;

  template <typename T1, typename T2>
  struct is_same : false_type {};

  template <typename T>
  struct is_same<T, T> : true_type {};

  template <typename Base, typename Derived>
  struct is_base_of : bool_constant<__is_base_of(Base, Derived)> {};

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
  struct remove_cv_and_reference {
    using type = typename remove_cv<typename remove_reference<T>::type>::type;
  };

  template <typename T>
  using remove_cv_and_reference_t = typename remove_cv_and_reference<T>::type;

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
                           is_rvalue_reference<T>> {};

  template <typename T>
  struct is_void_helper : false_type {};

  template <>
  struct is_void_helper<void> : true_type {};

  template <typename T>
  struct is_void : is_void_helper<typename remove_cv<T>::type> {};

  template <typename T>
  struct is_pointer_helper : false_type {};

  template <typename T>
  struct is_pointer_helper<T *> : true_type {};

  template <typename T>
  struct is_pointer : is_pointer_helper<typename remove_cv<T>::type> {};

  template <typename T>
  struct remove_pointer_helper {
    using type = T;
  };

  template <typename T>
  struct remove_pointer_helper<T *> {
    using type = T;
  };

  template <typename T>
  struct remove_pointer {
    using type = typename remove_pointer_helper<typename remove_cv<T>::type>::type;
  };

  template <typename From, typename To>
  struct is_convertible;

  template <typename PT1, typename PT2, bool = OR<is_same<PT1, PT2>,
                                                  is_convertible<typename remove_pointer<PT2>::type (*)[], typename remove_pointer<PT1>::type (*)[]>>::value>
  struct is_more_or_equale_cv_pointer_helper : false_type {};

  template <typename PT1, typename PT2>
  struct is_more_or_equale_cv_pointer_helper<PT1 *, PT2 *, true> : true_type {};

  template <typename PT1, typename PT2>
  struct is_more_or_equale_cv_pointer
    : is_more_or_equale_cv_pointer_helper<typename remove_cv<PT1>::type, typename remove_cv<PT2>::type>
  {};

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
    static void helper(To); // can't use To& for argument type, because std::declval() is normally a rvalue

    template <typename MFrom, typename = decltype(helper(std::declval<MFrom>()))>
    static true_type test(int);

    template <typename>
    static false_type test(...);

   public:
    using type = decltype(test<From>(0));
  };

  template <typename From, typename To>
  struct is_convertible : is_convertible_helper<From, To>::type {};

  template <typename Dst, typename Src>
  struct is_assignable_helper {
   private:
    template <typename MDst, typename MSrc, typename = decltype(std::declval<MDst>() = std::declval<MSrc>())>
    true_type test(int);

    template <typename, typename>
    false_type test(...);

   public:
    using type = decltype(test<Dst, Src>(0));
  };

  template <typename Dst, typename Src>
  struct is_assignable : is_assignable_helper<Dst, Src> {};

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
    template <typename T, typename = decltype(T())>
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
  
  struct __result_of_type_mem_func_ref {};
  struct __result_of_type_mem_func_deref {};
  struct __result_of_type_mem_obj_ref {};
  struct __result_of_type_mem_obj_deref {};
  struct __result_of_type_other {};

  template <typename PMFunc, typename Obj, typename ... Args>
  struct result_of_mem_func_ref {
    using type = decltype((std::declval<Obj>().*std::declval<PMFunc>())(std::declval<Args>() ...));
    using tag = __result_of_type_mem_func_ref;
  };

  template <typename PMFunc, typename Obj, typename ... Args>
  struct result_of_mem_func_deref {
    using type = decltype(((*std::declval<Obj>()).*std::declval<PMFunc>())(std::declval<Args>() ...));
    using tag = __result_of_type_mem_func_deref;
  };

  template <typename PMFunc, typename ... Args>
  struct result_of_mem_func;

  template <typename Func, typename Class, typename Obj, typename ... Args>
  struct result_of_mem_func<Func Class::*, Obj, Args ...> {
    using ObjType = typename remove_cv<typename remove_reference<Obj>::type>::type;
    using PMFuncPtr = Func Class::*;
    using result_type = typename conditional<OR<is_same<Class, ObjType>,
                                                is_base_of<Class, ObjType>>::value,
                                             result_of_mem_func_ref<PMFuncPtr, Obj, Args ...>,
                                             result_of_mem_func_deref<PMFuncPtr, Obj, Args ...>>::type;
    using type = typename result_type::type;
    using tag = typename result_type::tag;
  };

  template <typename PMObj, typename Obj>
  struct result_of_mem_obj_deref {
    using type = decltype(((*std::declval<Obj>()).*(std::declval<PMObj>())));
    using tag = __result_of_type_mem_obj_deref;
  };

  template <typename PMObj, typename Obj>
  struct result_of_mem_obj_ref {
    using type = decltype((std::declval<Obj>().*(std::declval<PMObj>())));
    using tag = __result_of_type_mem_obj_ref;
  };

  template <typename PMObj, typename Obj>
  struct result_of_mem_obj;

  template <typename TP, typename Class, typename Obj>
  struct result_of_mem_obj<TP Class::*, Obj> {
    using ObjType = typename remove_cv<typename remove_reference<Obj>::type>::type;
    using PMObjPtr = TP Class::*;
    using result_type = typename conditional<OR<is_same<Class, ObjType>,
                                                is_base_of<Class, ObjType>>::value,
                                             result_of_mem_obj_ref<PMObjPtr, Obj>,
                                             result_of_mem_obj_deref<PMObjPtr, Obj>>::type;
    using type = typename result_type::type;
    using tag = typename result_type::tag;
  };

  template <typename T>
  struct reference_wrapper;

  template <typename T, typename U = typename std::decay<T>::type>
  struct unwrapper_ref {
    using type = T;
  };

  template <typename T, typename U>
  struct unwrapper_ref<T, reference_wrapper<U>> {
    using type = U &;
  };

  template <bool IsMemFn, bool IsMemObj, typename CallObj, typename ... Args>
  struct result_of_impl;

  template <typename CallObj, typename Obj, typename ... Args>
  struct result_of_impl<true, false, CallObj, Obj, Args ...>
    : result_of_mem_func<typename std::decay<CallObj>::type,
                         typename unwrapper_ref<Obj>::type, Args ...>
  {};

  template <typename CallObj, typename Args>
  struct result_of_impl<false, true, CallObj, Args>
    : result_of_mem_obj<typename std::decay<CallObj>::type,
                        typename unwrapper_ref<Args>::type>
  {};

  template <typename CallObj, typename ... Args>
  struct result_of_impl<false, false, CallObj, Args ...> {
    // no need to dereference any possible reference_wrapper arguments,
    // because they can be implicitly converted to the underly type
    using type = decltype(std::declval<CallObj>()(std::declval<Args>() ...));
    using tag = __result_of_type_other;
  };

  template <typename T>
  struct result_of;

  template <typename Func, typename ... Args>
  struct result_of<Func(Args ...)>
    : result_of_impl<is_mem_func_pointer<typename remove_reference<Func>::type>::value,
                     is_mem_obj_pointer<typename remove_reference<Func>::type>::value,
                     Func, Args ...>
  {};

  template <typename T, typename U = typename unwrapper_ref<T>::type>
  constexpr U &&
  unwrap_forward(typename remove_reference<T>::type &t) {
    return static_cast<U &&>(t);
  }

  template <typename Res, typename Func, typename ... Args>
  constexpr Res
  invoke_impl(__result_of_type_other, Func &&func, Args &&... args) {
    return std::forward<Func>(func)(std::forward<Args>(args) ...);
  }

  template <typename Res, typename PMFunc, typename Obj, typename ... Args>
  constexpr Res
  invoke_impl(__result_of_type_mem_func_ref, PMFunc &&pmf, Obj &&obj, Args &&... args) {
    return (unwrap_forward<Obj>(obj).*pmf)(std::forward<Args>(args) ...);
  }

  template <typename Res, typename PMFunc, typename Obj, typename ... Args>
  constexpr Res
  invoke_impl(__result_of_type_mem_func_deref, PMFunc &&pmf, Obj &&obj, Args &&... args) {
    return ((*std::forward<Obj>(obj)).*pmf)(std::forward<Args>(args) ...);
  }

  template <typename Res, typename PMObj, typename Obj>
  constexpr Res
  invoke_impl(__result_of_type_mem_obj_ref, PMObj &&pmobj, Obj &&obj) {
    return unwrap_forward<Obj>(obj).*pmobj;
  }

  template <typename Res, typename PMObj, typename Obj>
  constexpr Res
  invoke_impl(__result_of_type_mem_obj_deref, PMObj &&pmobj, Obj &&obj) {
    return (*std::forward<Obj>(obj)).*pmobj;
  }

  template <typename Func, typename ... Args>
  constexpr typename result_of<Func &(Args ...)>::type
  invoke(Func &&func, Args &&... args) {
    using result_type = result_of<Func &(Args ...)>;
    using tag = typename result_type::tag;
    using type = typename result_type::type;
    return invoke_impl<type>(tag{}, std::forward<Func>(func), std::forward<Args>(args) ...);
  }

  template <typename T>
  class reference_wrapper {
    T *pt_;

   public:
    reference_wrapper(T &t)
      : pt_(address_of(t)) {}

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

  template <typename T>
  inline reference_wrapper<T>
  ref(T &obj) {
    return reference_wrapper<T>(obj);
  }

  template <typename T>
  inline reference_wrapper<const T>
  cref(const T &obj) {
    return reference_wrapper<const T>(obj);
  }

  template <typename T>
  inline reference_wrapper<T>
  ref(const T &&obj) {
    static_assert(false_assert<T>::value, "reference_wrapper cann't bind to rvalue.");
    return reference_wrapper<T>(const_cast<T &>(obj));
  }

  template <typename T>
  inline reference_wrapper<T>
  cref(const T &&obj) {
    static_assert(false_assert<T>::value, "reference_wrapper cann't bind to rvalue.");
    return reference_wrapper<T>(const_cast<T &>(obj));
  }

  template <typename T>
  inline reference_wrapper<T>
  ref(reference_wrapper<T> r) {
    return reference_wrapper<T>{r.get()};
  }

  template <typename T>
  inline reference_wrapper<const T>
  cref(reference_wrapper<T> r) {
    return reference_wrapper<const T>{r.get()};
  }

  template <typename T, typename U = typename std::decay<T>::type>
  struct decay_and_unwrap_ref {
    using type = U;
  };

  template <typename T, typename U>
  struct decay_and_unwrap_ref<T, reference_wrapper<U>> {
    using type = U &;
  };

  template <typename Class, typename ... TS>
  struct if_copy_constructed : false_type {};

  template <typename Class, typename T>
  struct if_copy_constructed<Class, T>
    : bool_constant<OR<is_same<Class, typename remove_reference<T>::type>,
                       is_base_of<Class, typename remove_reference<T>::type>>::value>
  {};

}

namespace std {

#if __cplusplus <= 201103L
  template <bool V>
  using enable_if_t = typename enable_if<V>::type;
#endif

  template <typename T1, typename T2>
  ostream &operator<<(ostream &os, const pair<T1, T2> &p) {
    return os << '[' << p.first << ", " << p.second << ']';
  }

  template <size_t Idx, size_t EndIdx, typename ... TS, typename = enable_if_t<(Idx < EndIdx)>>
  ostream &tuple_print_helper(ostream &os, const tuple<TS ...> &t) {
    os << get<Idx>(t) << ", ";
    return tuple_print_helper<Idx + 1, EndIdx>(os, t);
  }

  template <size_t Idx, size_t EndIdx, typename = enable_if_t<(Idx == EndIdx)>, typename ... TS>
  ostream &tuple_print_helper(ostream &os, const tuple<TS ...> &t) { // this form to overload the tuple_print_helper template function
    return os << get<Idx>(t);
  }

  template <typename ... TS>
  ostream &operator<<(ostream &os, const tuple<TS ...> &t) {
    os << '[';
    tuple_print_helper<0, sizeof...(TS) - 1>(os, t);
    return os << ']';
  }

}

#endif