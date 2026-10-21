#ifndef DDS_POLY_RING_VARIANTS_H
#define DDS_POLY_RING_VARIANTS_H

#include "poly_band.h"

struct ParSystem;
struct SdfMemBlock;

/* Arc and rotating-band owners share the ring template prefix, but have
 * distinct parameter tails and trailing per-entry record arrays. */
typedef struct PolyArc {
    PolyRingHead head;
    u32 spawnDelayStep;
    u32 segments;
    f32 radialWidth;
    f32 radius;
    f32 radiusJitter;
    void *dispatchArgB;             /* 0xD4 */
    u8 padD8[4];
    void *dispatchArgA;             /* 0xDC */
    struct ParSystem *strip;         /* 0xE0 */
    PolyArcRecord *records;          /* 0xE4 */
    struct SdfMemBlock *allocation;  /* 0xE8 */
    u8 padEC[4];
} PolyArc; /* 0xF0, followed by eight-byte records */

typedef struct PolyRotatingBand {
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
    void *dispatchArgB;             /* 0xE8 */
    u8 padEC[4];
    void *dispatchArgA;             /* 0xF0 */
    struct ParSystem *strip;         /* 0xF4 */
    PolyRotatingBandRecord *records; /* 0xF8 */
    struct SdfMemBlock *allocation;  /* 0xFC */
} PolyRotatingBand; /* 0x100, followed by 20-byte records */

/* Creation clamps the shared segment-count field through this typed view. */
typedef union PolyRing {
    PolyBand band;
    PolyArc arc;
    PolyRotatingBand rotating;
} PolyRing;

typedef char PolyArc_size_must_be_0xF0[(sizeof(PolyArc) == 0xF0) ? 1 : -1];
typedef char PolyArc_segments_offset_must_be_0xC4[
    ((u32)&((PolyArc *)0)->segments == 0xC4) ? 1 : -1];
typedef char PolyArc_records_offset_must_be_0xE4[
    ((u32)&((PolyArc *)0)->records == 0xE4) ? 1 : -1];
typedef char PolyArc_allocation_offset_must_be_0xE8[
    ((u32)&((PolyArc *)0)->allocation == 0xE8) ? 1 : -1];

typedef char PolyRotatingBand_size_must_be_0x100[
    (sizeof(PolyRotatingBand) == 0x100) ? 1 : -1];
typedef char PolyRotatingBand_segments_offset_must_be_0xC4[
    ((u32)&((PolyRotatingBand *)0)->segments == 0xC4) ? 1 : -1];
typedef char PolyRotatingBand_records_offset_must_be_0xF8[
    ((u32)&((PolyRotatingBand *)0)->records == 0xF8) ? 1 : -1];
typedef char PolyRotatingBand_allocation_offset_must_be_0xFC[
    ((u32)&((PolyRotatingBand *)0)->allocation == 0xFC) ? 1 : -1];

#endif
