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

/* Two GS register writes carried by one complete four-qword packet. */
typedef struct EffGsPairPacket {
    u64 dmaTag;
    u64 vifTag;
    u64 gifTag;
    u64 registerList;
    u64 firstValue;
    u64 firstAddress;
    u64 secondValue;
    u64 secondAddress;
} EffGsPairPacket;

#endif
