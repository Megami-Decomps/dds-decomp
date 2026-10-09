#ifndef SDF_LINKED_PACKET_H
#define SDF_LINKED_PACKET_H

#include "common.h"
#include "sdf_packet_patch.h"

struct SdfListHead;

/* Complete linked-packet list, distinct from the 0x20-byte DMA list head. */
typedef struct SdfLinkedPacketList {
    u32 unk0;
    SdfPacketPatchLink *first;
    SdfPacketPatchLink *last;
    u32 unkC;
} SdfLinkedPacketList;

/* Frame submission joins these 0x10-byte linked owners through head/tail. */
typedef struct SdfPacketChain {
    SdfLinkedPacketList *head;
    SdfLinkedPacketList *tail;
} SdfPacketChain;

typedef char SdfLinkedPacketList_size_must_be_0x10[
    sizeof(SdfLinkedPacketList) == 0x10 ? 1 : -1];
typedef char SdfLinkedPacketList_first_at_4[
    ((u32)&((SdfLinkedPacketList *)0)->first == 4) ? 1 : -1];
typedef char SdfLinkedPacketList_last_at_8[
    ((u32)&((SdfLinkedPacketList *)0)->last == 8) ? 1 : -1];
typedef char SdfPacketChain_size_must_be_8[sizeof(SdfPacketChain) == 8 ? 1 : -1];

void sdfClearLinkedPacketList(SdfLinkedPacketList *list);
void sdfAppendLinkedPacketNode(SdfLinkedPacketList *list, u32 *node);
void sdfClearPacketChain(SdfPacketChain *chain);
void sdfAppendPacketChainNode(SdfPacketChain *chain, SdfLinkedPacketList *node);
void sdfAppendLinkedPacketPayload(struct SdfListHead *dmaList,
                                  SdfLinkedPacketList *linkedList,
                                  u32 *linkedNode);

#endif
