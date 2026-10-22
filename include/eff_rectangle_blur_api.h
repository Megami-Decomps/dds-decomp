#ifndef EFF_RECTANGLE_BLUR_API_H
#define EFF_RECTANGLE_BLUR_API_H

#include "eff_blur.h"

/* Copy the 0x28-byte parameter block into the event rectangle-blur state. */
void Eve_BlurSetParam(const dds3Effect_Blur_t *parameters);

#endif
