#ifndef EFF_FIELD_COLOR_H
#define EFF_FIELD_COLOR_H

#include "common.h"

typedef struct EffFieldColorRecord {
    f32 channelScales[4];
    u32 packedColor;
} EffFieldColorRecord;

typedef char EffFieldColorRecordSizeCheck[sizeof(EffFieldColorRecord) == 0x14 ? 1 : -1];

EffFieldColorRecord *effBTLFieldColorGetBaseColor(s32 colorId, s16 variant, s16 kind, f32 *output);
EffFieldColorRecord *effBTLFieldColorLookupNarrowSelectors(s32 colorId, s16 variant, s16 kind, f32 *output);

void effBTLFieldColorSetSelectors(s32 baseId, u32 variant, s32 overrideId, s32 finalId);
#ifdef VERSION_DDS2
u8 effBTLFieldColorTestFlags(u32 bits);
#endif

#endif
