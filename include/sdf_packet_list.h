#ifndef SDF_PACKET_LIST_H
#define SDF_PACKET_LIST_H

#include "common.h"

struct SdfListHead;

struct SdfListHead *sdfAllocatePacketList(s32 (*allocator)(s32));

void sdfAppendPacket(struct SdfListHead *list, u32 packetAddress);
void sdfAppendPacketRange(struct SdfListHead *list, u32 packetAddress,
                          u32 rangeTailAddress);
void sdfAppendReferencePacket(struct SdfListHead *list, u32 packetAddress);
void sdfAppendCallPacket(struct SdfListHead *list, u32 packetAddress);

#endif /* SDF_PACKET_LIST_H */
