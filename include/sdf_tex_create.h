#ifndef SDF_TEX_CREATE_H
#define SDF_TEX_CREATE_H

#include "common.h"
#include "sdf_texture_format.h"

struct sdfTexHandle_t;

/* Allocate and register a texture. The shared registration helper initializes
 * its ordinary reference count to one; the caller releases that reference with
 * sdfTexReleaseReference or its deferred form. */
struct sdfTexHandle_t *sdfTexAlloc(s32 width, s32 height,
    SdfGsPixelStorageFormat pixelFormat, u32 maxMipLevel,
    SdfGsPixelStorageFormat clutFormat, u32 paletteCount);

#endif /* SDF_TEX_CREATE_H */
