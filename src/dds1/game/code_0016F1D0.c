#include "common.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

typedef struct {
    u8 pad0[8];
    f32 value;
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
    u32 handle68;
    u32 handle6C;
    u8 pad70[8];
    u32 handle78;
    u32 handle7C;
} EffectRecordGroup;

extern u64 effParamTableGetBlock(u64, u64);

extern void func_001705A0();

extern s32 effMultiplyPackedColors(s32 color, s32 param);

#define EFFECT_RING_START_ANGLE (-1.5707963f)
#define EFFECT_RING_FULL_TURN (6.2831853f)

/* Ring (fan) effect: a copy of the 0x58-byte parameter block followed by
 * `count` vertices spread evenly around the circle from -pi/2. */
typedef struct EffectRingHeader {
    u8 head[0x58];
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

extern u32 func_002D03F8(s32);
extern u32 sdfResourceRetainAddress(u32);
extern u8 *func_00170558(u32);
extern s32 effMiscRand(void *);
extern u8 D_0034DF38[];

/* Allocate and initialize a circular fan, with randomized per-vertex offsets. */
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

    handle = func_002D03F8(source->count * 16 + 0x80);
    block = (EffectRingBlock *)sdfResourceRetainAddress(handle);
    ring = &block->header;
    memcpy(ring, source, 0x58);
    ring->vertices = &block->vertices[0];
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
    ring->matrix = func_00170558(ring->count);
    *(f32 *)(ring->matrix + 0x5C) = 1.0f;
    *(u32 *)(ring->matrix + 0x50) = ring->unk54;
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
    u64 block;

    block = effParamTableGetBlock(table, 0);
    effCreateRingFan(block);
}

void func_0016F440(EffectRing *ring) {
    effCreateRingFan(ring);
}

void effReleaseRingResources(EffectRecordGroup *group) {
    func_001705A0(group->handle7C);
    func_002D0918(group->handle78);
}

void effCopyRingVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effSetRingColor(EffectRecordGroup *group, u32 records) {
    group->records = (u8 *)records;
}

void func_0016F4A0(u8 *work, f32 value) {
    *(f32 *)(work + 0x64) = value;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void effCopyRingTransformMatrix(void *work, void *src) {
    VU0_LOAD_MATRIX(src);
    VU0_STORE_MATRIX(*(void **)((u8 *)work + 0x7C));
}

void effFlashWriteRingColorSlots(u8 *work, s32 index, s32 param) {
    s32 slot;
    s32 rgb1;
    s32 rgb2;

    slot = func_00170870(*(void **)(work + 0x7C), index);
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

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_0016F5C8);

void effAdvanceVectorRecord(EffectRecordGroup *group, s32 index) {
    EffectVectorRecord *record;

    record = &group->vectors[index];
    record->value = record->value + group->increment;
}

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_0016F7B0);

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_0016FB08);

void effReleaseRecordGroupAssetAndHandle(EffectRecordGroup *group) {
    sdfQueueAssetRelease(group->handle68);
    func_002D0918(group->handle6C);
}

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_0016FC58);

s32 effGetIndexedEffectGroupRecord(EffectRecordGroup *group, s32 index) {
    return (s32)group->records + index * 0x50;
}

s32 effGetIndexedEffectGroupIndexEntry(EffectRecordGroup *group, s32 index) {
    return (s32)group->indices + index * 0x14;
}

void effSetVectorIncrementBits(EffectRecordGroup *group, u32 incrementBits) {
    *(u32 *)&group->increment = incrementBits;
}

void func_0016FF40(EffectRecordGroup *group, u32 value) {
    group->value54 = value;
}

void func_0016FF48(u8 *work, f32 value) {
    *(f32 *)(work + 0x5C) = value;
}

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_0016FF50);

void effReleaseRecordPoolResourceAndBuffer(EffectRecordGroup *group) {
    sdfQueueAssetRelease(group->handle68);
    func_002D0918(group->handle6C);
}

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_00170078);

s32 effGetGroupRecordByIndex(EffectRecordGroup *group, s32 index) {
    return (s32)group->records + index * 0x30;
}

s32 effGetGroupIndexRecord(EffectRecordGroup *group, s32 index) {
    return (s32)group->indices + index * 0xc;
}

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_00170250);

void func_00170350(EffectRecordGroup *group) {
    sdfQueueAssetRelease(group->handle68);
    func_002D0918(group->handle6C);
}

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_00170380);

s32 func_00170538(EffectRecordGroup *group, s32 index) {
    return (s32)group->records + index * 0x40;
}

s32 func_00170548(EffectRecordGroup *group, s32 index) {
    return (s32)group->indices + index * 0x10;
}

u8 *func_00170558(u32 count) {
    u8 *matrix = func_0016FB08(count);

    EE_MMI_UNIT_MATRIX(matrix);
    return matrix;
}

void func_001705A0(u32 id) {
    effReleaseRecordGroupAssetAndHandle(id);
}

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_001705B8);

s32 func_00170858(EffectRecordGroup *group, s32 index) {
    return (s32)group->records + index * 0x50;
}

s32 func_00170870(EffectRecordGroup *group, s32 index) {
    return (s32)group->indices + index * 0x14;
}

void func_00170888(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x50) = arg1;
}

void func_00170890(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x54) = arg1;
}

void func_00170898(u8 *work, f32 value) {
    *(f32 *)(work + 0x5C) = value;
}

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_001708A0);
