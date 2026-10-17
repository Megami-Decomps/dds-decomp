#ifndef PAR_RELEASE_RECORD_H
#define PAR_RELEASE_RECORD_H

#include "common.h"
#include "eff_vertex_draw_request.h"

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
    EffVertexDrawRequest request;
    struct SdfAsset *asset;
    u128 *positions;
    u32 (*texcoords)[2];
    u128 *normals;
    u32 *colors;
    struct ParReleaseRecord *next;
    s32 unk58;
} ParReleaseRecord;

typedef char ParReleaseRecordSizeCheck[
    sizeof(ParReleaseRecord) == 0x5C ? 1 : -1];

#endif
