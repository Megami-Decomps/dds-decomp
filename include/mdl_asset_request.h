#ifndef MDL_ASSET_REQUEST_H
#define MDL_ASSET_REQUEST_H

#include "common.h"

/* The API returns a group address in the same word used for its two non-ready
 * outcomes, so retain the signed 32-bit representation at the ABI boundary. */
typedef s32 MdlAssetRequestResult;

/* Zero queues asset loading; nonzero waits for the requested group to load. */
typedef s32 MdlAssetRequestMode;
enum {
    MDL_ASSET_REQUEST_NONBLOCKING = 0,
    MDL_ASSET_REQUEST_BLOCKING = 1,
};

enum {
    MDL_ASSET_REQUEST_QUEUED = 0,
    MDL_ASSET_REQUEST_ALREADY_PENDING = -1,
};

/* A nonblocking request returns QUEUED when it starts work and
 * ALREADY_PENDING when work for the key already exists. Otherwise the result
 * is the ready dds3ModelResrc_t address. */
MdlAssetRequestResult mdlPrepare(
    s32 group, s32 id, MdlAssetRequestMode requestMode);

#endif /* MDL_ASSET_REQUEST_H */
