#ifndef ACTOR_EFFECT_TASK_H
#define ACTOR_EFFECT_TASK_H

#include "btl_sound.h"
#include "common.h"

typedef struct ActorEffectTaskArgs {
    SoundResourceNode *source;
    BattleEffect *effect;
    ActorEffectOwner owner;
    u32 duration;
    s32 counter;
} ActorEffectTaskArgs;

typedef char ActorEffectTaskArgs_size_must_be_0x14[
    (sizeof(ActorEffectTaskArgs) == 0x14) ? 1 : -1];
typedef char ActorEffectTaskArgs_source_offset_must_be_0x00[
    ((u32)&((ActorEffectTaskArgs *)0)->source == 0x00) ? 1 : -1];
typedef char ActorEffectTaskArgs_effect_offset_must_be_0x04[
    ((u32)&((ActorEffectTaskArgs *)0)->effect == 0x04) ? 1 : -1];
typedef char ActorEffectTaskArgs_owner_offset_must_be_0x08[
    ((u32)&((ActorEffectTaskArgs *)0)->owner == 0x08) ? 1 : -1];
typedef char ActorEffectTaskArgs_duration_offset_must_be_0x0C[
    ((u32)&((ActorEffectTaskArgs *)0)->duration == 0x0C) ? 1 : -1];
typedef char ActorEffectTaskArgs_counter_offset_must_be_0x10[
    ((u32)&((ActorEffectTaskArgs *)0)->counter == 0x10) ? 1 : -1];

#endif
