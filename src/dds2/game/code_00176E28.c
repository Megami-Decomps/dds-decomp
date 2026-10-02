#include "common.h"
#include "eff.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

typedef struct SdfMemoryBlock SdfMemoryBlock;


extern u64 effParamTableGetBlock(u64, u64);

typedef struct EffVectorPart {
    u8 pad0[8];
    f32 accumulated;
    u8 padC[4];
} EffVectorPart;

typedef struct EffVectorWork {
    u8 pad0[0x50];
    f32 increment;
    u8 pad54[4];
    EffVectorPart *parts;
} EffVectorWork;

typedef struct EffRecordPool {
    f32 matrix[16];
    f32 origin[3];
    u8 pad4C[4];
    u32 drawMode;  /* 0x50: selects the packet submission surface */
    u32 color;     /* 0x54: packed color passed to the packet builder */
    s32 count;     /* 0x58 */
    f32 scale;     /* 0x5C */
    s32 recordBase;    /* 0x60: address of stride-dependent records */
    s32 auxRecordBase; /* 0x64: address of stride-dependent auxiliary records */
    u32 resource;   /* 0x68: released by sdfQueueAssetRelease */
    u32 buffer;     /* 0x6C: freed by sdfReleaseResourceAllocation */
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

extern EffPacketParams D_00451FF0[];
extern u32 D_003B12C0[];
extern EffDrawSurface *D_003B1308[];
extern EffDrawSurface *D_003B1388[];
extern EffDrawSurface *D_003B1318[];
extern EffDrawSurface *D_003B1378[];
extern u32 D_003B1330[];
extern EffDrawSurface D_00380248;
extern void *sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(void *);
extern void sdfAppendPacket(void *, void *);
extern void sdfConsAppendVuPacket(void *, s32);
extern void sdfConsAppendAssetPacket(void *, u32, s32);
extern void *func_00167A10(EffPacketParams *);


/* Ring (fan) effect: a copy of the 0x58-byte parameter block followed by
 * `count` vertices spread evenly around the circle from -pi/2. */
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

/* The vertex array lives inside the same block, 0x28 past the header. */
typedef struct EffectRingBlock {
    EffectRing header;              /* 0x00, 0x80 bytes */
    EffectRingVertex vertices[1];   /* 0x80 */
} EffectRingBlock;

extern SdfMemoryBlock *sdfAllocGeneralBlock(u32 size);

extern void *sdfResourceRetainAddress(SdfMemoryBlock *block);

extern u8 *effAllocateIdentityMatrixWork(u32);

extern u32 effMiscRand(void *state);

extern u8 D_003AA868[];
extern s32 effGetExtendedGroupElement(EffRecordPool *pool, s32 index);
extern f32 D_003B12B0[];
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32 angle);
extern f32 sdfSinPoly(f32 angle);

/* Allocate and initialize a circular fan, with randomized per-vertex offsets. */
/* K&R: effCreateRingFanFromParams passes the table block as the raw 64-bit value. */
extern s32 effMultiplyPackedColors(s32 color, s32 param);

EffectRing *effCreateRingFan(source)
EffectRing *source;

{
    u32 handle;
    EffectRingBlock *block;
    EffectRing *ring;
    f32 angle;
    f32 step;
    u32 spread;
    u32 i;

    handle = (u32)sdfAllocGeneralBlock(source->count * 16 + 0x80);
    block = (EffectRingBlock *)sdfResourceRetainAddress((SdfMemoryBlock *)handle);
    ring = &block->header;
    memcpy(ring, source, 0x58);
    ring->vertices = block->vertices;
    ring->handle = handle;
    ring->color = 0x80808080;
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
        ring->vertices[i].offset = -(effMiscRand(D_003AA868) % spread);
        ring->vertices[i].angle = angle;
        angle += step;
    }
    return ring;
}

/* Create a ring from the first parameter-table block. */
void effCreateRingFanFromParams(u64 params) {
    u64 block;

    block = effParamTableGetBlock(params, 0);
    effCreateRingFan(block);
}

void func_00177098(void) {
    effCreateRingFan();
}

/* Release both the ring's matrix work and its backing allocation. */
void effReleaseRingResources(EffectRing *ring) {
    func_001781F8(ring->matrix);
    sdfReleaseResourceAllocation(ring->handle);
}

void effCopyRingVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effSetRingColor(EffectRing *ring, u32 color) {
    ring->color = color;
}

void effSetRingScale(EffectRing *ring, f32 scale) {
    ring->scale = scale;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effCopyRingTransformMatrix(EffectRing *ring, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX(ring->matrix);
}

/* Each five-vertex fan group has one packed color word per vertex. */
typedef struct EffRingColorSlot {
    s32 colors[5];
} EffRingColorSlot;

/* Fill five fan-vertex colors; odd and even groups use different alpha values. */
void effFlashWriteRingColorSlots(u8 *work, s32 index, s32 param) {
    EffRingColorSlot *slot;
    s32 rgb1;
    s32 rgb2;

    slot = (EffRingColorSlot *)effGetExtendedGroupAuxEntry(((EffectRing *)work)->matrix, index);
    rgb1 = ((EffectRing *)work)->firstColor & 0xFFFFFF;
    rgb2 = ((EffectRing *)work)->secondColor & 0xFFFFFF;
    slot->colors[0] = effMultiplyPackedColors(rgb2, param);
    slot->colors[1] = effMultiplyPackedColors(rgb2, param);
    if (index & 1) {
        slot->colors[2] = effMultiplyPackedColors(0x80000000, param);
        slot->colors[3] = effMultiplyPackedColors(rgb1 | 0xFF000000, param);
        slot->colors[4] = effMultiplyPackedColors(0x80000000, param);
    } else {
        slot->colors[2] = effMultiplyPackedColors(0xFF000000, param);
        slot->colors[3] = effMultiplyPackedColors(rgb1 | 0x40000000, param);
        slot->colors[4] = effMultiplyPackedColors(0xFF000000, param);
    }
}

typedef struct {
    u8 pad00[8];
    f32 accumulator;
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
    EffRecordPool *resourceHandle;
} EffectArcQuadWork;

/* vu0 routine: billboard corner offsets for an arc particle */
void effBuildOrbitingArcQuadPoints(EffectArcQuadWork *work, s32 index) {
    EffectArcQuadPart *part = &work->parts[index];
    f32 *quad = (f32 *)effGetExtendedGroupElement(work->resourceHandle, index);
    f32 offset[4];
    f32 unit[4];
    f32 scaleA[4];
    f32 scaleB[4];
    f32 scaleC[4];
    f32 sinv;
    f32 height;

    VEC3_SPLAT(scaleA, work->unk6C);
    VEC3_SPLAT(scaleB, work->unk74);
    VEC3_SPLAT(scaleC, work->unk70);
    unit[0] = sdfEvaluateCosineViaSinePhaseShift(part->accumulator);
    unit[1] = 0;
    sinv = sdfSinPoly(part->accumulator);
    unit[2] = sinv;
    offset[0] = unit[0] * work->orbitRadius;
    offset[1] = 0;
    offset[2] = sinv * work->orbitRadius;
    height = part->unk0C;
    D_003B12B0[0] = unit[0] * height;
    D_003B12B0[1] = height + -1.0f;
    D_003B12B0[2] = sinv * height;
    VU0_LOAD_VF(vf10, D_003B12B0);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf11, vf10);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf10, unit);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, scaleB);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, scaleB);
    VU0_LOAD_VF(vf10, scaleC);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, scaleC);
    VU0_MOVE_VF(vf10, vf12);
    VU0_LOAD_VF(vf11, scaleA);
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf10, offset);
    VU0_LOAD_VF(vf11, scaleC);
    VU0_STORE_VF(vf10, quad + 12);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 8);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 16);
    VU0_LOAD_VF(vf10, offset);
    VU0_LOAD_VF(vf11, scaleB);
    VU0_ADD(vf10, vf10, vf12);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 4);
}

void effAdvanceVectorRecord(EffVectorWork *work, s32 index) {
    EffVectorPart *part;

    part = &work->parts[index];
    part->accumulated = part->accumulated + work->increment;
}

INCLUDE_ASM(const s32, "game/code_00176E28", func_00177408);

INCLUDE_ASM(const s32, "game/code_00176E28", func_00177760);

void effReleaseRecordGroupAssetAndHandle(EffRecordPool *pool) {
    sdfQueueAssetRelease(pool->resource);
    sdfReleaseResourceAllocation(pool->buffer);
}

void effDrawScaledRecordPool(EffRecordPool *work)
{
    f32 matrix[16] __attribute__((aligned(16)));
    void *list;
    s32 remaining;
    s32 chunk;
    u64 *packet;
    void *clearList;

    list = sdfAllocPacketAligned(0x20);
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
    remaining = work->count;
    D_00451FF0->colors = (u32 *)work->auxRecordBase;
    D_00451FF0->positions = (u128 *)work->recordBase;
    D_00451FF0->unk08 = work->color;
    D_00451FF0->parameterCount = 0xF;
    D_00451FF0->vertexCount = 0x19;
    D_00451FF0->parameters = D_003B12C0;
    while (remaining >= 0x19) {
        remaining -= 0x19;
        sdfAppendPacket(list, func_00167A10(D_00451FF0));
        D_00451FF0->positions += 0x19;
        D_00451FF0->colors += 0x19;
    }
    if (remaining >= 5) {
        chunk = remaining / 5;
        D_00451FF0->vertexCount = remaining;
        D_00451FF0->parameterCount = chunk * 3;
        sdfAppendPacket(list, func_00167A10(D_00451FF0));
    }
    if (work->drawMode < 4) {
        D_003B1308[work->drawMode]->submit(D_003B1308[work->drawMode], list);
    } else {
        clearList = sdfAllocPacketAligned(0x20);
        sdfInitPacketList(clearList);
        packet = sdfAllocPacketAligned(0x30);
        packet[4] = 6;
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[5] = 0x42;
        sdfAppendPacket(clearList, packet);
        D_00380248.submit(&D_00380248, clearList);
        packet = sdfAllocPacketAligned(0x30);
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[4] = 0x42;
        packet[5] = 0x42;
        sdfAppendPacket(list, packet);
        D_00380248.submit(&D_00380248, list);
    }
}

s32 effGetIndexedEffectGroupRecord(EffRecordPool *pool, s32 index) {
    return pool->recordBase + index * 0x50;
}

s32 effGetIndexedEffectGroupIndexEntry(EffRecordPool *pool, s32 index) {
    return pool->auxRecordBase + index * 0x14;
}

void effSetVectorIncrementBits(EffRecordPool *pool, u32 value) {
    pool->drawMode = value;
}

void func_00177B98(EffRecordPool *pool, u32 value) {
    pool->color = value;
}

void func_00177BA0(u8 *work, f32 value) {
    ((EffRecordPool *)work)->scale = value;
}

extern void *memset(void *dst, s32 value, u32 size);
extern u32 sdfCreateAssetWithDrawEntries(void);
extern void func_003332D0(u32 asset, f32 value);

EffRecordPool *effRecordPoolCreateTriad(s32 groups) {
    EffRecordPool *pool;
    u32 handle;
    u32 *block;
    s32 slots;
    s32 first;
    s32 second;
    u32 size;

    slots = groups * 3;
    first = slots * 4;
    second = slots;
    size = (first + second) * 4 + 0x70;
    handle = (u32)sdfAllocGeneralBlock(size);
    block = (u32 *)sdfResourceRetainAddress((SdfMemoryBlock *)handle);
    memset(block, 0, size);
    pool = (EffRecordPool *)(block + (first + second));
    pool->recordBase = (s32)block;
    pool->auxRecordBase = (s32)(block + first);
    pool->color = 0x80808080;
    pool->drawMode = 2;
    pool->count = second;
    pool->buffer = handle;
    pool->scale = 1.0f;
    pool->resource = sdfCreateAssetWithDrawEntries();
    func_003332D0(pool->resource, 1.0f);
    memset(D_00451FF0, 0, 0x2C);
    D_00451FF0->primitive = 0x4000;
    return pool;
}

void effReleaseRecordPoolResourceAndBuffer(EffRecordPool *pool) {
    sdfQueueAssetRelease(pool->resource);
    sdfReleaseResourceAllocation(pool->buffer);
}

/* Submit the position/color pool in triangle batches of at most 48 vertices. */
void effDrawTriangleRecordPool(EffRecordPool *pool)
{
    f32 matrix[16];
    void *packet = sdfAllocPacketAligned(0x20);
    s32 count;
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
    count = pool->count;
    D_00451FF0->colors = (u32 *)pool->auxRecordBase;
    D_00451FF0->positions = (u128 *)pool->recordBase;
    D_00451FF0->unk08 = pool->color;
    D_00451FF0->parameterCount = 0x10;
    D_00451FF0->vertexCount = 0x30;
    D_00451FF0->parameters = NULL;
    while (count >= 0x30) {
        count -= 0x30;
        sdfAppendPacket(packet, func_00167A10(D_00451FF0));
        D_00451FF0->positions += 0x30;
        D_00451FF0->colors += 0x30;
    }
    if (count >= 3) {
        D_00451FF0->parameterCount = count / 3;
        D_00451FF0->vertexCount = count;
        sdfAppendPacket(packet, func_00167A10(D_00451FF0));
    }
    surface = D_003B1318[pool->drawMode];
    surface->submit(surface, packet);
}

s32 effGetGroupRecordByIndex(EffRecordPool *pool, s32 index) {
    return pool->recordBase + index * 0x30;
}

s32 effGetGroupIndexRecord(EffRecordPool *pool, s32 index) {
    return pool->auxRecordBase + index * 0xc;
}

EffRecordPool *effRecordPoolCreate(s32 groups) {
    EffRecordPool *pool;
    u32 handle;
    u32 *block;
    s32 first;
    s32 second;
    u32 size;

    first = groups * 16;
    second = groups * 4;
    size = (first + second) * 4 + 0x70;
    handle = (u32)sdfAllocGeneralBlock(size);
    block = (u32 *)sdfResourceRetainAddress((SdfMemoryBlock *)handle);
    memset(block, 0, size);
    pool = (EffRecordPool *)(block + (first + second));
    pool->recordBase = (s32)block;
    pool->drawMode = 2;
    pool->auxRecordBase = (s32)(block + first);
    pool->count = second;
    pool->buffer = handle;
    pool->scale = 1.0f;
    pool->color = 0x80808080;
    pool->resource = sdfCreateAssetWithDrawEntries();
    func_003332D0(pool->resource, 1.0f);
    memset(D_00451FF0, 0, 0x2C);
    D_00451FF0->primitive = 0x4000;
    return pool;
}

void effReleaseRecordGroupResources(EffRecordPool *pool) {
    sdfQueueAssetRelease(pool->resource);
    sdfReleaseResourceAllocation(pool->buffer);
}

/* Submit the position/color pool in quad batches of at most 32 vertices. */
void effDrawQuadRecordPool(EffRecordPool *pool)
{
    f32 matrix[16];
    void *packet = sdfAllocPacketAligned(0x20);
    s32 count;
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
    count = pool->count;
    D_00451FF0->colors = (u32 *)pool->auxRecordBase;
    D_00451FF0->positions = (u128 *)pool->recordBase;
    D_00451FF0->unk08 = pool->color;
    D_00451FF0->parameterCount = 0x10;
    D_00451FF0->vertexCount = 0x20;
    D_00451FF0->parameters = D_003B1330;
    while (count >= 0x20) {
        count -= 0x20;
        sdfAppendPacket(packet, func_00167A10(D_00451FF0));
        D_00451FF0->positions += 0x20;
        D_00451FF0->colors += 0x20;
    }
    if (count >= 4) {
        D_00451FF0->parameterCount = count / 4 * 2;
        D_00451FF0->vertexCount = count;
        sdfAppendPacket(packet, func_00167A10(D_00451FF0));
    }
    surface = D_003B1378[pool->drawMode];
    surface->submit(surface, packet);
}

s32 effGetRecordGroupElement(EffRecordPool *pool, s32 index) {
    return pool->recordBase + index * 0x40;
}

s32 effGetRecordGroupAuxEntry(EffRecordPool *pool, s32 index) {
    return pool->auxRecordBase + index * 0x10;
}

u8 *effAllocateIdentityMatrixWork(u32 count) {
    u8 *matrix;

    matrix = (u8 *)func_00177760(count);
    EE_MMI_UNIT_MATRIX(matrix);
    return matrix;
}

void func_001781F8(void *work) {
    effReleaseRecordGroupAssetAndHandle(work);
}

void effDrawTransformedRecordPool(EffRecordPool *work)
{
    f32 matrix[16] __attribute__((aligned(16)));
    void *list;
    s32 remaining;
    s32 chunk;
    u64 *packet;
    void *clearList;

    list = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    VU0_COPY_MATRIX(matrix, work->matrix);
    matrix[12] = work->origin[0];
    matrix[13] = work->origin[1];
    matrix[14] = work->origin[2];
    VU0_LOAD_MATRIX(matrix);
    sdfConsAppendVuPacket(list, 0);
    sdfConsAppendAssetPacket(list, work->resource, 0);
    remaining = work->count;
    D_00451FF0->colors = (u32 *)work->auxRecordBase;
    D_00451FF0->positions = (u128 *)work->recordBase;
    D_00451FF0->unk08 = work->color;
    D_00451FF0->parameterCount = 0xF;
    D_00451FF0->vertexCount = 0x19;
    D_00451FF0->parameters = D_003B12C0;
    while (remaining >= 0x19) {
        remaining -= 0x19;
        sdfAppendPacket(list, func_00167A10(D_00451FF0));
        D_00451FF0->positions += 0x19;
        D_00451FF0->colors += 0x19;
    }
    if (remaining >= 5) {
        chunk = remaining / 5;
        D_00451FF0->vertexCount = remaining;
        D_00451FF0->parameterCount = chunk * 3;
        sdfAppendPacket(list, func_00167A10(D_00451FF0));
    }
    if (work->drawMode < 4) {
        D_003B1388[work->drawMode]->submit(D_003B1388[work->drawMode], list);
    } else {
        clearList = sdfAllocPacketAligned(0x20);
        sdfInitPacketList(clearList);
        packet = sdfAllocPacketAligned(0x30);
        packet[4] = 6;
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[5] = 0x42;
        sdfAppendPacket(clearList, packet);
        D_00380248.submit(&D_00380248, clearList);
        packet = sdfAllocPacketAligned(0x30);
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[4] = 0x42;
        packet[5] = 0x42;
        sdfAppendPacket(list, packet);
        D_00380248.submit(&D_00380248, list);
    }
}

s32 effGetExtendedGroupElement(EffRecordPool *pool, s32 index) {
    return pool->recordBase + index * 0x50;
}

s32 effGetExtendedGroupAuxEntry(EffRecordPool *pool, s32 index) {
    return pool->auxRecordBase + index * 0x14;
}

/* Select the record pool's packet submission mode. */
void effSetRecordPoolDrawMode(EffRecordPool *pool, u32 value) {
    pool->drawMode = value;
}

/* Set the packed color word passed to the pool's packet builder. */
void effSetRecordPoolColor(EffRecordPool *pool, u32 value) {
    pool->color = value;
}

void effSetRecordPoolScale(u8 *work, f32 value) {
    ((EffRecordPool *)work)->scale = value;
}

typedef struct PcpScatterRes PcpScatterRes;
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

PcpScatterRadialWork *func_001784F8(params, resource, particleParams)
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
        particle->age = ageOffset - effMiscRand(D_003AA868) % delaySpread;
        if ((i + 1) % segments == 0) {
            ageOffset -= delaySpread;
        }
    }
    return work;
}
