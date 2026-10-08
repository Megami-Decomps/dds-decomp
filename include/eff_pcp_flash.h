#ifndef EFF_PCP_FLASH_H
#define EFF_PCP_FLASH_H

#include "eff.h"

/* The factory copies this 0x30-byte parameter prefix into its larger work
 * allocation before appending particle and resource state. */
typedef struct PcpFlashTrianglePulseParams {
    f32 origin[3];
    u8 pad0C[4];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[3];
    s32 lifetime;
    s32 scaleRampTime;
    u32 colorA;
    u32 colorB;
    f32 maxScale;
    u32 drawMode;
} PcpFlashTrianglePulseParams;

PcpFlashTrianglePulseWork *effFlashRecordCreate(PcpFlashTrianglePulseParams *params);

typedef char PcpFlashTrianglePulseParamsSizeCheck[
    sizeof(PcpFlashTrianglePulseParams) == 0x30 ? 1 : -1];
typedef char PcpFlashTrianglePulseParamsCountOffsetCheck[
    ((u32)&((PcpFlashTrianglePulseParams *)0)->particleCount) == 0x10 ? 1 : -1];
typedef char PcpFlashTrianglePulseParamsDrawModeOffsetCheck[
    ((u32)&((PcpFlashTrianglePulseParams *)0)->drawMode) == 0x2C ? 1 : -1];

#endif
