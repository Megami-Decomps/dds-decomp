#include "common.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

typedef struct EffResourceEntry {
    f32 position[3];
    u8 pad0C[4];
    u32 value;
} EffResourceEntry;

typedef struct EffResourceWork {
    f32 matrix[16];
    EffResourceEntry *entries;
    s32 entryCount;
    s32 mode;
    f32 scale[3];
    u32 vertexCount;
    f32 (*positions)[4];
    f32 (*normals)[4];
    u32 *colors;
    u32 resource68;
    u32 graphics6C;
    u32 resource70;
} EffResourceWork;

/* Billboard set allocated by effCreateBillboardResourceWork. */
typedef struct EffBillboardWork {
    u8 pad00[0x10];
    u32 mode;   /* 0x10 */
    u32 color;  /* 0x14 */
    f32 scale;  /* 0x18 */
    u32 handle; /* 0x1C */
} EffBillboardWork;

/* Creation parameters read by effCreateBillboardResourceWork. */
typedef struct EffBillboardParams {
    s32 mode; /* 0x00: billboard mode for the new set */
} EffBillboardParams;

extern void *func_00328D68(s32 size);
extern u32 effRetainResource(s32 kind);
extern void billSetBillboardMode(u32 handle, s32 mode);
extern s32 sdfAllocGeneralBlock(s32 size);
extern s32 sdfResourceRetainAddress(s32 handle);
extern void *sdfCreateAssetWithDrawEntries(void);
extern void func_003332D0(u32 asset, f32 value);
typedef struct {
    u16 parameterCount;
    u16 vertexCount;
    u16 flags;
    u8 pad06[2];
    u32 color;
    void *parameters;
    f32 (*positions)[4];
    f32 (*normals)[4];
    u8 pad18[8];
    u32 *colors;
    u8 pad24[8];
} EffResourceRenderState;
extern EffResourceRenderState D_00452080;

void func_0017E680(EffResourceWork *work, f32 (*normals)[4]) {
    work->normals = normals;
}
extern void *sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(void *);
extern void sdfConsAppendAssetPacket(void *, u32, s32);
extern void sdfConsAppendVuPacket(void *, s32);
extern void sdfComposeVuMatrixFromRegisters(void);
extern void sdfAppendPacket(void *, void *);
extern void *func_00167A10(EffResourceRenderState *);
extern f32 D_003B1540[][4];
extern u32 D_003B1600[];

typedef struct EffResourceDrawSurface {
    u8 pad00[0x10];
    void (*submit)(struct EffResourceDrawSurface *, void *);
} EffResourceDrawSurface;
extern EffResourceDrawSurface *D_003B1630[];

/* Create an effect resource work with index entries, its entry list inline at +0x74. */
EffResourceWork *effCreateResourceEntryWork(s32 index) {
    s32 handle;
    EffResourceWork *work;
    EffResourceEntry *entries;
    u32 asset;
    u32 i;

    handle = sdfAllocGeneralBlock(index * 20 + 0x74);
    work = (EffResourceWork *)sdfResourceRetainAddress(handle);
    work->mode = 2;
    work->entries = (EffResourceEntry *)(work + 1);
    work->resource70 = handle;
    work->entryCount = index;
    work->scale[0] = 1.0f;
    work->scale[1] = 1.0f;
    work->scale[2] = 1.0f;
    work->resource68 = 0;
    EE_MMI_UNIT_MATRIX(work);
    asset = (u32)sdfCreateAssetWithDrawEntries();
    work->graphics6C = asset;
    func_003332D0(asset, 1.0f);
    entries = work->entries;
    i = 0;
    if (index != 0) {
        do {
            i++;
            entries->value = 0x80808080;
            entries++;
        } while (i < index);
    }
    memset(&D_00452080, 0, sizeof(D_00452080));
    D_00452080.flags = 0x4000;
    return work;
}

void effReleaseAttachedResources(u32 address) {
    EffResourceWork *effect = (EffResourceWork *)address;
    sdfQueueAssetRelease(effect->graphics6C);
    effReleaseOptionalResource(address);
    sdfReleaseResourceAllocation(effect->resource70);
}

/* Emit scaled, translated triangle batches with separate vector and packed-color streams. */
void func_0017E7E0(EffResourceWork *work) {
    f32 matrix[16] __attribute__((aligned(16)));
    void *packet;
    EffResourceEntry *entry;
    u32 i;
    u32 remaining;
    u32 triangleSize;

    packet = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(packet);
    sdfConsAppendAssetPacket(packet, work->graphics6C, 0);
    EE_MMI_UNIT_MATRIX(matrix);
    matrix[0] = work->scale[0];
    matrix[5] = work->scale[1];
    matrix[10] = work->scale[2];
    VU0_LOAD_MATRIX(matrix);
    VU0_LOAD_MATRIX_B(work->matrix);
    sdfComposeVuMatrixFromRegisters();
    VU0_STORE_MATRIX(matrix);
    D_00452080.parameters = NULL;
    entry = work->entries;
    for (i = 0; i < work->entryCount; i++, entry++) {
        triangleSize = 3;
        matrix[12] = entry->position[0];
        matrix[13] = entry->position[1];
        matrix[14] = entry->position[2];
        VU0_LOAD_MATRIX(matrix);
        sdfConsAppendVuPacket(packet, 0);
        if (work->resource68 == 0) {
            remaining = 12;
            D_00452080.colors = D_003B1600;
            D_00452080.positions = D_003B1540;
            D_00452080.normals = NULL;
        } else {
            D_00452080.positions = work->positions;
            D_00452080.colors = work->colors;
            D_00452080.normals = work->normals;
            remaining = work->vertexCount;
        }
        D_00452080.color = entry->value;
        D_00452080.parameterCount = 16;
        D_00452080.vertexCount = 48;
        while (remaining >= 48) {
            sdfAppendPacket(packet, func_00167A10(&D_00452080));
            if (work->resource68 == 0) {
                D_00452080.colors += 48;
                D_00452080.positions += 48;
            } else {
                D_00452080.colors += 48;
                D_00452080.positions += 48;
                D_00452080.normals += 48;
            }
            remaining -= 48;
        }
        if (remaining >= 3) {
            D_00452080.parameterCount = remaining / triangleSize;
            D_00452080.vertexCount = remaining;
            sdfAppendPacket(packet, func_00167A10(&D_00452080));
        }
    }
    D_003B1630[work->mode]->submit(D_003B1630[work->mode], packet);
}

INCLUDE_ASM(const s32, "game/code_0017E680", func_0017EA78);

void effReleaseOptionalResource(s32 address) {
    EffResourceWork *effect = (EffResourceWork *)address;
    if (effect->resource68 != 0) {
        sdfReleaseResourceAllocation(effect->resource68);
        return;
    }
}

void effSetResourceEntryPosition(EffResourceWork *effect, s32 index, f32 *vec) {
    f32 *dst = (f32 *)(index * 0x14 + (s32)effect->entries);
    dst[0] = vec[0];
    dst[1] = vec[1];
    dst[2] = vec[2];
}

void effGetResourceEntryPosition(EffResourceWork *effect, s32 index, f32 *vec) {
    f32 *src = (f32 *)(index * 0x14 + (s32)effect->entries);
    vec[0] = src[0];
    vec[1] = src[1];
    vec[2] = src[2];
}

void effSetResourceEntryValue(EffResourceWork *effect, s32 index, u32 value) {
    effect->entries[index].value = value;
}

/* vu0 routine: copy a 0x40-byte matrix from src to dst through vf28-vf31 */
void func_0017ED50(void *dst, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX(dst);
}

/* Create the shared billboard set with its default colour and scale. */
EffBillboardWork *effCreateBillboardResourceWork(EffBillboardParams *params) {
    EffBillboardWork *billboard = (EffBillboardWork *)func_00328D68(0x20);

    billboard->mode = params->mode;
    billboard->handle = effRetainResource(2);
    billSetBillboardMode(billboard->handle, 2);
    billboard->color = 0x80808080;
    billboard->scale = 100.0f;
    return billboard;
}
