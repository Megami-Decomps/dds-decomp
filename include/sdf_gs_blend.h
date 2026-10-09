#ifndef SDF_GS_BLEND_H
#define SDF_GS_BLEND_H

#include "sdf_gs_packet.h"

/* Register writers take the payload; DMA builders take its complete packet. */
void sdfInitPrimaryAlphaAdditiveRegisters(SdfGsBlendRegisters *packet);
void sdfInitPrimaryAlphaBlendRegisters(SdfGsBlendRegisters *packet);
void sdfInitPrimaryAlphaSubtractiveRegisters(SdfGsBlendRegisters *packet);
void sdfInitSecondaryAlphaAdditiveRegisters(SdfGsBlendRegisters *packet);
void sdfInitSecondaryAlphaBlendRegisters(SdfGsBlendRegisters *packet);
void sdfInitSecondaryAlphaSubtractiveRegisters(SdfGsBlendRegisters *packet);
void sdfSetPrimaryTestBlendRegisters(SdfGsBlendRegisters *packet);
void sdfSetSecondaryTestBlendRegisters(SdfGsBlendRegisters *packet);

void sdfBuildPrimaryAlphaAdditiveDmaPacket(SdfGsBlendPacket *packet);
void sdfBuildPrimaryAlphaBlendDmaPacket(SdfGsBlendPacket *packet);
void sdfBuildPrimaryAlphaSubtractiveDmaPacket(SdfGsBlendPacket *packet);
void sdfBuildPrimaryTestBlendPacket(SdfGsBlendPacket *packet);
void sdfBuildSecondaryAlphaAdditiveDmaPacket(SdfGsBlendPacket *packet);
void sdfBuildSecondaryAlphaBlendDmaPacket(SdfGsBlendPacket *packet);
void sdfBuildSecondaryAlphaSubtractiveDmaPacket(SdfGsBlendPacket *packet);
void sdfBuildSecondaryTestBlendPacket(SdfGsBlendPacket *packet);

#endif
