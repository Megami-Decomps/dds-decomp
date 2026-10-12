#ifndef BTL_SCENE_INITIALIZER_H
#define BTL_SCENE_INITIALIZER_H

#include "common.h"

/* One indexed scene row stores its initializer, updater, and behavior flags. */
typedef struct SceneInitializer {
    void (*initialize)(s32 scene);
    s32 (*update)(s32 scene);
    s32 flags;
} SceneInitializer;

typedef char SceneInitializer_extent_must_be_0x0C[
    (sizeof(SceneInitializer) == 0x0C) ? 1 : -1];
typedef char SceneInitializer_update_offset_must_be_0x04[
    ((u32)&((SceneInitializer *)0)->update == 0x04) ? 1 : -1];
typedef char SceneInitializer_flags_offset_must_be_0x08[
    ((u32)&((SceneInitializer *)0)->flags == 0x08) ? 1 : -1];

#endif /* BTL_SCENE_INITIALIZER_H */
