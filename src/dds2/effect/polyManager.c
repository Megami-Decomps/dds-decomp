#include "common.h"
#include "pcp_vu0.h"

typedef struct PolyTransform {
    u8 pad0[0xC8];
    f32 scaleC8;
    f32 scaleCC;
    f32 scaleD0;
    u8 padD4[8];
    f32 scaleDC;
} PolyTransform;

typedef struct PolyEntryPool {
    u8 pad00[0x10];
    u32 entryCount; /* 0x10 */
    u32 sentinel; /* 0x14 */
    u8 pad18[0x50];
    u32 stateA; /* 0x68 */
    u32 stateB; /* 0x6C */
    u8 pad70[0x88];
    s32 *records; /* 0xF8: five words per entry; -0xFFFFFF marks inactive */
} PolyEntryPool;

/* Point buffer of one strip; `count` is copied from the strip's own count. */
typedef struct {
    f32 *points; /* 0x0 */
    u32 unk4;     /* 0x4 */
    s32 count;    /* 0x8 */
    u32 unkC;     /* 0xC */
    u32 unk10;    /* 0x10 */
} PolyStripEntry; /* 0x14 bytes */

typedef struct {
    u8 pad00[8];             /* 0x0 */
    s32 count;               /* 0x8 */
    u8 pad0C[8];             /* 0xC */
    PolyStripEntry *entries; /* 0x14 */
} PolyStrip;

/* Node with a scale at 0xD0 and its strip at 0xDC. */
typedef struct {
    u8 pad00[0xD0];     /* 0x0 */
    f32 scale;          /* 0xD0 */
    u8 padD4[8];        /* 0xD4 */
    PolyStrip *strip;   /* 0xDC */
} PolyScaledStripNode;

extern f32 sdfEvaluateCosineViaSinePhaseShift(f32 angle);
extern f32 sdfSinPoly(f32 angle);
extern void func_003364B8(f32 angle);
extern void func_00336818(f32 angle);
extern void sdfMultiplyVuMatrixInPlace(void);

void effPolyDestroyWork(u32 work) {
    parReleaseCellSystem(*(u32 *)((s32)work + 0xdc));
    sdfReleaseChipBlock(work);
}

INCLUDE_ASM(const s32, "effect/polyManager", func_00165600);

void polyScaleTransformPair(float factor, PolyTransform *transform) {
    transform->scaleCC = transform->scaleCC * factor;
    transform->scaleD0 = transform->scaleD0 * factor;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_00165690);

void polyFinishAndReleaseNodeHandle(s32 work) {
    func_00165690();
    parPrependCellNode(*(u32 *)(work + 0xdc));
}

INCLUDE_ASM(const s32, "effect/polyManager", func_00165860);

/* Push each point pair of the strip entry apart along its own direction, by the node's scale. */
void polyStripPushPairsApart(PolyScaledStripNode *obj, s32 index) {
    PolyStrip *strip = obj->strip;
    PolyStripEntry *entry = &strip->entries[index];
    f32 scale[4];
    f32 *p;
    s32 pairs;
    s32 i;

    p = entry->points;
    pairs = strip->count / 2;
    scale[0] = scale[1] = scale[2] = obj->scale;
    VU0_LOAD_VF(vf12, scale);
    for (i = 0; i < pairs; i++) {
        VU0_LOAD_VF(vf10, p + 4);
        VU0_LOAD_VF(vf11, p);
        VU0_SUB(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10();
        VU0_MOVE_VF(vf11, vf12);
        VU0_MUL(vf10, vf10, vf11);
        VU0_MOVE_VF(vf11, vf10);
        VU0_LOAD_VF(vf10, p);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, p);
        VU0_LOAD_VF(vf10, p + 4);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, p + 4);
        p += 8;
    }
}

INCLUDE_ASM(const s32, "effect/polyManager", func_00165A78);

INCLUDE_ASM(const s32, "effect/polyManager", func_00165B98);

void polyReleaseBandNodeResources(s32 work) {
    parReleaseCellSystem(*(u32 *)(work + 0xf0));
    func_003297C8(*(u32 *)(work + 0xf8));
}

INCLUDE_ASM(const s32, "effect/polyManager", func_00165CF0);

INCLUDE_ASM(const s32, "effect/polyManager", func_00165D38);

/* Band node: an origin, a transform, the ring's segment count and a strip at 0xF0. */
typedef struct {
    f32 origin[4];      /* 0x0 */
    u8 pad10[0x10];     /* 0x10 */
    f32 matrix[16];     /* 0x20 */
    u8 pad60[0x64];     /* 0x60 */
    u32 segments;       /* 0xC4 */
    f32 unkC8;          /* 0xC8 */
    u8 padCC[0x24];     /* 0xCC */
    PolyStrip *strip;   /* 0xF0 */
} PolyBand;

/* Lay the band's point pairs of strip entry `index` around the ring: an inner and an outer radius, moved to the origin. */
void polyBandLayoutRing(PolyBand *obj, s32 index, f32 width)
{
    PolyStrip *strip = obj->strip;
    PolyStripEntry *entry = &strip->entries[index];
    f32 dir[4];
    f32 wide[4];
    f32 narrow[4];
    f32 step;
    f32 angle;
    f32 *out;
    f32 *first;
    s32 pairs;
    s32 i;

    entry->count = strip->count;
    out = entry->points;
    pairs = strip->count / 2;
    VEC3_SPLAT(wide, width);
    VEC3_SPLAT(narrow, width + obj->unkC8);
    step = 3.14159265f * 2.0f / (f32)obj->segments;
    VU0_LOAD_MATRIX(obj->matrix);
    angle = 0.0f;
    VU0_LOAD_VF(vf12, obj->origin);
    for (i = 0; i < pairs - 1; i++) {
        dir[0] = sdfEvaluateCosineViaSinePhaseShift(angle);
        dir[1] = 0;
        dir[2] = sdfSinPoly(angle);
        VU0_LOAD_VF(vf10, dir);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, dir);
        VU0_LOAD_VF(vf11, wide);
        VU0_MUL(vf10, vf10, vf11);
        VU0_ADD(vf10, vf10, vf12);
        VU0_STORE_VF(vf10, out + 4);
        VU0_LOAD_VF(vf10, dir);
        VU0_LOAD_VF(vf11, narrow);
        VU0_MUL(vf10, vf10, vf11);
        VU0_ADD(vf10, vf10, vf12);
        VU0_STORE_VF(vf10, out);
        out += 8;
        angle += step;
    }
    first = entry->points;
    PCP_COPY_VECTOR(out, first);
    PCP_COPY_VECTOR(out + 4, first + 4);
}
typedef struct {
    u32 unk0;
    f32 width;
} PolyRing2Rec; /* 8 bytes */

typedef struct {
    f32 origin[4];      /* 0x0 */
    u8 pad10[0x10];     /* 0x10 */
    f32 matrix[16];     /* 0x20 */
    u8 pad60[0x64];     /* 0x60 */
    u32 segments;       /* 0xC4 */
    u8 padC8[0x14];     /* 0xC8 */
    f32 lift;           /* 0xDC */
    u8 padE0[0x10];     /* 0xE0 */
    PolyStrip *strip;   /* 0xF0 */
    PolyRing2Rec *recs; /* 0xF4 */
} PolyRing2;

/* Lay a ring of point pairs for strip entry `index`, scaled per axis and lifted along y. */
void polyStripBuildScaledRing(PolyRing2 *obj, s32 index) {
    PolyStrip *strip = obj->strip;
    PolyRing2Rec *rec = &obj->recs[index];
    PolyStripEntry *entry = &strip->entries[index];
    f32 dir[4];
    f32 scale[4];
    f32 lift[4];
    f32 step;
    f32 angle;
    f32 *out;
    f32 *first;
    s32 pairs;
    s32 i;

    entry->count = strip->count;
    out = entry->points;
    pairs = strip->count >> 1;
    scale[2] = scale[1] = scale[0] = rec->width;
    lift[1] = obj->lift;
    lift[2] = lift[0] = 0;
    step = 3.14159265f * 2.0f / (f32)obj->segments;
    VU0_LOAD_MATRIX(obj->matrix);
    angle = 0.0f;
    for (i = 0; i < pairs - 1; i++) {
        dir[0] = sdfEvaluateCosineViaSinePhaseShift(angle);
        dir[1] = 0;
        dir[2] = sdfSinPoly(angle);
        VU0_LOAD_VF(vf10, dir);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_LOAD_VF(vf11, scale);
        VU0_MUL(vf10, vf10, vf11);
        VU0_LOAD_VF(vf11, lift);
        VU0_APPLY_MATRIX(vf11, vf11);
        VU0_ADD(vf10, vf10, vf11);
        VU0_MOVE_VF(vf12, vf10);
        VU0_LOAD_VF(vf11, out + 4);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, out + 4);
        VU0_MOVE_VF(vf10, vf12);
        VU0_LOAD_VF(vf11, out);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, out);
        out += 8;
        angle += step;
    }
    first = entry->points;
    PCP_COPY_VECTOR(out, first);
    PCP_COPY_VECTOR(out + 4, first + 4);
}

void effPolyScaleFourComponents(float factor, PolyTransform *transform) {
    transform->scaleC8 = transform->scaleC8 * factor;
    transform->scaleDC = transform->scaleDC * factor;
    transform->scaleCC = transform->scaleCC * factor;
    transform->scaleD0 = transform->scaleD0 * factor;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_001661C8);

INCLUDE_ASM(const s32, "effect/polyManager", func_00166350);

void polyReleaseNodeCellSystemAndBuffer(s32 work) {
    parReleaseCellSystem(*(u32 *)(work + 0xe0));
    func_003297C8(*(u32 *)(work + 0xe8));
}

INCLUDE_ASM(const s32, "effect/polyManager", func_001664A8);

INCLUDE_ASM(const s32, "effect/polyManager", func_001664F0);

typedef struct {
    s32 time;   /* 0x0 */
    u32 radius; /* 0x4 */
} PolyArcRec; /* 8 bytes */

typedef struct {
    f32 origin[4];      /* 0x0 */
    u8 pad10[4];        /* 0x10 */
    s32 duration;       /* 0x14 */
    u8 pad18[8];        /* 0x18 */
    f32 matrix[16];     /* 0x20 */
    u8 pad60[0x64];     /* 0x60 */
    u32 segments;       /* 0xC4 */
    f32 width;          /* 0xC8 */
    u8 padCC[0x14];     /* 0xCC */
    PolyStrip *strip;   /* 0xE0 */
    PolyArcRec *recs;   /* 0xE4 */
} PolyArc;

/* Lay a ring of point pairs for strip entry `index` on an arc of the node: the inner row sits at the arc's sine radius, the outer row `width` further out. */
void func_00166590(PolyArc *obj, s32 index) {
    PolyStrip *strip = obj->strip;
    PolyArcRec *rec = &obj->recs[index];
    PolyStripEntry *entry = &strip->entries[index];
    f32 dir[4];
    f32 ring[4];
    f32 radius;
    f32 inner;
    f32 drop;
    f32 width;
    f32 step;
    f32 angle;
    f32 *out;
    f32 *first;
    s32 pairs;
    s32 i;

    entry->count = strip->count;
    out = entry->points;
    pairs = strip->count >> 1;
    radius = rec->radius;
    angle = (f32)rec->time / (f32)obj->duration * 3.14159265f;
    inner = radius * sdfSinPoly(angle);
    drop = radius * sdfEvaluateCosineViaSinePhaseShift(angle) - radius;
    step = 3.14159265f * 2.0f / (f32)obj->segments;
    VU0_LOAD_MATRIX(obj->matrix);
    width = obj->width;
    angle = 0.0f;
    VU0_LOAD_VF(vf12, obj->origin);
    for (i = 0; i < pairs - 1; i++) {
        dir[0] = ring[0] = sdfEvaluateCosineViaSinePhaseShift(angle);
        dir[2] = sdfSinPoly(angle);
        ring[1] = drop;
        ring[2] = dir[2] * (inner + width);
        ring[0] *= inner + width;
        VU0_LOAD_VF(vf10, ring);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, ring);
        dir[1] = drop;
        dir[0] *= inner;
        dir[2] *= inner;
        VU0_LOAD_VF(vf10, dir);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_ADD(vf10, vf10, vf12);
        VU0_STORE_VF(vf10, out + 4);
        VU0_LOAD_VF(vf10, ring);
        VU0_ADD(vf10, vf10, vf12);
        VU0_STORE_VF(vf10, out);
        out += 8;
        angle += step;
    }
    first = entry->points;
    PCP_COPY_VECTOR(out, first);
    PCP_COPY_VECTOR(out + 4, first + 4);
}

void polyScaleTransformFirstComponent(float factor, PolyTransform *transform) {
    transform->scaleCC = transform->scaleCC * factor;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_001667F8);

INCLUDE_ASM(const s32, "effect/polyManager", func_00166980);

void polyReleaseCellBoundNodeResources(s32 work) {
    parReleaseCellSystem(*(u32 *)(work + 0xf4));
    func_003297C8(*(u32 *)(work + 0xfc));
}

INCLUDE_ASM(const s32, "effect/polyManager", func_00166AE0);

INCLUDE_ASM(const s32, "effect/polyManager", func_00166B40);

typedef struct {
    u8 pad00[4];
    f32 unk04;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
} PolyRec; /* 0x14 bytes */

/* Band node with its own rotation records at 0xF8 and a strip at 0xF4. */
typedef struct {
    f32 origin[4];      /* 0x0 */
    u8 pad10[0x10];     /* 0x10 */
    f32 matrix[16];     /* 0x20 */
    u8 pad60[0x64];     /* 0x60 */
    u32 segments;       /* 0xC4 */
    f32 unkC8;          /* 0xC8 */
    u8 padCC[0x18];     /* 0xCC */
    f32 unkE4;          /* 0xE4 */
    u8 padE8[0xC];      /* 0xE8 */
    PolyStrip *strip;   /* 0xF4 */
    PolyRec *recs;      /* 0xF8 */
} PolyBandC;

/* Same ring as polyBandLayoutRing, but the record's rotation is applied through the second matrix bank first. */
void polyBandLayoutRingRotated(PolyBandC *obj, s32 index)
{
    PolyStrip *strip = obj->strip;
    PolyRec *rec = &obj->recs[index];
    PolyStripEntry *entry = &strip->entries[index];
    f32 dir[4];
    f32 wide[4];
    f32 narrow[4];
    f32 step;
    f32 angle;
    f32 *out;
    f32 *first;
    s32 pairs;
    s32 i;

    entry->count = strip->count;
    out = entry->points;
    pairs = strip->count >> 1;
    func_003364B8(rec->unk0C);
    func_00336818(rec->unk10);
    sdfMultiplyVuMatrixInPlace();
    rec->unk10 += obj->unkE4 * (3.14159265f / 180.0f);
    angle = rec->unk04;
    rec->unk04 = angle + rec->unk08;
    step = 3.14159265f * 2.0f / (f32)obj->segments;
    VU0_LOAD_MATRIX_B(obj->matrix);
    sdfMultiplyVuMatrixInPlace();
    VEC3_SPLAT(wide, angle);
    VEC3_SPLAT(narrow, angle + obj->unkC8);
    angle = 0.0f;
    VU0_LOAD_VF(vf12, obj->origin);
    for (i = 0; i < pairs - 1; i++) {
        dir[0] = sdfEvaluateCosineViaSinePhaseShift(angle);
        dir[1] = 0;
        dir[2] = sdfSinPoly(angle);
        VU0_LOAD_VF(vf10, dir);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, dir);
        VU0_LOAD_VF(vf11, wide);
        VU0_MUL(vf10, vf10, vf11);
        VU0_ADD(vf10, vf10, vf12);
        VU0_STORE_VF(vf10, out + 4);
        VU0_LOAD_VF(vf10, dir);
        VU0_LOAD_VF(vf11, narrow);
        VU0_MUL(vf10, vf10, vf11);
        VU0_ADD(vf10, vf10, vf12);
        VU0_STORE_VF(vf10, out);
        out += 8;
        angle += step;
    }
    first = entry->points;
    PCP_COPY_VECTOR(out, first);
    PCP_COPY_VECTOR(out + 4, first + 4);
}
void polyScaleNodeFloatingParameters(float factor, PolyTransform *transform) {
    transform->scaleCC = transform->scaleCC * factor;
    transform->scaleD0 = transform->scaleD0 * factor;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_00166EC0);

/* Reset active records but leave inactive sentinel entries untouched. */
void polyResetEntries(PolyEntryPool *pool) {
    u32 entryCount;
    s32 *record;
    u32 index;

    entryCount = pool->entryCount;
    index = 0;
    pool->sentinel = 0xfffffff;
    pool->stateB = 0;
    pool->stateA = 0;
    record = pool->records;
    if (entryCount != 0) {
        do {
            if (*record != -0xffffff) {
                *record = 0xffffff0;
            }
            index = index + 1;
            record = record + 5;
        } while (index < entryCount);
    }
}
