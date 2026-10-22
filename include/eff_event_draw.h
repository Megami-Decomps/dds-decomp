#ifndef EFF_EVENT_DRAW_H
#define EFF_EVENT_DRAW_H

#include "eff_blur.h"

/* The stored screen record keeps eight bytes beyond the copied blur quad. */
typedef struct EffScreenDrawParams {
    EffBlurQuad source;
    u8 pad28[8];
} EffScreenDrawParams;

/* Complete color-rectangle draw parameters shared by setup and renderer code. */
typedef struct EffSolidRectParams {
    u32 color;
    s32 blendControl;
    /* Native movie blends interpolate these four corners with a 2x2 loop. */
    s32 corners[2][2];
} EffSolidRectParams;

typedef char EffScreenDrawParams_size_must_be_0x30[(sizeof(EffScreenDrawParams) == 0x30) ? 1 : -1];
typedef char EffScreenDrawParams_source_offset_must_be_0[(u32)&((EffScreenDrawParams *)0)->source == 0x00 ? 1 : -1];
typedef char EffScreenDrawParams_pad_offset_must_be_0x28[(u32)&((EffScreenDrawParams *)0)->pad28 == 0x28 ? 1 : -1];
typedef char EffSolidRectParams_size_must_be_0x18[(sizeof(EffSolidRectParams) == 0x18) ? 1 : -1];
typedef char EffSolidRectParams_blend_offset_must_be_4[(u32)&((EffSolidRectParams *)0)->blendControl == 0x04 ? 1 : -1];
typedef char EffSolidRectParams_left_offset_must_be_8[(u32)&((EffSolidRectParams *)0)->corners == 0x08 ? 1 : -1];
typedef char EffSolidRectParams_bottom_offset_must_be_0x14[(u32)&((EffSolidRectParams *)0)->corners[1][1] == 0x14 ? 1 : -1];

EffScreenDrawParams *effGetLoadDescA(void);
EffScreenDrawParams *effGetLoadDescD(void);
EffScreenDrawParams *Eve_BlurGetParam(void);
EffScreenDrawParams *effGetCh73Params(void);
EffSolidRectParams *Eve_FilterGetParams(void);
EffSolidRectParams *effEventGetSolidRectangleSetupParams(void);
void effEventSetSolidRectangleParameters(EffSolidRectParams *parameters);
void Eve_FilterSetParam(EffSolidRectParams *parameters);

/* Enable or disable each event-effect draw family. */
void Eve_BlurON(void);
void Eve_BlurOFF(void);
void Eve_PartialMultiBlurON(void);
void Eve_PartialMultiBlurOFF(void);
void Eve_DistortionBlurON(void);
void Eve_DistortionBlurOFF(void);
void Eve_RippleBlurON(void);
void Eve_RippleBlurOFF(void);
void effEnableFramebufferQuad(void);
void effDisableFramebufferQuad(void);
void Eve_FilterON(void);
void Eve_FilterOFF(void);
void Eve_PartialMultiFilterON(void);
void Eve_PartialMultiFilterOFF(void);

#endif
