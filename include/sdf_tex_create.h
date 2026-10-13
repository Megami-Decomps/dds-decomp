#ifndef SDF_TEX_CREATE_H
#define SDF_TEX_CREATE_H

#include "common.h"

struct SdfTex;

/* Allocate and register a texture. The shared registration helper initializes
 * its ordinary reference count to one; the caller releases that reference with
 * sdfTexReleaseReference or its deferred form. */
struct SdfTex *sdfTexCreateWithAllocatedResources(s32 width, s32 height,
    u32 pixelFormat, u32 maxMipLevel, u32 clutFormat, u32 paletteCount);

#endif /* SDF_TEX_CREATE_H */
