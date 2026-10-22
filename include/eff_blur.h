#ifndef EFF_BLUR_H
#define EFF_BLUR_H

#include "common.h"
#include "sdf_texture_draw_packet.h"

struct SdfTex;

/* Shared rectangle and sampling payload; color is packed or read by channel. */
typedef struct EffBlurQuad {
    u32 color; /* Byte consumers read the packed word through u8 object access. */
    s32 blendControl;
    f32 angle;
    f32 displacement;
    /* Native movie blends index the renderer position as a two-element vector. */
    s32 position[2];
    /* Native movie blends interpolate these four corners with a 2x2 loop. */
    s32 corners[2][2];
} EffBlurQuad;

/* The textured blur allocation stores this copied body followed by a
 * separately acquired source texture. */
typedef struct EffBlurTemplateBody {
    s32 extent;
    EffBlurQuad source;
} EffBlurTemplateBody;

typedef struct EffBlurTemplate {
    EffBlurTemplateBody body;
    struct SdfTex *texture;
} EffBlurTemplate;

typedef char EffBlurQuadSizeCheck[sizeof(EffBlurQuad) == 0x28 ? 1 : -1];
typedef char EffBlurQuadCenterOffsetCheck[((u32)&((EffBlurQuad *)0)->position == 0x10) ? 1 : -1];
typedef char EffBlurQuadEdgesOffsetCheck[((u32)&((EffBlurQuad *)0)->corners == 0x18) ? 1 : -1];
typedef char EffBlurTemplateBodySizeCheck[sizeof(EffBlurTemplateBody) == 0x2C ? 1 : -1];
typedef char EffBlurTemplateSourceOffsetCheck[((u32)&((EffBlurTemplateBody *)0)->source == 0x04) ? 1 : -1];
typedef char EffBlurTemplateSizeCheck[sizeof(EffBlurTemplate) == 0x30 ? 1 : -1];
typedef char EffBlurTemplateResourceOffsetCheck[((u32)&((EffBlurTemplate *)0)->texture == 0x2C) ? 1 : -1];

/* One ST/XYZ2 pair in the packed draw payload. */
typedef struct BlurPacketVertex {
    f32 s, t;
    u8 pad08[8];
    s32 x, y;
    u32 depth;
    u16 xyzControl;
    u8 pad1E[2];
} BlurPacketVertex;

typedef struct BlurPacketQuad {
    u32 color[4];
    BlurPacketVertex vertices[4];
} BlurPacketQuad;

/* Random-position blur parameters and their allocated slot owner. */
typedef struct EffBlurScatterParams {
    s32 count;
    s32 delaySpread;
    f32 angleStep;
    u32 color;
    s32 blendControl;
    f32 uvDisplacementAngleDegrees;
    f32 uvDisplacementAmplitude;
    /* Native movie blends index the renderer position as a two-element vector. */
    s32 position[2];
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
    struct SdfTex *texture;
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
    s32 blendControl; /* Copied to the slot quad blend control. */
    f32 uvDisplacementAngleDegrees; /* Copied to the slot quad angle. */
    f32 uvDisplacementAmplitude; /* Displacement amplitude used by the native update. */
    f32 angleStep;
    /* Native movie blends index the renderer position as a two-element vector. */
    s32 position[2];
    s32 size;
} EffBlurScaleParams;

typedef struct EffBlurScaleSlot {
    f32 phase;
    f32 angle;
    EffBlurQuad quad;
} EffBlurScaleSlot;

typedef struct EffBlurScaleWork {
    EffBlurScaleParams params;
    struct SdfTex *texture;
    struct SdfMemBlock *allocation;
    EffBlurScaleSlot *slots;
} EffBlurScaleWork;

typedef char EffBlurScaleParamsSizeCheck[sizeof(EffBlurScaleParams) == 0x2C ? 1 : -1];
typedef char EffBlurScaleSlotSizeCheck[sizeof(EffBlurScaleSlot) == 0x30 ? 1 : -1];
typedef char EffBlurScaleWorkSizeCheck[sizeof(EffBlurScaleWork) == 0x38 ? 1 : -1];
typedef char EffBlurScaleAllocationOffsetCheck[((u32)&((EffBlurScaleWork *)0)->allocation == 0x30) ? 1 : -1];
typedef char EffBlurScaleSlotsOffsetCheck[((u32)&((EffBlurScaleWork *)0)->slots == 0x34) ? 1 : -1];

/* Reset burst state and selected quad fields from the owning work parameters;
 * corner coordinates remain for the slot's updater to recompute. */
void dds3EffectRipBlurPartInit(EffBlurScaleWork *work, EffBlurScaleSlot *slot);
/* Allocate the native effect draw packet from its height and GS flags. */
SdfDrawPacket *effPkt2DAllocTriStrip(s32 height, s32 flags);

/* Factories accept the serialized parameter prefix used by effect callbacks. */
EffBlurScatterWork *dds3EffectDistortionBlurCreate(EffBlurScatterParams *params);
void dds3EffectDistortionBlurDelete(EffBlurScatterWork *work);
void effBlurStepScatterSlotsAndDraw(EffBlurScatterWork *work);
EffBlurScaleWork *dds3EffectRippleBlurCreate(EffBlurScaleParams *params);
void dds3EffectRippleBlurDelete(EffBlurScaleWork *work);

void effAppendBlurRectanglePackets(void *list, const EffBlurQuad *source, u8 fixedPointCoordinates);
void effDrawBlurSource(const EffBlurQuad *source, struct SdfTex *resource, u8 fixedPointCoordinates);
void effDrawBlurRectangle(const EffBlurQuad *source);
void effBlurDrawFramebufferQuad(const EffBlurQuad *source);
void effDrawBlurFixedPointRectangle(EffBlurTemplate *owner);
/* Borrow the live textured-blur owner; callers do not acquire or release it. */
EffBlurTemplate *Eve_PartialMultiBlurGetParams(void);
/* Borrow the live scatter-blur owner; callers do not acquire or release it. */
EffBlurScatterWork *Eve_DistortionBlurGetParams(void);
EffBlurTemplate *dds3EffectPartialMultiBlurCreate(const EffBlurTemplateBody *body);
void Eve_PartialMultiBlurSetParam(EffBlurTemplateBody *parameters);
void Eve_DistortionBlurSetParam(EffBlurScatterParams *parameters);
void Eve_RippleBlurSetParam(EffBlurScaleParams *parameters);
void dds3EffectPartialMultiBlurDelete(EffBlurTemplate *owner);
void effDrawBlurPixelRectWithResource(EffBlurTemplate *owner);

#endif
