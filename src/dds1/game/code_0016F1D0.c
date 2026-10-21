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


extern void effReleaseRingRecordPool(EffRecordPool *pool);

extern u32 effMultiplyPackedColors(u32 colorA, u32 colorB);
extern void *effGetExtendedGroupElement(EffRecordPool *pool, s32 index);
extern u32 *effGetExtendedGroupAuxEntry(EffRecordPool *pool, s32 index);
extern f32 effRingBasisScratchVector[];




extern EffPacketParams effRecordPoolDrawParams[];
extern u32 effFanPoolTriangleIndices[];
extern SdfPoolNode *effScaledFanDrawSurfaces[];
extern SdfPoolNode *effTransformedFanDrawSurfaces[];
extern SdfPoolNode *effTrianglePoolDrawSurfaces[];
extern SdfPoolNode *effQuadPoolDrawSurfaces[];
extern u32 effQuadPoolTriangleIndices[];
extern SdfPoolNode D_00325248;
#include "sdf_packet_alloc.h"
extern void *effCreateVertexDrawPacket(EffPacketParams *);


/* The vertex array starts immediately after the 0x80-byte header in this block. */
typedef struct EffectRingBlock {
    EffRingWork header;            /* 0x00, 0x80 bytes */
    EffRingParticle vertices[1];   /* 0x80 */
} EffectRingBlock;

extern EffRecordPool *effAllocateIdentityMatrixWork(u32 count);
extern EffRandState effDefaultRandomState[];


/* Allocate a circular fan group. Zero spread becomes one; count is not guarded. */
EffRingWork *effCreateRingFan(EffRingWork *source)
{
    SdfMemBlock *allocation;
    EffRingWork *ring;
    EffectRingBlock *block;
    f32 angle;
    f32 step;
    u32 spread;
    u32 i;

    allocation = sdfMemAllocDirect(source->count * EFF_RING_VERTEX_BYTES + EFF_RING_HEADER_BYTES);
    block = (EffectRingBlock *)sdfMemLock(allocation);
    ring = &block->header;
    memcpy(ring, source, EFF_RING_PARAM_BYTES);
    ring->vertices = &block->vertices[0];
    ring->allocationHandle = allocation;
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
    ring->recordPool = effAllocateIdentityMatrixWork(ring->count);
    ring->recordPool->scale = 1.0f;
    ring->recordPool->drawMode = ring->drawMode;
    step = EFFECT_RING_FULL_TURN / ring->count;
    spread = ring->spread;
    for (i = 0; i < ring->count; i++) {
        ring->vertices[i].age = -(sdfGetRand((EffRandState *)effDefaultRandomState) % spread);
        ring->vertices[i].angle = angle;
        angle += step;
    }
    return ring;
}

/* Create a ring from the first parameter-table block. */
EffRingWork *effCreateRingFanFromParams(void *table) {
    void *parameterBlock;

    parameterBlock = effParamTableGetBlock(table, 0);
    return effCreateRingFan(parameterBlock);
}

/* Kind 85 duplicate: recreate the parameter prefix, not the live vertex state.
 * Retail tail-jumps to the constructor, preserving its v0 result despite this void C surface. */
EffRingWork *effDuplicateRingFan(EffRingWork *ring) {
    return effCreateRingFan(ring);
}

/* Release the matrix pool's resources before the ring's backing allocation. */
void effReleaseRingResources(EffRingWork *ring) {
    effReleaseRingRecordPool(ring->recordPool);
    sdfMemFreeDirect(ring->allocationHandle);
}

/* Copy one quadword; the fourth component is copied along with XYZ. */
void effCopyRingVector(void *destination, void *source) {
    PCP_COPY_VECTOR(destination, source);
}

/* Replace the ring's packed modulation color without touching vertex colors. */
void effSetRingColor(EffRingWork *ring, u32 color) {
    ring->color = color;
}

/* Set the ring's overall render scale. */
void effSetRingScale(EffRingWork *work, f32 scale) {
    work->scale = scale;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effCopyRingTransformMatrix(EffRingWork *work, void *matrixSource) {
    VU0_LOAD_MATRIX(matrixSource);
    VU0_STORE_MATRIX(work->recordPool->matrix);
}

/* Fill five fan-vertex colors; odd and even groups use different alpha values. */
void effFlashWriteRingColorSlots(EffRingWork *work, s32 index, s32 modulationColor) {
    EffRingColorSlot *slot;
    s32 firstRgb;
    s32 secondRgb;

    slot = (EffRingColorSlot *)effGetExtendedGroupAuxEntry(work->recordPool, index);
    firstRgb = work->firstColor & EFF_RGB_MASK;
    secondRgb = work->secondColor & EFF_RGB_MASK;
    slot->colors[0] = effMultiplyPackedColors(secondRgb, modulationColor);
    slot->colors[1] = effMultiplyPackedColors(secondRgb, modulationColor);
    if (index & 1) {
        slot->colors[2] = effMultiplyPackedColors(0x80000000, modulationColor);
        slot->colors[3] = effMultiplyPackedColors(firstRgb | 0xFF000000, modulationColor);
        slot->colors[4] = effMultiplyPackedColors(0x80000000, modulationColor);
    } else {
        slot->colors[2] = effMultiplyPackedColors(0xFF000000, modulationColor);
        slot->colors[3] = effMultiplyPackedColors(firstRgb | 0x40000000, modulationColor);
        slot->colors[4] = effMultiplyPackedColors(0xFF000000, modulationColor);
    }
}


/* vu0 routine: build five orbiting fan positions from a phase-derived basis.
 * No camera matrix is read. Quadword loads retain the native scratch W values. */
void effBuildOrbitingArcQuadPoints(EffRingWork *work, s32 index) {
    EffRingParticle *part = &work->vertices[index];
    f32 *positions = (f32 *)effGetExtendedGroupElement(work->recordPool, index);
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
    sine = sdfSinPoly(part->angle);
    radialDirection[2] = sine;
    orbitOffset[0] = radialDirection[0] * work->currentRadius;
    orbitOffset[1] = 0;
    orbitOffset[2] = sine * work->currentRadius;
    basisFactor = part->basisFactor;
    effRingBasisScratchVector[0] = radialDirection[0] * basisFactor;
    effRingBasisScratchVector[1] = basisFactor + -1.0f;
    effRingBasisScratchVector[2] = sine * basisFactor;
    VU0_LOAD_VF(vf10, effRingBasisScratchVector);
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

/* Advance one ring particle's angle; index is unchecked. */
void effAdvanceVectorRecord(EffRingWork *work, s32 index) {
    EffRingParticle *particle;

    particle = &work->vertices[index];
    particle->angle = particle->angle + work->increment;
}
extern void effDrawTransformedRecordPool(EffRecordPool *);

void effUpdateOrbitingRingFan(EffRingWork *work)
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
    EffRecordPool *handle;
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
            effBuildOrbitingArcQuadPoints(work, index);
            effFlashWriteRingColorSlots(work, index, 0);
            part->color = 0x80808080;
        } else if (age >= lifetime) {
            if (restart != 0) {
                part->age = ~(sdfGetRand((EffRandState *)effDefaultRandomState) % randomRange);
            }
            color = 0;
            effFlashWriteRingColorSlots(work, index, color);
        } else if (age > 0) {
            progress = (f32)age / (f32)lifetime;
            part->basisFactor = heightDelta * progress + work->param48;
            effAdvanceVectorRecord(work, index);
            effBuildOrbitingArcQuadPoints(work, index);
            if (part->age < fadeIn && fadeIn != 0) {
                factor = (f32)part->age / (f32)fadeIn;
            } else if (fadeOut >= lifetime - part->age && fadeOut != 0) {
                factor = (f32)(lifetime - part->age) / (f32)fadeOut;
            } else {
                factor = 1.0f;
            }
            color = effMultiplyPackedColors(effBlendColor(0, part->color, factor), tintColor);
            effFlashWriteRingColorSlots(work, index, color);
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
    effDrawTransformedRecordPool(handle);
}

extern EffRecordPool *effRecordPoolCreateFiveVertexGroups(u32 count);

/* Allocate five 16-byte positions and five color words per group, then the 0x70-byte header. */
EffRecordPool *effRecordPoolCreateFiveVertexGroups(u32 count) {
    EffRecordPool *pool;
    SdfMemBlock *handle;
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
    pool = (EffRecordPool *)(block + recordBytes);
    pool->recordBase = (u8 *)block;
    pool->auxRecordBase = (u8 *)(block + vertexCount * 16);
    pool->color = EFF_NEUTRAL_COLOR;
    pool->drawMode = 2;
    pool->vertexCount = vertexCount;
    pool->buffer = handle;
    pool->scale = 1.0f;
    pool->resource = sdfCreateAssetWithDrawEntries();
    sdfSetPrimaryStateFloat(pool->resource, 1.0f);
    memset(effRecordPoolDrawParams, 0, EFF_PACKET_PARAMS_BYTES);
    effRecordPoolDrawParams->primitive = 0x4000;
    return pool;
}

/* Queue asset release, then free the pool allocation; neither handle is cleared. */
void effReleaseRecordGroupAssetAndHandle(EffRecordPool *group) {
    sdfQueueAssetRelease(group->resource);
    sdfMemFreeDirect(group->buffer);
}

/* Build scale/origin transform and submit five-vertex groups in batches of 25.
 * The remainder path retains its native vertex count, even for a partial group. */
void effDrawScaledRecordPool(EffRecordPool *work)
{
    f32 matrix[EFF_MATRIX_WORD_COUNT] __attribute__((aligned(16)));
    SdfListHead *list;
    s32 remaining;
    s32 fanCount;
    u64 *packet;
    SdfListHead *stateList;

    list = (SdfListHead *)sdfAllocPacketAligned(EFF_PACKET_LIST_BYTES);
    sdfInitPacketList(list);
    EE_MMI_UNIT_MATRIX(matrix);
    matrix[0] = work->scale;
    matrix[5] = work->scale;
    matrix[10] = work->scale;
    matrix[12] = work->origin[0];
    matrix[13] = work->origin[1];
    matrix[14] = work->origin[2];
    VU0_LOAD_MATRIX(matrix);
    sdfConsAppendVuPacket(list, 0);
    sdfConsAppendAssetPacket(list, work->resource, 0);
    remaining = work->vertexCount;
    effRecordPoolDrawParams->colors = (u32 *)work->auxRecordBase;
    effRecordPoolDrawParams->positions = (u128 *)work->recordBase;
    effRecordPoolDrawParams->color = work->color;
    effRecordPoolDrawParams->parameterCount = EFF_FAN_BATCH_PARAMS;
    effRecordPoolDrawParams->vertexCount = EFF_FAN_BATCH_VERTICES;
    effRecordPoolDrawParams->parameters = effFanPoolTriangleIndices;
    while (remaining >= EFF_FAN_BATCH_VERTICES) {
        remaining -= EFF_FAN_BATCH_VERTICES;
        sdfAppendPacket(list, (u32)effCreateVertexDrawPacket(effRecordPoolDrawParams));
        effRecordPoolDrawParams->positions += EFF_FAN_BATCH_VERTICES;
        effRecordPoolDrawParams->colors += EFF_FAN_BATCH_VERTICES;
    }
    if (remaining >= EFF_FAN_VERTEX_COUNT) {
        fanCount = remaining / EFF_FAN_VERTEX_COUNT;
        effRecordPoolDrawParams->vertexCount = remaining;
        effRecordPoolDrawParams->parameterCount = fanCount * 3;
        sdfAppendPacket(list, (u32)effCreateVertexDrawPacket(effRecordPoolDrawParams));
    }
    if (work->drawMode < EFF_DIRECT_SURFACE_COUNT) {
        effScaledFanDrawSurfaces[work->drawMode]->append(effScaledFanDrawSurfaces[work->drawMode], list);
    } else {
        stateList = (SdfListHead *)sdfAllocPacketAligned(EFF_PACKET_LIST_BYTES);
        sdfInitPacketList(stateList);
        packet = (u64 *)sdfAllocPacketAligned(EFF_STATE_PACKET_BYTES);
        packet[4] = 6;
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[5] = 0x42;
        sdfAppendPacket(stateList, (u32)packet);
        D_00325248.append(&D_00325248, stateList);
        packet = (u64 *)sdfAllocPacketAligned(EFF_STATE_PACKET_BYTES);
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[4] = 0x42;
        packet[5] = 0x42;
        sdfAppendPacket(list, (u32)packet);
        D_00325248.append(&D_00325248, list);
    }
}

/* Address the five-position record for a group; no index bounds check. */
void *effGetIndexedEffectGroupRecord(EffRecordPool *group, s32 groupIndex) {
    return group->recordBase + groupIndex * EFF_FAN_POSITION_BYTES;
}

/* Address the group's five packed color words. */
void *effGetIndexedEffectGroupIndexEntry(EffRecordPool *group, s32 groupIndex) {
    return group->auxRecordBase + groupIndex * EFF_FAN_COLOR_BYTES;
}

/* Store the record pool's raw submission-control bits. */
void effSetVectorIncrementBits(EffRecordPool *group, u32 incrementBits) {
    group->drawMode = incrementBits;
}

/* Store the record pool's packed modulation color. */
void effSetFanRecordPoolColor(EffRecordPool *group, u32 color) {
    group->color = color;
}

/* Set the pool scale. */
void effSetFanRecordPoolScale(EffRecordPool *work, f32 scale) {
    work->scale = scale;
}



/* Allocate three positions and three colors per triangle, then the header.
 * The complete block and shared packet-parameter record are cleared. */
EffRecordPool *effRecordPoolCreateTriple(s32 triangleCount) {
    EffRecordPool *pool;
    SdfMemBlock *handle;
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
    pool = (EffRecordPool *)(block + (positionWordCount + colorWordCount));
    pool->recordBase = (u8 *)block;
    pool->auxRecordBase = (u8 *)(block + positionWordCount);
    pool->color = EFF_NEUTRAL_COLOR;
    pool->drawMode = 2;
    pool->vertexCount = colorWordCount;
    pool->buffer = handle;
    pool->scale = 1.0f;
    pool->resource = sdfCreateAssetWithDrawEntries();
    sdfSetPrimaryStateFloat(pool->resource, 1.0f);
    memset(effRecordPoolDrawParams, 0, EFF_PACKET_PARAMS_BYTES);
    effRecordPoolDrawParams->primitive = 0x4000;
    return pool;
}

/* Release the asset and backing allocation; the header points into that block. */
void effReleaseRecordPoolResourceAndBuffer(EffRecordPool *group) {
    sdfQueueAssetRelease(group->resource);
    sdfMemFreeDirect(group->buffer);
}

/* Submit the position/color pool in triangle batches of at most 48 vertices. */
void effDrawTriangleRecordPool(EffRecordPool *pool)
{
    f32 matrix[EFF_MATRIX_WORD_COUNT];
    SdfListHead *packet = (SdfListHead *)sdfAllocPacketAligned(EFF_PACKET_LIST_BYTES);
    s32 remainingVertices;
    SdfPoolNode *surface;

    sdfInitPacketList(packet);
    EE_MMI_UNIT_MATRIX(matrix);
    matrix[0] = pool->scale;
    matrix[5] = pool->scale;
    matrix[10] = pool->scale;
    matrix[12] = pool->origin[0];
    matrix[13] = pool->origin[1];
    matrix[14] = pool->origin[2];
    VU0_LOAD_MATRIX(matrix);
    sdfConsAppendVuPacket(packet, 0);
    sdfConsAppendAssetPacket(packet, pool->resource, 0);
    remainingVertices = pool->vertexCount;
    effRecordPoolDrawParams->colors = (u32 *)pool->auxRecordBase;
    effRecordPoolDrawParams->positions = (u128 *)pool->recordBase;
    effRecordPoolDrawParams->color = pool->color;
    effRecordPoolDrawParams->parameterCount = 0x10;
    effRecordPoolDrawParams->vertexCount = EFF_TRIANGLE_BATCH_VERTICES;
    effRecordPoolDrawParams->parameters = NULL;
    while (remainingVertices >= EFF_TRIANGLE_BATCH_VERTICES) {
        remainingVertices -= EFF_TRIANGLE_BATCH_VERTICES;
        sdfAppendPacket(packet, (u32)effCreateVertexDrawPacket(effRecordPoolDrawParams));
        effRecordPoolDrawParams->positions += EFF_TRIANGLE_BATCH_VERTICES;
        effRecordPoolDrawParams->colors += EFF_TRIANGLE_BATCH_VERTICES;
    }
    if (remainingVertices >= EFF_TRIANGLE_VERTEX_COUNT) {
        effRecordPoolDrawParams->parameterCount = remainingVertices / EFF_TRIANGLE_VERTEX_COUNT;
        effRecordPoolDrawParams->vertexCount = remainingVertices;
        sdfAppendPacket(packet, (u32)effCreateVertexDrawPacket(effRecordPoolDrawParams));
    }
    surface = effTrianglePoolDrawSurfaces[pool->drawMode];
    surface->append(surface, packet);
}

/* Address three quadword positions for one triangle. */
void *effGetGroupRecordByIndex(EffRecordPool *group, s32 groupIndex) {
    return group->recordBase + groupIndex * EFF_TRIANGLE_POSITION_BYTES;
}

/* Address three packed color words for one triangle. */
void *effGetGroupIndexRecord(EffRecordPool *group, s32 groupIndex) {
    return group->auxRecordBase + groupIndex * EFF_TRIANGLE_COLOR_BYTES;
}

/* Allocate four positions and four colors per quad, followed by the header.
 * Both allocation and shared packet parameters are cleared before defaults. */
EffRecordPool *effRecordPoolCreate(s32 quadCount) {
    EffRecordPool *pool;
    SdfMemBlock *handle;
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
    pool = (EffRecordPool *)(block + (positionWordCount + colorWordCount));
    pool->recordBase = (u8 *)block;
    pool->drawMode = 2;
    pool->auxRecordBase = (u8 *)(block + positionWordCount);
    pool->vertexCount = colorWordCount;
    pool->buffer = handle;
    pool->scale = 1.0f;
    pool->color = EFF_NEUTRAL_COLOR;
    pool->resource = sdfCreateAssetWithDrawEntries();
    sdfSetPrimaryStateFloat(pool->resource, 1.0f);
    memset(effRecordPoolDrawParams, 0, EFF_PACKET_PARAMS_BYTES);
    effRecordPoolDrawParams->primitive = 0x4000;
    return pool;
}

/* Queue the asset before releasing the allocation containing this header. */
void effReleaseRecordGroupResources(EffRecordPool *group) {
    sdfQueueAssetRelease(group->resource);
    sdfMemFreeDirect(group->buffer);
}

/* Submit the position/color pool in quad batches of at most 32 vertices. */
void effDrawQuadRecordPool(EffRecordPool *pool)
{
    f32 matrix[EFF_MATRIX_WORD_COUNT];
    SdfListHead *packet = (SdfListHead *)sdfAllocPacketAligned(EFF_PACKET_LIST_BYTES);
    s32 remainingVertices;
    SdfPoolNode *surface;

    sdfInitPacketList(packet);
    EE_MMI_UNIT_MATRIX(matrix);
    matrix[0] = pool->scale;
    matrix[5] = pool->scale;
    matrix[10] = pool->scale;
    matrix[12] = pool->origin[0];
    matrix[13] = pool->origin[1];
    matrix[14] = pool->origin[2];
    VU0_LOAD_MATRIX(matrix);
    sdfConsAppendVuPacket(packet, 0);
    sdfConsAppendAssetPacket(packet, pool->resource, 0);
    remainingVertices = pool->vertexCount;
    effRecordPoolDrawParams->colors = (u32 *)pool->auxRecordBase;
    effRecordPoolDrawParams->positions = (u128 *)pool->recordBase;
    effRecordPoolDrawParams->color = pool->color;
    effRecordPoolDrawParams->parameterCount = 0x10;
    effRecordPoolDrawParams->vertexCount = EFF_QUAD_BATCH_VERTICES;
    effRecordPoolDrawParams->parameters = effQuadPoolTriangleIndices;
    while (remainingVertices >= EFF_QUAD_BATCH_VERTICES) {
        remainingVertices -= EFF_QUAD_BATCH_VERTICES;
        sdfAppendPacket(packet, (u32)effCreateVertexDrawPacket(effRecordPoolDrawParams));
        effRecordPoolDrawParams->positions += EFF_QUAD_BATCH_VERTICES;
        effRecordPoolDrawParams->colors += EFF_QUAD_BATCH_VERTICES;
    }
    if (remainingVertices >= EFF_QUAD_VERTEX_COUNT) {
        effRecordPoolDrawParams->parameterCount = remainingVertices / EFF_QUAD_VERTEX_COUNT * 2;
        effRecordPoolDrawParams->vertexCount = remainingVertices;
        sdfAppendPacket(packet, (u32)effCreateVertexDrawPacket(effRecordPoolDrawParams));
    }
    surface = effQuadPoolDrawSurfaces[pool->drawMode];
    surface->append(surface, packet);
}

/* Address four quadword positions for one quad. */
void *effGetRecordGroupElement(EffRecordPool *group, s32 groupIndex) {
    return group->recordBase + groupIndex * EFF_QUAD_POSITION_BYTES;
}

/* Address four packed color words for one quad. */
void *effGetRecordGroupAuxEntry(EffRecordPool *group, s32 groupIndex) {
    return group->auxRecordBase + groupIndex * EFF_QUAD_COLOR_BYTES;
}

/* Allocate five-vertex fan groups, then set the returned header's matrix. */
EffRecordPool *effAllocateIdentityMatrixWork(u32 fanCount) {
    EffRecordPool *pool = effRecordPoolCreateFiveVertexGroups(fanCount);

    EE_MMI_UNIT_MATRIX(pool->matrix);
    return pool;
}

void effReleaseRingRecordPool(EffRecordPool *pool) {
    effReleaseRecordGroupAssetAndHandle(pool);
}

/* Apply the stored matrix/origin, then submit batches of five-vertex groups.
 * Native state-only and drawing lists remain separate for non-direct modes. */
void effDrawTransformedRecordPool(EffRecordPool *work)
{
    f32 matrix[EFF_MATRIX_WORD_COUNT] __attribute__((aligned(16)));
    SdfListHead *list;
    s32 remaining;
    s32 fanCount;
    u64 *packet;
    SdfListHead *stateList;

    list = (SdfListHead *)sdfAllocPacketAligned(EFF_PACKET_LIST_BYTES);
    sdfInitPacketList(list);
    VU0_COPY_MATRIX(matrix, work->matrix);
    matrix[12] = work->origin[0];
    matrix[13] = work->origin[1];
    matrix[14] = work->origin[2];
    VU0_LOAD_MATRIX(matrix);
    sdfConsAppendVuPacket(list, 0);
    sdfConsAppendAssetPacket(list, work->resource, 0);
    remaining = work->vertexCount;
    effRecordPoolDrawParams->colors = (u32 *)work->auxRecordBase;
    effRecordPoolDrawParams->positions = (u128 *)work->recordBase;
    effRecordPoolDrawParams->color = work->color;
    effRecordPoolDrawParams->parameterCount = EFF_FAN_BATCH_PARAMS;
    effRecordPoolDrawParams->vertexCount = EFF_FAN_BATCH_VERTICES;
    effRecordPoolDrawParams->parameters = effFanPoolTriangleIndices;
    while (remaining >= EFF_FAN_BATCH_VERTICES) {
        remaining -= EFF_FAN_BATCH_VERTICES;
        sdfAppendPacket(list, (u32)effCreateVertexDrawPacket(effRecordPoolDrawParams));
        effRecordPoolDrawParams->positions += EFF_FAN_BATCH_VERTICES;
        effRecordPoolDrawParams->colors += EFF_FAN_BATCH_VERTICES;
    }
    if (remaining >= EFF_FAN_VERTEX_COUNT) {
        fanCount = remaining / EFF_FAN_VERTEX_COUNT;
        effRecordPoolDrawParams->vertexCount = remaining;
        effRecordPoolDrawParams->parameterCount = fanCount * 3;
        sdfAppendPacket(list, (u32)effCreateVertexDrawPacket(effRecordPoolDrawParams));
    }
    if (work->drawMode < EFF_DIRECT_SURFACE_COUNT) {
        effTransformedFanDrawSurfaces[work->drawMode]->append(effTransformedFanDrawSurfaces[work->drawMode], list);
    } else {
        stateList = (SdfListHead *)sdfAllocPacketAligned(EFF_PACKET_LIST_BYTES);
        sdfInitPacketList(stateList);
        packet = (u64 *)sdfAllocPacketAligned(EFF_STATE_PACKET_BYTES);
        packet[4] = 6;
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[5] = 0x42;
        sdfAppendPacket(stateList, (u32)packet);
        D_00325248.append(&D_00325248, stateList);
        packet = (u64 *)sdfAllocPacketAligned(EFF_STATE_PACKET_BYTES);
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[4] = 0x42;
        packet[5] = 0x42;
        sdfAppendPacket(list, (u32)packet);
        D_00325248.append(&D_00325248, list);
    }
}

/* Address the five-position record for one fan group. */
void *effGetExtendedGroupElement(EffRecordPool *group, s32 groupIndex) {
    return group->recordBase + groupIndex * EFF_FAN_POSITION_BYTES;
}

/* Address the five color words for one fan group. */
u32 *effGetExtendedGroupAuxEntry(EffRecordPool *group, s32 groupIndex) {
    return (u32 *)(group->auxRecordBase + groupIndex * EFF_FAN_COLOR_BYTES);
}

/* Select the record pool's packet submission mode. */
void effSetRecordPoolDrawMode(EffRecordPool *pool, u32 mode) {
    pool->drawMode = mode;
}

/* Set the packed color word passed to the pool's packet builder. */
void effSetRecordPoolColor(EffRecordPool *pool, u32 color) {
    pool->color = color;
}

/* Set the scalar used by the scale/origin rendering path. */
void effSetRecordPoolScale(EffRecordPool *work, f32 scale) {
    work->scale = scale;
}


extern PcpScatterPool *effPcpScatterPoolCreate(s32 groups);
extern void effPcpScatterCreatePoolResource(PcpScatterPool *work, u32 resource);
PcpScatterRadialWork *effScatterCreateRadialWork(params, resource, particleParams)
    const PcpScatterRadialParams *params;
    u32 resource;
    void *particleParams;
{
    PcpScatterRadialWork *work;
    PcpScatterRadialParticle *particle;
    SdfMemBlock *handle;
    EffParamWork **handles;
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
    work->childWork = effPcpScatterPoolCreate(params->particleCount);
    work->childWork->poolMode = params->poolMode;
    if (resource != 0) {
        effPcpScatterCreatePoolResource(work->childWork, resource);
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
        handles = (EffParamWork **)sdfMemLock(handle);
        work->duplicateAllocation = handle;
        work->duplicatedHandles = handles;
        work->duplicatedHandles[0] = effParamWorkCreate(EFF_PARAM_WORK_KIND_EXTENDED_WORK_WITH_MATRIX_CALLBACK, particleParams);
        for (i = 1; i < count; i++) {
            work->duplicatedHandles[i] = effParamWorkDuplicate(work->duplicatedHandles[0]);
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
        particle->age = ageOffset - sdfGetRand((EffRandState *)effDefaultRandomState) % delaySpread;
        if ((i + 1) % segments == 0) {
            ageOffset -= delaySpread;
        }
    }
    return work;
}
