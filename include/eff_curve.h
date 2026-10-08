#ifndef EFF_CURVE_H
#define EFF_CURVE_H

#include "common.h"

enum EffScalarCurveMode {
    EFF_SCALAR_CURVE_MODE_ENDPOINTS = 0,
    EFF_SCALAR_CURVE_MODE_ONE_INTERMEDIATE_KEY = 1,
    EFF_SCALAR_CURVE_MODE_TWO_INTERMEDIATE_KEYS = 2
};

/* Piecewise-linear scalar keys. Frames and duration are signed. */
typedef struct EffScalarCurve {
    u8 mode; /* enum EffScalarCurveMode, stored as a byte */
    u8 reserved01[3];
    f32 initialValue;
    f32 finalValue;
    u8 reserved0C[4];
    u8 headingMode; /* Used by heading tracks: 2 selects projected direction. */
    u8 reserved11[3];
    f32 firstValue;
    f32 firstFraction;
    f32 secondValue;
    f32 secondFraction;
} EffScalarCurve; /* 0x24 */

/* Scalar tracks in effect configurations reserve eight trailing bytes. */
typedef struct EffScalarTrack {
    EffScalarCurve curve;
    u8 reserved24[8];
} EffScalarTrack; /* 0x2C */

f32 effSampleScalarCurve(const EffScalarCurve *curve, s32 frame, s32 duration);

struct SdfColorTrack;
struct SdfAlphaTrack;

u32 effSampleColorAlphaTracks(const struct SdfColorTrack *colorTrack,
                              const struct SdfAlphaTrack *alphaTrack,
                              s32 frame, s32 duration);

#endif
