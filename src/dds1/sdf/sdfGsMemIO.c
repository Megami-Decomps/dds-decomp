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
#include "sdf_heap_api.h"
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
#include "pcp_vu0.h"
#include "sdf_draw.h"
#include "sdf_model.h"
#include "sdf_dev_slot_api.h"
#include "sdf_dma_tag.h"
#include "sdf_packet_alloc.h"

extern s32 sdfGetTexelBitCount(u32 format);

#define SDF_DMA_QWC_MASK 0xFFFF

#define SDF_DMA_ADDRESS_MASK 0x0FFFFFFF

#define SDF_PATCHABLE_PACKET_BYTES 0x100

#define SDF_PATCHABLE_PACKET_TAIL_OFFSET 0xD0

#define SDF_DESCRIPTOR_PACKET_BYTES 0xB0

#define SDF_DESCRIPTOR_PACKET_TAIL_OFFSET 0x80

extern sdfGrFrameSet_t D_003980E0;

void sdfPrimSetBitBltElem(SdfImageTransferRegisters *packet, u32 destinationBufferAddress, s32 destinationBufferWidth,
                  s64 destinationFormat, s64 destinationX, s64 destinationY,
                  u32 sourceBufferAddress, s32 sourceBufferWidth, s32 sourceFormat,
                  s32 sourceX, s32 sourceY, s32 transferWidth, s32 transferHeight, s32 transferDirection);

/* Transfer setup, deferred image metadata, and its linked primitive-reset tail. */
void sdfSetStoreImage(SdfResourcePacket *packet, sdfGsMemSlice_t *source,
    s32 sourceX, s32 sourceY, s32 width, s32 height, u32 resourceAddress, u32 arg7, u32 arg8) {
    s32 quadwordCount;

    packet->header[0] = 0x90000001ULL;
    packet->header[1] = 0x10000000ULL;
    packet->header[2] = 0;
    packet->header[3] = 0;
    packet->header[4] = 0x90000002ULL;
    packet->header[5] = 0x5000000206008000ULL;
    packet->header[6] = 0x1000000000000001ULL;
    packet->header[7] = 0xE;
    packet->header[8] = 0;
    packet->header[9] = 0x61;
    packet->header[10] = ((u64)((u32)packet->tail & SDF_DMA_ADDRESS_MASK) << 32) | 0xA0000005ULL;
    packet->header[11] = 0x5000000500000000ULL;
    packet->header[12] = 0x1000000000000004ULL;
    packet->header[13] = 0xE;
    sdfPrimSetBitBltElem(&packet->transfer, 0, 0, 0, 0, 0,
        source->word, source->width, source->format,
        sourceX, sourceY, width, height, 1);
    quadwordCount = (sdfGetTexelBitCount(source->format) * width * height) >> 7;
    packet->readback.quadwordCount = quadwordCount;
    packet->readback.destinationAddress = resourceAddress;
    packet->readback.callbackAddress = arg7;
    packet->readback.callbackArgument = arg8;
    packet->tail[0] = 0x20000002ULL;
    packet->tail[1] = 0x5000000206000000ULL;
    packet->tail[2] = 0x1000000000008001ULL;
    packet->tail[3] = 0xE;
    packet->tail[4] = 0x46;
    packet->tail[5] = 0;
}

void sdfLinkStoreImage(sdfDrawTag_t *list, sdfGsMemSlice_t *source, s32 sourceX,
                   s32 sourceY, s32 width, s32 height, s32 resourceAddress, s32 arg7,
                   s32 arg8, s32 (*allocate)(s32)) {
    s32 packetAddress;

    if (allocate == NULL) {
        allocate = sdfGrAllocPacketBuff;
    }
    packetAddress = allocate(0xf0);
    sdfSetStoreImage((SdfResourcePacket *)packetAddress, source,
        sourceX, sourceY, width, height, resourceAddress, arg7, arg8);
    sdfDrawTagLink2(list, packetAddress, packetAddress + 0xc0);
}

void sdfLinkStoreFrameImagePredraw(SdfPatchableResourcePacket *packet, s32 entryIndex) {
    packet->resourcePacket.transfer.bitbltbuf.value =
        (packet->resourcePacket.transfer.bitbltbuf.value & ~0x3FFF) |
        (u64)(u32)(D_003980E0.buffers[entryIndex]->word >> 6);
}

/* Allocate metadata plus a patchable DMA payload, registering both list views.
 * The drawing arguments remain opaque and are forwarded to the native builder. */
void sdfLinkStoreFrameImage(sdfDrawTag_t *list, sdfPreDrawTag_t *linkedList, s32 sourceX, s32 sourceY,
                   s32 width, s32 height, s32 resourceAddress, s32 arg7, s32 arg8,
                   s32 (*allocatePacket)(s32)) {
    SdfPatchableResourcePacket *packet;

    if (allocatePacket == NULL) {
        allocatePacket = sdfGrAllocPacketBuff;
    }
    packet = (SdfPatchableResourcePacket *)allocatePacket(SDF_PATCHABLE_PACKET_BYTES);
    packet->link.patch = sdfLinkStoreFrameImagePredraw;
    sdfSetStoreImage(&packet->resourcePacket,
        D_003980E0.buffers[0], sourceX, sourceY, width, height,
        resourceAddress, arg7, arg8);
    sdfPreDrawTagLink(linkedList, &packet->link);
    sdfDrawTagLink2(list, (u32)&packet->resourcePacket,
        (u32)packet + SDF_PATCHABLE_PACKET_TAIL_OFFSET);
}

void sdfSetLoadImage(SdfDescriptorPacket *packet, sdfGsMemSlice_t *source,
                   s64 destinationX, s64 destinationY, s32 transferWidth,
                   s32 transferHeight, u32 sourceAddress) {
    s32 qwc;

    packet->header[0] = 0x10000006;
    packet->header[1] = 0x5000000611000000;
    packet->header[2] = 0x1000000000000004;
    packet->header[3] = 0xE;
    sdfPrimSetBitBltElem(&packet->transfer, source->word, source->width,
                  source->format, destinationX, destinationY, 0, 0, 0, 0, 0,
                  transferWidth, transferHeight, 0);

    qwc = (sdfGetTexelBitCount(source->format) * transferWidth * transferHeight) >> 7;
    packet->imageReferenceTag = 0x0800000000000000 | qwc;
    packet->imageReferencePad = 0;
    packet->imageGifTag0 = (u32)((qwc & SDF_DMA_QWC_MASK) | 0x30000000) |
                           ((u64)(sourceAddress & SDF_DMA_ADDRESS_MASK) << 32);
    packet->imageGifTag1 = (u64)(0x51000000 | qwc) << 32;
    packet->flushTag0 = 0x20000002;
    packet->flushTag1 = 0x5000000200000000;
    packet->flushGifTag = 0x1000000000008001;
    packet->flushRegister = 0xE;
    packet->zero = 0;
    packet->finishRegister = 0x3F;
}

/* Allocate a descriptor packet, delegate its opaque options, and append its range. */
void sdfLinkLoadImage(sdfDrawTag_t *list, sdfGsMemSlice_t *source,
                   s32 destinationX, s32 destinationY, s32 width, s32 height,
                   s32 sourceAddress, s32 (*allocatePacket)(s32)) {
    s32 packetAddress;
    if (allocatePacket == NULL) {
        allocatePacket = sdfGrAllocPacketBuff;
    }
    packetAddress = allocatePacket(SDF_DESCRIPTOR_PACKET_BYTES);
    sdfSetLoadImage((SdfDescriptorPacket *)packetAddress, source,
                  destinationX, destinationY, width, height, sourceAddress);
    sdfDrawTagLink2(list, packetAddress, packetAddress + SDF_DESCRIPTOR_PACKET_TAIL_OFFSET);
}
