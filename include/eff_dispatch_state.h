#ifndef EFF_DISPATCH_STATE_H
#define EFF_DISPATCH_STATE_H

#include "common.h"

struct EffectEntry;

/* Handler-owned billboard effect state. DDS1 leaves its leading bytes opaque;
 * DDS2 additionally exposes entry/value fields in that prefix. The matrix,
 * color, and dispatch tail offsets are shared by both titles. */
typedef struct EffectDispatchState {
#ifdef VERSION_DDS2
    u8 pad00[0x14];
    struct EffectEntry *entries;
    u8 pad18[4];
    u32 value1C;
#else
    u8 pad00[0x20];
#endif
    u8 matrix20[0x40];
    u32 value60;
#ifdef VERSION_DDS2
    u8 value64;
    u8 pad65[0xB];
#else
    u8 pad64[0xC];
#endif
    u8 matrix70[0x40];
    u16 handler;
    u16 valueB2;
} EffectDispatchState;

typedef char EffectDispatchState_size_must_be_0xB4[
    (sizeof(EffectDispatchState) == 0xB4) ? 1 : -1];
typedef char EffectDispatchState_matrix20_offset_must_be_0x20[
    ((u32)&((EffectDispatchState *)0)->matrix20 == 0x20) ? 1 : -1];
typedef char EffectDispatchState_value60_offset_must_be_0x60[
    ((u32)&((EffectDispatchState *)0)->value60 == 0x60) ? 1 : -1];
typedef char EffectDispatchState_matrix70_offset_must_be_0x70[
    ((u32)&((EffectDispatchState *)0)->matrix70 == 0x70) ? 1 : -1];
typedef char EffectDispatchState_handler_offset_must_be_0xB0[
    ((u32)&((EffectDispatchState *)0)->handler == 0xB0) ? 1 : -1];
typedef char EffectDispatchState_valueB2_offset_must_be_0xB2[
    ((u32)&((EffectDispatchState *)0)->valueB2 == 0xB2) ? 1 : -1];

#endif
