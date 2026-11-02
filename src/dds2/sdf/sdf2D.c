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
#include "sdf_heap_api.h"
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

#define SDF_TEXTURE_DRAW_PACKET_BYTES 0x50

#define SDF_GIF_REGISTER_AD 0xE

#define SDF_GIF_NREG_SHIFT 60

#define SDF_GIF_PRIM_SHIFT 47

#define SDF_GIF_PRE_EOP_FLAGS 0x400000008000LL

#define SDF_VIF_DIRECT_COMMAND 0x50000000

/* Fixed allocation size; the texture is deliberately unused. */
u32 sdf2DGetTexPacketSize(sdfTexHandle_t *texture) {
    return SDF_TEXTURE_DRAW_PACKET_BYTES;
}

/* Fill TEX1, TEX0 and CLAMP A+D writes for the selected GS context. */
SdfTextureDrawPacket *sdf2DMakeTexPacket(SdfTextureDrawPacket *drawPacket, sdfTexHandle_t *texture, s32 context) {
    drawPacket->header.quadwords = 4;
    drawPacket->header.gifTag = 0x1000000000008003ULL;
    drawPacket->header.command = 0x50000004;
    drawPacket->header.pad08 = 0;
    drawPacket->header.payloadHeader = SDF_GIF_REGISTER_AD;
    drawPacket->textureWordA = sdfTexGetTex1Reg(texture);
    drawPacket->registerAddressA = context + 0x14;
    drawPacket->textureWordB = sdfTexGetTex0Reg(texture);
    drawPacket->registerAddressB = context + 6;
    drawPacket->textureWordC = sdfTexGetClampReg(texture);
    drawPacket->registerAddressC = context + 8;
    return drawPacket;
}

/* Allocate and append texture state; return its packet address. */
SdfTextureDrawPacket *sdf2DLinkTexPacket(sdfDrawTag_t *packetList, sdfTexHandle_t *texture, s32 context) {
    SdfTextureDrawPacket *packet = sdf2DMakeTexPacket((SdfTextureDrawPacket *)sdfGrAllocPacketBuff(sdf2DGetTexPacketSize(texture)), texture, context);
    sdfDrawTagLink(packetList, (u32)packet);
    return packet;
}

/* Exclude the two header quadwords from the reference payload count. */
void *sdf2DMakeStdPacket(void *packet, s32 packetBytes) {
    sdfGrSetRegsPacketHeader((SdfGsPacketHeader *)packet, (packetBytes >> SDF_DMA_QWORD_SHIFT) - 2);
    return packet;
}

/* Packed GIF loops contain registerCount values, plus two header quadwords. */
s32 sdf2DGetPrimPacketSize(s32 registerCount, s32 loopCount) {
    return (registerCount * loopCount + 2) << SDF_DMA_QWORD_SHIFT;
}

/* Return the packet payload address after the DMA/GIF header. */
u8 *sdf2DGetPrimPtr(SdfDrawPacket *packet) {
    return (u8 *)packet + sizeof(*packet);
}

/* Build packed GIF NLOOP/NREG/PRIM fields and its VIF DIRECT command. */
SdfDrawPacket *sdf2DMakePrimPacketHeader(SdfDrawPacket *packet, s32 primitiveFlags, s32 registerCount, s64 registerList, s32 loopCount) {
    s32 packetQuadwords = registerCount * loopCount + 1;
    s64 gifTag = loopCount | ((s64)registerCount << SDF_GIF_NREG_SHIFT);

    gifTag |= (s64)primitiveFlags << SDF_GIF_PRIM_SHIFT;
    gifTag |= SDF_GIF_PRE_EOP_FLAGS;
    packet->gifTag = gifTag;
    packet->command = packetQuadwords | SDF_VIF_DIRECT_COMMAND;
    packet->payloadHeader = registerList;
    packet->pad08 = 0;
    packet->quadwords = packetQuadwords;
    return packet;
}

/* Allocate loopCount sprite loops with RGBAQ, UV, XYZ2, UV, XYZ2 registers. */
SdfDrawPacket *sdf2DAllocSprite(s32 loopCount) {
    SdfDrawPacket *packet = (SdfDrawPacket *)sdfGrAllocPacketBuff(sdf2DGetPrimPacketSize(5, loopCount));
    sdf2DMakePrimPacketHeader(packet, 0x156, 5, 0x53531, loopCount);
    return packet;
}
