#include "common.h"
#include "kernel.h"
#include "libsdf.h"

extern u8 effSharedRandomState[];

extern void sdfRandomize(void *aCB, u32 aSeed);

void dds3InitRandCB(void) {
    sdfRandomize(effSharedRandomState, 0x12345678);
}
