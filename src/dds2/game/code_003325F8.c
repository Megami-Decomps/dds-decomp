#include "common.h"

#include "sdf.h"

extern u32 D_00438A3C;

typedef struct SdfSubParam {
    u64 unk0; /* 0x0 */
    u64 unk8; /* 0x8 */
    u32 unk10; /* 0x10 */
    u32 unk14; /* 0x14 */
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


extern f32 D_00438A48;

extern f32 D_00438A4C;

extern u8 D_00439170;

extern u8 D_00439178;

void sdfInitializeSynchronizedRequest(void *arg0, void (*arg1)(void));

void func_00333140(void);

extern SdfSubParam *sdfSubParamCreate(void);

void *func_00328D68(s32 size);

void func_0032CAE0(void *arg0, s32 arg1);

void sdfDestroyDevRequest(void *);

extern s32 D_00438A38;

extern void func_00328E48(void *);

void sdfAssetRelease(SdfAsset *);

extern u64 func_0032B318(SdfTex *);

extern u64 func_0032B328(SdfTex *);

extern u64 func_0032B338(SdfTex *);

void sdfAssetCopyTextureState(SdfAsset *, SdfAssetEntry *);

void func_00333B38(SdfAsset *, void *);

void *sdfChunkFindRecordById(SdfTextParam *, s32);

void func_00332BB0(SdfTextParam *param, void *resource);

void func_00332C30(SdfTextParam *param, void *resource);

typedef struct SdfResourceList {
    u32 unk0;
    s16 count;
    s16 capacity;
    u32 unk8;
    u32 *items;
} SdfResourceList;

void func_0032BBB0(u32);

INCLUDE_ASM(const s32, "game/code_003325F8", func_003325F8);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332860);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332920);

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

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332AD8);

u32 sdfCountMapPositionRecords(SdfTextParam *param) {
    SdfChunk *chunk = sdfChunkFindByTag(param, SDF_CHUNK_MAP_POSITIONS);
    if (chunk != NULL) {
        return (chunk->size - 0x10) >> 6;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332BB0);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332C30);

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

s32 func_00332D08(SdfTextParam *param, s32 id) {
    void *resource = sdfChunkFindRecordById(param, id);
    if (resource != NULL) {
        func_00332BB0(param, resource);
        return 1;
    }
    return 0;
}

s32 func_00332D48(SdfTextParam *param, s32 id) {
    void *resource = sdfChunkFindRecordById(param, id);
    if (resource != NULL) {
        func_00332C30(param, resource);
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

void sdfTextParamSetOverrides(SdfTextParam *param, f32 first, f32 second) {
    param->overrideFirst = first;
    param->overrideSecond = second;
    param->overrideFlags = param->overrideFlags | 2;
}

void sdfTextParamClearOverrides(SdfTextParam *param) {
    param->overrideFlags = param->overrideFlags & 0xfd;
}

f32 sdfTextParamGetFirstValue(SdfTextParam *param) {
    if ((param->overrideFlags & 2) != 0) {
        return param->overrideFirst;
    }
    return D_00438A48;
}

f32 sdfTextParamGetSecondValue(SdfTextParam *param) {
    if ((param->overrideFlags & 2) != 0) {
        return param->overrideSecond;
    }
    return D_00438A4C;
}

void func_00332E50(u32 arg0) {
    D_00438A3C = arg0;
}

void *sdfDevCreateBufferedRequest(s32, s32, s32);
void func_00332E58(u32 arg0) {
    sdfDevCreateBufferedRequest(arg0, 4, 4);
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
            func_0032BBB0(list->items[i]);
        }
    }
    sdfDestroyDevRequest(list);
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332F08);

void sdfReduceResourceListCount(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp_v0;

    if ((arg1 < *(s16 *)(arg0 + 4)) && (arg2 != 0)) {
        temp_v0 = (s32)arg1;
        do {
            temp_v0 = temp_v0 + 1;
        } while ((s64)temp_v0 != (s64)*(s16 *)(arg0 + 4));
        *(s16 *)(arg0 + 4) = (s16)arg1;
    }
    func_003405D8();
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332FC8);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333060);

void sdfRegisterResourceQueueCallbacks(void) {
    sdfInitializeSynchronizedRequest(&D_00439170, func_00333140);
    sdfInitializeSynchronizedRequest(&D_00439178, sdfAssetRelease);
}

SdfResourceList *sdfCreateResourceList(s32 capacity) {
    return sdfDevCreateBufferedRequest(capacity, 4, 8);
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333140);

void sdfReleaseQueuedResource(void *resource, s32 retained) {
    if (resource == NULL) {
        return;
    }
    if (retained != 0) {
        func_0032CAE0(&D_00439170, (s32)resource);
    } else {
        sdfDestroyDevRequest(resource);
    }
}

void func_003331F0(void) {
    func_00340558();
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333208);

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

    temp = func_00328D68(0x18);
    temp->unk8 = (((u64)0x3F800000 << 16 | 0x3F80) << 16);
    temp->unk0 = 0;
    temp->unk10 = 0;
    return temp;
}

void sdfEnsurePrimaryTextSubParam(SdfTextParam *param) {
    SdfSubParam *subParam;

    if (param->primarySubParam == NULL) {
        subParam = sdfSubParamCreate();
        param->primarySubParam = subParam;
    }
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333378);

INCLUDE_ASM(const s32, "game/code_003325F8", func_003333F8);

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

void sdfEnsureSecondaryTextSubParam(SdfTextParam *param) {
    SdfSubParam *subParam;

    if (param->secondarySubParam == NULL) {
        subParam = sdfSubParamCreate();
        param->secondarySubParam = subParam;
    }
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_003334E0);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333560);

void func_003335C8(SdfTextParam *param, f32 first, f32 second) {
    param->unk40 = first;
    param->unk44 = second;
    param->dirtyFlags = param->dirtyFlags | 0xC0;
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_003335E0);

INCLUDE_ASM(const s32, "game/code_003325F8", func_003336E0);

void sdfAssetRelease(SdfAsset *asset) {
    if (asset == NULL) {
        return;
    }
    D_00438A38--;
    func_00328E48(asset->entries[0]);
    func_00328E48(asset->entries[1]);
    func_00328E48(asset->third);
    func_00328E48(asset->fourth);
    func_00328E48(asset);
}

void sdfQueueAssetRelease(s32 arg0) {
    s32 id = arg0;

    if (id != 0) {
        func_0032CAE0(&D_00439178, id);
    }
}

void *sdfInitNodeHeaderFromWords(u32 *arg0, SdfNode *arg1, s32 arg2) {
    u32 *entry = arg0 + arg2;

    arg1->unk3 = 0x30;
    arg1->unk4 = entry[2] & 0x0FFFFFFF;
    arg1->unk0 = 0xA;
    arg1->unk8 = 0;
    arg1->unkC = 0;
    return (void *)((u8 *)arg1 + 0x10);
}

void sdfAssetCopyTextureState(SdfAsset *asset, SdfAssetEntry *entry) {
    SdfTex *resource;
    entry->unk04 = asset->unk10;
    entry->unk1C = asset->unk1C;
    entry->unk08 = asset->unk14;
    if (D_00438A3C == 1) {
        entry->unk10 = 0;
    } else {
        entry->unk10 = asset->unk20;
    }
    entry->unk14 = asset->unk28;
    resource = asset->unk2C;
    if (resource != NULL) {
        entry->unk38 = func_0032B328(resource);
        entry->unk40 = func_0032B318(resource);
        entry->unk48 = func_0032B338(resource);
    }
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333A30);

void func_00333B18(s32 arg0, s32 arg1) {
    func_00333A30(arg1 + 0x68, *(u32 *)(arg0 + 0x38));
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333B38);

void func_00333BC8(SdfTextParam *arg0, SdfTextParam *arg1) {
    arg1->unk28 = arg0->unk40;
    arg1->unk2C = arg0->unk44;
}

void sdfAssetApplyEntryChanges(SdfAsset *asset, s32 index) {
    u8 flags = asset->pad00[6];
    void *entry = asset->entries[index];
    if ((flags >> index) & 1) {
        sdfAssetCopyTextureState(asset, entry);
    }
    if (flags & (4 << index)) {
        func_00333B18(asset, entry);
    }
    if (flags & (16 << index)) {
        func_00333B38(asset, entry);
    }
    if (flags & (64 << index)) {
        func_00333BC8(asset, entry);
    }
    asset->pad00[6] = flags & (0x55 << (index ^ 1));
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333CB0);

SdfAsset *func_003335E0(void);
u8 *func_003336E0(SdfAsset *, SdfTextParam *, u8 *);
void func_00333208(SdfResourceList *, SdfAsset *);
SdfResourceList *sdfAssetListParse(SdfTextParam *param, u32 *data) {
    u32 count = *data;
    u8 *cursor = (u8 *)(data + 1);
    SdfResourceList *list = sdfCreateResourceList(count >= 0x20 ? count : 0x20);
    while (count != 0) {
        SdfAsset *asset = func_003335E0();
        cursor = func_003336E0(asset, param, cursor);
        func_00333208(list, asset);
        count--;
    }
    return list;
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333E38);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333E98);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333EF8);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00334008);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00334078);

INCLUDE_SDATA(const s32, "game/code_003325F8", D_00438A38);

INCLUDE_SDATA(const s32, "game/code_003325F8", D_00438A3C);

