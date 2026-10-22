#ifndef SDF_PACKET_APPEND_H
#define SDF_PACKET_APPEND_H

#include "sdf.h"

/* Append standard packets to a DMA packet list. A null allocator selects the
   default packet allocator. Asset packets reference the selected draw entry. */
void sdf3DPrimLinkResetTm(SdfListHead *packetList,
                              s32 (*allocatePacket)(s32));
void sdf3DPrimLinkTm(SdfListHead *packetList,
                           s32 (*allocatePacket)(s32));
void sdf3DPrimLinkMat(SdfListHead *packetList, SdfAsset *asset,
                              s32 (*allocatePacket)(s32));

#endif /* SDF_PACKET_APPEND_H */
