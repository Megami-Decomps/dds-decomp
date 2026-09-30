#ifndef BTL_H
#define BTL_H

#include "common.h"

/* Battle unit model flags pointer at +0x8C (0x90); DDS1/2 battle units. */
typedef struct BtlUnitModel {
    u8 unk_00[0x8C];
    u32 *flags; /* 0x8C */
} BtlUnitModel;

/* Battle effect actor, flags and timing (0x18); DDS1/2 identical views. */
typedef struct BattleEffectState {
    u32 actor, flags, value; /* +0x00/+0x04/+0x08 */
    u16 timer;               /* +0x0C */
    u8 active, phase;        /* +0x0E/+0x0F */
    u32 effect;              /* +0x10 */
    f32 speed;               /* +0x14 */
} BattleEffectState;

typedef struct BtlUnit BtlUnit;

/* Battle task link to a unit and task chain (0x170); four identical DDS1/2 views. */
typedef struct BtlTask {
    u8 unk_00[8];
    u32 flags;               /* +0x08 */
    u8 unk_0C[0xC];
    BtlUnit *unit;           /* +0x18 */
    u8 unk_1C[4];
    s32 result;              /* +0x20 */
    s32 arg;                 /* +0x24 */
    u8 unk_28[0x38];
    s32 unk_60;
    u8 unk_64[0x108];
    struct BtlTask *next;   /* +0x16C */
} BtlTask;

#endif /* BTL_H */
