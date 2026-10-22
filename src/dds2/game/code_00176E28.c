#include "sdk_ps2.h"
#include "eff_packet_params.h"
#include "common.h"
#include "eff_ring_color_slot.h"
#include "eff_random.h"
#include "sdf_trig.h"
#include "sdf_asset_state.h"
#include "sdf_asset_create.h"
#include "sdf_packet_list.h"
#include "sdf_packet_append.h"
#include "sdf_resource.h"
#include "eff.h"
#include "eff_pcp_ring_block.h"
#include "eff_math.h"
#include "eff_param.h"
#include "eff_pcp_scatter_radial.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

#define EFF_RING_PARAM_BYTES 0x58
#define EFF_RING_HEADER_BYTES 0x80
#define EFF_RING_VERTEX_BYTES 16
#define EFF_POOL_HEADER_BYTES 0x70
#define EFF_PACKET_PARAMS_BYTES 0x2C
#define EFF_PACKET_LIST_BYTES 0x20
#define EFF_STATE_PACKET_BYTES 0x30
#define EFF_MATRIX_WORD_COUNT 16
#define EFF_FAN_VERTEX_COUNT 5
#define EFF_FAN_BATCH_VERTICES 0x19
#define EFF_FAN_BATCH_PARAMS 0xF
#define EFF_TRIANGLE_VERTEX_COUNT 3
#define EFF_TRIANGLE_BATCH_VERTICES 0x30
#define EFF_QUAD_VERTEX_COUNT 4
#define EFF_QUAD_BATCH_VERTICES 0x20
#define EFF_TRIANGLE_POSITION_BYTES 0x30
#define EFF_TRIANGLE_COLOR_BYTES 0xc
#define EFF_QUAD_POSITION_BYTES 0x40
#define EFF_QUAD_COLOR_BYTES 0x10
#define EFF_FAN_POSITION_BYTES 0x50
#define EFF_FAN_COLOR_BYTES 0x14
#define EFF_NEUTRAL_COLOR 0x80808080
#define EFF_RGB_MASK 0xFFFFFF
#define EFF_DIRECT_SURFACE_COUNT 4

extern EffPacketParams effRecordPoolDrawParams[];
extern u32 effFanPoolTriangleIndices[];
extern SdfPoolNode *effScaledFanDrawSurfaces[];
extern SdfPoolNode *effTransformedFanDrawSurfaces[];
extern SdfPoolNode *effTrianglePoolDrawSurfaces[];
extern SdfPoolNode *effQuadPoolDrawSurfaces[];
extern u32 effQuadPoolTriangleIndices[];
extern SdfPoolNode D_00380248;
#include "sdf_packet_alloc.h"
extern void *effPkt3DPrimMakePoly(EffPacketParams *);
extern dds3FlashPolyList_t *dds3FlashPolyListCreate(u32 count);
extern void dds3FlashPolyListHDelete(dds3FlashPolyList_t *pool);
extern u32 *dds3FlashPolyListHGetVC(dds3FlashPolyList_t *pool, s32 index);



extern dds3FlashPolyList_t *dds3FlashPolyListHCreate(u32 count);


extern EffRandState effDefaultRandomState[];
extern f32 *dds3FlashPolyListHGetVertex(dds3FlashPolyList_t *pool, s32 index);
extern f32 effPCPFlashPolyRingExpHVertex[];

/* Allocate a circular fan group. Zero spread becomes one; count is not guarded. */
extern u32 effMagaCylinderCalcColor(u32 colorA, u32 colorB);


EffRingWork *effPCPFlashPolyRingExpHCreate(source)
EffRingWork *source;

{
    sdfMemHandle_t *handle;
    EffPcpRingBlock *block;
    EffRingWork *ring;
    f32 angle;
    f32 step;
    u32 spread;
    u32 i;

    handle = sdfMemAllocDirect(source->count * EFF_RING_VERTEX_BYTES + EFF_RING_HEADER_BYTES);
    block = (EffPcpRingBlock *)sdfMemLock(handle);
    ring = &block->header;
    memcpy(ring, source, EFF_RING_PARAM_BYTES);
    ring->vertices = block->vertices;
    ring->allocationHandle = handle;
    ring->color = EFF_NEUTRAL_COLOR;
    ring->currentRadius = ring->param38;
    ring->currentSideOffsetB = ring->param30;
    ring->currentSideOffsetA = ring->param34;
    ring->updateCount = 0;
    ring->scale = 1.0f;
    if (ring->spread == 0) {
        ring->spread = 1;
    }
    angle = EFFECT_RING_START_ANGLE;
    ring->recordPool = dds3FlashPolyListHCreate(ring->count);
    ring->recordPool->scale = 1.0f;
    ring->recordPool->drawMode = ring->drawMode;
    step = EFFECT_RING_FULL_TURN / ring->count;
    spread = ring->spread;
    for (i = 0; i < ring->count; i++) {
        ring->vertices[i].age = -(dds3GetRandA((EffRandState *)effDefaultRandomState) % spread);
        ring->vertices[i].angle = angle;
        angle += step;
    }
    return ring;
}

/* Create a ring from the first parameter-table block. */
EffRingWork *effPCPFlashPolyRingExpHCreatePCF(void *table) {
    void *parameterBlock;

    parameterBlock = dds3EffectPCPDataGetData(table, 0);
    return effPCPFlashPolyRingExpHCreate(parameterBlock);
}

/* Kind 85 duplicate: recreate the parameter prefix, not the live vertex state. */
EffRingWork *effPCPFlashPolyRingExpHCopy(EffRingWork *ring) {
    return effPCPFlashPolyRingExpHCreate(ring);
}

/* Release the matrix pool's resources before the ring's backing allocation. */
void effPCPFlashPolyRingExpHDelete(EffRingWork *ring) {
    dds3FlashPolyListHDelete(ring->recordPool);
    sdfMemFreeDirect(ring->allocationHandle);
}

/* Copy one quadword; the fourth component is copied along with XYZ. */
void effPCPFlashPolyRingExpHSetPosition(void *destination, void *source) {
    PCP_COPY_VECTOR(destination, source);
}

/* Replace the ring's packed modulation color without touching vertex colors. */
void effPCPFlashPolyRingExpHSetColor(EffRingWork *ring, u32 color) {
    ring->color = color;
}

/* Set the ring's scale factor. */
void effPCPFlashPolyRingExpHSetScale(EffRingWork *ring, f32 scale) {
    ring->scale = scale;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effPCPFlashPolyRingExpHSetRotateMatrix(EffRingWork *ring, void *matrixSource) {
    VU0_LOAD_MATRIX(matrixSource);
    VU0_STORE_MATRIX(ring->recordPool->matrix);
}

/* Fill five fan-vertex colors; odd and even groups use different alpha values. */
void effPCPFlashPolyRingExpHPartSetVC(EffRingWork *work, s32 index, s32 modulationColor) {
    EffRingColorSlot *slot;
    s32 firstRgb;
    s32 secondRgb;

    slot = (EffRingColorSlot *)dds3FlashPolyListHGetVC(work->recordPool, index);
    firstRgb = work->firstColor & EFF_RGB_MASK;
    secondRgb = work->secondColor & EFF_RGB_MASK;
    slot->colors[0] = effMagaCylinderCalcColor(secondRgb, modulationColor);
    slot->colors[1] = effMagaCylinderCalcColor(secondRgb, modulationColor);
    if (index & 1) {
        slot->colors[2] = effMagaCylinderCalcColor(0x80000000, modulationColor);
        slot->colors[3] = effMagaCylinderCalcColor(firstRgb | 0xFF000000, modulationColor);
        slot->colors[4] = effMagaCylinderCalcColor(0x80000000, modulationColor);
    } else {
        slot->colors[2] = effMagaCylinderCalcColor(0xFF000000, modulationColor);
        slot->colors[3] = effMagaCylinderCalcColor(firstRgb | 0x40000000, modulationColor);
        slot->colors[4] = effMagaCylinderCalcColor(0xFF000000, modulationColor);
    }
}

/* vu0 routine: build five orbiting fan positions from a phase-derived basis.
 * No camera matrix is read. Quadword loads retain the native scratch W values. */
void effPCPFlashPolyRingExpHPartSetVertex(EffRingWork *work, s32 index) {
    EffRingParticle *part = &work->vertices[index];
    f32 *positions = dds3FlashPolyListHGetVertex(work->recordPool, index);
    f32 orbitOffset[4];
    f32 radialDirection[4];
    f32 axisOffset[4];
    f32 sideOffsetA[4];
    f32 sideOffsetB[4];
    f32 sine;
    f32 basisFactor;

    VEC3_SPLAT(axisOffset, work->currentAxisOffset);
    VEC3_SPLAT(sideOffsetA, work->currentSideOffsetA);
    VEC3_SPLAT(sideOffsetB, work->currentSideOffsetB);
    radialDirection[0] = sdfCos(part->angle);
    radialDirection[1] = 0;
    sine = sdfSin(part->angle);
    radialDirection[2] = sine;
    orbitOffset[0] = radialDirection[0] * work->currentRadius;
    orbitOffset[1] = 0;
    orbitOffset[2] = sine * work->currentRadius;
    basisFactor = part->basisFactor;
    effPCPFlashPolyRingExpHVertex[0] = radialDirection[0] * basisFactor;
    effPCPFlashPolyRingExpHVertex[1] = basisFactor + -1.0f;
    effPCPFlashPolyRingExpHVertex[2] = sine * basisFactor;
    VU0_LOAD_VF(vf10, effPCPFlashPolyRingExpHVertex);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf11, vf10);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf10, radialDirection);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, sideOffsetA);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, sideOffsetA);
    VU0_LOAD_VF(vf10, sideOffsetB);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, sideOffsetB);
    VU0_MOVE_VF(vf10, vf12);
    VU0_LOAD_VF(vf11, axisOffset);
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf10, orbitOffset);
    VU0_LOAD_VF(vf11, sideOffsetB);
    VU0_STORE_VF(vf10, positions + 12);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, positions + 8);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, positions + 16);
    VU0_LOAD_VF(vf10, orbitOffset);
    VU0_LOAD_VF(vf11, sideOffsetA);
    VU0_ADD(vf10, vf10, vf12);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, positions);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, positions + 4);
}

/* Add the group's increment to one accumulated scalar; index is unchecked. */
void effPCPFlashPolyRingExpHPartRotate(EffRingWork *work, s32 index) {
    EffRingParticle *part;

    part = &work->vertices[index];
    part->angle = part->angle + work->increment;
}
extern void dds3FlashPolyListHDraw(dds3FlashPolyList_t *);

void effPCPFlashPolyRingExpHDraw(EffRingWork *work)
{
    s32 index;
    s32 count;
    EffRingParticle *part;
    s32 lifetime;
    s32 fadeIn;
    s32 fadeOut;
    u32 randomRange;
    s32 restart;
    s32 tintColor;
    dds3FlashPolyList_t *handle;
    f32 heightDelta;
    f32 delta;
    f32 progress;
    f32 factor;
    f32 start;
    f32 radiusEnd;
    f32 upStart;
    f32 acrossStart;
    f32 upEnd;
    f32 acrossEnd;
    f32 normalEnd;

    part = work->vertices;
    count = work->count;
    lifetime = work->duration;
    fadeIn = work->fadeIn;
    fadeOut = work->fadeOut;
    restart = work->respawn;
    randomRange = work->spread;
    tintColor = work->color;
    heightDelta = work->param4C - work->param48;
    progress = 1.0f;
    if (lifetime > 0) {
        progress = (f32)work->updateCount / (f32)lifetime;
    }
    start = work->param38;
    radiusEnd = work->param3C;
    delta = radiusEnd - start;
    work->currentRadius = start + delta * progress;
    if (start > 0.0f) {
        factor = radiusEnd / start;
    } else {
        factor = radiusEnd;
    }
    upStart = work->param30;
    acrossStart = work->param34;
    upEnd = upStart * factor;
    acrossEnd = acrossStart * factor;
    start = work->param40;
    normalEnd = work->param44;
    delta = upEnd - upStart;
    work->currentSideOffsetB = upStart + delta * progress;
    delta = acrossEnd - acrossStart;
    work->currentSideOffsetA = acrossStart + delta * progress;
    delta = normalEnd - start;
    work->currentAxisOffset = start + delta * progress;

    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;
        s32 color;

        if (age == 0) {
            part->basisFactor = work->param48;
            effPCPFlashPolyRingExpHPartSetVertex(work, index);
            effPCPFlashPolyRingExpHPartSetVC(work, index, 0);
            part->color = 0x80808080;
        } else if (age >= lifetime) {
            if (restart != 0) {
                part->age = ~(dds3GetRandA((EffRandState *)effDefaultRandomState) % randomRange);
            }
            color = 0;
            effPCPFlashPolyRingExpHPartSetVC(work, index, color);
        } else if (age > 0) {
            progress = (f32)age / (f32)lifetime;
            part->basisFactor = heightDelta * progress + work->param48;
            effPCPFlashPolyRingExpHPartRotate(work, index);
            effPCPFlashPolyRingExpHPartSetVertex(work, index);
            if (part->age < fadeIn && fadeIn != 0) {
                factor = (f32)part->age / (f32)fadeIn;
            } else if (fadeOut >= lifetime - part->age && fadeOut != 0) {
                factor = (f32)(lifetime - part->age) / (f32)fadeOut;
            } else {
                factor = 1.0f;
            }
            color = effMagaCylinderCalcColor(effColorBlend(0, part->color, factor), tintColor);
            effPCPFlashPolyRingExpHPartSetVC(work, index, color);
        }
        part->age = part->age + 1;
    }
    if (work->updateCount == lifetime) {
        work->updateCount = 0;
    } else {
        work->updateCount++;
    }
    handle = work->recordPool;
    handle->origin[0] = work->origin[0];
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->scale = work->scale;
    dds3FlashPolyListHDraw(handle);
}


/* Allocate five 16-byte positions and five color words per group, then the 0x70-byte header. */
dds3FlashPolyList_t *dds3FlashPolyListCreate(u32 count) {
    dds3FlashPolyList_t *pool;
    sdfMemHandle_t *handle;
    u8 *block;
    s32 vertexCount;
    s32 recordBytes;
    s32 size;
    s32 quads;

    vertexCount = count * 5;
    recordBytes = vertexCount * 5 * 4;
    quads = recordBytes >> 4;
    recordBytes = ((recordBytes & 0xF) != 0 ? quads + 1 : quads) << 4;
    size = recordBytes + EFF_POOL_HEADER_BYTES;
    handle = sdfMemAllocDirect(size);
    block = (u8 *)sdfMemLock(handle);
    memset(block, 0, size);
    pool = (dds3FlashPolyList_t *)(block + recordBytes);
    pool->recordBase = (u8 *)block;
    pool->auxRecordBase = (u8 *)(block + vertexCount * 16);
    pool->color = EFF_NEUTRAL_COLOR;
    pool->drawMode = 2;
    pool->vertexCount = vertexCount;
    pool->buffer = handle;
    pool->scale = 1.0f;
    pool->resource = sdfMatNew();
    sdfMatSetConstant(pool->resource, 1.0f);
    memset(effRecordPoolDrawParams, 0, EFF_PACKET_PARAMS_BYTES);
    effRecordPoolDrawParams->primitive = 0x4000;
    return pool;
}

/* Queue asset release, then free the pool allocation; neither handle is cleared. */
void dds3FlashPolyListDelete(dds3FlashPolyList_t *pool) {
    sdfMatFreeDelayed(pool->resource);
    sdfMemFreeDirect(pool->buffer);
}

/* Build scale/origin transform and submit five-vertex groups in batches of 25.
 * The remainder path retains its native vertex count, even for a partial group. */
void dds3FlashPolyListDraw(dds3FlashPolyList_t *work)
{
    f32 matrix[EFF_MATRIX_WORD_COUNT] __attribute__((aligned(16)));
    sdfDrawTag_t *list;
    s32 remaining;
    s32 fanCount;
    u64 *packet;
    sdfDrawTag_t *stateList;

    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EFF_PACKET_LIST_BYTES);
    sdfDrawTagInit(list);
    EE_MMI_UNIT_MATRIX(matrix);
    matrix[0] = work->scale;
    matrix[5] = work->scale;
    matrix[10] = work->scale;
    matrix[12] = work->origin[0];
    matrix[13] = work->origin[1];
    matrix[14] = work->origin[2];
    VU0_LOAD_MATRIX(matrix);
    sdf3DPrimLinkTm(list, 0);
    sdf3DPrimLinkMat(list, work->resource, 0);
    remaining = work->vertexCount;
    effRecordPoolDrawParams->colors = (u32 *)work->auxRecordBase;
    effRecordPoolDrawParams->positions = (u128 *)work->recordBase;
    effRecordPoolDrawParams->color = work->color;
    effRecordPoolDrawParams->parameterCount = EFF_FAN_BATCH_PARAMS;
    effRecordPoolDrawParams->vertexCount = EFF_FAN_BATCH_VERTICES;
    effRecordPoolDrawParams->parameters = effFanPoolTriangleIndices;
    while (remaining >= EFF_FAN_BATCH_VERTICES) {
        remaining -= EFF_FAN_BATCH_VERTICES;
        sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(effRecordPoolDrawParams));
        effRecordPoolDrawParams->positions += EFF_FAN_BATCH_VERTICES;
        effRecordPoolDrawParams->colors += EFF_FAN_BATCH_VERTICES;
    }
    if (remaining >= EFF_FAN_VERTEX_COUNT) {
        fanCount = remaining / EFF_FAN_VERTEX_COUNT;
        effRecordPoolDrawParams->vertexCount = remaining;
        effRecordPoolDrawParams->parameterCount = fanCount * 3;
        sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(effRecordPoolDrawParams));
    }
    if (work->drawMode < EFF_DIRECT_SURFACE_COUNT) {
        effScaledFanDrawSurfaces[work->drawMode]->append(effScaledFanDrawSurfaces[work->drawMode], list);
    } else {
        stateList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EFF_PACKET_LIST_BYTES);
        sdfDrawTagInit(stateList);
        packet = (u64 *)sdfGrAllocPacketBuff(EFF_STATE_PACKET_BYTES);
        packet[4] = 6;
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[5] = 0x42;
        sdfDrawTagLink(stateList, (u32)packet);
        D_00380248.append(&D_00380248, stateList);
        packet = (u64 *)sdfGrAllocPacketBuff(EFF_STATE_PACKET_BYTES);
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[4] = 0x42;
        packet[5] = 0x42;
        sdfDrawTagLink(list, (u32)packet);
        D_00380248.append(&D_00380248, list);
    }
}

/* Address the five-position record for a group; no index bounds check. */
f32 *dds3FlashPolyListGetVertex(dds3FlashPolyList_t *pool, s32 groupIndex) {
    return (f32 *)(pool->recordBase + groupIndex * EFF_FAN_POSITION_BYTES);
}

/* Address the group's five packed color words. */
void *dds3FlashPolyListGetVC(dds3FlashPolyList_t *pool, s32 groupIndex) {
    return pool->auxRecordBase + groupIndex * EFF_FAN_COLOR_BYTES;
}

/* Store raw control bits: this view interprets them as a submission mode. */
void dds3FlashPolyListSetBlend(dds3FlashPolyList_t *pool, u32 controlBits) {
    pool->drawMode = controlBits;
}

/* This pool view uses the control word as a packed modulation color. */
void dds3FlashPolyListSetABGR(dds3FlashPolyList_t *pool, u32 color) {
    pool->color = color;
}

/* Set the pool scale without changing the native byte-pointer interface. */
void dds3FlashPolyListSetScale(u8 *work, f32 scale) {
    ((dds3FlashPolyList_t *)work)->scale = scale;
}


/* Allocate three positions and three colors per triangle, then the header.
 * The complete block and shared packet-parameter record are cleared. */
dds3FlashPolyList_t *dds3FlashPolyList2Create(s32 triangleCount) {
    dds3FlashPolyList_t *pool;
    sdfMemHandle_t *handle;
    u32 *block;
    s32 vertexCount;
    s32 positionWordCount;
    s32 colorWordCount;
    u32 size;

    vertexCount = triangleCount * EFF_TRIANGLE_VERTEX_COUNT;
    positionWordCount = vertexCount * 4;
    colorWordCount = vertexCount;
    size = (positionWordCount + colorWordCount) * 4 + EFF_POOL_HEADER_BYTES;
    handle = sdfMemAllocDirect(size);
    block = (u32 *)sdfMemLock(handle);
    memset(block, 0, size);
    pool = (dds3FlashPolyList_t *)(block + (positionWordCount + colorWordCount));
    pool->recordBase = (u8 *)block;
    pool->auxRecordBase = (u8 *)(block + positionWordCount);
    pool->color = EFF_NEUTRAL_COLOR;
    pool->drawMode = 2;
    pool->vertexCount = colorWordCount;
    pool->buffer = handle;
    pool->scale = 1.0f;
    pool->resource = sdfMatNew();
    sdfMatSetConstant(pool->resource, 1.0f);
    memset(effRecordPoolDrawParams, 0, EFF_PACKET_PARAMS_BYTES);
    effRecordPoolDrawParams->primitive = 0x4000;
    return pool;
}

/* Release the asset and backing allocation; the header points into that block. */
void dds3FlashPolyList2Delete(dds3FlashPolyList_t *pool) {
    sdfMatFreeDelayed(pool->resource);
    sdfMemFreeDirect(pool->buffer);
}

/* Submit the position/color pool in triangle batches of at most 48 vertices. */
void dds3FlashPolyList2Draw(dds3FlashPolyList_t *pool)
{
    f32 matrix[EFF_MATRIX_WORD_COUNT];
    sdfDrawTag_t *packet = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EFF_PACKET_LIST_BYTES);
    s32 remainingVertices;
    SdfPoolNode *surface;

    sdfDrawTagInit(packet);
    EE_MMI_UNIT_MATRIX(matrix);
    matrix[0] = pool->scale;
    matrix[5] = pool->scale;
    matrix[10] = pool->scale;
    matrix[12] = pool->origin[0];
    matrix[13] = pool->origin[1];
    matrix[14] = pool->origin[2];
    VU0_LOAD_MATRIX(matrix);
    sdf3DPrimLinkTm(packet, 0);
    sdf3DPrimLinkMat(packet, pool->resource, 0);
    remainingVertices = pool->vertexCount;
    effRecordPoolDrawParams->colors = (u32 *)pool->auxRecordBase;
    effRecordPoolDrawParams->positions = (u128 *)pool->recordBase;
    effRecordPoolDrawParams->color = pool->color;
    effRecordPoolDrawParams->parameterCount = 0x10;
    effRecordPoolDrawParams->vertexCount = EFF_TRIANGLE_BATCH_VERTICES;
    effRecordPoolDrawParams->parameters = NULL;
    while (remainingVertices >= EFF_TRIANGLE_BATCH_VERTICES) {
        remainingVertices -= EFF_TRIANGLE_BATCH_VERTICES;
        sdfDrawTagLink(packet, (u32)effPkt3DPrimMakePoly(effRecordPoolDrawParams));
        effRecordPoolDrawParams->positions += EFF_TRIANGLE_BATCH_VERTICES;
        effRecordPoolDrawParams->colors += EFF_TRIANGLE_BATCH_VERTICES;
    }
    if (remainingVertices >= EFF_TRIANGLE_VERTEX_COUNT) {
        effRecordPoolDrawParams->parameterCount = remainingVertices / EFF_TRIANGLE_VERTEX_COUNT;
        effRecordPoolDrawParams->vertexCount = remainingVertices;
        sdfDrawTagLink(packet, (u32)effPkt3DPrimMakePoly(effRecordPoolDrawParams));
    }
    surface = effTrianglePoolDrawSurfaces[pool->drawMode];
    surface->append(surface, packet);
}

/* Address three quadword positions for one triangle. */
f32 *dds3FlashPolyList2GetVertex(dds3FlashPolyList_t *pool, s32 groupIndex) {
    return (f32 *)(pool->recordBase + groupIndex * EFF_TRIANGLE_POSITION_BYTES);
}

/* Address three packed color words for one triangle. */
void *dds3FlashPolyList2GetVC(dds3FlashPolyList_t *pool, s32 groupIndex) {
    return pool->auxRecordBase + groupIndex * EFF_TRIANGLE_COLOR_BYTES;
}

/* Allocate four positions and four colors per quad, followed by the header.
 * Both allocation and shared packet parameters are cleared before defaults. */
dds3FlashPolyList_t *dds3FlashPolyList3Create(s32 quadCount) {
    dds3FlashPolyList_t *pool;
    sdfMemHandle_t *handle;
    u32 *block;
    s32 positionWordCount;
    s32 colorWordCount;
    u32 size;

    positionWordCount = quadCount * 16;
    colorWordCount = quadCount * EFF_QUAD_VERTEX_COUNT;
    size = (positionWordCount + colorWordCount) * 4 + EFF_POOL_HEADER_BYTES;
    handle = sdfMemAllocDirect(size);
    block = (u32 *)sdfMemLock(handle);
    memset(block, 0, size);
    pool = (dds3FlashPolyList_t *)(block + (positionWordCount + colorWordCount));
    pool->recordBase = (u8 *)block;
    pool->drawMode = 2;
    pool->auxRecordBase = (u8 *)(block + positionWordCount);
    pool->vertexCount = colorWordCount;
    pool->buffer = handle;
    pool->scale = 1.0f;
    pool->color = EFF_NEUTRAL_COLOR;
    pool->resource = sdfMatNew();
    sdfMatSetConstant(pool->resource, 1.0f);
    memset(effRecordPoolDrawParams, 0, EFF_PACKET_PARAMS_BYTES);
    effRecordPoolDrawParams->primitive = 0x4000;
    return pool;
}

/* Queue the asset before releasing the allocation containing this header. */
void dds3FlashPolyList3Delete(dds3FlashPolyList_t *pool) {
    sdfMatFreeDelayed(pool->resource);
    sdfMemFreeDirect(pool->buffer);
}

/* Submit the position/color pool in quad batches of at most 32 vertices. */
void dds3FlashPolyList3Draw(dds3FlashPolyList_t *pool)
{
    f32 matrix[EFF_MATRIX_WORD_COUNT];
    sdfDrawTag_t *packet = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EFF_PACKET_LIST_BYTES);
    s32 remainingVertices;
    SdfPoolNode *surface;

    sdfDrawTagInit(packet);
    EE_MMI_UNIT_MATRIX(matrix);
    matrix[0] = pool->scale;
    matrix[5] = pool->scale;
    matrix[10] = pool->scale;
    matrix[12] = pool->origin[0];
    matrix[13] = pool->origin[1];
    matrix[14] = pool->origin[2];
    VU0_LOAD_MATRIX(matrix);
    sdf3DPrimLinkTm(packet, 0);
    sdf3DPrimLinkMat(packet, pool->resource, 0);
    remainingVertices = pool->vertexCount;
    effRecordPoolDrawParams->colors = (u32 *)pool->auxRecordBase;
    effRecordPoolDrawParams->positions = (u128 *)pool->recordBase;
    effRecordPoolDrawParams->color = pool->color;
    effRecordPoolDrawParams->parameterCount = 0x10;
    effRecordPoolDrawParams->vertexCount = EFF_QUAD_BATCH_VERTICES;
    effRecordPoolDrawParams->parameters = effQuadPoolTriangleIndices;
    while (remainingVertices >= EFF_QUAD_BATCH_VERTICES) {
        remainingVertices -= EFF_QUAD_BATCH_VERTICES;
        sdfDrawTagLink(packet, (u32)effPkt3DPrimMakePoly(effRecordPoolDrawParams));
        effRecordPoolDrawParams->positions += EFF_QUAD_BATCH_VERTICES;
        effRecordPoolDrawParams->colors += EFF_QUAD_BATCH_VERTICES;
    }
    if (remainingVertices >= EFF_QUAD_VERTEX_COUNT) {
        effRecordPoolDrawParams->parameterCount = remainingVertices / EFF_QUAD_VERTEX_COUNT * 2;
        effRecordPoolDrawParams->vertexCount = remainingVertices;
        sdfDrawTagLink(packet, (u32)effPkt3DPrimMakePoly(effRecordPoolDrawParams));
    }
    surface = effQuadPoolDrawSurfaces[pool->drawMode];
    surface->append(surface, packet);
}

/* Address four quadword positions for one quad. */
f32 *dds3FlashPolyList3GetVertex(dds3FlashPolyList_t *pool, s32 groupIndex) {
    return (f32 *)(pool->recordBase + groupIndex * EFF_QUAD_POSITION_BYTES);
}

/* Address four packed color words for one quad. */
void *dds3FlashPolyList3GetVC(dds3FlashPolyList_t *pool, s32 groupIndex) {
    return pool->auxRecordBase + groupIndex * EFF_QUAD_COLOR_BYTES;
}

/* Allocate five-vertex fan groups, then set the returned header's matrix. */
dds3FlashPolyList_t *dds3FlashPolyListHCreate(u32 fanCount) {
    dds3FlashPolyList_t *pool;

    pool = dds3FlashPolyListCreate(fanCount);
    EE_MMI_UNIT_MATRIX(pool->matrix);
    return pool;
}

void dds3FlashPolyListHDelete(dds3FlashPolyList_t *work) {
    dds3FlashPolyListDelete(work);
}

/* Apply the stored matrix/origin, then submit batches of five-vertex groups.
 * Native state-only and drawing lists remain separate for non-direct modes. */
void dds3FlashPolyListHDraw(dds3FlashPolyList_t *work)
{
    f32 matrix[EFF_MATRIX_WORD_COUNT] __attribute__((aligned(16)));
    sdfDrawTag_t *list;
    s32 remaining;
    s32 fanCount;
    u64 *packet;
    sdfDrawTag_t *stateList;

    list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EFF_PACKET_LIST_BYTES);
    sdfDrawTagInit(list);
    VU0_COPY_MATRIX(matrix, work->matrix);
    matrix[12] = work->origin[0];
    matrix[13] = work->origin[1];
    matrix[14] = work->origin[2];
    VU0_LOAD_MATRIX(matrix);
    sdf3DPrimLinkTm(list, 0);
    sdf3DPrimLinkMat(list, work->resource, 0);
    remaining = work->vertexCount;
    effRecordPoolDrawParams->colors = (u32 *)work->auxRecordBase;
    effRecordPoolDrawParams->positions = (u128 *)work->recordBase;
    effRecordPoolDrawParams->color = work->color;
    effRecordPoolDrawParams->parameterCount = EFF_FAN_BATCH_PARAMS;
    effRecordPoolDrawParams->vertexCount = EFF_FAN_BATCH_VERTICES;
    effRecordPoolDrawParams->parameters = effFanPoolTriangleIndices;
    while (remaining >= EFF_FAN_BATCH_VERTICES) {
        remaining -= EFF_FAN_BATCH_VERTICES;
        sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(effRecordPoolDrawParams));
        effRecordPoolDrawParams->positions += EFF_FAN_BATCH_VERTICES;
        effRecordPoolDrawParams->colors += EFF_FAN_BATCH_VERTICES;
    }
    if (remaining >= EFF_FAN_VERTEX_COUNT) {
        fanCount = remaining / EFF_FAN_VERTEX_COUNT;
        effRecordPoolDrawParams->vertexCount = remaining;
        effRecordPoolDrawParams->parameterCount = fanCount * 3;
        sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(effRecordPoolDrawParams));
    }
    if (work->drawMode < EFF_DIRECT_SURFACE_COUNT) {
        effTransformedFanDrawSurfaces[work->drawMode]->append(effTransformedFanDrawSurfaces[work->drawMode], list);
    } else {
        stateList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(EFF_PACKET_LIST_BYTES);
        sdfDrawTagInit(stateList);
        packet = (u64 *)sdfGrAllocPacketBuff(EFF_STATE_PACKET_BYTES);
        packet[4] = 6;
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[5] = 0x42;
        sdfDrawTagLink(stateList, (u32)packet);
        D_00380248.append(&D_00380248, stateList);
        packet = (u64 *)sdfGrAllocPacketBuff(EFF_STATE_PACKET_BYTES);
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[4] = 0x42;
        packet[5] = 0x42;
        sdfDrawTagLink(list, (u32)packet);
        D_00380248.append(&D_00380248, list);
    }
}

/* Address the five-position record for one fan group. */
f32 *dds3FlashPolyListHGetVertex(dds3FlashPolyList_t *pool, s32 groupIndex) {
    return (f32 *)(pool->recordBase + groupIndex * EFF_FAN_POSITION_BYTES);
}

/* Address the five color words for one fan group. */
u32 *dds3FlashPolyListHGetVC(dds3FlashPolyList_t *pool, s32 groupIndex) {
    return (u32 *)(pool->auxRecordBase + groupIndex * EFF_FAN_COLOR_BYTES);
}

/* Select the record pool's packet submission mode. */
void dds3FlashPolyListHSetBlend(dds3FlashPolyList_t *pool, u32 mode) {
    pool->drawMode = mode;
}

/* Set the packed color word passed to the pool's packet builder. */
void dds3FlashPolyListHSetABGR(dds3FlashPolyList_t *pool, u32 color) {
    pool->color = color;
}

/* Set the scalar used by the scale/origin rendering path. */
void dds3FlashPolyListHSetScale(dds3FlashPolyList_t *work, f32 scale) {
    work->scale = scale;
}


extern PcpScatterPool *dds3ScatterPolyListCreate(s32 groups);
extern void dds3ScatterPolyListSetTex(PcpScatterPool *work, u32 resource);
PcpScatterRadialWork *effPCPHamaCardSpiralCreate(params, resource, particleParams)
    const PcpScatterRadialParams *params;
    u32 resource;
    void *particleParams;
{
    PcpScatterRadialWork *work;
    PcpScatterRadialParticle *particle;
    sdfMemHandle_t *handle;
    effPartHandle_t **handles;
    u32 count;
    u32 i;
    s32 delaySpread;
    s32 ageOffset;
    u32 segments;

    handle = sdfMemAllocDirect(sizeof(PcpScatterRadialWork) + params->particleCount * sizeof(PcpScatterRadialParticle));
    work = (PcpScatterRadialWork *)sdfMemLock(handle);
    work->particles = (PcpScatterRadialParticle *)(work + 1);
    work->params = *params;
    work->color = 0x80808080;
    work->allocation = handle;
    work->scale = 1.0f;
    work->duplicatedHandles = NULL;
    work->duplicateAllocation = NULL;
    work->childWork = dds3ScatterPolyListCreate(params->particleCount);
    work->childWork->poolMode = params->poolMode;
    if (resource != 0) {
        dds3ScatterPolyListSetTex(work->childWork, resource);
    }
    if (particleParams != NULL && work->params.duplicateParticles != 0) {
        if (work->params.particlesPerGroup == 0) {
            work->params.particlesPerGroup = 1;
        }
        work->duplicateGroupCount = work->params.particleCount / work->params.particlesPerGroup;
        if (work->params.particleCount % work->params.particlesPerGroup != 0) {
            work->duplicateGroupCount++;
        }
        count = work->duplicateGroupCount;
        handle = sdfMemAllocDirect(count * sizeof(u32));
        handles = (effPartHandle_t **)sdfMemLock(handle);
        work->duplicateAllocation = handle;
        work->duplicatedHandles = handles;
        work->duplicatedHandles[0] = dds3EffectPartCreate(EFF_PARAM_WORK_KIND_EXTENDED_WORK_WITH_MATRIX_CALLBACK, particleParams);
        for (i = 1; i < count; i++) {
            work->duplicatedHandles[i] = dds3EffectPartCopy(work->duplicatedHandles[0]);
        }
    }
    delaySpread = work->params.delaySpread;
    ageOffset = 0;
    count = work->params.particleCount;
    particle = work->particles;
    if (delaySpread <= 0) {
        delaySpread = 1;
    }
    segments = work->params.radialSegments;
    for (i = 0; i < count; i++, particle++) {
        particle->age = ageOffset - dds3GetRandA((EffRandState *)effDefaultRandomState) % delaySpread;
        if ((i + 1) % segments == 0) {
            ageOffset -= delaySpread;
        }
    }
    return work;
}
