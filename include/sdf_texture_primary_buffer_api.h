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

#endif /* SDF_TEXTURE_PRIMARY_BUFFER_API_H */
