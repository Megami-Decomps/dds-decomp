#ifndef SDF_CHIP_H
#define SDF_CHIP_H

#include "common.h"

void sdfReleaseChipBlock(void *memory);
void *sdfAllocSizeClassBlock(s32 size);
void *sdfAllocAndClearQuadwords(s32 size);
s32 sdfChipIsInRange(s32 address);

#endif /* SDF_CHIP_H */
