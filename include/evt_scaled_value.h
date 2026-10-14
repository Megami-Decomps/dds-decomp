#ifndef EVT_SCALED_VALUE_H
#define EVT_SCALED_VALUE_H

#include "common.h"

/* Shared float-counter view. The leading word and trailing bytes remain
 * opaque; providers disagree on the leading word's current label. */
typedef struct EvtScaledValue {
    u32 word00;
    u32 flags;
    f32 base;
    f32 scaled;
    u8 pad10[8];
    u8 pad18[4];
} EvtScaledValue;

typedef char EvtScaledValue_flags_at_04[
    ((u32)&((EvtScaledValue *)0)->flags == 0x04) ? 1 : -1];
typedef char EvtScaledValue_base_at_08[
    ((u32)&((EvtScaledValue *)0)->base == 0x08) ? 1 : -1];
typedef char EvtScaledValue_scaled_at_0C[
    ((u32)&((EvtScaledValue *)0)->scaled == 0x0C) ? 1 : -1];
typedef char EvtScaledValue_size_must_be_0x1C[
    sizeof(EvtScaledValue) == 0x1C ? 1 : -1];

#endif
