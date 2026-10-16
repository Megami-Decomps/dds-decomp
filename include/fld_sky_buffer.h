#ifndef FLD_SKY_BUFFER_H
#define FLD_SKY_BUFFER_H

#include "common.h"

/* The packed sky resource is copied as one fixed-size payload; its fields remain opaque. */
typedef struct FldSkyBuffer {
    u32 words[0x3800];
} FldSkyBuffer;

typedef char FldSkyBufferSizeCheck[sizeof(FldSkyBuffer) == 0xE000 ? 1 : -1];

extern FldSkyBuffer *fldSkyLightSetBuffer;
void fldUploadSkyBuffer(FldSkyBuffer *source);

#endif
