#ifndef BTL_REWARD_LEVEL_SCALE_KIND_H
#define BTL_REWARD_LEVEL_SCALE_KIND_H

#include "common.h"

/* Selects one of the two reward columns in the level-difference scale table. */
typedef s32 BtlRewardLevelScaleKind;
enum {
    BTL_REWARD_LEVEL_SCALE_HUNT_EXPERIENCE = 0,
    BTL_REWARD_LEVEL_SCALE_ENEMY_EXPERIENCE = 1,
};

#endif
