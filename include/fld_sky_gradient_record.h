#ifndef FLD_SKY_GRADIENT_RECORD_H
#define FLD_SKY_GRADIENT_RECORD_H

#include "common.h"

/* FILTER.FLD loads 256 records of this mode and 8-by-9 color grid. */
typedef struct FldSkyGradientRecord {
    s32 mode;
    s32 colors[8][9];
} FldSkyGradientRecord;

typedef char FldSkyGradientRecordSizeCheck[
    (sizeof(FldSkyGradientRecord) == 0x124) ? 1 : -1];
typedef char FldSkyGradientRecordColorsOffsetCheck[
    ((u32)&((FldSkyGradientRecord *)0)->colors == 0x04) ? 1 : -1];

#endif
