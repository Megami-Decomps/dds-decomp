#ifndef EVT_FRAME_STATE_PACKET_H
#define EVT_FRAME_STATE_PACKET_H

#include "sdf_dma_tag.h"
#include "sdf_gs_packet.h"

/* One frame state's packet: a DMA end tag carrying VIF DIRECT, then a TEST
 * write, a full-screen sprite of two vertices, and the TEST and ALPHA writes
 * that close it (evtInitializeFrameStatePacketTemplates fills them). */
typedef struct EvtFrameStatePacket {
    SdfDmaTagHeader dma;
    u64 testGifTag;
    u64 testGifRegisters;
    SdfGsRegisterWrite clearTest;
    u64 clearGifTag;
    u64 clearGifRegisters;
    SdfGsRegisterWrite topLeftColor;
    SdfGsRegisterWrite topLeftPosition;
    SdfGsRegisterWrite bottomRightColor;
    SdfGsRegisterWrite bottomRightPosition;
    u64 blendGifTag;
    u64 blendGifRegisters;
    SdfGsBlendRegisters blend;
} EvtFrameStatePacket;

typedef char EvtFrameStatePacketSizeCheck[sizeof(EvtFrameStatePacket) == 0xB0 ? 1 : -1];

/* The three frame states' packets, one per draw buffer. */
extern EvtFrameStatePacket evtFrameStatePacketTemplates[3][2];

#endif /* EVT_FRAME_STATE_PACKET_H */
