#ifndef SDF_TEXTURE_DRAW_PACKET_H
#define SDF_TEXTURE_DRAW_PACKET_H

#include "sdf.h"

struct SdfDrawPacket;

u32 sdfConsGetTextureDrawPacketSize(SdfTex *texture);
struct SdfDrawPacket *sdfConsInitTextureDrawPacket(struct SdfDrawPacket *drawPacket, SdfTex *texture,
                                                   s32 contextOffset);
s32 sdfConsCreateDrawPacket(SdfListHead *packetList, SdfTex *texture, s32 contextOffset);

#endif
