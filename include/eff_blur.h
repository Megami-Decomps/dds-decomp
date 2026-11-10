#ifndef EFF_BLUR_H
#define EFF_BLUR_H

#include "common.h"
#include "sdf_texture_draw_packet.h"
#include "effect.h"

struct sdfTexHandle_t;
struct sdfDrawTag_t;

/* Reset burst state and selected quad fields from the owning work parameters;
 * corner coordinates remain for the slot's updater to recompute. */
void dds3EffectRipBlurPartInit(effBlurScaleWork_t *work, effBlurScaleSlot_t *slot);
/* Allocate the native effect draw packet from its height and GS flags. */
SdfDrawPacket *effPkt2DAllocTriStrip(s32 height, s32 flags);

/* Factories accept the serialized parameter prefix used by effect callbacks. */
effBlurScatterWork_t *dds3EffectDistortionBlurCreate(dds3Effect_DistortionBlurParam_t *params);
void dds3EffectDistortionBlurDelete(effBlurScatterWork_t *work);
void dds3EffectDistortionBlurDraw(effBlurScatterWork_t *work);
effBlurScaleWork_t *dds3EffectRippleBlurCreate(dds3Effect_RippleBlurParam_t *params);
void dds3EffectRippleBlurDelete(effBlurScaleWork_t *work);

void dds3EffectBlurDrawAlphaMulti(struct sdfDrawTag_t *list, const dds3Effect_Blur_t *source,
                                   u8 fixedPointCoordinates);
void dds3EffectBlurDraw(const dds3Effect_Blur_t *source, struct sdfTexHandle_t *resource, u8 fixedPointCoordinates);
void dds3EffectBlur(const dds3Effect_Blur_t *source);
void effBlurDrawFramebufferQuad(const dds3Effect_Blur_t *source);
void effDrawBlurFixedPointRectangle(effBlurTemplate_t *owner);
/* Borrow the live textured-blur owner; callers do not acquire or release it. */
effBlurTemplate_t *Eve_PartialMultiBlurGetParams(void);
/* Borrow the live scatter-blur owner; callers do not acquire or release it. */
effBlurScatterWork_t *Eve_DistortionBlurGetParams(void);
effBlurTemplate_t *dds3EffectPartialMultiBlurCreate(const dds3Effect_PartialMultiBlurParam_t *body);
void Eve_PartialMultiBlurSetParam(dds3Effect_PartialMultiBlurParam_t *parameters);
void Eve_DistortionBlurSetParam(dds3Effect_DistortionBlurParam_t *parameters);
void Eve_RippleBlurSetParam(dds3Effect_RippleBlurParam_t *parameters);
void dds3EffectPartialMultiBlurDelete(effBlurTemplate_t *owner);
void dds3EffectPartialMultiBlurDraw(effBlurTemplate_t *owner);

#endif
