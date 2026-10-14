#ifndef SDF_DEV_CONS_STACK_PACKETS_H
#define SDF_DEV_CONS_STACK_PACKETS_H

#include "common.h"

/* Stack allocation whose central 0x1000-byte region backs the console DMA ring.
 * The surrounding bytes are retained as opaque allocation space. */
typedef struct DevConsStackPackets {
    u8 opaquePrefix[0x30];
    u8 data[0x1000];
    u8 opaqueSuffix[0x10];
} DevConsStackPackets;

typedef char DevConsStackPacketsSizeCheck[
    (sizeof(DevConsStackPackets) == 0x1040) ? 1 : -1];
typedef char DevConsStackPacketsDataOffsetCheck[
    ((u32)&((DevConsStackPackets *)0)->data == 0x30) ? 1 : -1];
typedef char DevConsStackPacketsSuffixOffsetCheck[
    ((u32)&((DevConsStackPackets *)0)->opaqueSuffix == 0x1030) ? 1 : -1];

#endif /* SDF_DEV_CONS_STACK_PACKETS_H */
