#ifndef EFF_BALLISTIC_LAUNCH_MODE_H
#define EFF_BALLISTIC_LAUNCH_MODE_H

#include "common.h"

typedef u8 EffBallisticLaunchMode;

/* Spawn around a radius ring and transform the launch through the emitter matrix. */
#define EFF_BALLISTIC_LAUNCH_MODE_RING 0
/* Spawn at the emitter origin with a random or authored launch direction. */
#define EFF_BALLISTIC_LAUNCH_MODE_ORIGIN 1
/* With random directions enabled, start at the integrated endpoint and travel backward. */
#define EFF_BALLISTIC_LAUNCH_MODE_REVERSE_TRAJECTORY 2

#endif /* EFF_BALLISTIC_LAUNCH_MODE_H */
