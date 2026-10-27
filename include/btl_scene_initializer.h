#ifndef BTL_SCENE_INITIALIZER_H
#define BTL_SCENE_INITIALIZER_H

#include "common.h"

void btlInitializeBossCallbacks(s32 mode);
void btlRequestSceneEventResources(void);

/* The encounter setup consumes the roll only as a binary surprise decision. */
typedef s32 BtlSurpriseAttackResult;
enum {
    BTL_SURPRISE_ATTACK_NOT_TRIGGERED = 0,
    BTL_SURPRISE_ATTACK_TRIGGERED = 1,
};

BtlSurpriseAttackResult btlRollSurpriseAttack(void);

/* Zero keeps the current scene; a nonzero result queues that scene index. */
typedef s32 BtlSceneUpdateResult;

/* One indexed scene row stores its initializer, updater, and behavior flags. */
typedef struct SceneInitializer {
    void (*initialize)(void *sceneState);
    BtlSceneUpdateResult (*update)(void *sceneState);
    s32 flags;
} SceneInitializer;

typedef char SceneInitializer_extent_must_be_0x0C[
    (sizeof(SceneInitializer) == 0x0C) ? 1 : -1];
typedef char SceneInitializer_update_offset_must_be_0x04[
    ((u32)&((SceneInitializer *)0)->update == 0x04) ? 1 : -1];
typedef char SceneInitializer_flags_offset_must_be_0x08[
    ((u32)&((SceneInitializer *)0)->flags == 0x08) ? 1 : -1];

#endif /* BTL_SCENE_INITIALIZER_H */
