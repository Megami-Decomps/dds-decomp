#ifndef EFF_PCP_SPIN_H
#define EFF_PCP_SPIN_H

#include "eff_param.h"

/* The pair variants allocate through handle1 at +0x64 (0x68 bytes); the
 * single variants allocate the 0x64-byte prefix and only use handle0. The EE
 * ABI rounds sizeof this u128-aligned view up to 0x70. */
typedef struct EffPCPSpinWork {
    u8 pad00[0x10];
    u128 matrix[4];
    s32 frame;
    f32 angle;
    f32 scale;
    u32 color;
    EffParamWork *handle0;
    EffParamWork *handle1;
} EffPCPSpinWork;

typedef char EffPCPSpinWork_size_must_be_0x70[
    (sizeof(EffPCPSpinWork) == 0x70) ? 1 : -1];
typedef char EffPCPSpinWork_matrix_offset_must_be_0x10[
    ((u32)&((EffPCPSpinWork *)0)->matrix == 0x10) ? 1 : -1];
typedef char EffPCPSpinWork_frame_offset_must_be_0x50[
    ((u32)&((EffPCPSpinWork *)0)->frame == 0x50) ? 1 : -1];
typedef char EffPCPSpinWork_handle0_offset_must_be_0x60[
    ((u32)&((EffPCPSpinWork *)0)->handle0 == 0x60) ? 1 : -1];
typedef char EffPCPSpinWork_handle1_offset_must_be_0x64[
    ((u32)&((EffPCPSpinWork *)0)->handle1 == 0x64) ? 1 : -1];

#endif /* EFF_PCP_SPIN_H */
