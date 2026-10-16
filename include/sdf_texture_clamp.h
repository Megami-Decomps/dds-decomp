#ifndef SDF_TEXTURE_CLAMP_H
#define SDF_TEXTURE_CLAMP_H

#include "common.h"

/* Serialized byte consumed by the GS texture-state packet builder. */
typedef u8 SdfTextureClampMode;

enum {
    SDF_TEXTURE_CLAMP_MODE_DEFAULT = 0xFF,
    SDF_TEXTURE_CLAMP_STATE_MASK = 0x0F
};

struct SdfTex;
void sdfTexSetClampMode(struct SdfTex *texture, SdfTextureClampMode mode);

#endif /* SDF_TEXTURE_CLAMP_H */
