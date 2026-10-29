#ifndef SDF_CORE_PACKET_POINT_BUFF_TAG_H
#define SDF_CORE_PACKET_POINT_BUFF_TAG_H

#include "common.h"

typedef struct sdfCorePacketPointBuff_tag {
    f32 position[4];            /* 0x00: position vector consumed by VU clipping */
    u8 opaque10[0x10];          /* 0x10: only the W lane is used by clipping */
    f32 color[4];               /* 0x20: copied and interpolated as a vector */
    f32 xy[2];                  /* 0x30: interpolated XY pair */
    u8 opaque38[8];             /* 0x38: other lanes are unresolved */
    struct sdfCorePacketPointBuff_tag *next;   /* 0x40 */
    struct sdfCorePacketPointBuff_tag *sourceA; /* 0x44 */
    struct sdfCorePacketPointBuff_tag *sourceB; /* 0x48 */
    f32 weight;                 /* 0x4C */
    u8 region;                  /* 0x50: bit 0 selects the W = 0 clipping path */
    u8 clipFlags;               /* 0x51: clip-plane bits used for triangle culling */
    u8 opaque52[0x0E];          /* 0x52 */
} sdfCorePacketPointBuff_tag;

typedef char sdfCorePacketPointBuff_tag_size_must_be_0x60[
    (sizeof(sdfCorePacketPointBuff_tag) == 0x60) ? 1 : -1];
typedef char sdfCorePacketPointBuff_tag_color_offset_must_be_0x20[
    ((unsigned long)&((sdfCorePacketPointBuff_tag *)0)->color == 0x20) ? 1 : -1];
typedef char sdfCorePacketPointBuff_tag_xy_offset_must_be_0x30[
    ((unsigned long)&((sdfCorePacketPointBuff_tag *)0)->xy == 0x30) ? 1 : -1];
typedef char sdfCorePacketPointBuff_tag_next_offset_must_be_0x40[
    ((unsigned long)&((sdfCorePacketPointBuff_tag *)0)->next == 0x40) ? 1 : -1];
typedef char sdfCorePacketPointBuff_tag_weight_offset_must_be_0x4C[
    ((unsigned long)&((sdfCorePacketPointBuff_tag *)0)->weight == 0x4C) ? 1 : -1];
typedef char sdfCorePacketPointBuff_tag_region_offset_must_be_0x50[
    ((unsigned long)&((sdfCorePacketPointBuff_tag *)0)->region == 0x50) ? 1 : -1];
typedef char sdfCorePacketPointBuff_tag_clipFlags_offset_must_be_0x51[
    ((unsigned long)&((sdfCorePacketPointBuff_tag *)0)->clipFlags == 0x51) ? 1 : -1];

#endif /* SDF_CORE_PACKET_POINT_BUFF_TAG_H */
