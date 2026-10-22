#ifndef SDF_TEXTURED_RECT_H
#define SDF_TEXTURED_RECT_H

#include "common.h"

struct sdfTexHandle_t;

void evtSubmitTexturedGradientQuad(s32 x, s32 y, s32 width, s32 height,
                   s32 u, s32 v, s32 textureWidth, s32 textureHeight,
                   u32 color0, u32 color1, u32 color2, u32 color3,
                   struct sdfTexHandle_t *texture);

#endif /* SDF_TEXTURED_RECT_H */
