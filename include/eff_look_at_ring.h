#ifndef EFF_LOOK_AT_RING_H
#define EFF_LOOK_AT_RING_H

#include "eff.h"

/* Kind-specific extension used by the paired look-at ring emitter clients. */
typedef struct EffLookAtRingEmitter {
    EffEmitterHead head; /* shared emitter prefix, 0x150 bytes */
    u8 mode;             /* 0x150 */
    u8 loop;             /* 0x151 */
    u8 opaque152[10];
    u32 period;          /* 0x15C */
    f32 f160;            /* 0x160; meaning remains unresolved */
    f32 f164;            /* 0x164; meaning remains unresolved */
} EffLookAtRingEmitter;

typedef char EffLookAtRingModeOffsetCheck[((u32)&((EffLookAtRingEmitter *)0)->mode) == 0x150 ? 1 : -1];
typedef char EffLookAtRingLoopOffsetCheck[((u32)&((EffLookAtRingEmitter *)0)->loop) == 0x151 ? 1 : -1];
typedef char EffLookAtRingPeriodOffsetCheck[((u32)&((EffLookAtRingEmitter *)0)->period) == 0x15C ? 1 : -1];
typedef char EffLookAtRingFloat160OffsetCheck[((u32)&((EffLookAtRingEmitter *)0)->f160) == 0x160 ? 1 : -1];
typedef char EffLookAtRingFloat164OffsetCheck[((u32)&((EffLookAtRingEmitter *)0)->f164) == 0x164 ? 1 : -1];

#endif
