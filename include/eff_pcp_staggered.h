#ifndef EFF_PCP_STAGGERED_H
#define EFF_PCP_STAGGERED_H

#include "eff_param.h"

/* Eight delayed pairs share this size-class allocation. The input-position
 * callback copies all four vector lanes; the update uses the first three. */
typedef struct EffPCPStaggered {
    f32 position[4];
    u32 color;
    f32 scale;
    f32 verticalOffset[8];
    EffParamWork *handle[16];
    u32 remainingDelay[8];
} EffPCPStaggered;

typedef char EffPCPStaggered_size_must_be_0x98[
    (sizeof(EffPCPStaggered) == 0x98) ? 1 : -1];
typedef char EffPCPStaggered_color_offset_must_be_0x10[
    ((u32)&((EffPCPStaggered *)0)->color == 0x10) ? 1 : -1];
typedef char EffPCPStaggered_scale_offset_must_be_0x14[
    ((u32)&((EffPCPStaggered *)0)->scale == 0x14) ? 1 : -1];
typedef char EffPCPStaggered_verticalOffset_offset_must_be_0x18[
    ((u32)&((EffPCPStaggered *)0)->verticalOffset == 0x18) ? 1 : -1];
typedef char EffPCPStaggered_handle_offset_must_be_0x38[
    ((u32)&((EffPCPStaggered *)0)->handle == 0x38) ? 1 : -1];
typedef char EffPCPStaggered_remainingDelay_offset_must_be_0x78[
    ((u32)&((EffPCPStaggered *)0)->remainingDelay == 0x78) ? 1 : -1];

#endif /* EFF_PCP_STAGGERED_H */
