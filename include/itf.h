#ifndef ITF_H
#define ITF_H

#include "common.h"
#include "eff.h"

/* Native 0xC-byte fade record shared by message windows and sound UI state. */
typedef struct BtlFade {
    u8 kind;
    u8 pad1;
    s16 phase;
    s16 alpha;
    s16 timer;
    u32 unk08;
} BtlFade;

/* Common draw record allocated by the interface object constructors (0x40). */
/* Allocation fields own general-heap descriptors, not retained payload addresses. */
typedef struct UiSprite {
    SdfMemBlock *allocation;
    SdfMemBlock *payloadAllocation;
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
