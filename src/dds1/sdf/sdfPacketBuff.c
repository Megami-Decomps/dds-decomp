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

#define SDF_QWORD_ALIGNMENT_MASK 0xF

#define SDF_PACKET_BUFFER_ALIGNMENT_MASK 0x7F

#define SDF_PACKET_BUFFER_COUNT 2

extern s32 sdfGrPacketBuffSize;

extern s32 sdfPacketBuffers[2];

extern s32 sdfGrPacketBuffPtr;

extern s32 sdfGrPacketBuffLimit;

extern struct sdfMemHandle_t *sdfGrPacketBuffHandle;

/* Replace both packet buffers with one allocation, rounding each half to 128 bytes. */
void sdfGrMakePacketBuff(s32 bufferBytes) {
    s32 allocationAddress;
    if (sdfGrPacketBuffHandle != 0) {
        sdfMemFreeDirect(sdfGrPacketBuffHandle);
        sdfGrPacketBuffHandle = 0;
    }
    bufferBytes = (bufferBytes + SDF_PACKET_BUFFER_ALIGNMENT_MASK) & ~SDF_PACKET_BUFFER_ALIGNMENT_MASK;
    sdfGrPacketBuffSize = bufferBytes;
    sdfGrPacketBuffHandle = sdfMemAllocDirect(bufferBytes * SDF_PACKET_BUFFER_COUNT);
    allocationAddress = sdfMemLock(sdfGrPacketBuffHandle);
    sdfPacketBuffers[0] = allocationAddress;
    sdfPacketBuffers[1] = allocationAddress + bufferBytes;
}

/* Select one buffer half and reset the byte-address cursor and end bound. */
void sdfGrSetupPacketBuff(s32 bufferIndex) {
    sdfGrPacketBuffPtr = sdfPacketBuffers[bufferIndex];
    sdfGrPacketBuffLimit = sdfPacketBuffers[bufferIndex] + sdfGrPacketBuffSize;
}

s32 sdfGrGetPacketBuffRemain(void) {
    return sdfGrPacketBuffLimit - sdfGrPacketBuffPtr;
}

s32 sdfGrAllocPacketBuff(s32 size) {
    s32 packet;

    packet = sdfGrPacketBuffPtr;
    sdfGrPacketBuffPtr = sdfGrPacketBuffPtr + ((size + 0xfU) & 0xfffffff0);
    return packet;
}

s32 sdfGrOpenPacketBuff(void) {
    return sdfGrPacketBuffPtr;
}

/* Round a supplied byte address upward to the next quadword boundary. */
void sdfGrClosePacketBuff(s32 cursorAddress) {
    sdfGrPacketBuffPtr = (cursorAddress + SDF_QWORD_ALIGNMENT_MASK) & ~SDF_QWORD_ALIGNMENT_MASK;
}
INCLUDE_SDATA(const s32, "sdf/sdfPacketBuff", sdfGrPacketBuffHandle);

INCLUDE_SDATA(const s32, "sdf/sdfPacketBuff", sdfGrPacketBuffSize);

INCLUDE_SDATA(const s32, "sdf/sdfPacketBuff", sdfPacketBuffers);

INCLUDE_SDATA(const s32, "sdf/sdfPacketBuff", sdfGrPacketBuffPtr);

INCLUDE_SDATA(const s32, "sdf/sdfPacketBuff", sdfGrPacketBuffLimit);

