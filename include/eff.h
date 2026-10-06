#ifndef EFF_H
#define EFF_H

#include "common.h"
#include "sdf.h"

struct EffNode;

struct EffRequest;

/* Resource jobs retain a destination slot until their request completes. */
typedef struct EffectListNode {
    u32 state;
    struct EffectListNode *next;
    u32 value;
    u32 length;
    u32 kind;
    void **reference;
} EffectListNode;

typedef struct EffectList {
    u32 mode;
    s32 count;
    EffectListNode *first;
    EffectListNode *last;
    struct EffRequest *request;
} EffectList;

typedef char EffectListNode_size_must_be_0x18[(sizeof(EffectListNode) == 0x18) ? 1 : -1];
typedef char EffectList_size_must_be_0x14[(sizeof(EffectList) == 0x14) ? 1 : -1];

/* History arrays precede this 0x38-byte owner in one retained allocation. */
typedef struct EffMagatuhiValueWork {
    s32 count;
    u16 historyCount;
    u8 pad06[2];
    f32 unk08;
    f32 unk0C;
    u32 unk10;
    f32 (*positions)[4];
    u32 *colorTable;
    f32 *unk1C;
    u32 *slotColors;
    u16 *writeIndices;
    u16 *validCounts;
    f32 (*angleRows)[4];
    u32 texture;
    void *allocationHandle;
} EffMagatuhiValueWork;

/* Seven copied words; the parent appends its independently allocated history. */
typedef struct EffMagatuhiHistoryParams {
    s32 count;
    s32 historyCount;
    f32 unk08;
    u32 colorA;
    u32 colorB;
    s32 unk14;
    f32 unk18;
} EffMagatuhiHistoryParams;

typedef struct EffMagatuhiOwner {
    EffMagatuhiHistoryParams params;
    EffMagatuhiValueWork *valueWork;
} EffMagatuhiOwner;

typedef char EffMagatuhiValueWork_size_must_be_0x38[(sizeof(EffMagatuhiValueWork) == 0x38) ? 1 : -1];
typedef char EffMagatuhiHistoryParams_size_must_be_0x1C[(sizeof(EffMagatuhiHistoryParams) == 0x1C) ? 1 : -1];
typedef char EffMagatuhiOwner_size_must_be_0x20[(sizeof(EffMagatuhiOwner) == 0x20) ? 1 : -1];

EffMagatuhiOwner *effCloneMagatuhiWithColorResource(const u32 *source);
void effReleaseMagatuhiOwner(EffMagatuhiOwner *owner);
void effMagatuhiReleaseResource(EffMagatuhiValueWork *work);
void effMagatuhiSetValue(EffMagatuhiValueWork *work, s32 index, u32 color);
void effMagatuhiFillColorTable(EffMagatuhiValueWork *work, u32 colorA, u32 colorB);
#ifdef VERSION_DDS2
EffMagatuhiValueWork *func_00190E58(s32, s32, f32, s32, f32);
void func_00190DE0(EffMagatuhiOwner *owner);
void func_00190DF8(EffMagatuhiValueWork *work, s32 index);
void func_00191450(EffMagatuhiValueWork *work, s32 index, void *vector);
#else
EffMagatuhiValueWork *func_00189220(s32, s32, f32, s32, f32);
void func_001891A8(EffMagatuhiOwner *owner);
void func_001891C0(EffMagatuhiValueWork *work, s32 index);
void func_00189818(EffMagatuhiValueWork *work, s32 index, void *vector);
#endif

struct MdlCtx;
struct EffTrackPolyData;

/* Thirteen copied words, followed by an update counter and owned history. */
typedef struct {
    struct MdlCtx *model;
    s32 idA;
    s32 idB;
    f32 unk0C;
    f32 unk10;
    u32 sampleInterval;
    s32 historyLength;
    u32 unk1C;
    u32 kind;
    u32 gradientColors[4];
} EffTrackPolyParams;

typedef struct EffTrackPolyWork {
    EffTrackPolyParams params;
    u32 updateCount;
    struct EffTrackPolyData *data;
} EffTrackPolyWork;

typedef char EffTrackPolyParams_size_must_be_0x34[(sizeof(EffTrackPolyParams) == 0x34) ? 1 : -1];
typedef char EffTrackPolyWork_size_must_be_0x3C[(sizeof(EffTrackPolyWork) == 0x3C) ? 1 : -1];

EffTrackPolyWork *effTrackPolyCreateWork(EffTrackPolyParams *src);
void effTrackPolyRelease(EffTrackPolyWork *work);
void effTrackPolyUpdate(EffTrackPolyWork *work);

/* Each mapped record owns status storage expanded while its resource is loaded. */
typedef struct EffMappedRecord {
    u8 pad00[0x14];
    u32 category;
    u32 statusBytes;
    u8 pad1C[4];
    u8 *status;
} EffMappedRecord;

typedef struct EffMappedResource {
    s32 count;
    u32 allocation;
    EffMappedRecord *records;
} EffMappedResource;

typedef char EffMappedRecord_size_must_be_0x24[(sizeof(EffMappedRecord) == 0x24) ? 1 : -1];
typedef char EffMappedResource_size_must_be_0x0C[(sizeof(EffMappedResource) == 0x0C) ? 1 : -1];

void effCopyVectorToNodeInstance(struct EffNode *node, const void *vector);
void effApplyNodeTransformMatrix(struct EffNode *node, const void *matrix);

typedef struct EffBezierPoint {
    f32 x;
    f32 y;
    f32 z;
} EffBezierPoint;

/* Two consecutive cubic segments share seven control points in a 0x60-byte slot. */
typedef struct EffSegmentedBezierSlot {
    EffBezierPoint controlPoints[7];
    u32 pointIndex; /* First control point, not a segment ordinal. */
    f32 t;
    f32 parameterStep;
} EffSegmentedBezierSlot;

extern s32 effStepBezierSlotSegment(EffSegmentedBezierSlot *slot, f32 *out);

/* Parameter head (0x54 bytes) of the fragment effect, copied verbatim into the work. */
typedef struct {
    f32 start[4];
    f32 end[4];
    u16 systemParam;         /* 0x20 */
    u8 pad22[2];
    u32 fragmentCount;       /* 0x24 */
    u32 restartFrameLimit;  /* 0x28: zero permits unlimited restarts */
    f32 waveAmplitude;      /* 0x2C: sinusoidal displacement */
    u32 startDelayRange;     /* 0x30 modulus of delayFrames */
    u32 activeFrameRange;    /* 0x34 modulus of activeFrames before adding one */
    u16 halfLife;            /* 0x38 */
    u8 pad3A[2];
    f32 coreWidth;          /* 0x3C: half-width of the primary strip */
    u32 arg40;               /* 0x40 */
    f32 bandWidth;          /* 0x44: width of the continued strip */
    u32 arg48;               /* 0x48 */
    f32 edgeWidth;          /* 0x4C: added outer margin */
    u32 arg50;               /* 0x50 */
} EffThunderFragmentParams;

typedef struct {
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
} EffResourceRectBounds;

/* Passed directly to the quad renderer, independently of the extent/center. */
typedef struct {
    u8 color[4];
    s32 blendControl;
    EffResourceRectBounds bounds;
} EffResourceRectDrawParams;

/* Nine copied words; the selected source handle belongs to the owner. */
typedef struct {
    s32 extent;
    s32 centerX;
    s32 centerY;
    EffResourceRectDrawParams draw;
} EffResourceRectParams; /* 0x24 */

typedef struct {
    EffResourceRectParams params;
    u32 sourceHandle;
} EffResourceRectWork; /* 0x28 */

typedef char EffResourceRectDrawParams_size_must_be_0x18[(sizeof(EffResourceRectDrawParams) == 0x18) ? 1 : -1];
typedef char EffResourceRectParams_size_must_be_0x24[(sizeof(EffResourceRectParams) == 0x24) ? 1 : -1];
typedef char EffResourceRectWork_size_must_be_0x28[(sizeof(EffResourceRectWork) == 0x28) ? 1 : -1];
typedef char EffResourceRectDrawParams_layout[((u32)&((EffResourceRectParams *)0)->draw == 0xC && (u32)&((EffResourceRectDrawParams *)0)->blendControl == 4 && (u32)&((EffResourceRectDrawParams *)0)->bounds == 8) ? 1 : -1];

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
    u8 unk28[0xC];
    struct EffWork *prev; /* Previous node in the file-resource chain (+0x34). */
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

/* Header for count 0x6C-byte records; allocation is retained as a resource handle. */
typedef struct EffPayload {
    u32 allocation;
    u32 count;
    u8 *records;
} EffPayload;

/* Two-child draw descriptor embedded in the billboard instance at +0x34. */
typedef struct BillRenderPair {
    struct BillChildPayload *children[2];
    u16 unk8;
    u16 kind;
    u32 colors[2];
    SdfListHead *packetList; /* 0x14 */
    struct BillRenderPair *next; /* 0x18 */
} BillRenderPair;

typedef char BillRenderPair_size_must_be_0x1C[
    (sizeof(BillRenderPair) == 0x1C) ? 1 : -1];

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
    u16 unk50;
    u8 pad52[2];
    u32 modeFlags; /* 0x54: bits 0x40/0x80 select billboard entry modes */
    u32 unk58;
    s32 entryCount;
    void *unk60;
} BillObj;

typedef char BillObj_size_must_be_0x64[
    (sizeof(BillObj) == 0x64) ? 1 : -1];

/* Serialized entry offsets are relative to BillData.base. */
typedef struct BillAnimationEntry {
    s32 offset;
    s32 colorOffset;
    u32 unk8;
    u32 frameCount;
    u32 flags;
} BillAnimationEntry;

/* Ordinary animation frames contain geometry and texture coordinates. */
typedef struct BillRecord {
    s16 width;
    s16 height;
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

/* The 0x10000000 animation format is a list of positioned, delayed entry
 * references. Its trailing sixteen serialized bytes have no established use. */
typedef struct BillPluralRecord {
    s16 entryIndex; /* 0x00 */
    s16 delay;      /* 0x02 */
    s16 x;          /* 0x04 */
    s16 y;          /* 0x06 */
    u8 unk08[0x10];
} BillPluralRecord;

typedef char BillPluralRecord_size_must_be_0x18[
    (sizeof(BillPluralRecord) == 0x18) ? 1 : -1];

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
    u16 components[8];
} BillTextureQuad;

typedef char BillTextureQuad_size_must_be_0x10[
    (sizeof(BillTextureQuad) == 0x10) ? 1 : -1];

/* Fifteen billboard quads precede the child manager in its 0x450 allocation. */
typedef struct BillPacketWork {
    f32 positions[15][4]; /* 0x000 */
    u32 colors[15];       /* 0x0F0 */
    BillTextureQuad uv[15]; /* 0x12C */
    f32 offsets[15][8];   /* 0x21C: four X/Y corner offsets per quad */
    s32 count;           /* 0x3FC */
} BillPacketWork;

typedef char BillPacketWork_size_must_be_0x400[
    (sizeof(BillPacketWork) == 0x400) ? 1 : -1];

/* Child manager at retained allocation +0x400; shared by resource and draw owners. */
typedef struct BillChildPayload {
    s32 value; /* 0x00: acquired SdfTex address; preserve the existing word-access contract. */
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
    SdfListHead *pendingLists[5]; /* 0x2C */
    u16 packetListIndex; /* 0x40 */
    u8 pad42[2];
    void *allocation; /* 0x44: original allocation handle */
    BillPacketWork *work; /* 0x48: retained packet-buffer address */
    struct BillChildPayload *next; /* 0x4C */
} BillChildPayload;

typedef char BillChildPayload_size_must_be_0x50[
    (sizeof(BillChildPayload) == 0x50) ? 1 : -1];

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

/* Position/color arrays precede this render pool; constructors clear the full allocation. */
typedef struct EffRecordPool {
    f32 matrix[16];
    f32 origin[3];
    u8 pad4C[4];
    u32 drawMode; /* 0x50: packet submission surface */
    u32 color;
    s32 vertexCount; /* Positions/color words, not group count. */
    f32 scale;
    s32 recordBase;
    s32 auxRecordBase;
    u32 resource;
    SdfMemBlock *buffer;
} EffRecordPool; /* 0x70 */

typedef struct EffRingParticle {
    u32 color;
    s32 age;
    f32 angle;
    f32 basisFactor;
} EffRingParticle;

/* The ring allocator copies 0x58 bytes of parameters into this 0x80-byte
 * owner. Its update passes this same work to the phase, color and fan helpers. */
typedef struct EffRingWork {
    f32 origin[4];
    u32 count;
    u8 respawn;
    u8 pad15[3];
    s32 duration;
    s32 spread;
    u32 fadeIn;
    u32 fadeOut;
    u32 firstColor;
    u32 secondColor;
    f32 param30;
    f32 param34;
    f32 param38;
    f32 param3C;
    f32 param40;
    f32 param44;
    f32 param48;
    f32 param4C;
    f32 increment;
    u32 drawMode;
    EffRingParticle *vertices;
    u32 updateCount;
    u32 color;
    f32 scale;
    f32 unk68;
    f32 unk6C;
    f32 unk70;
    f32 unk74;
    SdfMemBlock *allocationHandle;
    EffRecordPool *recordPool;
} EffRingWork;

typedef char EffRingParticleSizeCheck[sizeof(EffRingParticle) == 0x10 ? 1 : -1];
typedef char EffRingWorkSizeCheck[sizeof(EffRingWork) == 0x80 ? 1 : -1];

typedef struct PcpFlashPulseParticle {
    u32 color;
    s32 age;
    f32 scale;
    u8 pad0C[4];
} PcpFlashPulseParticle;

/* effFlashRecordCreate copies the 0x30-byte parameter prefix, then places
 * these 0x10-byte particles immediately after the 0x48-byte work. */
typedef struct PcpFlashTrianglePulseWork {
    f32 origin[3];
    u8 pad0C[4];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[3];
    s32 lifetime;
    s32 scaleRampTime;
    u32 colorA;
    u32 colorB;
    f32 maxScale;
    u32 drawMode;
    PcpFlashPulseParticle *parts;
    s32 updateCount;
    u32 tintColor;
    f32 renderScale;
    SdfMemBlock *allocationHandle;
    EffRecordPool *resourceHandle;
} PcpFlashTrianglePulseWork;

typedef char PcpFlashPulseParticleSizeCheck[sizeof(PcpFlashPulseParticle) == 0x10 ? 1 : -1];
typedef char PcpFlashTrianglePulseWorkSizeCheck[sizeof(PcpFlashTrianglePulseWork) == 0x48 ? 1 : -1];

/* Shared scatter texture reference, created and released independently of pools. */
typedef struct PcpScatterRes {
    u32 textureHandle;
    s32 refCount;
} PcpScatterRes; /* 0x08 */

/* Appended after the two record arrays; resource helpers own this same control block. */
typedef struct PcpScatterPool {
    f32 origin[4];
    u32 unk10;
    u32 color;
    s32 secondWordCount;
    f32 unk1C;
    s32 recordBase;
    s32 auxRecordBase;
    u32 drawAsset;
    SdfMemBlock *allocation;
    PcpScatterRes *sharedResource;
} PcpScatterPool; /* 0x34 */



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
    SdfMemBlock *allocationHandle; /* Descriptor, not the retained data address. */
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
    s32 unk98;          /* 0x98: initialized from the source descriptor's final halfword. */
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
    u32 cornerColors[4]; /* 0x64: reordered into the live/saved corner colors. */
    s32 widthOverride;   /* 0x74: zero selects the source bounds' width. */
    s32 heightOverride;  /* 0x78: zero selects the source bounds' height. */
    u16 initialDelay;    /* 0x7C: loaded into each timed state's delay before its +1. */
    u16 unk7E;
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

/* Shared paired-chain parameter and resource owners. Points are filled before rendering. */
typedef struct EffThunderGroupParams {
    f32 points[10][4];
    s32 count;
    EffThunderFragmentParams params;
} EffThunderGroupParams;
typedef struct EffThunderGroup EffThunderGroup;

typedef struct PairedEffectParams {
    f32 startVec[4];
    EffThunderFragmentParams fragment;
    s32 total;
    s32 fadeIn;
    s32 fadeOut;
} PairedEffectParams;

typedef struct PairedEffectResources {
    f32 startVec[4];
    EffThunderFragmentParams fragment;
    s32 total;
    s32 fadeIn;
    s32 fadeOut;
    EffThunderGroup *resource[2];
    s32 frame;
    u32 colorWithAlpha;
} PairedEffectResources;

typedef char EffThunderGroupParamsSizeCheck[sizeof(EffThunderGroupParams) == 0xF8 ? 1 : -1];
typedef char EffThunderGroupParamsCountOffsetCheck[((u32)&((EffThunderGroupParams *)0)->count == 0xA0) ? 1 : -1];
typedef char EffThunderGroupParamsFragmentOffsetCheck[((u32)&((EffThunderGroupParams *)0)->params == 0xA4) ? 1 : -1];
typedef char PairedEffectParamsSizeCheck[sizeof(PairedEffectParams) == 0x70 ? 1 : -1];
typedef char PairedEffectResourcesSizeCheck[sizeof(PairedEffectResources) == 0x80 ? 1 : -1];
typedef char PairedEffectFragmentOffsetCheck[((u32)&((PairedEffectResources *)0)->fragment == 0x10) ? 1 : -1];
typedef char PairedEffectResourcesOffsetCheck[((u32)&((PairedEffectResources *)0)->resource == 0x70) ? 1 : -1];
typedef char PairedEffectFrameOffsetCheck[((u32)&((PairedEffectResources *)0)->frame == 0x78) ? 1 : -1];
typedef char PairedEffectColorOffsetCheck[((u32)&((PairedEffectResources *)0)->colorWithAlpha == 0x7C) ? 1 : -1];

#endif /* EFF_H */
