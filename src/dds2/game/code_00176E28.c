#include "common.h"

#include "pcp_vu0.h"
#include "ee_mmi.h"

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
    u8 pad00[0x50];
    u32 settingA;  /* 0x50 */
    u32 settingB;  /* 0x54 */
    s32 count;     /* 0x58 */
    f32 scale;     /* 0x5C */
    s32 recordBase;    /* 0x60: address of stride-dependent records */
    s32 auxRecordBase; /* 0x64: address of stride-dependent auxiliary records */
    u32 resource;   /* 0x68: released by sdfQueueAssetRelease */
    u32 buffer;     /* 0x6C: freed by func_003297C8 */
} EffRecordPool;

#define EFFECT_RING_START_ANGLE (-1.5707963f)

#define EFFECT_RING_FULL_TURN (6.2831853f)

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
    u8 pad20[0x10];
    f32 param30;
    f32 param34;
    f32 param38;
    u8 pad3C[0x14];
    u32 unk50;
    u32 unk54;
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

extern u32 func_003292A8(s32);

extern u32 sdfResourceRetainAddress(u32);

extern u8 *effAllocateIdentityMatrixWork(u32);

extern s32 effMiscRand(void *);

extern u8 D_003AA868[];
extern s32 func_001784B0(EffRecordPool *pool, s32 index);
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

    handle = func_003292A8(source->count * 16 + 0x80);
    block = (EffectRingBlock *)sdfResourceRetainAddress(handle);
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
    *(f32 *)(ring->matrix + 0x5C) = 1.0f;
    *(u32 *)(ring->matrix + 0x50) = ring->unk54;
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
    func_003297C8(ring->handle);
}

void effCopyRingVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effSetRingColor(EffectRing *ring, u32 color) {
    ring->color = color;
}

void func_001770F8(EffectRing *ring, f32 scale) {
    ring->scale = scale;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effCopyRingTransformMatrix(EffectRing *ring, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX(ring->matrix);
}

void effFlashWriteRingColorSlots(u8 *work, s32 index, s32 param) {
    s32 slot;
    s32 rgb1;
    s32 rgb2;

    slot = func_001784C8(*(void **)(work + 0x7C), index);
    rgb1 = *(u32 *)(work + 0x28) & 0xFFFFFF;
    rgb2 = *(u32 *)(work + 0x2C) & 0xFFFFFF;
    *(s32 *)(slot + 0) = effMultiplyPackedColors(rgb2, param);
    *(s32 *)(slot + 4) = effMultiplyPackedColors(rgb2, param);
    if (index & 1) {
        *(s32 *)(slot + 8) = effMultiplyPackedColors(0x80000000, param);
        *(s32 *)(slot + 0xC) = effMultiplyPackedColors(rgb1 | 0xFF000000, param);
        *(s32 *)(slot + 0x10) = effMultiplyPackedColors(0x80000000, param);
    } else {
        *(s32 *)(slot + 8) = effMultiplyPackedColors(0xFF000000, param);
        *(s32 *)(slot + 0xC) = effMultiplyPackedColors(rgb1 | 0x40000000, param);
        *(s32 *)(slot + 0x10) = effMultiplyPackedColors(0xFF000000, param);
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
void func_00177220(EffectArcQuadWork *work, s32 index) {
    EffectArcQuadPart *part = &work->parts[index];
    f32 *quad = (f32 *)func_001784B0(work->resourceHandle, index);
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
    func_003297C8(pool->buffer);
}

INCLUDE_ASM(const s32, "game/code_00176E28", func_001778B0);

s32 effGetIndexedEffectGroupRecord(EffRecordPool *pool, s32 index) {
    return pool->recordBase + index * 0x50;
}

s32 effGetIndexedEffectGroupIndexEntry(EffRecordPool *pool, s32 index) {
    return pool->auxRecordBase + index * 0x14;
}

void effSetVectorIncrementBits(EffRecordPool *pool, u32 value) {
    pool->settingA = value;
}

void func_00177B98(EffRecordPool *pool, u32 value) {
    pool->settingB = value;
}

void func_00177BA0(u8 *work, f32 value) {
    *(f32 *)(work + 0x5C) = value;
}

extern void *memset(void *dst, s32 value, u32 size);
extern u32 sdfCreateAssetWithDrawEntries(void);
extern void func_003332D0(u32 asset, f32 value);
extern u8 D_00451FF0[];

EffRecordPool *func_00177BA8(s32 groups) {
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
    handle = func_003292A8(size);
    block = (u32 *)sdfResourceRetainAddress(handle);
    memset(block, 0, size);
    pool = (EffRecordPool *)(block + (first + second));
    pool->recordBase = (s32)block;
    pool->auxRecordBase = (s32)(block + first);
    pool->settingB = 0x80808080;
    pool->settingA = 2;
    pool->count = second;
    pool->buffer = handle;
    pool->scale = 1.0f;
    pool->resource = sdfCreateAssetWithDrawEntries();
    func_003332D0(pool->resource, 1.0f);
    memset(D_00451FF0, 0, 0x2C);
    *(u16 *)(D_00451FF0 + 4) = 0x4000;
    return pool;
}

void effReleaseRecordPoolResourceAndBuffer(EffRecordPool *pool) {
    sdfQueueAssetRelease(pool->resource);
    func_003297C8(pool->buffer);
}

INCLUDE_ASM(const s32, "game/code_00176E28", func_00177CD0);

s32 effGetGroupRecordByIndex(EffRecordPool *pool, s32 index) {
    return pool->recordBase + index * 0x30;
}

s32 effGetGroupIndexRecord(EffRecordPool *pool, s32 index) {
    return pool->auxRecordBase + index * 0xc;
}

EffRecordPool *func_00177EA8(s32 groups) {
    EffRecordPool *pool;
    u32 handle;
    u32 *block;
    s32 first;
    s32 second;
    u32 size;

    first = groups * 16;
    second = groups * 4;
    size = (first + second) * 4 + 0x70;
    handle = func_003292A8(size);
    block = (u32 *)sdfResourceRetainAddress(handle);
    memset(block, 0, size);
    pool = (EffRecordPool *)(block + (first + second));
    pool->recordBase = (s32)block;
    pool->settingA = 2;
    pool->auxRecordBase = (s32)(block + first);
    pool->count = second;
    pool->buffer = handle;
    pool->scale = 1.0f;
    pool->settingB = 0x80808080;
    pool->resource = sdfCreateAssetWithDrawEntries();
    func_003332D0(pool->resource, 1.0f);
    memset(D_00451FF0, 0, 0x2C);
    *(u16 *)(D_00451FF0 + 4) = 0x4000;
    return pool;
}

void func_00177FA8(EffRecordPool *pool) {
    sdfQueueAssetRelease(pool->resource);
    func_003297C8(pool->buffer);
}

INCLUDE_ASM(const s32, "game/code_00176E28", func_00177FD8);

s32 func_00178190(EffRecordPool *pool, s32 index) {
    return pool->recordBase + index * 0x40;
}

s32 func_001781A0(EffRecordPool *pool, s32 index) {
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

INCLUDE_ASM(const s32, "game/code_00176E28", func_00178210);

s32 func_001784B0(EffRecordPool *pool, s32 index) {
    return pool->recordBase + index * 0x50;
}

s32 func_001784C8(EffRecordPool *pool, s32 index) {
    return pool->auxRecordBase + index * 0x14;
}

void func_001784E0(EffRecordPool *pool, u32 value) {
    pool->settingA = value;
}

void func_001784E8(EffRecordPool *pool, u32 value) {
    pool->settingB = value;
}

void func_001784F0(u8 *work, f32 value) {
    *(f32 *)(work + 0x5C) = value;
}

INCLUDE_ASM(const s32, "game/code_00176E28", func_001784F8);
