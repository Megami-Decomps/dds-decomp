#ifndef SDF_GS_BLEND_H
#define SDF_GS_BLEND_H

#include "sdf_gs_packet.h"

/* Register writers take the payload; DMA builders take its complete packet. */
void sdfPrimSetAddTransElem(SdfGsBlendRegisters *packet);
void sdfPrimSetOpaqueElem(SdfGsBlendRegisters *packet);
void sdfPrimSetSubTransElem(SdfGsBlendRegisters *packet);
void sdfPrimSetAddTrans2Elem(SdfGsBlendRegisters *packet);
void sdfPrimSetOpaque2Elem(SdfGsBlendRegisters *packet);
void sdfPrimSetSubTrans2Elem(SdfGsBlendRegisters *packet);
void sdfPrimSetSemiTransElem(SdfGsBlendRegisters *packet);
void sdfPrimSetSemiTrans2Elem(SdfGsBlendRegisters *packet);

void sdfPrimSetAddTransPacket(SdfGsBlendPacket *packet);
void sdfPrimSetOpaquePacket(SdfGsBlendPacket *packet);
void sdfPrimSetSubTransPacket(SdfGsBlendPacket *packet);
void sdfPrimSetSemiTransPacket(SdfGsBlendPacket *packet);
void sdfPrimSetAddTrans2Packet(SdfGsBlendPacket *packet);
void sdfPrimSetOpaque2Packet(SdfGsBlendPacket *packet);
void sdfPrimSetSubTrans2Packet(SdfGsBlendPacket *packet);
void sdfPrimSetSemiTrans2Packet(SdfGsBlendPacket *packet);

#endif
