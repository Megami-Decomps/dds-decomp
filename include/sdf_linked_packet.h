#ifndef SDF_LINKED_PACKET_H
#define SDF_LINKED_PACKET_H

#include "common.h"
#include "sdf_packet_patch.h"

struct sdfDrawTag_t;

/* Complete linked-packet list, distinct from the 0x20-byte DMA list head. */
typedef struct sdfPreDrawTag_t {
    u32 unk0;
    SdfPacketPatchLink *first;
    SdfPacketPatchLink *last;
    u32 unkC;
} sdfPreDrawTag_t;

/* Frame submission joins these 0x10-byte linked owners through head/tail. */
typedef struct SdfPacketChain {
    sdfPreDrawTag_t *head;
    sdfPreDrawTag_t *tail;
} SdfPacketChain;

typedef char SdfLinkedPacketList_size_must_be_0x10[
    sizeof(sdfPreDrawTag_t) == 0x10 ? 1 : -1];
typedef char SdfLinkedPacketList_first_at_4[
    ((u32)&((sdfPreDrawTag_t *)0)->first == 4) ? 1 : -1];
typedef char SdfLinkedPacketList_last_at_8[
    ((u32)&((sdfPreDrawTag_t *)0)->last == 8) ? 1 : -1];
typedef char SdfPacketChain_size_must_be_8[sizeof(SdfPacketChain) == 8 ? 1 : -1];

void sdfPreDrawTagInit(sdfPreDrawTag_t *list);
void sdfPreDrawTagLink(sdfPreDrawTag_t *list, SdfPacketPatchLink *node);
void sdfPreDrawChainInit(SdfPacketChain *chain);
void sdfPreDrawChainLink(SdfPacketChain *chain, sdfPreDrawTag_t *node);
void sdfGrLinkStdDrawEnv(struct sdfDrawTag_t *dmaList,
                                  sdfPreDrawTag_t *linkedList,
                                  SdfPacketPatchLink *linkedNode);

#endif
