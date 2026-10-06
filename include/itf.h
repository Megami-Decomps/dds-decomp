#ifndef ITF_H
#define ITF_H

#include "common.h"
#include "eff.h"
#include "sdf.h"

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

/* The message manager owns a fixed pool of 64 windows in this 0x520-byte bank. */
typedef struct ItfMesPoolNode {
    struct ItfMesPoolNode *previous;
    struct ItfMesPoolNode *next;
    s32 index;
    s32 stateAddress;
    s32 resourceHandle;
} ItfMesPoolNode;

typedef struct ItfMesPool {
    ItfMesPoolNode *activeHead;
    ItfMesPoolNode *activeTail;
    ItfMesPoolNode *firstFree;
    ItfMesPoolNode *lastFree;
} ItfMesPool;

typedef struct ItfMesGlobals {
    u32 activeWindowCount;
    SdfTex *windowTexture;
    u32 unk8;
    u16 flags;
    u16 unkE;
    ItfMesPool pool;
    ItfMesPoolNode nodes[0x40];
} ItfMesGlobals;

typedef char ItfMesPoolNode_size_must_be_0x14[(sizeof(ItfMesPoolNode) == 0x14) ? 1 : -1];
typedef char ItfMesPool_size_must_be_0x10[(sizeof(ItfMesPool) == 0x10) ? 1 : -1];
typedef char ItfMesGlobals_size_must_be_0x520[(sizeof(ItfMesGlobals) == 0x520) ? 1 : -1];



#endif /* ITF_H */
