#ifndef DDS_SDF_TEX_PACKET_H
#define DDS_SDF_TEX_PACKET_H

struct SdfTex;

/* Store the variant-zero resource packet on the texture's primary buffer. */
void sdfTexCreateFirstPacket(struct SdfTex *texture);

#endif
