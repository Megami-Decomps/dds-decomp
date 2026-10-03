#include "common.h"
#include "sdf.h"
#include "pcp_vu0.h"

#define SDF_CHUNK_NAMED_IDS 0x4D4E444E
#define SDF_ASSET_LIST_MIN_CAPACITY 0x20

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
    u8 pad00[6]; /* 0x00 */
    u8 dirtyFlags; /* 0x06: dirty flags for the setters below */
    u8 pad07[9]; /* 0x07 */
    u32 unk10; /* 0x10 */
    u32 unk14; /* 0x14 */
    u8 unk18; /* 0x18: func_002DA5B0 stores a u32 over 0x18-0x1B */
    u8 overrideFlags; /* 0x19: bit 0x2 selects overrideFirst/overrideSecond over defaults */
    u8 unk1A; /* 0x1A */
    u8 unk1B; /* 0x1B */
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


void sdfSetTextFloatPairOverride(SdfTextParam *param, f32 first, f32 second) {
    param->overrideFirst = first;
    param->overrideSecond = second;
    param->overrideFlags = param->overrideFlags | 2;
}

void sdfClearTextFloatPairOverride(SdfTextParam *param) {
    param->overrideFlags = param->overrideFlags & ~2;
}

f32 sdfGetFirstTextOverrideOrDefault(SdfTextParam *param) {
    if ((param->overrideFlags & 2) != 0) {
        return param->overrideFirst;
    }
    return D_003BD358;
}

f32 sdfGetSecondTextOverrideOrDefault(SdfTextParam *param) {
    if ((param->overrideFlags & 2) != 0) {
        return param->overrideSecond;
    }
    return D_003BD35C;
}

void sdfSetForcedAssetTextureMode(u32 value) {
    sdfForcedAssetTextureMode = value;
}

SdfResourceList *sdfCreateConfiguredBufferedResourceList(u32 capacity) {
    return sdfDevCreateBufferedRequest(capacity, 4, 4);
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

        if (*((u8 *)asset + 0x18) != 0) {
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

void func_002DA3C0(SdfTextParam *param, u32 value) {
    param->unk10 = value;
    param->dirtyFlags |= 3;
}

void func_002DA3D8(SdfTextParam *param, u32 value) {
    param->unk14 = value;
    param->dirtyFlags |= 3;
}

void func_002DA3F0(SdfTextParam *param, u32 value) {
    param->unk20 = value;
    param->dirtyFlags |= 3;
}

void func_002DA408(SdfTextParam *param, u32 bits) {
    *(u32 *)&param->unk28 = bits;
    param->dirtyFlags |= 3;
}

void func_002DA420(SdfTextParam *param, f32 value) {
    param->unk1C = value;
    param->dirtyFlags = param->dirtyFlags | 3;
}

void func_002DA438(SdfTextParam *param, u32 bits) {
    *(u32 *)&param->unk2C = bits;
    param->dirtyFlags |= 3;
}

SdfSubParam *sdfSubParamCreate(void) {
    SdfSubParam *temp;

    temp = sdfAllocSizeClassBlock(0x18);
    temp->packed.unk8 = (((u64)0x3F800000 << 16 | 0x3F80) << 16);
    temp->packed.unk0 = 0;
    temp->packed.unk10 = 0;
    return temp;
}

SdfSubParam *sdfEnsurePrimaryTextSubParam(SdfTextParam *param) {
    SdfSubParam *sub = param->primarySubParam;
    if (sub == NULL) {
        sub = sdfSubParamCreate();
        param->primarySubParam = sub;
    }
    return sub;
}

void sdfSetPrimaryTextScalars(SdfTextParam *param, f32 a, f32 b, f32 c, f32 d, f32 e) {
    f32 *values = sdfEnsurePrimaryTextSubParam(param)->scalar.values;
    values[0] = a;
    values[1] = b;
    values[2] = c;
    values[3] = d;
    values[4] = e;
    param->dirtyFlags |= 0xc;
}

void sdfCopyPrimaryTextScalars(SdfTextParam *param, const f32 *input) {
    f32 *values = sdfEnsurePrimaryTextSubParam(param)->scalar.values;
    values[0] = input[0];
    values[1] = input[1];
    values[2] = input[2];
    values[3] = input[3];
    values[4] = input[4];
    param->dirtyFlags |= 0xc;
}

void func_002DA5B0(SdfTextParam *param, u32 bits) {
    *(u32 *)((u8 *)param + 0x18) = bits;
    param->dirtyFlags |= 0x30;
}

void func_002DA5C8(SdfTextParam *param, u32 value) {
    param->unk34 = value;
    param->dirtyFlags |= 0x30;
}

void func_002DA5E0(SdfTextParam *param, u32 value) {
    param->unk30 = value;
    param->dirtyFlags |= 0x30;
}

SdfSubParam *sdfEnsureSecondaryTextSubParam(SdfTextParam *param) {
    SdfSubParam *sub = param->secondarySubParam;
    if (sub == NULL) {
        sub = sdfSubParamCreate();
        param->secondarySubParam = sub;
    }
    return sub;
}

void sdfSetSecondaryTextScalars(SdfTextParam *param, f32 a, f32 b, f32 c, f32 d, f32 e) {
    f32 *values = sdfEnsureSecondaryTextSubParam(param)->scalar.values;
    values[0] = a;
    values[1] = b;
    values[2] = c;
    values[3] = d;
    values[4] = e;
    param->dirtyFlags |= 0x30;
}

void sdfCopySecondaryTextScalars(SdfTextParam *param, const f32 *input) {
    f32 *values = sdfEnsureSecondaryTextSubParam(param)->scalar.values;
    values[0] = input[0];
    values[1] = input[1];
    values[2] = input[2];
    values[3] = input[3];
    values[4] = input[4];
    param->dirtyFlags |= 0x30;
}

void sdfSetTextScalarPair(SdfTextParam *param, f32 first, f32 second) {
    param->scalarPairFirst = first;
    param->scalarPairSecond = second;
    param->dirtyFlags = param->dirtyFlags | 0xC0;
}

SdfAsset *sdfCreateAssetWithDrawEntries(void) {
    SdfAsset *asset;
    u32 *entry;
    s32 i;

    sdfLiveAssetCount++;
    asset = sdfAllocAndClearQuadwords(0x48);
    asset->pad00[6] = 0xFF;
    for (i = 0; i != 2; i++) {
        entry = sdfAllocSizeClassBlock(0xA0);
        asset->entries[i] = entry;
        entry[0] = 0x6E05C000;
        entry[0x18 / 4] = 0x6005C005;
        entry[0x30 / 4] = 0;
        entry[0x34 / 4] = 0x640CC00A;
        entry[0x98 / 4] = 0x400000C;
        entry[0x9C / 4] = 0x14000000;
    }
    asset->unk40 = 0;
    asset->unk44 = 0;
    asset->unk10 = 0x80808080;
    asset->unk14 = 0x80808080;
    asset->unk18 = 0x80808080;
    return asset;
}

u8 *sdfParseAssetParameterFlags(SdfAsset *asset, SdfTextParam *lookup, u8 *data) {
    SdfTextParam *param = (SdfTextParam *)asset;
    u32 flags;
    u8 *cursor;
    u32 packed;

    *(u32 *)param = *(u32 *)data;
    *(u16 *)((u8 *)param + 4) = *(u16 *)(data + 4);
    flags = *(u16 *)(data + 6);
    cursor = data + 8;
    if (flags & 0x1) {
        func_002DA3C0(param, *(u32 *)cursor);
        cursor += 4;
    }
    if (flags & 0x2) {
        func_002DA3D8(param, *(u32 *)cursor);
        cursor += 4;
    }
    if (flags & 0x4) {
        func_002DA438(param, ((SdfResourceList *)lookup)->items[*(u16 *)cursor]);
        cursor += 4;
    }
    if (flags & 0x8) {
        sdfSetPrimaryTextScalars(param, ((f32 *)cursor)[0], ((f32 *)cursor)[1], ((f32 *)cursor)[2], ((f32 *)cursor)[3], ((f32 *)cursor)[4]);
        cursor += 0x14;
    }
    if (flags & 0x10) {
        func_002DA5B0(param, *(u32 *)cursor);
        cursor += 4;
    }
    if (flags & 0x20) {
        packed = *(u32 *)cursor;
        cursor += 4;
        func_002DA5E0(param, ((SdfResourceList *)lookup)->items[packed & 0xFFFF]);
        func_002DA5C8(param, packed >> 16);
    }
    if (flags & 0x40) {
        sdfSetSecondaryTextScalars(param, ((f32 *)cursor)[0], ((f32 *)cursor)[1], ((f32 *)cursor)[2], ((f32 *)cursor)[3], ((f32 *)cursor)[4]);
        cursor += 0x14;
    }
    if (flags & 0x80) {
        func_002DA3F0(param, *(u32 *)cursor);
        cursor += 4;
    }
    if (flags & 0x100) {
        func_002DA408(param, *(u32 *)cursor);
        cursor += 4;
    }
    if (flags & 0x200) {
        func_002DA420(param, *(f32 *)cursor);
        cursor += 4;
    }
    if (flags & 0x400) {
        sdfSetTextScalarPair(param, ((f32 *)cursor)[0], ((f32 *)cursor)[1]);
        cursor += 8;
    }
    return cursor;
}

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

void sdfQueueAssetRelease(SdfAsset *asset) {
    s32 id = (s32)asset;

    if (id != 0) {
        sdfPendingQueuePush(&sdfAssetReleaseQueue, id);
    }
}

void *sdfInitNodeHeaderFromWords(u32 *words, SdfNode *node, s32 offset) {
    u32 *entry = words + offset;

    node->unk3 = 0x30;
    node->unk4 = entry[2] & 0x0FFFFFFF;
    node->unk0 = 0xA;
    node->unk8 = 0;
    node->unkC = 0;
    return (void *)((u8 *)node + 0x10);
}

void sdfAssetCopyTextureState(SdfAsset *asset, SdfAssetEntry *entry) {
    SdfTex *resource;
    entry->unk04 = asset->unk10;
    entry->unk1C = asset->unk1C;
    entry->unk08 = asset->unk14;
    if (sdfForcedAssetTextureMode == 1) {
        entry->unk10 = 0;
    } else {
        entry->unk10 = asset->unk20;
    }
    entry->unk14 = asset->unk28;
    resource = asset->texture;
    if (resource != NULL) {
        entry->unk38 = sdfTexGetPrimarySamplingState(resource);
        entry->unk40 = sdfTexGetPrimaryTextureState(resource);
        entry->unk48 = sdfTexGetPrimaryClampState(resource);
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

void sdfApplyAssetSecondaryEntry(SdfAsset *asset, void *entryArg) {
    u8 *entry = entryArg;
    SdfTex *tex = *(SdfTex **)((u8 *)asset + 0x30);
    u32 mode;

    ((SdfDrawPacket *)entry)->color = asset->unk18;
    mode = *(u32 *)((u8 *)asset + 0x34);
    ((SdfDrawPacket *)entry)->mode = mode;
    ((SdfDrawPacket *)entry)->paletteValue = D_00398198[mode];
    if (tex != NULL) {
        ((SdfDrawPacket *)entry)->textureWords[0] = sdfTexGetPrimarySamplingState(tex);
        ((SdfDrawPacket *)entry)->textureWords[1] = sdfTexGetPrimaryTextureState(tex);
        ((SdfDrawPacket *)entry)->textureWords[2] = sdfTexGetPrimaryClampState(tex);
    }
    func_002DAB80(entry + 0x80, asset->fourth);
}

void sdfAssetCopyPairToTextParam(SdfAsset *asset, SdfTextParam *param) {
    param->unk28 = asset->unk40;
    param->unk2C = asset->unk44;
}

void sdfAssetApplyEntryChanges(SdfAsset *asset, s32 index) {
    u8 flags = asset->pad00[6];
    SdfAssetEntry *entry = asset->entries[index];
    if ((flags >> index) & 1) {
        sdfAssetCopyTextureState(asset, entry);
    }
    if (flags & (4 << index)) {
        sdfCopyAssetPrimarySubParameter(asset, (u8 *)entry);
    }
    if (flags & (16 << index)) {
        sdfApplyAssetSecondaryEntry(asset, entry);
    }
    if (flags & (64 << index)) {
        sdfAssetCopyPairToTextParam(asset, entry);
    }
    asset->pad00[6] = flags & (0x55 << (index ^ 1));
}

void sdfApplyAssetEntryChangesWithForcedTexture(SdfAsset *asset, s32 index) {
    u8 flags = asset->pad00[6];
    SdfAssetEntry *entry = asset->entries[index];

    if ((flags >> index) & 1) {
        sdfAssetCopyTextureState(asset, entry);
    } else if (sdfForcedAssetTextureMode != 0) {
        sdfAssetCopyTextureState(asset, entry);
    }
    if (flags & (4 << index)) {
        sdfCopyAssetPrimarySubParameter(asset, (u8 *)entry);
    }
    if (flags & (16 << index)) {
        sdfApplyAssetSecondaryEntry(asset, entry);
    }
    if (flags & (64 << index)) {
        sdfAssetCopyPairToTextParam(asset, entry);
    }
    asset->pad00[6] = flags & (0x55 << (index ^ 1));
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

    destination->pad00[6] = 0xFF;
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

