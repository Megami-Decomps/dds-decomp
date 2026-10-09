#ifndef SDF_PACKET_LIST_H
#define SDF_PACKET_LIST_H

#include "common.h"

struct SdfListHead;
struct SdfDmaNode;

struct SdfListHead *sdfAllocatePacketList(s32 (*allocator)(s32));
struct SdfListHead *sdfCreateResetPacketList(void);
void sdfInitPacketList(struct SdfListHead *list);

void sdfAppendPacket(struct SdfListHead *list, u32 packetAddress);
void sdfAppendPacketRange(struct SdfListHead *list, u32 packetAddress,
                          u32 rangeTailAddress);
void sdfAppendReferencePacket(struct SdfListHead *list, u32 packetAddress);
void sdfAppendCallPacket(struct SdfListHead *list, u32 packetAddress);
void sdfAppendDmaTagToList(struct SdfListHead *list, u32 packetAddress);
void sdfAppendDmaPrimary(struct SdfListHead *list, u32 source,
                         struct SdfDmaNode *node);

#endif /* SDF_PACKET_LIST_H */
