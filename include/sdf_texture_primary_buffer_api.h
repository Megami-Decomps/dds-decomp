#ifndef SDF_TEXTURE_PRIMARY_BUFFER_API_H
#define SDF_TEXTURE_PRIMARY_BUFFER_API_H

#include "common.h"

struct SdfTex;
struct SdfTexBuf;

/* Requires a non-NULL texture; returns its primary buffer as a borrowed view. */
struct SdfTexBuf *sdfTexGetPrimaryBuffer(struct SdfTex *texture);

/* Requires a non-NULL texture; returns 0 for no primary buffer, otherwise
 * ((gifTagWord & 0x7fff) + 1) * 16 bytes. */
s32 sdfTexGetPrimaryBufferSize(struct SdfTex *texture);

/* Returns the texture-owned secondary buffer as a borrowed view. If absent,
 * initializes and stores it through sdfTexCreateSecondPacket first. */
struct SdfTexBuf *sdfTexGetOrInitializeSecondaryBuffer(struct SdfTex *texture);

/* Requires a non-NULL texture; returns 0 if no buffer, else
 * ((gifTagWord & 0x7fff) + 1) * 16 bytes. Does not initialize it. */
s32 sdfTexGetSecondaryBufferSize(struct SdfTex *texture);

#endif /* SDF_TEXTURE_PRIMARY_BUFFER_API_H */
