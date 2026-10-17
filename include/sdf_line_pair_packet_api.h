#ifndef SDF_LINE_PAIR_PACKET_API_H
#define SDF_LINE_PAIR_PACKET_API_H

#include "common.h"

struct SdfPacket;

/* Return the packet header that precedes the emitted line-pair payload. */
struct SdfPacket *sdfDrawClippedLinePairs(const f32 vertices[][4], const u32 *colors,
                                          s32 vertexCount, u32 primitiveFlags);
struct SdfPacket *sdfDrawCachedMatrixLinePairsVU(const f32 vertices[][4], const u32 *colors,
                                                 s32 vertexCount, u32 primitiveFlags);
struct SdfPacket *sdfDrawPostmultipliedLinePairsVU(const f32 vertices[][4], const u32 *colors,
                                                   s32 vertexCount, u32 primitiveFlags);

#endif /* SDF_LINE_PAIR_PACKET_API_H */
