#ifndef PAR_RELEASE_RECORD_H
#define PAR_RELEASE_RECORD_H

#include "common.h"

struct SdfMemBlock;
struct SdfAsset;

/* Shared particle resource-owner view. The release and texture accessors
 * evidence flags at +0, allocation at +0x10, and asset at +0x40; gaps remain
 * opaque. */
typedef struct ParReleaseRecord {
    u16 flags;
    u8 opaque_02[0x0E];
    struct SdfMemBlock *allocation;
    u8 opaque_14[0x2C];
    struct SdfAsset *asset;
} ParReleaseRecord;

typedef char ParReleaseRecordSizeCheck[
    sizeof(ParReleaseRecord) == 0x44 ? 1 : -1];

#endif
