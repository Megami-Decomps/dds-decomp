#ifndef SDF_TRIG_H
#define SDF_TRIG_H

#include "common.h"

/* Angles are in radians; cosine applies a pi/2 shift to the sine input. */
f32 sdfSinPoly(f32 angle);
f32 sdfEvaluateCosineViaSinePhaseShift(f32 angle);

#endif /* SDF_TRIG_H */
