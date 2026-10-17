#ifndef SDF_COLUMN_PACKET_API_H
#define SDF_COLUMN_PACKET_API_H

#include "sdf_texture_draw_packet.h"

/* Allocate a variable-length sprite column packet with this draw header. */
SdfDrawPacket *sdfConsAllocateColumnPacket(s32 loopCount);

#endif /* SDF_COLUMN_PACKET_API_H */
