#ifndef SDF_TEX_COPY_IMAGE_DATA_API_H
#define SDF_TEX_COPY_IMAGE_DATA_API_H

#include "sdf.h"

/* Copy the texture's palette-sized byte range into its owned palette buffer. */
void sdfTexCopyImageData(SdfTex *texture, const u8 *source);

#endif /* SDF_TEX_COPY_IMAGE_DATA_API_H */
