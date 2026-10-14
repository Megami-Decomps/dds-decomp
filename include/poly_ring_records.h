#ifndef POLY_RING_RECORDS_H
#define POLY_RING_RECORDS_H

#include "common.h"

typedef struct PolyBandRecord {
    s32 age;
    f32 radiusStep;
} PolyBandRecord;

typedef struct PolyArcRecord {
    s32 age;
    u32 radius; /* Native initializer converts a float to u32. */
} PolyArcRecord;

typedef struct PolyRotatingBandRecord {
    s32 age;
    f32 radius;
    f32 radiusStep;
    f32 rotationXRadians;
    f32 rotationYRadians;
} PolyRotatingBandRecord;

typedef char PolyBandRecord_size_must_be_0x08[
    (sizeof(PolyBandRecord) == 0x08) ? 1 : -1];
typedef char PolyBandRecord_age_offset_must_be_0x00[
    ((u32)&((PolyBandRecord *)0)->age == 0x00) ? 1 : -1];
typedef char PolyBandRecord_radiusStep_offset_must_be_0x04[
    ((u32)&((PolyBandRecord *)0)->radiusStep == 0x04) ? 1 : -1];
typedef char PolyArcRecord_size_must_be_0x08[
    (sizeof(PolyArcRecord) == 0x08) ? 1 : -1];
typedef char PolyArcRecord_age_offset_must_be_0x00[
    ((u32)&((PolyArcRecord *)0)->age == 0x00) ? 1 : -1];
typedef char PolyArcRecord_radius_offset_must_be_0x04[
    ((u32)&((PolyArcRecord *)0)->radius == 0x04) ? 1 : -1];
typedef char PolyRotatingBandRecord_size_must_be_0x14[
    (sizeof(PolyRotatingBandRecord) == 0x14) ? 1 : -1];
typedef char PolyRotatingBandRecord_radius_offset_must_be_0x04[
    ((u32)&((PolyRotatingBandRecord *)0)->radius == 0x04) ? 1 : -1];
typedef char PolyRotatingBandRecord_radiusStep_offset_must_be_0x08[
    ((u32)&((PolyRotatingBandRecord *)0)->radiusStep == 0x08) ? 1 : -1];
typedef char PolyRotatingBandRecord_rotationXRadians_offset_must_be_0x0C[
    ((u32)&((PolyRotatingBandRecord *)0)->rotationXRadians == 0x0C) ? 1 : -1];
typedef char PolyRotatingBandRecord_rotationYRadians_offset_must_be_0x10[
    ((u32)&((PolyRotatingBandRecord *)0)->rotationYRadians == 0x10) ? 1 : -1];

#endif /* POLY_RING_RECORDS_H */
