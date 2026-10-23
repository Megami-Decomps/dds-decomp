#ifndef FLD_SKY_BUFFER_H
#define FLD_SKY_BUFFER_H

#include "common.h"
#include "fld_light_set.h"

/* The packed resource has both a raw transfer view and a light-set table view. */
typedef union FldSkyBuffer {
    u32 words[0x3800];
    FldLightSet lightSets[0x100];
} FldSkyBuffer;

typedef char FldSkyBufferSizeCheck[sizeof(FldSkyBuffer) == 0xE000 ? 1 : -1];

extern FldSkyBuffer *fldSkyLightSetBuffer;
void fldUploadSkyBuffer(FldSkyBuffer *source);

#endif
