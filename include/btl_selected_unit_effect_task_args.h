#ifndef BTL_SELECTED_UNIT_EFFECT_TASK_ARGS_H
#define BTL_SELECTED_UNIT_EFFECT_TASK_ARGS_H

#include "common.h"

struct BtlUnit;
struct SoundMixer;
struct BattleEffect;

/* Selected-unit effect updater state, allocated and cleared as one task block. */
typedef struct BtlSelectedUnitEffectTaskArgs {
    struct BtlUnit *unit;         /* 0x00: affected unit, also used by the finish hook */
    struct SoundMixer *mixer;     /* 0x04: cloned sound mixer, released by the finish hook */
    struct BattleEffect *effect;  /* 0x08: selected-unit effect voice */
    s32 duration;                /* 0x0C: update lifetime in frames */
    s32 counter;                 /* 0x10: elapsed update count */
} BtlSelectedUnitEffectTaskArgs;

typedef char BtlSelectedUnitEffectTaskArgs_size_check[
    (sizeof(BtlSelectedUnitEffectTaskArgs) == 0x14) ? 1 : -1];
typedef char BtlSelectedUnitEffectTaskArgs_unit_offset_check[
    ((u32)&((BtlSelectedUnitEffectTaskArgs *)0)->unit == 0x00) ? 1 : -1];
typedef char BtlSelectedUnitEffectTaskArgs_mixer_offset_check[
    ((u32)&((BtlSelectedUnitEffectTaskArgs *)0)->mixer == 0x04) ? 1 : -1];
typedef char BtlSelectedUnitEffectTaskArgs_effect_offset_check[
    ((u32)&((BtlSelectedUnitEffectTaskArgs *)0)->effect == 0x08) ? 1 : -1];
typedef char BtlSelectedUnitEffectTaskArgs_duration_offset_check[
    ((u32)&((BtlSelectedUnitEffectTaskArgs *)0)->duration == 0x0C) ? 1 : -1];
typedef char BtlSelectedUnitEffectTaskArgs_counter_offset_check[
    ((u32)&((BtlSelectedUnitEffectTaskArgs *)0)->counter == 0x10) ? 1 : -1];

#endif /* BTL_SELECTED_UNIT_EFFECT_TASK_ARGS_H */
