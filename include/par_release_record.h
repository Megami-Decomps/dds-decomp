#ifndef PAR_RELEASE_RECORD_H
#define PAR_RELEASE_RECORD_H

#include "common.h"
#include "eff_packet_params.h"

struct sdfMemHandle_t;
struct sdfMat_t;

/* The constructor appends this owner after its position and attribute streams. */
typedef struct dds3Polygon_t {
    s16 flags;
    u8 opaque02[2];
    s32 capacity;
    s32 unk08;
    s32 drawCount;
    struct sdfMemHandle_t *allocation;
    EffPacketParams request;
    struct sdfMat_t *asset;
    u128 *positions;
    f32 (*texcoords)[2];
    u128 *normals;
    u32 *colors;
    struct dds3Polygon_t *next;
    s32 unk58;
} dds3Polygon_t;

typedef char ParReleaseRecordSizeCheck[
    sizeof(dds3Polygon_t) == 0x5C ? 1 : -1];

#endif
