#include "sdk_ps2.h"
#include "sdf_gs_header.h"
#include "eeregs.h"
#include "common.h"
#include "sdf_vu0.h"
#include "sdf_vu_matrix.h"
#include "sdf_pad_entry.h"
#include "sdf_pad_index.h"
#include "sdf_pad_mode_api.h"
#include "sdf_pad_motor_api.h"
#include "sdf_cons_matrix_packet.h"
#include "sdf_dma_packet_header.h"
#include "kwln.h"
#include "sdf_asset_packets.h"
#include "sdf_asset_state.h"
#include "sdf_vu_lighting.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "sdf_packet_header.h"
#include "sdf_packet_append.h"
#include "sdf_resource.h"
#include "sdf_primitive_packet_api.h"
#include "sdf.h"
#include "sdf_draw.h"
#include "sdf_request.h"
#include "sdf_model.h"
#include "sdf_projection.h"
#include "pcp_vu0.h"
#include "sdf_vu_transform_api.h"
#include "ee_mmi.h"
#include "fpu.h"
#include "sdf_texture_file.h"
#include "sdf_column_packet_api.h"
#include "sdf_lookup_texture.h"
#include "sdf_compact_vertex_vif_packet.h"
#include "sdf_dev_console_node.h"
#include "btl.h"
#include "sdf_packet_alloc.h"

#define SDF_DMA_QWORD_SHIFT 4

#define SDF_QWORD_ALIGNMENT_MASK 0xF

#define SDF_VIF_ITOP_COMPACT_VERTEX 0x04000002

#define SDF_VIF_ITOP_WIDE_VERTEX 0x04000008

#define SDF_VIF_MSCAL_VERTICES 0x14000008

void sdfBillBoardMakeTMPacket(u8 *packet, const f32 *matrix) {
    *(u16 *)packet = 2;
    *(u32 *)(packet + 8) = 0x6403C000;
    *(f32 *)(packet + 12) = matrix[0];
    *(f32 *)(packet + 16) = matrix[1];
    *(f32 *)(packet + 20) = matrix[4];
    *(f32 *)(packet + 24) = matrix[5];
    *(f32 *)(packet + 28) = matrix[12];
    *(f32 *)(packet + 32) = matrix[13];
    *(u32 *)(packet + 36) = 0x04000000;
    *(u32 *)(packet + 40) = 0x14000008;
    memset(packet + 0x2C, 0, 12);
}

typedef struct SdfTexturedQuadVifPacket {
    u16 qwordCount;
    u8 pad02[6];
    u32 commandWords[8];
    f32 position[3];
    u32 commandWord34;
    u32 color;
    u32 commandWord3C;
    u8 uvPayload[0x10];
    u32 commandWord50;
    f32 quad[8];
    u32 trailer[3];
} SdfTexturedQuadVifPacket;

typedef char SdfTexturedQuadVifPacket_size_check[
    sizeof(SdfTexturedQuadVifPacket) == 0x80 ? 1 : -1];

typedef char SdfTexturedQuadVifPacket_position_offset_check[
    ((u32)&((SdfTexturedQuadVifPacket *)0)->position == 0x28) ? 1 : -1];

typedef char SdfTexturedQuadVifPacket_uv_offset_check[
    ((u32)&((SdfTexturedQuadVifPacket *)0)->uvPayload == 0x40) ? 1 : -1];

typedef char SdfTexturedQuadVifPacket_quad_offset_check[
    ((u32)&((SdfTexturedQuadVifPacket *)0)->quad == 0x54) ? 1 : -1];

/* Build a one-vertex textured-quad VIF packet: the position, color, the four
 * UV corners (copied as unaligned words) and the eight quad corner coordinates. */
struct BillTextureQuad;

void sdfBillBoardMake1(u8 *packet, const f32 *position, u32 color,
                                   const struct BillTextureQuad *uv,
                                   const f32 *quad) {
    SdfTexturedQuadVifPacket *view = (SdfTexturedQuadVifPacket *)packet;

    view->qwordCount = 7;
    view->commandWords[0] = 0x6C01C000;
    view->commandWords[1] = 1;
    view->commandWords[2] = 0xA0000000;
    view->commandWords[3] = 0x43434310;
    view->commandWords[4] = 0x43;
    view->commandWords[5] = 0x6001C001;
    view->commandWords[6] = 0x155;
    view->commandWords[7] = 0x6801C002;
    view->position[0] = position[0];
    view->position[1] = position[1];
    view->position[2] = position[2];
    view->commandWord34 = 0x6E01C003;
    view->color = color;
    view->commandWord3C = 0x6504C004;
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(view->uvPayload), (const u8 *)uv);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(view->uvPayload + 4), (const u8 *)uv + 4);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(view->uvPayload + 8), (const u8 *)uv + 8);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(view->uvPayload + 12), (const u8 *)uv + 12);
    view->commandWord50 = 0x6404C008;
    view->quad[0] = quad[0];
    view->quad[1] = quad[1];
    view->quad[2] = quad[2];
    view->quad[3] = quad[3];
    view->quad[4] = quad[4];
    view->quad[5] = quad[5];
    view->quad[6] = quad[6];
    view->quad[7] = quad[7];
    view->trailer[0] = 0x04000002;
    view->trailer[1] = 0x14000008;
    view->trailer[2] = 0;
}

s32 sdfBillBoardGetPacketSize(s32 count) {
    return count * 0x40 + 0x40;
}

/* Build XYZ and attribute VIF streams; inputs are copied into the allocated
 * packet, and vertexCount must be positive. */
/* vu0 routine: pack aligned positions into the VIF three-word stream. */
u32 *sdfBillBoardMakeN(const u128 *positions, const u32 *packedColors, const void *uvCorners, const f32 *cornerOffsets, s32 vertexCount, void *(*allocatePacket)(s32)) {
    s32 packetBytes;
    u32 *packet;
    u32 *cursor;
    u32 vuAddress;
    u32 unpackCode;
    s32 n;
    s32 vertexIndex;

    packetBytes = sdfBillBoardGetPacketSize(vertexCount);
    if (allocatePacket == NULL) {
        cursor = (u32 *)sdfGrAllocPacketBuff(packetBytes);
    } else {
        cursor = allocatePacket(packetBytes);
    }
    packet = cursor;
    packet[0] = (packetBytes >> SDF_DMA_QWORD_SHIFT) - 1;
    packet[1] = 0;
    packet[2] = 0x6C01C000;
    packet[3] = vertexCount;
    packet[4] = 0xA0000000;
    packet[5] = 0x43434310;
    packet[6] = 0x43;
    packet[7] = 0x6001C001;
    packet[8] = 0x155;
    packet[9] = (vertexCount << 16) | 0x6800C002;
    cursor = packet + 10;
    vertexIndex = 0;
    do {
        EE_MMI_STORE_VEC3_VALUE(cursor, positions[vertexIndex]);
        cursor += 3;
        vertexIndex++;
    } while (vertexIndex != vertexCount);
    n = vertexCount;
    unpackCode = n << 16;
    vuAddress = vertexCount + 2;
    *cursor++ = unpackCode | vuAddress | 0x6E00C000;
    memcpy(cursor, packedColors, n * 4);
    cursor += n;
    vuAddress += n;
    n = vertexCount * 4;
    unpackCode = n << 16;
    *cursor++ = unpackCode | vuAddress | 0x6500C000;
    vuAddress += n;
    unpackCode |= vuAddress;
    memcpy(cursor, uvCorners, n * 4);
    cursor += n;
    n *= 8;
    unpackCode |= 0x6400C000;
    *cursor++ = unpackCode;
    memcpy(cursor, cornerOffsets, n);
    cursor = (u32 *)((u8 *)cursor + n);
    cursor[0] = SDF_VIF_ITOP_COMPACT_VERTEX;
    cursor[1] = SDF_VIF_MSCAL_VERTICES;
    cursor += 2;
    while (((u32)cursor & SDF_QWORD_ALIGNMENT_MASK) != 0) {
        *cursor++ = 0;
    }
    return packet;
}

/* Build a one-vertex VIF packet for a two-texture quad: the position, both
 * colors, the interleaved primary/secondary UV corners (copied as unaligned
 * words) and the eight quad corner coordinates. */
void sdfMultiBillBoardMake1(u8 *packet, const f32 *position, u32 primaryColor, u32 secondaryColor,
                   const void *primaryUv, const void *secondaryUv, const f32 *quad) {
    *(u16 *)packet = 8;
    *(u32 *)(packet + 8) = 0x6C01C000;
    *(u32 *)(packet + 12) = 1;
    *(u32 *)(packet + 16) = 0xA0000000;
    *(u32 *)(packet + 20) = 0x43434310;
    *(u32 *)(packet + 24) = 0x43;
    *(u32 *)(packet + 28) = 0x6001C001;
    *(u32 *)(packet + 32) = 0x155;
    *(u32 *)(packet + 36) = 0x6801C002;
    *(f32 *)(packet + 40) = position[0];
    *(f32 *)(packet + 44) = position[1];
    *(f32 *)(packet + 48) = position[2];
    *(u32 *)(packet + 52) = 0x6E02C003;
    *(u32 *)(packet + 56) = primaryColor;
    *(u32 *)(packet + 60) = secondaryColor;
    *(u32 *)(packet + 64) = 0x6D04C005;
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 68), (const u8 *)primaryUv);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 72), (const u8 *)secondaryUv);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 76), (const u8 *)primaryUv + 4);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 80), (const u8 *)secondaryUv + 4);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 84), (const u8 *)primaryUv + 8);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 88), (const u8 *)secondaryUv + 8);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 92), (const u8 *)primaryUv + 12);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 96), (const u8 *)secondaryUv + 12);
    *(u32 *)(packet + 100) = 0x6404C009;
    *(f32 *)(packet + 104) = quad[0];
    *(f32 *)(packet + 108) = quad[1];
    *(f32 *)(packet + 112) = quad[2];
    *(f32 *)(packet + 116) = quad[3];
    *(f32 *)(packet + 120) = quad[4];
    *(f32 *)(packet + 124) = quad[5];
    *(f32 *)(packet + 128) = quad[6];
    *(f32 *)(packet + 132) = quad[7];
    *(u32 *)(packet + 136) = 0x04000004;
    *(u32 *)(packet + 140) = 0x14000008;
}

u32 sdfMultiBillBoardGetPacketSize(s32 count) {
    return (count * 0x54 + 0x4bU) & 0xfffffff0;
}

/* Build a quad packet from paired colors and per-corner UV sets; return its address. */
u32 *sdfMultiBillBoardMakeN(u128 *positions, void *attributes, void *halfAttributes, void *wordAttributes, s32 count, void *(*alloc)(s32)) {
    s32 bytes;
    u32 *packet;
    u32 *cursor;
    u32 index;
    u32 code;
    s32 n;
    s32 i;

    bytes = sdfMultiBillBoardGetPacketSize(count);
    if (alloc == NULL) {
        cursor = (u32 *)sdfGrAllocPacketBuff(bytes);
    } else {
        cursor = alloc(bytes);
    }
    packet = cursor;
    packet[0] = (bytes >> 4) - 1;
    packet[1] = 0;
    packet[2] = 0x6C01C000;
    packet[3] = count;
    packet[4] = 0xA0000000;
    packet[5] = 0x43434310;
    packet[6] = 0x43;
    packet[7] = 0x6001C001;
    packet[8] = 0x155;
    packet[9] = (count << 16) | 0x6800C002;
    cursor = packet + 10;
    i = 0;
    do {
        EE_MMI_STORE_VEC3_VALUE(cursor, positions[i]);
        cursor += 3;
        i++;
    } while (i != count);
    n = count * 2;
    index = count + 2;
    *cursor++ = (n << 16) | index | 0x6E00C000;
    memcpy(cursor, attributes, n * 4);
    cursor += n;
    index += n;
    n = count * 4;
    code = n << 16;
    *cursor++ = code | index | 0x6D00C000;
    memcpy(cursor, halfAttributes, n * 8);
    cursor += n * 2;
    index += n;
    code |= index;
    *cursor++ = code | 0x6400C000;
    memcpy(cursor, wordAttributes, n * 8);
    cursor += n * 2;
    cursor[0] = 0x04000004;
    cursor[1] = 0x14000008;
    cursor += 2;
    while (((u32)cursor & 0xF) != 0) {
        *cursor++ = 0;
    }
    return packet;
}

/* Build a one-vertex textured-quad VIF packet with four corner colors: the
 * position, colors, UV corners (copied as unaligned words) and the eight quad
 * corner coordinates. */
void sdfBillBoardVcMake1(u8 *packet, const f32 *position, const u32 *colors, const struct BillTextureQuad *uv, const f32 *quad) {
    *(u16 *)packet = 8;
    *(u32 *)(packet + 8) = 0x6C01C000;
    *(u32 *)(packet + 12) = 1;
    *(u32 *)(packet + 16) = 0xD0000000;
    *(u32 *)(packet + 20) = 0x34134130;
    *(u32 *)(packet + 24) = 0x41341;
    *(u32 *)(packet + 28) = 0x6001C001;
    *(u32 *)(packet + 32) = 0x155;
    *(u32 *)(packet + 36) = 0x6801C002;
    *(f32 *)(packet + 40) = position[0];
    *(f32 *)(packet + 44) = position[1];
    *(f32 *)(packet + 48) = position[2];
    *(u32 *)(packet + 52) = 0x6E04C003;
    *(u32 *)(packet + 56) = colors[0];
    *(u32 *)(packet + 60) = colors[1];
    *(u32 *)(packet + 64) = colors[2];
    *(u32 *)(packet + 68) = colors[3];
    *(u32 *)(packet + 72) = 0x6504C007;
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 76), (const u8 *)uv);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 80), (const u8 *)uv + 4);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 84), (const u8 *)uv + 8);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 88), (const u8 *)uv + 12);
    *(u32 *)(packet + 92) = 0x6404C00B;
    *(f32 *)(packet + 96) = quad[0];
    *(f32 *)(packet + 100) = quad[1];
    *(f32 *)(packet + 104) = quad[2];
    *(f32 *)(packet + 108) = quad[3];
    *(f32 *)(packet + 112) = quad[4];
    *(f32 *)(packet + 116) = quad[5];
    *(f32 *)(packet + 120) = quad[6];
    *(f32 *)(packet + 124) = quad[7];
    *(u32 *)(packet + 128) = 0x04000006;
    *(u32 *)(packet + 132) = 0x14000008;
    *(u32 *)(packet + 136) = 0;
    *(u32 *)(packet + 140) = 0;
}

u32 sdfBillBoardVcGetPacketSize(s32 count) {
    return (count * 0x4C + 0x4B) & ~0xF;
}

/* Build a quad packet with four corner colors, UV pairs and XY offset pairs. */
u32 *sdfBillBoardVcMakeN(const u128 *positions, const void *attributes,
                  const void *halfAttributes, const void *wordAttributes,
                  s32 count, void *(*alloc)(s32)) {
    s32 bytes;
    u32 *packet;
    u32 *cursor;
    u32 index;
    u32 code;
    s32 n;
    s32 i;

    bytes = sdfBillBoardGetPacketSize(count);
    if (alloc == NULL) {
        cursor = (u32 *)sdfGrAllocPacketBuff(bytes);
    } else {
        cursor = alloc(bytes);
    }
    packet = cursor;
    packet[0] = (bytes >> 4) - 1;
    packet[1] = 0;
    packet[2] = 0x6C01C000;
    packet[3] = count;
    packet[4] = 0xD0000000;
    packet[5] = 0x34134130;
    packet[6] = 0x41341;
    packet[7] = 0x6001C001;
    packet[8] = 0x155;
    packet[9] = (count << 16) | 0x6800C002;

    cursor = packet + 10;
    i = 0;
    do {
        EE_MMI_STORE_VEC3_VALUE(cursor, positions[i]);
        cursor += 3;
        i++;
    } while (i != count);

    code = count << 18;
    index = count + 2;
    n = count * 4;
    *cursor++ = code | index | 0x6E00C000;
    memcpy(cursor, attributes, n * 4);
    cursor += n;
    index += n;
    *cursor++ = code | index | 0x6500C000;
    memcpy(cursor, halfAttributes, n * 4);
    cursor += n;
    index += n;
    n = count * 8;
    *cursor++ = code | index | 0x6400C000;
    memcpy(cursor, wordAttributes, n * 4);
    cursor += n;
    cursor[0] = 0x04000006;
    cursor[1] = 0x14000008;
    cursor += 2;
    while (((u32)cursor & 0xF) != 0) {
        *cursor++ = 0;
    }
    return packet;
}

/* Build a one-vertex VIF packet for a two-texture quad with eight colors: the
 * position, colors, interleaved primary/secondary UV corners (copied as
 * unaligned words) and the eight quad corner coordinates. */
void sdfMultiBillBoardVcMake1(u8 *packet, const f32 *position, const void *colors, const void *primaryUv,
                   const void *secondaryUv, const void *quad) {
    *(u16 *)packet = 10;
    *(u32 *)(packet + 8) = 0x6C01C000;
    *(u32 *)(packet + 12) = 1;
    *(u32 *)(packet + 16) = 0xD0000000;
    *(u32 *)(packet + 20) = 0x34134130;
    *(u32 *)(packet + 24) = 0x41341;
    *(u32 *)(packet + 28) = 0x6001C001;
    *(u32 *)(packet + 32) = 0x15D;
    *(u32 *)(packet + 36) = 0x6801C002;
    *(f32 *)(packet + 40) = position[0];
    *(f32 *)(packet + 44) = position[1];
    *(f32 *)(packet + 48) = position[2];
    *(u32 *)(packet + 52) = 0x6E08C003;
    memcpy(packet + 56, colors, 32);
    *(u32 *)(packet + 88) = 0x6D04C00B;
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 92), (const u8 *)primaryUv);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 96), (const u8 *)secondaryUv);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 100), (const u8 *)primaryUv + 4);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 104), (const u8 *)secondaryUv + 4);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 108), (const u8 *)primaryUv + 8);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 112), (const u8 *)secondaryUv + 8);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 116), (const u8 *)primaryUv + 12);
    EE_LOAD_UNALIGNED_WORD(*(u32 *)(packet + 120), (const u8 *)secondaryUv + 12);
    *(u32 *)(packet + 124) = 0x6404C00F;
    memcpy(packet + 128, quad, 32);
    *(u32 *)(packet + 160) = 0x04000008;
    *(u32 *)(packet + 164) = 0x14000008;
    *(u32 *)(packet + 168) = 0;
    *(u32 *)(packet + 172) = 0;
}

u32 sdfMultiBillBoardVcGetPacketSize(s32 count) {
    return (count * 0x6c + 0x4bU) & 0xfffffff0;
}

/* Build the wider attribute VIF stream; return the packet address.
 * vertexCount must be positive; preserve the native copy lengths and n reuse. */
/* vu0 routine: pack aligned positions into the VIF three-word stream. */
u32 *sdfMultiBillBoardVcMakeN(u128 *positions, void *byteAttributes, void *halfAttributes, void *wordAttributes, s32 vertexCount, void *(*allocatePacket)(s32)) {
    s32 packetBytes;
    u32 *packet;
    u32 *cursor;
    u32 vuAddress;
    u32 unpackCode;
    s32 n;
    s32 vertexIndex;

    packetBytes = sdfMultiBillBoardVcGetPacketSize(vertexCount);
    if (allocatePacket == NULL) {
        cursor = (u32 *)sdfGrAllocPacketBuff(packetBytes);
    } else {
        cursor = allocatePacket(packetBytes);
    }
    packet = cursor;
    packet[0] = (packetBytes >> SDF_DMA_QWORD_SHIFT) - 1;
    packet[1] = 0;
    packet[2] = 0x6C01C000;
    packet[3] = vertexCount;
    packet[4] = 0xD0000000;
    packet[5] = 0x34134130;
    packet[6] = 0x41341;
    packet[7] = 0x6001C001;
    packet[8] = 0x15D;
    packet[9] = (vertexCount << 16) | 0x6800C002;
    cursor = packet + 10;
    vertexIndex = 0;
    do {
        EE_MMI_STORE_VEC3_VALUE(cursor, positions[vertexIndex]);
        cursor += 3;
        vertexIndex++;
    } while (vertexIndex != vertexCount);
    n = vertexCount * 8;
    vuAddress = vertexCount + 2;
    *cursor++ = (n << 16) | vuAddress | 0x6E00C000;
    memcpy(cursor, byteAttributes, n * 4);
    cursor += n;
    vuAddress += n;
    n = vertexCount * 4;
    unpackCode = n << 16;
    *cursor++ = unpackCode | vuAddress | 0x6D00C000;
    memcpy(cursor, halfAttributes, n * 8);
    cursor += n * 2;
    vuAddress += n;
    unpackCode |= vuAddress;
    *cursor++ = unpackCode | 0x6400C000;
    memcpy(cursor, wordAttributes, n * 8);
    cursor += n * 2;
    cursor[0] = SDF_VIF_ITOP_WIDE_VERTEX;
    cursor[1] = SDF_VIF_MSCAL_VERTICES;
    cursor += 2;
    while (((u32)cursor & SDF_QWORD_ALIGNMENT_MASK) != 0) {
        *cursor++ = 0;
    }
    return packet;
}
