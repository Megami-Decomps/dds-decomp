#include "common.h"
#include "kernel.h"
#include "libsdf.h"

extern u8 dds3RandCB[];

extern void sdfRandomize(void *aCB, u32 aSeed);

void dds3InitRandCB(void) {
    sdfRandomize(dds3RandCB, 0x12345678);
}
