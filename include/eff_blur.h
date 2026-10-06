#ifndef EFF_BLUR_H
#define EFF_BLUR_H

#include "common.h"

/* Shared rectangle and sampling payload; color is packed or read by channel. */
typedef struct EffBlurQuad {
    u32 color; /* Byte consumers read the packed word through u8 object access. */
    s32 blendControl;
    f32 angle;
    f32 displacement;
    s32 x;
    s32 y;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
} EffBlurQuad;

typedef char EffBlurQuadSizeCheck[sizeof(EffBlurQuad) == 0x28 ? 1 : -1];
typedef char EffBlurQuadCenterOffsetCheck[((u32)&((EffBlurQuad *)0)->x == 0x10) ? 1 : -1];
typedef char EffBlurQuadEdgesOffsetCheck[((u32)&((EffBlurQuad *)0)->left == 0x18) ? 1 : -1];

/* Random-position blur parameters and their allocated slot owner. */
typedef struct EffBlurScatterParams {
    s32 count;
    s32 delaySpread;
    f32 angleStep;
    u32 color;
    s32 unk10;
    f32 unk14;
    f32 unk18;
    s32 x;
    s32 y;
    s32 positionSpread;
    s32 size;
} EffBlurScatterParams;

typedef struct EffBlurScatterSlot {
    s32 delay;
    f32 angle;
    EffBlurQuad quad;
} EffBlurScatterSlot;

struct SdfMemBlock;
typedef struct EffBlurScatterWork {
    EffBlurScatterParams params;
    u32 sourceHandle;
    struct SdfMemBlock *allocation;
    EffBlurScatterSlot *slots;
} EffBlurScatterWork;

typedef char EffBlurScatterParamsSizeCheck[sizeof(EffBlurScatterParams) == 0x2C ? 1 : -1];
typedef char EffBlurScatterSlotSizeCheck[sizeof(EffBlurScatterSlot) == 0x30 ? 1 : -1];
typedef char EffBlurScatterWorkSizeCheck[sizeof(EffBlurScatterWork) == 0x38 ? 1 : -1];
typedef char EffBlurScatterAllocationOffsetCheck[((u32)&((EffBlurScatterWork *)0)->allocation == 0x30) ? 1 : -1];
typedef char EffBlurScatterSlotsOffsetCheck[((u32)&((EffBlurScatterWork *)0)->slots == 0x34) ? 1 : -1];

/* Phase-driven blur parameters and the scale-slot allocation they own. */
typedef struct EffBlurScaleParams {
    s32 count;
    f32 phaseStep;
    f32 spacing;
    u32 color;
    s32 unk10; /* Copied to the slot quad blend control. */
    f32 unk14; /* Copied to the slot quad angle. */
    f32 unk18; /* Displacement amplitude used by the native update. */
    f32 angleStep;
    s32 x;
    s32 y;
    s32 size;
} EffBlurScaleParams;

typedef struct EffBlurScaleSlot {
    f32 phase;
    f32 angle;
    EffBlurQuad quad;
} EffBlurScaleSlot;

typedef struct EffBlurScaleWork {
    EffBlurScaleParams params;
    u32 sourceHandle;
    struct SdfMemBlock *allocation;
    EffBlurScaleSlot *slots;
} EffBlurScaleWork;

typedef char EffBlurScaleParamsSizeCheck[sizeof(EffBlurScaleParams) == 0x2C ? 1 : -1];
typedef char EffBlurScaleSlotSizeCheck[sizeof(EffBlurScaleSlot) == 0x30 ? 1 : -1];
typedef char EffBlurScaleWorkSizeCheck[sizeof(EffBlurScaleWork) == 0x38 ? 1 : -1];
typedef char EffBlurScaleAllocationOffsetCheck[((u32)&((EffBlurScaleWork *)0)->allocation == 0x30) ? 1 : -1];
typedef char EffBlurScaleSlotsOffsetCheck[((u32)&((EffBlurScaleWork *)0)->slots == 0x34) ? 1 : -1];

/* Factories accept the serialized parameter prefix used by effect callbacks. */
EffBlurScatterWork *func_00186F90(void *params);
EffBlurScatterWork *func_0018EBC8(void *params);
void effBlurReleaseFirstResource(EffBlurScatterWork *work);
void func_00187098(EffBlurScatterWork *work);
void func_0018ECD0(EffBlurScatterWork *work);

void effAppendBlurRectanglePackets(void *list, EffBlurQuad *source, u8 fixedPointCoordinates);
void effDrawBlurSource(EffBlurQuad *source, s32 resource, u8 fixedPointCoordinates);

#endif
