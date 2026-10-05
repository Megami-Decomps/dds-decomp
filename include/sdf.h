#ifndef SDF_H
#define SDF_H

#include "common.h"

/* Native 0x90-byte camera/viewport owner shared by both projection builders. */
typedef struct SdfCamera {
    u32 flags;       // 0x00: 1 half-height, 2 field-of-view projection
    f32 aspect;      // 0x04
    f32 scale;       // 0x08
    f32 fov;         // 0x0C
    f32 offsetX;     // 0x10
    f32 offsetY;     // 0x14
    f32 width;       // 0x18
    f32 height;      // 0x1C
    f32 top;         // 0x20
    f32 bottom;      // 0x24
    f32 nearZ;       // 0x28
    f32 farZ;        // 0x2C
    u8 matrix[0x40]; // 0x30
    f32 halfWidth;   // 0x70
    f32 halfHeight;  // 0x74
    f32 centerY;     // 0x78
    f32 one;         // 0x7C
    f32 originX;     // 0x80
    f32 originY;     // 0x84
    f32 bottomY;     // 0x88
    u32 zero;        // 0x8C
} SdfCamera;

typedef char SdfCamera_size_must_be_0x90[(sizeof(SdfCamera) == 0x90) ? 1 : -1];

extern SdfCamera sdfSceneProjectionParameters;
#ifdef VERSION_DDS1
extern SdfCamera D_00324980;
#elif defined(VERSION_DDS2)
extern SdfCamera D_0037F980;
#endif
extern void sdfCameraBuildProjection(SdfCamera *);

/* Native battle-parameter blobs: DDS1 0xA6C bytes, DDS2 0xC14 bytes.
 * Level tables begin at level one; seven-entry scales use index three for zero. */
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
    u8 pad8DC[0x98];
    f32 rewardLevelScale[31 * 2]; /* 0x974: level difference and reward kind */
#ifdef VERSION_DDS2
    u8 padA6C[0x74];
    f32 partyEntryScaleA[7]; /* 0xAE0 */
    f32 enemyEntryScaleA[7]; /* 0xAFC */
    f32 partyEntryScaleB[7]; /* 0xB18 */
    f32 enemyEntryScaleB[7]; /* 0xB34 */
    u8 padB50[0x1C];
    f32 specialAffinityScale; /* 0xB6C */
    u8 padB70[0x34];
    f32 partyHpScale[10]; /* 0xBA4 */
    f32 enemyHpScale[10]; /* 0xBCC */
    f32 hekatoRatioScale; /* 0xBF4 */
    f32 hekatoRatioMax; /* 0xBF8 */
    u8 padBFC[4];
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

/* Texture resource word at +0xC (0x10); DDS1/2 game/code_002D10B0/00329F60.c via SdfTex. */
typedef struct {
    u8 pad00[0xC];
    u32 word;
} SdfTexResource;

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

/* Asset entry with packed payload words (0x50); DDS1/2 game/code_002D9748/003325F8.c. */
typedef struct SdfAssetEntry {
    u32 pad00;
    u32 unk04;
    u32 unk08;
    u32 pad0C;
    u32 unk10;
    u32 unk14;
    u32 pad18;
    f32 unk1C;
    u8 pad20[0x18];
    u64 unk38;
    u64 unk40;
    u64 unk48;
} SdfAssetEntry;

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

/* Draw-node vector slots (SdfDrawNode vectors array indices). */
#define SDF_DRAW_TRANSLATION_VECTOR 0
#define SDF_DRAW_SCALE_VECTOR 1
#define SDF_DRAW_X_AXIS_VECTOR 2
#define SDF_DRAW_Y_AXIS_VECTOR 3
#define SDF_DRAW_Z_AXIS_VECTOR 4

/* SDF chunk fourCC values (little-endian byte order). */
#define SDF_CHUNK_UNIQUE_VALUE 0x51494e55 /* "UNIQ" in little-endian byte order */
#define SDF_CHUNK_MAP_POSITIONS 0x534F504D /* "MPOS" in little-endian byte order */
#define SDF_CHUNK_LOD_VALUE 0x43444f4c /* "LODC" in little-endian byte order */

/* Free-list kind for SDF pools. */
#define SDF_POOL_FREE_KIND 0xFFFF

/* Alternate item setup mode for SDF model entries. */
#define SDF_MODEL_ALTERNATE_ITEM_SETUP 4

#endif /* SDF_H */
