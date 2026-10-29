#ifndef EFF_PACKET_PARAMS_H
#define EFF_PACKET_PARAMS_H

#include "common.h"

typedef struct sdf3DPrimPolyParm_t {
    s16 faceCount;
    s16 pointCount;
    u16 attr;
    u16 attrInhibit;
    u32 color;
    const u32 *faceIndex;
    u128 *vertex;
    u128 *normal;
    f32 *uv;
    u32 *multiUv;
    u32 *vertexColor;
    void *(*allocator)(s32);
    f32 zBias;
} sdf3DPrimPolyParm_t;

#endif
