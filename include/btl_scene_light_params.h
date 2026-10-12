#ifndef BTL_SCENE_LIGHT_PARAMS_H
#define BTL_SCENE_LIGHT_PARAMS_H

#include "common.h"

/* Position and two RGB values passed to the linkage-scene effect task. */
typedef struct BtlSceneLightParams {
    f32 position[3]; /* 0x00 */
    u8 opaque0C[4]; /* 0x0C */
    f32 color[3]; /* 0x10 */
    u8 opaque1C[4]; /* 0x1C */
    f32 secondaryColor[3]; /* 0x20 */
    u8 opaque2C[4]; /* 0x2C */
} BtlSceneLightParams;

typedef char BtlSceneLightParams_size_must_be_0x30[
    (sizeof(BtlSceneLightParams) == 0x30) ? 1 : -1];
typedef char BtlSceneLightParams_position_offset_must_be_0[
    ((u32)&((BtlSceneLightParams *)0)->position == 0x00) ? 1 : -1];
typedef char BtlSceneLightParams_opaque0C_offset_must_be_0x0C[
    ((u32)&((BtlSceneLightParams *)0)->opaque0C == 0x0C) ? 1 : -1];
typedef char BtlSceneLightParams_color_offset_must_be_0x10[
    ((u32)&((BtlSceneLightParams *)0)->color == 0x10) ? 1 : -1];
typedef char BtlSceneLightParams_opaque1C_offset_must_be_0x1C[
    ((u32)&((BtlSceneLightParams *)0)->opaque1C == 0x1C) ? 1 : -1];
typedef char BtlSceneLightParams_secondaryColor_offset_must_be_0x20[
    ((u32)&((BtlSceneLightParams *)0)->secondaryColor == 0x20) ? 1 : -1];
typedef char BtlSceneLightParams_opaque2C_offset_must_be_0x2C[
    ((u32)&((BtlSceneLightParams *)0)->opaque2C == 0x2C) ? 1 : -1];

#endif /* BTL_SCENE_LIGHT_PARAMS_H */
