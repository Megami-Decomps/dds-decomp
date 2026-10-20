#ifndef PAR_RELEASE_RECORD_H
#define PAR_RELEASE_RECORD_H

#include "common.h"
#include "eff_packet_params.h"

struct SdfMemBlock;
struct SdfAsset;

/* The constructor appends this owner after its position and attribute streams. */
typedef struct ParReleaseRecord {
    s16 flags;
    u8 opaque02[2];
    s32 capacity;
    s32 unk08;
    s32 drawCount;
    struct SdfMemBlock *allocation;
    EffPacketParams request;
    struct SdfAsset *asset;
    u128 *positions;
    f32 (*texcoords)[2];
    u128 *normals;
    u32 *colors;
    struct ParReleaseRecord *next;
    s32 unk58;
} ParReleaseRecord;

typedef char ParReleaseRecordSizeCheck[
    sizeof(ParReleaseRecord) == 0x5C ? 1 : -1];

#endif
