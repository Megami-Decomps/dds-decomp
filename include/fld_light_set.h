#ifndef FLD_LIGHT_SET_H
#define FLD_LIGHT_SET_H

#include "common.h"

/* Shared row read from the sky-light resource table by both field resolvers. */
typedef struct FldLightSet {
    u8 fadeArea; /* 0x00: selects the fade state entry */
    u8 pad1[3];
    s32 displayStateSlot14; /* 0x04 */
    s32 fadeMode; /* 0x08 */
    s32 swayMode; /* 0x0C */
    u8 pad10[0xC];
    s32 fogNearValue; /* 0x1C: converted to the draw-vector float argument */
    s32 depthNearValue; /* 0x20 */
    s32 fogFarValue; /* 0x24 */
    s32 depthFarValue; /* 0x28 */
    s32 drawColorFixedR; /* 0x2C, divided by 256 before color packing */
    s32 drawColorFixedG; /* 0x30 */
    s32 drawColorFixedB; /* 0x34 */
    f32 lightColorAX;
    f32 lightColorAY;
    f32 lightColorAZ;
    f32 lightDirectionAX;
    f32 lightDirectionAY;
    f32 lightDirectionAZ;
    f32 lightColorBX;
    f32 lightColorBY;
    f32 lightColorBZ;
    f32 lightDirectionBX;
    f32 lightDirectionBY;
    f32 lightDirectionBZ;
    f32 lightColorCX;
    f32 lightColorCY;
    f32 lightColorCZ;
    f32 lightDirectionCX;
    f32 lightDirectionCY;
    f32 lightDirectionCZ;
    f32 backgroundColorX;
    f32 backgroundColorY;
    f32 backgroundColorZ;
    f32 unitColorA[3];
    f32 unitLightDirection[3];
    u8 padA4[0x30];
    f32 unitColorB[3];
} FldLightSet;

typedef char FldLightSet_size_must_be_0xE0[
    sizeof(FldLightSet) == 0xE0 ? 1 : -1];
typedef char FldLightSet_fadeMode_offset_must_be_0x08[
    ((u32)&((FldLightSet *)0)->fadeMode) == 0x08 ? 1 : -1];
typedef char FldLightSet_fogNear_offset_must_be_0x1C[
    ((u32)&((FldLightSet *)0)->fogNearValue) == 0x1C ? 1 : -1];
typedef char FldLightSet_drawColorFixedR_offset_must_be_0x2C[
    ((u32)&((FldLightSet *)0)->drawColorFixedR) == 0x2C ? 1 : -1];
typedef char FldLightSet_lightColorAX_offset_must_be_0x38[
    ((u32)&((FldLightSet *)0)->lightColorAX) == 0x38 ? 1 : -1];
typedef char FldLightSet_backgroundColorX_offset_must_be_0x80[
    ((u32)&((FldLightSet *)0)->backgroundColorX) == 0x80 ? 1 : -1];
typedef char FldLightSet_unitColorB_offset_must_be_0xD4[
    ((u32)&((FldLightSet *)0)->unitColorB) == 0xD4 ? 1 : -1];

#endif
