#ifndef SDF_TEXTURE_SUBMIT_H
#define SDF_TEXTURE_SUBMIT_H

#include "sdf.h"

/* Zero copies into queue-owned storage; nonzero queues the input as borrowed until transfer. */
/* Returns the input pointer advanced by the submitted image byte count. */
u8 *sdfTexSubmitImageCopy(u32 destination, s32 width, s32 height, u32 format, u8 *pixels, s32 borrowPixels);

/* Submit a palette using dimensions selected from the texture pixel format. */
u8 *sdfTexSubmitPixelsForFormat(SdfTex *texture, u32 destination, u8 *pixels, s32 borrowPixels);

#endif
