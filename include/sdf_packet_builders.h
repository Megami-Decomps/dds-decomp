#ifndef SDF_PACKET_BUILDERS_H
#define SDF_PACKET_BUILDERS_H

#include "sdf.h"
#include "sdf_linked_packet.h"

void sdfCreateResourcePacket(
    SdfListHead *list, SdfTexResource *source, s32 sourceX, s32 sourceY,
    s32 width, s32 height, s32 arg6, s32 arg7, s32 arg8,
    s32 (*allocatePacket)(s32));

void sdfCreateDescriptorPacket(
    SdfListHead *list, SdfTexResource *source, s32 destinationX,
    s32 destinationY, s32 width, s32 height, s32 sourceAddress,
    s32 (*allocatePacket)(s32));

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

void sdfAppendTexturedLinePacket(
    SdfListHead *list, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0,
    s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 depth,
    s32 (*allocatePacket)(s32));
void sdfAppendFillRectanglePacket(
    SdfListHead *list, s32 color, s32 primitive, s32 left, s32 top, s32 right,
    s32 bottom, s32 depth, s32 (*allocatePacket)(s32));
void sdfAppendClosedRectanglePacket(
    SdfListHead *list, s32 color, s32 primitive, s32 left, s32 top, s32 right,
    s32 bottom, s32 depth, s32 (*allocatePacket)(s32));

#endif
