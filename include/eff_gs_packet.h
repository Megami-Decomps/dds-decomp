#ifndef EFF_GS_PACKET_H
#define EFF_GS_PACKET_H

#include "common.h"

/* An effect's GS register packet: DMA, VIF and GIF tags, then one register write. */
typedef struct EffGsPacket {
    u64 dmaTag;
    u64 vifTag;
    u64 gifTag;
    u64 registerList;
    u64 registerValue;
    u64 registerAddress;
} EffGsPacket;

#endif
