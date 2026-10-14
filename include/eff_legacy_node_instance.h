#ifndef EFF_LEGACY_NODE_INSTANCE_H
#define EFF_LEGACY_NODE_INSTANCE_H

#include "common.h"
#include "sdf_mat4.h"

/* Matrix slots written by effCreateNodeFromDescriptor for legacy node records.
 * The intervening type-specific state and prefix remain opaque here. */
typedef struct EffLegacyNodeInstanceView {
    u8 opaquePrefix[0x20];
    SdfMat4 matrixAt20;
    u8 opaqueAt60[0x50];
    SdfMat4 matrixAtB0;
} EffLegacyNodeInstanceView;

typedef char EffLegacyNodeInstanceView_size_must_be_0xF0[
    (sizeof(EffLegacyNodeInstanceView) == 0xF0) ? 1 : -1];
typedef char EffLegacyNodeInstanceView_matrixAt20_must_be_at_0x20[
    ((u32)&((EffLegacyNodeInstanceView *)0)->matrixAt20 == 0x20) ? 1 : -1];
typedef char EffLegacyNodeInstanceView_matrixAtB0_must_be_at_0xB0[
    ((u32)&((EffLegacyNodeInstanceView *)0)->matrixAtB0 == 0xB0) ? 1 : -1];

#endif /* EFF_LEGACY_NODE_INSTANCE_H */
