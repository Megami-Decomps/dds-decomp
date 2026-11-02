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

#define SDF_DMA_ADDRESS_MASK 0x0FFFFFFF

#define SDF_VIF_ITOP_LIGHTING 0x04000010

#define SDF_VIF_MSCAL_COMMAND 0x14000000

extern u8 D_00398580[];

/* Append the fixed clear block; NULL allocatePacket selects the packet allocator. */
void sdf3DPrimLinkResetTm(sdfDrawTag_t *packetList,
                             s32 (*allocatePacket)(s32)) {
    u64 *referencePacket;
    if (allocatePacket == NULL) {
        allocatePacket = sdfGrAllocPacketBuff;
    }
    referencePacket = (u64 *)allocatePacket(0x20);
    referencePacket[0] = ((u64)((u32)D_00398580 & SDF_DMA_ADDRESS_MASK) << 32) | 0x30000008;
    referencePacket[1] = 0x6C07C000ULL << 32;
    *(u128 *)&referencePacket[2] = 0;
    sdfDrawTagLinkRef(packetList, (u32)referencePacket);
}

/* vu0 routine: store the resident matrix and inverse-column-length scaled rows, then VIF ITOP/MSCAL. */
void sdf3DPrimSetTmPacket(VuLightingPacket *lightingPacket) {
    VU0_STORE_MATRIX_AND_UNIT_ROWS(lightingPacket);
    lightingPacket->tag[0] = SDF_VIF_ITOP_LIGHTING;
    lightingPacket->tag[1] = SDF_VIF_MSCAL_COMMAND;
    lightingPacket->tag[2] = 0;
    lightingPacket->tag[3] = 0;
}

/* Append inline matrix/lighting data using allocatePacket or the default allocator. */
void sdf3DPrimLinkTm(sdfDrawTag_t *packetList,
                           s32 (*allocatePacket)(s32)) {
    u64 *dmaPacket;
    if (allocatePacket == NULL) {
        allocatePacket = sdfGrAllocPacketBuff;
    }
    dmaPacket = (u64 *)allocatePacket(0x90);
    dmaPacket[0] = ((u64)((u32)(dmaPacket + 2) & SDF_DMA_ADDRESS_MASK) << 32) | 0x20000008;
    dmaPacket[1] = 0x6C07C000ULL << 32;
    sdf3DPrimSetTmPacket((VuLightingPacket *)(dmaPacket + 2));
    sdfDrawTagLink(packetList, (u32)dmaPacket);
}

extern vu8 sdfFrameId;

/* Apply current-buffer changes and append the asset reference; keep both index reads. */
void sdf3DPrimLinkMat(sdfDrawTag_t *packetList, sdfMat_t *asset,
                              s32 (*allocatePacket)(s32)) {
    u64 *referencePacket;
    if (allocatePacket == NULL) {
        allocatePacket = sdfGrAllocPacketBuff;
    }
    sdfMatUpdatePacket(asset, (s8)sdfFrameId);
    referencePacket = (u64 *)allocatePacket(0x20);
    sdfMatMakePacket(asset, (struct SdfDmaTagHeader *)referencePacket, (s8)sdfFrameId);
    *(u128 *)&referencePacket[2] = 0;
    sdfDrawTagLinkRef(packetList, (u32)referencePacket);
}

extern f32 sdfFaceDivSoften;

extern f32 sdfFaceDivBias;

extern u32 D_00398500[];

/* Serialize the requested VIF attribute streams into one aligned DMA packet. */
u32 *sdf3DPrimMakePoly(sdf3DPrimPolyParm_t *request) {
    s32 stripWords = request->faceCount;
    s32 count = request->pointCount;
    s32 headerWords = 18;
    s32 wordsPerVertex = 3;
    u32 primitive = request->attr | 0x68;
    u32 format = 0x60;
    s32 bytes;
    void *(*allocate)(s32);
    u32 *packet;
    u32 *cursor;
    const u128 *vector;
    const u32 *coordinates;
    const u32 *secondCoordinates;
    s32 i;
    u32 countCode;

    if (request->normal != NULL) {
        format = 0xE0;
        headerWords = 19;
        wordsPerVertex = 6;
    }
    if (request->uv != NULL) {
        format |= 0x100;
        primitive |= 0x10;
        headerWords++;
        wordsPerVertex += 2;
        if (request->multiUv != NULL) {
            primitive |= 0x1000;
            wordsPerVertex += 2;
        }
    }
    if (request->vertexColor != NULL) {
        format |= 0x200;
        primitive |= 0x800;
        headerWords++;
        wordsPerVertex++;
    }
    bytes = (headerWords + stripWords + wordsPerVertex * count) * 4;
    bytes = (bytes + 15) & ~15;
    allocate = request->allocator;
    if (allocate == NULL) allocate = sdfGrAllocPacketBuff;
    packet = allocate(bytes);
    *(u64 *)packet = (u64)(u16)((bytes >> 4) - 1) | 0x20000000ULL;
    packet[2] = 0x6102C000;
    packet[3] = (~request->attrInhibit) & 0xFFFF;
    packet[4] = 0x6E01C002;
    packet[5] = request->color;
    packet[6] = 0x6003C003;
    cursor = packet + 7;
    *cursor++ = (u32)request->zBias;
    *cursor++ = (u32)sdfFaceDivSoften;
    *cursor++ = (u32)sdfFaceDivBias;
    *cursor++ = 0x04000004;
    *cursor++ = 0x14000000;
    *cursor++ = 0x6D01C000;
    *cursor++ = (u16)stripWords | ((u32)(u16)count << 16);
    *cursor++ = (primitive & 0xFFFF) | (format << 16);
    *cursor++ = (stripWords << 16) | 0x6E00C001;
    if (request->faceIndex == NULL) {
        memcpy(cursor, D_00398500, stripWords * 4);
    } else {
        memcpy(cursor, request->faceIndex, stripWords * 4);
    }
    cursor += stripWords;
    stripWords++;
    countCode = count << 16;
    *cursor++ = countCode | stripWords | 0x6800C000;
    vector = request->vertex;
    i = 0;
    do {
        EE_MMI_STORE_VEC3_VALUE(cursor, *vector);
        cursor += 3;
        vector++;
        i++;
    } while (i != count);
    stripWords += count;
    vector = request->normal;
    if (vector != NULL) {
        *cursor++ = countCode | stripWords | 0x6800C000;
        i = 0;
        do {
            EE_MMI_STORE_VEC3_VALUE(cursor, *vector);
            cursor += 3;
            vector++;
            i++;
        } while (i != count);
        stripWords += count;
    }
    coordinates = request->uv;
    if (coordinates != NULL) {
        secondCoordinates = request->multiUv;
        if (secondCoordinates == NULL) {
            *cursor++ = countCode | stripWords | 0x6400C000;
            memcpy(cursor, coordinates, count * 8);
            cursor += count * 2;
        } else {
            *cursor++ = countCode | stripWords | 0x6C00C000;
            i = 0;
            do {
                *cursor++ = *coordinates++;
                *cursor++ = *coordinates++;
                *cursor++ = *secondCoordinates++;
                *cursor++ = *secondCoordinates++;
                i++;
            } while (i != count);
        }
        stripWords += count;
    }
    if (request->vertexColor != NULL) {
        *cursor++ = countCode | stripWords | 0x6E00C000;
        memcpy(cursor, request->vertexColor, count * 4);
        cursor += count;
    }
    *cursor = 0x1400000C;
    while ((u32)++cursor & 15) {
        *cursor = 0;
    }
    return packet;
}
