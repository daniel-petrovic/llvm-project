// RUN: %clang_cc1 -std=c++14 -verify=expected,cxx20 %s
// RUN: %clang_cc1 -std=c++20 -verify=expected,cxx20 %s
// RUN: %clang_cc1 -std=c++23 -verify=expected %s

namespace std {
struct type_info {
  virtual ~type_info();
};
}

// A type_info object has no value available to the constant evaluator. Make
// sure that diagnosing virtual dispatch on it does not crash (GH237168).
constexpr void destroy_type_info() { // cxx20-error {{constexpr function never produces a constant expression}}
  typeid(int).~type_info(); // expected-note {{destruction of object 'typeid(int)' whose value is not known}}
}

#if __cplusplus > 202002L
static_assert((destroy_type_info(), true)); // expected-error {{static assertion expression is not an integral constant expression}} \
                                           // expected-note {{in call to 'destroy_type_info()'}}
#endif
