#ifndef SDF_ASSET_PACKETS_H
#define SDF_ASSET_PACKETS_H

#include "common.h"

struct sdfMat_t;
struct SdfDmaTagHeader;

/* Write the reference header for one buffered asset draw entry and return
 * the packet payload immediately after the header. */
struct SdfDmaTagHeader *sdfMatMakePacket(struct sdfMat_t *asset,
                                                           struct SdfDmaTagHeader *tag,
                                                           s32 frame);

#endif /* SDF_ASSET_PACKETS_H */
