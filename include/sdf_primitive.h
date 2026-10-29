#ifndef SDF_PRIMITIVE_H
#define SDF_PRIMITIVE_H

#include "common.h"

/* Native 0x2C VIF primitive request, shared by its quad/triangle producers.
 * Attribute buffers carry serialized words; coordinate floats retain their bits.
 * Native callers supply a positive vertex count and aligned 16-byte position/normal rows. */
#include "eff_packet_params.h"

typedef char sdf3DPrimPolyParm_t_size_0x2C[(sizeof(sdf3DPrimPolyParm_t) == 0x2C) ? 1 : -1];
typedef char sdf3DPrimPolyParm_t_faceCount_layout[((u32)&((sdf3DPrimPolyParm_t *)0)->faceCount == 0x00 && sizeof(((sdf3DPrimPolyParm_t *)0)->faceCount) == 2) ? 1 : -1];
typedef char sdf3DPrimPolyParm_t_pointCount_layout[((u32)&((sdf3DPrimPolyParm_t *)0)->pointCount == 0x02 && sizeof(((sdf3DPrimPolyParm_t *)0)->pointCount) == 2) ? 1 : -1];
typedef char sdf3DPrimPolyParm_t_attr_layout[((u32)&((sdf3DPrimPolyParm_t *)0)->attr == 0x04 && sizeof(((sdf3DPrimPolyParm_t *)0)->attr) == 2) ? 1 : -1];
typedef char sdf3DPrimPolyParm_t_attrInhibit_layout[((u32)&((sdf3DPrimPolyParm_t *)0)->attrInhibit == 0x06 && sizeof(((sdf3DPrimPolyParm_t *)0)->attrInhibit) == 2) ? 1 : -1];
typedef char sdf3DPrimPolyParm_t_color_layout[((u32)&((sdf3DPrimPolyParm_t *)0)->color == 0x08 && sizeof(((sdf3DPrimPolyParm_t *)0)->color) == 4) ? 1 : -1];
typedef char sdf3DPrimPolyParm_t_faceIndex_layout[((u32)&((sdf3DPrimPolyParm_t *)0)->faceIndex == 0x0C && sizeof(((sdf3DPrimPolyParm_t *)0)->faceIndex) == 4) ? 1 : -1];
typedef char sdf3DPrimPolyParm_t_vertex_layout[((u32)&((sdf3DPrimPolyParm_t *)0)->vertex == 0x10 && sizeof(((sdf3DPrimPolyParm_t *)0)->vertex) == 4) ? 1 : -1];
typedef char sdf3DPrimPolyParm_t_normal_layout[((u32)&((sdf3DPrimPolyParm_t *)0)->normal == 0x14 && sizeof(((sdf3DPrimPolyParm_t *)0)->normal) == 4) ? 1 : -1];
typedef char sdf3DPrimPolyParm_t_uv_layout[((u32)&((sdf3DPrimPolyParm_t *)0)->uv == 0x18 && sizeof(((sdf3DPrimPolyParm_t *)0)->uv) == 4) ? 1 : -1];
typedef char sdf3DPrimPolyParm_t_multiUv_layout[((u32)&((sdf3DPrimPolyParm_t *)0)->multiUv == 0x1C && sizeof(((sdf3DPrimPolyParm_t *)0)->multiUv) == 4) ? 1 : -1];
typedef char sdf3DPrimPolyParm_t_vertexColor_layout[((u32)&((sdf3DPrimPolyParm_t *)0)->vertexColor == 0x20 && sizeof(((sdf3DPrimPolyParm_t *)0)->vertexColor) == 4) ? 1 : -1];
typedef char sdf3DPrimPolyParm_t_allocator_layout[((u32)&((sdf3DPrimPolyParm_t *)0)->allocator == 0x24 && sizeof(((sdf3DPrimPolyParm_t *)0)->allocator) == 4) ? 1 : -1];
typedef char sdf3DPrimPolyParm_t_zBias_layout[((u32)&((sdf3DPrimPolyParm_t *)0)->zBias == 0x28 && sizeof(((sdf3DPrimPolyParm_t *)0)->zBias) == 4) ? 1 : -1];

#endif /* SDF_PRIMITIVE_H */
