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

extern void sdfWaitVblank(s32 requestedSleepCount);

#define SDF_DMA_TAG_NEXT_BYTE 0x20

#define SDF_DMA_TAG_REF_BYTE 0x30

#define SDF_DMA_TAG_CALL_BYTE 0x50

#define SDF_DMA_TAG_END_BYTE 0x70

#define SDF_DMA_QWC_MASK 0xFFFF

#define SDF_DMA_ADDRESS_MASK 0x0FFFFFFF

#define SDF_QWORD_BYTES 0x10

#define SDF_DMA_EXTENDED_TAIL_OFFSET 0x30

#define SDF_DMA_REFERENCE_NODE_BYTES 0x20

#define SDF_DMA_TAG_REF_WORD 0x30000000

#define SDF_DMA_TAG_NEXT_WORD 0x20000000

#define SDF_VIF_DIRECT_WORD 0x50000000

#define SDF_VIF_FLUSHE_WORD 0x10000000

#define SDF_GIF_ONE_REGISTER_WORD 0x10000000

#define SDF_GIF_EOP_BIT 0x8000

#define SDF_DMA_CNT_ONE_WORD 0x10000001

#define SDF_VIF_DIRECT_ONE_FLUSHE 0x5000000110000000ULL

#define SDF_GIF_PACKED_AD_BITS 0x1000000000008000ULL

#define SDF_VIF_DIRECT_ONE_NOP 0x5000000100000000ULL

#define SDF_GS_CENTER_BIAS 0x1000

#define SDF_GS_FIELD_OFFSET_STEP 8

#define SDF_GS_DEFAULT_TEXA 0x4000000080ULL

#define SDF_GS_DEFAULT_ALPHA 0x44

#define SDF_GS_SCENE_TEST 0x517FB

#define SDF_SCENE_DRAW_PAYLOAD_QWORDS 5

#define SDF_TEXTURE_SCENE_PAYLOAD_QWORDS 0x15

#define SDF_GRAPH_DEPTH_BUFFER_INDEX 2

extern u16 D_003BD334;

extern s32 D_003BD340;

extern s8 sdfDrawImageTrans;

extern vu8 sdfFrameId;

extern sdfGrRefPacketHeader_t *sdfGrNewExPacket(SdfDmaTag *sourceTag);

extern volatile s8 sdfReqDrawDelayCount;

extern s32 sdfDrawReqQueueIndex;

extern SdfPacketSlot sdfGrDrawState[];

/* Hand the oldest pending slot to the worker, shift the slot list down and * refill the last slot with the newly detached queue. */ void sdfDelayedResrcProc(void);

s32 sdfGrAllocPacketBuff(s32 size);

void sdfDrawTagInit(sdfDrawTag_t *list) {
    list->unkC = 0xFFFF;
    list->next = NULL;
    list->head = 0;
    list->tail = 0;
    list->firstReferenceSource = 0;
    list->secondReferenceSource = 0;
    list->unk18 = 0;
    list->unk1C = 0;
}

/* Chain a DMA packet by rewriting the previous packet's tag to NEXT. */
void sdfDrawTagLink(sdfDrawTag_t *list, u32 packet) {
    s32 last;

    last = list->tail;
    if (last == 0) {
        list->head = packet;
    }
    else {
        ((SdfDmaTag *)last)->kind = SDF_DMA_TAG_NEXT_BYTE;
        ((SdfDmaTag *)last)->address = packet & SDF_DMA_ADDRESS_MASK;
    }
    list->tail = packet;
}

/* Append a DMA range, keeping its start and trailing tag addresses distinct. */
void sdfAppendPacketRange(sdfDrawTag_t *list, u32 packetAddress, u32 rangeTailAddress) {
    s32 previousTailAddress;

    previousTailAddress = list->tail;
    if (previousTailAddress == 0) {
        list->head = packetAddress;
    }
    else {
        ((SdfDmaTag *)previousTailAddress)->kind = SDF_DMA_TAG_NEXT_BYTE;
        ((SdfDmaTag *)previousTailAddress)->address = packetAddress & SDF_DMA_ADDRESS_MASK;
    }
    list->tail = rangeTailAddress;
}

/* REF packets carry an extra quadword after the DMA tag. */
void sdfDrawTagLinkRef(sdfDrawTag_t *list, u32 packetAddress) {
    s32 previousTailAddress;

    ((SdfDmaTag *)packetAddress)->kind = SDF_DMA_TAG_REF_BYTE;
    previousTailAddress = list->tail;
    if (previousTailAddress == 0) {
        list->head = packetAddress;
    }
    else {
        ((SdfDmaTag *)previousTailAddress)->kind = SDF_DMA_TAG_NEXT_BYTE;
        ((SdfDmaTag *)previousTailAddress)->address = packetAddress & SDF_DMA_ADDRESS_MASK;
    }
    list->tail = packetAddress + SDF_QWORD_BYTES;
}

/* Append a DMA packet whose trailing tag is three quadwords after its start. */
void sdfDrawTagLinkRefGif(sdfDrawTag_t *list, u32 packetAddress) {
    s32 previousTailAddress;

    previousTailAddress = list->tail;
    if (previousTailAddress == 0) {
        list->head = packetAddress;
    }
    else {
        ((SdfDmaTag *)previousTailAddress)->kind = SDF_DMA_TAG_NEXT_BYTE;
        ((SdfDmaTag *)previousTailAddress)->address = packetAddress & SDF_DMA_ADDRESS_MASK;
    }
    list->tail = packetAddress + SDF_DMA_EXTENDED_TAIL_OFFSET;
}

/* CALL packets likewise end one quadword after their tag. */
void sdfDrawTagLinkCall(sdfDrawTag_t *list, u32 packetAddress) {
    s32 previousTailAddress;

    ((SdfDmaTag *)packetAddress)->kind = SDF_DMA_TAG_CALL_BYTE;
    previousTailAddress = list->tail;
    if (previousTailAddress == 0) {
        list->head = packetAddress;
    }
    else {
        ((SdfDmaTag *)previousTailAddress)->kind = SDF_DMA_TAG_NEXT_BYTE;
        ((SdfDmaTag *)previousTailAddress)->address = packetAddress & SDF_DMA_ADDRESS_MASK;
    }
    list->tail = packetAddress + SDF_QWORD_BYTES;
}

/* Prepend a nonempty packet list, connecting its DMA tail to the former first list. */
void sdfGrOtFuncLinkToHead(sdfGrOt_t *destinationList, sdfDrawTag_t *incomingList) {
    sdfDrawTag_t *firstList;

    if (incomingList->tail == 0) {
        return;
    }
    firstList = destinationList->tagHead;
    if (firstList == NULL) {
        destinationList->tagTail = incomingList;
    } else {
        sdfGrJoinDrawTag(incomingList, firstList);
    }
    incomingList->next = firstList;
    destinationList->tagHead = incomingList;
}

void sdfGrOtNormalReg(sdfGrOt_t *list, sdfDrawTag_t *item) {
    sdfDrawTag_t *last;

    if (item->head != 0) {
        last = list->tagTail;
        if (last == NULL) {
            list->tagHead = item;
        }
        else {
            last->next = item;
            sdfGrJoinDrawTag(last, item);
        }
        list->tagTail = item;
    }
}

s32 sdfGrOtNormalCallback(sdfGrOt_t *list, s32 mode, sdfDrawTag_t *packet) {
    if (mode == 1) {
        sdfGrOtFuncLinkToHead(list, packet);
    }
}

extern void sdfGrOtInit(sdfGrOt_t *, s32);

/* Initialize and link count pool nodes, terminating the last; count must be at least one. */
void sdfGrOtInit(sdfGrOt_t *nodes, s32 count) {
    sdfGrOt_t *node = nodes;

    for (;;) {
        node->append = sdfGrOtNormalReg;
        node->prepend = sdfGrOtNormalCallback;
        node->tagHead = 0;
        node->tagTail = 0;
        node->unkC = 0;
        node->unk18 = 0;
        if (--count == 0) {
            node->next = NULL;
            break;
        }
        node->next = node + 1;
        node++;
    }
}

/* Make a REF DMA node for the payload following the source tag. */
sdfGrRefPacketHeader_t *sdfGrNewExPacket(SdfDmaTag *sourceTag) {
    sdfGrRefPacketHeader_t *referenceNode = (sdfGrRefPacketHeader_t *)sdfGrAllocPacketBuff(SDF_DMA_REFERENCE_NODE_BYTES);
    u64 dmaHeader = sourceTag->quadwordCount;
    u32 payloadAddress = ((u32)sourceTag + SDF_QWORD_BYTES) & SDF_DMA_ADDRESS_MASK;
    s64 packedAddress = (s64)payloadAddress << 32;

    dmaHeader |= SDF_DMA_TAG_REF_WORD;
    dmaHeader |= packedAddress;
    referenceNode->dmaTag = dmaHeader;
    referenceNode->nextTag = 0;
    referenceNode->vifCommands = sourceTag->vifCommands;
    return referenceNode;
}

/* Patch the prior NEXT tag to chain in a reference to source's payload. */
SdfDmaTag *sdfGrAddExPacket(SdfDmaTag *previousTag, SdfDmaTag *sourceTag) {
    sdfGrRefPacketHeader_t *referenceNode;

    referenceNode = sdfGrNewExPacket(sourceTag);
    previousTag->kind = SDF_DMA_TAG_NEXT_BYTE;
    previousTag->address = (u32)referenceNode & SDF_DMA_ADDRESS_MASK;
    return (SdfDmaTag *)((u32)referenceNode + SDF_QWORD_BYTES);
}

/* Insert differing nonzero reference sources before linking the incoming DMA chain.
 * Zero incoming sources inherit prior state; the locals cover both source slots. */
void sdfGrJoinDrawTag(sdfDrawTag_t *previousList, sdfDrawTag_t *incomingList) {
    SdfDmaTag *tailTag = (SdfDmaTag *)previousList->tail;
    u32 previousSource;
    u32 incomingSource;

    previousSource = previousList->firstReferenceSource;
    incomingSource = incomingList->firstReferenceSource;
    if (previousSource != incomingSource) {
        if (incomingSource != 0) {
            tailTag = sdfGrAddExPacket(tailTag, (SdfDmaTag *)incomingSource);
        } else {
            incomingList->firstReferenceSource = previousSource;
        }
    }
    previousSource = previousList->secondReferenceSource;
    incomingSource = incomingList->secondReferenceSource;
    if (previousSource != incomingSource) {
        if (incomingSource != 0) {
            tailTag = sdfGrAddExPacket(tailTag, (SdfDmaTag *)incomingSource);
        } else {
            incomingList->secondReferenceSource = previousSource;
        }
    }
    tailTag->kind = SDF_DMA_TAG_NEXT_BYTE;
    tailTag->address = incomingList->head & SDF_DMA_ADDRESS_MASK;
}

/* Prepend the second reference, then the first: first, second, payload.
 * Patch only the NEXT header's low doubleword; its VIF half stays zero. */
void sdfGrInitExPacket(sdfDrawTag_t *list) {
    sdfGrRefPacketHeader_t *referenceNode;
    u32 priorHeadAddress;
    u32 sourceTagAddress;

    sourceTagAddress = list->secondReferenceSource;
    if (sourceTagAddress != 0) {
        referenceNode = sdfGrNewExPacket((SdfDmaTag *)sourceTagAddress);
        priorHeadAddress = list->head & SDF_DMA_ADDRESS_MASK;
        list->head = (u32)referenceNode;
        *(u64 *)&referenceNode->nextTag = ((s64)priorHeadAddress << 32) | SDF_DMA_TAG_NEXT_WORD;
    }
    sourceTagAddress = list->firstReferenceSource;
    if (sourceTagAddress != 0) {
        referenceNode = sdfGrNewExPacket((SdfDmaTag *)sourceTagAddress);
        priorHeadAddress = list->head & SDF_DMA_ADDRESS_MASK;
        list->head = (u32)referenceNode;
        *(u64 *)&referenceNode->nextTag = ((s64)priorHeadAddress << 32) | SDF_DMA_TAG_NEXT_WORD;
    }
}

/* Flush every pool entry, chain the packet lists together and terminate the last. */
sdfDrawTag_t *sdfFlushPoolNodes(sdfGrOt_t *node) {
    sdfDrawTag_t *tail = NULL;
    sdfDrawTag_t *head = NULL;

    for (; node != NULL; node = node->next) {
        node->prepend(node, 0, NULL);
        if (node->tagHead != 0) {
            if (head != 0) {
                sdfGrJoinDrawTag(tail, node->tagHead);
            } else {
                head = node->tagHead;
                sdfGrInitExPacket(head);
            }
            tail = node->tagTail;
        }
    }
    if (tail != NULL) {
        ((SdfDmaTag *)tail->tail)->kind = SDF_DMA_TAG_END_BYTE;
        ((SdfDmaTag *)tail->tail)->address = 0;
    }
    return head;
}

void sdfPreDrawTagInit(sdfPreDrawTag_t *list) {
    list->unk0 = 0;
    list->first = 0;
    list->last = 0;
    list->unkC = 0;
}

void sdfPreDrawTagLink(sdfPreDrawTag_t *list, SdfPacketPatchLink *node) {
    if (list->last == 0) {
        list->first = node;
    }
    else {
        list->last->next = node;
    }
    list->last = node;
    node->next = NULL;
}

void sdfPreDrawChainInit(SdfPacketChain *chain) {
    chain->head = NULL;
    chain->tail = NULL;
}

void sdfPreDrawChainLink(SdfPacketChain *chain, sdfPreDrawTag_t *node) {
    if (chain->tail == NULL) {
        chain->head = node;
    }
    else {
        chain->tail->last->next = node->last;
    }
    chain->tail = node;
}

/* Encode the packed A+D payload count and its DMA/VIF transfer length. */
void sdfGrSetRegsPacketHeader(SdfGsPacketHeader *header, s32 payloadQwords) {
    s64 dmaQwords;

    dmaQwords = payloadQwords + 1;
    header->vifCommands = (((dmaQwords | SDF_VIF_DIRECT_WORD) << 32) | SDF_VIF_FLUSHE_WORD);
    header->gifTag = (payloadQwords | (((s64)SDF_GIF_ONE_REGISTER_WORD << 32) | SDF_GIF_EOP_BIT));
    header->dmaTag = dmaQwords;
    header->gifRegisters = SDF_GIF_REGISTER_AD;
}

/* Emit a GIF header, a REF tag with masked count/address, and the trailing NEXT tag. */
void sdfGrSetRefGifPacket(SdfDmaReferenceChainPacket *packet, u32 sourceAddress, s32 qwordCount) {
    packet->header.dmaTag = SDF_DMA_CNT_ONE_WORD;
    packet->header.vifCommands = SDF_VIF_DIRECT_ONE_FLUSHE;
    packet->header.gifTag = (u64)qwordCount | SDF_GIF_PACKED_AD_BITS;
    packet->header.gifRegisters = SDF_GIF_REGISTER_AD;
    packet->referenceTag = (u32)((qwordCount & SDF_DMA_QWC_MASK) | SDF_DMA_TAG_REF_WORD) | ((u64)(sourceAddress & SDF_DMA_ADDRESS_MASK) << 32);
    packet->referenceVifCommands = SDF_VIF_DIRECT_ONE_NOP;
    packet->nextTag = SDF_DMA_TAG_NEXT_WORD;
    packet->zeroTailWord = 0;
}

/* Emit FRAME/ZBUF/XYOFFSET/SCISSOR A+D pairs for the selected GS context.
 * Register IDs are separate from their values; fieldOffset adds a half-pixel Y step. */
void sdfGrSetDrawFramePacket(SdfGsContextRegisters *packet, s32 frameAddress, s32 width, s32 height,
                  s32 frameFormat, s32 depthAddress, s32 depthFormat,
                  s32 fieldOffset, SdfGsContext context) {
    s64 frameRegisterId;
    s64 depthRegisterId;
    s64 offsetRegisterId;
    s64 scissorRegisterId;
    s32 xOffset;
    s32 yOffset;

    if (context == SDF_GS_CONTEXT_1) {
        frameRegisterId = SDF_GS_FRAME_1;
        depthRegisterId = SDF_GS_ZBUF_1;
        offsetRegisterId = SDF_GS_XYOFFSET_1;
        scissorRegisterId = SDF_GS_SCISSOR_1;
    } else {
        frameRegisterId = SDF_GS_FRAME_2;
        depthRegisterId = SDF_GS_ZBUF_2;
        offsetRegisterId = SDF_GS_XYOFFSET_2;
        scissorRegisterId = SDF_GS_SCISSOR_2;
    }
    packet->scissor.registerId = scissorRegisterId;
    yOffset = (SDF_GS_CENTER_BIAS - height) << 3;
    xOffset = (SDF_GS_CENTER_BIAS - width) << 3;
    packet->frame.value = (s64)(frameFormat << 24) | (s64)(((width + 63) >> 6) << 16) | (s64)(frameAddress >> 11);
    if (fieldOffset != 0) {
        yOffset += SDF_GS_FIELD_OFFSET_STEP;
    }
    packet->frame.registerId = frameRegisterId;
    packet->zbuf.value = (s64)(depthFormat << 24) | (s64)(depthAddress >> 11);
    packet->zbuf.registerId = depthRegisterId;
    packet->xyoffset.registerId = offsetRegisterId;
    packet->scissor.value = ((s64)(height - 1) << 48) | ((s64)(width - 1) << 16);
    packet->xyoffset.value = xOffset | ((s64)yOffset << 32);
}

/* Encode centered viewport bounds in GS coordinate words; the last two arguments are unused. */
void sdfGrSetClearPacket(SdfGsCenteredBoundsRegisters *packet, s32 width, s32 height, s32 unused0, s32 unused1) {
    u32 lowerBounds = ((SDF_GS_CENTER_BIAS - height) << 19) | ((SDF_GS_CENTER_BIAS - width) << 3);
    u32 upperBounds = ((height + SDF_GS_CENTER_BIAS) << 19) | ((width + SDF_GS_CENTER_BIAS) << 3);

    packet->primitive.registerId = SDF_GS_PRIM;
    packet->xyz2[0].registerId = SDF_GS_XYZ2;
    packet->test.value = SDF_GS_CENTERED_VIEW_BOUNDS_TEST;
    packet->test.registerId = SDF_GS_TEST_1;
    packet->primitive.value = SDF_GS_PRIMITIVE_SPRITE;
    packet->rgbaq.value = (u64)0xFE00 << 46;
    packet->rgbaq.registerId = SDF_GS_RGBAQ;
    packet->xyz2[0].value = lowerBounds;
    packet->xyz2[1].value = upperBounds;
    packet->xyz2[1].registerId = SDF_GS_XYZ2;
}

/* Seed PRMODECONT, COLCLAMP, DTHE and TEXA drawing registers. */
void sdfGrSetDrawDefParmPacket(SdfGsDrawDefaultsRegisters *packet) {
    packet->prmodecont.value = 1;
    packet->prmodecont.registerId = SDF_GS_PRMODECONT;
    packet->colclamp.value = 1;
    packet->colclamp.registerId = SDF_GS_COLCLAMP;
    packet->dthe.value = 0;
    packet->dthe.registerId = SDF_GS_DTHE;
    packet->texa.value = SDF_GS_DEFAULT_TEXA;
    packet->texa.registerId = SDF_GS_TEXA;
}

/* Build the common header and FRAME/ZBUF/XYOFFSET/SCISSOR state for one GS context. */
void sdfGrSetDrawFrame(SdfGsContextPacket *packet, s32 frameAddress, s32 width, s32 height,
                           s32 frameFormat, s32 depthAddress, s32 depthFormat, SdfGsContext context) {
    sdfGrSetRegsPacketHeader(&packet->header, SDF_SCENE_DRAW_PAYLOAD_QWORDS);
    sdfGrSetDrawFramePacket(&packet->registers, frameAddress, width, height, frameFormat, depthAddress, depthFormat, 0, context);
}

extern u8 sdfGrBufferSlotIndex;

/* Build both GS drawing contexts from the selected frame buffer and shared depth buffer. */
void sdfGrSetDrawEnv(SdfSceneDrawPacket *packet, sdfGrFrameSet_t *view, s32 bufferIndex) {
    s32 frameAddress;
    s32 depthAddress;
    s32 width;
    s32 height;
    s32 frameFormat;
    s32 depthFormat;

    sdfGrSetRegsPacketHeader(&packet->header, SDF_TEXTURE_SCENE_PAYLOAD_QWORDS);
    frameAddress = view->buffers[bufferIndex]->word;
    depthAddress = view->buffers[SDF_GRAPH_DEPTH_BUFFER_INDEX]->word;
    width = view->width;
    frameFormat = view->bufferFormat;
    height = view->height;
    depthFormat = view->auxiliaryFormat;
    sdfGrSetDrawFramePacket(&packet->contextOne, frameAddress, width, height, frameFormat, depthAddress, depthFormat, sdfGrBufferSlotIndex, SDF_GS_CONTEXT_1);
    sdfGrSetDrawFramePacket(&packet->contextTwo, frameAddress, width, height, frameFormat, depthAddress, depthFormat, sdfGrBufferSlotIndex, SDF_GS_CONTEXT_2);
    sdfGrSetClearPacket(&packet->centeredBounds, view->width, view->height, view->bufferFormat, view->auxiliaryFormat);
    packet->blendState.contextOne.test.value = SDF_GS_SCENE_TEST;
    packet->blendState.contextOne.test.registerId = SDF_GS_TEST_1;
    packet->blendState.contextOne.alpha.value = SDF_GS_DEFAULT_ALPHA;
    packet->blendState.contextOne.alpha.registerId = SDF_GS_ALPHA_1;
    packet->blendState.contextTwo.test.value = SDF_GS_SCENE_TEST;
    packet->blendState.contextTwo.test.registerId = SDF_GS_TEST_2;
    packet->blendState.contextTwo.alpha.value = SDF_GS_DEFAULT_ALPHA;
    packet->blendState.contextTwo.alpha.registerId = SDF_GS_ALPHA_2;
    sdfGrSetDrawDefParmPacket(&packet->drawDefaults);
}

#ifdef NON_MATCHING
#include "common.h"
#include "sdf.h"
#include "sdf_gs_packet.h"
#include "sdf_scene_packet.h"
#include "sdf_tex_packet.h"

extern u8 sdfGrBufferSlotIndex;

#define SDF_GRAPH_DEPTH_BUFFER 2 /* the view's buffers: two colour, then depth */
#define SDF_FRAME_MASK_ALPHA 0x00FFFFFF00000000LL /* FBMSK: keep destination alpha */

/* Point a scene node's packets at the view's colour buffer for this frame:
 * both contexts' frame, depth and scissor; FRAME_1 and FRAME_2 writing colour
 * only; and textures of this colour buffer and the other one. Upstream:
 * INCLUDE_ASM in both games; written from the retail routine. It
 * differs from retail in four words of scheduling; the calls, arguments and
 * stores are the same. */
void sdfGrStdDrawEnvPredraw(SdfSceneNode *node, s32 bufferIndex) {
    sdfGrFrameSet_t *view = node->view;
    s32 frameAddress = view->buffers[bufferIndex]->word;
    s16 width = view->width;
    s32 depthAddress = view->buffers[SDF_GRAPH_DEPTH_BUFFER]->word;
    s32 frameFormat = view->bufferFormat;
    s32 height = view->height;
    s32 depthFormat = view->auxiliaryFormat;
    s64 frame;

    sdfGrSetDrawFramePacket(&node->contextOne, frameAddress, width, height, frameFormat, depthAddress,
                                    depthFormat, sdfGrBufferSlotIndex, SDF_GS_CONTEXT_1);
    sdfGrSetDrawFramePacket(&node->contextTwo, frameAddress, width, height, frameFormat, depthAddress,
                                    depthFormat, sdfGrBufferSlotIndex, SDF_GS_CONTEXT_2);
    /* FRAME: FBP in 2048-word pages, FBW in 64-pixel units, PSM. */
    frame = (s64)(frameFormat << 24) | (s64)(frameAddress >> 11) | (s64)((width >> 6) << 16) |
            SDF_FRAME_MASK_ALPHA;
    node->frameContextOne.registerId = SDF_GS_FRAME_1;
    node->frameContextOne.value = frame;
    node->frameContextTwo.registerId = SDF_GS_FRAME_2;
    node->frameContextTwo.value = frame;
    if (frameFormat == 0) {
        frameFormat = 1; /* a PSMCT32 buffer is sampled as PSMCT24 */
    }
    sdfTexSetPacket(&node->texturePackets[1], width, height, frameAddress, frameFormat, 0, 0, 1, 0, 0, 5,
                               SDF_GS_CONTEXT_1);
    sdfTexSetPacket(&node->texturePackets[0], width, height, view->buffers[bufferIndex ^ 1]->word,
                               frameFormat, 0, 0, 1, 0, 0, 5, SDF_GS_CONTEXT_1);
}
#else
INCLUDE_ASM(const s32, "sdf/sdfDraw", sdfGrStdDrawEnvPredraw);
#endif

/* Retain the render-target view and initialize the scene callback and fixed drawing state. */
void sdfGrSetStdDrawEnv(SdfSceneNode *node, sdfGrFrameSet_t *view) {
    sdfGrSetRegsPacketHeader(&node->header, SDF_TEXTURE_SCENE_PAYLOAD_QWORDS);
    node->view = view;
    node->link.patch = sdfGrStdDrawEnvPredraw;
    sdfGrSetClearPacket(&node->centeredBounds, view->width, view->height, view->bufferFormat, view->auxiliaryFormat);
    node->blendState.contextOne.test.value = SDF_GS_SCENE_TEST;
    node->blendState.contextOne.test.registerId = SDF_GS_TEST_1;
    node->blendState.contextOne.alpha.value = SDF_GS_DEFAULT_ALPHA;
    node->blendState.contextOne.alpha.registerId = SDF_GS_ALPHA_1;
    node->blendState.contextTwo.test.value = SDF_GS_SCENE_TEST;
    node->blendState.contextTwo.test.registerId = SDF_GS_TEST_2;
    node->blendState.contextTwo.alpha.value = SDF_GS_DEFAULT_ALPHA;
    node->blendState.contextTwo.alpha.registerId = SDF_GS_ALPHA_2;
    sdfGrSetDrawDefParmPacket(&node->drawDefaults);
}

/* Link the metadata node separately from the DMA payload one quadword later. */
void sdfGrLinkStdDrawEnv(sdfDrawTag_t *dmaList, sdfPreDrawTag_t *linkedList, SdfPacketPatchLink *linkedNode) {
    sdfPreDrawTagLink(linkedList, linkedNode);
    sdfDrawTagLink(dmaList, (s32)linkedNode + SDF_QWORD_BYTES);
}

/* Reference the masked FRAME write (FBMSK 0x00FFFFFF); zero selects GS context one, otherwise two. */
void sdfGrSetFrameMaskRGB(const u8 *source, SdfDmaReferenceChainPacket *packet, s32 gsContext) {
    if (gsContext == 0) {
        sdfGrSetRefGifPacket(packet, (u32)(source + 0x180), 1);
        return;
    }
    sdfGrSetRefGifPacket(packet, (u32)(source + 400), 1);
}

/* Reference the unmasked FRAME write; zero selects GS context one, otherwise two. */
void sdfGrSetFrameMaskReset(const u8 *source, SdfDmaReferenceChainPacket *packet, s32 gsContext) {
    if (gsContext == 0) {
        sdfGrSetRefGifPacket(packet, (u32)(source + 0x70), 1);
        return;
    }
    sdfGrSetRefGifPacket(packet, (u32)(source + 0xb0), 1);
}

void sdfGrLinkDispFrameTex(sdfDrawTag_t *list, u32 source, sdfGrRefPacketHeader_t *node) {
    node->vifCommands = (((u64)0x50000004 << 16) | 0x1000) << 16;
    node->dmaTag = ((u64)((source + 0x1a0) & 0xfffffff) << 32) | 0x30000004;
    node->nextTag = 0;
    sdfDrawTagLinkRef(list, (u32)node);
}

void sdfGrLinkDrawFrameTex(sdfDrawTag_t *list, u32 source, sdfGrRefPacketHeader_t *node) {
    node->vifCommands = (((u64)0x50000004 << 16) | 0x1000) << 16;
    node->dmaTag = ((u64)((source + 0x1e0) & 0xfffffff) << 32) | 0x30000004;
    node->nextTag = 0;
    sdfDrawTagLinkRef(list, (u32)node);
}

void sdfGrFramePacketPredraw(SdfPacketBuilder *packet, s32 bufferIndex) {
    sdfGrFrameSet_t *view = packet->source;
    u64 frameRegister;
    u64 depthRegister;
    s16 widthPages;

    if (packet->mode == 0) {
        frameRegister = 0x4C;
        depthRegister = 0x4E;
    } else {
        frameRegister = 0x4D;
        depthRegister = 0x4F;
    }
    widthPages = (view->width + 63) >> 6;
    packet->frame.value = ((u64)packet->frameMask << 32) |
        (u64)(view->buffers[bufferIndex]->word >> 11) |
        ((s64)(view->bufferFormat << 24) | ((s64)widthPages << 16));
    packet->frame.registerId = frameRegister;
    packet->zbuf.value = (s64)(view->auxiliaryFormat << 24) |
        (u64)(view->buffers[2]->word >> 11);
    packet->zbuf.registerId = depthRegister;
}

void sdfGrSetFramePacket(SdfPacketBuilder *packet, sdfGrFrameSet_t *source, u32 frameMask, s32 region, s32 mode) {
    sdfGrSetRegsPacketHeader(&packet->packetHeader, 2);
    packet->mode = mode;
    packet->source = source;
    packet->frameMask = frameMask;
    packet->region = region;
    packet->prepare = sdfGrFramePacketPredraw;
}

void sdfGrReqDraw(sdfDrawTag_t *list, SdfPacketChain *chain) {
    u32 start = sdfGetRunTimeCountBase();
    s32 interrupts;
    s32 index;
    SdfPacketSlot *slot;

    while (sdfReqDrawDelayCount != 0) {
    }
    D_003BD334 = sdfGetRunTimeCount(start);
    interrupts = DIntr();
    index = sdfDrawReqQueueIndex + 1;
    if (index == 3) {
        index = 0;
    }
    slot = &sdfGrDrawState[index];
    slot->packetList = NULL;
    slot->callbackHead = NULL;
    slot->slotUpdatePhase = 0;
    if (list != NULL && list->tail != 0) {
        slot->packetList = list;
        if (chain != NULL) {
            slot->callbackHead = chain->head->first;
            if (slot->callbackHead != NULL) {
                chain->tail->last->next = NULL;
            }
        }
        slot->queuedBufferIndex = sdfFrameId;
    }
    sdfDrawReqQueueIndex = index;
    sdfDrawImageTrans = 1;
    D_003BD340++;
    if (interrupts != 0) {
        EIntr();
    }
}

void sdfGrReqDrawDelay(s32 value) {
    sdfReqDrawDelayCount = (value < 0) ? 0 : value;
}

void sdfGrInitDraw(void) {
    sdfReqDrawDelayCount = 0;
    sdfGrDrawState[0].packetList = NULL;
    sdfDrawReqQueueIndex = 0;
}

#ifdef NON_MATCHING
#include "common.h"
#include "eeregs.h"
#include "sdf.h"
#include "sdf_packet_patch.h"
#include "sdf_packet_slot.h"
#include "sdk_libkernl.h"
#include "sdk_libdma.h"

extern SdfPacketSlot sdfGrDrawState[];
extern s32 sdfDrawReqQueueIndex;
extern s32 dds3GsThreadId;
extern u8 sdfGrDrawFirstFlag;
extern u8 sdfFrameEvenOdd;
extern s8 sdfGrFrameComplete;
extern u8 sdfGrDrawFrameId;
extern u8 sdfGrBufferSlotIndex;
extern u32 sdfGrDrawFrameTagHead;
extern s8 sdfDrawImageTrans;
extern volatile u8 sdfDispThreadFlag;
extern volatile u8 sdfDrawFrameFlag;

extern void sdfDeliverDrawStart(s32 oldBuffer, s32 nextBuffer);
extern void sdfDeliverDrawEnd(s32 updateSingleSlot, s32 bufferIndex, s32 slotIndex);
extern void sdfImageSetTrans(void);
extern void sdfDevConsDrawAll(void);
extern void sdfDelayedResrcProc(void);
extern void sceDmaRecvN(void *channel, void *destination, s32 quadwords);
extern u64 sceGsGetIMR(void);
extern u64 sceGsPutIMR(u64 imr);

#define CHCR_STR 0x100
#define CHCR_TIE_TTE 0xC0
#define GS_CSR_FINISH 2
#define GS_IMR_FINISH_MASK 0x200

/* The data the chain points at when it pauses for a readback: a transfer of
 * GS memory into EE RAM, then an optional end call before the chain resumes.
 * The end function is a code address held in a word. */
typedef struct SdfGsReadback {
    s32 quadwordCount;
    u32 destination;
    u32 endFunction;
    u32 endArgument;
} SdfGsReadback;

/* The chain paused at an interrupting tag. Let the GS finish drawing, then
 * read the requested GS memory into EE RAM over VIF1 and resume the chain
 * where it paused. Returns 0 if the worker is told to stop. */
static s32 sdfServeGsReadback(void *channel) {
    u32 imr = (u32)sceGsGetIMR();
    SdfGsReadback *readback;
    u32 resume;
    u32 destination;
    s32 remaining;
    s32 count;

    sceGsPutIMR(imr | GS_IMR_FINISH_MASK);
    HW_WRITE(GS_CSR, GS_CSR_FINISH);
    HW_WRITE(D1_CHCR, HW_READ(D1_CHCR) | CHCR_STR);
    SleepThread();
    while (!(HW_READ(GS_CSR) & GS_CSR_FINISH)) {
        if (sdfDrawFrameFlag) {
            sceGsPutIMR(imr);
            return 0;
        }
    }
    HW_WRITE(GS_CSR, 0);
    sceGsPutIMR(imr);
    HW_WRITE(D1_CHCR, HW_READ(D1_CHCR) | CHCR_STR);
    SleepThread();

    resume = HW_READ(D1_TADR);
    readback = (SdfGsReadback *)HW_READ(D1_MADR);
    while (HW_READ(VIF1_STAT) & 3) {
        if (sdfDrawFrameFlag) return 0;
    }
    while (HW_READ(GIF_STAT) & 0x80) {
        if (sdfDrawFrameFlag) return 0;
    }
    HW_WRITE(GIF_CTRL, 8);            /* PSE: pause the GIF */
    HW_WRITE(VIF1_STAT, 0x800000);    /* FDR: the VIF1 FIFO reads from the GS */
    HW_WRITE(GS_BUSDIR, 1);
    HW_WRITE(GIF_CTRL, 0);
    remaining = readback->quadwordCount;
    destination = readback->destination;
    while (remaining > 0) {
        count = remaining > 0xF000 ? 0xF000 : remaining;
        sceDmaRecvN(channel, (void *)destination, count);
        remaining -= count;
        SleepThread();
        destination += count * 16;
        if (sdfDrawFrameFlag) break;
    }
    HW_WRITE(VIF1_STAT, 0);
    HW_WRITE(GS_BUSDIR, 0);
    HW_WRITE(D1_CHCR, HW_READ(D1_CHCR) & ~1);
    if (sdfDrawFrameFlag) return 0;
    if (readback->endFunction != 0) {
        ((void (*)(u32, u32))readback->endFunction)(readback->endArgument, resume);
    }
    sceDmaSend(channel, (void *)resume);
    return 1;
}

/* Send the current slot's frame to the GS over VIF1 DMA, serving the GS
 * readbacks its chain requests, then commit the slot's buffers. Upstream:
 * INCLUDE_ASM in both games; same register and call sequence as the
 * retail routine. */
void sdfGrDrawFrame(void) {
    SdfPacketSlot *slot = &sdfGrDrawState[sdfDrawReqQueueIndex];
    struct sdfDrawTag_t *packets = slot->packetList;
    SdfPacketPatchLink *link;
    void *channel;

    if (packets == NULL || slot->slotUpdatePhase == 2) {
        SignalSema(dds3GsThreadId);
    } else {
        sdfGrBufferSlotIndex = (slot->slotUpdatePhase != 0 ? slot->bufferSlotIndex : sdfFrameEvenOdd) ^ 1;
        slot->bufferSlotIndex = sdfGrBufferSlotIndex;
        slot->bufferIndex = sdfGrDrawFrameId;
        sdfDeliverDrawStart(sdfGrDrawFrameId, slot->queuedBufferIndex);
        SignalSema(dds3GsThreadId);
        if (sdfDrawImageTrans != 0) {
            sdfDrawImageTrans = 0;
            sdfImageSetTrans();
        }
        for (link = slot->callbackHead; link != NULL; link = link->next) {
            link->patch(link, slot->bufferIndex);
        }

        FlushCache(0);
        channel = sceDmaGetChan(1);
        HW_WRITE(D1_CHCR, HW_READ(D1_CHCR) | CHCR_TIE_TTE);
        sdfDispThreadFlag = 3;
        sceDmaSend(channel, (void *)(packets->head & 0x0FFFFFFF));
        sdfGrDrawFrameTagHead = packets->head;
        for (;;) {
            SleepThread();
            if (sdfDrawFrameFlag) return;
            /* The chain stops at its end tag, or pauses at an interrupting one. */
            if (((HW_READ(D1_CHCR) >> 16) & 0x7000) == 0x7000) break;
            if (!sdfServeGsReadback(channel)) return;
        }
        while (HW_READ(VIF1_STAT) & 3) {
        }
        while (HW_READ(GIF_STAT) & 0x180) {
        }
        sdfDeliverDrawEnd(slot->slotUpdatePhase, slot->bufferIndex, slot->bufferSlotIndex);
        sdfGrDrawFirstFlag = 1;
        slot->slotUpdatePhase++;
        sdfGrDrawFrameId ^= 1;
    }
    sdfGrFrameComplete = 1;
    if (sdfGrDrawFirstFlag) sdfDevConsDrawAll();
    sdfDelayedResrcProc();
}
#else
INCLUDE_ASM(const s32, "sdf/sdfDraw", sdfGrDrawFrame);
#endif

extern volatile SdfDrawFrameExecutionState D_003BD32D;

/* Wait for frame execution to finish, then sleep until the active slot is empty or ready. */
void sdfDrawSync(void) {
    SdfPacketSlot *table = sdfGrDrawState;

    for (;;) {
        while (D_003BD32D != SDF_DRAW_FRAME_IDLE) {
        }
        if (table[sdfDrawReqQueueIndex].packetList == NULL) {
            break;
        }
        if (table[sdfDrawReqQueueIndex].slotUpdatePhase >= 2) {
            break;
        }
        sdfWaitVblank(0);
    }
}
INCLUDE_SDATA(const s32, "sdf/sdfDraw", sdfDrawFrameFlag);

INCLUDE_SDATA(const s32, "sdf/sdfDraw", D_003BD32D);

INCLUDE_SDATA(const s32, "sdf/sdfDraw", D_003BD32E);

INCLUDE_SDATA(const s32, "sdf/sdfDraw", sdfGrFrameComplete);

INCLUDE_SDATA(const s32, "sdf/sdfDraw", sdfGrDrawFrameId);

INCLUDE_SDATA(const s32, "sdf/sdfDraw", sdfGrBufferSlotIndex);

INCLUDE_SDATA(const s32, "sdf/sdfDraw", sdfReqDrawDelayCount);

INCLUDE_SDATA(const s32, "sdf/sdfDraw", D_003BD334);

INCLUDE_SDATA(const s32, "sdf/sdfDraw", sdfDrawReqQueueIndex);

INCLUDE_SDATA(const s32, "sdf/sdfDraw", sdfGrDrawFrameTagHead);

INCLUDE_SDATA(const s32, "sdf/sdfDraw", D_003BD340);

