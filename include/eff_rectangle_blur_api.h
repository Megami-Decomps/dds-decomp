#ifndef EFF_RECTANGLE_BLUR_API_H
#define EFF_RECTANGLE_BLUR_API_H

#include "eff_blur.h"

/* Copy the 0x28-byte parameter block into the event rectangle-blur state. */
void effCopyRectangleBlurParameters(const EffBlurQuad *parameters);

#endif
