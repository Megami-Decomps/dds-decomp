#include "common.h"
#include "sdf.h"

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
    f32 unk40; /* 0x40 */
    f32 unk44; /* 0x44 */
    u8 pad48[0x40]; /* 0x48 */
    f32 overrideFirst; /* 0x88 */
    f32 overrideSecond; /* 0x8C */
    void *chunkTable; /* 0x90: resource chunk searched by tag */
} SdfTextParam;

typedef struct SdfChunk {
    u32 id;   /* 0x0: entry id, 0 terminates the list */
    u32 size; /* 0x4: byte offset to the next entry */
} SdfChunk;

typedef struct SdfMapPositionRecord {
    u32 unk00;
    s32 id;
    u8 pad08[0x38];
} SdfMapPositionRecord;

#define SDF_CHUNK_MAP_POSITIONS 0x534F504D /* "MPOS" in little-endian byte order */

typedef struct SdfResourceList {
    u32 unk0;
    s16 count;
    s16 capacity;
    u32 unk8;
    u32 *items;
} SdfResourceList;

extern SdfSubParam *sdfSubParamCreate(void);

extern u32 D_003BD34C;
extern f32 D_003BD358;
extern f32 D_003BD35C;
extern u8 D_003BDA10;
extern u8 D_003BDA18;
extern s32 D_003BD348;
extern void func_002CFF98(void *);
void *sdfDevCreateBufferedRequest(s32, s32, s32);
extern u64 func_002D2468(SdfTex *);
extern u64 func_002D2478(SdfTex *);
extern u64 func_002D2488(SdfTex *);

void *sdfChunkFindById(SdfChunk *chunk, s32 id);
void *func_002CFEB8(s32 size);
void *sdfChunkFindRecordById(SdfTextParam *, s32);
void func_002D9D00(SdfTextParam *param, void *resource);
void func_002D9D80(SdfTextParam *param, void *resource);
void sdfInitializeSynchronizedRequest(void *arg0, void (*arg1)(void));
void func_002D3C30(void *arg0, s32 arg1);
void func_002DA290(void);
void sdfAssetRelease(SdfAsset *);
void sdfDestroyDevRequest(void *);
void func_002D2D00(u32);
SdfAsset *func_002DA730(void);
u8 *func_002DA830(SdfAsset *, SdfTextParam *, u8 *);
void func_002DA358(SdfResourceList *, SdfAsset *);
void sdfAssetCopyTextureState(SdfAsset *, SdfAssetEntry *);
void func_002DAC88(SdfAsset *, void *);
INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9748);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D99B0);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9A70);

void *sdfChunkFindById(SdfChunk *chunk, s32 id) {
    u32 currentId;

    if (chunk == NULL) {
        return NULL;
    }
    currentId = chunk->id;
    while (currentId != 0) {
        if (currentId == id) {
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

s32 sdfNamedChunkFindId(SdfTextParam *param, const char *name) {
    SdfChunk *chunk = sdfChunkFindByTag(param, 0x4D4E444E);
    u8 *entry;
    u8 *end;
    u32 length;
    if (chunk == NULL) {
        return -1;
    }
    entry = (u8 *)chunk + 8;
    end = (u8 *)chunk + chunk->size;
    length = strlen(name);
    do {
        u32 entryLength = strlen((char *)entry);
        u8 *next = (u8 *)(((u32)(entry + entryLength + 4)) & ~3U);
        if (entryLength == length && memcmp(entry, name, length) == 0) {
            return *(s32 *)next;
        }
        entry = next + 4;
    } while (entry < end);
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

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9D00);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002D9D80);

void *sdfChunkFindRecordById(SdfTextParam *param, s32 id) {
    SdfChunk *chunk = sdfChunkFindByTag(param, SDF_CHUNK_MAP_POSITIONS);
    SdfMapPositionRecord *entry;
    u8 *end;
    if (chunk == NULL) {
        return NULL;
    }
    entry = (SdfMapPositionRecord *)((u8 *)chunk + 0x10);
    end = (u8 *)chunk + chunk->size;
    while ((u8 *)entry < end) {
        if (entry->id == id) {
            return entry;
        }
        entry++;
    }
    return NULL;
}

s32 func_002D9E58(SdfTextParam *param, s32 id) {
    void *resource = sdfChunkFindRecordById(param, id);
    if (resource != NULL) {
        func_002D9D00(param, resource);
        return 1;
    }
    return 0;
}

s32 func_002D9E98(SdfTextParam *param, s32 id) {
    void *resource = sdfChunkFindRecordById(param, id);
    if (resource != NULL) {
        func_002D9D80(param, resource);
        return 1;
    }
    return 0;
}

u32 sdfGetUniqueChunkValue(SdfTextParam *param) {
    SdfChunk *chunk = sdfChunkFindByTag(param, 0x51494e55);
    if (chunk != NULL) {
        return *(u32 *)((u8 *)chunk + 8);
    }
    return 0;
}

u32 sdfGetLodChunkValue(SdfTextParam *param) {
    SdfChunk *chunk = sdfChunkFindByTag(param, 0x43444f4c);
    if (chunk != NULL) {
        return *(u32 *)((u8 *)chunk + 8);
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

void func_002D9FA0(u32 value) {
    D_003BD34C = value;
}

void func_002D9FA8(u32 capacity) {
    sdfDevCreateBufferedRequest(capacity, 4, 4);
}

void sdfResourceListRelease(SdfResourceList *list, s32 freeItems) {
    s32 i;
    s32 count;
    if (list == NULL) {
        return;
    }
    if (freeItems != 0) {
        count = list->count;
        for (i = 0; i < count; i++) {
            func_002D2D00(list->items[i]);
        }
    }
    sdfDestroyDevRequest(list);
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA058);

void sdfReduceResourceListCount(SdfResourceList *list, s32 count, s32 enabled) {
    s32 cursor;

    if ((count < list->count) && (enabled != 0)) {
        cursor = (s32)count;
        do {
            cursor = cursor + 1;
        } while ((s64)cursor != (s64)list->count);
        list->count = (s16)count;
    }
    func_002E7730();
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA118);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA1B0);

void sdfRegisterResourceQueueCallbacks(void) {
    sdfInitializeSynchronizedRequest(&D_003BDA10, func_002DA290);
    sdfInitializeSynchronizedRequest(&D_003BDA18, sdfAssetRelease);
}

SdfResourceList *sdfCreateResourceList(s32 capacity) {
    return sdfDevCreateBufferedRequest(capacity, 4, 8);
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA290);

void sdfReleaseQueuedResource(void *resource, s32 retained) {
    if (resource == NULL) {
        return;
    }
    if (retained != 0) {
        func_002D3C30(&D_003BDA10, (s32)resource);
    } else {
        sdfDestroyDevRequest(resource);
    }
}

void func_002DA340(void) {
    func_002E76B0();
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA358);

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

    temp = func_002CFEB8(0x18);
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

void func_002DA4C8(SdfTextParam *param, f32 a, f32 b, f32 c, f32 d, f32 e) {
    f32 *values = sdfEnsurePrimaryTextSubParam(param)->scalar.values;
    values[0] = a;
    values[1] = b;
    values[2] = c;
    values[3] = d;
    values[4] = e;
    param->dirtyFlags |= 0xc;
}

void func_002DA548(SdfTextParam *param, const f32 *input) {
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

void func_002DA630(SdfTextParam *param, f32 a, f32 b, f32 c, f32 d, f32 e) {
    f32 *values = sdfEnsureSecondaryTextSubParam(param)->scalar.values;
    values[0] = a;
    values[1] = b;
    values[2] = c;
    values[3] = d;
    values[4] = e;
    param->dirtyFlags |= 0x30;
}

void func_002DA6B0(SdfTextParam *param, const f32 *input) {
    f32 *values = sdfEnsureSecondaryTextSubParam(param)->scalar.values;
    values[0] = input[0];
    values[1] = input[1];
    values[2] = input[2];
    values[3] = input[3];
    values[4] = input[4];
    param->dirtyFlags |= 0x30;
}

void func_002DA718(SdfTextParam *param, f32 first, f32 second) {
    param->unk40 = first;
    param->unk44 = second;
    param->dirtyFlags = param->dirtyFlags | 0xC0;
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA730);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DA830);

void sdfAssetRelease(SdfAsset *asset) {
    if (asset == NULL) {
        return;
    }
    D_003BD348--;
    func_002CFF98(asset->entries[0]);
    func_002CFF98(asset->entries[1]);
    func_002CFF98(asset->third);
    func_002CFF98(asset->fourth);
    func_002CFF98(asset);
}

void sdfQueueAssetRelease(SdfAsset *asset) {
    s32 id = (s32)asset;

    if (id != 0) {
        func_002D3C30(&D_003BDA18, id);
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
    if (D_003BD34C == 1) {
        entry->unk10 = 0;
    } else {
        entry->unk10 = asset->unk20;
    }
    entry->unk14 = asset->unk28;
    resource = asset->unk2C;
    if (resource != NULL) {
        entry->unk38 = func_002D2478(resource);
        entry->unk40 = func_002D2468(resource);
        entry->unk48 = func_002D2488(resource);
    }
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DAB80);

void func_002DAC68(SdfAsset *asset, u8 *entry) {
    func_002DAB80(entry + 0x68, asset->third);
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DAC88);

void func_002DAD18(SdfAsset *arg0, SdfTextParam *arg1) {
    arg1->unk28 = arg0->unk40;
    arg1->unk2C = arg0->unk44;
}

void sdfAssetApplyEntryChanges(SdfAsset *asset, s32 index) {
    u8 flags = asset->pad00[6];
    SdfAssetEntry *entry = asset->entries[index];
    if ((flags >> index) & 1) {
        sdfAssetCopyTextureState(asset, entry);
    }
    if (flags & (4 << index)) {
        func_002DAC68(asset, (u8 *)entry);
    }
    if (flags & (16 << index)) {
        func_002DAC88(asset, entry);
    }
    if (flags & (64 << index)) {
        func_002DAD18(asset, entry);
    }
    asset->pad00[6] = flags & (0x55 << (index ^ 1));
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DAE00);

SdfResourceList *sdfAssetListParse(SdfTextParam *param, u32 *data) {
    u32 count = *data;
    u8 *cursor = (u8 *)(data + 1);
    SdfResourceList *list = sdfCreateResourceList(count >= 0x20 ? count : 0x20);
    while (count != 0) {
        SdfAsset *asset = func_002DA730();
        cursor = func_002DA830(asset, param, cursor);
        func_002DA358(list, asset);
        count--;
    }
    return list;
}

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DAF88);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DAFE8);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DB048);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DB158);

INCLUDE_ASM(const s32, "game/code_002D9748", func_002DB1C8);

INCLUDE_SDATA(const s32, "game/code_002D9748", D_003BD348);

INCLUDE_SDATA(const s32, "game/code_002D9748", D_003BD34C);

