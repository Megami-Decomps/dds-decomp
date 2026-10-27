#ifndef BTL_REFERENCED_BATTLE_EFFECT_TASK_H
#define BTL_REFERENCED_BATTLE_EFFECT_TASK_H

#include "common.h"
#include "eff_battle_voice_kind.h"

struct BtlRuntimeTask;
struct BtlUnit;
struct SoundResourceNode;

/* The factory borrows these pointers. Starting the returned task retains the
 * effect and both units; finishing it releases those temporary references. */
struct BtlRuntimeTask *btlCreateReferencedBattleEffectTask(
    struct SoundResourceNode *borrowedEffect, struct BtlUnit *sourceOwner,
    struct BtlUnit *targetOwner, EffBattleVoiceKind kind);

#endif /* BTL_REFERENCED_BATTLE_EFFECT_TASK_H */
