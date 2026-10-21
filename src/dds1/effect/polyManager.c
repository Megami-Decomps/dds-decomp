/* Keeps its own declaration of the functions below (see sdk_libc.h note). */
#define SDK_SKIP_memcpy
#include "sdk_ps2.h"
#include "common.h"
#include "sdf_vu_rotation_axes.h"
#include "sdf_trig.h"
#include "eff_random.h"
#include "sdf_chip.h"
#include "par_cell_api.h"
#include "sdf_resource.h"
#include "pcp_vu0.h"
#include "sdf_vu_matrix.h"
#include "sdf_vu_matrix_multiply_api.h"
#include "ee_mmi.h"
#include "eff.h"
#include "poly_band.h"
#include "poly_node.h"

struct SdfMemBlock;

enum {
    POLY_INACTIVE_ENTRY_AGE = -0xFFFFFF,
    POLY_RESET_ENTRY_AGE = 0xFFFFFF0,
    POLY_RESET_DURATION = 0xFFFFFFF
};

typedef struct {
    PolyRingHead head;
    u32 spawnDelayStep;
    u32 segments;
    f32 radialWidth;
    f32 radius;
    f32 radiusJitter;
    void *dispatchArgB;            /* 0xD4 */
    u8 padD8[4];
    void *dispatchArgA;            /* 0xDC */
    ParSystem *strip;              /* 0xE0 */
    PolyArcRecord *records;        /* 0xE4 */
    struct SdfMemBlock *allocation; /* 0xE8 */
    u8 padEC[4];
} PolyArc; /* 0xF0, followed by eight-byte records */

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
    void *dispatchArgB;            /* 0xE8 */
    u8 padEC[4];
    void *dispatchArgA;            /* 0xF0 */
    ParSystem *strip;              /* 0xF4 */
    PolyRotatingBandRecord *records; /* 0xF8 */
    struct SdfMemBlock *allocation; /* 0xFC */
} PolyRotatingBand; /* 0x100, followed by 20-byte records */

/* The ring kinds share the template prefix and their leading parameter words;
 * the creation paths clamp the segment count through this shared view. */
typedef union {
    PolyBand band;
    PolyArc arc;
    PolyRotatingBand rotating;
} PolyRing;

void polyUpdateBasicRingCells(PolyNode *node);
void polyBuildBasicRadialFan(PolyNode *node, s32 cellIndex);
void polyStripPushPairsApart(PolyNode *node, s32 cellIndex);
extern void memcpy();
extern void parDispatchSub(void *work, s32 sub, void *dispatchArgA, void *dispatchArgB);
void polyInitializeBandRecordAges(PolyBand *band);
void polyInitializeArcRecordAges(PolyArc *arc);
void polyInitializeRotatingBandRecordAges(PolyRotatingBand *rotatingBand);

/* Release the cell system, then the basic node itself. */
void effPolyDestroyWork(PolyNode *node) {
    parReleaseCellSystem(node->strip);
    sdfChipFreeDirect(node);
}

/* The cell initializer writes the same ages that the basic-ring update reads. */
void polyInitializeBasicRingCells(PolyNode *node) {
    u16 count;
    s32 age;
    s32 *ages;
    u16 i;
    ParSystem *system;
    ParCell *cells;

    count = node->entryCount;
    age = POLY_INACTIVE_ENTRY_AGE;
    ages = node->ages;
    system = node->strip;
    i = 0;
    if (node->entryCount != 0) {
        cells = system->cells;
        do {
            cells[i].historyAdvanceCountdown = 0;
            cells[i].vertexCount = 0;
            cells[i].color = 0x00808080;
            *ages = age;
            ages++;
            age -= node->spawnDelayStep;
            i++;
        } while (i < count);
    }
}

/* Scale the basic ring radius and its point-pair displacement. */
void polyScaleTransformPair(f32 scale, PolyNode *node) {
    node->radius = node->radius * scale;
    node->pairDisplacement = node->pairDisplacement * scale;
}

/* Fade active basic-ring cells, update their geometry, and restart or finish ages. */
void polyUpdateBasicRingCells(PolyNode *node) {
    u16 index;
    u16 completed;
    u16 count;
    u8 loop;
    s32 duration;
    f32 alphaStep;
    ParCell *entry;
    s32 *ages;

    if (node->active != 0) {
        duration = node->duration;
        completed = 0;
        count = node->entryCount;
        entry = node->strip->cells;
        alphaStep = 128.0f / (f32)duration;
        ages = node->ages;
        loop = node->loop;
        for (index = 0; index < count; index++) {
            s32 age = *ages;

            if (age == POLY_INACTIVE_ENTRY_AGE) {
                polyBuildBasicRadialFan(node, index);
                *ages = 0;
            } else if (age >= 0) {
                entry->color = ((u32)(alphaStep * (f32)(duration - age)) << 24) | 0x808080;
                polyStripPushPairsApart(node, index);
            }
            if (age >= duration) {
                entry->color = 0x808080;
                if (loop != 0) {
                    *ages = POLY_INACTIVE_ENTRY_AGE;
                } else {
                    completed++;
                    if (completed >= count) {
                        entry->color = 0;
                        node->active = 0;
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
void polyFinishAndReleaseNodeHandle(PolyNode *node) {
    polyUpdateBasicRingCells(node);
    parPrependCellNode(node->strip);
}

/* Build a closed radial fan: an origin point and rotated ring point per pair. */
void polyBuildBasicRadialFan(PolyNode *node, s32 cellIndex) {
    ParSystem *strip = node->strip;
    ParCell *cell = &strip->cells[cellIndex];
    f32 radius = node->radius;
    f32 origin[4];
    f32 offset[4];
    f32 angle;
    f32 step;
    f32 cosine;
    f32 sine;
    u128 *p;
    u128 *hist;
    s32 half;
    s32 pairs;
    s32 i;

    step = EFFECT_RING_FULL_TURN / (f32)node->segments;
    cell->vertexCount = strip->vertexWordCount;
    p = cell->history;
    half = strip->vertexWordCount / 2;
    VU0_LOAD_MATRIX(node->matrix);
    angle = 0.0f;
    PCP_COPY_VECTOR_F32(origin, node->origin);
    pairs = half - 1;
    for (i = 0; i < pairs; i++) {
        cosine = sdfCos(angle);
        sine = sdfSinPoly(angle);
        VU0_LOAD_VF(vf11, origin);
        VU0_STORE_VF(vf11, p);
        offset[0] = cosine * radius;
        offset[1] = 0.0f;
        offset[2] = sine * radius;
        VU0_LOAD_VF(vf10, offset);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, p + 1);
        p += 2;
        angle += step;
    }
    hist = cell->history;
    PCP_COPY_VECTOR(p, hist);
    PCP_COPY_VECTOR(p + 1, hist + 1);
}

/* Apply the same displacement to both points of each pair along their separation direction. */
void polyStripPushPairsApart(PolyNode *node, s32 cellIndex) {
    ParSystem *strip = node->strip;
    ParCell *entry = &strip->cells[cellIndex];
    f32 scale[4];
    f32 *p;
    s32 pairs;
    s32 i;

    p = (f32 *)entry->history;
    pairs = strip->vertexWordCount / 2;
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

extern f32 polyTimedTintEndColor[4];
extern f32 polyTimedTintStartColor[4];

/* Blend two constant colour vectors by elapsed/duration (1 once elapsed reaches duration) and tint the result with the packed colour. */
u32 polyBlendTimedTintColor(u32 elapsed, u32 duration, u32 color) {
    f32 ratio = 1.0f;
    s32 tint[4];
    u8 unused[16]; /* retail frame 0x20: unused local storage */
    u32 packed;

    if (elapsed < duration) {
        ratio = (f32)elapsed / (f32)duration;
    }
    VU0_LOAD_VF(vf10, polyTimedTintStartColor);
    VU0_LOAD_VF(vf11, polyTimedTintEndColor);
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

/* Create a band from its template: the template body, then the 0x40-byte parameter tail that follows it, with eight-byte records after the header. */
PolyBand *polyCreateExpandingRingBands(PolyRingHead *templateHead) {
    u32 size = templateHead->entryCount * sizeof(PolyBandRecord) + sizeof(PolyBand);
    struct SdfMemBlock *allocation = sdfMemAllocDirect(size);
    PolyBand *band = (PolyBand *)sdfMemLock(allocation);

    memset(band, 0, size);
    memcpy(band, templateHead, templateHead->templateSize);
    memcpy(&band->spawnDelayStep, (u8 *)templateHead + templateHead->templateSize, 0x40);
    band->head.templateSize = sizeof(PolyRingHead);
    band->records = (PolyBandRecord *)(band + 1);
    band->allocation = allocation;
    if (((PolyRing *)band)->band.segments < 3) {
        ((PolyRing *)band)->band.segments = 3;
    }
    band->strip = parAllocateCellSystem(band->head.entryCount, band->segments, 1, 0);
    parDispatchSub(band->strip, 0, band->dispatchArgA, band->dispatchArgB);
    polyInitializeBandRecordAges(band);
    VU0_LOAD_MATRIX(band->head.matrix);
    VU0_STORE_MATRIX(band->head.matrixCopy);
    parSetCellDrawBucket(band->strip, band->head.drawBucket);
    return band;
}

/* Release the band's cell system and backing allocation handle, not the node itself. */
void polyReleaseBandNodeResources(PolyBand *band) {
    parReleaseCellSystem(band->strip);
    sdfMemFreeDirect(band->allocation);
}

/* Seed band records with staggered inactive ages. */
void polyInitializeBandRecordAges(PolyBand *band) {
    u32 count = band->head.entryCount;
    PolyBandRecord *record = band->records;
    u32 delayStep = band->spawnDelayStep;
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


extern EffRandState effDefaultRandomState[];

/* Randomize record `index`: pick jittered start and end radii, derive the per-frame radius step, and lay the ring out. */
void polyRingRandomizeRecord(PolyBand *spawner, s32 index) {
    PolyBandRecord *record = spawner->records;
    f32 spread;
    f32 start;
    f32 end;

    record += index;
    spread = spawner->initialRadiusJitter;
    start = spawner->initialRadius * (sdfGetRandFloat(effDefaultRandomState) * spread + (1.0f - spread));
    spread = spawner->targetRadiusJitter;
    end = spawner->targetRadius * (sdfGetRandFloat(effDefaultRandomState) * spread + (1.0f - spread));
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
    ParSystem *strip = band->strip;
    ParCell *entry = &strip->cells[index];
    f32 radialDirection[4];
    f32 baseRadiusVector[4];
    f32 offsetRadiusVector[4];
    f32 angleStep;
    f32 angle;
    f32 *pointCursor;
    f32 *firstPointPair;
    s32 pairs;
    s32 i;

    entry->vertexCount = strip->vertexWordCount;
    pointCursor = (f32 *)entry->history;
    pairs = strip->vertexWordCount / 2;
    VEC3_SPLAT(baseRadiusVector, radius);
    VEC3_SPLAT(offsetRadiusVector, radius + band->radialWidth);
    angleStep = 3.14159265f * 2.0f / (f32)band->segments;
    VU0_LOAD_MATRIX(band->head.matrix);
    angle = 0.0f;
    VU0_LOAD_VF(vf12, band->head.origin);
    for (i = 0; i < pairs - 1; i++) {
        radialDirection[0] = sdfCos(angle);
        radialDirection[1] = 0;
        radialDirection[2] = sdfSinPoly(angle);
        VU0_LOAD_VF(vf10, radialDirection);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, radialDirection);
        VU0_LOAD_VF(vf11, baseRadiusVector);
        VU0_MUL(vf10, vf10, vf11);
        VU0_ADD(vf10, vf10, vf12);
        VU0_STORE_VF(vf10, pointCursor + 4);
        VU0_LOAD_VF(vf10, radialDirection);
        VU0_LOAD_VF(vf11, offsetRadiusVector);
        VU0_MUL(vf10, vf10, vf11);
        VU0_ADD(vf10, vf10, vf12);
        VU0_STORE_VF(vf10, pointCursor);
        pointCursor += 8;
        angle += angleStep;
    }
    firstPointPair = (f32 *)entry->history;
    PCP_COPY_VECTOR(pointCursor, firstPointPair);
    PCP_COPY_VECTOR(pointCursor + 4, firstPointPair + 4);
}

/* Add one radius step and local-y lift step to existing point pairs, then close the strip. */
void polyStripBuildScaledRing(PolyBand *band, s32 index) {
    ParSystem *strip = band->strip;
    PolyBandRecord *rec = &band->records[index];
    ParCell *entry = &strip->cells[index];
    f32 radialDirection[4];
    f32 radiusStepVector[4];
    f32 liftVector[4];
    f32 angleStep;
    f32 angle;
    f32 *pointCursor;
    f32 *firstPointPair;
    s32 pairs;
    s32 i;

    entry->vertexCount = strip->vertexWordCount;
    pointCursor = (f32 *)entry->history;
    pairs = strip->vertexWordCount >> 1;
    radiusStepVector[2] = radiusStepVector[1] = radiusStepVector[0] = rec->radiusStep;
    liftVector[1] = band->liftStep;
    liftVector[2] = liftVector[0] = 0;
    angleStep = 3.14159265f * 2.0f / (f32)band->segments;
    VU0_LOAD_MATRIX(band->head.matrix);
    angle = 0.0f;
    for (i = 0; i < pairs - 1; i++) {
        radialDirection[0] = sdfCos(angle);
        radialDirection[1] = 0;
        radialDirection[2] = sdfSinPoly(angle);
        VU0_LOAD_VF(vf10, radialDirection);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_LOAD_VF(vf11, radiusStepVector);
        VU0_MUL(vf10, vf10, vf11);
        VU0_LOAD_VF(vf11, liftVector);
        VU0_APPLY_MATRIX(vf11, vf11);
        VU0_ADD(vf10, vf10, vf11);
        VU0_MOVE_VF(vf12, vf10);
        VU0_LOAD_VF(vf11, pointCursor + 4);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, pointCursor + 4);
        VU0_MOVE_VF(vf10, vf12);
        VU0_LOAD_VF(vf11, pointCursor);
        VU0_ADD(vf10, vf10, vf11);
        VU0_STORE_VF(vf10, pointCursor);
        pointCursor += 8;
        angle += angleStep;
    }
    firstPointPair = (f32 *)entry->history;
    PCP_COPY_VECTOR(pointCursor, firstPointPair);
    PCP_COPY_VECTOR(pointCursor + 4, firstPointPair + 4);
}

/* Scale the band's row width, lift step, and initial/target radii in retail order. */
void effPolyScaleFourComponents(f32 scale, PolyBand *band) {
    band->radialWidth = band->radialWidth * scale;
    band->liftStep = band->liftStep * scale;
    band->initialRadius = band->initialRadius * scale;
    band->targetRadius = band->targetRadius * scale;
}

/* Advance band ages, fades, geometry, and loop completion. */
void polyUpdateBandCells(PolyBand *band) {
    s32 recordIndex = 0;
    u32 completed = 0;
    u8 loop;
    ParSystem *strip;
    s32 count;
    PolyBandRecord *record;
    s32 duration;
    u32 color;
    ParCell *entry;

    strip = band->strip;
    count = band->head.entryCount;
    record = band->records;
    duration = band->head.duration;
    loop = band->head.loop;
    color = band->head.color;
    entry = strip->cells;

    if (count > 0) {
        do {
            s32 age = record->age;

            if (age == POLY_INACTIVE_ENTRY_AGE) {
                polyRingRandomizeRecord(band, recordIndex);
                age = record->age;
            }
            if (age < 0) {
                age++;
            } else {
                if ((u32)age < band->head.startColorRampFrames) {
                    entry->color = polyBlendTimedTintColor(age, band->head.startColorRampFrames, color);
                } else if (age <= duration &&
                           (u32)age >= duration - band->head.endColorRampFrames) {
                    entry->color = polyBlendTimedTintColor(duration - age, band->head.endColorRampFrames, color);
                }

                if (entry->color & 0xFF000000) {
                    polyStripBuildScaledRing(band, recordIndex);
                    age++;
                } else {
                    entry->vertexCount = 0;
                    age++;
                }
            }

            if (age >= duration) {
                if (loop != 0) {
                    age = POLY_INACTIVE_ENTRY_AGE;
                } else {
                    completed++;
                    if (completed >= (u32)count) {
                        band->head.active = 0;
                    }
                }
                entry->vertexCount = 0;
            }

            record->age = age;
            recordIndex++;
            entry++;
            record++;
        } while (recordIndex < count);
    }
    parPrependCellNode(band->strip);
}

PolyArc *polyCreateArcRingNode(PolyArc *source) {
    s32 tailBytes = sizeof(PolyArc) - sizeof(PolyRingHead);
    s32 size = source->head.entryCount * sizeof(PolyArcRecord) + sizeof(PolyRingHead) + tailBytes;
    struct SdfMemBlock *allocation = sdfMemAllocDirect(size);
    PolyArc *clone = (PolyArc *)sdfMemLock(allocation);

    memset(clone, 0, size);
    memcpy(clone, source, source->head.templateSize);
    memcpy(&clone->spawnDelayStep, (u8 *)source + source->head.templateSize, tailBytes);
    clone->head.templateSize = 0xC0;
    clone->records = (PolyArcRecord *)(clone + 1);
    clone->allocation = allocation;
    if (((PolyRing *)clone)->arc.segments < 3) {
        ((PolyRing *)clone)->arc.segments = 3;
    }
    clone->strip = parAllocateCellSystem(clone->head.entryCount, clone->segments, 1, 0);
    parDispatchSub(clone->strip, 0, clone->dispatchArgA, clone->dispatchArgB);
    polyInitializeArcRecordAges(clone);
    VU0_LOAD_MATRIX(clone->head.matrix);
    VU0_STORE_MATRIX(clone->head.matrixCopy);
    parSetCellDrawBucket(clone->strip, clone->head.drawBucket);
    return clone;
}

/* Release the arc's cell system and backing allocation handle, leaving the node alive. */
void polyReleaseNodeCellSystemAndBuffer(PolyArc *arc) {
    parReleaseCellSystem(arc->strip);
    sdfMemFreeDirect(arc->allocation);
}

/* Seed arc records with staggered inactive ages. */
void polyInitializeArcRecordAges(PolyArc *arc) {
    u32 count = arc->head.entryCount;
    PolyArcRecord *record = arc->records;
    u32 delayStep = arc->spawnDelayStep;
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
void polyRandomizeArcRecord(PolyArc *arc, s32 recordIndex) {
    PolyArcRecord *record = arc->records;
    f32 spread = arc->radiusJitter;

    record += recordIndex;
    record->age = 0;
    record->radius = arc->radius * (sdfGetRandFloat(effDefaultRandomState) * spread + (1.0f - spread));
}


/* Lay an arc ring's inner and outer point rows at the projected radius and
 * radial offset for this record's age. */
void polyUpdateArcRingStripPoints(PolyArc *arc, s32 recordIndex) {
    ParSystem *strip = arc->strip;
    PolyArcRecord *record = &arc->records[recordIndex];
    ParCell *stripEntry = &strip->cells[recordIndex];
    f32 innerPointVector[4];
    f32 outerPointVector[4];
    f32 recordRadius;
    f32 projectedInnerRadius;
    f32 verticalOffset;
    f32 radialWidth;
    f32 angleStep;
    f32 angle;
    f32 *pointCursor;
    f32 *firstPointPair;
    s32 pairs;
    s32 i;

    stripEntry->vertexCount = strip->vertexWordCount;
    pointCursor = (f32 *)stripEntry->history;
    pairs = strip->vertexWordCount >> 1;
    recordRadius = record->radius;
    angle = (f32)record->age / (f32)arc->head.duration * 3.14159265f;
    projectedInnerRadius = recordRadius * sdfSinPoly(angle);
    verticalOffset = recordRadius * sdfCos(angle) - recordRadius;
    angleStep = 3.14159265f * 2.0f / (f32)arc->segments;
    VU0_LOAD_MATRIX(arc->head.matrix);
    radialWidth = arc->radialWidth;
    angle = 0.0f;
    VU0_LOAD_VF(vf12, arc->head.origin);
    for (i = 0; i < pairs - 1; i++) {
        innerPointVector[0] = outerPointVector[0] = sdfCos(angle);
        innerPointVector[2] = sdfSinPoly(angle);
        outerPointVector[1] = verticalOffset;
        outerPointVector[2] = innerPointVector[2] * (projectedInnerRadius + radialWidth);
        outerPointVector[0] *= projectedInnerRadius + radialWidth;
        VU0_LOAD_VF(vf10, outerPointVector);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, outerPointVector);
        innerPointVector[1] = verticalOffset;
        innerPointVector[0] *= projectedInnerRadius;
        innerPointVector[2] *= projectedInnerRadius;
        VU0_LOAD_VF(vf10, innerPointVector);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_ADD(vf10, vf10, vf12);
        VU0_STORE_VF(vf10, pointCursor + 4);
        VU0_LOAD_VF(vf10, outerPointVector);
        VU0_ADD(vf10, vf10, vf12);
        VU0_STORE_VF(vf10, pointCursor);
        pointCursor += 8;
        angle += angleStep;
    }
    firstPointPair = (f32 *)stripEntry->history;
    PCP_COPY_VECTOR(pointCursor, firstPointPair);
    PCP_COPY_VECTOR(pointCursor + 4, firstPointPair + 4);
}

/* Scale the arc's base radius, leaving its jitter fraction unchanged. */
void polyScaleTransformFirstComponent(f32 scale, PolyArc *arc) {
    arc->radius = arc->radius * scale;
}

/* Advance arc ages, fades and geometry; no-ramp windows retain the prior tint. */
void polyUpdateArcCells(PolyArc *arc) {
    s32 index = 0;
    u32 completed = 0;
    u8 loop;
    ParSystem *strip;
    s32 count;
    PolyArcRecord *record;
    s32 duration;
    u32 color;
    ParCell *entry;

    strip = arc->strip;
    count = arc->head.entryCount;
    record = arc->records;
    duration = arc->head.duration;
    loop = arc->head.loop;
    color = arc->head.color;
    entry = strip->cells;

    if (count > 0) {
        do {
            s32 age = record->age;

            if (age == POLY_INACTIVE_ENTRY_AGE) {
                polyRandomizeArcRecord(arc, index);
                age = record->age;
            }
            if (age < 0) {
                age++;
            } else {
                if ((u32)age < arc->head.startColorRampFrames) {
                    entry->color = polyBlendTimedTintColor(age, arc->head.startColorRampFrames, color);
                } else if (age <= duration &&
                           (u32)age >= duration - arc->head.endColorRampFrames) {
                    entry->color = polyBlendTimedTintColor(duration - age, arc->head.endColorRampFrames, color);
                }

                if (entry->color & 0xFF000000) {
                    polyUpdateArcRingStripPoints(arc, index);
                    age++;
                } else {
                    entry->vertexCount = 0;
                    age++;
                }
            }

            if (age >= duration) {
                if (loop != 0) {
                    age = POLY_INACTIVE_ENTRY_AGE;
                } else {
                    completed++;
                    if (completed >= (u32)count) {
                        arc->head.active = 0;
                    }
                }
                entry->vertexCount = 0;
            }

            record->age = age;
            index++;
            entry++;
            record++;
        } while (index < count);
    }
    parPrependCellNode(arc->strip);
}

/* Rotating-band counterpart of polyCreateExpandingRingBands: twenty-byte records follow the header. */
PolyRotatingBand *polyCreateRotatingBand(PolyRingHead *templateHead) {
    u32 size = templateHead->entryCount * sizeof(PolyRotatingBandRecord) + sizeof(PolyRotatingBand);
    struct SdfMemBlock *allocation = sdfMemAllocDirect(size);
    PolyRotatingBand *band = (PolyRotatingBand *)sdfMemLock(allocation);

    memset(band, 0, size);
    memcpy(band, templateHead, templateHead->templateSize);
    memcpy(&band->spawnDelayStep, (u8 *)templateHead + templateHead->templateSize, 0x40);
    band->head.templateSize = sizeof(PolyRingHead);
    band->records = (PolyRotatingBandRecord *)(band + 1);
    band->allocation = allocation;
    if (((PolyRing *)band)->rotating.segments < 3) {
        ((PolyRing *)band)->rotating.segments = 3;
    }
    band->strip = parAllocateCellSystem(band->head.entryCount, band->segments, 1, 0);
    parDispatchSub(band->strip, 0, band->dispatchArgA, band->dispatchArgB);
    polyInitializeRotatingBandRecordAges(band);
    VU0_LOAD_MATRIX(band->head.matrix);
    VU0_STORE_MATRIX(band->head.matrixCopy);
    parSetCellDrawBucket(band->strip, band->head.drawBucket);
    return band;
}

/* Release the rotating band's cell system and backing allocation handle, not the node. */
void polyReleaseCellBoundNodeResources(PolyRotatingBand *rotatingBand) {
    parReleaseCellSystem(rotatingBand->strip);
    sdfMemFreeDirect(rotatingBand->allocation);
}

/* Seed rotating-band records, advancing the age after each delay group. */
void polyInitializeRotatingBandRecordAges(PolyRotatingBand *rotatingBand) {
    u32 count = rotatingBand->head.entryCount;
    PolyRotatingBandRecord *record = rotatingBand->records;
    u32 delayStep = rotatingBand->spawnDelayStep;
    s32 age = POLY_INACTIVE_ENTRY_AGE;
    u32 index;

    index = 0;
    if (count != 0) {
        do {
            record->age = age;
            record++;
            if ((index + 1) % rotatingBand->spawnDelayGroupSize == 0) {
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
    record->radius = spawner->initialRadius * (sdfGetRandFloat(effDefaultRandomState) * spread + (1.0f - spread));
    spread = spawner->targetRadiusJitter;
    record->radiusStep = (spawner->targetRadius * (sdfGetRandFloat(effDefaultRandomState) * spread + (1.0f - spread)) - record->radius) / (f32)spawner->head.duration;
    record->rotationXRadians = spawner->rotationXDegrees * 0.017453292f;
    record->rotationYRadians = 6.2831852f / (f32)spawner->spawnDelayGroupSize * (f32)(index % spawner->spawnDelayGroupSize);
}


/* Build the paired ring using current radius/rotation, then retain their next-step values. */
void polyBandLayoutRingRotated(PolyRotatingBand *rotatingBand, s32 index)
{
    ParSystem *strip = rotatingBand->strip;
    PolyRotatingBandRecord *rec = &rotatingBand->records[index];
    ParCell *entry = &strip->cells[index];
    f32 dir[4];
    f32 baseRadiusVector[4];
    f32 offsetRadiusVector[4];
    f32 step;
    f32 value; /* Radius before emission; angular phase inside the loop. */
    f32 *out;
    f32 *first;
    s32 pairs;
    s32 i;

    entry->vertexCount = strip->vertexWordCount;
    out = (f32 *)entry->history;
    pairs = strip->vertexWordCount >> 1;
    sdfVuLoadRotationMatrixX(rec->rotationXRadians);
    sdfVuLoadSecondaryRotationMatrixY(rec->rotationYRadians);
    sdfMultiplyVuMatrixInPlace();
    rec->rotationYRadians += rotatingBand->rotationStepDegrees * (3.14159265f / 180.0f);
    value = rec->radius;
    rec->radius = value + rec->radiusStep;
    step = 3.14159265f * 2.0f / (f32)rotatingBand->segments;
    VU0_LOAD_MATRIX_B(rotatingBand->head.matrix);
    sdfMultiplyVuMatrixInPlace();
    VEC3_SPLAT(baseRadiusVector, value);
    VEC3_SPLAT(offsetRadiusVector, value + rotatingBand->radialWidth);
    value = 0.0f;
    VU0_LOAD_VF(vf12, rotatingBand->head.origin);
    for (i = 0; i < pairs - 1; i++) {
        dir[0] = sdfCos(value);
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
    first = (f32 *)entry->history;
    PCP_COPY_VECTOR(out, first);
    PCP_COPY_VECTOR(out + 4, first + 4);
}
/* Scale the rotating band's initial and target radii. */
void polyScaleNodeFloatingParameters(f32 scale, PolyRotatingBand *rotatingBand) {
    rotatingBand->initialRadius = rotatingBand->initialRadius * scale;
    rotatingBand->targetRadius = rotatingBand->targetRadius * scale;
}

/* Advance rotating-band cells; native completion requires a duration of at least two. */
void polyUpdateRotatingBandCells(PolyRotatingBand *rotatingBand) {
    s32 index = 0;
    u32 completed = 0;
    u8 loop;
    ParSystem *strip;
    s32 count;
    PolyRotatingBandRecord *record;
    s32 duration;
    u32 color;
    ParCell *entry;

    strip = rotatingBand->strip;
    count = rotatingBand->head.entryCount;
    record = rotatingBand->records;
    duration = rotatingBand->head.duration;
    loop = rotatingBand->head.loop;
    color = rotatingBand->head.color;
    entry = strip->cells;

    if (count > 0) {
        s32 inactiveAge = POLY_INACTIVE_ENTRY_AGE;
        s32 shortDuration = duration < 2;

        do {
            s32 age = record->age;

            if (age == inactiveAge) {
                polyRotatingRandomizeRecord(rotatingBand, index);
                age = record->age;
            }
            if (age < 0) {
                age++;
            } else {
                if ((u32)age < rotatingBand->head.startColorRampFrames) {
                    entry->color = polyBlendTimedTintColor(age, rotatingBand->head.startColorRampFrames, color);
                } else if (age <= duration &&
                           (u32)age >= duration - rotatingBand->head.endColorRampFrames) {
                    entry->color = polyBlendTimedTintColor(duration - age, rotatingBand->head.endColorRampFrames, color);
                } else {
                    entry->color = color;
                }

                if (entry->color & 0xFF000000) {
                    polyBandLayoutRingRotated(rotatingBand, index);
                } else {
                    entry->vertexCount = 0;
                }
                age++;
            }

            if (age >= duration && !shortDuration) {
                if (loop != 0) {
                    age = inactiveAge;
                } else {
                    completed++;
                    if (completed >= (u32)count) {
                        rotatingBand->head.active = 0;
                    }
                }
                entry->vertexCount = 0;
            }

            record->age = age;
            index++;
            entry++;
            record++;
        } while (index < count);
    }
    parPrependCellNode(rotatingBand->strip);
}

/* Reset active records but leave inactive sentinel entries untouched. */
void polyResetEntries(PolyRotatingBand *rotatingBand) {
    u32 count;
    PolyRotatingBandRecord *entry;
    u32 i;

    count = rotatingBand->head.entryCount;
    i = 0;
    rotatingBand->head.duration = POLY_RESET_DURATION;
    rotatingBand->head.endColorRampFrames = 0;
    rotatingBand->head.startColorRampFrames = 0;
    entry = rotatingBand->records;
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
