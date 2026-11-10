#ifndef SDK_LIBGCC_H
#define SDK_LIBGCC_H

#include "common.h"

f64 fptodp(f32 value) __attribute__((const));
f64 dpadd(f64 left, f64 right);
f64 dpsub(f64 left, f64 right);
f64 dpmul(f64 left, f64 right);
f64 dpdiv(f64 left, f64 right);
s32 dpcmp(f64 left, f64 right) __attribute__((const));
u32 dptoul(f64 value);
s32 dptoli(f64 value);
f64 litodp(s32 value);
f32 dptofp(f64 value);

#endif
