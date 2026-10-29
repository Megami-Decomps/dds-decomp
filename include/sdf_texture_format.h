#ifndef SDF_TEXTURE_FORMAT_H
#define SDF_TEXTURE_FORMAT_H

#include "common.h"

/* PlayStation 2 GS pixel storage formats (PSM). */
typedef u32 SdfGsPixelStorageFormat;

enum {
    SDF_PSMCT32 = 0x00,
    SDF_PSMCT24 = 0x01,
    SDF_PSMCT16 = 0x02,
    SDF_PSMCT16S = 0x0A,
    SDF_PSMT8 = 0x13,
    SDF_PSMT4 = 0x14,
    SDF_PSMT8H = 0x1B,
};

#endif /* SDF_TEXTURE_FORMAT_H */
