#include "common.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

struct SdfMemBlock;

enum {
    POLY_INACTIVE_ENTRY_AGE = -0xFFFFFF,
    POLY_RESET_ENTRY_AGE = 0xFFFFFF0,
    POLY_RESET_DURATION = 0xFFFFFFF
};

/* A cell's coordinate vectors and packed colors are separate owned arrays. */
typedef struct {
    f32 *points;
    u32 *colors;
    s32 count;
    u32 unk0C;
    u32 color;
} PolyStripEntry; /* 0x14 */

typedef struct PolyStrip {
    s16 kind;
    s16 unk02;
    s32 entryCount;
    s32 count;
    s32 groupDivisor;
    struct SdfMemBlock *allocation;
    PolyStripEntry *entries;
    f32 *points;
    u32 *colors;
    u32 asset;
    struct PolyStrip *next;
    u32 unk28;
} PolyStrip; /* 0x2C: parAllocateCellSystem's header */

/* Basic node: +0xDC is a cell-system pointer, not the band's float step. */
typedef struct {
    f32 origin[4];
    u16 entryCount;                /* 0x10 */
    u8 pad12[2];
    s32 duration;                  /* 0x14 */
    u8 pad18[8];
    f32 matrix[16];                /* 0x20: radial-vector transform */
    u8 pad60[0x52];
    u16 active;                    /* 0xB2: cleared when every entry finishes */
    u8 padB4[0xC];
    u8 loop;                       /* 0xC0: restart finished entries */
    u8 padC1[3];
    s32 spawnDelayStep;            /* 0xC4: stagger inactive entry ages */
    u16 segments;
    u8 padCA[2];
    f32 radius;
    f32 pairDisplacement;
    u8 padD4[8];
    PolyStrip *strip;
    s32 *ages;
} PolyNode;

/* The allocated ring families copy this common 0xC0-byte prefix and append
 * their own parameters/resources; their tails are not interchangeable. */
typedef struct {
    f32 origin[4];
    u32 entryCount;                /* 0x10 */
    s32 duration;                  /* 0x14 */
    u32 templateSize;              /* 0x18 */
    u8 pad1C[4];
    f32 matrix[16];                /* 0x20 */
    u32 color;                     /* 0x60 */
    u8 loop;                       /* 0x64 */
    u8 pad65;
    u16 unk66;
    u32 startColorRampFrames;      /* 0x68 */
    u32 endColorRampFrames;        /* 0x6C */
    f32 matrixCopy[16];            /* 0x70: initialized from the template transform */
    u8 padB0[2];
    u16 active;                    /* 0xB2: cleared when all entries finish */
    u8 padB4[0xC];
} PolyRingHead; /* 0xC0 */

/* +4 is the per-update radius delta, not an absolute radius. */
typedef struct {
    s32 age;
    f32 radiusStep;
} PolyBandRecord; /* 0x08 */

typedef struct {
    PolyRingHead head;
    u32 spawnDelayStep;
    u32 segments;
    f32 radialWidth;
    f32 initialRadius;
    f32 targetRadius;
    f32 initialRadiusJitter;
    f32 targetRadiusJitter;
    f32 liftStep;
    u8 padE0[4];
    void *unkE4;
    u8 padE8[4];
    void *unkEC;
    PolyStrip *strip;              /* 0xF0 */
    PolyBandRecord *records;       /* 0xF4 */
    struct SdfMemBlock *allocation; /* 0xF8 */
    u8 padFC[4];
} PolyBand; /* 0x100, followed by eight-byte records */

typedef struct {
    s32 age;
    u32 radius; /* Native initializer deliberately converts a float to u32. */
} PolyArcRecord; /* 0x08 */

typedef struct {
    PolyRingHead head;
    u32 spawnDelayStep;
    u32 segments;
    f32 radialWidth;
    f32 radius;
    f32 radiusJitter;
    u8 padD4[0xC];
    PolyStrip *strip;              /* 0xE0 */
    PolyArcRecord *records;        /* 0xE4 */
    struct SdfMemBlock *allocation; /* 0xE8 */
    u8 padEC[4];
} PolyArc; /* 0xF0, followed by eight-byte records */

typedef struct {
    s32 age;
    f32 radius;
    f32 radiusStep;
    f32 rotationXRadians;
    f32 rotationYRadians;
} PolyRotatingBandRecord; /* 0x14 */


typedef struct {
    PolyRingHead head;
    u32 spawnDelayStep;
    u32 segments;
    f32 radialWidth;
    f32 initialRadius;
    f32 targetRadius;
    f32 initialRadiusJitter;
    f32 targetRadiusJitter;
    u32 spawnDelayGroupSize;
    f32 rotationXDegrees;
    f32 rotationStepDegrees;
    u8 padE8[0xC];
    PolyStrip *strip;              /* 0xF4 */
    PolyRotatingBandRecord *records; /* 0xF8 */
    struct SdfMemBlock *allocation; /* 0xFC */
} PolyRotatingBand; /* 0x100, followed by 20-byte records */

void parReleaseCellSystem(PolyStrip *strip);
void parPrependCellNode(PolyStrip *strip);
void sdfReleaseResourceAllocation(struct SdfMemBlock *allocation);
void sdfReleaseChipBlock(void *arg);
void polyUpdateBasicRingCells(PolyNode *obj);
void func_00165860(PolyNode *node, s32 index);
void polyStripPushPairsApart(PolyNode *node, s32 index);
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32 angle);
extern f32 sdfSinPoly(f32 angle);
extern void func_003364B8(f32 angle);
extern void func_00336818(f32 angle);
extern void sdfMultiplyVuMatrixInPlace(void);

/* Release the cell system, then the basic node itself. */
void effPolyDestroyWork(PolyNode *obj) {
    parReleaseCellSystem(obj->strip);
    sdfReleaseChipBlock(obj);
}

/* The cell initializer writes the same ages that the basic-ring update reads. */
void func_00165600(PolyNode *node) {
    u16 count;
    s32 age;
    s32 *ages;
    u16 i;
    PolyStrip *system;
    PolyStripEntry *cells;

    count = node->entryCount;
    age = POLY_INACTIVE_ENTRY_AGE;
    ages = node->ages;
    system = node->strip;
    i = 0;
    if (node->entryCount != 0) {
        cells = system->entries;
        do {
            cells[i].unk0C = 0;
            cells[i].count = 0;
            cells[i].color = 0x00808080;
            *ages = age;
            ages++;
            age -= node->spawnDelayStep;
            i++;
        } while (i < count);
    }
}

/* Scale the basic ring radius and its point-pair displacement. */
void polyScaleTransformPair(f32 scale, PolyNode *obj) {
    obj->radius = obj->radius * scale;
    obj->pairDisplacement = obj->pairDisplacement * scale;
}

/* Fade active basic-ring cells, update their geometry, and restart or finish ages. */
void polyUpdateBasicRingCells(PolyNode *obj) {
    u16 index;
    u16 completed;
    u16 count;
    u8 loop;
    s32 duration;
    f32 alphaStep;
    PolyStripEntry *entry;
    s32 *ages;

    if (obj->active != 0) {
        duration = obj->duration;
        completed = 0;
        count = obj->entryCount;
        entry = obj->strip->entries;
        alphaStep = 128.0f / (f32)duration;
        ages = obj->ages;
        loop = obj->loop;
        for (index = 0; index < count; index++) {
            s32 age = *ages;

            if (age == POLY_INACTIVE_ENTRY_AGE) {
                func_00165860(obj, index);
                *ages = 0;
            } else if (age >= 0) {
                entry->color = ((u32)(alphaStep * (f32)(duration - age)) << 24) | 0x808080;
                polyStripPushPairsApart(obj, index);
            }
            if (age >= duration) {
                entry->color = 0x808080;
                if (loop != 0) {
                    *ages = POLY_INACTIVE_ENTRY_AGE;
                } else {
                    completed++;
                    if (completed >= count) {
                        entry->color = 0;
                        obj->active = 0;
                    }
                }
            } else {
                (*ages)++;
            }
            ages++;
            entry++;
        }
    }
}

/* Run the shared finish step, then enqueue this node's cell system. */
void polyFinishAndReleaseNodeHandle(PolyNode *obj) {
    polyUpdateBasicRingCells(obj);
    parPrependCellNode(obj->strip);
}

INCLUDE_ASM(const s32, "effect/polyManager", func_00165860);

/* Apply the same displacement to both points of each pair along their separation direction. */
void polyStripPushPairsApart(PolyNode *node, s32 index) {
    PolyStrip *strip = node->strip;
    PolyStripEntry *entry = &strip->entries[index];
    f32 scale[4];
    f32 *p;
    s32 pairs;
    s32 i;

    p = entry->points;
    pairs = strip->count / 2;
    scale[0] = scale[1] = scale[2] = node->pairDisplacement;
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

extern f32 D_003AAF60[4];
extern f32 D_003AAF70[4];

/* Blend two constant colour vectors by elapsed/duration (1 once elapsed reaches duration) and tint the result with the packed colour. */
u32 polyBlendTimedTintColor(u32 elapsed, u32 duration, u32 color) {
    f32 ratio = 1.0f;
    s32 tint[4];
    u8 unused[16]; /* retail frame 0x20: unused local storage */
    u32 packed;

    if (elapsed < duration) {
        ratio = (f32)elapsed / (f32)duration;
    }
    VU0_LOAD_VF(vf10, D_003AAF70);
    VU0_LOAD_VF(vf11, D_003AAF60);
    VU0_SCALE_VF(vf10, 1.0f - ratio);
    VU0_SCALE_VF(vf11, ratio);
    VU0_ADD(vf10, vf10, vf11);
    VU0_MOVE_VF(vf11, vf10);
    tint[0] = color;
    EE_MMI_RGBA_UNPACK(tint, 1.0f / 128.0f);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
    return packed;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_00165B98);

/* Release the band's cell system and backing allocation handle, not the node itself. */
void polyReleaseBandNodeResources(PolyBand *obj) {
    parReleaseCellSystem(obj->strip);
    sdfReleaseResourceAllocation(obj->allocation);
}

/* Seed band records with staggered inactive ages. */
void func_00165CF0(PolyBand *obj) {
    u32 count = obj->head.entryCount;
    PolyBandRecord *record = obj->records;
    u32 delayStep = obj->spawnDelayStep;
    s32 age = POLY_INACTIVE_ENTRY_AGE;
    u32 index;

    index = 0;
    if (count != 0) {
        do {
            index++;
            record->age = age;
            age -= delayStep;
            record++;
        } while (index < count);
    }
}


void polyBandLayoutRing(PolyBand *band, s32 index, f32 radius);


extern u8 D_003AA868[];
extern f32 effMiscRandUnitFloat(void *state);

/* Randomize record `index`: pick jittered start and end radii, derive the per-frame radius step, and lay the ring out. */
void polyRingRandomizeRecord(PolyBand *spawner, s32 index) {
    PolyBandRecord *record = spawner->records;
    f32 spread;
    f32 start;
    f32 end;

    record += index;
    spread = spawner->initialRadiusJitter;
    start = spawner->initialRadius * (effMiscRandUnitFloat(D_003AA868) * spread + (1.0f - spread));
    spread = spawner->targetRadiusJitter;
    end = spawner->targetRadius * (effMiscRandUnitFloat(D_003AA868) * spread + (1.0f - spread));
    record->age = 0;
    if (spawner->head.duration > 0) {
        record->radiusStep = (end - start) / (f32)spawner->head.duration;
    } else {
        record->radiusStep = end - start;
    }
    polyBandLayoutRing(spawner, index, start);
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
    VU0_LOAD_MATRIX(band->head.matrix);
    angle = 0.0f;
    VU0_LOAD_VF(vf12, band->head.origin);
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

/* Add one radius step and local-y lift step to existing point pairs, then close the strip. */
void polyStripBuildScaledRing(PolyBand *ring, s32 index) {
    PolyStrip *strip = ring->strip;
    PolyBandRecord *rec = &ring->records[index];
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
    scale[2] = scale[1] = scale[0] = rec->radiusStep;
    lift[1] = ring->liftStep;
    lift[2] = lift[0] = 0;
    step = 3.14159265f * 2.0f / (f32)ring->segments;
    VU0_LOAD_MATRIX(ring->head.matrix);
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

/* Scale the band's row width, lift step, and initial/target radii in retail order. */
void effPolyScaleFourComponents(f32 scale, PolyBand *obj) {
    obj->radialWidth = obj->radialWidth * scale;
    obj->liftStep = obj->liftStep * scale;
    obj->initialRadius = obj->initialRadius * scale;
    obj->targetRadius = obj->targetRadius * scale;
}

/* Advance band ages, fades, geometry, and loop completion. */
void func_001661C8(PolyBand *obj) {
    s32 index = 0;
    u32 completed = 0;
    u8 loop;
    PolyStrip *strip;
    s32 count;
    PolyBandRecord *record;
    s32 duration;
    u32 color;
    PolyStripEntry *entry;

    strip = obj->strip;
    count = obj->head.entryCount;
    record = obj->records;
    duration = obj->head.duration;
    loop = obj->head.loop;
    color = obj->head.color;
    entry = strip->entries;

    if (count > 0) {
        do {
            s32 age = record->age;

            if (age == POLY_INACTIVE_ENTRY_AGE) {
                polyRingRandomizeRecord(obj, index);
                age = record->age;
            }
            if (age < 0) {
                age++;
            } else {
                if ((u32)age < obj->head.startColorRampFrames) {
                    entry->color = polyBlendTimedTintColor(age, obj->head.startColorRampFrames, color);
                } else if (age <= duration &&
                           (u32)age >= duration - obj->head.endColorRampFrames) {
                    entry->color = polyBlendTimedTintColor(duration - age, obj->head.endColorRampFrames, color);
                }

                if (entry->color & 0xFF000000) {
                    polyStripBuildScaledRing(obj, index);
                    age++;
                } else {
                    entry->count = 0;
                    age++;
                }
            }

            if (age >= duration) {
                if (loop != 0) {
                    age = POLY_INACTIVE_ENTRY_AGE;
                } else {
                    completed++;
                    if (completed >= (u32)count) {
                        obj->head.active = 0;
                    }
                }
                entry->count = 0;
            }

            record->age = age;
            index++;
            entry++;
            record++;
        } while (index < count);
    }
    parPrependCellNode(obj->strip);
}

INCLUDE_ASM(const s32, "effect/polyManager", func_00166350);

/* Release the arc's cell system and backing allocation handle, leaving the node alive. */
void polyReleaseNodeCellSystemAndBuffer(PolyArc *obj) {
    parReleaseCellSystem(obj->strip);
    sdfReleaseResourceAllocation(obj->allocation);
}

/* Seed arc records with staggered inactive ages. */
void func_001664A8(PolyArc *obj) {
    u32 count = obj->head.entryCount;
    PolyArcRecord *record = obj->records;
    u32 delayStep = obj->spawnDelayStep;
    s32 age = POLY_INACTIVE_ENTRY_AGE;
    u32 index;

    index = 0;
    if (count != 0) {
        do {
            index++;
            record->age = age;
            age -= delayStep;
            record++;
        } while (index < count);
    }
}

/* Reset one arc record and choose its jittered radius. */
void func_001664F0(PolyArc *obj, s32 index) {
    PolyArcRecord *record = obj->records;
    f32 spread = obj->radiusJitter;

    record += index;
    record->age = 0;
    record->radius = obj->radius * (effMiscRandUnitFloat(D_003AA868) * spread + (1.0f - spread));
}


/* Lay a ring of point pairs for strip entry `index` on an arc of the node: the inner row sits at the arc's sine radius, the outer row `width` further out. */
void polyUpdateArcRingStripPoints(PolyArc *obj, s32 index) {
    PolyStrip *strip = obj->strip;
    PolyArcRecord *rec = &obj->records[index];
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
    angle = (f32)rec->age / (f32)obj->head.duration * 3.14159265f;
    inner = radius * sdfSinPoly(angle);
    drop = radius * sdfEvaluateCosineViaSinePhaseShift(angle) - radius;
    step = 3.14159265f * 2.0f / (f32)obj->segments;
    VU0_LOAD_MATRIX(obj->head.matrix);
    width = obj->radialWidth;
    angle = 0.0f;
    VU0_LOAD_VF(vf12, obj->head.origin);
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

/* Scale the arc's base radius, leaving its jitter fraction unchanged. */
void polyScaleTransformFirstComponent(f32 scale, PolyArc *obj) {
    obj->radius = obj->radius * scale;
}

void func_001667F8(PolyArc *obj) {
    s32 index = 0;
    u32 completed = 0;
    u8 loop;
    PolyStrip *strip;
    s32 count;
    PolyArcRecord *record;
    s32 duration;
    u32 color;
    PolyStripEntry *entry;

    strip = obj->strip;
    count = obj->head.entryCount;
    record = obj->records;
    duration = obj->head.duration;
    loop = obj->head.loop;
    color = obj->head.color;
    entry = strip->entries;

    if (count > 0) {
        do {
            s32 age = record->age;

            if (age == POLY_INACTIVE_ENTRY_AGE) {
                func_001664F0(obj, index);
                age = record->age;
            }
            if (age < 0) {
                age++;
            } else {
                if ((u32)age < obj->head.startColorRampFrames) {
                    entry->color = polyBlendTimedTintColor(age, obj->head.startColorRampFrames, color);
                } else if (age <= duration &&
                           (u32)age >= duration - obj->head.endColorRampFrames) {
                    entry->color = polyBlendTimedTintColor(duration - age, obj->head.endColorRampFrames, color);
                }

                if (entry->color & 0xFF000000) {
                    polyUpdateArcRingStripPoints(obj, index);
                    age++;
                } else {
                    entry->count = 0;
                    age++;
                }
            }

            if (age >= duration) {
                if (loop != 0) {
                    age = POLY_INACTIVE_ENTRY_AGE;
                } else {
                    completed++;
                    if (completed >= (u32)count) {
                        obj->head.active = 0;
                    }
                }
                entry->count = 0;
            }

            record->age = age;
            index++;
            entry++;
            record++;
        } while (index < count);
    }
    parPrependCellNode(obj->strip);
}

INCLUDE_ASM(const s32, "effect/polyManager", func_00166980);

/* Release the rotating band's cell system and backing allocation handle, not the node. */
void polyReleaseCellBoundNodeResources(PolyRotatingBand *obj) {
    parReleaseCellSystem(obj->strip);
    sdfReleaseResourceAllocation(obj->allocation);
}

/* Seed rotating-band records, advancing the age after each delay group. */
void func_00166AE0(PolyRotatingBand *obj) {
    u32 count = obj->head.entryCount;
    PolyRotatingBandRecord *record = obj->records;
    u32 delayStep = obj->spawnDelayStep;
    s32 age = POLY_INACTIVE_ENTRY_AGE;
    u32 index;

    index = 0;
    if (count != 0) {
        do {
            record->age = age;
            record++;
            if ((index + 1) % obj->spawnDelayGroupSize == 0) {
                age -= delayStep;
            }
            index++;
        } while (index < count);
    }
}


/* Randomize record `index`: jittered start and end radii, radius step, start angle (degrees to radians) and the phase within its delay group. */
void polyRotatingRandomizeRecord(PolyRotatingBand *spawner, u32 index) {
    PolyRotatingBandRecord *record = spawner->records;
    f32 spread;

    record += index;
    spread = spawner->initialRadiusJitter;
    record->age = 0;
    record->radius = spawner->initialRadius * (effMiscRandUnitFloat(D_003AA868) * spread + (1.0f - spread));
    spread = spawner->targetRadiusJitter;
    record->radiusStep = (spawner->targetRadius * (effMiscRandUnitFloat(D_003AA868) * spread + (1.0f - spread)) - record->radius) / (f32)spawner->head.duration;
    record->rotationXRadians = spawner->rotationXDegrees * 0.017453292f;
    record->rotationYRadians = 6.2831852f / (f32)spawner->spawnDelayGroupSize * (f32)(index % spawner->spawnDelayGroupSize);
}


/* Build the paired ring using current radius/rotation, then retain their next-step values. */
void polyBandLayoutRingRotated(PolyRotatingBand *obj, s32 index)
{
    PolyStrip *strip = obj->strip;
    PolyRotatingBandRecord *rec = &obj->records[index];
    PolyStripEntry *entry = &strip->entries[index];
    f32 dir[4];
    f32 baseRadiusVector[4];
    f32 offsetRadiusVector[4];
    f32 step;
    f32 value; /* Radius before emission; angular phase inside the loop. */
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
    value = rec->radius;
    rec->radius = value + rec->radiusStep;
    step = 3.14159265f * 2.0f / (f32)obj->segments;
    VU0_LOAD_MATRIX_B(obj->head.matrix);
    sdfMultiplyVuMatrixInPlace();
    VEC3_SPLAT(baseRadiusVector, value);
    VEC3_SPLAT(offsetRadiusVector, value + obj->radialWidth);
    value = 0.0f;
    VU0_LOAD_VF(vf12, obj->head.origin);
    for (i = 0; i < pairs - 1; i++) {
        dir[0] = sdfEvaluateCosineViaSinePhaseShift(value);
        dir[1] = 0;
        dir[2] = sdfSinPoly(value);
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
        value += step;
    }
    first = entry->points;
    PCP_COPY_VECTOR(out, first);
    PCP_COPY_VECTOR(out + 4, first + 4);
}
/* Scale the rotating band's initial and target radii. */
void polyScaleNodeFloatingParameters(f32 scale, PolyRotatingBand *obj) {
    obj->initialRadius = obj->initialRadius * scale;
    obj->targetRadius = obj->targetRadius * scale;
}

void func_00166EC0(PolyRotatingBand *obj) {
    s32 index = 0;
    u32 completed = 0;
    u8 loop;
    PolyStrip *strip;
    s32 count;
    PolyRotatingBandRecord *record;
    s32 duration;
    u32 color;
    PolyStripEntry *entry;

    strip = obj->strip;
    count = obj->head.entryCount;
    record = obj->records;
    duration = obj->head.duration;
    loop = obj->head.loop;
    color = obj->head.color;
    entry = strip->entries;

    if (count > 0) {
        s32 inactiveAge = POLY_INACTIVE_ENTRY_AGE;
        s32 shortDuration = duration < 2;

        do {
            s32 age = record->age;

            if (age == inactiveAge) {
                polyRotatingRandomizeRecord(obj, index);
                age = record->age;
            }
            if (age < 0) {
                age++;
            } else {
                if ((u32)age < obj->head.startColorRampFrames) {
                    entry->color = polyBlendTimedTintColor(age, obj->head.startColorRampFrames, color);
                } else if (age <= duration &&
                           (u32)age >= duration - obj->head.endColorRampFrames) {
                    entry->color = polyBlendTimedTintColor(duration - age, obj->head.endColorRampFrames, color);
                } else {
                    entry->color = color;
                }

                if (entry->color & 0xFF000000) {
                    polyBandLayoutRingRotated(obj, index);
                } else {
                    entry->count = 0;
                }
                age++;
            }

            if (age >= duration && !shortDuration) {
                if (loop != 0) {
                    age = inactiveAge;
                } else {
                    completed++;
                    if (completed >= (u32)count) {
                        obj->head.active = 0;
                    }
                }
                entry->count = 0;
            }

            record->age = age;
            index++;
            entry++;
            record++;
        } while (index < count);
    }
    parPrependCellNode(obj->strip);
}

/* Reset active records but leave inactive sentinel entries untouched. */
void polyResetEntries(PolyRotatingBand *obj) {
    u32 count;
    PolyRotatingBandRecord *entry;
    u32 i;

    count = obj->head.entryCount;
    i = 0;
    obj->head.duration = POLY_RESET_DURATION;
    obj->head.endColorRampFrames = 0;
    obj->head.startColorRampFrames = 0;
    entry = obj->records;
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
