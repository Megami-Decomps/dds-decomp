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
    s32 unk0C;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
    s32 unk20;
    s32 screenY;
    s32 scrollSpan; /* Panel edge-fade divisor; both games read it as a signed word. */
    /* The secondary panel notification copies these four opaque values. */
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    u8 kind; /* Unsigned index into the panel handler tables. */
    u8 pad3D[3];
} UiSprite;

/* Camera color effects use the same float and packed-color fields in both games. */
typedef struct RgbAlpha {
    u8 pad00[0x18];
    u32 rgb;
    u8 pad1C[0x1C];
    u32 alpha;
    u32 x3C;
    u8 pad40[0x10];
    f32 f50;
} RgbAlpha;

typedef struct CfSrc {
    u8 pad00[4];
    u32 rgb;
    u8 pad08[0x1C];
    u32 alpha;
    u32 x28;
    u8 pad2C[0x10];
    f32 f3C;
} CfSrc;


#endif /* ITF_H */
