#ifndef BTL_UNIT_MODEL_PROGRESS_API_H
#define BTL_UNIT_MODEL_PROGRESS_API_H

#include "common.h"

struct BtlUnit;

typedef s32 BtlUnitModelFrameCount;

/* Both queries return zero when the unit's model flag is clear. */
f32 btlGetUnitModelCurrentFrame(const struct BtlUnit *unit);
BtlUnitModelFrameCount btlGetUnitModelFrameCount(const struct BtlUnit *unit);

#endif /* BTL_UNIT_MODEL_PROGRESS_API_H */
