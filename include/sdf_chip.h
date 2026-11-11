#ifndef SDF_CHIP_H
#define SDF_CHIP_H

#include "common.h"

void sdfChipFreeDirect(void *memory);
void *sdfChipAllocDirect(s32 size);
void *sdfChipAllocZeroDirect(s32 size);
s32 sdfChipCheck(const void *address);

#endif /* SDF_CHIP_H */
