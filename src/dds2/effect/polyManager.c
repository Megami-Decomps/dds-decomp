#include "common.h"
#include "pcp_vu0.h"

enum {
    POLY_INACTIVE_ENTRY_AGE = -0xFFFFFF,
    POLY_RESET_ENTRY_AGE = 0xFFFFFF0,
    POLY_RESET_DURATION = 0xFFFFFFF
};

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
    u32 duration; /* 0x14: lifetime used by native age comparisons */
    u8 pad18[0x50];
    u32 startColorRampFrames; /* 0x68: color interpolation window from the start */
    u32 endColorRampFrames; /* 0x6C: color interpolation window before duration */
    u8 pad70[0x88];
    s32 *records; /* 0xF8: five words per entry; -0xFFFFFF marks inactive */
} PolyEntryPool;

/* Point buffer of one strip; `count` is copied from the strip's own count. */
typedef struct {
    f32 *points; /* 0x0 */
    u32 colorBufferAddress; /* 0x4: per-point colors owned by the cell system */
    s32 count;    /* 0x8 */
    u32 unkC;     /* 0xC */
    u32 color;    /* 0x10: packed color set by the native color-ramp updater */
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

/* Release the cell system, then the node itself; retain the existing raw address view. */
void effPolyDestroyWork(u32 work) {
    parReleaseCellSystem(*(u32 *)((s32)work + 0xdc));
    sdfReleaseChipBlock(work);
}

INCLUDE_ASM(const s32, "effect/polyManager", func_00165600);

/* Multiply both existing floating parameters without assigning them axis-specific roles. */
void polyScaleTransformPair(float factor, PolyTransform *transform) {
    transform->scaleCC = transform->scaleCC * factor;
    transform->scaleD0 = transform->scaleD0 * factor;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_00165690);

/* Run the shared finish step, then enqueue this node's cell system. */
void polyFinishAndReleaseNodeHandle(s32 work) {
    func_00165690();
    parPrependCellNode(*(u32 *)(work + 0xdc));
}

INCLUDE_ASM(const s32, "effect/polyManager", func_00165860);

/* Apply the same displacement to both points of each pair along their separation direction. */
void polyStripPushPairsApart(PolyScaledStripNode *node, s32 index) {
    PolyStrip *strip = node->strip;
    PolyStripEntry *entry = &strip->entries[index];
    f32 scale[4];
    f32 *p;
    s32 pairs;
    s32 i;

    p = entry->points;
    pairs = strip->count / 2;
    scale[0] = scale[1] = scale[2] = node->scale;
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

/* Release the band's cell system and backing allocation handle, not the node itself. */
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
    f32 radialWidth;    /* 0xC8: added to the supplied radius for the other row */
    u8 padCC[0x24];     /* 0xCC */
    PolyStrip *strip;   /* 0xF0 */
} PolyBand;

/* Write radii radius and radius + radialWidth, then close the strip with its first pair. */
void polyBandLayoutRing(PolyBand *band, s32 index, f32 radius)
{
    PolyStrip *strip = band->strip;
    PolyStripEntry *entry = &strip->entries[index];
    f32 dir[4];
    f32 baseRadiusVector[4];
    f32 offsetRadiusVector[4];
    f32 step;
    f32 angle;
    f32 *out;
    f32 *first;
    s32 pairs;
    s32 i;

    entry->count = strip->count;
    out = entry->points;
    pairs = strip->count / 2;
    VEC3_SPLAT(baseRadiusVector, radius);
    VEC3_SPLAT(offsetRadiusVector, radius + band->radialWidth);
    step = 3.14159265f * 2.0f / (f32)band->segments;
    VU0_LOAD_MATRIX(band->matrix);
    angle = 0.0f;
    VU0_LOAD_VF(vf12, band->origin);
    for (i = 0; i < pairs - 1; i++) {
        dir[0] = sdfEvaluateCosineViaSinePhaseShift(angle);
        dir[1] = 0;
        dir[2] = sdfSinPoly(angle);
        VU0_LOAD_VF(vf10, dir);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, dir);
        VU0_LOAD_VF(vf11, baseRadiusVector);
        VU0_MUL(vf10, vf10, vf11);
        VU0_ADD(vf10, vf10, vf12);
        VU0_STORE_VF(vf10, out + 4);
        VU0_LOAD_VF(vf10, dir);
        VU0_LOAD_VF(vf11, offsetRadiusVector);
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
    u32 age; /* Native update interprets these stored bits as a signed age. */
    f32 radius;
} PolyLiftedRingRecord; /* 8 bytes */

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
    PolyLiftedRingRecord *recs; /* 0xF4 */
} PolyLiftedRing;

/* Add a uniformly scaled ring and local-y lift to existing point pairs, then close the strip. */
void polyStripBuildScaledRing(PolyLiftedRing *ring, s32 index) {
    PolyStrip *strip = ring->strip;
    PolyLiftedRingRecord *rec = &ring->recs[index];
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
    scale[2] = scale[1] = scale[0] = rec->radius;
    lift[1] = ring->lift;
    lift[2] = lift[0] = 0;
    step = 3.14159265f * 2.0f / (f32)ring->segments;
    VU0_LOAD_MATRIX(ring->matrix);
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

/* Scale the four existing parameters in retail order; they are not proven XYZW components. */
void effPolyScaleFourComponents(float factor, PolyTransform *transform) {
    transform->scaleC8 = transform->scaleC8 * factor;
    transform->scaleDC = transform->scaleDC * factor;
    transform->scaleCC = transform->scaleCC * factor;
    transform->scaleD0 = transform->scaleD0 * factor;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_001661C8);

INCLUDE_ASM(const s32, "effect/polyManager", func_00166350);

/* Release the arc's cell system and backing allocation handle, leaving the node alive. */
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
void polyUpdateArcRingStripPoints(PolyArc *obj, s32 index) {
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

/* Scale only the first member of the existing floating pair. */
void polyScaleTransformFirstComponent(float factor, PolyTransform *transform) {
    transform->scaleCC = transform->scaleCC * factor;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_001667F8);

INCLUDE_ASM(const s32, "effect/polyManager", func_00166980);

/* Release the rotating band's cell system and backing allocation handle, not the node. */
void polyReleaseCellBoundNodeResources(s32 work) {
    parReleaseCellSystem(*(u32 *)(work + 0xf4));
    func_003297C8(*(u32 *)(work + 0xfc));
}

INCLUDE_ASM(const s32, "effect/polyManager", func_00166AE0);

INCLUDE_ASM(const s32, "effect/polyManager", func_00166B40);

typedef struct {
    u8 ageBytes[4]; /* Native updates use this signed age word; retain its byte-array view. */
    f32 radius;
    f32 radiusStep;
    f32 rotationXRadians;
    f32 rotationYRadians;
} PolyRotatingBandRecord; /* 0x14 bytes */

/* Band node with its own rotation records at 0xF8 and a strip at 0xF4. */
typedef struct {
    f32 origin[4];      /* 0x0 */
    u8 pad10[0x10];     /* 0x10 */
    f32 matrix[16];     /* 0x20 */
    u8 pad60[0x64];     /* 0x60 */
    u32 segments;       /* 0xC4 */
    f32 radialWidth;    /* 0xC8: separation between the two radius rows */
    u8 padCC[0x18];     /* 0xCC */
    f32 rotationStepDegrees; /* 0xE4: converted to radians before advancing Y rotation */
    u8 padE8[0xC];      /* 0xE8 */
    PolyStrip *strip;   /* 0xF4 */
    PolyRotatingBandRecord *recs; /* 0xF8 */
} PolyRotatingBand;

/* Build the paired ring using current radius/rotation, then retain their next-step values. */
void polyBandLayoutRingRotated(PolyRotatingBand *obj, s32 index)
{
    PolyStrip *strip = obj->strip;
    PolyRotatingBandRecord *rec = &obj->recs[index];
    PolyStripEntry *entry = &strip->entries[index];
    f32 dir[4];
    f32 baseRadiusVector[4];
    f32 offsetRadiusVector[4];
    f32 step;
    f32 radiusOrAngle; /* Radius before emission; angular phase inside the loop. */
    f32 *out;
    f32 *first;
    s32 pairs;
    s32 i;

    entry->count = strip->count;
    out = entry->points;
    pairs = strip->count >> 1;
    func_003364B8(rec->rotationXRadians);
    func_00336818(rec->rotationYRadians);
    sdfMultiplyVuMatrixInPlace();
    rec->rotationYRadians += obj->rotationStepDegrees * (3.14159265f / 180.0f);
    radiusOrAngle = rec->radius;
    rec->radius = radiusOrAngle + rec->radiusStep;
    step = 3.14159265f * 2.0f / (f32)obj->segments;
    VU0_LOAD_MATRIX_B(obj->matrix);
    sdfMultiplyVuMatrixInPlace();
    VEC3_SPLAT(baseRadiusVector, radiusOrAngle);
    VEC3_SPLAT(offsetRadiusVector, radiusOrAngle + obj->radialWidth);
    radiusOrAngle = 0.0f;
    VU0_LOAD_VF(vf12, obj->origin);
    for (i = 0; i < pairs - 1; i++) {
        dir[0] = sdfEvaluateCosineViaSinePhaseShift(radiusOrAngle);
        dir[1] = 0;
        dir[2] = sdfSinPoly(radiusOrAngle);
        VU0_LOAD_VF(vf10, dir);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, dir);
        VU0_LOAD_VF(vf11, baseRadiusVector);
        VU0_MUL(vf10, vf10, vf11);
        VU0_ADD(vf10, vf10, vf12);
        VU0_STORE_VF(vf10, out + 4);
        VU0_LOAD_VF(vf10, dir);
        VU0_LOAD_VF(vf11, offsetRadiusVector);
        VU0_MUL(vf10, vf10, vf11);
        VU0_ADD(vf10, vf10, vf12);
        VU0_STORE_VF(vf10, out);
        out += 8;
        radiusOrAngle += step;
    }
    first = entry->points;
    PCP_COPY_VECTOR(out, first);
    PCP_COPY_VECTOR(out + 4, first + 4);
}
/* Scale the same floating pair as polyScaleTransformPair, preserving the duplicate body. */
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
    pool->duration = POLY_RESET_DURATION;
    pool->endColorRampFrames = 0;
    pool->startColorRampFrames = 0;
    record = pool->records;
    if (entryCount != 0) {
        do {
            if (*record != POLY_INACTIVE_ENTRY_AGE) {
                *record = POLY_RESET_ENTRY_AGE;
            }
            index = index + 1;
            record = record + 5;
        } while (index < entryCount);
    }
}
