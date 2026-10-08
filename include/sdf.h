#ifndef SDF_H
#define SDF_H

#include "common.h"

/* Native battle-parameter blobs: DDS1 0xA6C bytes, DDS2 0xC14 bytes.
 * Level tables begin at level one; seven-entry scales use index three for zero. */
typedef struct SdfBattleMoneyRewardBand {
    s8 levelThreshold;
    u8 pad01[3];
    f32 moneyScale;
} SdfBattleMoneyRewardBand;

typedef char SdfBattleMoneyRewardBand_size_must_be_8[(sizeof(SdfBattleMoneyRewardBand) == 8) ? 1 : -1];

typedef struct SdfBattleParameters {
    f32 maxHpGrowth[99]; /* 0x000 */
    f32 maxMpGrowth[99]; /* 0x18C */
    f32 hpGradeValues[8]; /* 0x318 */
    f32 hpFineGradeValues[11]; /* 0x338 */
    f32 levelValuesA[99]; /* 0x364 */
    f32 levelValuesB[99]; /* 0x4F0 */
    f32 levelValuesC[99]; /* 0x67C */
    u8 pad808[0xB8];
    f32 adjustmentScale[7]; /* 0x8C0: signed adjustment -3..3 */
    u8 rewardLevelAllowance; /* 0x8DC: level gap before reducing defeat experience. */
    u8 pad8DD[3];
    f32 rewardDivisor; /* 0x8E0: zero leaves defeat experience unreduced. */
    f32 moneyTurnScales[16]; /* 0x8E4 */
    SdfBattleMoneyRewardBand moneyRewardBands[10]; /* 0x924 */
    f32 rewardLevelScale[31 * 2]; /* 0x974: level difference and reward kind */
#ifdef VERSION_DDS2
    u8 padA6C[0x74];
    f32 partyEntryScaleA[7]; /* 0xAE0 */
    f32 enemyEntryScaleA[7]; /* 0xAFC */
    f32 partyEntryScaleB[7]; /* 0xB18 */
    f32 enemyEntryScaleB[7]; /* 0xB34 */
    u8 padB50[0x1C];
    f32 specialAffinityScale; /* 0xB6C */
    f32 criticalScale; /* 0xB70: special-mode multiplier for the critical roll. */
    u8 padB74[8];
    f32 preemptiveModeScale; /* 0xB7C: encounter-kind-3 preemptive chance scale. */
    f32 majinRewardScale; /* 0xB80: DDS2 battle-mode-3 experience multiplier. */
    u8 padB84[0x14];
    s8 criticalPartyAttackerBias; /* 0xB98: signed critical chance adjustments. */
    s8 criticalPartyDefenderBias; /* 0xB99 */
    u8 padB9A[0xA];
    f32 partyHpScale[10]; /* 0xBA4 */
    f32 enemyHpScale[10]; /* 0xBCC */
    f32 hekatoRatioScale; /* 0xBF4 */
    f32 hekatoRatioMax; /* 0xBF8 */
    f32 unkBFC; /* 0xBFC */
    f32 actionScale; /* 0xC00 */
    f32 brahmaRatioMultiplier; /* 0xC04 */
    f32 brahmaRatioMaximum; /* 0xC08 */
    f32 specialActionScale; /* 0xC0C */
    u8 padC10[4];
#endif
} SdfBattleParameters;

#ifdef VERSION_DDS1
typedef char SdfBattleParameters_size_must_be_0xA6C[(sizeof(SdfBattleParameters) == 0xA6C) ? 1 : -1];
#endif
#ifdef VERSION_DDS2
typedef char SdfBattleParameters_size_must_be_0xC14[(sizeof(SdfBattleParameters) == 0xC14) ? 1 : -1];
#endif

extern SdfBattleParameters *datBattleParameters;

/* Four integer words used as a camera input record (0x10), not ScrVec4 floats. */
typedef struct SdfQuad {
    u32 word[4];
} SdfQuad;

/* Reference-counted texture handle (0x8); DDS1 sdf/sdfTex.c and DDS1/2 SdfTex owners. */
typedef struct SdfTexRef {
    void *unk0; /* Non-null suppresses primary-resource release. */
    s32 refCount;
} SdfTexRef;

/* Texture buffer GPU command words (0x40); DDS1/2 game/code_002D10B0/00329F60.c. */
typedef struct SdfTexBuf {
    s32 gifTagWord; /* Low GIFtag word; NLOOP occupies bits 0-14. */
    u8 pad4[0xC];
    u64 samplingState; /* GS TEX1 data, including minification/magnification filters. */
    u64 unk18;
    u64 textureState; /* GS TEX0 data. */
    u64 unk28;
    u64 clampState; /* GS CLAMP data. */
    u64 clampRegister; /* GS CLAMP_1/CLAMP_2 register selector. */
} SdfTexBuf;

/* Native VRAM range descriptor (0x1C), shared by textures, graph buffers and streams.
 * Allocation mode is an unsigned classification: zero is a free range. */
typedef struct SdfTexResource {
    struct SdfTexResource *next; /* 0x00 */
    struct SdfTexResource *prev; /* 0x04 */
    u32 allocationMode;         /* 0x08 */
    u32 word;                   /* 0x0C: VRAM offset in 32-bit words */
    s32 size;                   /* 0x10: range length in 32-bit words */
    s16 width;                  /* 0x14 */
    s16 height;                 /* 0x16 */
    s32 format;                 /* 0x18: GS pixel-storage mode */
} SdfTexResource;

typedef char SdfTexResource_size_must_be_0x1C[
    (sizeof(SdfTexResource) == 0x1C) ? 1 : -1];
/* IPU register state saved across stream callbacks (0x24 bytes). */
typedef struct IpuDmaState {
    u32 inputAddress;
    u32 inputTagAddress;
    u32 inputQwords;
    u32 inputControl;
    u32 outputAddress;
    u32 outputQwords;
    u32 outputControl;
    u32 bitPointer;
    u32 ipuControl;
} IpuDmaState;

typedef char IpuDmaState_size_must_be_0x24[
    (sizeof(IpuDmaState) == 0x24) ? 1 : -1];

/* One 0x8C allocation owns the stream/sound links and IPU transfer state. */
typedef struct SdfStreamFrameNode {
    struct SdfStreamFrameNode *streamPrev;
    struct SdfStreamFrameNode *streamNext;
    struct SdfStreamFrameNode *next;
    u8 active;
    u8 queued;
    u8 unk0E;
    u8 drained;
    u8 firstStop;
    u8 unk11;
    u8 pad12;
    u8 unk13;
    u8 audioMode;
    u8 loopMode;
    u8 playbackMode;
    u8 pad17;
    u8 bufferIndex;
    u8 pad19;
    u8 unk1A;
    u8 pad1B;
    s32 bufferSize;
    u32 buffers[2];
    s32 textureResources[2];
    u8 pad30[4];
    s32 resourceWord;
    SdfTexResource *textureHead;
    u16 width;
    u16 height;
    u32 cycleLength;
    u32 tickCount;
    s32 unk48; /* Movie progress reader; no producer has been located. */
    u32 unk4C;
    u8 headerReady;
    u8 done;
    u8 filledSlots;
    u8 firstSlot;
    u32 scratchBuffer;
    u8 pad58[4];
    s32 (*read)(struct SdfStreamFrameNode *, u32, s32, void *, s32);
    u32 source;
    u8 pad64;
    u8 unk65;
    u8 pad66[2];
    IpuDmaState dma;
} SdfStreamFrameNode;

typedef char SdfStreamFrameNode_size_must_be_0x8C[
    (sizeof(SdfStreamFrameNode) == 0x8C) ? 1 : -1];
typedef char SdfStreamFrameNode_dma_offset_must_be_0x68[
    ((u32)&((SdfStreamFrameNode *)0)->dma == 0x68) ? 1 : -1];

typedef s32 (*SdfStreamRead)(SdfStreamFrameNode *, u32, s32, void *, s32);

typedef struct SdfMovieDescriptor {
    u16 unk00;
    u16 unk02;
    u32 unk04;
    u16 unk08;
    u16 unk0A;
    s32 source;
    u8 unk10;
    u8 unk11;
    u8 unk12;
    u8 pad13;
} SdfMovieDescriptor;

typedef char SdfMovieDescriptor_size_must_be_0x14[
    (sizeof(SdfMovieDescriptor) == 0x14) ? 1 : -1];

typedef struct MovObj {
    u8 active;
    u8 state;
    u8 stopRequested;
    u8 isPac;
    u16 unk04;
    u16 unk06;
    u32 unk08;
    u16 unk0C;
    u16 unk0E;
    struct DevState *deviceState;
    s32 totalBytes;
    s32 remainingBytes;
    void *stream;
    u8 pacEnabled;
    u8 pad21;
    u8 packetLimit;
    u8 pad23;
    SdfStreamFrameNode soundNode;
} MovObj;

typedef char MovObj_size_must_be_0xB0[
    (sizeof(MovObj) == 0xB0) ? 1 : -1];

void func_002ED8D0(MovObj *, SdfMovieDescriptor *, const char *);
void func_00346778(MovObj *, SdfMovieDescriptor *, const char *);

/* Graph target: two color buffers followed by the auxiliary/depth buffer (0x14). */
typedef struct SdfGraphObj {
    s16 width;
    s16 unk2;
    s16 height;
    u8 bufferFormat;
    u8 auxiliaryFormat;
    SdfTexResource *buffers[3];
} SdfGraphObj;
/* A mode's 12-byte display defaults row, copied into SdfGraphObj. */
typedef struct SdfGraphModeDefaults {
    s16 width;
    s16 unk2;
    s16 height;
    u16 bufferFormat;
    u16 auxiliaryFormat;
    u8 pad0A[2];
} SdfGraphModeDefaults;

typedef char SdfGraphModeDefaults_size_must_be_0xC[
    (sizeof(SdfGraphModeDefaults) == 0xC) ? 1 : -1];

/* Linked texture and its two buffers/resources (0x40); DDS1/2 sdf/sdfTex.c and game texture units. */
typedef struct SdfTex {
    struct SdfTex *next;
    struct SdfTex *prev;
    SdfTexRef *reference;
    s16 width;
    s16 height;
    SdfTexResource *primaryResource;
    SdfTexResource *secondaryResource;
    u8 unk18;
    u8 clutFormat;
    u8 pixelFormat;
    u8 maxMipLevel;
    u16 lodParameters; /* Packed GS TEX1 L/K parameters. */
    u8 unk1E;
    u8 clampMode;
    s32 unk20;
    s32 unk24;
    SdfTexBuf *primaryBuffer;
    SdfTexBuf *secondaryBuffer;
    u8 *data;
    s32 dataSize;
    s32 unk38;
    void *auxiliaryAllocation; /* Owned heap allocation released alongside data. */
} SdfTex;

/* Semaphore ID and attached work pointers (0x14); DDS1/2 game/code_002D10B0/00329F60.c. */
typedef struct SdfSemaObj {
    s32 semaphoreId;
    void *unk4;
    void *releaseTail;
    void *unkC;
    s32 packetTail;
} SdfSemaObj;

/* DMA packet list cursors and endpoints (0x20); DDS1/2 game/code_002D33C8/0032C278.c. */
typedef struct SdfListHead {
    u32 unk0;
    u32 first;
    u32 last;
    u32 unkC;
    u32 firstReferenceSource; /* Optional first DMA reference prefix. */
    u32 secondReferenceSource; /* Optional second DMA reference prefix. */
    u32 unk18;
    u32 unk1C;
} SdfListHead;

/* Draw-surface pool entry (0x20); the SDK initializes and flushes this owner. */
typedef struct SdfPoolNode {
    struct SdfPoolNode *next; /* 0x00 */
    u32 first;               /* 0x04 */
    u32 last;                /* 0x08 */
    u32 unkC;
    void (*append)(SdfListHead *, SdfListHead *);      /* 0x10 */
    s32 (*prepend)(SdfListHead *, s32, SdfListHead *); /* 0x14 */
    u32 unk18;
    u32 unk1C;
} SdfPoolNode;

typedef char SdfPoolNode_size_must_be_0x20[(sizeof(SdfPoolNode) == 0x20) ? 1 : -1];

/* Callback-list hooks also accept the shared task-entry no-op. */
typedef void (*SdfListCallback)();

typedef struct SdfListNode {
    u32 index;
    s32 key;
    struct SdfListNode *next;
    struct SdfListNode *prev;
    void *value;
} SdfListNode;

typedef struct SdfList {
    u32 allocation;
    u32 count;
    SdfListNode *head;
    SdfListNode *tail;
    void *userData;
    SdfListCallback onRemove;
    SdfListCallback onDestroy;
} SdfList;

typedef char SdfListNode_size_must_be_0x14[(sizeof(SdfListNode) == 0x14) ? 1 : -1];
typedef char SdfList_size_must_be_0x1C[(sizeof(SdfList) == 0x1C) ? 1 : -1];

SdfList *sdfCreateTaskHeader(void *);
void sdfDestroyTaskWork(SdfList *);
void sdfSetTaskDestroyCallback(SdfList *, SdfListCallback);
void sdfSetTaskSecondaryCallback(SdfList *, SdfListCallback);
SdfListNode *sdfListAppend(SdfList *, s32, void *);
SdfListNode *sdfListInsertAfter(SdfList *, SdfListNode *, s32, void *);
SdfListNode *sdfRemoveAndReindexListNode(SdfList *, SdfListNode *);
SdfListNode *sdfListRemoveNode(SdfList *, SdfListNode *);
void sdfClearTaskList(SdfList *);
void sdfSwapLinkedListNodes(SdfList *, SdfListNode *, SdfListNode *);
SdfListNode *sdfFindTaskListNodeByKey(SdfList *, s32);

/* Four-doubleword DMA packet payload (0x20); DDS1/2 game/code_002D33C8/0032C278.c. */
typedef struct SdfPacket {
    u64 unk0;
    u64 unk8;
    u64 unk10;
    u64 unk18;
} SdfPacket;

/* DMA source header and trailing 64-bit field (0x10); DDS1/2 game/code_002D33C8/0032C278.c. */
typedef struct SdfDmaSrc {
    u16 quadwordCount;
    u8 pad2[6];
    u64 vifCommands;
} SdfDmaSrc;

/* DMA node with a 128-bit command (0x20); DDS1/2 game/code_002D33C8/0032C278.c. */
typedef struct SdfDmaNode {
    u64 unk0;
    u64 unk8;
    int __attribute__((mode(TI))) unk10;
} SdfDmaNode;

/* Resource entry with word at +0xC (0x10); DDS1/2 game/code_002D33C8/0032C278.c. */
typedef struct SdfResEntry {
    u8 pad00[0xC];
    u32 baseAddress; /* Shifted right six bits when patching a GS texture base. */
} SdfResEntry;

/* Large DMA packet fields at +0x30/+0x80 (0x88); DDS1/2 game/code_002D33C8/0032C278.c. */
typedef struct SdfBigPacket {
    u8 pad00[8];
    s32 resourceIndexXor;
    u8 pad0C[0x24];
    u64 unk30; /* Low 14 bits receive the indexed texture base. */
    u8 pad38[0x48];
    u64 unk80; /* Low 14 bits receive the indexed texture base. */
} SdfBigPacket;

/* Two-slot packet builder and source/mode state (0x60); DDS1/2 game/code_002D33C8/0032C278.c. */
typedef struct SdfPacketBuilder {
    u8 pad00[4];
    void (*prepare)(void);
    u8 pad08[8];
    SdfPacket packets[2];
    s32 source;
    s32 data;
    s32 region;
    s32 mode;
} SdfPacketBuilder;

/* Linked named resource (0x24); DDS1/2 game/code_002D33C8/0032C278.c. */
typedef struct SdfResource {
    u32 unk00;
    struct SdfResource *next;
    u8 pad08[0x18];
    s32 id;
} SdfResource;

/* Graphics packet header (0x10); DDS1/2 game/code_002D9748/003325F8.c. */
typedef struct SdfNode {
    u16 unk0;
    u8 unk2;
    u8 unk3;
    u32 unk4;
    u32 unk8;
    u32 unkC;
} SdfNode;

/* Asset holding texture and resource entries (0x48); DDS1/2 game/code_002D9748/003325F8.c. */
typedef struct SdfAsset {
    u8 pad00[8];
    void *entries[2];
    u32 unk10;
    u32 unk14;
    u32 unk18;
    f32 unk1C;
    u32 unk20;
    u32 unk24;
    u32 unk28;
    SdfTex *texture;
    SdfTex *secondaryTexture; /* 0x30 */
    u32 secondaryMode;       /* 0x34: secondary packet and palette selector */
    void *third;
    void *fourth;
    f32 unk40;
    f32 unk44;
} SdfAsset;

/* Draw entries store six transform floats and expose the same bytes as three qwords. */
typedef union SdfDrawTransform {
    f32 m[6];
    u64 words[3];
} SdfDrawTransform;

/* Native 0xA0-byte draw-entry block allocated by SDF_ASSET_DRAW_ENTRY_BYTES. */
typedef struct SdfAssetEntry {
    u32 pad00;             /* 0x00 */
    u32 unk04;             /* 0x04 */
    u32 unk08;             /* 0x08 */
    u32 unk0C;             /* 0x0C */
    u32 unk10;             /* 0x10 */
    u32 unk14;             /* 0x14 */
    u32 pad18;             /* 0x18 */
    f32 unk1C;             /* 0x1C */
    u32 unk20;             /* 0x20 */
    u32 mode;              /* 0x24 */
    f32 y;                 /* 0x28 */
    f32 x;                 /* 0x2C */
    u8 pad30[8];           /* 0x30 */
    u64 unk38;             /* 0x38 */
    u64 unk40;             /* 0x40 */
    u64 unk48;             /* 0x48 */
    u64 unk50;             /* 0x50 */
    u64 unk58;             /* 0x58 */
    u64 unk60;             /* 0x60 */
    SdfDrawTransform transforms[2]; /* 0x68 */
    u32 unk98;             /* 0x98 */
    u32 unk9C;             /* 0x9C */
} SdfAssetEntry;

typedef char SdfAssetEntry_size_must_be_0xA0[
    (sizeof(SdfAssetEntry) == 0xA0) ? 1 : -1];

/* Linked thread registry entry (0x8); DDS1/2 sdfThread and thread-control units. */
typedef struct SdfThreadNode {
    struct SdfThreadNode *next; /* 0x00 */
    s32 threadId;               /* 0x04 */
} SdfThreadNode;

extern s32 sdfTrackedThreadSemaphore;
extern SdfThreadNode *sdfTrackedThreadHead;

/* General-heap allocation descriptor (0x10); links bound the represented span. */
typedef struct SdfMemBlock {
    struct SdfMemBlock *prev; /* 0x00 */
    struct SdfMemBlock *next; /* 0x04 */
    s32 address;              /* 0x08: start of represented span */
    u16 state;                /* 0x0C: free, used, or end sentinel */
    s16 referenceCount;       /* 0x0E: -1 for sentinels */
} SdfMemBlock;

/* Embedded end sentinels and backing-span metadata for the general heap (0x28). */
typedef struct SdfMemHeap {
    SdfMemBlock head; /* 0x00: low-address sentinel */
    SdfMemBlock tail; /* 0x10: high-address sentinel */
    u32 base;         /* 0x20: unaligned backing allocation */
    u32 size;         /* 0x24 */
} SdfMemHeap;

typedef char SdfMemBlock_size_must_be_0x10[(sizeof(SdfMemBlock) == 0x10) ? 1 : -1];
typedef char SdfMemHeap_size_must_be_0x28[(sizeof(SdfMemHeap) == 0x28) ? 1 : -1];

extern SdfMemHeap sdfGeneralHeap;

#define SDF_CHIP_CLASS_COUNT 7

typedef struct SdfChipCell SdfChipCell;
typedef struct SdfChipPage SdfChipPage;
typedef struct SdfChipClass SdfChipClass;
typedef struct SdfPendingNode SdfPendingNode;

/* A free chip cell carries its next-list link at byte 8. */
struct SdfChipCell {
    u8 pad00[8];
    SdfChipCell *nextFree;
};

/* One power-of-two allocation class in the chip heap (0x0C). */
struct SdfChipClass {
    SdfChipPage *currentPage;
    SdfChipPage *availablePages;
    s16 cellSize;
    s16 cellCount;
};

typedef SdfChipCell *(*SdfChipAllocateFn)(SdfChipClass *, SdfChipPage *);

/* Per-page allocation metadata for one 4 KiB chip-heap page (0x18). */
struct SdfChipPage {
    SdfChipPage *next;
    u8 *base;
    SdfChipAllocateFn allocate;
    SdfChipClass *sizeClass;
    SdfChipCell *freeCells;
    s16 usedCells;
    s16 bumpCellsRemaining;
};

/* Interrupt-synchronized deferred callback owner (0x08). */
typedef struct SdfPendingRequest {
    void (*handler)(u32);
    SdfPendingNode *pending;
} SdfPendingRequest;

typedef char SdfChipClass_size_must_be_0x0C[(sizeof(SdfChipClass) == 0x0C) ? 1 : -1];
typedef char SdfChipPage_size_must_be_0x18[(sizeof(SdfChipPage) == 0x18) ? 1 : -1];
typedef char SdfPendingRequest_size_must_be_0x08[(sizeof(SdfPendingRequest) == 0x08) ? 1 : -1];

/* Seven size classes followed by the retail range's four-byte alignment tail. */
typedef struct SdfChipClassTable {
    SdfChipClass classes[SDF_CHIP_CLASS_COUNT];
    u8 pad54[4];
} SdfChipClassTable;

typedef char SdfChipClassTable_size_must_be_0x58[(sizeof(SdfChipClassTable) == 0x58) ? 1 : -1];

extern SdfChipPage *sdfChipPages;
extern SdfChipPage *sdfFreeChipPages;
extern u8 *sdfChipHeapStart;
extern u8 *sdfChipHeapEnd;
extern s32 sdfChipPageCount[2];
extern SdfPendingRequest sdfChipReleaseRequest;
extern SdfChipClassTable sdfChipClassTable;

/* The flag-list factory appends this owner after its vertex, color and mark
 * arrays. Its parameter record is copied whole into the owner. */
typedef struct SdfColorTrack {
    u8 mode;
    u8 pad01[3];
    u32 colorA;
    u32 colorB;
    u32 colorC;
    f32 fractionA;
    u32 colorD;
    f32 fractionB;
    u8 pad1C[8];
} SdfColorTrack;

typedef struct SdfAlphaTrack {
    u32 alpha;
    s32 surfaceIndex;
    f32 fadeIn;
    f32 fadeOut;
} SdfAlphaTrack;

typedef struct SdfFlagListParams {
    SdfColorTrack color;
    SdfAlphaTrack alpha;
    s32 maxFrames;
    u32 count;
    f32 speed;
} SdfFlagListParams;

typedef struct SdfFlagListMark {
    s32 timer;
    u8 alpha;
    u8 pad05[3];
} SdfFlagListMark;

typedef struct SdfFlagListWork {
    s32 frame;
    u32 unk04;
    SdfFlagListMark *marks;
    f32 (*vertices)[4];
    u32 *colors;
    SdfFlagListParams params;
    u32 resource;
} SdfFlagListWork;

typedef char SdfColorTrack_size_must_be_0x24[(sizeof(SdfColorTrack) == 0x24) ? 1 : -1];
typedef char SdfAlphaTrack_size_must_be_0x10[(sizeof(SdfAlphaTrack) == 0x10) ? 1 : -1];
typedef char SdfFlagListParams_size_must_be_0x40[(sizeof(SdfFlagListParams) == 0x40) ? 1 : -1];
typedef char SdfFlagListMark_size_must_be_0x08[(sizeof(SdfFlagListMark) == 0x08) ? 1 : -1];
typedef char SdfFlagListWork_size_must_be_0x58[(sizeof(SdfFlagListWork) == 0x58) ? 1 : -1];

void sdfResetFlagListEntries(SdfFlagListWork *);
void sdfReleaseFlagListResource(SdfFlagListWork *);
SdfFlagListWork *sdfInitializeFlagListFromResource(void *);
SdfFlagListWork *effCreateSelectionFlagListFromWork(const SdfFlagListParams *);
SdfFlagListWork *effCreateSelectionFlagListFromFile(void *);
SdfFlagListWork *effCreateEmbeddedSelectionFlagList(SdfFlagListWork *);
void effReleaseSelectionFlagList(SdfFlagListWork *);
void effResetSelectionEntryBuffers(SdfFlagListWork *);
void effResetSelectionEntriesAndState(SdfFlagListWork *);
void effDrawSelectionEntryVectors(SdfFlagListWork *);
void effUpdateAndDrawSelectionEntries(SdfFlagListWork *);
float scrGetOperandFloatValue(SdfFlagListWork *);
void scrSetOperandFloatValue(SdfFlagListWork *, float);
void itfSetPackedRgbAlpha(SdfFlagListWork *, u32);
void itfCopyColorFields(SdfFlagListWork *, const SdfFlagListParams *);

#ifdef VERSION_DDS1
SdfFlagListWork *func_002CEAE8(const SdfFlagListParams *);
void func_002CEC40(SdfFlagListWork *);
void func_002CF248(SdfFlagListWork *);
void effUpdateSelectionEntryState(SdfFlagListWork *);
#elif defined(VERSION_DDS2)
SdfFlagListWork *func_00316528(const SdfFlagListParams *);
void func_00316680(SdfFlagListWork *);
void func_00316C88(SdfFlagListWork *);
void func_002DEC08(SdfFlagListWork *);
#endif


/* SDF chunk fourCC values (little-endian byte order). */
#define SDF_CHUNK_UNIQUE_VALUE 0x51494e55 /* "UNIQ" in little-endian byte order */
#define SDF_CHUNK_MAP_POSITIONS 0x534F504D /* "MPOS" in little-endian byte order */
#define SDF_CHUNK_LOD_VALUE 0x43444f4c /* "LODC" in little-endian byte order */

/* Free-list kind for SDF pools. */
#define SDF_POOL_FREE_KIND 0xFFFF

#endif /* SDF_H */
