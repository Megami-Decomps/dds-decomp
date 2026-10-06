#ifndef SDF_PRIMITIVE_H
#define SDF_PRIMITIVE_H

#include "common.h"

/* Native 0x2C VIF primitive request, shared by its quad/triangle producers.
 * Attribute buffers carry serialized words; coordinate floats retain their bits.
 * Native callers supply a positive vertex count and aligned 16-byte position/normal rows. */
typedef struct SdfPrimitiveRequest {
    s16 stripWordCount;
    s16 vertexCount;
    u16 primitiveFlags;
    u16 clipMask;
    u32 color;
    const void *strip;
    const void *positions;
    const void *normals;
    const void *coordinates;
    const void *secondCoordinates;
    const void *vertexColors;
    void *(*allocate)(s32);
    f32 depth;
} SdfPrimitiveRequest;

typedef char SdfPrimitiveRequest_size_0x2C[(sizeof(SdfPrimitiveRequest) == 0x2C) ? 1 : -1];
typedef char SdfPrimitiveRequest_stripWordCount_layout[((u32)&((SdfPrimitiveRequest *)0)->stripWordCount == 0x00 && sizeof(((SdfPrimitiveRequest *)0)->stripWordCount) == 2) ? 1 : -1];
typedef char SdfPrimitiveRequest_vertexCount_layout[((u32)&((SdfPrimitiveRequest *)0)->vertexCount == 0x02 && sizeof(((SdfPrimitiveRequest *)0)->vertexCount) == 2) ? 1 : -1];
typedef char SdfPrimitiveRequest_primitiveFlags_layout[((u32)&((SdfPrimitiveRequest *)0)->primitiveFlags == 0x04 && sizeof(((SdfPrimitiveRequest *)0)->primitiveFlags) == 2) ? 1 : -1];
typedef char SdfPrimitiveRequest_clipMask_layout[((u32)&((SdfPrimitiveRequest *)0)->clipMask == 0x06 && sizeof(((SdfPrimitiveRequest *)0)->clipMask) == 2) ? 1 : -1];
typedef char SdfPrimitiveRequest_color_layout[((u32)&((SdfPrimitiveRequest *)0)->color == 0x08 && sizeof(((SdfPrimitiveRequest *)0)->color) == 4) ? 1 : -1];
typedef char SdfPrimitiveRequest_strip_layout[((u32)&((SdfPrimitiveRequest *)0)->strip == 0x0C && sizeof(((SdfPrimitiveRequest *)0)->strip) == 4) ? 1 : -1];
typedef char SdfPrimitiveRequest_positions_layout[((u32)&((SdfPrimitiveRequest *)0)->positions == 0x10 && sizeof(((SdfPrimitiveRequest *)0)->positions) == 4) ? 1 : -1];
typedef char SdfPrimitiveRequest_normals_layout[((u32)&((SdfPrimitiveRequest *)0)->normals == 0x14 && sizeof(((SdfPrimitiveRequest *)0)->normals) == 4) ? 1 : -1];
typedef char SdfPrimitiveRequest_coordinates_layout[((u32)&((SdfPrimitiveRequest *)0)->coordinates == 0x18 && sizeof(((SdfPrimitiveRequest *)0)->coordinates) == 4) ? 1 : -1];
typedef char SdfPrimitiveRequest_secondCoordinates_layout[((u32)&((SdfPrimitiveRequest *)0)->secondCoordinates == 0x1C && sizeof(((SdfPrimitiveRequest *)0)->secondCoordinates) == 4) ? 1 : -1];
typedef char SdfPrimitiveRequest_vertexColors_layout[((u32)&((SdfPrimitiveRequest *)0)->vertexColors == 0x20 && sizeof(((SdfPrimitiveRequest *)0)->vertexColors) == 4) ? 1 : -1];
typedef char SdfPrimitiveRequest_allocate_layout[((u32)&((SdfPrimitiveRequest *)0)->allocate == 0x24 && sizeof(((SdfPrimitiveRequest *)0)->allocate) == 4) ? 1 : -1];
typedef char SdfPrimitiveRequest_depth_layout[((u32)&((SdfPrimitiveRequest *)0)->depth == 0x28 && sizeof(((SdfPrimitiveRequest *)0)->depth) == 4) ? 1 : -1];

#endif /* SDF_PRIMITIVE_H */
