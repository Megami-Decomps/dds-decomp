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

#endif
