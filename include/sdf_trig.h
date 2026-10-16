#ifndef SDF_TRIG_H
#define SDF_TRIG_H

#include "common.h"

/* Angles are in radians; cosine applies a pi/2 shift to the sine input. */
f32 sdfAtan2(f32 y, f32 x);
f32 sdfSinPoly(f32 angle);
f32 sdfEvaluateCosineViaSinePhaseShift(f32 angle);
f32 sdfTableInterpolate(f32 value, const f32 *table, s32 sampleCount);

#endif /* SDF_TRIG_H */
