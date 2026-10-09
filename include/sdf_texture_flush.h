#ifndef SDF_TEXTURE_FLUSH_H
#define SDF_TEXTURE_FLUSH_H

#include "sdf_gs_packet.h"

void sdfInitializeTextureFlushRegister(SdfGsRegisterWrite *write);
void sdfInitializeTextureFlushPacket(SdfGsTextureFlushPacket *packet);

#endif
