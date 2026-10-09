#ifndef SDF_PACKET_APPEND_H
#define SDF_PACKET_APPEND_H

#include "sdf.h"

/* Append standard packets to a DMA packet list. A null allocator selects the
   default packet allocator. Asset packets reference the selected draw entry. */
void sdfConsAppendClearPacket(SdfListHead *packetList,
                              s32 (*allocatePacket)(s32));
void sdfConsAppendVuPacket(SdfListHead *packetList,
                           s32 (*allocatePacket)(s32));
void sdfConsAppendAssetPacket(SdfListHead *packetList, SdfAsset *asset,
                              s32 (*allocatePacket)(s32));

#endif /* SDF_PACKET_APPEND_H */
