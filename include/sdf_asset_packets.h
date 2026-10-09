#ifndef SDF_ASSET_PACKETS_H
#define SDF_ASSET_PACKETS_H

#include "common.h"

struct SdfAsset;
struct SdfNode;

/* Write the reference header for one buffered asset draw entry and return
 * the packet payload immediately after the header. */
void *sdfInitAssetDrawEntryReferenceNode(struct SdfAsset *asset, struct SdfNode *node,
                                s32 frame);

#endif /* SDF_ASSET_PACKETS_H */
