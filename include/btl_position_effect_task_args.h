#ifndef BTL_POSITION_EFFECT_TASK_ARGS_H
#define BTL_POSITION_EFFECT_TASK_ARGS_H

#include "common.h"

struct BtlUnit;

/* Cleared 0x10-byte payload shared by the actor position-effect tasks. */
typedef struct BtlPositionEffectArgs {
    struct BtlUnit *unit; /* 0x00: actor whose position is updated */
    s32 tick;             /* 0x04: callback invocation count */
    f32 amount;           /* 0x08: current normalized displacement */
    f32 velocity;         /* 0x0C: displacement step */
} BtlPositionEffectArgs;

typedef char BtlPositionEffectArgs_size[(sizeof(BtlPositionEffectArgs) == 0x10) ? 1 : -1];
typedef char BtlPositionEffectArgs_unit_offset[((unsigned long)&((BtlPositionEffectArgs *)0)->unit == 0x00) ? 1 : -1];
typedef char BtlPositionEffectArgs_tick_offset[((unsigned long)&((BtlPositionEffectArgs *)0)->tick == 0x04) ? 1 : -1];
typedef char BtlPositionEffectArgs_amount_offset[((unsigned long)&((BtlPositionEffectArgs *)0)->amount == 0x08) ? 1 : -1];
typedef char BtlPositionEffectArgs_velocity_offset[((unsigned long)&((BtlPositionEffectArgs *)0)->velocity == 0x0C) ? 1 : -1];

#endif
