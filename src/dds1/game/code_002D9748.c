#include "common.h"
#include "sdf.h"
#include "pcp_vu0.h"

#define SDF_CHUNK_NAMED_IDS 0x4D4E444E
#define SDF_ASSET_LIST_MIN_CAPACITY 0x20

/* Four change groups, with adjacent bits for the two draw entries. */
#define SDF_ASSET_PRIMARY_STATE_BIT 1
#define SDF_ASSET_PRIMARY_SCALARS_BIT 4
#define SDF_ASSET_SECONDARY_STATE_BIT 16
#define SDF_ASSET_PAIR_STATE_BIT 64
#define SDF_ASSET_ENTRY_CHANGE_BITS 0x55
#define SDF_ASSET_PRIMARY_STATE_DIRTY 3
#define SDF_ASSET_PRIMARY_SCALARS_DIRTY 0xC
#define SDF_ASSET_SECONDARY_STATE_DIRTY 0x30
#define SDF_ASSET_PAIR_STATE_DIRTY 0xC0
#define SDF_ASSET_ALL_STATE_DIRTY 0xFF
#define SDF_ASSET_ENTRY_COUNT 2
#define SDF_TEXT_PAIR_OVERRIDE_BIT 2
#define SDF_SUBPARAM_BYTES 0x18
#define SDF_ASSET_BYTES 0x48
#define SDF_ASSET_DRAW_ENTRY_BYTES 0xA0
#define SDF_RESOURCE_LIST_ITEM_BYTES 4
#define SDF_RESOURCE_LIST_GROWTH 4
#define SDF_FORCED_TEXTURE_ZERO_STATE_MODE 1
#define SDF_NODE_SOURCE_WORD_MASK 0x0FFFFFFF
#define SDF_NODE_HEADER_BYTES 0x10

/* Serialized presence flags are distinct from the draw-entry change bits. */
#define SDF_PARAM_PRIMARY_WORD_FIRST_PRESENT 0x1
#define SDF_PARAM_PRIMARY_WORD_SECOND_PRESENT 0x2
#define SDF_PARAM_PRIMARY_TEXTURE_PRESENT 0x4
#define SDF_PARAM_PRIMARY_SCALARS_PRESENT 0x8
#define SDF_PARAM_SECONDARY_COLOR_PRESENT 0x10
#define SDF_PARAM_SECONDARY_TEXTURE_STATE_PRESENT 0x20
#define SDF_PARAM_SECONDARY_SCALARS_PRESENT 0x40
#define SDF_PARAM_PRIMARY_WORD_THIRD_PRESENT 0x80
#define SDF_PARAM_PRIMARY_WORD_FOURTH_PRESENT 0x100
#define SDF_PARAM_PRIMARY_FLOAT_PRESENT 0x200
#define SDF_PARAM_SCALAR_PAIR_PRESENT 0x400
#define SDF_PARAM_HEADER_BYTES 8
#define SDF_PARAM_WORD_BYTES 4
#define SDF_PARAM_SCALAR_BLOCK_BYTES 0x14
#define SDF_PARAM_SCALAR_PAIR_BYTES 8
#define SDF_PARAM_TEXTURE_INDEX_MASK 0xFFFF
#define SDF_PARAM_PACKET_MODE_SHIFT 16

typedef union SdfSubParam {
    struct {
        u64 unk0;
        u64 unk8;
        u32 unk10;
        u32 unk14;
    } packed;
    struct {
        f32 values[5];
        u32 unk14;
    } scalar;
} SdfSubParam;

typedef struct SdfTextParam {
    u32 unk00;
    u16 unk04;
    u8 dirtyFlags; /* 0x06: dirty flags for the setters below */
    u8 pad07[9]; /* 0x07 */
    u32 unk10; /* 0x10 */
    u32 unk14; /* 0x14 */
    union {
        u32 packedColor;
        struct {
            u8 unk18;
            u8 overrideFlags; /* bit 0x2 selects scalar overrides */
            u8 unk1A;
            u8 unk1B;
        };
    };
    f32 unk1C; /* 0x1C */
    u32 unk20; /* 0x20 */
    u8 pad24[4]; /* 0x24 */
    f32 unk28; /* 0x28 */
    f32 unk2C; /* 0x2C */
    u32 unk30; /* 0x30 */
    u32 unk34; /* 0x34 */
    SdfSubParam *primarySubParam; /* 0x38 */
    SdfSubParam *secondarySubParam; /* 0x3C */
    f32 scalarPairFirst; /* 0x40 */
    f32 scalarPairSecond; /* 0x44 */
    u8 pad48[0x40]; /* 0x48 */
    f32 overrideFirst; /* 0x88 */
    f32 overrideSecond; /* 0x8C */
    void *chunkTable; /* 0x90: resource chunk searched by tag */
} SdfTextParam;

typedef struct SdfChunk {
    u32 id;   /* 0x0: entry id, 0 terminates the list */
    u32 size; /* 0x4: byte offset to the next entry */
    u32 firstValue; /* 0x8: payload of single-value UNIQ/LODC chunks */
} SdfChunk;

/* A map record has separate draw-node and lookup IDs, followed by three
 * vectors consumed by the VU basis/position routines. */
typedef struct SdfMapPositionRecord {
    u32 nodeId;        /* 0x00: passed to the draw-node lookup */
    s32 id;            /* 0x04: used to find this map record */
    u8 pad08[8];
    u128 position;     /* 0x10 */
    u128 up;           /* 0x20 */
    u128 direction;    /* 0x30: negated when building the basis */
} SdfMapPositionRecord;

/* Map-position chunk: 0x10 header, then the records back to back. */
typedef struct SdfMapPositionChunk {
    SdfChunk header;
    u8 pad0C[4];
    SdfMapPositionRecord records[1]; /* 0x10 */
} SdfMapPositionChunk;

typedef struct SdfResourceList {
    u32 unk0;
    s16 count;
    s16 capacity;
    u32 unk8;
    u32 *items;
} SdfResourceList;

struct DevRequest;
/* The list capacity occupies the buffered request's count word. */
extern void sdfDevBufferedRequestGrow(struct DevRequest *request);

extern SdfSubParam *sdfSubParamCreate(void);

extern u32 sdfForcedAssetTextureMode;
extern f32 D_003BD358;
extern f32 D_003BD35C;
extern u8 sdfResourceReleaseQueue;
extern u8 sdfAssetReleaseQueue;
extern s32 sdfLiveAssetCount;
extern void sdfReleaseChipBlock(void *);
void *sdfDevCreateBufferedRequest(s32, s32, s32);
extern u64 sdfTexGetPrimaryTextureState(SdfTex *);
extern u64 sdfTexGetPrimarySamplingState(SdfTex *);
extern u64 sdfTexGetPrimaryClampState(SdfTex *);

extern void sdfInitPacketList(void *);

extern void sdfAppendPacket(void *, void *);

extern void sdfBuildPrimaryAlphaBlendDmaPacket(void *);

extern void sdfBuildPrimaryTestBlendPacket(void *);

extern void sdfBuildPrimaryAlphaAdditiveDmaPacket(void *);

extern void sdfBuildPrimaryAlphaSubtractiveDmaPacket(void *);

void *sdfChunkFindById(SdfChunk *chunk, s32 chunkId);
void *sdfAllocSizeClassBlock(s32 size);
void *sdfAllocAndClearQuadwords(s32 size);
void *sdfChunkFindRecordById(SdfTextParam *, s32);
void sdfSetLookAtBasisFromRecord(SdfTextParam *param, SdfMapPositionRecord *record);
void sdfVuTransformMapRecordPosition(SdfTextParam *param, SdfMapPositionRecord *record);
void sdfInitializeSynchronizedRequest(void *arg0, void (*arg1)(void *));
void sdfPendingQueuePush(void *arg0, s32 arg1);
void sdfResourceListReleaseAssets(SdfResourceList *list);
void sdfCopyAssetParameterState(SdfAsset *, SdfAsset *);
void sdfAssetRelease(SdfAsset *);
void sdfDestroyDevRequest(void *);
void sdfTexReleaseReferenceViaHandler(u32);
SdfAsset *sdfCreateAssetWithDrawEntries(void);
u8 *sdfParseAssetParameterFlags(SdfAsset *, SdfTextParam *, u8 *);
void sdfAppendAssetToResourceList(SdfResourceList *, SdfAsset *);
void sdfAssetCopyTextureState(SdfAsset *, SdfAssetEntry *);
void sdfApplyAssetSecondaryEntry(SdfAsset *, void *);
u8 *sdfModelFindDrawNode(void *chunk, s32 id);
void sdfPostmultiplyVuMatrixFromMemory(void *);
void sdfApplyAssetEntryChangesWithForcedTexture(SdfAsset *, s32);
extern void func_002DAB80(u8 *, void *);
extern u16 D_00398198[];
extern SdfTex *func_002D2800(SdfTex *);
extern void func_002D33C8(u32, s32, f32);

/* Four 0x60 draw groups and a final sync list/tag occupy one 0x1B0 record.
 * Each blend builder fills the 0x40-byte packet area after the list head. */
typedef struct SdfDrawPacketGroup {
    SdfListHead list;
    SdfNode header;
    u8 pad30[0x30];
} SdfDrawPacketGroup;

typedef struct SdfDrawPacketGroups {
    SdfDrawPacketGroup groups[4];
    SdfListHead syncList;
    u64 unk1A0;
    u64 unk1A8;
} SdfDrawPacketGroups;


typedef struct SdfPacketOwner {
    u8 pad00[0x10];
    void (*sync)(struct SdfPacketOwner *, void *);
    void (*draw)(struct SdfPacketOwner *, s32, void *);
} SdfPacketOwner;
INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9748);

/* Initialize one draw-group record in the kernel's byte-buffer storage. */
void sdfInitializeDrawPacketGroups(u8 *memory) {
    SdfDrawPacketGroups *ctx = (SdfDrawPacketGroups *)memory;
    SdfDrawPacketGroup *packet = ctx->groups;
    s32 i;

    sdfBuildPrimaryAlphaBlendDmaPacket(&ctx->groups[0].header);
    sdfBuildPrimaryTestBlendPacket(&ctx->groups[1].header);
    sdfBuildPrimaryAlphaAdditiveDmaPacket(&ctx->groups[2].header);
    sdfBuildPrimaryAlphaSubtractiveDmaPacket(&ctx->groups[3].header);
    for (i = 0; i != 4; i++) {
        /* Replace only the first VIF word; preserve the builder's DIRECT word. */
        packet->header.unk8 = 0x11000000;
        sdfInitPacketList(&packet->list);
        sdfAppendPacket(&packet->list, &packet->header);
        packet++;
    }
    sdfInitPacketList(&ctx->syncList);
    ctx->unk1A0 = 0;
    ctx->unk1A8 = 0x13000000;
    sdfAppendPacket(&ctx->syncList, &ctx->unk1A0);
}

/* Submit the four draw lists, then the trailing list to the fourth owner. */
void sdfSubmitDrawPacketGroups(SdfPacketOwner **owners, u8 *memory) {
    SdfDrawPacketGroups *packets = (SdfDrawPacketGroups *)memory;
    s32 i;

    for (i = 0; i != 4; i++) {
        owners[i]->draw(owners[i], 1, &packets->groups[i]);
    }
    owners[3]->sync(owners[3], &packets->syncList);
}

/* Follow chunk byte extents until the requested ID or the zero-ID terminator is reached. */
void *sdfChunkFindById(SdfChunk *chunk, s32 chunkId) {
    u32 currentId;

    if (chunk == NULL) {
        return NULL;
    }
    currentId = chunk->id;
    while (currentId != 0) {
        if (currentId == chunkId) {
            return (void *)chunk;
        }
        chunk = (SdfChunk *)((u8 *)chunk + chunk->size);
        currentId = chunk->id;
    }
    return NULL;
}

void *sdfChunkFindByTag(SdfTextParam *param, s32 tag) {
    return sdfChunkFindById(param->chunkTable, tag);
}

/* Names are followed by a four-byte-aligned ID word; missing chunks/names return -1. */
s32 sdfNamedChunkFindId(SdfTextParam *param, const char *name) {
    SdfChunk *namesChunk = sdfChunkFindByTag(param, SDF_CHUNK_NAMED_IDS);
    u8 *entryName;
    u8 *chunkEnd;
    u32 nameLength;
    if (namesChunk == NULL) {
        return -1;
    }
    entryName = (u8 *)namesChunk + 8;
    chunkEnd = (u8 *)namesChunk + namesChunk->size;
    nameLength = strlen(name);
    do {
        u32 entryNameLength = strlen((char *)entryName);
        u8 *idWord = (u8 *)(((u32)(entryName + entryNameLength + 4)) & ~3U);
        if (entryNameLength == nameLength && memcmp(entryName, name, nameLength) == 0) {
            return *(s32 *)idWord;
        }
        entryName = idWord + 4;
    } while (entryName < chunkEnd);
    return -1;
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9C28);

u32 sdfCountMapPositionRecords(SdfTextParam *param) {
    SdfChunk *chunk = sdfChunkFindByTag(param, SDF_CHUNK_MAP_POSITIONS);
    if (chunk != NULL) {
        return (chunk->size - 0x10) >> 6;
    }
    return 0;
}

/* vu0 routine: build basis rows in vf28-vf31 from the map record, then
 * postmultiply using its draw-node matrix. */
void sdfSetLookAtBasisFromRecord(SdfTextParam *param, SdfMapPositionRecord *record) {
    u8 *matrix = sdfModelFindDrawNode(param, record->nodeId);
    u8 *vector;

    vector = (u8 *)&record->up;
    VU0_LOAD_VF(vf10, vector);
    VU0_MOVE_VF(vf30, vf10);
    VU0_MOVE_VF(vf11, vf10);
    vector = (u8 *)&record->direction;
    VU0_LOAD_VF(vf10, vector);
    VU0_NEGATE_XYZ(vf10);
    VU0_MOVE_VF(vf29, vf10);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf28, vf10);
    VU0_LOAD_VF(vf31, &record->position);
    VU0_SET_W_ONE(vf31);
    sdfPostmultiplyVuMatrixFromMemory(matrix + 0xC0);
}

/* vu0 routine: transform the map-record position by its draw-node matrix;
 * the transformed position is left in vf10. */
void sdfVuTransformMapRecordPosition(SdfTextParam *param, SdfMapPositionRecord *record) {
    u8 *matrix = sdfModelFindDrawNode(param, record->nodeId) + 0xC0;

    VU0_LOAD_MATRIX(matrix);
    VU0_LOAD_VF(vf10, &record->position);
    VU0_TRANSFORM_POINT(vf10, vf10);
}

/* Search the fixed-size map-position records within the chunk's declared byte extent. */
void *sdfChunkFindRecordById(SdfTextParam *param, s32 recordId) {
    SdfChunk *positionsChunk = sdfChunkFindByTag(param, SDF_CHUNK_MAP_POSITIONS);
    SdfMapPositionChunk *positions;
    SdfMapPositionRecord *record;
    u8 *chunkEnd;
    if (positionsChunk == NULL) {
        return NULL;
    }
    positions = (SdfMapPositionChunk *)positionsChunk;
    record = positions->records;
    chunkEnd = (u8 *)positionsChunk + positionsChunk->size;
    while ((u8 *)record < chunkEnd) {
        if (record->id == recordId) {
            return record;
        }
        record++;
    }
    return NULL;
}

s32 sdfLoadMapRecordLookAtBasis(SdfTextParam *param, s32 id) {
    void *resource = sdfChunkFindRecordById(param, id);
    if (resource != NULL) {
        sdfSetLookAtBasisFromRecord(param, resource);
        return 1;
    }
    return 0;
}

s32 sdfLoadMapRecordPositionVector(SdfTextParam *param, s32 id) {
    void *resource = sdfChunkFindRecordById(param, id);
    if (resource != NULL) {
        sdfVuTransformMapRecordPosition(param, resource);
        return 1;
    }
    return 0;
}

u32 sdfGetUniqueChunkValue(SdfTextParam *param) {
    SdfChunk *chunk = sdfChunkFindByTag(param, SDF_CHUNK_UNIQUE_VALUE);
    if (chunk != NULL) {
        return chunk->firstValue;
    }
    return 0;
}

u32 sdfGetLodChunkValue(SdfTextParam *param) {
    SdfChunk *chunk = sdfChunkFindByTag(param, SDF_CHUNK_LOD_VALUE);
    if (chunk != NULL) {
        return chunk->firstValue;
    }
    return 0;
}


/* Store the override pair and select it instead of the shared defaults. */
void sdfSetTextFloatPairOverride(SdfTextParam *param, f32 first, f32 second) {
    param->overrideFirst = first;
    param->overrideSecond = second;
    param->overrideFlags = param->overrideFlags | SDF_TEXT_PAIR_OVERRIDE_BIT;
}

/* Clear only the pair-override selection bit. */
void sdfClearTextFloatPairOverride(SdfTextParam *param) {
    param->overrideFlags = param->overrideFlags & ~SDF_TEXT_PAIR_OVERRIDE_BIT;
}

/* Return the first override value when selected, otherwise its shared default. */
f32 sdfGetFirstTextOverrideOrDefault(SdfTextParam *param) {
    if ((param->overrideFlags & SDF_TEXT_PAIR_OVERRIDE_BIT) != 0) {
        return param->overrideFirst;
    }
    return D_003BD358;
}

/* Return the second override value when selected, otherwise its shared default. */
f32 sdfGetSecondTextOverrideOrDefault(SdfTextParam *param) {
    if ((param->overrideFlags & SDF_TEXT_PAIR_OVERRIDE_BIT) != 0) {
        return param->overrideSecond;
    }
    return D_003BD35C;
}

/* Set the native mode word; its consumers distinguish mode one from other values. */
void sdfSetForcedAssetTextureMode(u32 mode) {
    sdfForcedAssetTextureMode = mode;
}

/* Create a four-byte-item buffer with growth in groups of four slots. */
SdfResourceList *sdfCreateConfiguredBufferedResourceList(u32 capacity) {
    return sdfDevCreateBufferedRequest(capacity, SDF_RESOURCE_LIST_ITEM_BYTES, SDF_RESOURCE_LIST_GROWTH);
}

/* Optionally release item references before destroying the buffered list; null lists are ignored. */
void sdfResourceListRelease(SdfResourceList *list, s32 releaseItems) {
    s32 itemIndex;
    s32 itemCount;
    if (list == NULL) {
        return;
    }
    if (releaseItems != 0) {
        itemCount = list->count;
        for (itemIndex = 0; itemIndex < itemCount; itemIndex++) {
            sdfTexReleaseReferenceViaHandler(list->items[itemIndex]);
        }
    }
    sdfDestroyDevRequest(list);
}

void sdfAppendResourceListItem(SdfResourceList *list, u32 item) {
    if (list->count >= list->capacity) {
        sdfDevBufferedRequestGrow((struct DevRequest *)list);
    }
    list->items[list->count] = item;
    list->count++;
}

void sdfReduceResourceListCount(SdfResourceList *list, s32 count, s32 enabled) {
    s32 cursor;

    if ((count < list->count) && (enabled != 0)) {
        cursor = (s32)count;
        do {
            cursor = cursor + 1;
        } while ((s64)cursor != (s64)list->count);
        list->count = (s16)count;
    }
    sdfDevResizeBufferedRequest();
}

/* Clone every texture into a new buffered list; null sources return null.
 * Entries remain address words, so bridge them explicitly at the texture API. */
SdfResourceList *sdfResourceListClone(SdfResourceList *source) {
    s32 itemCount;
    SdfResourceList *clone;
    s32 itemIndex;

    if (source == NULL) {
        return NULL;
    }
    itemCount = source->count;
    clone = sdfCreateConfiguredBufferedResourceList(itemCount);
    for (itemIndex = 0; itemIndex < itemCount; itemIndex++) {
        clone->items[itemIndex] = (u32)func_002D2800((SdfTex *)source->items[itemIndex]);
    }
    clone->count = itemCount;
    return clone;
}

/* Apply the scalar update only to assets whose byte at 0x18 is nonzero. */
void sdfUpdateActiveResourceListScalars(SdfResourceList *list, s32 arg, f32 value) {
    s32 itemIndex;
    s32 itemCount;

    if (list == NULL) {
        return;
    }
    itemCount = list->count;
    for (itemIndex = 0; itemIndex < itemCount; itemIndex++) {
        SdfAsset *asset = (SdfAsset *)list->items[itemIndex];

        if ((u8)asset->unk18 != 0) {
            func_002D33C8((u32)asset, arg, value);
        }
    }
}

void sdfRegisterResourceQueueCallbacks(void) {
    sdfInitializeSynchronizedRequest(&sdfResourceReleaseQueue, sdfResourceListReleaseAssets);
    sdfInitializeSynchronizedRequest(&sdfAssetReleaseQueue, sdfAssetRelease);
}

SdfResourceList *sdfCreateResourceList(s32 capacity) {
    return sdfDevCreateBufferedRequest(capacity, 4, 8);
}

/* Release every asset, then destroy its buffered list. */
void sdfResourceListReleaseAssets(SdfResourceList *list) {
    s32 itemIndex;

    for (itemIndex = 0; itemIndex < list->count; itemIndex++) {
        sdfAssetRelease((SdfAsset *)list->items[itemIndex]);
    }
    sdfDestroyDevRequest(list);
}

void sdfReleaseQueuedResource(void *resource, s32 retained) {
    if (resource == NULL) {
        return;
    }
    if (retained != 0) {
        sdfPendingQueuePush(&sdfResourceReleaseQueue, (s32)resource);
    } else {
        sdfDestroyDevRequest(resource);
    }
}

void sdfGrowResourceListStorage(SdfResourceList *list) {
    sdfDevBufferedRequestGrow((struct DevRequest *)list);
}

void sdfAppendAssetToResourceList(SdfResourceList *list, SdfAsset *asset) {
    if (list->count >= list->capacity) {
        sdfGrowResourceListStorage(list);
    }
    list->items[list->count] = (u32)asset;
    list->count++;
}

/* Store the first primary-state word and dirty both draw entries. */
void func_002DA3C0(SdfTextParam *param, u32 value) {
    param->unk10 = value;
    param->dirtyFlags |= SDF_ASSET_PRIMARY_STATE_DIRTY;
}

/* Store the second primary-state word and dirty both draw entries. */
void func_002DA3D8(SdfTextParam *param, u32 value) {
    param->unk14 = value;
    param->dirtyFlags |= SDF_ASSET_PRIMARY_STATE_DIRTY;
}

/* Store the third primary-state word and dirty both draw entries. */
void func_002DA3F0(SdfTextParam *param, u32 value) {
    param->unk20 = value;
    param->dirtyFlags |= SDF_ASSET_PRIMARY_STATE_DIRTY;
}

/* Store raw bits in the fourth primary-state word, without float conversion. */
void func_002DA408(SdfTextParam *param, u32 bits) {
    *(u32 *)&param->unk28 = bits;
    param->dirtyFlags |= SDF_ASSET_PRIMARY_STATE_DIRTY;
}

/* Store the primary-state float and dirty both draw entries. */
void func_002DA420(SdfTextParam *param, f32 value) {
    param->unk1C = value;
    param->dirtyFlags = param->dirtyFlags | SDF_ASSET_PRIMARY_STATE_DIRTY;
}

/* Store the primary texture address as raw bits and dirty both draw entries. */
void func_002DA438(SdfTextParam *param, u32 textureAddress) {
    *(u32 *)&param->unk2C = textureAddress;
    param->dirtyFlags |= SDF_ASSET_PRIMARY_STATE_DIRTY;
}

/* Initialize packed scalar values 0,0,1,1,0; the final opaque word is untouched. */
SdfSubParam *sdfSubParamCreate(void) {
    SdfSubParam *subParameter;

    subParameter = sdfAllocSizeClassBlock(SDF_SUBPARAM_BYTES);
    subParameter->packed.unk8 = (((u64)0x3F800000 << 16 | 0x3F80) << 16);
    subParameter->packed.unk0 = 0;
    subParameter->packed.unk10 = 0;
    return subParameter;
}

/* Lazily create the primary scalar block; this does not itself dirty an entry. */
SdfSubParam *sdfEnsurePrimaryTextSubParam(SdfTextParam *param) {
    SdfSubParam *subParameter = param->primarySubParam;
    if (subParameter == NULL) {
        subParameter = sdfSubParamCreate();
        param->primarySubParam = subParameter;
    }
    return subParameter;
}

/* Store the five primary scalars in order and dirty both draw entries. */
void sdfSetPrimaryTextScalars(SdfTextParam *param, f32 a, f32 b, f32 c, f32 d, f32 e) {
    f32 *scalarValues = sdfEnsurePrimaryTextSubParam(param)->scalar.values;
    scalarValues[0] = a;
    scalarValues[1] = b;
    scalarValues[2] = c;
    scalarValues[3] = d;
    scalarValues[4] = e;
    param->dirtyFlags |= SDF_ASSET_PRIMARY_SCALARS_DIRTY;
}

/* Copy five scalar values, leaving the block's opaque final word unchanged. */
void sdfCopyPrimaryTextScalars(SdfTextParam *param, const f32 *sourceScalars) {
    f32 *scalarValues = sdfEnsurePrimaryTextSubParam(param)->scalar.values;
    scalarValues[0] = sourceScalars[0];
    scalarValues[1] = sourceScalars[1];
    scalarValues[2] = sourceScalars[2];
    scalarValues[3] = sourceScalars[3];
    scalarValues[4] = sourceScalars[4];
    param->dirtyFlags |= SDF_ASSET_PRIMARY_SCALARS_DIRTY;
}

/* Write the full packed secondary-color word and dirty both draw entries. */
void func_002DA5B0(SdfTextParam *param, u32 packedColor) {
    param->packedColor = packedColor;
    param->dirtyFlags |= SDF_ASSET_SECONDARY_STATE_DIRTY;
}

/* Store the mode used by the secondary packet's mode and palette lookup. */
void func_002DA5C8(SdfTextParam *param, u32 packetMode) {
    param->unk34 = packetMode;
    param->dirtyFlags |= SDF_ASSET_SECONDARY_STATE_DIRTY;
}

/* Store the secondary texture address and dirty both draw entries. */
void func_002DA5E0(SdfTextParam *param, u32 textureAddress) {
    param->unk30 = textureAddress;
    param->dirtyFlags |= SDF_ASSET_SECONDARY_STATE_DIRTY;
}

/* Lazily create the secondary scalar block; this does not itself dirty an entry. */
SdfSubParam *sdfEnsureSecondaryTextSubParam(SdfTextParam *param) {
    SdfSubParam *subParameter = param->secondarySubParam;
    if (subParameter == NULL) {
        subParameter = sdfSubParamCreate();
        param->secondarySubParam = subParameter;
    }
    return subParameter;
}

/* Store the five secondary scalars in order and dirty both draw entries. */
void sdfSetSecondaryTextScalars(SdfTextParam *param, f32 a, f32 b, f32 c, f32 d, f32 e) {
    f32 *scalarValues = sdfEnsureSecondaryTextSubParam(param)->scalar.values;
    scalarValues[0] = a;
    scalarValues[1] = b;
    scalarValues[2] = c;
    scalarValues[3] = d;
    scalarValues[4] = e;
    param->dirtyFlags |= SDF_ASSET_SECONDARY_STATE_DIRTY;
}

/* Copy five scalar values, leaving the block's opaque final word unchanged. */
void sdfCopySecondaryTextScalars(SdfTextParam *param, const f32 *sourceScalars) {
    f32 *scalarValues = sdfEnsureSecondaryTextSubParam(param)->scalar.values;
    scalarValues[0] = sourceScalars[0];
    scalarValues[1] = sourceScalars[1];
    scalarValues[2] = sourceScalars[2];
    scalarValues[3] = sourceScalars[3];
    scalarValues[4] = sourceScalars[4];
    param->dirtyFlags |= SDF_ASSET_SECONDARY_STATE_DIRTY;
}

/* Store the scalar pair and mark its corresponding group dirty for both entries. */
void sdfSetTextScalarPair(SdfTextParam *param, f32 first, f32 second) {
    param->scalarPairFirst = first;
    param->scalarPairSecond = second;
    param->dirtyFlags = param->dirtyFlags | SDF_ASSET_PAIR_STATE_DIRTY;
}

/* Allocate both native draw-entry command blocks and mark every change group dirty. */
SdfAsset *sdfCreateAssetWithDrawEntries(void) {
    SdfAsset *asset;
    u32 *entryWords;
    s32 entryIndex;

    sdfLiveAssetCount++;
    asset = sdfAllocAndClearQuadwords(SDF_ASSET_BYTES);
    asset->pad00[6] = SDF_ASSET_ALL_STATE_DIRTY;
    for (entryIndex = 0; entryIndex != SDF_ASSET_ENTRY_COUNT; entryIndex++) {
        entryWords = sdfAllocSizeClassBlock(SDF_ASSET_DRAW_ENTRY_BYTES);
        asset->entries[entryIndex] = entryWords;
        entryWords[0] = 0x6E05C000;
        entryWords[0x18 / 4] = 0x6005C005;
        entryWords[0x30 / 4] = 0;
        entryWords[0x34 / 4] = 0x640CC00A;
        entryWords[0x98 / 4] = 0x400000C;
        entryWords[0x9C / 4] = 0x14000000;
    }
    asset->unk40 = 0;
    asset->unk44 = 0;
    asset->unk10 = 0x80808080;
    asset->unk14 = 0x80808080;
    asset->unk18 = 0x80808080;
    return asset;
}

/* Consume supported presence bits in their native order and return the next byte.
 * Texture indices and packet modes are unchecked; unknown bits consume no payload. */
u8 *sdfParseAssetParameterFlags(SdfAsset *asset, SdfTextParam *resourceLookup, u8 *serializedData) {
    SdfTextParam *param = (SdfTextParam *)asset;
    u32 parameterFlags;
    u8 *parameterCursor;
    u32 packedTextureMode;

    param->unk00 = *(u32 *)serializedData;
    param->unk04 = *(u16 *)(serializedData + 4);
    parameterFlags = *(u16 *)(serializedData + 6);
    parameterCursor = serializedData + SDF_PARAM_HEADER_BYTES;
    if (parameterFlags & SDF_PARAM_PRIMARY_WORD_FIRST_PRESENT) {
        func_002DA3C0(param, *(u32 *)parameterCursor);
        parameterCursor += SDF_PARAM_WORD_BYTES;
    }
    if (parameterFlags & SDF_PARAM_PRIMARY_WORD_SECOND_PRESENT) {
        func_002DA3D8(param, *(u32 *)parameterCursor);
        parameterCursor += SDF_PARAM_WORD_BYTES;
    }
    if (parameterFlags & SDF_PARAM_PRIMARY_TEXTURE_PRESENT) {
        func_002DA438(param, ((SdfResourceList *)resourceLookup)->items[*(u16 *)parameterCursor]);
        parameterCursor += SDF_PARAM_WORD_BYTES;
    }
    if (parameterFlags & SDF_PARAM_PRIMARY_SCALARS_PRESENT) {
        sdfSetPrimaryTextScalars(param, ((f32 *)parameterCursor)[0], ((f32 *)parameterCursor)[1], ((f32 *)parameterCursor)[2], ((f32 *)parameterCursor)[3], ((f32 *)parameterCursor)[4]);
        parameterCursor += SDF_PARAM_SCALAR_BLOCK_BYTES;
    }
    if (parameterFlags & SDF_PARAM_SECONDARY_COLOR_PRESENT) {
        func_002DA5B0(param, *(u32 *)parameterCursor);
        parameterCursor += SDF_PARAM_WORD_BYTES;
    }
    if (parameterFlags & SDF_PARAM_SECONDARY_TEXTURE_STATE_PRESENT) {
        packedTextureMode = *(u32 *)parameterCursor;
        parameterCursor += SDF_PARAM_WORD_BYTES;
        func_002DA5E0(param, ((SdfResourceList *)resourceLookup)->items[packedTextureMode & SDF_PARAM_TEXTURE_INDEX_MASK]);
        func_002DA5C8(param, packedTextureMode >> SDF_PARAM_PACKET_MODE_SHIFT);
    }
    if (parameterFlags & SDF_PARAM_SECONDARY_SCALARS_PRESENT) {
        sdfSetSecondaryTextScalars(param, ((f32 *)parameterCursor)[0], ((f32 *)parameterCursor)[1], ((f32 *)parameterCursor)[2], ((f32 *)parameterCursor)[3], ((f32 *)parameterCursor)[4]);
        parameterCursor += SDF_PARAM_SCALAR_BLOCK_BYTES;
    }
    if (parameterFlags & SDF_PARAM_PRIMARY_WORD_THIRD_PRESENT) {
        func_002DA3F0(param, *(u32 *)parameterCursor);
        parameterCursor += SDF_PARAM_WORD_BYTES;
    }
    if (parameterFlags & SDF_PARAM_PRIMARY_WORD_FOURTH_PRESENT) {
        func_002DA408(param, *(u32 *)parameterCursor);
        parameterCursor += SDF_PARAM_WORD_BYTES;
    }
    if (parameterFlags & SDF_PARAM_PRIMARY_FLOAT_PRESENT) {
        func_002DA420(param, *(f32 *)parameterCursor);
        parameterCursor += SDF_PARAM_WORD_BYTES;
    }
    if (parameterFlags & SDF_PARAM_SCALAR_PAIR_PRESENT) {
        sdfSetTextScalarPair(param, ((f32 *)parameterCursor)[0], ((f32 *)parameterCursor)[1]);
        parameterCursor += SDF_PARAM_SCALAR_PAIR_BYTES;
    }
    return parameterCursor;
}

/* Release both draw entries and the parameter blocks, then the asset; null is ignored. */
void sdfAssetRelease(SdfAsset *asset) {
    if (asset == NULL) {
        return;
    }
    sdfLiveAssetCount--;
    sdfReleaseChipBlock(asset->entries[0]);
    sdfReleaseChipBlock(asset->entries[1]);
    sdfReleaseChipBlock(asset->third);
    sdfReleaseChipBlock(asset->fourth);
    sdfReleaseChipBlock(asset);
}

/* Queue the non-null asset address for deferred release. */
void sdfQueueAssetRelease(SdfAsset *asset) {
    s32 queuedAddress = (s32)asset;

    if (queuedAddress != 0) {
        sdfPendingQueuePush(&sdfAssetReleaseQueue, queuedAddress);
    }
}

/* Initialize the node header from the selected source words and return its payload. */
void *sdfInitNodeHeaderFromWords(u32 *words, SdfNode *node, s32 wordIndex) {
    u32 *headerWords = words + wordIndex;

    node->unk3 = 0x30;
    node->unk4 = headerWords[2] & SDF_NODE_SOURCE_WORD_MASK;
    node->unk0 = 0xA;
    node->unk8 = 0;
    node->unkC = 0;
    return (void *)((u8 *)node + SDF_NODE_HEADER_BYTES);
}

/* Copy primary words and optional GS texture state; only mode one zeros its state word. */
void sdfAssetCopyTextureState(SdfAsset *asset, SdfAssetEntry *entry) {
    SdfTex *texture;
    entry->unk04 = asset->unk10;
    entry->unk1C = asset->unk1C;
    entry->unk08 = asset->unk14;
    if (sdfForcedAssetTextureMode == SDF_FORCED_TEXTURE_ZERO_STATE_MODE) {
        entry->unk10 = 0;
    } else {
        entry->unk10 = asset->unk20;
    }
    entry->unk14 = asset->unk28;
    texture = asset->texture;
    if (texture != NULL) {
        entry->unk38 = sdfTexGetPrimarySamplingState(texture);
        entry->unk40 = sdfTexGetPrimaryTextureState(texture);
        entry->unk48 = sdfTexGetPrimaryClampState(texture);
    }
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DAB80);

void sdfCopyAssetPrimarySubParameter(SdfAsset *asset, u8 *entry) {
    func_002DAB80(entry + 0x68, asset->third);
}

typedef struct SdfDrawPacket {
    u8 pad00[0xC];
    u32 color; /* 0x0C */
    u8 pad10[0x10];
    u32 paletteValue; /* 0x20 */
    u32 mode;         /* 0x24 */
    u8 pad28[0x28];
    u64 textureWords[3]; /* 0x50, 0x58, 0x60 */
} SdfDrawPacket;

/* Copy color, unchecked mode/palette state and optional GS texture words,
 * then apply the secondary scalar block to the entry's native location. */
void sdfApplyAssetSecondaryEntry(SdfAsset *asset, void *drawEntry) {
    u8 *entryBytes = drawEntry;
    SdfTex *texture = asset->secondaryTexture;
    u32 packetMode;

    ((SdfDrawPacket *)entryBytes)->color = asset->unk18;
    packetMode = asset->secondaryMode;
    ((SdfDrawPacket *)entryBytes)->mode = packetMode;
    ((SdfDrawPacket *)entryBytes)->paletteValue = D_00398198[packetMode];
    if (texture != NULL) {
        ((SdfDrawPacket *)entryBytes)->textureWords[0] = sdfTexGetPrimarySamplingState(texture);
        ((SdfDrawPacket *)entryBytes)->textureWords[1] = sdfTexGetPrimaryTextureState(texture);
        ((SdfDrawPacket *)entryBytes)->textureWords[2] = sdfTexGetPrimaryClampState(texture);
    }
    func_002DAB80(entryBytes + 0x80, asset->fourth);
}

/* Copy the source pair into the destination entry's two scalar slots. */
void sdfAssetCopyPairToTextParam(SdfAsset *source, SdfTextParam *destination) {
    destination->unk28 = source->unk40;
    destination->unk2C = source->unk44;
}

/* Apply the selected entry's groups and retain the other entry's captured bits.
 * entryIndex is unchecked and expected to be zero or one. */
void sdfAssetApplyEntryChanges(SdfAsset *asset, s32 entryIndex) {
    u8 dirtyFlags = asset->pad00[6];
    SdfAssetEntry *drawEntry = asset->entries[entryIndex];
    if ((dirtyFlags >> entryIndex) & SDF_ASSET_PRIMARY_STATE_BIT) {
        sdfAssetCopyTextureState(asset, drawEntry);
    }
    if (dirtyFlags & (SDF_ASSET_PRIMARY_SCALARS_BIT << entryIndex)) {
        sdfCopyAssetPrimarySubParameter(asset, (u8 *)drawEntry);
    }
    if (dirtyFlags & (SDF_ASSET_SECONDARY_STATE_BIT << entryIndex)) {
        sdfApplyAssetSecondaryEntry(asset, drawEntry);
    }
    if (dirtyFlags & (SDF_ASSET_PAIR_STATE_BIT << entryIndex)) {
        sdfAssetCopyPairToTextParam(asset, drawEntry);
    }
    asset->pad00[6] = dirtyFlags & (SDF_ASSET_ENTRY_CHANGE_BITS << (entryIndex ^ 1));
}

/* Nonzero forced mode refreshes primary state even when its change bit is clear.
 * Retain the other entry's captured bits; entryIndex remains unchecked. */
void sdfApplyAssetEntryChangesWithForcedTexture(SdfAsset *asset, s32 entryIndex) {
    u8 dirtyFlags = asset->pad00[6];
    SdfAssetEntry *drawEntry = asset->entries[entryIndex];

    if ((dirtyFlags >> entryIndex) & SDF_ASSET_PRIMARY_STATE_BIT) {
        sdfAssetCopyTextureState(asset, drawEntry);
    } else if (sdfForcedAssetTextureMode != 0) {
        sdfAssetCopyTextureState(asset, drawEntry);
    }
    if (dirtyFlags & (SDF_ASSET_PRIMARY_SCALARS_BIT << entryIndex)) {
        sdfCopyAssetPrimarySubParameter(asset, (u8 *)drawEntry);
    }
    if (dirtyFlags & (SDF_ASSET_SECONDARY_STATE_BIT << entryIndex)) {
        sdfApplyAssetSecondaryEntry(asset, drawEntry);
    }
    if (dirtyFlags & (SDF_ASSET_PAIR_STATE_BIT << entryIndex)) {
        sdfAssetCopyPairToTextParam(asset, drawEntry);
    }
    asset->pad00[6] = dirtyFlags & (SDF_ASSET_ENTRY_CHANGE_BITS << (entryIndex ^ 1));
}

/* Parse the declared number of variable-sized assets, reserving at least the minimum capacity. */
SdfResourceList *sdfAssetListParse(SdfTextParam *lookup, u32 *data) {
    u32 remainingAssets = *data;
    u8 *parameterCursor = (u8 *)(data + 1);
    SdfResourceList *list = sdfCreateResourceList(remainingAssets >= SDF_ASSET_LIST_MIN_CAPACITY ? remainingAssets : SDF_ASSET_LIST_MIN_CAPACITY);
    while (remainingAssets != 0) {
        SdfAsset *asset = sdfCreateAssetWithDrawEntries();
        parameterCursor = sdfParseAssetParameterFlags(asset, lookup, parameterCursor);
        sdfAppendAssetToResourceList(list, asset);
        remainingAssets--;
    }
    return list;
}

/* Apply one entry index to every asset in the list. */
void sdfResourceListApplyEntryChanges(SdfResourceList *list, s32 entryIndex) {
    s32 itemIndex;
    s32 itemCount = list->count;
    u32 *assetItems = list->items;

    for (itemIndex = 0; itemIndex < itemCount; itemIndex++) {
        sdfAssetApplyEntryChanges((SdfAsset *)assetItems[itemIndex], entryIndex);
    }
}

/* Apply the selected entry to each asset using the forced-texture variant. */
void sdfApplyResourceListEntriesWithForcedTexture(SdfResourceList *list, s32 entryIndex) {
    s32 itemIndex;
    s32 itemCount = list->count;
    u32 *assetItems = list->items;

    for (itemIndex = 0; itemIndex < itemCount; itemIndex++) {
        sdfApplyAssetEntryChangesWithForcedTexture((SdfAsset *)assetItems[itemIndex], entryIndex);
    }
}


/* Copy base state; only present source subparameters overwrite destination
 * blocks. Each whole scalar aggregate includes its final opaque word. */
void sdfCopyAssetParameterState(SdfAsset *destination, SdfAsset *source) {
    SdfSubParam *subParameters;

    destination->pad00[6] = SDF_ASSET_ALL_STATE_DIRTY;
    destination->unk10 = source->unk10;
    destination->unk14 = source->unk14;
    destination->unk1C = source->unk1C;
    destination->unk18 = source->unk18;
    *(u16 *)&destination->pad00[4] = *(u16 *)&source->pad00[4];
    destination->texture = source->texture;
    destination->unk20 = source->unk20;
    destination->unk28 = source->unk28;
    subParameters = source->third;
    if (subParameters != NULL) {
        sdfEnsurePrimaryTextSubParam((SdfTextParam *)destination)->scalar = subParameters->scalar;
    }
    subParameters = source->fourth;
    if (subParameters != NULL) {
        sdfEnsureSecondaryTextSubParam((SdfTextParam *)destination)->scalar = subParameters->scalar;
    }
    destination->unk40 = source->unk40;
    destination->unk44 = source->unk44;
}

/* Copy paired items using the destination count; source must contain at least that many items. */
void sdfCopyAssetListParameterState(SdfResourceList *destination, SdfResourceList *source) {
    s32 itemIndex;
    s32 itemCount = destination->count;
    u32 *sourceItems = source->items;
    u32 *destinationItems = destination->items;

    for (itemIndex = 0; itemIndex < itemCount; itemIndex++) {
        sdfCopyAssetParameterState(destinationItems[itemIndex], sourceItems[itemIndex]);
    }
}

extern s32 (*D_003981A8[])(u32, u32);

/* The high halfword selects the handler, which receives the complete command word unchanged. */
s32 sdfDispatchAssetCommandWord(u32 context, u32 commandWord) {
    D_003981A8[commandWord >> 16](context, commandWord);
}

INCLUDE_SDATA(const s32, "game/code_002D9748", sdfLiveAssetCount);

INCLUDE_SDATA(const s32, "game/code_002D9748", sdfForcedAssetTextureMode);

