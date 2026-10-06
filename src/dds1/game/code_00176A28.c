#include "common.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "eff.h"


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

extern void *sdfAllocSizeClassBlock(s32 size);
extern u32 effRetainResource(s32 kind);
extern void billSetBillboardMode(u32 handle, s32 mode);

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
extern s32 sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(SdfListHead *);
extern void sdfConsAppendAssetPacket(s32, void *, s32 (*)(s32));
extern void sdfConsAppendVuPacket(s32, s32 (*)(s32));
extern void sdfComposeVuMatrixFromRegisters(void);
extern void sdfAppendPacket(SdfListHead *, u32);
extern s32 func_0015FE20(EffResourceRenderState *);
extern f32 D_00354C10[][4];
extern u32 D_00354CD0[];

extern SdfPoolNode *D_00354D00[];

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
    work->graphics6C = sdfCreateAssetWithDrawEntries();
    func_002DA420(work->graphics6C, 1.0f);
    entry = work->entries;
    for (i = 0; i < count; i++, entry++) {
        entry->value = 0x80808080;
    }
    memset(&D_003D65E0, 0, sizeof(D_003D65E0));
    D_003D65E0.flags = 0x4000;
    return work;
}

void effReleaseAttachedResources(EffResourceWork *work) {
    sdfQueueAssetRelease(work->graphics6C);
    effReleaseOptionalResource(work);
    sdfReleaseResourceAllocation(work->resource70);
}

/* Emit scaled, translated triangle batches with separate vector and packed-color streams. */
void effDrawInstancedResourceTrianglesVU(EffResourceWork *work) {
    f32 matrix[16] __attribute__((aligned(16)));
    SdfListHead *packet;
    EffResourceEntry *entry;
    u32 i;
    u32 remaining;
    u32 triangleSize;

    packet = (SdfListHead *)sdfAllocPacketAligned(0x20);
    sdfInitPacketList(packet);
    sdfConsAppendAssetPacket((s32)packet, (void *)work->graphics6C, 0);
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
        sdfConsAppendVuPacket((s32)packet, 0);
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
    D_00354D00[work->mode]->append((SdfListHead *)D_00354D00[work->mode], packet);
}

extern f32 sdfEvaluateCosineViaSinePhaseShift(f32 angle);
extern f32 sdfSinPoly(f32 angle);

void effBuildRadialFanStreams(EffResourceWork *work, u32 count, u32 centerColor, u32 outerColor,
                   f32 radiusScale, f32 height) {
    u32 oldResource = work->resource68;
    u32 recordCount;
    u32 allocation;
    f32 (*positions)[4];
    f32 (*unitVectors)[4];
    u32 *colors;
    u32 i;
    f32 angle;
    f32 step;
    f32 halfStep;
    f32 middleAngle;
    f32 negativeHeight;

    if (count < 3) {
        count = 3;
    }
    if (oldResource != 0) {
        effReleaseOptionalResource(work);
    }
    angle = 0.0f;
    recordCount = count * 3;
    allocation = sdfAllocGeneralBlock(recordCount * 0x24);
    work->resource68 = allocation;
    positions = sdfResourceRetainAddress(allocation);
    unitVectors = positions + recordCount;
    colors = (u32 *)(unitVectors + recordCount);
    work->vertexCount = recordCount;
    work->positions = positions;
    work->normals = unitVectors;
    work->colors = colors;
    step = 6.2831852f / (f32)count;
    for (i = 0; i < count; i++) {
        f32 sine;

        negativeHeight = -height;
        unitVectors[0][0] = sdfEvaluateCosineViaSinePhaseShift(angle);
        unitVectors[0][1] = sdfSinPoly(angle);
        unitVectors[0][2] = 0.0f;
        halfStep = step * 0.5f;
        middleAngle = angle + halfStep;
        angle += step;
        positions[0][0] = unitVectors[0][0] * radiusScale;
        sine = unitVectors[0][1];
        unitVectors++;
        positions[0][2] = negativeHeight;
        positions[0][1] = sine * radiusScale;
        positions++;
        unitVectors[0][0] =
            sdfEvaluateCosineViaSinePhaseShift(middleAngle);
        unitVectors[0][1] = sdfSinPoly(middleAngle);
        unitVectors[0][2] = 0.0f;
        positions[0][0] = 0.0f;
        positions[0][1] = 0.0f;
        positions[0][2] = 0.0f;
        unitVectors++;
        positions++;
        unitVectors[0][0] = sdfEvaluateCosineViaSinePhaseShift(angle);
        unitVectors[0][1] = sdfSinPoly(angle);
        unitVectors[0][2] = 0.0f;
        positions[0][0] = unitVectors[0][0] * radiusScale;
        positions[0][1] = unitVectors[0][1] * radiusScale;
        positions[0][2] = negativeHeight;
        colors[0] = outerColor;
        colors[1] = centerColor;
        colors[2] = outerColor;
        colors += 3;
        positions++;
        unitVectors++;
    }
}

void effReleaseOptionalResource(EffResourceWork *work) {
    if (work->resource68 != 0) {
        sdfReleaseResourceAllocation(work->resource68);
        return;
    }
}

void effSetResourceEntryPosition(EffResourceWork *work, s32 index, f32 *vec) {
    f32 *dst = (f32 *)(index * sizeof(EffResourceEntry) + (s32)work->entries);
    dst[0] = vec[0];
    dst[1] = vec[1];
    dst[2] = vec[2];
}

void effGetResourceEntryPosition(EffResourceWork *work, s32 index, f32 *vec) {
    f32 *src = (f32 *)(index * sizeof(EffResourceEntry) + (s32)work->entries);
    vec[0] = src[0];
    vec[1] = src[1];
    vec[2] = src[2];
}

void effSetResourceEntryValue(EffResourceWork *work, s32 index, u32 value) {
    work->entries[index].value = value;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void func_001770F8(void *dst, void *src) {
    VU0_COPY_MATRIX(dst, src);
}

/* Create the shared billboard set with its default colour and scale. */
EffBillboardWork *effCreateBillboardResourceWork(EffBillboardParams *params) {
    EffBillboardWork *billboard = (EffBillboardWork *)sdfAllocSizeClassBlock(0x20);

    billboard->mode = params->mode;
    billboard->handle = effRetainResource(2);
    billSetBillboardMode(billboard->handle, 2);
    billboard->color = 0x80808080;
    billboard->scale = 100.0f;
    return billboard;
}
