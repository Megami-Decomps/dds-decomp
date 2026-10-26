#ifndef BTL_SCENE_COUNTER_MODE_H
#define BTL_SCENE_COUNTER_MODE_H

#include "common.h"

/* Action-result classification forwarded to the scene counter task. */
typedef s32 BtlSceneCounterMode;

enum {
    BTL_SCENE_COUNTER_MODE_NOOP = 0,
    BTL_SCENE_COUNTER_MODE_NORMAL = 1,
    BTL_SCENE_COUNTER_MODE_CRITICAL = 2,
    BTL_SCENE_COUNTER_MODE_WEAKNESS = 3,
};

#endif /* BTL_SCENE_COUNTER_MODE_H */
