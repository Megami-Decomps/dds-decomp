#ifndef SDF_TEXTURED_RECT_H
#define SDF_TEXTURED_RECT_H

#include "common.h"

struct SdfTex;

#ifdef VERSION_DDS2
void func_00108EC0(s32 x, s32 y, s32 width, s32 height,
                   s32 u, s32 v, s32 textureWidth, s32 textureHeight,
                   u32 color0, u32 color1, u32 color2, u32 color3,
                   struct SdfTex *texture);
#else
void func_00108FA0(s32 x, s32 y, s32 width, s32 height,
                   s32 u, s32 v, s32 textureWidth, s32 textureHeight,
                   u32 color0, u32 color1, u32 color2, u32 color3,
                   struct SdfTex *texture);
#endif

#endif /* SDF_TEXTURED_RECT_H */
