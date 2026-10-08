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
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
} EffSolidRectParams;

typedef char EffScreenDrawParams_size_must_be_0x30[(sizeof(EffScreenDrawParams) == 0x30) ? 1 : -1];
typedef char EffScreenDrawParams_source_offset_must_be_0[(u32)&((EffScreenDrawParams *)0)->source == 0x00 ? 1 : -1];
typedef char EffScreenDrawParams_pad_offset_must_be_0x28[(u32)&((EffScreenDrawParams *)0)->pad28 == 0x28 ? 1 : -1];
typedef char EffSolidRectParams_size_must_be_0x18[(sizeof(EffSolidRectParams) == 0x18) ? 1 : -1];
typedef char EffSolidRectParams_blend_offset_must_be_4[(u32)&((EffSolidRectParams *)0)->blendControl == 0x04 ? 1 : -1];
typedef char EffSolidRectParams_left_offset_must_be_8[(u32)&((EffSolidRectParams *)0)->left == 0x08 ? 1 : -1];
typedef char EffSolidRectParams_bottom_offset_must_be_0x14[(u32)&((EffSolidRectParams *)0)->bottom == 0x14 ? 1 : -1];

EffScreenDrawParams *effGetLoadDescA(void);
EffScreenDrawParams *effGetLoadDescD(void);
EffScreenDrawParams *effGetCh70Params(void);
EffScreenDrawParams *effGetCh73Params(void);
EffSolidRectParams *effGetCh74Params(void);
EffSolidRectParams *effEventGetSolidRectangleSetupParams(void);
void effEventSetSolidRectangleParameters(EffSolidRectParams *parameters);
void effCopyColorRectangleParameters(EffSolidRectParams *parameters);

#endif
