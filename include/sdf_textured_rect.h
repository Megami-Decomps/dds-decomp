#ifndef SDF_TEXTURED_RECT_H
#define SDF_TEXTURED_RECT_H

#include "common.h"

struct SdfTex;

#ifdef VERSION_DDS2
void evtSubmitTexturedGradientQuad(s32 x, s32 y, s32 width, s32 height,
                   s32 u, s32 v, s32 textureWidth, s32 textureHeight,
                   u32 color0, u32 color1, u32 color2, u32 color3,
                   struct SdfTex *texture);
#else
void kwlnDrawTexturedColorQuad(s32 x, s32 y, s32 width, s32 height,
                   s32 u, s32 v, s32 textureWidth, s32 textureHeight,
                   u32 color0, u32 color1, u32 color2, u32 color3,
                   struct SdfTex *texture);
#endif

#endif /* SDF_TEXTURED_RECT_H */
