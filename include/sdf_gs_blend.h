#ifndef SDF_GS_BLEND_H
#define SDF_GS_BLEND_H

#include "sdf_gs_packet.h"

/* Register writers take the payload; DMA builders take its complete packet. */
void sdfPrimSetAddTransElem(sdfPrimTransElem_t *packet);
void sdfPrimSetOpaqueElem(sdfPrimTransElem_t *packet);
void sdfPrimSetSubTransElem(sdfPrimTransElem_t *packet);
void sdfPrimSetAddTrans2Elem(sdfPrimTransElem_t *packet);
void sdfPrimSetOpaque2Elem(sdfPrimTransElem_t *packet);
void sdfPrimSetSubTrans2Elem(sdfPrimTransElem_t *packet);
void sdfPrimSetSemiTransElem(sdfPrimTransElem_t *packet);
void sdfPrimSetSemiTrans2Elem(sdfPrimTransElem_t *packet);

void sdfPrimSetAddTransPacket(sdfPrimTransPacket_t *packet);
void sdfPrimSetOpaquePacket(sdfPrimTransPacket_t *packet);
void sdfPrimSetSubTransPacket(sdfPrimTransPacket_t *packet);
void sdfPrimSetSemiTransPacket(sdfPrimTransPacket_t *packet);
void sdfPrimSetAddTrans2Packet(sdfPrimTransPacket_t *packet);
void sdfPrimSetOpaque2Packet(sdfPrimTransPacket_t *packet);
void sdfPrimSetSubTrans2Packet(sdfPrimTransPacket_t *packet);
void sdfPrimSetSemiTrans2Packet(sdfPrimTransPacket_t *packet);

#endif
