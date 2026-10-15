#ifndef SDF_PACKET_ALLOC_H
#define SDF_PACKET_ALLOC_H

#include "common.h"

/* Reserve size bytes from the active packet buffer, rounded up to 16 bytes.
 * Returns the prior cursor as a 32-bit address; the buffer owns this storage. */
s32 sdfAllocPacketAligned(s32 size);

/* Read the active packet cursor or restore it at a quadword boundary. */
s32 sdfGetPacketCursor(void);
void sdfSetPacketCursorAligned(s32 cursorAddress);

#endif /* SDF_PACKET_ALLOC_H */
