#ifndef SDF_CHIP_RANGE_API_H
#define SDF_CHIP_RANGE_API_H

#include "common.h"

/* Classify a payload address by whether it lies inside the chip heap. */
s32 sdfChipIsInRange(const void *address);

#endif /* SDF_CHIP_RANGE_API_H */
