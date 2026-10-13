#ifndef SDF_PACKET_HEADER_H
#define SDF_PACKET_HEADER_H

#include "sdf_texture_draw_packet.h"

SdfDrawPacket *sdfConsInitPacketHeader(SdfDrawPacket *packet, s32 primitiveFlags,
                                       s32 registerCount, s64 registerList,
                                       s32 loopCount);

#endif /* SDF_PACKET_HEADER_H */
