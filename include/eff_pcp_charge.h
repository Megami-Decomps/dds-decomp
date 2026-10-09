#ifndef EFF_PCP_CHARGE_H
#define EFF_PCP_CHARGE_H

#include "eff_param.h"

struct SdfMemBlock;

/* Captured model-point history and its two parameter resources share this
 * general-heap allocation. Creation and cloning initialize the live tail and
 * vector words; neither operation copies the sampled arrays. */
typedef struct EffPCPChargeWork {
    f32 samplePositions[25][7][4];
    u32 vectorWords[4];
    u32 animationFrames[25][7];
    f32 sampleScales[25][7];
    u32 sampleAges[25][7];
    u32 color;
    f32 scale;
    u32 historyCount;
    u32 updateCount;
    u32 baseColor;
    EffParamWork *secondaryHandle;
    EffParamWork *primaryHandle;
    struct SdfMemBlock *allocationHandle;
} EffPCPChargeWork;

typedef char EffPCPChargeWork_size_must_be_0x1354[
    (sizeof(EffPCPChargeWork) == 0x1354) ? 1 : -1];
typedef char EffPCPChargeWork_vectorWords_offset_must_be_0xAF0[
    ((u32)&((EffPCPChargeWork *)0)->vectorWords == 0xAF0) ? 1 : -1];
typedef char EffPCPChargeWork_historyCount_offset_must_be_0x133C[
    ((u32)&((EffPCPChargeWork *)0)->historyCount == 0x133C) ? 1 : -1];
typedef char EffPCPChargeWork_handles_offset_must_be_0x1348[
    ((u32)&((EffPCPChargeWork *)0)->secondaryHandle == 0x1348) ? 1 : -1];
typedef char EffPCPChargeWork_allocationHandle_offset_must_be_0x1350[
    ((u32)&((EffPCPChargeWork *)0)->allocationHandle == 0x1350) ? 1 : -1];

#endif /* EFF_PCP_CHARGE_H */
