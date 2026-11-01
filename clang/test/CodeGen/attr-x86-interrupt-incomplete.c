// RUN: %clang_cc1 -triple x86_64-unknown-linux-gnu -emit-llvm -o - %s | FileCheck %s --check-prefixes=CHECK,X64
// RUN: %clang_cc1 -triple i386-unknown-linux-gnu -emit-llvm -o - %s | FileCheck %s --check-prefixes=CHECK,X32
// RUN: %clang_cc1 -triple x86_64-unknown-linux-gnux32 -emit-llvm -o - %s | FileCheck %s --check-prefixes=CHECK,X64
// RUN: %clang_cc1 -triple x86_64-unknown-linux-gnu -x c++ -emit-llvm -o - %s | FileCheck %s --check-prefixes=CHECK,X64
// RUN: %clang_cc1 -triple i386-unknown-linux-gnu -x c++ -emit-llvm -o - %s | FileCheck %s --check-prefixes=CHECK,X32
// RUN: %clang_cc1 -triple x86_64-unknown-linux-gnu -target-feature -x87 -target-feature -sse -target-feature -mmx -O2 -emit-obj -o %t %s
// RUN: %clang_cc1 -triple i386-unknown-linux-gnu -target-feature -x87 -target-feature -sse -target-feature -mmx -O2 -emit-obj -o %t %s

// https://github.com/llvm/llvm-project/issues/237266
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __x86_64__
typedef __UINT64_TYPE__ uword;
#else
typedef __UINT32_TYPE__ uword;
#endif

struct interrupt_frame;
struct interrupt_frame *volatile saved_frame;

// CHECK-LABEL: define{{.*}} x86_intrcc void @handler(
// CHECK-SAME: ptr noundef byval(i8) %{{.*}})
__attribute__((interrupt)) void handler(struct interrupt_frame *frame) {
  saved_frame = frame;
}

// CHECK-LABEL: define{{.*}} x86_intrcc void @handler_ec(
// X64-SAME: ptr noundef byval(i8) %{{.*}}, i64 noundef %{{.*}})
// X32-SAME: ptr noundef byval(i8) %{{.*}}, i32 noundef %{{.*}})
__attribute__((interrupt)) void handler_ec(struct interrupt_frame *frame,
                                         uword error_code) {
  saved_frame = frame;
}

#ifdef __cplusplus
}
#endif
