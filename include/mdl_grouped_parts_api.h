#ifndef MDL_GROUPED_PARTS_API_H
#define MDL_GROUPED_PARTS_API_H

#include "sdf_pac_work.h"

/* Apply a grouped packet sequence and return the packet after that sequence. */
PacWork *mdlApplyGroupedPartPackets(PacWork *packetWork, s32 group, s32 id, s32 mode);

#endif /* MDL_GROUPED_PARTS_API_H */
