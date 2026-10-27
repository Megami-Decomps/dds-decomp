#ifndef SDF_COMPACT_VERTEX_VIF_PACKET_H
#define SDF_COMPACT_VERTEX_VIF_PACKET_H

#include "common.h"

/* Inputs are borrowed for this call and copied into the allocated VIF packet.
 * Each vertex contributes XYZ, one packed color word, eight raw UV halfwords,
 * and eight float corner offsets. A null allocator selects the default packet
 * allocator. */
u32 *sdfBillBoardMakeN(const u128 *positions, const u32 *packedColors,
    const void *uvCorners, const f32 *cornerOffsets, s32 vertexCount,
    void *(*allocatePacket)(s32));

#endif
