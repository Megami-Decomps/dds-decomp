#include "common.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

typedef struct {
    f32 position[3];
    u8 pad0C[4];
    u32 value;
} EffResourceEntry;

/* Nested resource group released when its effect work is destroyed. */
typedef struct {
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
    u32 resource6C;
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

extern void *func_002CFEB8(s32 size);
extern u32 effRetainResource(s32 kind);
extern void billSetBillboardMode(u32 handle, s32 mode);
extern void effReleaseOptionalResource(s32 work);

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

extern EffResourceRenderState D_003D65E0;
extern u32 sdfAllocGeneralBlock(u32 size);
extern void *sdfResourceRetainAddress(u32 handle);
extern u32 sdfCreateAssetWithDrawEntries(void);
extern void func_002DA420(u32 resource, f32 scale);
extern void *memset(void *, s32, u32);
extern void *sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(void *);
extern void sdfConsAppendAssetPacket(void *, u32, s32);
extern void sdfConsAppendVuPacket(void *, s32);
extern void sdfComposeVuMatrixFromRegisters(void);
extern void sdfAppendPacket(void *, void *);
extern void *func_0015FE20(EffResourceRenderState *);
extern f32 D_00354C10[][4];
extern u32 D_00354CD0[];

typedef struct EffResourceDrawSurface {
    u8 pad00[0x10];
    void (*submit)(struct EffResourceDrawSurface *, void *);
} EffResourceDrawSurface;
extern EffResourceDrawSurface *D_00354D00[];

void effSetResourceNormalStream(EffResourceWork *work, f32 (*normals)[4]) {
    work->normals = normals;
}

/* Allocate the resource group and initialize its matrix, scale and entry colors. */
EffResourceWork *effCreateResourceEntryWork(u32 count)
{
    u32 allocation = sdfAllocGeneralBlock(count * sizeof(EffResourceEntry) + sizeof(EffResourceWork));
    EffResourceWork *work = sdfResourceRetainAddress(allocation);
    EffResourceEntry *entry;
    u32 i;

    work->mode = 2;
    work->entries = (EffResourceEntry *)(work + 1);
    work->resource70 = allocation;
    work->entryCount = count;
    work->scale[0] = 1.0f;
    work->scale[1] = 1.0f;
    work->scale[2] = 1.0f;
    work->resource68 = 0;
    EE_MMI_UNIT_MATRIX(work->matrix);
    work->resource6C = sdfCreateAssetWithDrawEntries();
    func_002DA420(work->resource6C, 1.0f);
    entry = work->entries;
    for (i = 0; i < count; i++, entry++) {
        entry->value = 0x80808080;
    }
    memset(&D_003D65E0, 0, sizeof(D_003D65E0));
    D_003D65E0.flags = 0x4000;
    return work;
}

void effReleaseAttachedResources(u32 work) {
    sdfQueueAssetRelease(((EffResourceWork *)work)->resource6C);
    effReleaseOptionalResource(work);
    sdfReleaseResourceAllocation(((EffResourceWork *)work)->resource70);
}

/* Emit scaled, translated triangle batches with separate vector and packed-color streams. */
void effDrawInstancedResourceTrianglesVU(EffResourceWork *work) {
    f32 matrix[16] __attribute__((aligned(16)));
    void *packet;
    EffResourceEntry *entry;
    u32 i;
    u32 remaining;
    u32 triangleSize;

    packet = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(packet);
    sdfConsAppendAssetPacket(packet, work->resource6C, 0);
    EE_MMI_UNIT_MATRIX(matrix);
    matrix[0] = work->scale[0];
    matrix[5] = work->scale[1];
    matrix[10] = work->scale[2];
    VU0_LOAD_MATRIX(matrix);
    VU0_LOAD_MATRIX_B(work->matrix);
    sdfComposeVuMatrixFromRegisters();
    VU0_STORE_MATRIX(matrix);
    D_003D65E0.parameters = NULL;
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
            D_003D65E0.colors = D_00354CD0;
            D_003D65E0.positions = D_00354C10;
            D_003D65E0.normals = NULL;
        } else {
            D_003D65E0.positions = work->positions;
            D_003D65E0.colors = work->colors;
            D_003D65E0.normals = work->normals;
            remaining = work->vertexCount;
        }
        D_003D65E0.color = entry->value;
        D_003D65E0.parameterCount = 16;
        D_003D65E0.vertexCount = 48;
        while (remaining >= 48) {
            sdfAppendPacket(packet, func_0015FE20(&D_003D65E0));
            if (work->resource68 == 0) {
                D_003D65E0.colors += 48;
                D_003D65E0.positions += 48;
            } else {
                D_003D65E0.colors += 48;
                D_003D65E0.positions += 48;
                D_003D65E0.normals += 48;
            }
            remaining -= 48;
        }
        if (remaining >= 3) {
            D_003D65E0.parameterCount = remaining / triangleSize;
            D_003D65E0.vertexCount = remaining;
            sdfAppendPacket(packet, func_0015FE20(&D_003D65E0));
        }
    }
    D_00354D00[work->mode]->submit(D_00354D00[work->mode], packet);
}

INCLUDE_ASM(const s32, "game/code_00176A28", func_00176E20);

void effReleaseOptionalResource(s32 work) {
    if (((EffResourceWork *)work)->resource68 != 0) {
        sdfReleaseResourceAllocation(((EffResourceWork *)work)->resource68);
        return;
    }
}

void effSetResourceEntryPosition(u8 *obj, s32 index, f32 *vec) {
    f32 *dst = (f32 *)(index * 0x14 + *(s32 *)(obj + 0x40));
    dst[0] = vec[0];
    dst[1] = vec[1];
    dst[2] = vec[2];
}

void effGetResourceEntryPosition(u8 *obj, s32 index, f32 *vec) {
    f32 *src = (f32 *)(index * 0x14 + *(s32 *)(obj + 0x40));
    vec[0] = src[0];
    vec[1] = src[1];
    vec[2] = src[2];
}

void effSetResourceEntryValue(s32 work, s32 index, u32 value) {
    ((EffResourceWork *)work)->entries[index].value = value;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void func_001770F8(void *dst, void *src) {
    VU0_COPY_MATRIX(dst, src);
}

/* Create the shared billboard set with its default colour and scale. */
EffBillboardWork *effCreateBillboardResourceWork(EffBillboardParams *params) {
    EffBillboardWork *billboard = (EffBillboardWork *)func_002CFEB8(0x20);

    billboard->mode = params->mode;
    billboard->handle = effRetainResource(2);
    billSetBillboardMode(billboard->handle, 2);
    billboard->color = 0x80808080;
    billboard->scale = 100.0f;
    return billboard;
}
