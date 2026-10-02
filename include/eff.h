#ifndef EFF_H
#define EFF_H

#include "common.h"

/* Random slot index and associated release id (0x8); DDS1/2 effect and panel views. */
typedef struct EffCntRec {
    s32 randomIndex; /* +0x00: advanced while filling random records */
    u32 unk4;        /* +0x04: id released with its owner */
} EffCntRec;

/* Effect primitive work: owned buffers, channel-B cursor and random records (0x170).
 * Shared by DDS1/2 effect and panel units. */
typedef struct EffPrim {
    void *primaryResource; /* +0x00: resource released by effFreeBuffers */
    void *secondaryResource; /* +0x04: resource released by effFreeBuffers */
    u32 recordCount;       /* +0x08: number of keyframe records */
    u16 unkC;              /* +0x0C: flag set during creation */
    u8 unkE[2];
    s32 unk10;
    void *unk14;           /* +0x14: optional buffer */
    void *unk18;
    void *unk1C;
    u32 cursorIndex;       /* +0x20: channel-B record index */
    f32 cursorPosition;    /* +0x24: channel-B interpolation position */
    f32 cursorStep;        /* +0x28: channel-B position increment */
    u8 unk2C[0x18];
    u32 randomCount;       /* +0x44: number of random records */
    u32 randomModulus;     /* +0x48: modulus for each random slot */
    u8 unk4C[0x11C];
    EffCntRec *counterRecords; /* +0x168 */
    s32 *slotLookup;       /* +0x16C: slot lookup base */
} EffPrim;

/* Type-indexed effect work and texture handle (0x40); DDS1/2 game/code_0018CAC8/00194700.c. */
typedef struct EffWork {
    u32 type;
    void *payload; /* Type-specific callback argument; also owned by list roots. */
    u32 listHead;  /* First node when this work owns a list. */
    u8 unkC[8];
    u32 unk14;
    u8 unk18[8];
    u32 unk20;
    u32 unk24;
    u8 unk28[0x10];
    void *next;
    u32 textureHandle; /* Retained texture reference. */
} EffWork;

/* Effect callback dispatch entry (0x18); DDS1/2 game/code_0018CAC8/00194700.c. */
typedef struct EffHandler {
    void (*handler)(void *);
    u8 unk4[0x14];
} EffHandler;

/* Integer-returning effect callback entry (0x18); DDS1/2 game/code_0018CAC8/00194700.c. */
typedef struct EffHandler32 {
    s32 (*handler)(s32);
    u8 unk4[0x14];
} EffHandler32;

/* Input selector and callback result (0x8); DDS1/2 game/code_0018CAC8/00194700.c. */
typedef struct EffResult {
    s32 unk0; /* Input selector. */
    s32 unk4; /* Handler result. */
} EffResult;

/* Effect subrecord with byte fields at +0x20/+0x40/+0x50 (0x51); DDS1/2 game/code_0018CAC8/00194700.c. */
typedef struct EffSub {
    u8 unk0[0x20];
    u8 unk20;
    u8 unk21[0x1F];
    u8 unk40;
    u8 unk41[0xF];
    u8 unk50;
} EffSub;

/* Message resource with prefixed name records (0x44); DDS1/2 game/code_0018CAC8/00194700.c. */
typedef struct EffMsg {
    s32 unk0;
    s32 unk4;
    u8 unk8[0x20];
    u32 unk28;
    u32 unk2C;
    u8 unk30[4];
    u32 *nameRecord; /* Name text starts at +4; the formatter returns the first word. */
    u8 unk38[8];
    u32 *prefixRecord; /* Prefix string pointer is stored in the second word. */
} EffMsg;

/* Effect allocator slot and float value (0x38); DDS1/2 game/code_0018CAC8/00194700.c. */
typedef struct EffSlot38 {
    u8 unk0[0x30];
    s32 unk30;
    f32 unk34;
} EffSlot38;

/* Effect slot array owner and allocation handle (0xC); DDS1/2 game/code_0018CAC8/00194700.c. */
typedef struct EffArrHdr {
    void *slots; /* Slot array base, read by effMathGetSlotAt. */
    u32 unk4;   /* Slot count. */
    void *allocation; /* Allocation handle. */
} EffArrHdr;

/* Billboard instance and kind-specific payload (0x64); DDS1/2 effect/billManager.c and game billboard units. */
typedef struct BillObj {
    f32 unk0;
    f32 unk4;
    f32 unk08;
    f32 unk0C;
    f32 childScaleX;  /* 0x10: set by billSetChildScaleComponents */
    f32 childScaleY;  /* 0x14 */
    f32 unk18;
    f32 unk1C;
    f32 lengthScale;  /* 0x20: set by billSetLengthExtent */
    u32 childParam;   /* 0x24: set by billSetChildParameter */
    void (*callback)(); /* 0x28: called by billInvokeCallback; set from the
                          per-index table by billCreateIndexed */
    u16 kind;         /* 0x2C: Kind: child (0) or entry list (1). */
    u16 unk2E;
    void *entryList;  /* 0x30 */
    u8 pad34[8];
    u16 unk3C;
    u8 pad3E[10];
    u32 unk48;
    u32 unk4C;
    u16 unk50;
    u8 pad52[6];
    u32 unk58;
    s32 entryCount;
    void *unk60;
} BillObj;

/* Billboard entry count and payload (0x30); DDS1/2 effect/billManager.c. */
typedef struct BillData {
    u8 pad[8];
    s32 childRefCount;
    s32 entryCount;
    u8 pad10[4];
    s32 listRefCount;
    u8 pad18[24];
} BillData;

/* Kind-zero billboard payload shared by the resource initializer and accessors. */
typedef struct BillChildPayload {
    s32 value;
    union {
        s16 signedVariant;
        u16 variant;
    };
    u8 pad06[0x1E];
    f32 halfWidth;  /* 0x24 */
    f32 halfHeight; /* 0x28 */
} BillChildPayload;

/* Billboard callbacks and metadata (0xC); DDS1/2 effect/billManager.c and game billboard units. */
typedef struct {
    void *(*func)();
    void (*callback)();
    u32 unk8;
} BillDispatch;

/* Resource index configuration (0xC); DDS1/2 game/code_00151F58/00159B48.c. */
typedef struct EffectConfig {
    s16 billboardKind;
    u8 pad02[10];
} EffectConfig;

/* Per-resource effect buffer entry (0x40); DDS1/2 game/code_00151F58/00159B48.c. */
typedef struct EffectBufferRecord {
    u8 pad00[0x20];
    s32 unk20;       /* 0x20: per-record tag, decays by decayStep */
    s32 unk24;
    u8 pad28[0x18];
} EffectBufferRecord;

/* Buffer allocation and record array owner (0x8); DDS1/2 game/code_00151F58/00159B48.c. */
typedef struct EffectBufferTail {
    s32 allocation;
    EffectBufferRecord *records;
} EffectBufferTail;

/* Ring-effect geometry: starting angle (-pi/2) and one full turn. */
#define EFFECT_RING_START_ANGLE (-1.5707963f)
#define EFFECT_RING_FULL_TURN (6.2831853f)

/* Mid-grey fragment colour (0x80 in each channel). */
#define EFF_THUNDER_FRAGMENT_GREY 0x80808080

#endif /* EFF_H */
