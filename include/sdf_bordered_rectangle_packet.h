#ifndef SDF_BORDERED_RECTANGLE_PACKET_H
#define SDF_BORDERED_RECTANGLE_PACKET_H

#include "common.h"

void *sdfCreateBorderedRectanglePacket(s32 x, s32 y, s32 depth, s32 width, s32 height, u32 colorA, u32 colorB);
void *mdlBuildViewerRectanglePacket(s32 x, s32 y, s32 depth, s32 width, s32 height);

#endif
