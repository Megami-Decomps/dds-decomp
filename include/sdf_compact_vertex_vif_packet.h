#ifndef SDF_COMPACT_VERTEX_VIF_PACKET_H
#define SDF_COMPACT_VERTEX_VIF_PACKET_H

#include "common.h"

u32 *sdfBuildCompactVertexVifPacket(const u128 *positions, const void *byteAttributes,
    const void *halfAttributes, const void *wordAttributes, s32 vertexCount,
    void *(*allocatePacket)(s32));

#endif
