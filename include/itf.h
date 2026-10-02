#ifndef ITF_H
#define ITF_H

#include "common.h"

typedef struct SdfAllocation SdfAllocation;

/* Common draw record allocated by the interface object constructors (0x40). */
typedef struct UiSprite {
    SdfAllocation *allocation;
    SdfAllocation *payloadAllocation;
    u32 *payload;
    u8 pad0C[4];
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 unk20;
    s32 screenY;
    u8 pad28[0x14];
    s8 kind;
    u8 pad3D[3];
} UiSprite;

#endif /* ITF_H */
