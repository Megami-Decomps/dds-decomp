#ifndef SDF_PACKET_BUILDERS_H
#define SDF_PACKET_BUILDERS_H

#include "sdf.h"
#include "sdf_linked_packet.h"

/* resourceAddress is copied as a packet metadata word, not dereferenced. */
void sdfCreatePatchableResourcePacket(
    SdfListHead *list, SdfLinkedPacketList *linkedList, s32 arg2, s32 arg3,
    s32 arg4, s32 arg5, s32 resourceAddress, s32 arg7, s32 arg8,
    s32 (*allocatePacket)(s32));

/* destination is the texture's native VRAM resource descriptor. */
void sdfCreateGraphBufferCopyPacket(
    SdfListHead *drawList, SdfLinkedPacketList *linkedList,
    SdfTexResource *destination, s32 destinationX, s32 destinationY,
    s32 sourceX, s32 sourceY, s32 transferWidth, s32 transferHeight,
    s32 resourceIndexXor, s32 (*allocatePacket)(s32));

#endif
