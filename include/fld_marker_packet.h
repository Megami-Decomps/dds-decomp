#ifndef FLD_MARKER_PACKET_H
#define FLD_MARKER_PACKET_H

#include "eff.h"

/* Stack-built input shared by the field marker packet producers and submitter. */
typedef struct FldMarkerPacket {
    f32 pos[3];
    u8 opaque0C[4];
    BillTextureQuad uv;
    f32 quad[8];
    s32 color;
    f32 scale;
} FldMarkerPacket;

typedef char FldMarkerPacketSizeCheck[
    (sizeof(FldMarkerPacket) == 0x48) ? 1 : -1];
typedef char FldMarkerPacketUvOffsetCheck[
    ((u32)&((FldMarkerPacket *)0)->uv == 0x10) ? 1 : -1];
typedef char FldMarkerPacketQuadOffsetCheck[
    ((u32)&((FldMarkerPacket *)0)->quad == 0x20) ? 1 : -1];
typedef char FldMarkerPacketColorOffsetCheck[
    ((u32)&((FldMarkerPacket *)0)->color == 0x40) ? 1 : -1];
typedef char FldMarkerPacketScaleOffsetCheck[
    ((u32)&((FldMarkerPacket *)0)->scale == 0x44) ? 1 : -1];

#endif
