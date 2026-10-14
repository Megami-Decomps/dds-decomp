#ifndef FLD_QUAD_STATE_H
#define FLD_QUAD_STATE_H

#include "common.h"
#include "sdf.h"

/* Stack state shared by the field resolver's positioned quad helpers. */
typedef struct FldQuadState {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 rowX;
    s32 rowY;
    s32 drawDepth;
    SdfListHead *packetList;
} FldQuadState;

typedef char FldQuadState_size_must_be_0x2C[
    sizeof(FldQuadState) == 0x2C ? 1 : -1];
typedef char FldQuadState_rowX_offset_must_be_0x1C[
    ((u32)&((FldQuadState *)0)->rowX) == 0x1C ? 1 : -1];
typedef char FldQuadState_rowY_offset_must_be_0x20[
    ((u32)&((FldQuadState *)0)->rowY) == 0x20 ? 1 : -1];
typedef char FldQuadState_drawDepth_offset_must_be_0x24[
    ((u32)&((FldQuadState *)0)->drawDepth) == 0x24 ? 1 : -1];
typedef char FldQuadState_packetList_offset_must_be_0x28[
    ((u32)&((FldQuadState *)0)->packetList) == 0x28 ? 1 : -1];

#endif
