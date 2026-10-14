#ifndef EFF_PCP_BEAM_DRAW_PARAMS_H
#define EFF_PCP_BEAM_DRAW_PARAMS_H

#include "common.h"

/* Beam batch request prefix consumed by effCreateVertexDrawPacket. The
 * optional streams and allocator/depth tail remain opaque for this caller. */
typedef struct EffPCPBeamDrawParams {
    u16 parameterCount;
    u16 vertexCount;
    u16 primitive;
    u16 mask;
    u32 color;
    void *parameters;
    f32 *points;
    u8 opaque14[0xC];
    u32 *colors;
    u8 opaque24[8];
} EffPCPBeamDrawParams;

typedef char EffPCPBeamDrawParamsSize[
    (sizeof(EffPCPBeamDrawParams) == 0x2C) ? 1 : -1];
typedef char EffPCPBeamDrawParamsOffsets[
    ((u32)&((EffPCPBeamDrawParams *)0)->parameterCount == 0x00 &&
     (u32)&((EffPCPBeamDrawParams *)0)->vertexCount == 0x02 &&
     (u32)&((EffPCPBeamDrawParams *)0)->primitive == 0x04 &&
     (u32)&((EffPCPBeamDrawParams *)0)->mask == 0x06 &&
     (u32)&((EffPCPBeamDrawParams *)0)->color == 0x08 &&
     (u32)&((EffPCPBeamDrawParams *)0)->parameters == 0x0C &&
     (u32)&((EffPCPBeamDrawParams *)0)->points == 0x10 &&
     (u32)&((EffPCPBeamDrawParams *)0)->opaque14 == 0x14 &&
     (u32)&((EffPCPBeamDrawParams *)0)->colors == 0x20 &&
     (u32)&((EffPCPBeamDrawParams *)0)->opaque24 == 0x24) ? 1 : -1];

#endif
