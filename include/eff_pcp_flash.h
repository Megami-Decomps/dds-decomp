#ifndef EFF_PCP_FLASH_H
#define EFF_PCP_FLASH_H

#include "eff.h"

/* Copied input prefixes for the larger, title-local flash work records. */
typedef struct PcpFlashScalingOrbitParams {
    f32 origin[3];
    u8 pad0C[4];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[3];
    s32 lifetime;
    s32 scaleRampTime;
    u32 randomRange;
    u32 colorA;
    u32 colorB;
    f32 upSpan;
    f32 acrossSpan;
    f32 orbitRadius;
    f32 maxScale;
    f32 tilt;
    f32 angularStep;
    u32 unk44;
} PcpFlashScalingOrbitParams;

typedef struct PcpFlashAccumulatingParams {
    f32 origin[3];
    u8 pad0C[4];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[3];
    s32 lifetime;
    u32 randomRange;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 unk30;
    f32 unk34;
    f32 orbitRadius;
    f32 unk3C;
    f32 tilt;
    f32 increment;
    f32 radialStep;
    u32 unk4C;
} PcpFlashAccumulatingParams;

typedef struct PcpFlashOrbitArcParams {
    f32 origin[3];
    u8 pad0C[4];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[3];
    s32 lifetime;
    u32 randomRange;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 angularStep;
    u32 unk54;
} PcpFlashOrbitArcParams;

typedef struct PcpFlashFadingOrbitParams {
    f32 origin[3];
    u8 pad0C[4];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[3];
    s32 lifetime;
    s32 scaleRampTime;
    u32 randomRange;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 upSpan;
    f32 acrossSpan;
    f32 orbitRadius;
    f32 maxScale;
    f32 tilt;
    f32 angularStep;
    u32 unk4C;
} PcpFlashFadingOrbitParams;

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

typedef char PcpFlashScalingOrbitParamsSizeCheck[
    sizeof(PcpFlashScalingOrbitParams) == 0x48 ? 1 : -1];
typedef char PcpFlashAccumulatingParamsSizeCheck[
    sizeof(PcpFlashAccumulatingParams) == 0x50 ? 1 : -1];
typedef char PcpFlashOrbitArcParamsSizeCheck[
    sizeof(PcpFlashOrbitArcParams) == 0x58 ? 1 : -1];
typedef char PcpFlashFadingOrbitParamsSizeCheck[
    sizeof(PcpFlashFadingOrbitParams) == 0x50 ? 1 : -1];
typedef char PcpFlashTrianglePulseParamsSizeCheck[
    sizeof(PcpFlashTrianglePulseParams) == 0x30 ? 1 : -1];
typedef char PcpFlashTrianglePulseParamsCountOffsetCheck[
    ((u32)&((PcpFlashTrianglePulseParams *)0)->particleCount) == 0x10 ? 1 : -1];
typedef char PcpFlashTrianglePulseParamsDrawModeOffsetCheck[
    ((u32)&((PcpFlashTrianglePulseParams *)0)->drawMode) == 0x2C ? 1 : -1];

#endif
