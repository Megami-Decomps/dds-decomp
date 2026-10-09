#ifndef SDF_GS_HEADER_H
#define SDF_GS_HEADER_H

#include "sdf_gs_packet.h"

/* DMA/VIF transfer header and packed A+D GIF tag, excluding register payload. */
void sdfInitializeDmaReferenceTag(SdfGsPacketHeader *header, s32 payloadQwords);

/* GIF tag followed by a REF transfer and the patchable NEXT link. */
typedef struct SdfDmaReferenceChainPacket {
    SdfGsPacketHeader header;
    u64 referenceTag;
    u64 referenceVifCommands;
    u64 nextTag;
    u64 zeroTailWord;
} SdfDmaReferenceChainPacket;

typedef char SdfDmaReferenceChainPacket_size_must_be_0x40[
    (sizeof(SdfDmaReferenceChainPacket) == 0x40) ? 1 : -1];
typedef char SdfDmaReferenceChainPacket_reference_at_0x20[
    ((u32)&((SdfDmaReferenceChainPacket *)0)->referenceTag == 0x20) ? 1 : -1];
typedef char SdfDmaReferenceChainPacket_next_at_0x30[
    ((u32)&((SdfDmaReferenceChainPacket *)0)->nextTag == 0x30) ? 1 : -1];

void sdfBuildDmaReferenceChain(SdfDmaReferenceChainPacket *packet,
    u32 sourceAddress, s32 qwordCount);

#endif
