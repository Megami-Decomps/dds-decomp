#ifndef UI_COLOR_H
#define UI_COLOR_H

#include "common.h"

/* Blends packed RGBA channels; weights above 0x100 are mirrored from 0x200. */
u32 uiBlendColors(u32 color0, u32 color1, u32 weight);

#endif /* UI_COLOR_H */
