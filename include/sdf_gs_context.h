#ifndef SDF_GS_CONTEXT_H
#define SDF_GS_CONTEXT_H

#include "sdf_gs_packet.h"

/* Four A+D writes define one GS drawing context, without a DMA/GIF header. */
typedef struct SdfGsContextRegisters {
    SdfGsRegisterWrite frame;
    SdfGsRegisterWrite zbuf;
    SdfGsRegisterWrite xyoffset;
    SdfGsRegisterWrite scissor;
} SdfGsContextRegisters;

typedef char SdfGsContextRegisters_size_must_be_0x40[
    (sizeof(SdfGsContextRegisters) == 0x40) ? 1 : -1];
typedef char SdfGsContextRegisters_zbuf_at_0x10[
    ((u32)&((SdfGsContextRegisters *)0)->zbuf == 0x10) ? 1 : -1];
typedef char SdfGsContextRegisters_xyoffset_at_0x20[
    ((u32)&((SdfGsContextRegisters *)0)->xyoffset == 0x20) ? 1 : -1];
typedef char SdfGsContextRegisters_scissor_at_0x30[
    ((u32)&((SdfGsContextRegisters *)0)->scissor == 0x30) ? 1 : -1];

void sdfBuildFrameDepthScissorPacket(SdfGsContextRegisters *packet,
    s32 frameAddress, s32 width, s32 height, s32 frameFormat,
    s32 depthAddress, s32 depthFormat, s32 fieldOffset, s32 gsContext);

#endif
