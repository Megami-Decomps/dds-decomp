#ifndef SDF_TEXTURE_PRIMARY_BUFFER_API_H
#define SDF_TEXTURE_PRIMARY_BUFFER_API_H

#include "common.h"

struct sdfTexHandle_t;
struct sdfTexPacket_t;

/* Requires a non-NULL texture; returns its primary buffer as a borrowed view. */
struct sdfTexPacket_t *sdfTexGetPacket(const struct sdfTexHandle_t *texture);

/* Requires a non-NULL texture; returns 0 for no primary buffer, otherwise
 * ((gifTagWord & 0x7fff) + 1) * 16 bytes. */
s32 sdfTexGetPacketLength(const struct sdfTexHandle_t *texture);

/* Returns the texture-owned secondary buffer as a borrowed view. If absent,
 * initializes and stores it through sdfTexMakeGIFPacketCtx2 first. */
struct sdfTexPacket_t *sdfTexGetPacketCtx2(struct sdfTexHandle_t *texture);

/* Requires a non-NULL texture; returns 0 if no buffer, else
 * ((gifTagWord & 0x7fff) + 1) * 16 bytes. Does not initialize it. */
s32 sdfTexGetPacketCtx2Length(const struct sdfTexHandle_t *texture);

#endif /* SDF_TEXTURE_PRIMARY_BUFFER_API_H */
