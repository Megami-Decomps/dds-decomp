#ifndef MNU_CAMP_DISPLAY_H
#define MNU_CAMP_DISPLAY_H

#include "common.h"

typedef struct CampDisplayDefaults {
    s32 x;          /* 0x00 */
    s32 y;          /* 0x04 */
    u8 color[4];    /* 0x08 */
    f32 scaleY;     /* 0x0C */
    f32 scaleX;     /* 0x10 */
    s32 enabled;    /* 0x14 */
    s32 variant;    /* 0x18 */
#ifdef VERSION_DDS2
    s32 opaque1C;   /* 0x1C: DDS2-only initialized extension */
#endif
} CampDisplayDefaults;

typedef char CampDisplayDefaults_x_at_0[
    ((u32)&((CampDisplayDefaults *)0)->x == 0x00) ? 1 : -1];
typedef char CampDisplayDefaults_y_at_4[
    ((u32)&((CampDisplayDefaults *)0)->y == 0x04) ? 1 : -1];
typedef char CampDisplayDefaults_color_at_8[
    ((u32)&((CampDisplayDefaults *)0)->color == 0x08) ? 1 : -1];
typedef char CampDisplayDefaults_scaleY_at_C[
    ((u32)&((CampDisplayDefaults *)0)->scaleY == 0x0C) ? 1 : -1];
typedef char CampDisplayDefaults_scaleX_at_10[
    ((u32)&((CampDisplayDefaults *)0)->scaleX == 0x10) ? 1 : -1];
typedef char CampDisplayDefaults_enabled_at_14[
    ((u32)&((CampDisplayDefaults *)0)->enabled == 0x14) ? 1 : -1];
typedef char CampDisplayDefaults_variant_at_18[
    ((u32)&((CampDisplayDefaults *)0)->variant == 0x18) ? 1 : -1];
#ifdef VERSION_DDS2
typedef char CampDisplayDefaults_opaque1C_at_1C[
    ((u32)&((CampDisplayDefaults *)0)->opaque1C == 0x1C) ? 1 : -1];
typedef char CampDisplayDefaults_size_is_20[
    (sizeof(CampDisplayDefaults) == 0x20) ? 1 : -1];
#else
typedef char CampDisplayDefaults_size_is_1C[
    (sizeof(CampDisplayDefaults) == 0x1C) ? 1 : -1];
#endif

#endif /* MNU_CAMP_DISPLAY_H */
