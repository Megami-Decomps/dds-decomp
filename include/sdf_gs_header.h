#ifndef SDF_GS_HEADER_H
#define SDF_GS_HEADER_H

#include "sdf_gs_packet.h"

/* DMA/VIF transfer header and packed A+D GIF tag, excluding register payload. */
void sdfInitializeDmaReferenceTag(SdfGsPacketHeader *header, s32 payloadQwords);

#endif
