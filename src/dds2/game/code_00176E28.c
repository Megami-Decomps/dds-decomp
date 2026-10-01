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
    u8 pad58[8];
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

INCLUDE_ASM(const s32, "game/code_00176E28", func_00177220);

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

INCLUDE_ASM(const s32, "game/code_00176E28", func_00177BA8);

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

INCLUDE_ASM(const s32, "game/code_00176E28", func_00177EA8);

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
