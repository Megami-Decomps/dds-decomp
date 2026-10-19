#ifndef FLD_SHADOW_STRIP_H
#define FLD_SHADOW_STRIP_H

#include "common.h"

/* Both field renderers copy complete 64-byte primitive strip templates. */
typedef struct FldShadowStrip {
    u64 words[8];
} FldShadowStrip;

typedef char FldShadowStripSize[(sizeof(FldShadowStrip) == 64) ? 1 : -1];

#endif /* FLD_SHADOW_STRIP_H */
