#ifndef EFF_TRACK_POLY_DATA_H
#define EFF_TRACK_POLY_DATA_H

#include "sdf.h"

/* Owner record stored after the track's point and gradient buffers. */
typedef struct dds3HermitianTrackPoly_tag {
    u32 drawTarget;             /* 0x00: surface or special draw target */
    u32 color;                  /* 0x04 */
    s32 count;                  /* 0x08: slots, including the reserved pair */
    s32 activePointCount;       /* 0x0C */
    s32 position;               /* 0x10 */
    s32 step;                   /* 0x14 */
    u128 *points;                /* 0x18 */
    u32 *colors;                 /* 0x1C: gradient entries after points */
    sdfMat_t *nodeHandle;        /* 0x20: owned draw asset */
    sdfMemHandle_t *resourceHandle; /* 0x24: containing allocation */
} dds3HermitianTrackPoly_tag;

typedef char EffTrackPolyData_size_must_be_0x28[
    (sizeof(dds3HermitianTrackPoly_tag) == 0x28) ? 1 : -1];
typedef char EffTrackPolyData_points_at_0x18[
    ((u32)&((dds3HermitianTrackPoly_tag *)0)->points == 0x18) ? 1 : -1];
typedef char EffTrackPolyData_resourceHandle_at_0x24[
    ((u32)&((dds3HermitianTrackPoly_tag *)0)->resourceHandle == 0x24) ? 1 : -1];

#endif /* EFF_TRACK_POLY_DATA_H */
