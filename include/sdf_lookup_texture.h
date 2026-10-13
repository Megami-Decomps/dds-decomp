#ifndef SDF_LOOKUP_TEXTURE_H
#define SDF_LOOKUP_TEXTURE_H

#include "sdf_texture_file.h"

/* Generated 128x64 grayscale power lookup texture: a serialized texture
 * header, 256 RGBA palette words, then the full 64x128 index image. */
typedef struct SdfGeneratedLookupTexture {
    SdfTextureFileHeader header; /* 0x0000 */
    u32 palette[256];            /* 0x0040 */
    u8 indices[64][128];         /* 0x0440 */
} SdfGeneratedLookupTexture;

typedef char SdfGeneratedLookupTexture_size_must_be_0x2440[
    (sizeof(SdfGeneratedLookupTexture) == 0x2440) ? 1 : -1];
typedef char SdfGeneratedLookupTexture_palette_must_be_at_0x40[
    ((u32)&((SdfGeneratedLookupTexture *)0)->palette == 0x40) ? 1 : -1];
typedef char SdfGeneratedLookupTexture_indices_must_be_at_0x440[
    ((u32)&((SdfGeneratedLookupTexture *)0)->indices == 0x440) ? 1 : -1];

#endif /* SDF_LOOKUP_TEXTURE_H */
