#ifndef SDF_CHIP_RANGE_API_H
#define SDF_CHIP_RANGE_API_H

#include "common.h"

typedef s32 SdfChipAddressRangeResult;

#define SDF_CHIP_ADDRESS_OUTSIDE_RANGE 0
#define SDF_CHIP_ADDRESS_INSIDE_RANGE 1

/* Test signed EE address membership in [sdfChipPageTop, sdfChipPageEnd). */
SdfChipAddressRangeResult sdfChipIsInRange(const void *address);

#endif /* SDF_CHIP_RANGE_API_H */
