#ifndef SDF_TEXTURE_FLUSH_H
#define SDF_TEXTURE_FLUSH_H

#include "sdf_gs_packet.h"

void sdfPrimSetTexflushElem(SdfGsRegisterWrite *write);
void sdfPrimSetTexflushPacket(SdfGsTextureFlushPacket *packet);

#endif
