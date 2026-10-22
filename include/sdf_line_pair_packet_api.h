#ifndef SDF_LINE_PAIR_PACKET_API_H
#define SDF_LINE_PAIR_PACKET_API_H

#include "common.h"

struct SdfPacket;

/* Return the packet header that precedes the emitted line-pair payload. */
struct SdfPacket *sdfDrawModelWithMatrix(const f32 vertices[][4], const u32 *colors,
                                          s32 vertexCount, u32 primitiveFlags);
struct SdfPacket *effDebugDrawWireModel(const f32 vertices[][4], const u32 *colors,
                                                 s32 vertexCount, u32 primitiveFlags);
struct SdfPacket *sdfDrawRotatedModel(const f32 vertices[][4], const u32 *colors,
                                                   s32 vertexCount, u32 primitiveFlags);

#endif /* SDF_LINE_PAIR_PACKET_API_H */
