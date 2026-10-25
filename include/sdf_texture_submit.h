#ifndef SDF_TEXTURE_SUBMIT_H
#define SDF_TEXTURE_SUBMIT_H

#include "sdf.h"

/* Select whether an upload copies its pixels or borrows them until transfer. */
typedef enum SdfTextureUploadOwnership {
    SDF_TEXTURE_UPLOAD_COPY = 0,
    SDF_TEXTURE_UPLOAD_BORROW = 1
} SdfTextureUploadOwnership;

/* Returns the input pointer advanced by the submitted image byte count. */
u8 *sdfTexTransData(u32 destination, s32 width, s32 height, u32 format, u8 *pixels,
                    SdfTextureUploadOwnership borrowPixels);

/* Submit a palette using dimensions selected from the texture pixel format. */
u8 *sdfTexTransCLUT(sdfTexHandle_t *texture, u32 destination, u8 *pixels,
                    SdfTextureUploadOwnership borrowPixels);

#endif
