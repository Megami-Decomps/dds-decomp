#ifndef MDL_PART_INFO_API_H
#define MDL_PART_INFO_API_H

#include "mdl.h"
#include "sdf_pac_work.h"

typedef s32 MdlPartInfoPacketResult;

#define MDL_PART_INFO_PACKET_NO_MATCH ((MdlPartInfoPacketResult)0)
#define MDL_PART_INFO_PACKET_MATCH ((MdlPartInfoPacketResult)1)

/* Read the number of attached resource parts from a borrowed PIB record. */
u16 mdlPibGetPartCount(const MdlRecord *record);

/* Test a borrowed PAC packet header for the grouped part-info command/tag. */
MdlPartInfoPacketResult mdlIsPartInfoPacket(const PacWork *packetWork);

#endif /* MDL_PART_INFO_API_H */
