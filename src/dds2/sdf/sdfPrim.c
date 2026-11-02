#include "sdk_ps2.h"
#include "ee_mmi.h"
#include "sdf_scene_packet.h"
#include "sdf_gs_gouraud_textured.h"
#include "sdf_gs_scene_state.h"
#include "sdf_gs_textured_shapes.h"
#include "sdf_request.h"
#include "sdf_gs_context.h"
#include "sdf_gs_gouraud_shapes.h"
#include "sdf_gs_header.h"
#include "sdf_gs_geometry.h"
#include "sdf_texture_flush.h"
#include "sdf_tex_lifetime.h"
#include "sdf_gs_blend.h"
#include "common.h"
#include "sdf_delayed_resource_check_api.h"
#include "sdf_vu0.h"
#include "sdf_timer.h"
#include "sdf_packet_list.h"
#include "sdf_packet_slot.h"
#include "sdf_draw_frame_execution_state.h"
#include "sdf_chip.h"
#include "sdf_resource.h"
#include "pcp_vu0.h"
#include "sdf.h"
#include "sdf_semaphore_api.h"
#include "sdf_texture_registry.h"
#include "sdf_image_packets.h"
#include "sdf_pending.h"
#include "sdf_queued_resource_api.h"
#include "sdf_linked_packet.h"
#include "sdf_packet_builders.h"
#include "sdf_gs_flat_shapes.h"
#include "sdf_gs_packet.h"
#include "sdf_draw.h"
#include "sdf_model.h"
#include "sdf_dev_slot_api.h"
#include "sdf_dma_tag.h"
#include "sdf_packet_alloc.h"

#define SDF_PACKET_LIST_BYTES 0x20

#define SDF_GIF_ONE_REGISTER_WORD 0x10000000

#define SDF_GS_DEFAULT_ALPHA 0x44

#define SDF_GS_ALPHA_TEST 0x717FB

#define SDF_ALPHA_DMA_QWORDS 3

#define SDF_VIF_DIRECT_THREE_WORD 0x50000003

#define SDF_VIF_FLUSHE_HALFWORD 0x1000

#define SDF_GIF_TWO_AD_LOOPS_EOP 0x8002

extern sdfGrFrameSet_t D_0040B290;

void sdfPrimSetBitBltElem(SdfImageTransferRegisters *packet, u32 destinationBufferAddress, s32 destinationBufferWidth,
                  s64 destinationFormat, s64 destinationX, s64 destinationY,
                  u32 sourceBufferAddress, s32 sourceBufferWidth, s32 sourceFormat,
                  s32 sourceX, s32 sourceY, s32 transferWidth, s32 transferHeight, s32 transferDirection);

/* Reserve packet space at a 16-byte boundary, returning the old cursor. */ s32 sdfGrAllocPacketBuff(s32 size);

/* Clear all links and metadata before building a new packet list. */ void sdfDrawTagInit(sdfDrawTag_t *list);

/* Link the previous DMA packet to this packet with a NEXT tag. */ void sdfDrawTagLink(sdfDrawTag_t *list, u32 packet);

void sdfPreDrawTagLink(sdfPreDrawTag_t *list, SdfPacketPatchLink *node);

/* Allocate and initialize an empty packet list using the selected allocator. */
sdfDrawTag_t *sdfPrimAllocDrawTag(s32 (*allocatorArgument)(s32)) {
    s32 (*allocator)(s32) = allocatorArgument;
    sdfDrawTag_t *list;

    if (allocator == NULL) {
        allocator = sdfGrAllocPacketBuff;
    }
    list = (sdfDrawTag_t *)(u32)allocator(SDF_PACKET_LIST_BYTES);
    sdfDrawTagInit(list);
    return list;
}

/* Allocate the requested packet bytes, initialize through the callback, and append. */
void sdfPrimLinkSimplePacket(s32 listAddress, void (*initialize)(s32), s32 packetBytes, s32 (*allocatorArgument)(s32)) {
    s32 (*allocator)(s32) = allocatorArgument;
    s32 packetAddress;

    if (allocator == NULL) {
        allocator = sdfGrAllocPacketBuff;
    }
    packetAddress = allocator(packetBytes);
    initialize(packetAddress);
    sdfDrawTagLink(listAddress, packetAddress);
}

/* Set primary-context TEST and ALPHA values; 0x44 is blend data, not a register ID. */
void sdfPrimSetOpaqueElem(sdfPrimTransElem_t *packet) {
    packet->test.value = SDF_GS_ALPHA_TEST;
    packet->test.registerId = SDF_GS_TEST_1;
    packet->alpha.value = SDF_GS_DEFAULT_ALPHA;
    packet->alpha.registerId = SDF_GS_ALPHA_1;
}

/* Set the same TEST/ALPHA values for the secondary GS context. */
void sdfPrimSetOpaque2Elem(sdfPrimTransElem_t *packet) {
    packet->test.value = SDF_GS_ALPHA_TEST;
    packet->test.registerId = SDF_GS_TEST_2;
    packet->alpha.value = SDF_GS_DEFAULT_ALPHA;
    packet->alpha.registerId = SDF_GS_ALPHA_2;
}

/* Transfer a GIF tag and two primary-context A+D register writes with FLUSHE/DIRECT. */
void sdfPrimSetOpaquePacket(sdfPrimTransPacket_t *packet) {
    sdfPrimSetOpaqueElem(&packet->registers);
    packet->header.dmaTag = SDF_ALPHA_DMA_QWORDS;
    packet->header.vifCommands = (((u64)SDF_VIF_DIRECT_THREE_WORD << 16 | SDF_VIF_FLUSHE_HALFWORD) << 16);
    packet->header.gifTag = (((u64)SDF_GIF_ONE_REGISTER_WORD << 32) | SDF_GIF_TWO_AD_LOOPS_EOP);
    packet->header.gifRegisters = SDF_GIF_REGISTER_AD;
}

/* Transfer the corresponding secondary-context A+D pair without changing tag encoding. */
void sdfPrimSetOpaque2Packet(sdfPrimTransPacket_t *packet) {
    sdfPrimSetOpaque2Elem(&packet->registers);
    packet->header.dmaTag = SDF_ALPHA_DMA_QWORDS;
    packet->header.vifCommands = (((u64)SDF_VIF_DIRECT_THREE_WORD << 16 | SDF_VIF_FLUSHE_HALFWORD) << 16);
    packet->header.gifTag = (((u64)SDF_GIF_ONE_REGISTER_WORD << 32) | SDF_GIF_TWO_AD_LOOPS_EOP);
    packet->header.gifRegisters = SDF_GIF_REGISTER_AD;
}

void sdfPrimSetSemiTransElem(sdfPrimTransElem_t *packet) {
    packet->test.value = 0x717FB;
    packet->test.registerId = 0x47;
    packet->alpha.value = 0x44;
    packet->alpha.registerId = 0x42;
}

void sdfPrimSetSemiTrans2Elem(sdfPrimTransElem_t *packet) {
    packet->test.value = 0x717FB;
    packet->test.registerId = 0x48;
    packet->alpha.value = 0x44;
    packet->alpha.registerId = 0x43;
}

void sdfPrimSetSemiTransPacket(sdfPrimTransPacket_t *packet) {
    sdfPrimSetSemiTransElem(&packet->registers);
    packet->header.dmaTag = 3;
    packet->header.vifCommands = (((u64)0x50000003 << 16 | 0x1000) << 16);
    packet->header.gifTag = (((u64)0x10000000 << 32) | 0x8002);
    packet->header.gifRegisters = 0xE;
}

void sdfPrimSetSemiTrans2Packet(sdfPrimTransPacket_t *packet) {
    sdfPrimSetSemiTrans2Elem(&packet->registers);
    packet->header.dmaTag = 3;
    packet->header.vifCommands = (((u64)0x50000003 << 16 | 0x1000) << 16);
    packet->header.gifTag = (((u64)0x10000000 << 32) | 0x8002);
    packet->header.gifRegisters = 0xE;
}

void sdfPrimSetAddTransElem(sdfPrimTransElem_t *packet) {
    packet->test.value = 0x71801;
    packet->test.registerId = 0x47;
    packet->alpha.value = 0x48;
    packet->alpha.registerId = 0x42;
}

void sdfPrimSetAddTrans2Elem(sdfPrimTransElem_t *packet) {
    packet->test.value = 0x71801;
    packet->test.registerId = 0x48;
    packet->alpha.value = 0x48;
    packet->alpha.registerId = 0x43;
}

void sdfPrimSetAddTransPacket(sdfPrimTransPacket_t *packet) {
    sdfPrimSetAddTransElem(&packet->registers);
    packet->header.dmaTag = 3;
    packet->header.vifCommands = (((u64)0x50000003 << 16 | 0x1000) << 16);
    packet->header.gifTag = (((u64)0x10000000 << 32) | 0x8002);
    packet->header.gifRegisters = 0xE;
}

void sdfPrimSetAddTrans2Packet(sdfPrimTransPacket_t *packet) {
    sdfPrimSetAddTrans2Elem(&packet->registers);
    packet->header.dmaTag = 3;
    packet->header.vifCommands = (((u64)0x50000003 << 16 | 0x1000) << 16);
    packet->header.gifTag = (((u64)0x10000000 << 32) | 0x8002);
    packet->header.gifRegisters = 0xE;
}

void sdfPrimSetSubTransElem(sdfPrimTransElem_t *packet) {
    packet->test.value = 0x71801;
    packet->test.registerId = 0x47;
    packet->alpha.value = 0x42;
    packet->alpha.registerId = 0x42;
}

void sdfPrimSetSubTrans2Elem(sdfPrimTransElem_t *packet) {
    packet->test.value = 0x71801;
    packet->test.registerId = 0x48;
    packet->alpha.value = 0x42;
    packet->alpha.registerId = 0x43;
}

void sdfPrimSetSubTransPacket(sdfPrimTransPacket_t *packet) {
    sdfPrimSetSubTransElem(&packet->registers);
    packet->header.dmaTag = 3;
    packet->header.vifCommands = (((u64)0x50000003 << 16 | 0x1000) << 16);
    packet->header.gifTag = (((u64)0x10000000 << 32) | 0x8002);
    packet->header.gifRegisters = 0xE;
}

void sdfPrimSetSubTrans2Packet(sdfPrimTransPacket_t *packet) {
    sdfPrimSetSubTrans2Elem(&packet->registers);
    packet->header.dmaTag = 3;
    packet->header.vifCommands = (((u64)0x50000003 << 16 | 0x1000) << 16);
    packet->header.gifTag = (((u64)0x10000000 << 32) | 0x8002);
    packet->header.gifRegisters = 0xE;
}

void sdfPrimSetTexflushElem(SdfGsRegisterWrite *write) {
    write->value = 0;
    write->registerId = SDF_GS_TEXFLUSH;
}

void sdfPrimSetTexflushPacket(SdfGsTextureFlushPacket *packet) {
    sdfPrimSetTexflushElem(&packet->textureFlush);
    packet->header.dmaTag = 2;
    packet->header.vifCommands = (((u64)0x50000002 << 16 | 0x1000) << 16);
    packet->header.gifTag = (((u64)0x10000000 << 32) | 0x8001);
    packet->header.gifRegisters = 0xE;
}

/* Emit the four A+D registers controlling an image transfer. */
void sdfPrimSetBitBltElem(SdfImageTransferRegisters *packet, u32 destinationBufferAddress, s32 destinationBufferWidth,
                  s64 destinationFormat, s64 destinationX, s64 destinationY,
                  u32 sourceBufferAddress, s32 sourceBufferWidth, s32 sourceFormat,
                  s32 sourceX, s32 sourceY, s32 transferWidth, s32 transferHeight, s32 transferDirection) {
    packet->bitbltbuf.value = (destinationFormat << 56) |
        ((u64)(destinationBufferWidth >> 6) << 48) |
        ((u64)(destinationBufferAddress >> 6) << 32) |
        (sourceFormat << 24) | ((sourceBufferWidth >> 6) << 16) |
        (sourceBufferAddress >> 6);
    packet->bitbltbuf.registerId = SDF_GS_BITBLTBUF;
    packet->trxpos.value = (destinationY << 48) | (destinationX << 32) |
        (sourceY << 16) | sourceX;
    packet->trxpos.registerId = SDF_GS_TRXPOS;
    packet->trxreg.value = transferWidth | ((u64)transferHeight << 32);
    packet->trxreg.registerId = SDF_GS_TRXREG;
    packet->trxdir.value = transferDirection;
    packet->trxdir.registerId = SDF_GS_TRXDIR;
}

/* Wrap BITBLTBUF, TRXPOS, TRXREG and TRXDIR image-transfer settings in a DMA/GIF header. */
void sdfPrimSetBitBltPacket(SdfImageTransferPacket *packet, u32 destinationBufferAddress, s32 destinationBufferWidth,
                                    s64 destinationFormat, s64 destinationX, s64 destinationY,
                                    u32 sourceBufferAddress, s32 sourceBufferWidth, s32 sourceFormat,
                                    s32 sourceX, s32 sourceY, s32 transferWidth, s32 transferHeight, s32 transferDirection) {
    packet->header.dmaTag = 5;
    packet->header.vifCommands = (((u64)0x50000005 << 16 | 0x1000) << 16);
    packet->header.gifTag = (((u64)0x10000000 << 32) | 0x8004);
    packet->header.gifRegisters = 0xE;
    sdfPrimSetBitBltElem(&packet->transfer, destinationBufferAddress, destinationBufferWidth,
                  destinationFormat, destinationX, destinationY, sourceBufferAddress, sourceBufferWidth,
                  sourceFormat, sourceX, sourceY, transferWidth, transferHeight, transferDirection);
}

/* Allocate and append the image-transfer packet; use the packet arena when allocator is null. */
void sdfPrimLinkBitBltPacket(sdfDrawTag_t *packetList, u32 destinationBufferAddress, s32 destinationBufferWidth,
                             s64 destinationFormat, s64 destinationX, s64 destinationY,
                             u32 sourceBufferAddress, s32 sourceBufferWidth, s32 sourceFormat,
                             s32 sourceX, s32 sourceY, s32 transferWidth, s32 transferHeight,
                             s32 transferDirection, s32 (*allocator)(s32)) {
    s32 packetAddress;

    if (allocator == NULL) {
        allocator = sdfGrAllocPacketBuff;
    }
    packetAddress = allocator(0x60);
    sdfPrimSetBitBltPacket((SdfImageTransferPacket *)packetAddress, destinationBufferAddress, destinationBufferWidth,
                                    destinationFormat, destinationX, destinationY, sourceBufferAddress, sourceBufferWidth,
                                    sourceFormat, sourceX, sourceY, transferWidth, transferHeight, transferDirection);
    sdfDrawTagLink(packetList, packetAddress);
}

void sdfPrimFrameMovePredraw(SdfGraphCopyPacket *packet, s32 entryIndex) {
    packet->draw.transfer.bitbltbuf.value =
        (packet->draw.transfer.bitbltbuf.value & ~0x3FFF) |
        (u64)(u32)(D_0040B290.buffers[entryIndex ^ packet->resourceIndexXor]->word >> 6);
}

/* Build a local-to-local GS copy from graph buffer zero into destination. */
void sdfPrimLinkFrameMovePacket(sdfDrawTag_t *drawList, sdfPreDrawTag_t *linkedList,
                   sdfGsMemSlice_t *destination, s32 destinationX, s32 destinationY,
                   s32 sourceX, s32 sourceY, s32 transferWidth, s32 transferHeight,
                   s32 resourceIndexXor, s32 (*allocPacket)(s32)) {
    SdfGraphCopyPacket *packet;
    SdfImageTransferPacket *drawPacket;

    if (allocPacket == NULL) {
        allocPacket = sdfGrAllocPacketBuff;
    }
    packet = (SdfGraphCopyPacket *)allocPacket(0x70);
    packet->resourceIndexXor = resourceIndexXor;
    packet->link.patch = sdfPrimFrameMovePredraw;
    drawPacket = &packet->draw;

    sdfPrimSetBitBltPacket(
        drawPacket, destination->word, destination->width,
        destination->format, destinationX, destinationY, D_0040B290.buffers[0]->word,
        D_0040B290.width, D_0040B290.bufferFormat, sourceX, sourceY, transferWidth,
        transferHeight, 2);
    sdfPreDrawTagLink(linkedList, &packet->link);
    sdfDrawTagLink(drawList, (s32)drawPacket);
}

/* Pack two UV/XYZ vertex pairs after the common primitive and color. */
void sdfPrimSetSpriteElem(SdfGsTexturedPairPayload *packet, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 depth) {
    u64 depthHigh = (u64)depth << 32;

    packet->gifTag = 0x6400000000008001ULL;
    packet->gifRegisters = 0x535310;
    packet->primitive = (u32)(primitive | 0x116);
    packet->rgbaq = (u32)color | ((u64)0xFE00 << 46);
    packet->vertices[0].uv = (u0 & 0xFFFF) | (v0 << 16);
    packet->vertices[0].xyz2 = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    packet->vertices[1].uv = (u1 & 0xFFFF) | (v1 << 16);
    packet->vertices[1].xyz2 = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
}

void sdfPrimLinkSprite(sdfDrawTag_t *list, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0,
                   s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 depth, s32 (*alloc)(s32)) {
    SdfGsTexturedPairPacket *packet;
    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (SdfGsTexturedPairPacket *)alloc(0x50);
    packet->dmaTag = 0x20000004;
    packet->vifCommands = (((u64)0x50000004 << 16) | 0x1000) << 16;
    sdfPrimSetSpriteElem(&packet->drawing, color, primitive, x0, y0, u0, v0, x1, y1, u1, v1, depth);
    sdfDrawTagLink(list, (s32)packet);
}

/* Pack three GS XYZ vertices with a common depth. */
void sdfPrimSetF3Elem(SdfGsFlatTrianglePayload *packet, s32 color, s32 primitive, s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 depth) {
    u64 depthHigh = (u64)depth << 32;

    packet->gifTag = 0x6400000000008001ULL;
    packet->gifRegisterList = 0xF55510;
    packet->primitive = (u32)(primitive | 0x104);
    packet->rgbaq = (u32)color | ((u64)0xFE00 << 46);
    packet->xyz2[0] = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    packet->xyz2[1] = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
    packet->xyz2[2] = (u32)((x2 & 0xFFFF) | (y2 << 16)) | depthHigh;
}

void sdfPrimLinkF3(sdfDrawTag_t *list, s32 color, s32 primitive, s32 x0, s32 y0, s32 x1,
                   s32 y1, s32 x2, s32 y2, s32 depth, s32 (*alloc)(s32)) {
    SdfGsFlatTrianglePacket *packet;
    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (SdfGsFlatTrianglePacket *)alloc(0x50);
    packet->dmaTag = 0x20000004;
    packet->vifCommands = (((u64)0x50000004 << 16) | 0x1000) << 16;
    sdfPrimSetF3Elem(&packet->drawing, color, primitive, x0, y0, x1, y1, x2, y2, depth);
    sdfDrawTagLink(list, (s32)packet);
}

/* Pack four GS XYZ vertices with a common depth. */
void sdfPrimSetF4Elem(SdfGsFlatQuadPayload *packet, s32 color, s32 primitive, s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 x3, s32 y3, s32 depth) {
    u64 depthHigh = (u64)depth << 32;

    packet->gifTag = 0x6400000000008001ULL;
    packet->gifRegisterList = 0x555510;
    packet->primitive = (u32)(primitive | 0x104);
    packet->rgbaq = (u32)color | ((u64)0xFE00 << 46);
    packet->xyz2[0] = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    packet->xyz2[1] = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
    packet->xyz2[2] = (u32)((x2 & 0xFFFF) | (y2 << 16)) | depthHigh;
    packet->xyz2[3] = (u32)((x3 & 0xFFFF) | (y3 << 16)) | depthHigh;
}

void sdfPrimLinkF4(sdfDrawTag_t *list, s32 color, s32 primitive, s32 x0, s32 y0, s32 x1,
                   s32 y1, s32 x2, s32 y2, s32 x3, s32 y3, s32 depth, s32 (*alloc)(s32)) {
    SdfGsFlatQuadPacket *packet;
    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (SdfGsFlatQuadPacket *)alloc(0x50);
    packet->dmaTag = 0x20000004;
    packet->vifCommands = (((u64)0x50000004 << 16) | 0x1000) << 16;
    sdfPrimSetF4Elem(&packet->drawing, color, primitive, x0, y0, x1, y1, x2, y2, x3, y3, depth);
    sdfDrawTagLink(list, (s32)packet);
}

/* Emit three GS vertices, each with its own packed color and shared depth. */
void sdfPrimSetG3Elem(SdfGsGouraudTrianglePayload *packet, s32 primitive, s32 x0, s32 y0, s32 color0, s32 x1, s32 y1, s32 color1, s32 x2, s32 y2, s32 color2, s32 depth) {
    u64 depthHigh = (u64)depth << 32;

    packet->gifTag = 0x8400000000008001ULL;
    packet->gifRegisters = 0xFFFFFFFFF5151510ULL;
    packet->primitive = (u32)(primitive | 0x10C);
    packet->vertices[0].rgbaq = (u32)color0 | ((u64)0xFE00 << 46);
    packet->vertices[0].xyz2 = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    packet->vertices[1].rgbaq = (u32)color1 | ((u64)0xFE00 << 46);
    packet->vertices[1].xyz2 = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
    packet->vertices[2].rgbaq = (u32)color2 | ((u64)0xFE00 << 46);
    packet->vertices[2].xyz2 = (u32)((x2 & 0xFFFF) | (y2 << 16)) | depthHigh;
}

void sdfPrimLinkG3(s32 list, s32 primitive, s32 x0, s32 y0, s32 color0, s32 x1, s32 y1,
                   s32 color1, s32 x2, s32 y2, s32 color2, s32 depth, s32 (*alloc)(s32)) {
    SdfGsGouraudTrianglePacket *packet;
    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (SdfGsGouraudTrianglePacket *)alloc(0x60);
    packet->dmaTag = 0x20000005;
    packet->vifCommands = (((u64)0x50000005 << 16) | 0x1000) << 16;
    sdfPrimSetG3Elem(&packet->drawing, primitive, x0, y0, color0, x1, y1, color1, x2, y2, color2, depth);
    sdfDrawTagLink(list, (s32)packet);
}

/* Four vertices each carry their own color, with a common Z value. */
void sdfPrimSetG4Elem(SdfGsGouraudQuadPayload *packet, s32 primitive, s32 x0, s32 y0, s32 color0, s32 x1, s32 y1, s32 color1, s32 x2, s32 y2, s32 color2, s32 x3, s32 y3, s32 color3, s32 depth) {
    u64 depthHigh = (u64)depth << 32;

    packet->gifTag = 0xA400000000008001ULL;
    packet->gifRegisters = 0xF515151510ULL;
    packet->primitive = (u32)(primitive | 0x10C);
    packet->vertices[0].rgbaq = (u32)color0 | ((u64)0xFE00 << 46);
    packet->vertices[0].xyz2 = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    packet->vertices[1].rgbaq = (u32)color1 | ((u64)0xFE00 << 46);
    packet->vertices[1].xyz2 = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
    packet->vertices[2].rgbaq = (u32)color2 | ((u64)0xFE00 << 46);
    packet->vertices[2].xyz2 = (u32)((x2 & 0xFFFF) | (y2 << 16)) | depthHigh;
    packet->vertices[3].rgbaq = (u32)color3 | ((u64)0xFE00 << 46);
    packet->vertices[3].xyz2 = (u32)((x3 & 0xFFFF) | (y3 << 16)) | depthHigh;
}

void sdfPrimLinkG4(sdfDrawTag_t *list, s32 primitive, s32 x0, s32 y0, s32 color0, s32 x1,
                   s32 y1, s32 color1, s32 x2, s32 y2, s32 color2, s32 x3,
                   s32 y3, s32 color3, s32 depth, s32 (*alloc)(s32)) {
    SdfGsGouraudQuadPacket *packet;
    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (SdfGsGouraudQuadPacket *)alloc(0x70);
    packet->dmaTag = 0x20000006;
    packet->vifCommands = (((u64)0x50000006 << 16) | 0x1000) << 16;
    sdfPrimSetG4Elem(&packet->drawing, primitive, x0, y0, color0, x1, y1, color1, x2, y2, color2, x3, y3, color3, depth);
    sdfDrawTagLink(list, (s32)packet);
}

/* Pack three UV/XYZ vertex pairs after the common primitive and color. */
void sdfPrimSetFt3Elem(SdfGsTexturedTrianglePayload *packet, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 x2, s32 y2, s32 u2, s32 v2, s32 depth) {
    u64 depthHigh = (u64)depth << 32;

    packet->gifTag = 0x8400000000008001ULL;
    packet->gifRegisters = 0x53535310;
    packet->primitive = (u32)(primitive | 0x114);
    packet->rgbaq = (u32)color | ((u64)0xFE00 << 46);
    packet->vertices[0].uv = (u0 & 0xFFFF) | (v0 << 16);
    packet->vertices[0].xyz2 = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    packet->vertices[1].uv = (u1 & 0xFFFF) | (v1 << 16);
    packet->vertices[1].xyz2 = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
    packet->vertices[2].uv = (u2 & 0xFFFF) | (v2 << 16);
    packet->vertices[2].xyz2 = (u32)((x2 & 0xFFFF) | (y2 << 16)) | depthHigh;
}

void sdfPrimLinkFt3(sdfDrawTag_t *list, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0,
                   s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 x2, s32 y2,
                   s32 u2, s32 v2, s32 depth, s32 (*alloc)(s32)) {
    SdfGsTexturedTrianglePacket *packet;
    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (SdfGsTexturedTrianglePacket *)alloc(0x60);
    packet->dmaTag = 0x20000005;
    packet->vifCommands = (((u64)0x50000005 << 16) | 0x1000) << 16;
    sdfPrimSetFt3Elem(&packet->drawing, color, primitive, x0, y0, u0, v0, x1, y1, u1, v1, x2, y2, u2, v2, depth);
    sdfDrawTagLink(list, (s32)packet);
}

/* Pack four textured vertices with a common color and depth. */
void sdfPrimSetFt4Elem(SdfGsTexturedQuadPayload *packet, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0,
                   s32 x1, s32 y1, s32 u1, s32 v1, s32 x2, s32 y2, s32 u2, s32 v2,
                   s32 x3, s32 y3, s32 u3, s32 v3, s32 depth) {
    u64 depthHigh = (u64)depth << 32;

    packet->gifTag = 0xA400000000008001ULL;
    packet->gifRegisters = 0x5353535310ULL;
    packet->primitive = (u32)(primitive | 0x114);
    packet->rgbaq = (u32)color | ((u64)0xFE00 << 46);
    packet->vertices[0].uv = (u0 & 0xFFFF) | (v0 << 16);
    packet->vertices[0].xyz2 = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    packet->vertices[1].uv = (u1 & 0xFFFF) | (v1 << 16);
    packet->vertices[1].xyz2 = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
    packet->vertices[2].uv = (u2 & 0xFFFF) | (v2 << 16);
    packet->vertices[2].xyz2 = (u32)((x2 & 0xFFFF) | (y2 << 16)) | depthHigh;
    packet->vertices[3].uv = (u3 & 0xFFFF) | (v3 << 16);
    packet->vertices[3].xyz2 = (u32)((x3 & 0xFFFF) | (y3 << 16)) | depthHigh;
}

void sdfPrimLinkFt4(sdfDrawTag_t *list, s32 color, s32 primitive, s32 x0, s32 y0, s32 u0,
                   s32 v0, s32 x1, s32 y1, s32 u1, s32 v1, s32 x2, s32 y2,
                   s32 u2, s32 v2, s32 x3, s32 y3, s32 u3, s32 v3, s32 depth,
                   s32 (*alloc)(s32)) {
    SdfGsTexturedQuadPacket *packet;
    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (SdfGsTexturedQuadPacket *)alloc(0x70);
    packet->dmaTag = 0x20000006;
    packet->vifCommands = (((u64)0x50000006 << 16) | 0x1000) << 16;
    sdfPrimSetFt4Elem(&packet->drawing, color, primitive, x0, y0, u0, v0, x1, y1, u1, v1, x2, y2, u2, v2, x3, y3, u3, v3, depth);
    sdfDrawTagLink(list, (s32)packet);
}

/* Three vertices each carry UV and color plus a shared Z value. */
void sdfPrimSetGt3Elem(SdfGsGouraudTexturedTrianglePayload *packet, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 color0,
                   s32 x1, s32 y1, s32 u1, s32 v1, s32 color1, s32 x2, s32 y2,
                   s32 u2, s32 v2, s32 color2, s32 depth) {
    u64 depthHigh = (u64)depth << 32;

    packet->gifTag = 0xA400000000008001ULL;
    packet->gifRegisters = 0x5135135130ULL;
    packet->primitive = (u32)(primitive | 0x11C);
    packet->vertices[0].uv = (u0 & 0xFFFF) | (v0 << 16);
    packet->vertices[0].rgbaq = (u32)color0 | ((u64)0xFE00 << 46);
    packet->vertices[0].xyz2 = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    packet->vertices[1].uv = (u1 & 0xFFFF) | (v1 << 16);
    packet->vertices[1].rgbaq = (u32)color1 | ((u64)0xFE00 << 46);
    packet->vertices[1].xyz2 = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
    packet->vertices[2].uv = (u2 & 0xFFFF) | (v2 << 16);
    packet->vertices[2].rgbaq = (u32)color2 | ((u64)0xFE00 << 46);
    packet->vertices[2].xyz2 = (u32)((x2 & 0xFFFF) | (y2 << 16)) | depthHigh;
}

void sdfPrimLinkGt3(sdfDrawTag_t *list, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0,
                   s32 color0, s32 x1, s32 y1, s32 u1, s32 v1, s32 color1,
                   s32 x2, s32 y2, s32 u2, s32 v2, s32 color2, s32 depth,
                   s32 (*alloc)(s32)) {
    SdfGsGouraudTexturedTrianglePacket *packet;
    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (SdfGsGouraudTexturedTrianglePacket *)alloc(0x70);
    packet->dmaTag = 0x20000006;
    packet->vifCommands = (((u64)0x50000006 << 16) | 0x1000) << 16;
    sdfPrimSetGt3Elem(&packet->drawing, primitive, x0, y0, u0, v0, color0, x1, y1, u1, v1, color1, x2, y2, u2, v2, color2, depth);
    sdfDrawTagLink(list, (s32)packet);
}

/* Pack four vertices with individual UV and color and a common depth. */
void sdfPrimSetGt4Elem(SdfGsGouraudTexturedQuadPayload *packet, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0, s32 color0,
                   s32 x1, s32 y1, s32 u1, s32 v1, s32 color1, s32 x2, s32 y2, s32 u2,
                   s32 v2, s32 color2, s32 x3, s32 y3, s32 u3, s32 v3, s32 color3, s32 depth) {
    u64 depthHigh = (u64)depth << 32;

    packet->gifTag = 0xE400000000008001ULL;
    packet->gifRegisters = 0xF5135135135130ULL;
    packet->primitive = (u32)(primitive | 0x11C);
    packet->vertices[0].uv = (u0 & 0xFFFF) | (v0 << 16);
    packet->vertices[0].rgbaq = (u32)color0 | ((u64)0xFE00 << 46);
    packet->vertices[0].xyz2 = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    packet->vertices[1].uv = (u1 & 0xFFFF) | (v1 << 16);
    packet->vertices[1].rgbaq = (u32)color1 | ((u64)0xFE00 << 46);
    packet->vertices[1].xyz2 = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
    packet->vertices[2].uv = (u2 & 0xFFFF) | (v2 << 16);
    packet->vertices[2].rgbaq = (u32)color2 | ((u64)0xFE00 << 46);
    packet->vertices[2].xyz2 = (u32)((x2 & 0xFFFF) | (y2 << 16)) | depthHigh;
    packet->vertices[3].uv = (u3 & 0xFFFF) | (v3 << 16);
    packet->vertices[3].rgbaq = (u32)color3 | ((u64)0xFE00 << 46);
    packet->vertices[3].xyz2 = (u32)((x3 & 0xFFFF) | (y3 << 16)) | depthHigh;
}

void sdfPrimLinkGt4(sdfDrawTag_t *list, s32 primitive, s32 x0, s32 y0, s32 u0, s32 v0,
                   s32 color0, s32 x1, s32 y1, s32 u1, s32 v1, s32 color1,
                   s32 x2, s32 y2, s32 u2, s32 v2, s32 color2, s32 x3, s32 y3,
                   s32 u3, s32 v3, s32 color3, s32 depth, s32 (*alloc)(s32)) {
    SdfGsGouraudTexturedQuadPacket *packet;
    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (SdfGsGouraudTexturedQuadPacket *)alloc(0x90);
    packet->dmaTag = 0x20000008;
    packet->vifCommands = (((u64)0x50000008 << 16) | 0x1000) << 16;
    sdfPrimSetGt4Elem(&packet->drawing, primitive, x0, y0, u0, v0, color0, x1, y1, u1, v1, color1, x2, y2, u2, v2, color2, x3, y3, u3, v3, color3, depth);
    sdfDrawTagLink(list, (s32)packet);
}

/* Emit two packed GS XYZ vertices; the high word supplies their shared depth. */
void sdfPrimSetRectElem(SdfGsTwoVertexPayload *packet, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth) {
    u64 depthHigh = (u64)depth << 32;

    packet->gifTag = 0x4400000000008001ULL;
    packet->gifRegisters = 0x5510;
    packet->primitive = (u32)(primitive | 0x106);
    packet->rgbaq = (u32)color | ((u64)0xFE00 << 46);
    packet->xyz2[0] = (u32)((left & 0xFFFF) | (top << 16)) | depthHigh;
    packet->xyz2[1] = (u32)((right & 0xFFFF) | (bottom << 16)) | depthHigh;
}

void sdfPrimLinkRect(sdfDrawTag_t *list, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth, s32 (*alloc)(s32)) {
    SdfGsTwoVertexPacket *packet;

    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (SdfGsTwoVertexPacket *)alloc(0x40);
    packet->dmaTag = 0x20000003ULL;
    packet->vifCommands = 0x5000000310000000ULL;
    sdfPrimSetRectElem(&packet->drawing, color, primitive, left, top, right, bottom, depth);
    sdfDrawTagLink(list, (u32)packet);
}

void sdfPrimSetLineElem(SdfGsTwoVertexPayload *packet, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth) {
    u64 depthHigh = (u64)depth << 32;

    packet->gifTag = 0x4400000000008001ULL;
    packet->gifRegisters = 0x5510;
    packet->primitive = (u32)(primitive | 0x101);
    packet->rgbaq = (u32)color | ((u64)0xFE00 << 46);
    packet->xyz2[0] = (u32)((left & 0xFFFF) | (top << 16)) | depthHigh;
    packet->xyz2[1] = (u32)((right & 0xFFFF) | (bottom << 16)) | depthHigh;
}

void sdfPrimLinkLine(sdfDrawTag_t *list, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth, s32 (*alloc)(s32)) {
    SdfGsTwoVertexPacket *packet;

    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (SdfGsTwoVertexPacket *)alloc(0x40);
    packet->dmaTag = 0x20000003ULL;
    packet->vifCommands = 0x5000000310000000ULL;
    sdfPrimSetLineElem(&packet->drawing, color, primitive, left, top, right, bottom, depth);
    sdfDrawTagLink(list, (u32)packet);
}

/* Close the rectangle by repeating its first GS XYZ vertex. */
void sdfPrimSetBoxLineElem(SdfGsClosedRectanglePayload *packet, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth) {
    u64 depthHigh = (u64)depth << 32;

    packet->gifTag = 0x8400000000008001ULL;
    packet->gifRegisterList = 0xFFFFFFFFF5555510ULL;
    packet->primitive = (u32)(primitive | 0x102);
    packet->rgbaq = (u32)color | ((u64)0xFE00 << 46);
    packet->xyz2[0] = (u32)((left & 0xFFFF) | (top << 16)) | depthHigh;
    packet->xyz2[1] = (u32)((right & 0xFFFF) | (top << 16)) | depthHigh;
    packet->xyz2[2] = (u32)((right & 0xFFFF) | (bottom << 16)) | depthHigh;
    packet->xyz2[3] = (u32)((left & 0xFFFF) | (bottom << 16)) | depthHigh;
    packet->xyz2[4] = (u32)((left & 0xFFFF) | (top << 16)) | depthHigh;
}

void sdfPrimLinkBoxLine(sdfDrawTag_t *list, s32 color, s32 primitive, s32 left, s32 top, s32 right, s32 bottom, s32 depth, s32 (*alloc)(s32)) {
    SdfGsClosedRectanglePacket *packet;

    if (alloc == NULL) {
        alloc = sdfGrAllocPacketBuff;
    }
    packet = (SdfGsClosedRectanglePacket *)alloc(0x60);
    packet->dmaTag = 0x20000005ULL;
    packet->vifCommands = 0x5000000510000000ULL;
    sdfPrimSetBoxLineElem(&packet->drawing, color, primitive, left, top, right, bottom, depth);
    sdfDrawTagLink(list, (s32)packet);
}
