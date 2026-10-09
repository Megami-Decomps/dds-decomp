#ifndef SDF_VU_LIGHTING_H
#define SDF_VU_LIGHTING_H

#include "common.h"

/* One frame of transformed matrix rows, normalized lighting rows and VIF codes. */
typedef struct VuLightingPacket {
    u128 matrix[4];
    u128 scaledRows[3];
    u32 tag[4];
} VuLightingPacket;

typedef char VuLightingPacket_size_must_be_0x80[
    (sizeof(VuLightingPacket) == 0x80) ? 1 : -1];
typedef char VuLightingPacket_scaledRows_at_40[
    ((u32)&((VuLightingPacket *)0)->scaledRows == 0x40) ? 1 : -1];
typedef char VuLightingPacket_tag_at_70[
    ((u32)&((VuLightingPacket *)0)->tag == 0x70) ? 1 : -1];

/* Store the resident VU matrix and scaled rows into the supplied frame packet. */
void sdfWriteVuLightingPacket(VuLightingPacket *lightingPacket);

#endif /* SDF_VU_LIGHTING_H */
