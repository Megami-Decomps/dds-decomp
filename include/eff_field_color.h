#ifndef EFF_FIELD_COLOR_H
#define EFF_FIELD_COLOR_H

#include "common.h"

void effBTLFieldColorSetSelectors(s32 baseId, u32 variant, s32 overrideId, s32 finalId);
#ifdef VERSION_DDS2
u8 effBTLFieldColorTestFlags(u32 bits);
#endif

#endif
