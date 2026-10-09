#ifndef SDF_GS_SCENE_STATE_H
#define SDF_GS_SCENE_STATE_H

#include "sdf_gs_packet.h"

/* A+D writes that draw the centered viewport bounds as a sprite. */
typedef struct SdfGsCenteredBoundsRegisters {
    SdfGsRegisterWrite test;
    SdfGsRegisterWrite primitive;
    SdfGsRegisterWrite rgbaq;
    SdfGsRegisterWrite xyz2[2];
} SdfGsCenteredBoundsRegisters;

typedef char SdfGsCenteredBoundsRegisters_size_must_be_0x50[
    (sizeof(SdfGsCenteredBoundsRegisters) == 0x50) ? 1 : -1];
typedef char SdfGsCenteredBoundsRegisters_primitive_at_0x10[
    ((u32)&((SdfGsCenteredBoundsRegisters *)0)->primitive == 0x10) ? 1 : -1];
typedef char SdfGsCenteredBoundsRegisters_rgbaq_at_0x20[
    ((u32)&((SdfGsCenteredBoundsRegisters *)0)->rgbaq == 0x20) ? 1 : -1];
typedef char SdfGsCenteredBoundsRegisters_xyz2_at_0x30[
    ((u32)&((SdfGsCenteredBoundsRegisters *)0)->xyz2 == 0x30) ? 1 : -1];

void sdfBuildCenteredViewBoundsPacket(SdfGsCenteredBoundsRegisters *packet,
    s32 width, s32 height, s32 unused0, s32 unused1);

/* Shared A+D state for GS primitive modes, color clamping, dithering and TEXA. */
typedef struct SdfGsDrawDefaultsRegisters {
    SdfGsRegisterWrite prmodecont;
    SdfGsRegisterWrite colclamp;
    SdfGsRegisterWrite dthe;
    SdfGsRegisterWrite texa;
} SdfGsDrawDefaultsRegisters;

typedef char SdfGsDrawDefaultsRegisters_size_must_be_0x40[
    (sizeof(SdfGsDrawDefaultsRegisters) == 0x40) ? 1 : -1];
typedef char SdfGsDrawDefaultsRegisters_colclamp_at_0x10[
    ((u32)&((SdfGsDrawDefaultsRegisters *)0)->colclamp == 0x10) ? 1 : -1];
typedef char SdfGsDrawDefaultsRegisters_dthe_at_0x20[
    ((u32)&((SdfGsDrawDefaultsRegisters *)0)->dthe == 0x20) ? 1 : -1];
typedef char SdfGsDrawDefaultsRegisters_texa_at_0x30[
    ((u32)&((SdfGsDrawDefaultsRegisters *)0)->texa == 0x30) ? 1 : -1];

void sdfInitDrawPacket(SdfGsDrawDefaultsRegisters *packet);

/* TEST/ALPHA A+D state for the two independent GS drawing contexts. */
typedef struct SdfGsSceneBlendRegisters {
    SdfGsBlendRegisters contextOne;
    SdfGsBlendRegisters contextTwo;
} SdfGsSceneBlendRegisters;

typedef char SdfGsSceneBlendRegisters_size_must_be_0x40[
    (sizeof(SdfGsSceneBlendRegisters) == 0x40) ? 1 : -1];
typedef char SdfGsSceneBlendRegisters_contextTwo_at_0x20[
    ((u32)&((SdfGsSceneBlendRegisters *)0)->contextTwo == 0x20) ? 1 : -1];

#endif
