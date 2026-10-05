#include "common.h"
#include "eff.h"
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

typedef struct SdfMemoryBlock SdfMemoryBlock;


typedef struct {
    u8 pad0[8];
    f32 accumulated;
    u8 padC[4];
} EffectVectorRecord;

typedef struct {
    u8 pad0[0x50];
    f32 increment;
    u32 value54;
    EffectVectorRecord *vectors;
    u8 pad5C[4];
    u8 *records;
    u8 *indices;
    u32 resource;
    u32 buffer;
    u8 pad70[8];
    u32 handle78;
    u32 handle7C;
} EffectRecordGroup;

extern u64 effParamTableGetBlock(u64, u64);

extern void func_001705A0();

extern s32 effMultiplyPackedColors(s32 color, s32 param);
extern s32 effGetExtendedGroupElement(EffectRecordGroup *group, s32 index);
extern f32 D_00354980[];
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32 angle);
extern f32 sdfSinPoly(f32 angle);

/* Position and color arrays precede this 0x70-byte pool header. Constructors
 * below clear their entire allocation, including both arrays and the header. */
typedef struct EffRecordPool {
    f32 matrix[EFF_MATRIX_WORD_COUNT];
    f32 origin[3];
    u8 pad4C[4];
    u32 drawMode;      /* 0x50: selects the packet submission surface */
    u32 color;         /* 0x54: 0x80808080 on creation */
    s32 vertexCount;   /* 0x58: positions and color words, not group count */
    f32 scale;         /* 0x5C: 1.0f on creation */
    s32 recordBase;    /* 0x60 */
    s32 auxRecordBase; /* 0x64 */
    u32 resource;      /* 0x68 */
    u32 buffer;        /* 0x6C */
} EffRecordPool;

typedef struct EffPacketParams {
    s16 parameterCount;
    s16 vertexCount;
    u16 primitive;
    u16 mask;
    u32 unk08;
    u32 *parameters;
    u128 *positions;
    u128 *normals;
    u32 *texcoords;
    u32 *extraTexcoords;
    u32 *colors;
    void *(*allocate)(s32);
    f32 depth;
} EffPacketParams;

typedef struct EffDrawSurface {
    u8 pad00[0x10];
    void (*submit)(struct EffDrawSurface *, void *);
} EffDrawSurface;

extern EffPacketParams D_003D6550[];
extern u32 D_00354990[];
extern EffDrawSurface *D_003549D8[];
extern EffDrawSurface *D_00354A58[];
extern EffDrawSurface *D_003549E8[];
extern EffDrawSurface *D_00354A48[];
extern u32 D_00354A00[];
extern EffDrawSurface D_00325248;
extern void *sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(void *);
extern void sdfAppendPacket(void *, void *);
extern void sdfConsAppendVuPacket(void *, s32);
extern void sdfConsAppendAssetPacket(void *, u32, s32);
extern void *func_0015FE20(EffPacketParams *);

/* Ring (fan) effect: a copy of the 0x58-byte parameter block followed by
 * `count` evenly spaced angular records; each expands to a five-vertex fan. */
typedef struct EffectRingHeader {
    u8 head[EFF_RING_PARAM_BYTES];
} EffectRingHeader;

typedef struct EffectRingVertex {
    s32 pad0;
    s32 offset;
    f32 angle;
    s32 padC;
} EffectRingVertex;

typedef struct EffectRing {
    u8 pad00[0x10];
    u32 count;
    u8 pad14[8];
    s32 spread;
    u8 pad20[8];
    u32 firstColor;
    u32 secondColor;
    f32 param30;
    f32 param34;
    f32 param38;
    u8 pad3C[0x14];
    u32 unk50;
    u32 drawMode;
    EffectRingVertex *vertices;
    s32 unk5C;
    u32 color;
    f32 scale;
    f32 unk68;
    u8 pad6C[4];
    f32 unk70;
    f32 unk74;
    u32 handle;
    u8 *matrix;
} EffectRing;

/* The vertex array starts immediately after the 0x80-byte header in this block. */
typedef struct EffectRingBlock {
    EffectRing header;              /* 0x00, 0x80 bytes */
    EffectRingVertex vertices[1];   /* 0x80 */
} EffectRingBlock;

extern SdfMemoryBlock *sdfAllocGeneralBlock(u32 size);
extern void *sdfResourceRetainAddress(SdfMemoryBlock *block);
extern u8 *effAllocateIdentityMatrixWork(u32);
extern u32 effMiscRand(void *state);
extern u8 D_0034DF38[];

/* Allocate a circular fan group. Zero spread becomes one; count is not guarded. */
/* K&R: effCreateRingFanFromParams passes the table block as the raw 64-bit value. */
EffectRing *effCreateRingFan(source)
EffectRing *source;
{
    u32 handle;
    EffectRing *ring;
    EffectRingBlock *block;
    f32 angle;
    f32 step;
    u32 spread;
    u32 i;

    handle = (u32)sdfAllocGeneralBlock(source->count * EFF_RING_VERTEX_BYTES + EFF_RING_HEADER_BYTES);
    block = (EffectRingBlock *)sdfResourceRetainAddress((SdfMemoryBlock *)handle);
    ring = &block->header;
    memcpy(ring, source, EFF_RING_PARAM_BYTES);
    ring->vertices = &block->vertices[0];
    ring->handle = handle;
    ring->color = EFF_NEUTRAL_COLOR;
    ring->unk68 = ring->param38;
    ring->unk70 = ring->param30;
    ring->unk74 = ring->param34;
    ring->unk5C = 0;
    ring->scale = 1.0f;
    if (ring->spread == 0) {
        ring->spread = 1;
    }
    angle = EFFECT_RING_START_ANGLE;
    ring->matrix = effAllocateIdentityMatrixWork(ring->count);
    ((EffRecordPool *)ring->matrix)->scale = 1.0f;
    ((EffRecordPool *)ring->matrix)->drawMode = ring->drawMode;
    step = EFFECT_RING_FULL_TURN / ring->count;
    spread = ring->spread;
    for (i = 0; i < ring->count; i++) {
        ring->vertices[i].offset = -(effMiscRand(D_0034DF38) % spread);
        ring->vertices[i].angle = angle;
        angle += step;
    }
    return ring;
}

/* Create a ring from the first parameter-table block. */
void effCreateRingFanFromParams(u64 table) {
    u64 parameterBlock;

    parameterBlock = effParamTableGetBlock(table, 0);
    effCreateRingFan(parameterBlock);
}

void func_0016F440(EffectRing *ring) {
    effCreateRingFan(ring);
}

/* Release the matrix pool's resources before the ring's backing allocation. */
void effReleaseRingResources(EffectRing *ring) {
    func_001705A0((u32)ring->matrix);
    sdfReleaseResourceAllocation(ring->handle);
}

/* Copy one quadword; the fourth component is copied along with XYZ. */
void effCopyRingVector(void *destination, void *source) {
    PCP_COPY_VECTOR(destination, source);
}

/* Replace the ring's packed modulation color without touching vertex colors. */
void effSetRingColor(EffectRing *ring, u32 color) {
    ring->color = color;
}

/* Set the ring's scalar, preserving the byte-pointer calling interface. */
void effSetRingScale(u8 *work, f32 scale) {
    ((EffectRing *)work)->scale = scale;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effCopyRingTransformMatrix(void *work, void *matrixSource) {
    VU0_LOAD_MATRIX(matrixSource);
    VU0_STORE_MATRIX(((EffectRing *)work)->matrix);
}

/* Each five-vertex fan group has one packed color word per vertex. */
typedef struct EffRingColorSlot {
    s32 colors[EFF_FAN_VERTEX_COUNT];
} EffRingColorSlot;

/* Fill five fan-vertex colors; odd and even groups use different alpha values. */
void effFlashWriteRingColorSlots(u8 *work, s32 index, s32 modulationColor) {
    EffRingColorSlot *slot;
    s32 firstRgb;
    s32 secondRgb;

    slot = (EffRingColorSlot *)effGetExtendedGroupAuxEntry(((EffectRing *)work)->matrix, index);
    firstRgb = ((EffectRing *)work)->firstColor & EFF_RGB_MASK;
    secondRgb = ((EffectRing *)work)->secondColor & EFF_RGB_MASK;
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

typedef struct {
    u8 pad00[8];
    f32 phase;
    f32 unk0C;
} EffectArcQuadPart;

typedef struct {
    u8 pad00[0x58];
    EffectArcQuadPart *parts;
    u8 pad5C[0x0C];
    f32 orbitRadius;
    f32 unk6C;
    f32 unk70;
    f32 unk74;
    u8 pad78[4];
    EffectRecordGroup *recordPool;
} EffectArcQuadWork;

/* vu0 routine: build five orbiting fan positions from a phase-derived basis.
 * No camera matrix is read. Quadword loads retain the native scratch W values. */
void effBuildOrbitingArcQuadPoints(EffectArcQuadWork *work, s32 index) {
    EffectArcQuadPart *part = &work->parts[index];
    f32 *positions = (f32 *)effGetExtendedGroupElement(work->recordPool, index);
    f32 orbitOffset[4];
    f32 radialDirection[4];
    f32 axisOffset[4];
    f32 sideOffsetA[4];
    f32 sideOffsetB[4];
    f32 sine;
    f32 basisFactor;

    VEC3_SPLAT(axisOffset, work->unk6C);
    VEC3_SPLAT(sideOffsetA, work->unk74);
    VEC3_SPLAT(sideOffsetB, work->unk70);
    radialDirection[0] = sdfEvaluateCosineViaSinePhaseShift(part->phase);
    radialDirection[1] = 0;
    sine = sdfSinPoly(part->phase);
    radialDirection[2] = sine;
    orbitOffset[0] = radialDirection[0] * work->orbitRadius;
    orbitOffset[1] = 0;
    orbitOffset[2] = sine * work->orbitRadius;
    basisFactor = part->unk0C;
    D_00354980[0] = radialDirection[0] * basisFactor;
    D_00354980[1] = basisFactor + -1.0f;
    D_00354980[2] = sine * basisFactor;
    VU0_LOAD_VF(vf10, D_00354980);
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
void effAdvanceVectorRecord(EffectRecordGroup *group, s32 index) {
    EffectVectorRecord *record;

    record = &group->vectors[index];
    record->accumulated = record->accumulated + group->increment;
}

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_0016F7B0);

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_0016FB08);

/* Queue asset release, then free the pool allocation; neither handle is cleared. */
void effReleaseRecordGroupAssetAndHandle(EffectRecordGroup *group) {
    sdfQueueAssetRelease(group->resource);
    sdfReleaseResourceAllocation(group->buffer);
}

/* Build scale/origin transform and submit five-vertex groups in batches of 25.
 * The remainder path retains its native vertex count, even for a partial group. */
void effDrawScaledRecordPool(EffRecordPool *work)
{
    f32 matrix[EFF_MATRIX_WORD_COUNT] __attribute__((aligned(16)));
    void *list;
    s32 remaining;
    s32 fanCount;
    u64 *packet;
    void *stateList;

    list = sdfAllocPacketAligned(EFF_PACKET_LIST_BYTES);
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
    D_003D6550->colors = (u32 *)work->auxRecordBase;
    D_003D6550->positions = (u128 *)work->recordBase;
    D_003D6550->unk08 = work->color;
    D_003D6550->parameterCount = EFF_FAN_BATCH_PARAMS;
    D_003D6550->vertexCount = EFF_FAN_BATCH_VERTICES;
    D_003D6550->parameters = D_00354990;
    while (remaining >= EFF_FAN_BATCH_VERTICES) {
        remaining -= EFF_FAN_BATCH_VERTICES;
        sdfAppendPacket(list, func_0015FE20(D_003D6550));
        D_003D6550->positions += EFF_FAN_BATCH_VERTICES;
        D_003D6550->colors += EFF_FAN_BATCH_VERTICES;
    }
    if (remaining >= EFF_FAN_VERTEX_COUNT) {
        fanCount = remaining / EFF_FAN_VERTEX_COUNT;
        D_003D6550->vertexCount = remaining;
        D_003D6550->parameterCount = fanCount * 3;
        sdfAppendPacket(list, func_0015FE20(D_003D6550));
    }
    if (work->drawMode < EFF_DIRECT_SURFACE_COUNT) {
        D_003549D8[work->drawMode]->submit(D_003549D8[work->drawMode], list);
    } else {
        stateList = sdfAllocPacketAligned(EFF_PACKET_LIST_BYTES);
        sdfInitPacketList(stateList);
        packet = sdfAllocPacketAligned(EFF_STATE_PACKET_BYTES);
        packet[4] = 6;
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[5] = 0x42;
        sdfAppendPacket(stateList, packet);
        D_00325248.submit(&D_00325248, stateList);
        packet = sdfAllocPacketAligned(EFF_STATE_PACKET_BYTES);
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[4] = 0x42;
        packet[5] = 0x42;
        sdfAppendPacket(list, packet);
        D_00325248.submit(&D_00325248, list);
    }
}

/* Address the five-position record for a group; no index bounds check. */
s32 effGetIndexedEffectGroupRecord(EffectRecordGroup *group, s32 groupIndex) {
    return (s32)group->records + groupIndex * EFF_FAN_POSITION_BYTES;
}

/* Address the group's five packed color words. */
s32 effGetIndexedEffectGroupIndexEntry(EffectRecordGroup *group, s32 groupIndex) {
    return (s32)group->indices + groupIndex * EFF_FAN_COLOR_BYTES;
}

/* Store raw control bits: this view interprets them as a floating increment. */
void effSetVectorIncrementBits(EffectRecordGroup *group, u32 incrementBits) {
    *(u32 *)&group->increment = incrementBits;
}

/* Opaque control word: only the store is understood here. */
void func_0016FF40(EffectRecordGroup *group, u32 value) {
    group->value54 = value;
}

/* Set the pool scale without changing the native byte-pointer interface. */
void func_0016FF48(u8 *work, f32 scale) {
    ((EffRecordPool *)work)->scale = scale;
}


extern void *memset(void *dst, s32 value, u32 size);
extern u32 sdfCreateAssetWithDrawEntries(void);
extern void func_002DA420(u32 asset, f32 value);

/* Allocate three positions and three colors per triangle, then the header.
 * The complete block and shared packet-parameter record are cleared. */
EffRecordPool *effRecordPoolCreateTriple(s32 triangleCount) {
    EffRecordPool *pool;
    u32 handle;
    u32 *block;
    s32 vertexCount;
    s32 positionWordCount;
    s32 colorWordCount;
    u32 size;

    vertexCount = triangleCount * EFF_TRIANGLE_VERTEX_COUNT;
    positionWordCount = vertexCount * 4;
    colorWordCount = vertexCount;
    size = (positionWordCount + colorWordCount) * 4 + EFF_POOL_HEADER_BYTES;
    handle = (u32)sdfAllocGeneralBlock(size);
    block = (u32 *)sdfResourceRetainAddress((SdfMemoryBlock *)handle);
    memset(block, 0, size);
    pool = (EffRecordPool *)(block + (positionWordCount + colorWordCount));
    pool->recordBase = (s32)block;
    pool->auxRecordBase = (s32)(block + positionWordCount);
    pool->color = EFF_NEUTRAL_COLOR;
    pool->drawMode = 2;
    pool->vertexCount = colorWordCount;
    pool->buffer = handle;
    pool->scale = 1.0f;
    pool->resource = sdfCreateAssetWithDrawEntries();
    func_002DA420(pool->resource, 1.0f);
    memset(D_003D6550, 0, EFF_PACKET_PARAMS_BYTES);
    D_003D6550->primitive = 0x4000;
    return pool;
}

/* Release the asset and backing allocation; the header points into that block. */
void effReleaseRecordPoolResourceAndBuffer(EffectRecordGroup *group) {
    sdfQueueAssetRelease(group->resource);
    sdfReleaseResourceAllocation(group->buffer);
}

/* Submit the position/color pool in triangle batches of at most 48 vertices. */
void effDrawTriangleRecordPool(EffRecordPool *pool)
{
    f32 matrix[EFF_MATRIX_WORD_COUNT];
    void *packet = sdfAllocPacketAligned(EFF_PACKET_LIST_BYTES);
    s32 remainingVertices;
    EffDrawSurface *surface;

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
    D_003D6550->colors = (u32 *)pool->auxRecordBase;
    D_003D6550->positions = (u128 *)pool->recordBase;
    D_003D6550->unk08 = pool->color;
    D_003D6550->parameterCount = 0x10;
    D_003D6550->vertexCount = EFF_TRIANGLE_BATCH_VERTICES;
    D_003D6550->parameters = NULL;
    while (remainingVertices >= EFF_TRIANGLE_BATCH_VERTICES) {
        remainingVertices -= EFF_TRIANGLE_BATCH_VERTICES;
        sdfAppendPacket(packet, func_0015FE20(D_003D6550));
        D_003D6550->positions += EFF_TRIANGLE_BATCH_VERTICES;
        D_003D6550->colors += EFF_TRIANGLE_BATCH_VERTICES;
    }
    if (remainingVertices >= EFF_TRIANGLE_VERTEX_COUNT) {
        D_003D6550->parameterCount = remainingVertices / EFF_TRIANGLE_VERTEX_COUNT;
        D_003D6550->vertexCount = remainingVertices;
        sdfAppendPacket(packet, func_0015FE20(D_003D6550));
    }
    surface = D_003549E8[pool->drawMode];
    surface->submit(surface, packet);
}

/* Address three quadword positions for one triangle. */
s32 effGetGroupRecordByIndex(EffectRecordGroup *group, s32 groupIndex) {
    return (s32)group->records + groupIndex * EFF_TRIANGLE_POSITION_BYTES;
}

/* Address three packed color words for one triangle. */
s32 effGetGroupIndexRecord(EffectRecordGroup *group, s32 groupIndex) {
    return (s32)group->indices + groupIndex * EFF_TRIANGLE_COLOR_BYTES;
}

/* Allocate four positions and four colors per quad, followed by the header.
 * Both allocation and shared packet parameters are cleared before defaults. */
EffRecordPool *effRecordPoolCreate(s32 quadCount) {
    EffRecordPool *pool;
    u32 handle;
    u32 *block;
    s32 positionWordCount;
    s32 colorWordCount;
    u32 size;

    positionWordCount = quadCount * 16;
    colorWordCount = quadCount * EFF_QUAD_VERTEX_COUNT;
    size = (positionWordCount + colorWordCount) * 4 + EFF_POOL_HEADER_BYTES;
    handle = (u32)sdfAllocGeneralBlock(size);
    block = (u32 *)sdfResourceRetainAddress((SdfMemoryBlock *)handle);
    memset(block, 0, size);
    pool = (EffRecordPool *)(block + (positionWordCount + colorWordCount));
    pool->recordBase = (s32)block;
    pool->drawMode = 2;
    pool->auxRecordBase = (s32)(block + positionWordCount);
    pool->vertexCount = colorWordCount;
    pool->buffer = handle;
    pool->scale = 1.0f;
    pool->color = EFF_NEUTRAL_COLOR;
    pool->resource = sdfCreateAssetWithDrawEntries();
    func_002DA420(pool->resource, 1.0f);
    memset(D_003D6550, 0, EFF_PACKET_PARAMS_BYTES);
    D_003D6550->primitive = 0x4000;
    return pool;
}

/* Queue the asset before releasing the allocation containing this header. */
void effReleaseRecordGroupResources(EffectRecordGroup *group) {
    sdfQueueAssetRelease(group->resource);
    sdfReleaseResourceAllocation(group->buffer);
}

/* Submit the position/color pool in quad batches of at most 32 vertices. */
void effDrawQuadRecordPool(EffRecordPool *pool)
{
    f32 matrix[EFF_MATRIX_WORD_COUNT];
    void *packet = sdfAllocPacketAligned(EFF_PACKET_LIST_BYTES);
    s32 remainingVertices;
    EffDrawSurface *surface;

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
    D_003D6550->colors = (u32 *)pool->auxRecordBase;
    D_003D6550->positions = (u128 *)pool->recordBase;
    D_003D6550->unk08 = pool->color;
    D_003D6550->parameterCount = 0x10;
    D_003D6550->vertexCount = EFF_QUAD_BATCH_VERTICES;
    D_003D6550->parameters = D_00354A00;
    while (remainingVertices >= EFF_QUAD_BATCH_VERTICES) {
        remainingVertices -= EFF_QUAD_BATCH_VERTICES;
        sdfAppendPacket(packet, func_0015FE20(D_003D6550));
        D_003D6550->positions += EFF_QUAD_BATCH_VERTICES;
        D_003D6550->colors += EFF_QUAD_BATCH_VERTICES;
    }
    if (remainingVertices >= EFF_QUAD_VERTEX_COUNT) {
        D_003D6550->parameterCount = remainingVertices / EFF_QUAD_VERTEX_COUNT * 2;
        D_003D6550->vertexCount = remainingVertices;
        sdfAppendPacket(packet, func_0015FE20(D_003D6550));
    }
    surface = D_00354A48[pool->drawMode];
    surface->submit(surface, packet);
}

/* Address four quadword positions for one quad. */
s32 effGetRecordGroupElement(EffectRecordGroup *group, s32 groupIndex) {
    return (s32)group->records + groupIndex * EFF_QUAD_POSITION_BYTES;
}

/* Address four packed color words for one quad. */
s32 effGetRecordGroupAuxEntry(EffectRecordGroup *group, s32 groupIndex) {
    return (s32)group->indices + groupIndex * EFF_QUAD_COLOR_BYTES;
}

/* Allocate five-vertex fan groups, then set the returned header's matrix. */
u8 *effAllocateIdentityMatrixWork(u32 fanCount) {
    u8 *matrix = func_0016FB08(fanCount);

    EE_MMI_UNIT_MATRIX(matrix);
    return matrix;
}

void func_001705A0(u32 id) {
    effReleaseRecordGroupAssetAndHandle(id);
}

/* Apply the stored matrix/origin, then submit batches of five-vertex groups.
 * Native state-only and drawing lists remain separate for non-direct modes. */
void effDrawTransformedRecordPool(EffRecordPool *work)
{
    f32 matrix[EFF_MATRIX_WORD_COUNT] __attribute__((aligned(16)));
    void *list;
    s32 remaining;
    s32 fanCount;
    u64 *packet;
    void *stateList;

    list = sdfAllocPacketAligned(EFF_PACKET_LIST_BYTES);
    sdfInitPacketList(list);
    VU0_COPY_MATRIX(matrix, work->matrix);
    matrix[12] = work->origin[0];
    matrix[13] = work->origin[1];
    matrix[14] = work->origin[2];
    VU0_LOAD_MATRIX(matrix);
    sdfConsAppendVuPacket(list, 0);
    sdfConsAppendAssetPacket(list, work->resource, 0);
    remaining = work->vertexCount;
    D_003D6550->colors = (u32 *)work->auxRecordBase;
    D_003D6550->positions = (u128 *)work->recordBase;
    D_003D6550->unk08 = work->color;
    D_003D6550->parameterCount = EFF_FAN_BATCH_PARAMS;
    D_003D6550->vertexCount = EFF_FAN_BATCH_VERTICES;
    D_003D6550->parameters = D_00354990;
    while (remaining >= EFF_FAN_BATCH_VERTICES) {
        remaining -= EFF_FAN_BATCH_VERTICES;
        sdfAppendPacket(list, func_0015FE20(D_003D6550));
        D_003D6550->positions += EFF_FAN_BATCH_VERTICES;
        D_003D6550->colors += EFF_FAN_BATCH_VERTICES;
    }
    if (remaining >= EFF_FAN_VERTEX_COUNT) {
        fanCount = remaining / EFF_FAN_VERTEX_COUNT;
        D_003D6550->vertexCount = remaining;
        D_003D6550->parameterCount = fanCount * 3;
        sdfAppendPacket(list, func_0015FE20(D_003D6550));
    }
    if (work->drawMode < EFF_DIRECT_SURFACE_COUNT) {
        D_00354A58[work->drawMode]->submit(D_00354A58[work->drawMode], list);
    } else {
        stateList = sdfAllocPacketAligned(EFF_PACKET_LIST_BYTES);
        sdfInitPacketList(stateList);
        packet = sdfAllocPacketAligned(EFF_STATE_PACKET_BYTES);
        packet[4] = 6;
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[5] = 0x42;
        sdfAppendPacket(stateList, packet);
        D_00325248.submit(&D_00325248, stateList);
        packet = sdfAllocPacketAligned(EFF_STATE_PACKET_BYTES);
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[4] = 0x42;
        packet[5] = 0x42;
        sdfAppendPacket(list, packet);
        D_00325248.submit(&D_00325248, list);
    }
}

/* Address the five-position record for one fan group. */
s32 effGetExtendedGroupElement(EffectRecordGroup *group, s32 groupIndex) {
    return (s32)group->records + groupIndex * EFF_FAN_POSITION_BYTES;
}

/* Address the five color words for one fan group. */
s32 effGetExtendedGroupAuxEntry(EffectRecordGroup *group, s32 groupIndex) {
    return (s32)group->indices + groupIndex * EFF_FAN_COLOR_BYTES;
}

/* Select the record pool's packet submission mode. */
void effSetRecordPoolDrawMode(s32 poolAddress, u32 mode) {
    ((EffRecordPool *)poolAddress)->drawMode = mode;
}

/* Set the packed color word passed to the pool's packet builder. */
void effSetRecordPoolColor(s32 poolAddress, u32 color) {
    ((EffRecordPool *)poolAddress)->color = color;
}

/* Set the scalar used by the scale/origin rendering path. */
void effSetRecordPoolScale(u8 *work, f32 scale) {
    ((EffRecordPool *)work)->scale = scale;
}

typedef struct PcpScatterPool {
    u8 pad00[0x10];
    u32 unk10;
    u32 color;
    s32 secondWordCount;
    f32 unk1C;
    s32 recordBase;
    s32 auxRecordBase;
    u32 resource;
    SdfMemoryBlock *buffer;
    PcpScatterRes *sharedResource;
} PcpScatterPool;

typedef struct {
    f32 origin[4];
    f32 width;
    f32 height;
    u32 poolMode;
    u8 respawn;
    u8 pad1D[3];
    u32 particleCount;
    u32 radialSegments;
    s32 delaySpread;
    u32 fadeIn;
    u32 fadeOut;
    s32 duration;
    u8 pad38[4];
    f32 unk3C;
    u32 unk40;
    f32 unk44;
    u32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 radiusJitter;
    f32 targetRadiusJitter;
    f32 speedJitter;
    u8 pad60[8];
    u8 duplicateParticles;
    u8 pad69[3];
    s32 duplicateStartAge;
    u32 particlesPerGroup;
} PcpScatterRadialParams;

typedef struct {
    s32 age;
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 radius;
    f32 angle;
    f32 unk18;
} PcpScatterRadialParticle;

typedef struct PcpScatterRadialWork PcpScatterRadialWork;

struct PcpScatterRadialWork {
    PcpScatterRadialParams params;
    PcpScatterRadialParticle *particles;
    f32 scale;
    u32 color;
    PcpScatterPool *childWork;
    SdfMemoryBlock *ownedResource;
    u32 duplicatedCount;
    u32 *duplicatedHandles;
    SdfMemoryBlock *duplicateAllocation;
};

extern PcpScatterPool *effPcpScatterPoolCreate(s32 groups);
extern void effPcpScatterCreatePoolResource(PcpScatterPool *work, u32 resource);
extern u32 effParamWorkCreate(s32 kind, void *params);
extern u32 effParamWorkDuplicate(u32 handle);

PcpScatterRadialWork *func_001708A0(params, resource, particleParams)
    const PcpScatterRadialParams *params;
    u32 resource;
    void *particleParams;
{
    PcpScatterRadialWork *work;
    PcpScatterRadialParticle *particle;
    SdfMemoryBlock *handle;
    u32 *handles;
    u32 count;
    u32 i;
    s32 delaySpread;
    s32 ageOffset;
    u32 segments;

    handle = sdfAllocGeneralBlock(sizeof(PcpScatterRadialWork) + params->particleCount * sizeof(PcpScatterRadialParticle));
    work = sdfResourceRetainAddress(handle);
    work->particles = (PcpScatterRadialParticle *)(work + 1);
    work->params = *params;
    work->color = 0x80808080;
    work->ownedResource = handle;
    work->scale = 1.0f;
    work->duplicatedHandles = NULL;
    work->duplicateAllocation = NULL;
    work->childWork = effPcpScatterPoolCreate(params->particleCount);
    work->childWork->unk10 = params->poolMode;
    if (resource != 0) {
        effPcpScatterCreatePoolResource(work->childWork, resource);
    }
    if (particleParams != NULL && work->params.duplicateParticles != 0) {
        if (work->params.particlesPerGroup == 0) {
            work->params.particlesPerGroup = 1;
        }
        work->duplicatedCount = work->params.particleCount / work->params.particlesPerGroup;
        if (work->params.particleCount % work->params.particlesPerGroup != 0) {
            work->duplicatedCount++;
        }
        count = work->duplicatedCount;
        handle = sdfAllocGeneralBlock(count * sizeof(u32));
        handles = sdfResourceRetainAddress(handle);
        work->duplicateAllocation = handle;
        work->duplicatedHandles = handles;
        work->duplicatedHandles[0] = effParamWorkCreate(6, particleParams);
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
        particle->age = ageOffset - effMiscRand(D_0034DF38) % delaySpread;
        if ((i + 1) % segments == 0) {
            ageOffset -= delaySpread;
        }
    }
    return work;
}
