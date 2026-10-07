#ifndef SDF_LINKED_PACKET_H
#define SDF_LINKED_PACKET_H

#include "common.h"

/* Complete linked-packet list, distinct from the 0x20-byte DMA list head. */
typedef struct SdfLinkedPacketList {
    u32 unk0;
    u32 first; /* Address word for the first linked packet node. */
    u32 last;  /* Address word for the last linked packet node. */
    u32 unkC;
} SdfLinkedPacketList;

/* Frame submission joins these 0x10-byte linked owners through head/tail. */
typedef struct SdfPacketChain {
    SdfLinkedPacketList *head;
    SdfLinkedPacketList *tail;
} SdfPacketChain;

typedef char SdfLinkedPacketList_size_must_be_0x10[
    sizeof(SdfLinkedPacketList) == 0x10 ? 1 : -1];
typedef char SdfPacketChain_size_must_be_8[sizeof(SdfPacketChain) == 8 ? 1 : -1];

#endif
