#ifndef SDF_PACKET_LIST_H
#define SDF_PACKET_LIST_H

#include "common.h"

struct SdfListHead;

struct SdfListHead *sdfAllocatePacketList(s32 (*allocator)(s32));

#endif /* SDF_PACKET_LIST_H */
