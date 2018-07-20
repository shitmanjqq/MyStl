#ifndef FUNCTION_FUNCTIONAL_H_
#define FUNCTION_FUNCTIONAL_H_

#include "utility/type_traits.h"

namespace MyStl {

  template <typename T>
  void swap(T &a, T &b) {
    T temp = std::move(a);
    a = std::move(b);
    b = std::move(temp);
  }

  template <typename T, std::size_t N>
  void swap(T (&a)[N], T (&b)[N]) {
    for (std::size_t i = 0; i < N; ++i) {
      swap(a[i], b[i]);
    }
  }

}

#endif