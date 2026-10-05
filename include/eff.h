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

/* Two-child draw descriptor embedded in the billboard instance at +0x34. */
typedef struct BillRenderPair {
    struct BillChildPayload *children[2];
    u16 unk8;
    u16 kind;
    u32 colors[2];
} BillRenderPair;

typedef char BillRenderPair_size_must_be_0x14[
    (sizeof(BillRenderPair) == 0x14) ? 1 : -1];

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
    BillRenderPair pair; /* 0x34: used by the mode-0x80 entry renderer */
    u32 unk48;
    u32 unk4C;
    u16 unk50;
    u8 pad52[2];
    u32 modeFlags; /* 0x54: bits 0x40/0x80 select billboard entry modes */
    u32 unk58;
    s32 entryCount;
    void *unk60;
} BillObj;

/* Serialized entry offsets are relative to BillData.base. */
typedef struct BillAnimationEntry {
    s32 offset;
    s32 colorOffset;
    u32 unk8;
    u32 frameCount;
    u32 flags;
} BillAnimationEntry;

/* Plural entries use the first two halfwords as an entry index and delay. */
typedef struct BillRecord {
    union {
        s16 width;
        s16 entryIndex;
    };
    union {
        s16 height;
        s16 delay;
    };
    s16 x;
    s16 y;
    u16 u0;
    u16 v0;
    u16 u1;
    u16 v1;
    s16 childIndex;
    s16 value;
    f32 scale;
} BillRecord;

typedef struct BillOut {
    s32 unk0;
    s32 frameIndex;
    s32 framesRemaining;
    BillAnimationEntry *entry;
    BillRecord *record;
} BillOut;

/* Runtime header precedes the copied resource bytes; the child table is indirect. */
typedef struct BillData {
    void *allocation;
    u8 *base;
    BillAnimationEntry *entries;
    s32 entryCount;
    s32 childCount;
    s32 listRefCount;
    struct BillChildPayload **children;
} BillData;

typedef char BillAnimationEntry_size_must_be_0x14[
    (sizeof(BillAnimationEntry) == 0x14) ? 1 : -1];
typedef char BillRecord_size_must_be_0x18[
    (sizeof(BillRecord) == 0x18) ? 1 : -1];
typedef char BillOut_size_must_be_0x14[
    (sizeof(BillOut) == 0x14) ? 1 : -1];
typedef char BillData_size_must_be_0x1C[
    (sizeof(BillData) == 0x1C) ? 1 : -1];

typedef struct BillTextureCoordinate {
    u16 u;
    u16 v;
} BillTextureCoordinate;

/* Native billboard UVs are four ordered U/V corners, not a float vector. */
typedef struct BillTextureQuad {
    BillTextureCoordinate corners[4];
} BillTextureQuad;

typedef char BillTextureQuad_size_must_be_0x10[
    (sizeof(BillTextureQuad) == 0x10) ? 1 : -1];

/* Kind-zero billboard payload shared by the resource initializer and accessors. */
typedef struct BillChildPayload {
    s32 value;
    union {
        s16 signedVariant;
        u16 variant;
    };
    u8 pad06[2];
    s32 refCount;      /* 0x08 */
    BillTextureQuad uv; /* 0x0C */
    f32 x;             /* 0x1C */
    f32 y;             /* 0x20 */
    f32 halfWidth;  /* 0x24 */
    f32 halfHeight; /* 0x28 */
} BillChildPayload;

typedef char BillChildPayload_size_must_be_0x2C[
    (sizeof(BillChildPayload) == 0x2C) ? 1 : -1];

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

/* Resource-entry work shared by the needle constructor and its renderer. */
typedef struct EffResourceEntry {
    f32 position[3];
    u8 pad0C[4];
    u32 value;
} EffResourceEntry; /* 0x14 */

typedef struct EffResourceWork {
    f32 matrix[16];
    EffResourceEntry *entries;
    u32 entryCount;
    u32 mode;
    f32 scale[3];
    u32 vertexCount;
    f32 (*positions)[4];
    f32 (*normals)[4];
    u32 *colors;
    u32 resource68;
    u32 graphics6C;
    u32 resource70;
} EffResourceWork; /* 0x74 */

/* Exactly the 0x5C bytes copied into the needle's runtime work. */
typedef struct EffPCPNeedleParams {
    f32 position[4];
    u32 count;
    s32 duration;
    u32 mode;
    s32 fadeIn;
    s32 fadeOut;
    u32 randomDelayRange;
    f32 speed;
    f32 acceleration;
    f32 radiusBase;
    f32 radiusJitter;
    f32 angleStep;
    u32 fanSegments;
    f32 radiusScale;
    f32 viewOffset;
    u32 centerColor;
    u32 outerColor;
    f32 width;
    s32 unk54;
    s32 unk58;
} EffPCPNeedleParams;

typedef struct EffPCPNeedleSlot {
    f32 direction[4];
    u8 pad10[0x10];
    s32 age;
    f32 radius;
    f32 angle;
} EffPCPNeedleSlot; /* 0x2C */

typedef struct EffPCPNeedleWork {
    EffPCPNeedleParams params;
    EffPCPNeedleSlot *slots;
    u32 color;
    u32 count;
    u32 system;
    EffResourceWork *resource;
    u32 allocationHandle;
} EffPCPNeedleWork; /* 0x74, followed by count slots */

typedef char EffResourceEntrySizeCheck[sizeof(EffResourceEntry) == 0x14 ? 1 : -1];
typedef char EffResourceWorkSizeCheck[sizeof(EffResourceWork) == 0x74 ? 1 : -1];
typedef char EffPCPNeedleParamsSizeCheck[sizeof(EffPCPNeedleParams) == 0x5C ? 1 : -1];
typedef char EffPCPNeedleSlotSizeCheck[sizeof(EffPCPNeedleSlot) == 0x2C ? 1 : -1];
typedef char EffPCPNeedleWorkSizeCheck[sizeof(EffPCPNeedleWork) == 0x74 ? 1 : -1];

#ifdef VERSION_DDS2
extern EffPCPNeedleWork *func_0017DD50(EffPCPNeedleParams *);
extern EffResourceWork *effCreateResourceEntryWork(s32);
#else
extern EffPCPNeedleWork *func_001760F8(EffPCPNeedleParams *);
extern EffResourceWork *effCreateResourceEntryWork(u32);
#endif

extern void effReleaseOptionalResource(EffResourceWork *);
extern void effReleaseAttachedResources(EffResourceWork *);
extern void effSetResourceEntryPosition(EffResourceWork *, s32, f32 *);
extern void effGetResourceEntryPosition(EffResourceWork *, s32, f32 *);
extern void effSetResourceEntryValue(EffResourceWork *, s32, u32);
extern void effDrawInstancedResourceTrianglesVU(EffResourceWork *);
extern void effBuildRadialFanStreams(EffResourceWork *, u32, u32, u32, f32, f32);

/* Native 0x14-byte timed state embedded in a slot's kind-specific payload. */
typedef struct EffTimedState {
    u32 flags;
    s32 value;
    union {
        s32 delay;
        u32 materialFlags;
    };
    union {
        s32 delayMax;
        u32 materialValue;
    };
    u8 *source;
} EffTimedState;

/* Native 0xA0-byte slot work. Material, grid and textured-surface operations
 * use overlapping payload fields in the same slot, not separate allocations. */
typedef struct BdWork {
    s32 flags;           /* 0x00 */
    s32 xOffset;         /* 0x04 */
    s32 yOffset;         /* 0x08 */
    s32 width;           /* 0x0C */
    s32 height;          /* 0x10 */
    u32 cornerColors[4]; /* 0x14 */
    f32 angleDegrees;    /* 0x24 */
    EffTimedState states[2]; /* 0x28 */
    /* Per-mode words: corner-color offsets, grid x/y/width/height, or bar crop. */
    s32 parameters[4];   /* 0x50: bar crop width uses word 2 */
    void *owner;         /* 0x60 */
    s32 slotIndex;       /* 0x64 */
    union {
        struct {
            u32 rect[4]; /* 0x68 */
            u8 pad78[4];
        } texture;
        struct {
            u8 pad68[4];
            s32 quantizedBounds[4]; /* 0x6C */
        } grid;
    } bounds;
    s32 sourceWidth;     /* 0x7C */
    s32 sourceHeight;    /* 0x80 */
    u32 savedColors[4];  /* 0x84 */
    s32 slotOffset;      /* 0x94 */
    u8 pad98[4];
    union {
        s32 address;
        u32 bits;
        struct BdWork *asset;
    } alternate;        /* 0x9C */
} BdWork;

/* Native 0x80-byte source descriptor; its bounds feed both grid and slot drawing. */
typedef struct EffectSlotDescription {
    u8 pad00[0x14];
    u32 textureIndex;
    s32 flags;
    u8 pad1C[0x10];
    s32 presetMode;
    u8 pad30[4];
    u32 rect[4];
    s32 xOffset;
    s32 yOffset;
    s32 width;
    s32 height;
    s32 colors[4];
    u8 pad64[0x1C];
} EffectSlotDescription;

/* Native 0x30-byte resource-slot owner: source descriptors and live work arrays. */
typedef struct EffectSlotSet {
    u32 sourceAllocation;
    u32 unk04;
    u32 count;
    u32 descriptionAllocation;
    EffectSlotDescription *descriptions;
    u32 workAllocation;
    BdWork *workEntries;
    u32 textureCount;
    u32 textureAllocation;
    void **handles;
    s32 defaultValue;
    u8 pad2C[4];
} EffectSlotSet;

typedef char BdWorkSizeCheck[sizeof(BdWork) == 0xA0 ? 1 : -1];
typedef char EffectSlotDescriptionSizeCheck[sizeof(EffectSlotDescription) == 0x80 ? 1 : -1];
typedef char EffectSlotSetSizeCheck[sizeof(EffectSlotSet) == 0x30 ? 1 : -1];

/* Particle cell shared by the DDS1/2 particle subroutines (0x14 bytes). */
typedef struct ParCell {
    u128 *history;   /* 0x00 */
    void *vertices;  /* 0x04 */
    s32 vertexCount; /* 0x08: processed in groups of three */
    s32 unk0C;       /* 0x0C cleared */
    u32 color;       /* 0x10 initialized to grey 0x80808080 */
} ParCell;

/* Ring-effect geometry: starting angle (-pi/2) and one full turn. */
#define EFFECT_RING_START_ANGLE (-1.5707963f)
#define EFFECT_RING_FULL_TURN (6.2831853f)

/* Mid-grey fragment colour (0x80 in each channel). */
#define EFF_THUNDER_FRAGMENT_GREY 0x80808080

#endif /* EFF_H */
