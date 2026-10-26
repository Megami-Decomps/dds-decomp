#ifndef SDF_TEXTURE_SUBMIT_H
#define SDF_TEXTURE_SUBMIT_H

#include "sdf.h"

/* Select whether an upload copies its pixels or borrows them until transfer. */
typedef enum SdfTextureUploadOwnership {
    SDF_TEXTURE_UPLOAD_COPY = 0,
    SDF_TEXTURE_UPLOAD_BORROW = 1
} SdfTextureUploadOwnership;

/* Reads the source bytes and returns its cursor advanced past the submitted image. */
const u8 *sdfTexTransData(u32 destination, s32 width, s32 height, u32 format,
                    const u8 *pixels,
                    SdfTextureUploadOwnership borrowPixels);

/* Submit a palette using dimensions selected from the texture pixel format. */
const u8 *sdfTexTransCLUT(sdfTexHandle_t *texture, u32 destination,
                    const u8 *pixels,
                    SdfTextureUploadOwnership borrowPixels);

#endif
