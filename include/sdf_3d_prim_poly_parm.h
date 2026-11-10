#ifndef SDF_3D_PRIM_POLY_PARM_H
#define SDF_3D_PRIM_POLY_PARM_H

#include "common.h"
#include "sdf_types.h"

/* A polygon handed to sdf3DPrimMakePoly, which derives the VIF packet from it
 * rather than keeping it. attr is the GS PRIM word; each non-null array below
 * adds its own bit to it and its own UNPACK to the packet. */
typedef struct
{
    s16 faceCount;
    s16 pointCount;
    u16 attr;
    u16 attrInhibit; /* inverted and ANDed into attr */
    u32 color;
    u32 *faceIndex; /* null uses sdf3DPrimDefaultFaceIndex */
    sdfVector4_t *vertex;
    sdfVector4_t *normal;
    sdfVector2_t *uv;
    sdfVector2_t *multiUv;
    u32 *vertexColor;
    void *allocator; /* called instead of sdfGrAllocPacketBuff when set */
    f32 zBias;
} sdf3DPrimPolyParm_t;

#endif /* SDF_3D_PRIM_POLY_PARM_H */
