#include "common.h"

void dds3ReleaseModelAllocation(s32 arg0) {
    sdfReleaseChipBlock(*(u32 *)(arg0 + 0x18));
}
