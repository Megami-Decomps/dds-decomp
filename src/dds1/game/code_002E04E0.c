#include "sdf_gs_header.h"
#include "common.h"
#include "kwln.h"
#include "sdf_asset_packets.h"
#include "sdf_asset_state.h"
#include "sdf_vu_lighting.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "sdf_packet_alloc.h"
#include "sdf_texture_draw_packet.h"
#include "sdf_packet_append.h"
#include "sdf_resource.h"
#include "sdf_primitive.h"
#include "sdf.h"
#include "sdf_draw.h"
#include "sdf_projection.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "sdf_texture_file.h"
#include "sdf_vu_blend_node.h"
#include "sdf_vu_work.h"

#define SDF_DMA_ADDRESS_MASK 0x0FFFFFFF




/* Finish the selected node's transformed rows and emit its textured geometry. */ void corePktEmitMultiTexPass(VuWork *work);

/* Split ring addressing at position 128; retain the native 96-byte stride. */ void corePktSetupPointBuff(VuWork *work, s32 ringPosition);

/* Select textured or colored emission; preserve the native no-argument calls. */ void corePktEmitBase(VuWork *work);

/* Emit selected geometry, then apply flag callbacks; the first may change flags. */
void corePktEmitAllPasses(VuWork *work) {
    u32 selectedFlags;

    corePktEmitBase(work);
    selectedFlags = work->selectedFlags;
    if ((selectedFlags & 0x1000) != 0) {
        corePktEmitMultiTexPass(work);
        selectedFlags = work->selectedFlags;
    }
    if ((selectedFlags & 1) != 0) {
        corePktEmitRefMapPass((u32)work);
        return;
    }
}

extern void corePktBuildFaceList(VuWork *work);

extern void corePktBuildBackFaceList(VuWork *work);

extern void corePktRelightBackFaces(u32 base, s32 count);

/* Align the packet to 64 bytes; the optional second pass preserves vf24-vf26. */
void corePktOpenAndEmit(VuWork *work) {
    u32 packetStart = sdfGrOpenPacketBuff();
    u32 alignedStart = (packetStart + 0x3F) & ~0x3F;
    u32 ringAddress = ((alignedStart + 0x40) & SDF_DMA_ADDRESS_MASK) | 0x30000000;
    work->packetStart = packetStart;
    work->header = alignedStart + 0x30;
    work->ringStart = (u32 *)ringAddress;
    work->cursor = (u32 *)ringAddress;
    if ((work->selectedFlags & 0x4000) != 0) {
        u8 savedVectors[0x30];
        VU0_STORE_VF_UNCLOBBERED(vf24, savedVectors);
        VU0_STORE_VF_AT_UNCLOBBERED(vf25, 16, savedVectors);
        VU0_STORE_VF_AT_UNCLOBBERED(vf26, 32, savedVectors);
        corePktBuildFaceList(work);
        corePktEmitAllPasses(work);
        corePktSetupPointBuff(work, work->param1);
        VU0_LOAD_VF(vf24, savedVectors);
        VU0_LOAD_VF_AT(vf25, 16, savedVectors);
        VU0_LOAD_VF_AT(vf26, 32, savedVectors);
        corePktRelightBackFaces(work->dmaBase, work->param1);
        corePktBuildBackFaceList(work);
        corePktEmitAllPasses(work);
    } else {
        corePktBuildFaceList(work);
        corePktEmitAllPasses(work);
    }
}

/* Pad and finalize the packet; inactive or empty work returns NULL. */
u64 *corePktClosePacket(VuWork *work) {
    u32 cursor;
    u32 start;
    s32 quadwords;
    u32 descriptor;
    u64 gifTag;
    u64 *header;

    if (work->packetStart == 0) {
        return NULL;
    }

    cursor = (u32)work->cursor;
    start = (u32)work->ringStart;
    if (cursor == start) {
        sdfGrClosePacketBuff(work->packetStart);
        return NULL;
    }

    while (((u32)cursor & 0xC) != 0) {
        *(u32 *)cursor = 0;
        cursor += 4;
    }
    quadwords = (s32)(cursor - start) >> 4;

    while (((u32)cursor & 0x30) != 0) {
        *(u128 *)cursor = 0;
        cursor += 0x10;
    }

    sdfGrClosePacketBuff(cursor & 0x0FFFFFFF);
    header = (u64 *)work->header;
    descriptor = 0x20000000 | (quadwords & 0xFFFF);
    gifTag = 0x1100000000000000ULL;
    header[0] = descriptor;
    header[1] = gifTag;
    return header;
}
