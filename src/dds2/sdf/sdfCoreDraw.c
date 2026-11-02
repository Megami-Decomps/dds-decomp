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

#define SDF_DMA_CHCR_TTE 0x40

extern const struct SdfDmaTagHeader *corePktCurrLight;

extern u32 D_0043918C;

extern f32 D_00439190;

extern u8 D_0037B080[];

extern u8 D_0037B610[];

extern sdfMemHandle_t *D_00438A40;

extern s32 D_00439180;

#ifdef NON_MATCHING
#include "sdf_gs_header.h"
#include "eeregs.h"
#include "common.h"
#include "sdf_vu_matrix.h"
#include "sdf_pad_entry.h"
#include "sdf_pad_mode_api.h"
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
#include "sdf_packet_alloc.h"
#include "sdfCorePacketCb_t.h"



/* vf1 = (1, 1, 1, -1) from vf0 = (0, 0, 0, 1). */
#define MODEL_VU_INITIALIZE_PLANE(vf) __asm__ volatile ( \
    ".set noreorder\n\tvaddw.xyz " #vf ", vf0, vf0w\n\t" \
    "vsubw.w " #vf ", vf0, vf0w\n\t.set reorder")
extern void corePktInitCb(sdfCorePacketCb_t *work, u16 *parameterWords, u32 selectionMask);
extern void corePktSetFaceDivParm(void *out, SdfAssetEntry *entry, u32 packedColor, f32 deltaX, f32 deltaY);
extern void corePktLoadWorldLightMtx(u8 *matrix);
extern void corePktOpenAndEmit(sdfCorePacketCb_t *work);
extern u32 corePktClosePacket(sdfCorePacketCb_t *work);
extern s32 corePktVu0Run(u32 dmaBase, s32 cursor, s32 count, s32 a, s32 b);
extern void corePktSkinPointsFirst(u32 dmaBase, s32 count, s32 vertices, s32 first, s32 second);
extern void corePktSkinPointsAccum(u32 dmaBase, s32 count, s32 vertices, s32 first, s32 second);
extern void corePktSkinVertFirst(u32 dmaBase, s32 count, s32 vertices, s32 first, s32 second);
extern void corePktSkinVertAccum(u32 dmaBase, s32 count, s32 vertices, s32 first, s32 second);
extern void corePktLoadVerts(u32 dmaBase, s32 count, s32 first);
extern void corePktLoadVertsNorms(u32 dmaBase, s32 count, s32 first, s32 second);
extern void corePktMorphVertsNorms(u32 dmaBase, s32 count, s32 first, s32 second, f32 weight);
extern void corePktMorphVerts(u32 dmaBase, s32 count, s32 first, f32 weight);
extern void corePktNormalizeNorms(u32 dmaBase, s32 count);
extern void corePktXfmLightClip(u32 dmaBase, s32 count, s32 positions, s32 normals);
extern void corePktXfmLightClipPB(u32 dmaBase, s32 count);
extern void corePktXfmLightClipUV(u32 dmaBase, s32 count, s32 positions, s32 normals, s32 coordinates, void *transform);
extern void corePktXfmLightClipUVPB(u32 dmaBase, s32 count, s32 coordinates, void *transform);
extern u8 sdf3DScreenTm[];
extern u128 sdfModelColorTransform[3];

#define MODEL_ALIGN16(x) (((x) + 0xF) & ~0xF)

/* VU0 data memory, which the skinning microprogram reads (in quadwords). */
#define MODEL_VU0_MEMORY ((u128 *)0x11004000)

/* Lay out the node coordinates, second coordinates and vertex colours after
 * cursor, as the parameters select, and return the new cursor. */
static s32 sdfModelLayOutTexturing(sdfCorePacketCb_t *work, s32 cursor, u32 nextParam, s32 vertexCount) {
    if (nextParam & 0x100) {
        work->uvPtr = (u8 *)cursor;
        cursor = MODEL_ALIGN16(cursor + vertexCount * 8);
        if (work->orgAttrBits & 0x1000) {
            work->uv2Ptr = (u8 *)cursor;
            cursor = MODEL_ALIGN16(cursor + vertexCount * 8);
        }
    }
    work->vertColPtr = (u8 *)cursor;
    return cursor;
}

/* The strip indices follow the payload when the parameters select them. */
static s32 sdfModelLayOutStrip(sdfCorePacketCb_t *work, s32 cursor, u32 nextParam, s32 stripCount) {
    if (nextParam & 0x20) {
        work->facePtr = (u8 *)cursor;
        cursor = MODEL_ALIGN16(cursor + stripCount * 6);
    }
    return cursor;
}

/* Load the node's frame matrix into vf28..vf31, times the view matrix, and
 * rotate the object basis. */
static void sdfModelLoadNodeMatrix(u8 *matrix) {
    VU0_LOAD_MATRIX(matrix);
    sdfPostmultiplyVuMatrixFromMemory(sdf3DScreenTm);
    corePktLoadWorldLightMtx(matrix);
}

/* Build a kind-3 model command's VU packet for one frame. The resource's
 * kind and parameter word choose how the vertices reach the VU:
 * - kinds 7 and 8 upload matrices and the node's transform to VU0 data
 *   memory for its skinning microprogram (kind 7 with every listed node's
 *   frame workspace, kind 8 with the node's own matrix);
 * - parameter 0x800 blends listed nodes' vertices (with normals for 0x80);
 * - parameter 0x1000 blends a slot pair's two weighted targets;
 * - otherwise the node's own positions (and normals) are used.
 * Then texture coordinates and colours, and the packet is finalized.
 * Upstream: INCLUDE_ASM in DDS1's game/code_002E0820 and DDS2's
 * game/code_003396D0; written from the retail routine and checked path by
 * path. Retail fills each 64-bit transform row's quadword
 * with an ld, leaving the upper half as whatever the register held; the
 * microprogram reads only the row, so this writes zero there. */
u32 sdfCorePacketGen(sdfModelNode_t *node, void *resourceData, s32 nodeMask, s32 color, f32 offsetX,
                                    f32 offsetY, s32 frame, s32 flag) {
    sdfCorePacketCb_t work;
    u8 *resource = (u8 *)resourceData;
    s32 frameOffset = frame << 7;
    u8 *matrix = (u8 *)node->workspace + frameOffset;
    sdfMat_t **assets;
    SdfAssetEntry *asset;
    u32 selected;
    u32 nextParam;
    s32 stripCount;
    s32 vertexCount;
    u32 dmaBase;
    s32 cursor;

    MODEL_VU_INITIALIZE_PLANE(vf1);
    corePktInitCb(&work, (u16 *)(resource + 0x10), nodeMask);
    work.backfaceSign = flag == 0 ? 1.0f : -1.0f;
    work.color = color;
    work.fdSoften = offsetX;
    work.fdBias = offsetY;
    assets = (sdfMat_t **)node->root->resources->buffer;
    asset = assets[*(u16 *)(resource + 6)]->drawEntries[frame];
    work.matPacketPtr = asset;
    stripCount = (s16)work.faceCount; /* lh */
    vertexCount = work.pointCount;
    selected = work.attrBits;
    nextParam = work.packetBits;
    corePktSetFaceDivParm(sdfModelColorTransform, asset, work.color, offsetX, offsetY);
    dmaBase = work.pointBuff;

    if (resource[0] == 7 || resource[0] == 8) {
        u128 *vu = MODEL_VU0_MEMORY;
        s32 transferCount, transferStride;

        if (resource[0] == 7) {
            u16 *list = (u16 *)work.dataPtr;
            u16 listCount = *list++;
            sdfModelNode_t **nodes = (sdfModelNode_t **)node->root->list->buffer;
            s32 i, k;

            cursor = MODEL_ALIGN16((s32)(list + listCount));
            work.facePtr = (u8 *)cursor;
            cursor = MODEL_ALIGN16(cursor + stripCount * 6);
            for (i = 0; i < 4; i++) {
                vu[0xDC + i] = ((u128 *)sdf3DScreenTm)[i];
            }
            for (i = 0; i < 3; i++) {
                vu[0xE0 + i] = ((u128 *)corePktCurrLight)[1 + i];
            }
            for (i = 0; i < 3; i++) {
                vu[0xE6 + i] = asset->transforms[0].words[i];
            }
            for (i = 0; i < listCount; i++) {
                u128 *source = (u128 *)((u8 *)nodes[list[i]]->workspace + frameOffset);
                for (k = 0; k < 7; k++) {
                    vu[0xC0 + i * 7 + k] = source[k];
                }
            }
            for (i = 0; i < 3; i++) {
                vu[0xE3 + i] = sdfModelColorTransform[i];
            }
            transferCount = listCount * 12 + 20;
            transferStride = listCount * 28 + 8;
        } else {
            s32 i;

            sdfModelLoadNodeMatrix(matrix);
            VU0_STORE_VF(vf28, &work.tmSave[0]);
            VU0_STORE_VF_AT_UNCLOBBERED(vf29, 16, &work.tmSave[0]);
            VU0_STORE_VF_AT_UNCLOBBERED(vf30, 32, &work.tmSave[0]);
            VU0_STORE_VF_AT_UNCLOBBERED(vf31, 48, &work.tmSave[0]);
            cursor = MODEL_ALIGN16((s32)work.dataPtr);
            work.facePtr = (u8 *)cursor;
            cursor = MODEL_ALIGN16(cursor + stripCount * 6);
            for (i = 0; i < 3; i++) {
                vu[0xE6 + i] = asset->transforms[0].words[i];
            }
            for (i = 0; i < 3; i++) {
                vu[0xE3 + i] = sdfModelColorTransform[i];
            }
            transferCount = 28;
            transferStride = 32;
        }
        cursor = corePktVu0Run(dmaBase, cursor, vertexCount, transferCount, transferStride);
        if (work.orgAttrBits & 0x1000) {
            work.uv2Ptr = (u8 *)cursor;
            cursor = MODEL_ALIGN16(cursor + vertexCount * 8);
        }
        work.vertColPtr = (u8 *)cursor;
        corePktOpenAndEmit(&work);
    } else if (nextParam & 0x800) {
        u16 *list = (u16 *)work.dataPtr;
        u16 listCount = *list++;
        sdfModelNode_t **nodes = (sdfModelNode_t **)node->root->list->buffer;
        s32 i;

        cursor = sdfModelLayOutStrip(&work, MODEL_ALIGN16((s32)(list + listCount)), nextParam, stripCount);
        for (i = 0; i < listCount; i++) {
            s32 positions = cursor;
            s32 normals = MODEL_ALIGN16(positions + vertexCount * 16);
            s32 vertices = (s32)((u8 *)nodes[list[i]]->workspace + frameOffset);

            cursor = normals;
            if (nextParam & 0x80) {
                cursor = MODEL_ALIGN16(normals + vertexCount * 12);
                if (i == 0) {
                    corePktSkinPointsFirst(dmaBase, vertexCount, vertices, positions, normals);
                } else {
                    corePktSkinPointsAccum(dmaBase, vertexCount, vertices, positions, normals);
                }
            } else if (i == 0) {
                corePktSkinVertFirst(dmaBase, vertexCount, vertices, positions, normals);
            } else {
                corePktSkinVertAccum(dmaBase, vertexCount, vertices, positions, normals);
            }
        }
        if (nextParam & 0x80) {
            corePktNormalizeNorms(dmaBase, vertexCount);
        }
        sdfModelLayOutTexturing(&work, cursor, nextParam, vertexCount);
        VU0_LOAD_MATRIX(sdf3DScreenTm);
        VU0_LOAD_VF_AT(vf24, 16, (u128 *)corePktCurrLight);
        VU0_LOAD_VF_AT(vf25, 32, (u128 *)corePktCurrLight);
        VU0_LOAD_VF_AT(vf26, 48, (u128 *)corePktCurrLight);
        if (selected & 0x10) {
            corePktXfmLightClipUVPB(dmaBase, vertexCount, (s32)work.uvPtr, (u8 *)asset + 0x68);
        } else {
            corePktXfmLightClipPB(dmaBase, vertexCount);
        }
        corePktOpenAndEmit(&work);
    } else if (nextParam & 0x1000) {
        u32 targets = *(u32 *)(resource + 8);
        s32 slot = targets >> 16;
        s32 targetCount = targets & 0xFFFF;
        DevRequest *slots = node->root->slotPairs;
        u8 *pair;
        s32 stride;
        s32 i;

        cursor = MODEL_ALIGN16((s32)work.dataPtr);
        if (slots == NULL) {
            return 0;
        }
        cursor = sdfModelLayOutStrip(&work, cursor, nextParam, stripCount);
        if (slot >= slots->usedCount) {
            return 0;
        }
        pair = (u8 *)slots->buffer + slot * 16; /* two (target index, weight) entries */
        stride = MODEL_ALIGN16(vertexCount * 12);
        if (nextParam & 0x80) {
            corePktLoadVertsNorms(dmaBase, vertexCount, cursor, MODEL_ALIGN16(cursor + vertexCount * 12));
            stride *= 2;
        } else {
            corePktLoadVerts(dmaBase, vertexCount, cursor);
        }
        for (i = 0; i < 2; i++) {
            s32 index = ((s32 *)pair)[i * 2];
            f32 weight = ((f32 *)pair)[i * 2 + 1];

            if (weight > 0.000001f && index < targetCount) {
                s32 target = cursor + index * stride + stride;

                if (nextParam & 0x80) {
                    corePktMorphVertsNorms(dmaBase, vertexCount, target, MODEL_ALIGN16(target + vertexCount * 12), weight);
                } else {
                    corePktMorphVerts(dmaBase, vertexCount, target, weight);
                }
            }
        }
        cursor += stride * targetCount;
        if (nextParam & 0x80) {
            corePktNormalizeNorms(dmaBase, vertexCount);
        }
        sdfModelLayOutTexturing(&work, cursor, nextParam, vertexCount);
        sdfModelLoadNodeMatrix(matrix);
        if (selected & 0x10) {
            corePktXfmLightClipUVPB(dmaBase, vertexCount, (s32)work.uvPtr, (u8 *)asset + 0x68);
        } else {
            corePktXfmLightClipPB(dmaBase, vertexCount);
        }
        corePktOpenAndEmit(&work);
    } else {
        s32 positions = sdfModelLayOutStrip(&work, MODEL_ALIGN16((s32)work.dataPtr), nextParam, stripCount);

        cursor = MODEL_ALIGN16(positions + vertexCount * 12);
        if (nextParam & 0x80) {
            work.normalPtr = (u8 *)cursor;
            cursor = MODEL_ALIGN16(cursor + vertexCount * 12);
        }
        sdfModelLayOutTexturing(&work, cursor, nextParam, vertexCount);
        sdfModelLoadNodeMatrix(matrix);
        if (selected & 0x10) {
            corePktXfmLightClipUV(dmaBase, vertexCount, positions, (s32)work.normalPtr, (s32)work.uvPtr,
                          (u8 *)asset + 0x68);
        } else {
            corePktXfmLightClip(dmaBase, vertexCount, positions, (s32)work.normalPtr);
        }
        corePktOpenAndEmit(&work);
    }
    return corePktClosePacket(&work);
}
#else
INCLUDE_ASM(const s32, "sdf/sdfCoreDraw", sdfCorePacketGen);
#endif

void sdfCoreDrawScanOneSkinNodes(sdfModelRootNode_t *model, SdfObjectRefCommand *source) {
    sdfModelNode_t **objects = model->list->buffer;
    u16 *indices = &source->count;
    if ((source->flags & 0x800) != 0) {
        s32 count = *indices;
        if (count != 0) {
            indices++;
            do {
                sdfModelAllocTmPacket(objects[*indices++]);
            } while (--count != 0);
        }
    }
}

void sdfCoreDrawSetLight(const struct SdfDmaTagHeader *lightingPacket) {
    corePktCurrLight = lightingPacket;
}

extern u8 D_00476240[];

/* Cache the object's vector only when its address changes, not its contents. */
void sdfCoreDrawSetFog(const SdfLightingPacketStorage *sourcePacket) {
    if (D_0043918C != (u32)sourcePacket) {
        D_0043918C = (u32)sourcePacket;
        SDF_VU0_COPYVECTOR(D_00476240, sourcePacket->normal[0]);
    }
}

void sdfCoreDrawSetZBias(f32 scale) {
    D_00439190 = scale;
}

void sdfVuClearTransformCache(void) {
    corePktCurrLight = 0;
    D_0043918C = 0;
}

/* Upload the VIF0 program synchronously, then replace the ring workspace. */
void sdfCoreDrawInit(s32 workspaceBytes) {
    u32 *dmaChannel = sceDmaGetChan(0);
    HW_WRITE(dmaChannel, HW_READ(dmaChannel) & ~SDF_DMA_CHCR_TTE);
    sceDmaSendN(dmaChannel, D_0037B080, (D_0037B610 - D_0037B080) >> SDF_DMA_QWORD_SHIFT);
    sceDmaSync(dmaChannel, 0, 0);
    sdfMemFreeHandleDirect(&D_00438A40);
    D_00438A40 = sdfMemAllocDirect(workspaceBytes);
    D_00439180 = sdfMemLock(D_00438A40);
}
INCLUDE_SDATA(const s32, "sdf/sdfCoreDraw", D_00438A40);

INCLUDE_SDATA(const s32, "sdf/sdfCoreDraw", sdfFaceDivSoften);

INCLUDE_SDATA(const s32, "sdf/sdfCoreDraw", sdfFaceDivBias);

