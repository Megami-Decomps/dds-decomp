#include "common.h"
#include "pcp_vu0.h"

enum {
    POLY_INACTIVE_ENTRY_AGE = -0xFFFFFF,
    POLY_RESET_ENTRY_AGE = 0xFFFFFF0,
    POLY_RESET_DURATION = 0xFFFFFFF
};

/* Polygon node with an f32 scale pair and a resource handle at 0xDC.
   (Retail lwc1 at +0xDC belongs to the PolyQuad flavor below, whose
   +0xDC is a float, so the two layouts are distinct node types.) */
typedef struct {
    u8 pad[0xCC]; /* 0x0 */
    f32 unkCC;    /* 0xCC scaled by polyScaleTransformPair/polyScaleNodeFloatingParameters/polyScaleTransformFirstComponent */
    f32 unkD0;    /* 0xD0 scaled by polyScaleTransformPair/polyScaleNodeFloatingParameters */
    u8 padD4[8];  /* 0xD4 */
    u32 cellSystemAddress; /* 0xDC: ParSystem address retained in the existing word type */
} PolyNode;

/* Node of four f32s scaled together by effPolyScaleFourComponents. */
typedef struct {
    u8 pad[0xC8]; /* 0x0 */
    f32 unkC8;    /* 0xC8 */
    f32 unkCC;    /* 0xCC */
    f32 unkD0;    /* 0xD0 */
    u8 padD4[8];  /* 0xD4 */
    f32 unkDC;    /* 0xDC */
} PolyQuad;

/* Three further node flavors, each destructor releasing its own pair. */
typedef struct {
    u8 pad[0xE0]; /* 0x0 */
    u32 cellSystemAddress; /* 0xE0 */
    u8 padE4[4];  /* 0xE4 */
    void *bufferHandle; /* 0xE8: allocation handle retained in the existing pointer type */
} PolyArcResourceNode;

typedef struct {
    u8 pad[0xF0]; /* 0x0 */
    u32 cellSystemAddress; /* 0xF0 */
    u8 padF4[4];  /* 0xF4 */
    void *bufferHandle; /* 0xF8: allocation handle retained in the existing pointer type */
} PolyBandResourceNode;

typedef struct {
    u8 pad[0xF4]; /* 0x0 */
    u32 cellSystemAddress; /* 0xF4 */
    u8 padF8[4];  /* 0xF8 */
    void *bufferHandle; /* 0xFC: allocation handle retained in the existing pointer type */
} PolyRotatingBandResourceNode;

typedef struct {
    s32 age;   /* 0x0: native updates advance this; the inactive sentinel survives reset */
    s32 unk4;  /* 0x4 */
    s32 unk8;  /* 0x8 */
    s32 unkC;  /* 0xC */
    s32 unk10; /* 0x10 */
} PolyEntry; /* 0x14 bytes */

typedef struct {
    u8 pad[0x10];      /* 0x0 */
    u32 entryCount;     /* 0x10 */
    u32 duration;      /* 0x14: lifetime used by native age comparisons */
    u8 pad18[0x50];    /* 0x18 */
    u32 startColorRampFrames; /* 0x68: color interpolation window from the start */
    u32 endColorRampFrames; /* 0x6C: color interpolation window before duration */
    u8 pad70[0x50];    /* 0x70 */
    u32 spawnDelayStep; /* 0xC0: subtracted between initial record ages */
    u8 padC4[0x18];    /* 0xC4 */
    u32 spawnDelayGroupSize; /* 0xDC: rotating records share a delay within each group */
    u8 padE0[4];       /* 0xE0 */
    u32 *arcRecordWords; /* 0xE4: native arc records, two words each */
    u8 padE8[0xC];     /* 0xE8 */
    u32 *ringRecordWords; /* 0xF4: native ring records, two words each */
    PolyEntry *entries; /* 0xF8 */
} PolyList;

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

void parReleaseCellSystem(u32 arg);
void parPrependCellNode(u32 arg);
void func_0015DAA0(void);
void sdfReleaseChipBlock(void *arg);
void func_002D0918(void *arg);
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32 angle);
extern f32 sdfSinPoly(f32 angle);
extern void func_002DD608(f32 angle);
extern void func_002DD968(f32 angle);
extern void sdfMultiplyVuMatrixInPlace(void);

/* Release the cell system, then the node itself; the address remains a u32 word. */
void effPolyDestroyWork(PolyNode *obj) {
    parReleaseCellSystem(obj->cellSystemAddress);
    sdfReleaseChipBlock(obj);
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015DA10);

/* Multiply both existing floating parameters without assigning them axis-specific roles. */
void polyScaleTransformPair(f32 scale, PolyNode *obj) {
    obj->unkCC = obj->unkCC * scale;
    obj->unkD0 = obj->unkD0 * scale;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015DAA0);

/* Run the shared finish step, then enqueue this node's cell system. */
void polyFinishAndReleaseNodeHandle(PolyNode *obj) {
    func_0015DAA0();
    parPrependCellNode(obj->cellSystemAddress);
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015DC70);

/* Apply the same displacement to both points of each pair along their separation direction. */
void polyStripPushPairsApart(PolyNode *node, s32 index) {
    PolyStrip *strip = (PolyStrip *)node->cellSystemAddress;
    PolyStripEntry *entry = &strip->entries[index];
    f32 scale[4];
    f32 *p;
    s32 pairs;
    s32 i;

    p = entry->points;
    pairs = strip->count / 2;
    scale[0] = scale[1] = scale[2] = node->unkD0;
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

INCLUDE_ASM(const s32, "effect/polyManager", func_0015DE88);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015DFA8);

/* Release the band's cell system and backing allocation handle, not the node itself. */
void polyReleaseBandNodeResources(PolyBandResourceNode *obj) {
    parReleaseCellSystem(obj->cellSystemAddress);
    func_002D0918(obj->bufferHandle);
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E100);

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

void polyBandLayoutRing(PolyBand *band, s32 index, f32 radius);

/* Ring record: the age counter and the per-frame radius change. */
typedef struct PolyRingRecord {
    s32 age;   /* 0x0 */
    f32 step;  /* 0x4 */
} PolyRingRecord;

/* Band node as seen by the ring spawner: two base radii, each with a random jitter fraction, and the record table. */
typedef struct PolyRingSpawner {
    u8 pad00[0x14];
    s32 duration;       /* 0x14 */
    u8 pad18[0xB4];
    f32 startRadius;    /* 0xCC */
    f32 endRadius;      /* 0xD0 */
    f32 startJitter;    /* 0xD4 */
    f32 endJitter;      /* 0xD8 */
    u8 padDC[0x18];
    PolyRingRecord *records; /* 0xF4 */
} PolyRingSpawner;

extern u8 D_0034DF38[];
extern f32 effMiscRandUnitFloat(void *state);

/* Randomize record `index`: pick jittered start and end radii, derive the per-frame radius step, and lay the ring out. */
void func_0015E148(PolyRingSpawner *spawner, s32 index) {
    PolyRingRecord *record = spawner->records;
    f32 spread;
    f32 start;
    f32 end;

    record += index;
    spread = spawner->startJitter;
    start = spawner->startRadius * (effMiscRandUnitFloat(D_0034DF38) * spread + (1.0f - spread));
    spread = spawner->endJitter;
    end = spawner->endRadius * (effMiscRandUnitFloat(D_0034DF38) * spread + (1.0f - spread));
    record->age = 0;
    if (spawner->duration > 0) {
        record->step = (end - start) / (f32)spawner->duration;
    } else {
        record->step = end - start;
    }
    polyBandLayoutRing((PolyBand *)spawner, index, start);
}

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
void effPolyScaleFourComponents(f32 scale, PolyQuad *obj) {
    obj->unkC8 = obj->unkC8 * scale;
    obj->unkDC = obj->unkDC * scale;
    obj->unkCC = obj->unkCC * scale;
    obj->unkD0 = obj->unkD0 * scale;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E5D8);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E760);

/* Release the arc's cell system and backing allocation handle, leaving the node alive. */
void polyReleaseNodeCellSystemAndBuffer(PolyArcResourceNode *obj) {
    parReleaseCellSystem(obj->cellSystemAddress);
    func_002D0918(obj->bufferHandle);
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E8B8);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015E900);

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
void polyScaleTransformFirstComponent(f32 scale, PolyNode *obj) {
    obj->unkCC = obj->unkCC * scale;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015EC08);

INCLUDE_ASM(const s32, "effect/polyManager", func_0015ED90);

/* Release the rotating band's cell system and backing allocation handle, not the node. */
void polyReleaseCellBoundNodeResources(PolyRotatingBandResourceNode *obj) {
    parReleaseCellSystem(obj->cellSystemAddress);
    func_002D0918(obj->bufferHandle);
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015EEF0);

/* Rotating-band record: age, start radius, per-frame radius step, start angle and ring phase. */
typedef struct PolyRotatingRecord {
    s32 age;        /* 0x00 */
    f32 radius;     /* 0x04 */
    f32 radiusStep; /* 0x08 */
    f32 angle;      /* 0x0C */
    f32 phase;      /* 0x10 */
} PolyRotatingRecord; /* 0x14 */

/* Band node as seen by the rotating spawner: jittered base radii, delay group size, start angle and the record table. */
typedef struct PolyRotatingSpawner {
    u8 pad00[0x14];
    s32 duration;       /* 0x14 */
    u8 pad18[0xB4];
    f32 startRadius;    /* 0xCC */
    f32 endRadius;      /* 0xD0 */
    f32 startJitter;    /* 0xD4 */
    f32 endJitter;      /* 0xD8 */
    u32 groupSize;      /* 0xDC */
    f32 angleScale;     /* 0xE0 */
    u8 padE4[0x14];
    PolyRotatingRecord *records; /* 0xF8 */
} PolyRotatingSpawner;

/* Randomize record `index`: jittered start and end radii, radius step, start angle (degrees to radians) and the phase within its delay group. */
void func_0015EF50(PolyRotatingSpawner *spawner, u32 index) {
    PolyRotatingRecord *record = spawner->records;
    f32 spread;

    record += index;
    spread = spawner->startJitter;
    record->age = 0;
    record->radius = spawner->startRadius * (effMiscRandUnitFloat(D_0034DF38) * spread + (1.0f - spread));
    spread = spawner->endJitter;
    record->radiusStep = (spawner->endRadius * (effMiscRandUnitFloat(D_0034DF38) * spread + (1.0f - spread)) - record->radius) / (f32)spawner->duration;
    record->angle = spawner->angleScale * 0.017453292f;
    record->phase = 6.2831852f / (f32)spawner->groupSize * (f32)(index % spawner->groupSize);
}

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
    func_002DD608(rec->rotationXRadians);
    func_002DD968(rec->rotationYRadians);
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
void polyScaleNodeFloatingParameters(f32 scale, PolyNode *obj) {
    obj->unkCC = obj->unkCC * scale;
    obj->unkD0 = obj->unkD0 * scale;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_0015F2D0);

/* Reset active records but leave inactive sentinel entries untouched. */
void polyResetEntries(PolyList *obj) {
    u32 count;
    PolyEntry *entry;
    u32 i;

    count = obj->entryCount;
    i = 0;
    obj->duration = POLY_RESET_DURATION;
    obj->endColorRampFrames = 0;
    obj->startColorRampFrames = 0;
    entry = obj->entries;
    if (count != 0) {
        do {
            if (entry->age != POLY_INACTIVE_ENTRY_AGE) {
                entry->age = POLY_RESET_ENTRY_AGE;
            }
            i++;
            entry++;
        } while (i < count);
    }
}
