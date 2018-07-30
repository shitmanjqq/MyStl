#ifndef UTILITY_SMART_POINTER_H_
#define UTILITY_SMART_POINTER_H_

#include "utility/type_traits.h"
#include "utility/tuple.h"

namespace MyStl {

  template <typename T>
  struct default_delete {
    constexpr default_delete() = default;

    template <typename MT, typename = enable_if_t<is_convertible<MT *, T *>::value>>
    constexpr default_delete(const default_delete<MT> &) {}

    void operator()(T *ptr) const {
      static_assert(!is_void<T>::value, "Can't release resources pointed by void *.");
      static_assert(sizeof(T) > 0, "Can't release incomplete type resources.");
      delete ptr;
    }
  };

  template <typename T>
  struct default_delete<T[]> {
    constexpr default_delete() = default;

    template <typename MT, typename = enable_if_t<is_convertible<MT (*)[], T (*)[]>::value>>
    constexpr default_delete(const default_delete<MT []> &) {}

    // template <
    void operator()(T *ptr) {
      static_assert(sizeof(T) > 0, "Can't release incomplete type resources.");
      delete[] ptr;
    }
  };

  template <typename T, typename DP>
  struct unique_ptr_impl {
   private:
    template <typename MT, typename MDP, typename = void>
    struct ptr_helper {
      using type = T *;
    };

    template <typename MT, typename MDP>
    struct ptr_helper<MT, MDP, void_t<typename remove_reference<MDP>::type::pointer>> {
      // using the pointer type from deleter, because deleter is responsible for 
      // release the resources refered by the pointer, so, type matching is important
      using type = typename remove_reference<MDP>::type::pointer;
    };

   public:
    using _DeleterConstraint = enable_if<AND<NOT<is_pointer<DP>>, is_default_constructible<DP>>::value>;
    using pointer = typename ptr_helper<T, DP>::type;

    unique_ptr_impl() = default;
    unique_ptr_impl(pointer p) : mpd_() {
      M_ptr() = p;
    }

    template <typename MDP>
    unique_ptr_impl(pointer p, MDP &&dp)
      : mpd_(p, std::forward<MDP>(dp)) {}

    pointer &M_ptr() {
      return get<0>(mpd_);
    }

    pointer M_ptr() const {
      return get<0>(mpd_);
    }

    DP &M_del() {
      return get<1>(mpd_);
    }

    const DP &M_del() const {
      return get<1>(mpd_);
    }

   private:
    tuple<pointer, DP> mpd_;
  };

  template <typename T, typename DP = default_delete<T>>
  struct unique_ptr {
   private:
    template <typename MDP>
    using _DeleterConstraint = typename unique_ptr_impl<T, MDP>::_DeleterConstraint::type;

   public:
    using pointer = typename unique_ptr_impl<T, DP>::pointer;
    using element_type = T;
    using deleter_type = DP;

    template <typename MT, typename MDP>
    using safe_conversion_up
      = AND<is_convertible<typename unique_ptr<MT, MDP>::pointer, pointer>,
            NOT<is_array<MT>>,
            AND<is_reference<MDP>, is_same<MDP, DP>>,
            AND<NOT<is_reference<MDP>>, is_convertible<MDP, DP>>>;

    unique_ptr() = default;

    explicit unique_ptr(pointer p)
      : mt_(p) {}

    unique_ptr(pointer p, typename conditional<is_reference<deleter_type>::value,
                                               deleter_type,
                                               const deleter_type &>::type d)
      : mt_(p, d) {}
    
    unique_ptr(pointer p, typename remove_reference<deleter_type>::type &&d)
      : mt_(p, std::move(d))
    {
      static_assert(!is_reference<deleter_type>::value, "rvalue bound to reference");
    }

    unique_ptr(nullptr_t) : unique_ptr() {}

    unique_ptr(unique_ptr &&up)
      : mt_(up.release(), std::forward<deleter_type>(up.get_deleter())) {}

    template <typename MT, typename MDP,
              typename = Require<safe_conversion_up<MT, MDP>,
                                 typename conditional<is_reference<MDP>::value,
                                                      is_same<MDP, DP>,
                                                      is_convertible<MDP, DP>>::type>>
    unique_ptr(unique_ptr<MT, MDP> &&up)
      : mt_(up.release(), std::forward<MDP>(up.get_deleter())) {}

    ~unique_ptr() {
      if (*this) {
        get_deleter()(get());
      }
      mt_.M_ptr() = pointer();
    }

    unique_ptr &operator=(unique_ptr &&up) {
      reset(up.release());
      get_deleter() = std::forward<deleter_type>(up.get_deleter());
      return *this;
    }

    template <typename MT, typename MDP,
              typename = Require<safe_conversion_up<MT, MDP>,
                                 is_assignable<DP &, MDP &&>>>
    unique_ptr &operator=(unique_ptr<MT, MDP> &&up) {
      reset(up.release());
      get_deleter() = std::forward<MDP>(up.get_deleter());
      return *this;
    }

    unique_ptr &operator=(nullptr_t) {
      reset();
      return *this;
    }

    T &operator*() const {
      return *get();
    }

    pointer operator->() const {
      return get();
    }

    pointer get() const {
      return mt_.M_ptr();
    }

    DP &get_deleter() {
      return mt_.M_del();
    }

    const DP &get_deleter() const {
      return mt_.M_del();
    }

    operator bool() const {
      return get() == pointer() ? false : true;
    }

    pointer release() {
      pointer p = mt_.M_ptr();
      mt_.M_ptr() = pointer();
      return p;
    }

    void reset(pointer p = pointer()) {
      if (*this) {
        mt_.M_del()(mt_.M_ptr());
      }
      mt_.M_ptr() = p;
    }

    void swap(unique_ptr &p) {
      using std::swap;
      swap(mt_, p.mt_);
    }

    unique_ptr(const unique_ptr &) = delete;
    unique_ptr &operator=(const unique_ptr &) = delete;
    
   private:
    unique_ptr_impl<T, DP> mt_;
  };

  template <typename T, typename DP>
  struct unique_ptr<T[], DP> {
   private:
    template <typename MDP>
    using _DeleterConstraint = typename unique_ptr_impl<T, MDP>::_DeleterConstraint::type;

   public:
    using pointer = typename unique_ptr_impl<T, DP>::pointer;
    using element_type = T;
    using deleter_type = DP;

    template <typename PT, typename MDP,
              typename up_pointer = typename unique_ptr<PT, MDP>::pointer>
    using safe_convertied_up_arr =
      AND<is_array<PT>,
          is_more_or_equale_cv_pointer<pointer, up_pointer>,
          OR<AND<is_reference<deleter_type>, is_same<deleter_type, MDP>>,
             AND<NOT<is_reference<deleter_type>>, is_convertible<MDP, deleter_type>>>>;

    template <typename PT>
    using safe_convertied_up_row =
      OR<is_more_or_equale_cv_pointer<pointer, PT>,
         is_same<PT, nullptr_t>,
         AND<is_pointer<PT>,
             is_more_or_equale_cv_pointer<pointer, element_type *>,
             is_convertible<PT (*)[], pointer (*)[]>>>;

    template <typename = _DeleterConstraint<DP>>
    unique_ptr() : mt_() {}

    template <typename PT, typename = _DeleterConstraint<DP>, typename = enable_if_t<safe_convertied_up_row<PT>::value>>
    explicit unique_ptr(PT p) : mt_(p) {}

    template <typename PT, typename = enable_if_t<safe_convertied_up_row<PT>::value>>
    unique_ptr(PT p, typename conditional<is_reference<deleter_type>::value, deleter_type, const deleter_type &>::type d)
      : mt_(p, d) {}
    
    template <typename PT, typename = enable_if_t<safe_convertied_up_row<PT>::value>>
    unique_ptr(PT p, typename remove_reference<deleter_type>::type &&d)
      : mt_(p, std::move(d)) {}

    unique_ptr(unique_ptr &&up)
      : mt_(up.release(), std::forward<deleter_type>(up.get_deleter()))
    {}

    template <typename PT, typename MDP, typename = enable_if_t<safe_convertied_up_arr<PT, MDP>::value>>
    unique_ptr(unique_ptr<PT, MDP> &&up)
      : mt_(up.release(), std::forward<MDP>(up.get_deleter()))
    {}

    ~unique_ptr() {
      reset();
    }

    unique_ptr &operator=(unique_ptr &&up) {
      reset(up.release());
      get_deleter() = std::forward<deleter_type>(up.get_deleter());
      return *this;
    }

    template <typename PT, typename MDP, typename = enable_if_t<safe_convertied_up_arr<PT, MDP>::value>>
    unique_ptr &operator=(unique_ptr<PT, MDP> &&up) {
      reset(up.release());
      get_deleter() = std::forward<MDP>(up.get_deleter());
      return *this;
    }

    unique_ptr &operator=(nullptr_t) {
      reset();
      return *this;
    }

    decltype(*pointer()) &
    operator [](std::size_t idx) const {
      return get()[idx];
    }

    pointer get() const {
      return mt_.M_ptr();
    }

    DP &get_deleter() {
      return mt_.M_del();
    }

    const DP &get_deleter() const {
      return mt_.M_del();
    }

    operator bool() {
      return mt_.M_ptr() == pointer() ? false : true;
    }

    pointer release() {
      pointer p = mt_.M_ptr();
      mt_.M_ptr() = pointer();
      return p;
    }

    void reset(pointer p = pointer()) {
      pointer &old_p = mt_.M_ptr();
      if (old_p != pointer()) {
        mt_.M_del()(old_p);
      }

      old_p = p;
    }

    void swap(unique_ptr &up) {
      using std::swap;
      swap(*this, up);
    }

    unique_ptr(const unique_ptr &) = delete;
    unique_ptr &operator=(const unique_ptr &) = delete;
   private:
    unique_ptr_impl<T, DP> mt_;
  };

  // bool operator==(unique_ptr)

}

#endif