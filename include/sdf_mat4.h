#ifndef SDF_MAT4_H
#define SDF_MAT4_H

#include "common.h"

typedef struct SdfMat4 {
    f32 m[16];
} SdfMat4;

typedef char SdfMat4SizeCheck[(sizeof(SdfMat4) == 0x40) ? 1 : -1];
typedef char SdfMat4ElementsOffsetCheck[
    ((u32)&((SdfMat4 *)0)->m == 0) ? 1 : -1];

#endif /* SDF_MAT4_H */
