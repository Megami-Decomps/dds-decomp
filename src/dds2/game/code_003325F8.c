#include "common.h"
#include "pcp_vu0.h"

#include "sdf.h"

extern u32 D_00438A3C;

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
    u32 firstValue; /* 0x8: payload of single-value UNIQ/LODC chunks */
} SdfChunk;

typedef struct SdfMapPositionRecord {
    u32 unk00;
    s32 id;
    u8 pad08[0x38];
} SdfMapPositionRecord;

#define SDF_CHUNK_MAP_POSITIONS 0x534F504D /* "MPOS" in little-endian byte order */
#define SDF_CHUNK_UNIQUE_VALUE 0x51494e55 /* "UNIQ" in little-endian byte order */
#define SDF_CHUNK_LOD_VALUE 0x43444f4c /* "LODC" in little-endian byte order */


extern f32 D_00438A48;

extern f32 D_00438A4C;

extern u8 D_00439170;

extern u8 D_00439178;

void sdfInitializeSynchronizedRequest(void *request, void (*callback)(void));

typedef struct SdfResourceList SdfResourceList;
void sdfResourceListReleaseAssets(SdfResourceList *);

extern SdfSubParam *sdfSubParamCreate(void);

void *func_00328D68(s32 size);

void sdfPendingQueuePush(void *queue, s32 assetId);

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

void sdfSetLookAtBasisFromRecord(SdfTextParam *param, void *resource);

void func_00332C30(SdfTextParam *param, void *resource);

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
extern void func_0032E4B8(void *);
extern void func_0032E5C8(void *);
extern void func_0032E6D8(void *);
extern void func_0032E7E8(void *);
typedef struct SdfPacketCommand {
    u8 pad00[0x28];
    u32 opcode; /* 0x28: packet header command */
} SdfPacketCommand;

typedef struct SdfPacketFooter {
    u64 data;
    u64 opcode;
} SdfPacketFooter;

void func_00332860(u8 *ctx) {
    u8 *packet = ctx;
    s32 i;

    func_0032E4B8(ctx + 0x20);
    func_0032E5C8(ctx + 0x80);
    func_0032E6D8(ctx + 0xE0);
    func_0032E7E8(ctx + 0x140);
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
void func_00332920(SdfPacketOwner **owners, u8 *packets) {
    s32 i;

    for (i = 0; i != 4; i++) {
        owners[i]->draw(owners[i], 1, packets + i * 0x60);
    }
    owners[3]->sync(owners[3], packets + 0x180);
}

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

extern void func_00336C10(void *);
/* vu0 routine: look-at basis rows in vf28-vf31 from the resource vectors (+0x10, +0x20, +0x30), then transform by the matrix */
void sdfSetLookAtBasisFromRecord(SdfTextParam *param, void *resource) {
    u8 *matrix = sdfModelFindDrawNode(param, *(u32 *)resource);
    u8 *p;

    p = (u8 *)resource + 0x20;
    __asm__ volatile(
        ".set noreorder\n\t"
        "lqc2 vf10, 0(%0)\n\t"
        "vmove.xyzw vf30, vf10\n\t"
        "vmove.xyzw vf11, vf10\n\t"
        ".set reorder"
        : : "r"(p));
    p = (u8 *)resource + 0x30;
    __asm__ volatile(
        ".set noreorder\n\t"
        "lqc2 vf10, 0(%0)\n\t"
        "vsub.xyz vf10, vf0, vf10\n\t"
        "vmove.xyzw vf29, vf10\n\t"
        "vopmula.xyz ACC, vf10, vf11\n\t"
        "vopmsub.xyz vf10, vf11, vf10\n\t"
        "vmul.xyz vf2, vf10, vf10\n\t"
        "vmulax.w ACC, vf0, vf2x\n\t"
        "vmadday.w ACC, vf0, vf2y\n\t"
        "vmaddz.w vf2, vf0, vf2z\n\t"
        "vrsqrt Q, vf0w, vf2w\n\t"
        "vwaitq\n\t"
        "vmulq.xyz vf10, vf10, Q\n\t"
        "vmove.xyzw vf28, vf10\n\t"
        ".set reorder"
        : : "r"(p));
    __asm__ volatile(
        ".set noreorder\n\t"
        "lqc2 vf31, 0(%0)\n\t"
        "vmove.w vf31, vf0\n\t"
        ".set reorder"
        : : "r"((u8 *)resource + 0x10));
    func_00336C10(matrix + 0xC0);
}

extern u8 *sdfModelFindDrawNode(SdfTextParam *, u32);
void func_00332C30(SdfTextParam *param, void *resource) {
    u8 *matrix = sdfModelFindDrawNode(param, *(u32 *)resource) + 0xC0;

    VU0_LOAD_MATRIX(matrix);
    __asm__ volatile(
        ".set noreorder\n\t"
        "lqc2 vf10, 0(%0)\n\t"
        "vmulax.xyzw ACC, vf28, vf10x\n\t"
        "vmadday.xyzw ACC, vf29, vf10y\n\t"
        "vmaddaz.xyzw ACC, vf30, vf10z\n\t"
        "vmaddw.xyzw vf10, vf31, vf0w\n\t"
        ".set reorder"
        : : "r"((u8 *)resource + 0x10));
}

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
        sdfSetLookAtBasisFromRecord(param, resource);
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

void func_00332E50(u32 mode) {
    D_00438A3C = mode;
}

void *sdfDevCreateBufferedRequest(s32, s32, s32);
SdfResourceList *func_00332E58(s32 capacity) {
    return sdfDevCreateBufferedRequest(capacity, 4, 4);
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
            sdfTexReleaseReferenceViaHandler(list->items[i]);
        }
    }
    sdfDestroyDevRequest(list);
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332F08);

void sdfReduceResourceListCount(s32 listAddress, s32 newCount, s32 applyReduction) {
    s32 countCursor;

    if ((newCount < ((SdfResourceList *)listAddress)->count) && (applyReduction != 0)) {
        countCursor = (s32)newCount;
        do {
            countCursor = countCursor + 1;
        } while ((s64)countCursor != (s64)((SdfResourceList *)listAddress)->count);
        ((SdfResourceList *)listAddress)->count = (s16)newCount;
    }
    func_003405D8();
}

extern u32 func_0032B6B0(u32);
SdfResourceList *sdfResourceListClone(SdfResourceList *src) {
    s32 count;
    SdfResourceList *dst;
    s32 i;

    if (src == NULL) {
        return NULL;
    }
    count = src->count;
    dst = func_00332E58(count);
    for (i = 0; i < count; i++) {
        dst->items[i] = func_0032B6B0(src->items[i]);
    }
    dst->count = count;
    return dst;
}

void func_00333060(SdfResourceList *list, s32 arg, f32 value) {
    s32 i;
    s32 count;

    if (list == NULL) {
        return;
    }
    count = list->count;
    for (i = 0; i < count; i++) {
        SdfAsset *item = (SdfAsset *)list->items[i];

        if (*((u8 *)item + 0x18) != 0) {
            func_0032C278((u32)item, arg, value);
        }
    }
}

void sdfRegisterResourceQueueCallbacks(void) {
    sdfInitializeSynchronizedRequest(&D_00439170, sdfResourceListReleaseAssets);
    sdfInitializeSynchronizedRequest(&D_00439178, sdfAssetRelease);
}

SdfResourceList *sdfCreateResourceList(s32 capacity) {
    return sdfDevCreateBufferedRequest(capacity, 4, 8);
}

void sdfResourceListReleaseAssets(SdfResourceList *list) {
    s32 i;

    for (i = 0; i < list->count; i++) {
        sdfAssetRelease((SdfAsset *)list->items[i]);
    }
    sdfDestroyDevRequest(list);
}

void sdfReleaseQueuedResource(void *resource, s32 retained) {
    if (resource == NULL) {
        return;
    }
    if (retained != 0) {
        sdfPendingQueuePush(&D_00439170, (s32)resource);
    } else {
        sdfDestroyDevRequest(resource);
    }
}

void func_003331F0(void) {
    sdfDevBufferedRequestGrow();
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

void func_00333378(SdfTextParam *param, f32 a, f32 b, f32 c, f32 d, f32 e) {
    f32 *values = sdfEnsurePrimaryTextSubParam(param)->scalar.values;
    values[0] = a;
    values[1] = b;
    values[2] = c;
    values[3] = d;
    values[4] = e;
    param->dirtyFlags |= 0xc;
}

void func_003333F8(SdfTextParam *param, const f32 *input) {
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

void func_003334E0(SdfTextParam *param, f32 a, f32 b, f32 c, f32 d, f32 e) {
    f32 *values = sdfEnsureSecondaryTextSubParam(param)->scalar.values;
    values[0] = a;
    values[1] = b;
    values[2] = c;
    values[3] = d;
    values[4] = e;
    param->dirtyFlags |= 0x30;
}

void func_00333560(SdfTextParam *param, const f32 *input) {
    f32 *values = sdfEnsureSecondaryTextSubParam(param)->scalar.values;
    values[0] = input[0];
    values[1] = input[1];
    values[2] = input[2];
    values[3] = input[3];
    values[4] = input[4];
    param->dirtyFlags |= 0x30;
}

void func_003335C8(SdfTextParam *param, f32 first, f32 second) {
    param->unk40 = first;
    param->unk44 = second;
    param->dirtyFlags = param->dirtyFlags | 0xC0;
}

extern void *func_00328E18(s32);
SdfAsset *func_003335E0(void) {
    SdfAsset *asset;
    u32 *entry;
    s32 i;

    D_00438A38++;
    asset = func_00328E18(0x48);
    asset->pad00[6] = 0xFF;
    for (i = 0; i != 2; i++) {
        entry = func_00328D68(0xA0);
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

u8 *func_003336E0(SdfTextParam *param, SdfTextParam *lookup, u8 *data) {
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
        func_00333378(param, ((f32 *)cursor)[0], ((f32 *)cursor)[1], ((f32 *)cursor)[2], ((f32 *)cursor)[3], ((f32 *)cursor)[4]);
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
        func_003334E0(param, ((f32 *)cursor)[0], ((f32 *)cursor)[1], ((f32 *)cursor)[2], ((f32 *)cursor)[3], ((f32 *)cursor)[4]);
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
        func_003335C8(param, ((f32 *)cursor)[0], ((f32 *)cursor)[1]);
        cursor += 8;
    }
    return cursor;
}

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

void sdfQueueAssetRelease(s32 assetId) {
    s32 id = assetId;

    if (id != 0) {
        sdfPendingQueuePush(&D_00439178, id);
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

void func_00333B18(s32 assetAddress, s32 entryAddress) {
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
void func_00333B38(SdfAsset *asset, void *entryArg) {
    u8 *entry = entryArg;
    SdfTex *tex = *(SdfTex **)((u8 *)asset + 0x30);
    u32 mode;

    ((SdfDrawPacket *)entry)->color = asset->unk18;
    mode = *(u32 *)((u8 *)asset + 0x34);
    ((SdfDrawPacket *)entry)->mode = mode;
    ((SdfDrawPacket *)entry)->paletteValue = D_0040B348[mode];
    if (tex != NULL) {
        ((SdfDrawPacket *)entry)->textureWords[0] = func_0032B328(tex);
        ((SdfDrawPacket *)entry)->textureWords[1] = func_0032B318(tex);
        ((SdfDrawPacket *)entry)->textureWords[2] = func_0032B338(tex);
    }
    func_00333A30(entry + 0x80, asset->fourth);
}

void sdfAssetCopyPairToTextParam(SdfTextParam *asset, SdfTextParam *param) {
    param->unk28 = asset->unk40;
    param->unk2C = asset->unk44;
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
        sdfAssetCopyPairToTextParam(asset, entry);
    }
    asset->pad00[6] = flags & (0x55 << (index ^ 1));
}

void func_00333CB0(SdfAsset *asset, s32 index) {
    u8 flags = asset->pad00[6];
    void *entry = asset->entries[index];
    if ((flags >> index) & 1) {
        sdfAssetCopyTextureState(asset, entry);
    } else if (D_00438A3C != 0) {
        sdfAssetCopyTextureState(asset, entry);
    }
    if (flags & (4 << index)) {
        func_00333B18(asset, entry);
    }
    if (flags & (16 << index)) {
        func_00333B38(asset, entry);
    }
    if (flags & (64 << index)) {
        sdfAssetCopyPairToTextParam(asset, entry);
    }
    asset->pad00[6] = flags & (0x55 << (index ^ 1));
}

SdfAsset *func_003335E0(void);
u8 *func_003336E0(SdfTextParam *, SdfTextParam *, u8 *);
void func_00333208(SdfResourceList *, SdfAsset *);
SdfResourceList *sdfAssetListParse(SdfTextParam *param, u32 *data) {
    u32 count = *data;
    u8 *cursor = (u8 *)(data + 1);
    SdfResourceList *list = sdfCreateResourceList(count >= 0x20 ? count : 0x20);
    while (count != 0) {
        SdfAsset *asset = func_003335E0();
        cursor = func_003336E0((SdfTextParam *)asset, param, cursor);
        func_00333208(list, asset);
        count--;
    }
    return list;
}

INCLUDE_ASM(const s32, "game/code_003325F8", sdfResourceListApplyEntryChanges);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333E98);

typedef struct SdfSubParamWords {
    u32 word[6];
} SdfSubParamWords;

void func_00333EF8(SdfAsset *dst, SdfAsset *src) {
    SdfSubParamWords *sub;

    dst->pad00[6] = 0xFF;
    dst->unk10 = src->unk10;
    dst->unk14 = src->unk14;
    dst->unk1C = src->unk1C;
    dst->unk18 = src->unk18;
    *(u16 *)&dst->pad00[4] = *(u16 *)&src->pad00[4];
    dst->unk2C = src->unk2C;
    dst->unk20 = src->unk20;
    dst->unk28 = src->unk28;
    sub = src->third;
    if (sub != NULL) {
        *(SdfSubParamWords *)sdfEnsurePrimaryTextSubParam((SdfTextParam *)dst) = *sub;
    }
    sub = src->fourth;
    if (sub != NULL) {
        *(SdfSubParamWords *)sdfEnsureSecondaryTextSubParam((SdfTextParam *)dst) = *sub;
    }
    dst->unk40 = src->unk40;
    dst->unk44 = src->unk44;
}

void func_00333EF8(SdfAsset *, SdfAsset *);
void func_00334008(SdfResourceList *dst, SdfResourceList *src) {
    s32 count = dst->count;
    u32 *srcItems = src->items;
    u32 *dstItems = dst->items;
    s32 i;

    for (i = 0; i < count; i++) {
        func_00333EF8((SdfAsset *)dstItems[i], (SdfAsset *)srcItems[i]);
    }
}

extern s32 (*D_0040B358[])(u32, u32);
s32 func_00334078(u32 context, u32 command) {
    D_0040B358[command >> 16](context, command);
}

INCLUDE_SDATA(const s32, "game/code_003325F8", D_00438A38);

INCLUDE_SDATA(const s32, "game/code_003325F8", D_00438A3C);

