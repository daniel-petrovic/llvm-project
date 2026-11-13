// RUN: %clang_cc1 -std=c++17 -fsyntax-only -fdeclspec -Wno-ignored-attributes -verify %s
// RUN: %clang_cc1 -fsyntax-only -fms-extensions -Wno-ignored-attributes -verify %s

struct [[]] __attribute__((lockable)) __declspec(dllexport) A {}; // ok
struct [[]] __declspec(dllexport) __attribute__((lockable)) B {}; // ok
struct [[]] [[]] __declspec(dllexport) __attribute__((lockable)) C {}; // ok
struct __declspec(dllexport) [[]] __attribute__((lockable)) D {}; // ok
struct __declspec(dllexport) __attribute__((lockable)) [[]] E {}; // ok
struct __attribute__((lockable)) __declspec(dllexport) [[]] F {}; // ok
struct __attribute__((lockable)) [[]] __declspec(dllexport) G {}; // ok
struct [[]] __attribute__((lockable)) [[]] __declspec(dllexport) H {}; // ok

[[noreturn]] __attribute__((cdecl)) __declspec(dllexport) void a(); // ok
[[noreturn]] __declspec(dllexport) __attribute__((cdecl)) void b(); // ok
[[]] [[noreturn]] __attribute__((cdecl)) __declspec(dllexport) void c(); // ok

__declspec(dllexport) [[noreturn]] __attribute__((cdecl)) void d(); // ok
__declspec(dllexport) __attribute__((cdecl)) [[noreturn]] void e(); // ok
__attribute__((cdecl)) __declspec(dllexport) [[noreturn]] void f(); // ok

__attribute__((cdecl)) [[noreturn]] __declspec(dllexport) void g(); // ok

[[noreturn]] __attribute__((cdecl))
[[]]
__declspec(dllexport) void h();

template <int a>
__attribute__((cdecl)) [[noreturn]] __declspec(dllexport) void i(); // ok

template <int a>
[[]] [[noreturn]] __attribute__((cdecl)) __declspec(dllexport) void j(); // ok

template <int a>
[[noreturn]] __declspec(dllexport) __attribute__((cdecl)) void k(); // ok

extern "C" {
  __attribute__ ((__warn_unused_result__)) [[__maybe_unused__]] int l(int); // ok
  [[__maybe_unused__]] __attribute__ ((__warn_unused_result__)) int m(int); // ok
}

extern "C" {
  __attribute__ ((__warn_unused_result__)) [[__maybe_unused__]] int n (int); // ok
  __attribute__ ((__warn_unused_result__)) [[__maybe_unused__]] static int o (int x) { return x; }; // ok
}

extern "C" __attribute__ ((__warn_unused_result__)) [[__maybe_unused__]] int p(int); // ok
extern "C" [[__maybe_unused__]] __attribute__ ((__warn_unused_result__)) int q(int); // ok

// Mixed GNU and standard attributes also work in namespace scope (GH240537).
namespace {
__attribute__((unused)) [[maybe_unused]] int x;
[[maybe_unused]] __attribute__((unused)) int y;
__attribute__((unused)) [[maybe_unused]]
__attribute__((aligned(16))) [[]] int z;
static_assert(__alignof__(z) == 16, "GNU attributes must be preserved");
}

namespace N {
__attribute__((warn_unused_result)) [[maybe_unused]] int f();
[[maybe_unused]] __attribute__((warn_unused_result)) int g();
[[maybe_unused]] __attribute__((warn_unused_result)) [[]]
__attribute__((nothrow)) [[nodiscard]] int h();
}

// Leading attribute permutations at global and namespace scope.
[[]] __declspec() int declspecAfterStandard;
__declspec() [[]] int standardAfterDeclspec;

__attribute__(()) [[]] __declspec() int mixedGlobal0;
[[]] __attribute__(()) __declspec() int mixedGlobal1;
__attribute__(()) __declspec() [[]] int mixedGlobal2;
__declspec() __attribute__(()) [[]] int mixedGlobal3;
__declspec() [[]] __attribute__(()) int mixedGlobal4;
[[]] __declspec() __attribute__(()) int mixedGlobal5;

namespace Mixed {
[[]] __declspec() int declspecAfterStandard;
__declspec() [[]] int standardAfterDeclspec;
__attribute__(()) [[]] __declspec() int mixedNamed0;
[[]] __attribute__(()) __declspec() int mixedNamed1;
__attribute__(()) __declspec() [[]] int mixedNamed2;
__declspec() __attribute__(()) [[]] int mixedNamed3;
__declspec() [[]] __attribute__(()) int mixedNamed4;
[[]] __declspec() __attribute__(()) int mixedNamed5;
}
__declspec(align(32)) [[maybe_unused]] __attribute__((aligned(16)))
__declspec() [[]] int preservedDeclspec;
static_assert(__alignof__(preservedDeclspec) == 32, "declspec must be preserved");
__declspec() [[maybe_unused]] __attribute__((aligned(64))) int preservedGNU;
static_assert(__alignof__(preservedGNU) == 64, "GNU attribute must be preserved");
__declspec() [[nodiscard]] __attribute__(()) int preservedStandard();
void checkStandard() {
  preservedStandard(); // expected-warning {{ignoring return value of function declared with 'nodiscard' attribute}}
}

// Mixed leading attributes also work in linkage specifications.
extern "C" {
__declspec() [[maybe_unused]] int linkageBlock0;
__declspec() __attribute__(()) [[maybe_unused]] int linkageBlock1;
__attribute__(()) __declspec() [[maybe_unused]] int linkageBlock2;
}
extern "C" __declspec() [[maybe_unused]] int linkageSingle0;
extern "C" __declspec() __attribute__(()) [[maybe_unused]] int linkageSingle1;
extern "C" __attribute__(()) __declspec() [[maybe_unused]] int linkageSingle2;
