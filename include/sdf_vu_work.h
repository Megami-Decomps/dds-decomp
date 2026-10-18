#ifndef SDF_VU_WORK_H
#define SDF_VU_WORK_H

#include "common.h"
#include "sdf.h"
#include "sdf_vu_blend_node.h"

/* The VU packet builders' work block, filled by
 * sdfInitializeVuWorkParameters from a command's parameter words. */
typedef struct {
    f32 matrix[4][4];      /* 0x00 */
    u16 param0;            /* 0x40 */
    s16 param1;            /* 0x42 */
    u32 selectedFlags;     /* 0x44 */
    u32 nextParam;         /* 0x48 */
    u32 flags;             /* 0x4C */
    s16 nodeCount;         /* 0x50 */
    s16 selectedTriangleCount; /* 0x52: rejected-triangle index count */
    SdfAssetEntry *node;   /* 0x54 */
    u32 packedColor;       /* 0x58: packed RGBA word */
    f32 offsetX;           /* 0x5C */
    f32 offsetY;           /* 0x60 */
    u32 dmaBase;           /* 0x64 */
    VuBlendNode *dmaAddrA; /* 0x68: next free ring node */
    VuBlendNode *dmaAddrB; /* 0x6C: ring wrap node */
    u32 dmaCountA;         /* 0x70 */
    u32 dmaCountB;         /* 0x74 */
    VuBlendNode *blendList; /* 0x78 */
    VuBlendNode **dmaEnd;  /* 0x7C: list tail link */
    u32 packetStart;       /* 0x80 */
    u32 header;            /* 0x84: aligned packet cursor + 0x30 */
    u32 *ringStart;        /* 0x88 */
    u32 *cursor;           /* 0x8C */
    u8 *payload;           /* 0x90 */
    u8 *strip;             /* 0x94 */
    u8 *positions;         /* 0x98 */
    u8 *normals;           /* 0x9C */
    u8 *coordinates;       /* 0xA0 */
    u8 *secondCoordinates; /* 0xA4 */
    u8 *vertexColors;      /* 0xA8 */
    f32 depth;             /* 0xAC */
} VuWork;

#endif /* SDF_VU_WORK_H */
