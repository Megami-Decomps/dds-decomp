#ifndef BTL_SCENE_LIGHTING_TASK_ARGS_H
#define BTL_SCENE_LIGHTING_TASK_ARGS_H

#include "common.h"

/* Both task creators allocate this four-byte payload and initialize its value. */
typedef struct SceneLightRestoreArgs {
    u32 value;
} SceneLightRestoreArgs;

typedef char SceneLightRestoreArgs_size_must_be_0x04[
    (sizeof(SceneLightRestoreArgs) == 0x04) ? 1 : -1];
typedef char SceneLightRestoreArgs_value_offset_must_be_0[
    ((u32)&((SceneLightRestoreArgs *)0)->value == 0x00) ? 1 : -1];

/* The deferred transition task copies three four-float vectors and appends its value. */
typedef struct SceneLightTransitionArgs {
    f32 direction[4];
    f32 lightColor[4];
    f32 ambientColor[4];
    u32 value;
} SceneLightTransitionArgs;

typedef char SceneLightTransitionArgs_size_must_be_0x34[
    (sizeof(SceneLightTransitionArgs) == 0x34) ? 1 : -1];
typedef char SceneLightTransitionArgs_direction_offset_must_be_0[
    ((u32)&((SceneLightTransitionArgs *)0)->direction == 0x00) ? 1 : -1];
typedef char SceneLightTransitionArgs_lightColor_offset_must_be_0x10[
    ((u32)&((SceneLightTransitionArgs *)0)->lightColor == 0x10) ? 1 : -1];
typedef char SceneLightTransitionArgs_ambientColor_offset_must_be_0x20[
    ((u32)&((SceneLightTransitionArgs *)0)->ambientColor == 0x20) ? 1 : -1];
typedef char SceneLightTransitionArgs_value_offset_must_be_0x30[
    ((u32)&((SceneLightTransitionArgs *)0)->value == 0x30) ? 1 : -1];

#endif /* BTL_SCENE_LIGHTING_TASK_ARGS_H */
