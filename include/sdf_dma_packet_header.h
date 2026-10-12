#ifndef SDF_DMA_PACKET_HEADER_H
#define SDF_DMA_PACKET_HEADER_H

#include "common.h"

/* Shared 0x20-byte DMA/VIF reference header. Unrecovered bytes stay opaque. */
typedef struct DmaPacketHeader {
    u16 quadwords; /* 0x00 */
    u8 pad02[2]; /* 0x02 */
    u32 address; /* 0x04 */
    u32 tag; /* 0x08 */
    u32 command; /* 0x0C */
    u16 unknown10; /* 0x10 */
    u8 opaque12[6]; /* 0x12 */
    u32 unknown18; /* 0x18 */
    u32 unknown1C; /* 0x1C */
} DmaPacketHeader;

typedef char DmaPacketHeader_size_must_be_0x20[
    (sizeof(DmaPacketHeader) == 0x20) ? 1 : -1];
typedef char DmaPacketHeader_quadwords_offset_must_be_0[
    ((u32)&((DmaPacketHeader *)0)->quadwords == 0x00) ? 1 : -1];
typedef char DmaPacketHeader_address_offset_must_be_0x04[
    ((u32)&((DmaPacketHeader *)0)->address == 0x04) ? 1 : -1];
typedef char DmaPacketHeader_tag_offset_must_be_0x08[
    ((u32)&((DmaPacketHeader *)0)->tag == 0x08) ? 1 : -1];
typedef char DmaPacketHeader_command_offset_must_be_0x0C[
    ((u32)&((DmaPacketHeader *)0)->command == 0x0C) ? 1 : -1];
typedef char DmaPacketHeader_unknown10_offset_must_be_0x10[
    ((u32)&((DmaPacketHeader *)0)->unknown10 == 0x10) ? 1 : -1];
typedef char DmaPacketHeader_opaque12_offset_must_be_0x12[
    ((u32)&((DmaPacketHeader *)0)->opaque12 == 0x12) ? 1 : -1];
typedef char DmaPacketHeader_unknown18_offset_must_be_0x18[
    ((u32)&((DmaPacketHeader *)0)->unknown18 == 0x18) ? 1 : -1];
typedef char DmaPacketHeader_unknown1C_offset_must_be_0x1C[
    ((u32)&((DmaPacketHeader *)0)->unknown1C == 0x1C) ? 1 : -1];

#endif /* SDF_DMA_PACKET_HEADER_H */
