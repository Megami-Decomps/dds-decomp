#include "common.h"
#include "pcp_vu0.h"

#include "sdf.h"

extern u32 sdfForcedAssetTextureMode;

struct DevRequest;
/* The list capacity occupies the buffered request's count word. */
extern void sdfDevBufferedRequestGrow(struct DevRequest *request);

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
    u8 dirtyFlags; /* 0x06: set by parameter setters below */
    u8 pad07[9]; /* 0x07 */
    u32 unk10; /* 0x10 */
    u32 unk14; /* 0x14 */
    u8 unk18; /* 0x18: func_002DA5B0 stores a u32 over 0x18-0x1B */
    u8 overrideFlags; /* 0x19: bit 0x2 selects overrideFirst/overrideSecond */
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

typedef struct SdfMapPositionRecord {
    u32 unk00;
    s32 id;
    u8 pad08[0x38];
} SdfMapPositionRecord;
/* Map-position chunk: 0x10 header, then the records back to back. */
typedef struct SdfMapPositionChunk {
    SdfChunk header;
    u8 pad0C[4];
    SdfMapPositionRecord records[1]; /* 0x10 */
} SdfMapPositionChunk;

extern f32 D_00438A48;

extern f32 D_00438A4C;

extern u8 sdfResourceReleaseQueue;

extern u8 sdfAssetReleaseQueue;

void sdfInitializeSynchronizedRequest(void *request, void (*callback)(void));

typedef struct SdfResourceList SdfResourceList;
void sdfResourceListReleaseAssets(SdfResourceList *);

extern SdfSubParam *sdfSubParamCreate(void);

void *sdfAllocSizeClassBlock(s32 size);

void sdfPendingQueuePush(void *queue, s32 assetId);

void sdfDestroyDevRequest(void *);

extern s32 sdfLiveAssetCount;

extern void sdfReleaseChipBlock(void *);

void sdfAssetRelease(SdfAsset *);

extern u64 sdfTexGetPrimaryTextureState(SdfTex *);

extern u64 sdfTexGetPrimarySamplingState(SdfTex *);

extern u64 sdfTexGetPrimaryClampState(SdfTex *);

void sdfAssetCopyTextureState(SdfAsset *, SdfAssetEntry *);

void sdfApplyAssetSecondaryEntry(SdfAsset *, void *);

void *sdfChunkFindRecordById(SdfTextParam *, s32);

/* 0x40-byte map record: an id word, then the three basis vectors. The
 * record loop in the caller steps by sizeof(SdfMapBasisRecord). */
typedef struct SdfMapBasisRecord {
    u32 id;        /* 0x00 */
    u32 pad04;
    u128 eye;      /* 0x10 */
    u128 up;       /* 0x20 */
    u128 at;       /* 0x30 */
} SdfMapBasisRecord; /* 0x40 */

void sdfSetLookAtBasisFromRecord(SdfTextParam *param, SdfMapBasisRecord *record);

void sdfVuTransformMapRecordPosition(SdfTextParam *param, SdfMapBasisRecord *record);

typedef struct SdfResourceList {
    u32 unk0;
    s16 count;
    s16 capacity;
    u32 unk8;
    u32 *items;
} SdfResourceList;

void func_0032C278(u32 asset, s32 arg, f32 value);

void sdfTexReleaseReferenceViaHandler(u32);

INCLUDE_ASM(const s32, "game/code_003325F8", func_003325F8);

extern void sdfInitPacketList(void *);
extern void sdfAppendPacket(void *, void *);
extern void sdfBuildPrimaryAlphaBlendDmaPacket(void *);
extern void sdfBuildPrimaryTestBlendPacket(void *);
extern void sdfBuildPrimaryAlphaAdditiveDmaPacket(void *);
extern void sdfBuildPrimaryAlphaSubtractiveDmaPacket(void *);
typedef struct SdfPacketCommand {
    u8 pad00[0x28];
    u32 opcode; /* 0x28: packet header command */
} SdfPacketCommand;

typedef struct SdfPacketFooter {
    u64 data;
    u64 opcode;
} SdfPacketFooter;

void sdfInitializeDrawPacketGroups(u8 *ctx) {
    u8 *packet = ctx;
    s32 i;

    sdfBuildPrimaryAlphaBlendDmaPacket(ctx + 0x20);
    sdfBuildPrimaryTestBlendPacket(ctx + 0x80);
    sdfBuildPrimaryAlphaAdditiveDmaPacket(ctx + 0xE0);
    sdfBuildPrimaryAlphaSubtractiveDmaPacket(ctx + 0x140);
    for (i = 0; i != 4; i++) {
        ((SdfPacketCommand *)packet)->opcode = 0x11000000;
        sdfInitPacketList(packet);
        sdfAppendPacket(packet, packet + 0x20);
        packet += 0x60;
    }
    sdfInitPacketList(ctx + 0x180);
    ((SdfPacketFooter *)(ctx + 0x1A0))->data = 0;
    ((SdfPacketFooter *)(ctx + 0x1A0))->opcode = 0x13000000;
    sdfAppendPacket(ctx + 0x180, ctx + 0x1A0);
}

typedef struct SdfPacketOwner {
    u8 pad00[0x10];
    void (*sync)(struct SdfPacketOwner *, void *);
    void (*draw)(struct SdfPacketOwner *, s32, void *);
} SdfPacketOwner;
void sdfSubmitDrawPacketGroups(SdfPacketOwner **owners, u8 *packets) {
    s32 i;

    for (i = 0; i != 4; i++) {
        owners[i]->draw(owners[i], 1, packets + i * 0x60);
    }
    owners[3]->sync(owners[3], packets + 0x180);
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

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332AD8);

u32 sdfCountMapPositionRecords(SdfTextParam *param) {
    SdfChunk *chunk = sdfChunkFindByTag(param, SDF_CHUNK_MAP_POSITIONS);
    if (chunk != NULL) {
        return (chunk->size - 0x10) >> 6;
    }
    return 0;
}

extern void sdfPostmultiplyVuMatrixFromMemory(void *);
/* vu0 routine: look-at basis rows in vf28-vf31 from the record's vectors, then transform by the matrix */
void sdfSetLookAtBasisFromRecord(SdfTextParam *param, SdfMapBasisRecord *record) {
    u8 *matrix = sdfModelFindDrawNode(param, record->id);
    u8 *p;

    p = (u8 *)record + 0x20;
    VU0_LOAD_VF(vf10, p);
    VU0_MOVE_VF(vf30, vf10);
    VU0_MOVE_VF(vf11, vf10);
    p = (u8 *)record + 0x30;
    VU0_LOAD_VF(vf10, p);
    VU0_NEGATE_XYZ(vf10);
    VU0_MOVE_VF(vf29, vf10);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf28, vf10);
    VU0_LOAD_VF(vf31, (u8 *)record + 0x10);
    VU0_SET_W_ONE(vf31);
    sdfPostmultiplyVuMatrixFromMemory(matrix + 0xC0);
}

extern u8 *sdfModelFindDrawNode(SdfTextParam *, u32);
void sdfVuTransformMapRecordPosition(SdfTextParam *param, SdfMapBasisRecord *record) {
    u8 *matrix = sdfModelFindDrawNode(param, record->id) + 0xC0;

    VU0_LOAD_MATRIX(matrix);
    VU0_LOAD_VF(vf10, (u8 *)record + 0x10);
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
    param->overrideFlags = param->overrideFlags & 0xfd;
}

f32 sdfGetFirstTextOverrideOrDefault(SdfTextParam *param) {
    if ((param->overrideFlags & 2) != 0) {
        return param->overrideFirst;
    }
    return D_00438A48;
}

f32 sdfGetSecondTextOverrideOrDefault(SdfTextParam *param) {
    if ((param->overrideFlags & 2) != 0) {
        return param->overrideSecond;
    }
    return D_00438A4C;
}

void sdfSetForcedAssetTextureMode(u32 mode) {
    sdfForcedAssetTextureMode = mode;
}

void *sdfDevCreateBufferedRequest(s32, s32, s32);
SdfResourceList *sdfCreateConfiguredBufferedResourceList(s32 capacity) {
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

void sdfReduceResourceListCount(s32 listAddress, s32 newCount, s32 applyReduction) {
    s32 countCursor;

    if ((newCount < ((SdfResourceList *)listAddress)->count) && (applyReduction != 0)) {
        countCursor = (s32)newCount;
        do {
            countCursor = countCursor + 1;
        } while ((s64)countCursor != (s64)((SdfResourceList *)listAddress)->count);
        ((SdfResourceList *)listAddress)->count = (s16)newCount;
    }
    sdfDevResizeBufferedRequest();
}

extern u32 func_0032B6B0(u32);
/* Allocate a list of cloned resource items; a null source returns null. */
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
        clone->items[itemIndex] = func_0032B6B0(source->items[itemIndex]);
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
            func_0032C278((u32)asset, arg, value);
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

void func_00333270(SdfTextParam *param, u32 value) {
    param->unk10 = value;
    param->dirtyFlags = param->dirtyFlags | 3;
}

void func_00333288(SdfTextParam *param, u32 value) {
    param->unk14 = value;
    param->dirtyFlags = param->dirtyFlags | 3;
}

void func_003332A0(SdfTextParam *param, u32 value) {
    param->unk20 = value;
    param->dirtyFlags = param->dirtyFlags | 3;
}

void func_003332B8(SdfTextParam *param, u32 value) {
    *(u32 *)&param->unk28 = value;
    param->dirtyFlags = param->dirtyFlags | 3;
}

void func_003332D0(SdfTextParam *param, f32 value) {
    param->unk1C = value;
    param->dirtyFlags = param->dirtyFlags | 3;
}

void func_003332E8(SdfTextParam *param, u32 value) {
    *(u32 *)&param->unk2C = value;
    param->dirtyFlags = param->dirtyFlags | 3;
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

void func_00333460(SdfTextParam *param, u32 value) {
    *(u32 *)&param->unk18 = value;
    param->dirtyFlags = param->dirtyFlags | 0x30;
}

void func_00333478(SdfTextParam *param, u32 value) {
    param->unk34 = value;
    param->dirtyFlags = param->dirtyFlags | 0x30;
}

void func_00333490(SdfTextParam *param, u32 value) {
    param->unk30 = value;
    param->dirtyFlags = param->dirtyFlags | 0x30;
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

extern void *sdfAllocAndClearQuadwords(s32);
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

u8 *sdfParseAssetParameterFlags(SdfTextParam *param, SdfTextParam *lookup, u8 *data) {
    u32 flags;
    u8 *cursor;
    u32 packed;

    *(u32 *)param = *(u32 *)data;
    *(u16 *)((u8 *)param + 4) = *(u16 *)(data + 4);
    flags = *(u16 *)(data + 6);
    cursor = data + 8;
    if (flags & 0x1) {
        func_00333270(param, *(u32 *)cursor);
        cursor += 4;
    }
    if (flags & 0x2) {
        func_00333288(param, *(u32 *)cursor);
        cursor += 4;
    }
    if (flags & 0x4) {
        func_003332E8(param, ((SdfResourceList *)lookup)->items[*(u16 *)cursor]);
        cursor += 4;
    }
    if (flags & 0x8) {
        sdfSetPrimaryTextScalars(param, ((f32 *)cursor)[0], ((f32 *)cursor)[1], ((f32 *)cursor)[2], ((f32 *)cursor)[3], ((f32 *)cursor)[4]);
        cursor += 0x14;
    }
    if (flags & 0x10) {
        func_00333460(param, *(u32 *)cursor);
        cursor += 4;
    }
    if (flags & 0x20) {
        packed = *(u32 *)cursor;
        cursor += 4;
        func_00333490(param, ((SdfResourceList *)lookup)->items[packed & 0xFFFF]);
        func_00333478(param, packed >> 16);
    }
    if (flags & 0x40) {
        sdfSetSecondaryTextScalars(param, ((f32 *)cursor)[0], ((f32 *)cursor)[1], ((f32 *)cursor)[2], ((f32 *)cursor)[3], ((f32 *)cursor)[4]);
        cursor += 0x14;
    }
    if (flags & 0x80) {
        func_003332A0(param, *(u32 *)cursor);
        cursor += 4;
    }
    if (flags & 0x100) {
        func_003332B8(param, *(u32 *)cursor);
        cursor += 4;
    }
    if (flags & 0x200) {
        func_003332D0(param, *(f32 *)cursor);
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

void sdfQueueAssetRelease(s32 assetId) {
    s32 id = assetId;

    if (id != 0) {
        sdfPendingQueuePush(&sdfAssetReleaseQueue, id);
    }
}

void *sdfInitNodeHeaderFromWords(u32 *words, SdfNode *node, s32 wordIndex) {
    u32 *entry = words + wordIndex;

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

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333A30);

void sdfCopyAssetPrimarySubParameter(s32 assetAddress, s32 entryAddress) {
    func_00333A30(entryAddress + 0x68, ((SdfAsset *)assetAddress)->third);
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

extern u16 D_0040B348[];
void sdfApplyAssetSecondaryEntry(SdfAsset *asset, void *entryArg) {
    u8 *entry = entryArg;
    SdfTex *tex = *(SdfTex **)((u8 *)asset + 0x30);
    u32 mode;

    ((SdfDrawPacket *)entry)->color = asset->unk18;
    mode = *(u32 *)((u8 *)asset + 0x34);
    ((SdfDrawPacket *)entry)->mode = mode;
    ((SdfDrawPacket *)entry)->paletteValue = D_0040B348[mode];
    if (tex != NULL) {
        ((SdfDrawPacket *)entry)->textureWords[0] = sdfTexGetPrimarySamplingState(tex);
        ((SdfDrawPacket *)entry)->textureWords[1] = sdfTexGetPrimaryTextureState(tex);
        ((SdfDrawPacket *)entry)->textureWords[2] = sdfTexGetPrimaryClampState(tex);
    }
    func_00333A30(entry + 0x80, asset->fourth);
}

void sdfAssetCopyPairToTextParam(SdfTextParam *asset, SdfTextParam *param) {
    param->unk28 = asset->scalarPairFirst;
    param->unk2C = asset->scalarPairSecond;
}

void sdfAssetApplyEntryChanges(SdfAsset *asset, s32 index) {
    u8 flags = asset->pad00[6];
    void *entry = asset->entries[index];
    if ((flags >> index) & 1) {
        sdfAssetCopyTextureState(asset, entry);
    }
    if (flags & (4 << index)) {
        sdfCopyAssetPrimarySubParameter(asset, entry);
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
    void *entry = asset->entries[index];
    if ((flags >> index) & 1) {
        sdfAssetCopyTextureState(asset, entry);
    } else if (sdfForcedAssetTextureMode != 0) {
        sdfAssetCopyTextureState(asset, entry);
    }
    if (flags & (4 << index)) {
        sdfCopyAssetPrimarySubParameter(asset, entry);
    }
    if (flags & (16 << index)) {
        sdfApplyAssetSecondaryEntry(asset, entry);
    }
    if (flags & (64 << index)) {
        sdfAssetCopyPairToTextParam(asset, entry);
    }
    asset->pad00[6] = flags & (0x55 << (index ^ 1));
}

SdfAsset *sdfCreateAssetWithDrawEntries(void);
u8 *sdfParseAssetParameterFlags(SdfTextParam *, SdfTextParam *, u8 *);
/* Parse the declared number of variable-sized assets, reserving at least the minimum capacity. */
SdfResourceList *sdfAssetListParse(SdfTextParam *lookup, u32 *data) {
    u32 remainingAssets = *data;
    u8 *parameterCursor = (u8 *)(data + 1);
    SdfResourceList *list = sdfCreateResourceList(remainingAssets >= SDF_ASSET_LIST_MIN_CAPACITY ? remainingAssets : SDF_ASSET_LIST_MIN_CAPACITY);
    while (remainingAssets != 0) {
        SdfAsset *asset = sdfCreateAssetWithDrawEntries();
        parameterCursor = sdfParseAssetParameterFlags((SdfTextParam *)asset, lookup, parameterCursor);
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

typedef struct SdfSubParamWords {
    u32 word[6];
} SdfSubParamWords;

/* Copy base state; only present source subparameters overwrite destination subparameter blocks. */
void sdfCopyAssetParameterState(SdfAsset *destination, SdfAsset *source) {
    SdfSubParamWords *subParameters;

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
        *(SdfSubParamWords *)sdfEnsurePrimaryTextSubParam((SdfTextParam *)destination) = *subParameters;
    }
    subParameters = source->fourth;
    if (subParameters != NULL) {
        *(SdfSubParamWords *)sdfEnsureSecondaryTextSubParam((SdfTextParam *)destination) = *subParameters;
    }
    destination->unk40 = source->unk40;
    destination->unk44 = source->unk44;
}

void sdfCopyAssetParameterState(SdfAsset *, SdfAsset *);
/* Copy paired items using the destination count; source must contain at least that many items. */
void sdfCopyAssetListParameterState(SdfResourceList *destination, SdfResourceList *source) {
    s32 itemCount = destination->count;
    u32 *sourceItems = source->items;
    u32 *destinationItems = destination->items;
    s32 itemIndex;

    for (itemIndex = 0; itemIndex < itemCount; itemIndex++) {
        sdfCopyAssetParameterState((SdfAsset *)destinationItems[itemIndex], (SdfAsset *)sourceItems[itemIndex]);
    }
}

extern s32 (*D_0040B358[])(u32, u32);
/* The high halfword selects the handler, which receives the complete command word unchanged. */
s32 sdfDispatchAssetCommandWord(u32 context, u32 commandWord) {
    D_0040B358[commandWord >> 16](context, commandWord);
}

INCLUDE_SDATA(const s32, "game/code_003325F8", sdfLiveAssetCount);

INCLUDE_SDATA(const s32, "game/code_003325F8", sdfForcedAssetTextureMode);

