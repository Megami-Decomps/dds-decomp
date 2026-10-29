#include "sdk_ps2.h"
#include "common.h"
#include "sdf_vu0.h"
#include "sdf_model_packet_callback.h"
#include "sdf_measure_command.h"
#include "sdf_asset_packets.h"
#include "sdf_dma_tag.h"
#include "sdf_vu_lighting.h"
#include "sdf_chip.h"
#include "sdf_model.h"
#include "sdf_model_eval_api.h"
#include "sdf_material_asset_list.h"
#include "sdf_forced_asset_texture_mode.h"
#include "pcp_vu0.h"
#include "sdf_draw.h"
#include "sdf_model_copy.h"
#include "sdf_vu_matrix.h"
#include "sdf_vu_matrix_multiply_api.h"
#include "sdf_message.h"
#include "sdf_key_pool.h"
#include "sdf_model_command_views.h"
#include "sdf_packet_alloc.h"
#include "sdf_packet_list.h"

/* SdfCommandNode payload forms: owned packet bytes or borrowed commands. */
enum {
    SDF_COMMAND_NODE_KIND_PACKET_DATA = 1,
    SDF_COMMAND_NODE_KIND_SHAPED_COMMANDS = 2,
    SDF_COMMAND_NODE_KIND_CORE_COMMANDS = 3,
};

/* Declared before first use (were implicit int). */
extern void sdfMatListUpdatePacket(DevRequest *list, s32 entryIndex);
extern void sdfResourceListApplyEntryChanges(DevRequest *list, s32 entryIndex);
extern void sdfCoreDrawSetFog(const SdfLightingPacketStorage *sourcePacket);
extern void sdfCoreDrawSetLight(const struct SdfDmaTagHeader *lightingPacket);

void sdfModelMakeLocalTm(sdfModelNode_t *node);
void sdfModelNodeBaseSet(sdfModelNode_t *node, const SdfItem *item);







extern vu8 sdfFrameId;

/* Retail selects by nodeId when SDF_MODEL_FIND_DRAW_NODE_BY_ID is set,
 * otherwise by array index. */
sdfModelNode_t *sdfModelSearchNode(sdfModelRootNode_t *model, s32 nodeIdOrIndex) {
    DevRequest *list = model->list;
    s16 entryCount;
    sdfModelNode_t **entries;
    s32 entryIndex;

    entryCount = list->usedCount;
    entries = list->buffer;
    if (model->flags & SDF_MODEL_FIND_DRAW_NODE_BY_ID) {
        for (entryIndex = 0; entryIndex < entryCount; entryIndex++) {
            sdfModelNode_t *node = entries[entryIndex];
            if (node->nodeId == nodeIdOrIndex) {
                return node;
            }
        }
        return 0;
    }
    if ((u32)nodeIdOrIndex >= (u32)entryCount) {
        return 0;
    }
    return entries[nodeIdOrIndex];
}

/* Append a DMA REF for eight quadwords and a VIF V4-32 UNPACK for seven vectors. */
SdfModelPacket *sdfModelSetTmRefPacket(const sdfModelNode_t *node, SdfModelPacket *packet,
                                       SdfGraphBufferIndex bufferIndex) {
    u32 address = (u32)(node->workspace + (bufferIndex << 7)) & 0x0FFFFFFF;

    packet->dmaTag.bits = ((s64)address << 32) | 0x30000008;
    packet->vifCodes.fields.secondCode = 0x6C07C000;
    packet->vifCodes.fields.firstCode = 0;
    return packet + 1;
}

/* Append a DMA RET tag with no transferred quadwords and zero VIF codes. */
SdfModelPacket *sdfModelSetTermPacket(SdfModelPacket *packet) {
    packet->vifCodes.bits = 0;
    packet->dmaTag.bits = 0x60000000;
    return packet + 1;
}

/* Select an asset packet from the root model's parsed resource table. */
SdfDmaTagHeader *sdfModelSetMatRefPacket(sdfModelRootNode_t *model, s32 index,
                                         SdfDmaTagHeader *packet, SdfGraphBufferIndex bufferIndex) {
    return sdfMatMakePacket(((struct sdfMat_t **)model->resources->buffer)[index], packet,
                            bufferIndex);
}

/* Walk a read-only command list and report its total packet byte size, the
 * number of kind-3 entries, and the kind 4-8 quadword total. */
s32 sdfModelGetDrawPacketSize(const u32 *list, s32 *outCount, s32 *outQuadwords) {
    s32 remainingCommandCount;
    const SdfMeasureCommandPrefix *command;
    s32 bytes;
    s32 kind3Count;
    s32 quadwords;

    if (list == 0) {
        *outCount = 0;
        *outQuadwords = 0;
        return 0;
    }
    remainingCommandCount = list[0];
    list++;
    bytes = 0;
    kind3Count = 0;
    remainingCommandCount &= 0xFFFF;
    quadwords = 0;
    if (remainingCommandCount != 0) {
        do {
            command = (const SdfMeasureCommandPrefix *)*list;
            list++;
            switch (command->kind) {
            case 1:
                bytes += 0x20;
                break;
            case 2:
                bytes += (command->quadwordCount << 4) + 0x40;
                break;
            case 3:
                kind3Count++;
                break;
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
                quadwords += 0x10;
                break;
            }
            remainingCommandCount--;
        } while (remainingCommandCount != 0);
    }
    *outCount += kind3Count;
    *outQuadwords = quadwords;
    return bytes != 0 ? bytes + 0x20 : 0;
}

SdfDmaTagHeader *sdfModelSetNormalDrawPacket(sdfModelNode_t *node, const SdfIndexedCommandView *command,
                                             SdfDmaTagHeader *packet, SdfGraphBufferIndex bufferIndex) {
    u32 packed = command->assetIndexAndCount;
    u16 assetIndex = packed >> 16;
    u16 quadwordCount = packed;
    SdfDmaTagHeader *payload;

    payload = sdfModelSetMatRefPacket(node->root, assetIndex, packet, bufferIndex);
    payload->control = 0x30;
    payload->quadwordCount = quadwordCount;
    payload->address = command->address & 0x0FFFFFFF;
    payload->firstVifCode = 0;
    payload->secondVifCode = 0;
    return payload + 1;
}

#ifdef NON_MATCHING
#include "common.h"
#include "sdf_measure_command.h"
#include "sdf_asset_packets.h"
#include "sdf_dma_tag.h"
#include "sdf_vu_lighting.h"
#include "sdf_chip.h"
#include "sdf_model.h"
#include "pcp_vu0.h"
#include "sdf_draw.h"
#include "sdf_model_copy.h"
#include "sdf_vu_matrix.h"
#include "sdf_vu_matrix_multiply_api.h"
#include "sdf_message.h"
#include "sdf_key_pool.h"
#include "sdf_model_command_views.h"
#include "sdf_packet_alloc.h"

extern SdfDmaTagHeader *sdfModelSetMatRefPacket(sdfModelRootNode_t *model, s32 index, SdfDmaTagHeader *packet, s32 frame);

/* Indexed command with the per-node unpack list that follows the payload words. */
typedef struct SdfIndexedNodeCommandView {
    u8 kind;
    u8 pad01[3];
    u32 assetIndexAndCount; /* 0x04 */
    u32 address;            /* 0x08 */
    u32 nodeCount;          /* 0x0C */
    u16 nodeIds[0];         /* 0x10 */
} SdfIndexedNodeCommandView;

/* Emit an indexed command's packets: the asset packet, a 7-quadword unpack of
 * each listed node's frame workspace, the closing VIF block, then the command's
 * own payload. Upstream: INCLUDE_ASM in sdf/sdfModel; written from the retail
 * routine. 87 of 96 words match retail; two saved registers swap. */
SdfDmaTagHeader *sdfModelSetOneSkinDrawPacket(sdfModelNode_t *node, SdfIndexedNodeCommandView *command,
                                                    SdfDmaTagHeader *packet, s32 frame) {
    u32 packed = command->assetIndexAndCount;
    u32 count;
    u32 i;
    u16 *ids;
    u32 unpackAddress;
    u8 *workspace;
    u32 *vifBlock;

    packet = sdfModelSetMatRefPacket(node->root, packed >> 16, packet, frame);
    packed &= 0xFFFF;
    count = command->nodeCount;
    ids = command->nodeIds;
    unpackAddress = 1;
    i = 0;
    do {
        sdfModelNode_t *target = sdfModelSearchNode(node->root, *ids++);
        i++;
        packet->quadwordCount = 7;
        packet->control = 0x30;
        workspace = sdfModelAllocTmPacket(target);
        packet->address = (u32)(workspace + frame * 0x80) & 0x0FFFFFFF;
        packet->firstVifCode = 0;
        packet->secondVifCode = unpackAddress | 0x6C07C000;
        unpackAddress += 7;
        packet++;
    } while (i != count);
    packet->quadwordCount = 1;
    packet->control = 0x10;
    packet->firstVifCode = 0;
    packet->secondVifCode = 0;
    vifBlock = (u32 *)(packet + 1);
    packet += 2;
    vifBlock[1] = count;
    vifBlock[0] = 0x6001C000;
    vifBlock[2] = 0x0400000E;
    vifBlock[3] = 0x14000000;
    packet->control = 0x30;
    packet->quadwordCount = packed;
    packet->address = command->address & 0x0FFFFFFF;
    packet->firstVifCode = 0;
    packet->secondVifCode = 0;
    return packet + 1;
}
#else
INCLUDE_ASM(const s32, "sdf/sdfModel", sdfModelSetOneSkinDrawPacket);
#endif

#ifdef NON_MATCHING
#include "common.h"
#include "sdf_measure_command.h"
#include "sdf_asset_packets.h"
#include "sdf_dma_tag.h"
#include "sdf_vu_lighting.h"
#include "sdf_chip.h"
#include "sdf_model.h"
#include "pcp_vu0.h"
#include "sdf_draw.h"
#include "sdf_model_copy.h"
#include "sdf_vu_matrix.h"
#include "sdf_vu_matrix_multiply_api.h"
#include "sdf_message.h"
#include "sdf_key_pool.h"
#include "sdf_model_command_views.h"
#include "sdf_packet_alloc.h"
#include "sdf_packet_list.h"
#include "sdf.h"

extern SdfDmaTagHeader *sdfModelSetMatRefPacket(sdfModelRootNode_t *model, s32 index, SdfDmaTagHeader *packet, s32 frame);

/* A run of quadwords a DMA ref tag sends. */
typedef struct SdfShapedSource {
    u32 address;
    u16 quadwordCount;
    u16 pad06;
} SdfShapedSource;

/* One item of a shaped command: its base data, then one run per shape
 * target, indexed by the slot pairs' indices. */
typedef struct SdfShapedItem {
    SdfShapedSource base;
    SdfShapedSource targets[1];
} SdfShapedItem;

/* A kind-2 (shaped) command's payload, as this function reads it. */
typedef struct SdfShapedCommand {
    u8 pad00[4];
    u32 slotIndexAndAssetIndex; /* 0x04: slot entry in the low half, asset in the high */
    u16 itemCount;              /* 0x08 */
    u16 pad0A;
    SdfShapedItem *items[1];    /* 0x0C */
} SdfShapedCommand;

/* The weights' packet: a cnt tag with the UNPACK of two words, then the
 * quadword it sends, two weights followed by ITOP 10 and MSCAL 0. */
typedef struct SdfShapedWeightsPacket {
    SdfModelPacket tag;
    f32 weights[2];
    u32 itopCode;
    u32 mscalCode;
} SdfShapedWeightsPacket;

#define SDF_SHAPED_DMA_REF 0x30000000
#define SDF_SHAPED_PHYSICAL_MASK 0x0FFFFFFF

/* Link a kind-2 (shaped) command's packets onto a list: the asset's packet,
 * then the weights above 1e-6 of the command's slot entry (one of the model's
 * slot pairs; at most two, zero-filled) unpacked to the VU ahead of ITOP 10
 * and MSCAL 0, then for each item a DMA ref to each weighted shape target's
 * run and one to the item's base, and a closing empty quadword. Upstream: INCLUDE_ASM in sdf/sdfModel in
 * both games, whose routines are the same; written from the retail routine.
 * Kept from retail: the item loop is a do-while, so a command with no items
 * would emit the first item's references and not stop until its count
 * wrapped. */
void sdfModelLinkShapedDrawPacket(sdfModelNode_t *node, SdfShapedCommand *command, sdfDrawTag_t *list,
                                  s32 frame) {
    SdfSlotEntry *slot =
        (SdfSlotEntry *)node->root->slotPairs->buffer + (command->slotIndexAndAssetIndex & 0xFFFF);
    u32 assetIndex = command->slotIndexAndAssetIndex >> 16;
    u16 itemCount = command->itemCount;
    SdfSlotPair shapes[2];
    s32 shapeCount = 0;
    s32 packetStart;
    SdfShapedWeightsPacket *weightsPacket;
    SdfModelPacket *packet;
    SdfShapedItem *item;
    s32 i, k;

    for (i = 0; i != 2; i++) {
        if (slot->pair[i].weight > 1e-6f) {
            shapes[shapeCount++] = slot->pair[i];
        }
    }
    packetStart = sdfGrAllocPacketBuff(((shapeCount << 4) + 0x10) * itemCount + 0x40);
    weightsPacket = (SdfShapedWeightsPacket *)sdfModelSetMatRefPacket(
        node->root, assetIndex, (SdfDmaTagHeader *)packetStart, frame);
    weightsPacket->tag.dmaTag.bits = 0x10000001;
    weightsPacket->tag.vifCodes.bits = (s64)0x6002C000 << 32;
    for (i = 0; i < shapeCount; i++) {
        weightsPacket->weights[i] = shapes[i].weight;
    }
    for (; i < 2; i++) {
        weightsPacket->weights[i] = 0.0f;
    }
    weightsPacket->mscalCode = 0x14000000;
    weightsPacket->itopCode = 0x0400000A;
    packet = (SdfModelPacket *)(weightsPacket + 1);
    i = 0;
    do {
        item = command->items[i];
        for (k = 0; k < shapeCount; k++) {
            packet->vifCodes.bits = 0;
            packet->dmaTag.bits =
                (item->targets[shapes[k].index].quadwordCount | SDF_SHAPED_DMA_REF) |
                ((u64)(item->targets[shapes[k].index].address & SDF_SHAPED_PHYSICAL_MASK) << 32);
            packet++;
        }
        packet->vifCodes.bits = 0;
        packet->dmaTag.bits = (item->base.quadwordCount | SDF_SHAPED_DMA_REF) |
                              ((u64)(item->base.address & SDF_SHAPED_PHYSICAL_MASK) << 32);
        packet++;
    } while (++i != itemCount);
    *(u128 *)packet = 0;
    sdfAppendPacketRange(list, packetStart, (u32)packet);
}
#else
INCLUDE_ASM(const s32, "sdf/sdfModel", sdfModelLinkShapedDrawPacket);
#endif

/* Prepend a chip-allocated command node to one buffered draw list. */
SdfCommandNode *sdfModelNewPacketHeader(sdfModelNode_t *drawNode, s32 packetSelector,
                                       SdfGraphBufferIndex bufferIndex) {
    SdfCommandNode *node = sdfChipAllocDirect(sizeof(*node));
    SdfCommandNode **head = (SdfCommandNode **)((u32)bufferIndex * sizeof(*drawNode->lists) + (u32)drawNode +
                                               (u32)&((sdfModelNode_t *)0)->lists);
    SdfCommandNode *next = *head;

    node->packetSelector = packetSelector;
    *head = node;
    node->quadwordCount = 1;
    node->allocation = NULL;
    node->next = next;
    return node;
}

struct sdfMemHandle_t;
extern struct sdfMemHandle_t * sdfMemAllocDirect(s32 requestedBytes);

/* The list head slot is declared `void **` (generic list storage), not `SdfCommandNode **`: its alias set must
 * conflict with the node's `void *payload` store. cc1probe --sched (hooked==unhooked): sched2 then ranks the
 * kind/selector/payload stores ahead of the halfword/pointer stores by #dependents, reproducing retail's
 * sb kind, sb selector, sw payload, sh, sw next, sw alloc, sw head order. Loop cases 4/6/8 and 5/7 stay separate
 * bodies (a fall-through merges the argument moves). */
void sdfModelLinkDrawPacket(sdfModelNode_t *node, const u32 *commandList,
                            s32 packetSelector, s32 alternateSelector,
                            SdfGraphBufferIndex bufferIndex) {
    s32 commandNodeCount;
    s32 referenceQuadwords;
    s32 bytes;
    u32 count;
    SdfCommandNode *listNode;
    SdfDmaTagHeader *packet = NULL;
    const u32 *cursor;
    SdfMeasureCommandPrefix *command;
    struct sdfMemHandle_t *allocation;
    SdfCommandNode *next;
    void **head;

    bytes = sdfModelGetDrawPacketSize(commandList, &commandNodeCount, &referenceQuadwords);
    if (bytes == 0 && commandNodeCount == 0 && referenceQuadwords == 0) {
        return;
    }
    cursor = commandList;
    sdfModelAllocTmPacket(node);
    count = *cursor++;
    if (alternateSelector != 0) {
        u16 headerSelector = count >> 16;
        packetSelector = headerSelector;
    }
    count &= 0xFFFF;
    if (bytes != 0) {
        allocation = sdfMemAllocDirect(bytes);
        listNode = (SdfCommandNode *)sdfMemLock(allocation);
        head = (void **)((u32)bufferIndex * sizeof(*node->lists) + (u32)node +
                         (u32)&((sdfModelNode_t *)0)->lists);
        next = *head;
        packet = (SdfDmaTagHeader *)(listNode + 1);
        listNode->kind = SDF_COMMAND_NODE_KIND_PACKET_DATA;
        listNode->packetSelector = packetSelector;
        listNode->payload = packet;
        listNode->quadwordCount = (bytes + 0xF) >> 4;
        listNode->next = next;
        listNode->allocation = allocation;
        *head = listNode;
    }
    do {
        command = (SdfMeasureCommandPrefix *)*cursor++;
        switch (command->kind) {
        case 1:
            packet = sdfModelSetNormalDrawPacket(node, (const SdfIndexedCommandView *)command, packet, bufferIndex);
            break;
        case 2:
            packet = sdfModelSetOneSkinDrawPacket(node, command, packet, bufferIndex);
            break;
        case 3:
            listNode = sdfModelNewPacketHeader(node, packetSelector, bufferIndex);
            listNode->kind = SDF_COMMAND_NODE_KIND_SHAPED_COMMANDS;
            listNode->payload = command;
            break;
        case 4:
        case 6:
        case 8:
            listNode = sdfModelNewPacketHeader(node, packetSelector, bufferIndex);
            listNode->kind = SDF_COMMAND_NODE_KIND_CORE_COMMANDS;
            listNode->payload = command;
            break;
        case 5:
        case 7:
            sdfCoreDrawScanOneSkinNodes(node->root, (SdfObjectRefCommand *)command);
            listNode = sdfModelNewPacketHeader(node, packetSelector, bufferIndex);
            listNode->kind = SDF_COMMAND_NODE_KIND_CORE_COMMANDS;
            listNode->payload = command;
            break;
        }
        count--;
    } while (count != 0);
    if (bytes != 0) {
        sdfModelSetTermPacket((SdfModelPacket *)packet);
    }
}

/* Build the local rotation basis and append its translation as the fourth row. */
void sdfModelMakeLocalTm(sdfModelNode_t *node) {
    SDF_VU0_LOADVECTOR(node->quaternion);
    effMiscQuaternionToMatrixVU();
    SDF_VU0_LOAD_ROW3(node->translation);
    VU0_STORE_MATRIX(node->localMatrix);
}

/* Copy item identity, Euler rotation, translation, scale and optional clipping bounds. */
void sdfModelNodeBaseSet(sdfModelNode_t *node, const SdfItem *item) {
    node->sourceData = item;
    node->nodeId = item->nodeId;
    sdfConvertEulerAnglesToQuaternionVU(item->rotationX, item->rotationY, item->rotationZ);
    SDF_VU0_STOREVECTOR(node->quaternion);
    SDF_VU0_COPYVECTOR(node->translation, &item->translation);
    SDF_VU0_COPYVECTOR(node->scale, &item->scale);
    node->translation[3] = 1.0f;
    sdfModelMakeLocalTm(node);
    node->boundsCorners = item->boundsCorners;
}

void sdfModelNodeSet(sdfModelNode_t *node, const SdfItem *item) {
    SdfGraphBufferIndex bufferIndex;
    s32 slot;
    u32 **cursor;
    u32 *commandList;

    sdfModelNodeBaseSet(node, item);
    switch (item->commandSetupMode) {
    case SDF_ITEM_COMMAND_SETUP_INLINE_LISTS:
        for (bufferIndex = 0; bufferIndex != 2; bufferIndex++) {
            for (slot = 0; slot != 3; slot++) {
                sdfModelLinkDrawPacket(node, item->commandData.inlineCommandLists[slot],
                                       slot, 0, bufferIndex);
            }
        }
        break;
    case SDF_ITEM_COMMAND_SETUP_NULL_TERMINATED_LIST_TABLE:
        if (item->commandData.commandList.commandLists != NULL) {
            for (bufferIndex = 0; bufferIndex != 2; bufferIndex++) {
                cursor = item->commandData.commandList.commandLists;
                while ((commandList = *cursor++) != NULL) {
                    sdfModelLinkDrawPacket(node, commandList, 0, 1, bufferIndex);
                }
            }
        }
        break;
    }
}

/* Reset a draw node's command lists and initialize both buffered passes. */
void sdfModelNodeSetShape(sdfModelNode_t *node, const u32 *commandList, s32 packetSelector) {
    SdfGraphBufferIndex bufferIndex;

    node->sourceData = commandList;
    sdfModelPacketFree(node);
    sdfModelAllocTmPacket(node);
    sdfModelMakeLocalTm(node);
    for (bufferIndex = 0; bufferIndex != 2; bufferIndex++) {
        sdfModelLinkDrawPacket(node, commandList, packetSelector, 0, bufferIndex);
    }
}


/* Allocate `count` zeroed two-pair slot entries and attach them to the model. */
void sdfModelMakeMorphList(sdfModelRootNode_t *model, s32 count) {
    DevRequest *buf;
    SdfSlotEntry *entries;
    s32 slotIndex;
    s32 pairIndex;

    if (count > 0) {
        buf = sdfDevCreateBufferedRequest(count, 0x10, 1);
        model->slotPairs = buf;
        entries = buf->buffer;
        for (slotIndex = 0; slotIndex != count; slotIndex++) {
            for (pairIndex = 0; pairIndex != 2; pairIndex++) {
                entries[slotIndex].pair[pairIndex].index = 0;
                entries[slotIndex].pair[pairIndex].weight = 0.0f;
            }
        }
        buf->usedCount = count;
    }
}

sdfModelNode_t *sdfModelNodeNew(void);
sdfModelRootNode_t *sdfModelRootNew(void);
void sdfModelAllocNodeList(sdfModelRootNode_t *model, s32 count);
void sdfModelNodeAdd(sdfModelRootNode_t *list, sdfModelNode_t *owner, sdfModelNode_t *node);

/* Build the buffered model, parse its asset records and create one draw node per
 * item, linking each node under its parent item's node when it has one. */
sdfModelRootNode_t *sdfModelMakeRoot(DevRequest *resourceList, sdfModelDef_t *listRef) {
    s32 itemIndex = 0;
    sdfModelRootNode_t *model = sdfModelRootNew();
    const SdfItemList *list;
    s32 count;
    const SdfItem *item;
    sdfModelNode_t *node;
    sdfModelNode_t *parent;

    model->assetData = resourceList;
    model->resources = sdfMatListMake(resourceList, listRef->serializedAssets);
    model->chunkTable = listRef->chunkTable;
    sdfModelMakeMorphList(model, listRef->slotPairCount);
    list = listRef->items;
    count = list->count;
    list++;
    sdfModelAllocNodeList(model, count);
    item = (const SdfItem *)list;
    if (count != itemIndex) {
        do {
            node = sdfModelNodeNew();
            parent = NULL;
            if (item->parentNodeIndex >= 0) {
                parent = ((sdfModelNode_t **)model->list->buffer)[item->parentNodeIndex];
            }
            sdfModelNodeAdd(model, parent, node);
            itemIndex++;
            node->nodeId = item->nodeId;
            item++;
        } while (itemIndex != count);
    }
    return model;
}

/* Attach each 0x50-byte item to the corresponding model draw node. */
sdfModelRootNode_t *sdfModelMake(DevRequest *resourceList, sdfModelDef_t *listRef) {
    s32 itemIndex = 0;
    sdfModelRootNode_t *model = sdfModelMakeRoot(resourceList, listRef);
    const SdfItemList *list = listRef->items;
    s32 count = list->count;
    const SdfItem *item = list->firstItem;

    if (count != itemIndex) {
        do {
            sdfModelNodeSet(((sdfModelNode_t **)model->list->buffer)[itemIndex], item);
            item++;
            itemIndex++;
        } while (itemIndex != count);
    }
    return model;
}

/* Attach each item using the alternate draw-node setup path. */
sdfModelRootNode_t *sdfModelMakeEarly(DevRequest *resourceList, sdfModelDef_t *listRef) {
    s32 itemIndex = 0;
    sdfModelRootNode_t *model = sdfModelMakeRoot(resourceList, listRef);
    const SdfItemList *list;
    s32 count;
    const SdfItem *item;

    model->flags |= SDF_MODEL_ALTERNATE_ITEM_SETUP;
    list = listRef->items;
    count = list->count;
    item = list->firstItem;
    if (count != itemIndex) {
        do {
            sdfModelNodeBaseSet(((sdfModelNode_t **)model->list->buffer)[itemIndex], item);
            item++;
            itemIndex++;
        } while (itemIndex != count);
    }
    return model;
}

/* Compose each draw node's VU transform and visit its circular child list. */
void sdfModelEvalNode(sdfModelNode_t *drawNode, const f32 (*parentMatrix)[4],
                      SdfGraphBufferIndex bufferIndex) {
    f32 *xAxis = drawNode->localMatrix[0];
    f32 *yAxis;
    f32 *zAxis;
    f32 *scale;
    f32 *translation;
    f32 (*transformed)[4];
    u8 *workspace;
    sdfModelNode_t *child;

    SDF_VU0_LOAD_ROW0_BARRIER(xAxis);
    yAxis = drawNode->localMatrix[1];
    SDF_VU0_LOAD_ROW1_BARRIER(yAxis);
    zAxis = drawNode->localMatrix[2];
    SDF_VU0_LOAD_ROW2_BARRIER(zAxis);
    scale = drawNode->scale;
    SDF_VU0_LOADVECTOR_BARRIER(scale);
    SDF_VU0_SCALE_XYZ();
    translation = drawNode->translation;
    SDF_VU0_LOAD_ROW3_BARRIER(translation);
    SDF_VU0_LOADMATRIX_B(parentMatrix);
    MultMatrixRevNoArg();
    transformed = drawNode->worldMatrix;
    VU0_STORE_MATRIX(transformed);
    workspace = drawNode->workspace;
    if (workspace != NULL) {
        sdf3DPrimSetTmPacket((VuLightingPacket *)(workspace + (bufferIndex << 7)));
    }
    child = drawNode->children;
    if (child == 0) {
        return;
    }
    do {
        sdfModelEvalNode(child, transformed, bufferIndex);
        child = child->next;
    } while (child != drawNode->children);
}

/* Scale the root transform and propagate it into the first draw node. */
void sdfModelEvalTm2(sdfModelRootNode_t *model, SdfGraphBufferIndex bufferIndex) {
    /* Keep the VU-aligned store view and read-only row-matrix view together. */
    union {
        u128 quadwords[4];
        const f32 rows[4][4];
    } rootMatrix;
    f32 (*transform)[4] = model->matrix;
    f32 *scale;
    DevRequest *list;

    VU0_LOAD_MATRIX(transform);
    scale = model->scaleVector;
    SDF_VU0_LOADVECTOR_BARRIER(scale);
    SDF_VU0_SCALE_XYZ_REV();
    VU0_STORE_MATRIX_M(rootMatrix.quadwords[0], rootMatrix.quadwords[1], rootMatrix.quadwords[2],
                       rootMatrix.quadwords[3]);
    list = model->list;
    sdfModelEvalNode(((sdfModelNode_t **)list->buffer)[0], rootMatrix.rows, bufferIndex);
}

/* Update the model with the engine's current signed frame index. */
void sdfModelEvalTm(sdfModelRootNode_t *model) {
    sdfModelEvalTm2(model, (s8)sdfFrameId);
}

/* Store a provider's four draw surfaces and make the second a key-sorted pool. */
void sdfStoreMessageWordsAndNotifyConsumer(SdfModelDrawSurfaceSet surfaces, struct SdfPoolNode *first,
                                           struct SdfPoolNode *pool, struct SdfPoolNode *third,
                                           struct SdfPoolNode *fourth) {
    surfaces[3] = fourth;
    surfaces[0] = first;
    surfaces[2] = third;
    surfaces[1] = pool;
    sdfModelSetOtCallbacks((SdfPool *)pool);
}

typedef u8 SdfClipPlaneMask;

/* Return clip-plane bits shared by every corner of the bounds box. */
SdfClipPlaneMask sdfModelCheckBounds(const f32 *bounds) {
    s32 flags;
    SdfClipPlaneMask result;

    VU0_CLIP_TEST_BOX_CORNERS(flags, bounds);
    result = flags & 0x3F;
    result &= (flags >> 12) & (flags >> 6);
    result &= flags >> 18;
    return result;
}

#ifdef NON_MATCHING
#include "common.h"
#include "sdf_measure_command.h"
#include "sdf_asset_packets.h"
#include "sdf_dma_tag.h"
#include "sdf_vu_lighting.h"
#include "sdf_chip.h"
#include "sdf_model.h"
#include "pcp_vu0.h"
#include "sdf_draw.h"
#include "sdf_model_copy.h"
#include "sdf_vu_matrix.h"
#include "sdf_vu_matrix_multiply_api.h"
#include "sdf_message.h"
#include "sdf_key_pool.h"
#include "sdf_model_command_views.h"
#include "sdf_packet_alloc.h"
#include "sdf_packet_list.h"
#include "sdf.h"

extern SdfDmaTagHeader *sdfModelSetMatRefPacket(sdfModelRootNode_t *model, s32 index, SdfDmaTagHeader *packet, s32 frame);

extern void sdfResourceListApplyEntryChanges(DevRequest *list, s32 entryIndex);
extern vu8 sdfFrameId;

extern void sdfResourceListApplyEntryChanges(DevRequest *, s32);
extern void sdfCoreDrawSetZBias(f32);
extern f32 sdf3DScreenTm[16];
extern u8 corePktSoftenBias;
extern f32 sdfFaceDivSoften, sdfFaceDivBias;

extern void *sdf3DFogDefault;
extern const struct SdfDmaTagHeader *sdf3DLightDefault;
/* Queue a model's visible draw nodes: for each command-list node that passes
 * the clip test, start each output list with the node's lighting and unpack
 * state (the model's light and fog, the 3D layer's defaults where it has
 * none; its soften and bias, the 3D layer's unless it overrides them; soften
 * switched off among the unpack flags), append its indexed asset packets,
 * then hand each list to its surface; mark the model drawn when any were
 * queued. Upstream: INCLUDE_ASM in both games' sdf/sdfModel; written from the
 * retail routine. Its instructions equal retail's apart from register
 * allocation and scheduling. */
void sdfModelTempPacketLinkOt(SdfPoolNode **surfaces, sdfModelRootNode_t *model) {
    sdfDrawTag_t *lists[4];
    DevRequest *nodeList;
    s32 nodeIndex;
    s32 frame;
    u32 modelColor;
    u32 color;
    u32 drawFlags;
    SdfDmaTagHeader *light;
    SdfDmaTagHeader *fog;
    SdfDmaTagHeader *defaultLight, *defaultFog;
    u32 overrides;
    u32 softenOff;
    s32 anyPackets;
    u32 softenFlags;
    u64 unpack;
    s32 negativeColor;
    f32 soften, bias, depth;
    u32 **groups;
    u32 *group;
    SdfIndexedCommandView *command;
    sdfModelNode_t *node;
    const SdfItem *item;
    sdfDrawTag_t *list;
    s32 j;
    s32 header;
    u32 nodeFlags;
    void *matrix;
    SdfModelPacket *packet;
    SdfDmaTagHeader *payload;
    frame = (s8)sdfFrameId;
    sdfResourceListApplyEntryChanges(model->resources, frame);
    modelColor = model->color;
    defaultLight = (SdfDmaTagHeader *)sdf3DLightDefault;
    light = model->lighting;
    overrides = model->flags & 2;
    fog = model->secondaryLighting;
    defaultFog = (SdfDmaTagHeader *)sdf3DFogDefault;
    light = light == NULL ? defaultLight : light;
    fog = fog == NULL ? defaultFog : fog;
    drawFlags = model->drawFlags;
    softenOff = corePktSoftenBias;
    if (overrides) {
        soften = model->scalarOverrideFirst;
        bias = model->scalarOverrideSecond;
    } else {
        soften = sdfFaceDivSoften;
        bias = sdfFaceDivBias;
    }
    nodeIndex = 0;
    anyPackets = 0;
    for (; nodeIndex < model->list->usedCount; nodeIndex++) {
            nodeList = model->list;
            softenFlags = softenOff;
            softenFlags <<= 16;
            node = ((sdfModelNode_t **)nodeList->buffer)[nodeIndex];
            item = node->sourceData;
            if (item == NULL) continue;
            if (item->commandSetupMode != 1) continue;
            groups = item->commandData.commandList.commandLists;
            if (groups == NULL) continue;
            if (node->flags & 0x11) continue;
            matrix = node->worldMatrix;
            VU0_LOAD_MATRIX(matrix);
            sdfPostmultiplyVuMatrixFromMemory(sdf3DScreenTm);
            if (node->boundsCorners != NULL && sdfModelCheckBounds(node->boundsCorners) != 0) continue;
            VU0_MOVE_VF(vf10, vf31);
            SDF_VU0_GETVECTOR_W(depth);
            color = modelColor;
            if (node->flags & 2) color = node->color;
            nodeFlags = drawFlags & (u16)node->drawFlagMask;
            for (j = 0; j != 4; j++) lists[j] = NULL;
            sdfCoreDrawSetZBias(model->modelScale);
            group = *groups++;
            if (group != NULL) {
                negativeColor = (color >> 31) != 0;
                unpack = ((u64)(nodeFlags | softenFlags) << 32) | 0x6102C000;
                do {
                    header = *group++;
                    j = (u32)header >> 16;
                    j = j == 0 ? negativeColor == 0 : j;
                    list = lists[j];
                    if (list == NULL) {
                        SdfModelPacket *statePacket;
                        list = sdfPrimAllocDrawTag(NULL);
                        ((f32 *)list)[3] = depth;
                        list->firstReferenceSource = (u32)light;
                        lists[j] = list;
                        list->secondReferenceSource = (u32)fog;
                        statePacket = (SdfModelPacket *)(u32)sdfGrAllocPacketBuff(0xC0);
                        statePacket[0].dmaTag.bits = 0x10000008;
                        statePacket[0].vifCodes.bits = (u64)0x6C07C000 << 32;
                        VU0_LOAD_MATRIX(matrix);
                        sdf3DPrimSetTmPacket((VuLightingPacket *)(statePacket + 1));
                        *(u64 *)((u8 *)statePacket + 0x90) = 0x20000002;
                        *(u32 *)((u8 *)statePacket + 0xA0) = 0x6E01C002;
                        *(u32 *)((u8 *)statePacket + 0xA8) = 0x6003C003;
                        *(u32 *)((u8 *)statePacket + 0xA4) = color;
                        *(u32 *)((u8 *)statePacket + 0xAC) = *(u32 *)&model->modelScale;
                        *(u32 *)((u8 *)statePacket + 0xB8) = 0x04000004;
                        *(u32 *)((u8 *)statePacket + 0xBC) = 0x14000000;
                        *(u64 *)((u8 *)statePacket + 0x98) = unpack;
                        *(f32 *)((u8 *)statePacket + 0xB0) = soften;
                        *(f32 *)((u8 *)statePacket + 0xB4) = bias;
                        sdfAppendPacketRange(list, (u32)statePacket, (u32)statePacket + 0x90);
                    }
                    command = (SdfIndexedCommandView *)*group++;
                    if (command != NULL) {
                        do {
                            if (command->kind == 1) {
                                u32 packed;
                                u16 quadwordCount;
                                packet = (SdfModelPacket *)(u32)sdfGrAllocPacketBuff(0x30);
                                packed = command->assetIndexAndCount;
                                quadwordCount = packed;
                                payload = sdfModelSetMatRefPacket(model, packed >> 16, (SdfDmaTagHeader *)packet, frame);
                                ((SdfModelPacket *)payload)->vifCodes.bits = 0;
                                ((SdfModelPacket *)payload)->dmaTag.bits = (u64)(0x30000000 | quadwordCount) | ((u64)(command->address & 0x0FFFFFFF) << 32);
                                payload++;
                                ((SdfModelPacket *)payload)->dmaTag.bits = 0x20000000;
                                ((SdfModelPacket *)payload)->vifCodes.bits = 0;
                                sdfAppendPacketRange(list, (u32)packet, (u32)payload);
                            }
                            command = (SdfIndexedCommandView *)*group++;
                        } while (command != NULL);
                    }
                    group = *groups++;
                } while (group != NULL);
            }
            j = 0;
            do {
                list = lists[j];
                if (list != NULL) {
                    SdfPoolNode **slot = surfaces + j;
                    anyPackets = 1;
                    (*slot)->append(*slot, list);
                }
                lists[j] = NULL;
                j++;
            } while (j != 4);
    }
    if (anyPackets) model->flags |= 0x10;
}
#else
INCLUDE_ASM(const s32, "sdf/sdfModel", sdfModelTempPacketLinkOt);
#endif

#include "sdf.h"
extern u8 corePktSoftenBias;
extern const struct SdfDmaTagHeader *sdf3DLightDefault;
extern void *sdf3DFogDefault;
extern f32 sdfFaceDivSoften, sdfFaceDivBias;
extern f32 sdf3DScreenTm[16];
extern void sdfCoreDrawSetZBias(f32);
/* Build one per-frame packet list for each visible node resource variant. */
void sdfModelBuildNodeResourcePackets(SdfPoolNode **surfaces, sdfModelRootNode_t *model) {
    SdfGraphBufferIndex bufferIndex;
    s32 nodeIndex;
    u32 modelColor;
    u32 nodeMask;
    s32 nodeFlags;
    const struct SdfDmaTagHeader *lighting;
    s32 appended;
    u32 globalModeBits;
    u64 secondHeader;
    s32 negativeColor;
    void *secondaryLighting;
    s32 skipCommands;
    u32 globalByte;
    f32 firstScalar;
    f32 secondScalar;

    model->flags &= (u8)~0x10;
    if (model->flags & SDF_MODEL_ALTERNATE_ITEM_SETUP) {
        sdfBuildModelPacketsVia(sdfModelTempPacketLinkOt, surfaces, model);
    }

    bufferIndex = (s8)sdfFrameId;
    if (model->flags & SDF_MODEL_FORCE_ASSET_TEXTURE_REFRESH_WITH_ZERO_STATE_WORD) {
        sdfSetForcedAssetTextureMode(SDF_FORCED_ASSET_TEXTURE_MODE_REFRESH_WITH_ZERO_STATE_WORD);
        sdfMatListUpdatePacket(model->resources, bufferIndex);
        sdfSetForcedAssetTextureMode(SDF_FORCED_ASSET_TEXTURE_MODE_NORMAL);
    } else if (model->flags & SDF_MODEL_FORCE_ASSET_TEXTURE_REFRESH) {
        sdfSetForcedAssetTextureMode(SDF_FORCED_ASSET_TEXTURE_MODE_REFRESH);
        sdfMatListUpdatePacket(model->resources, bufferIndex);
        sdfSetForcedAssetTextureMode(SDF_FORCED_ASSET_TEXTURE_MODE_NORMAL);
    } else {
        sdfResourceListApplyEntryChanges(model->resources, bufferIndex);
    }

    lighting = model->lighting;
    if (lighting == NULL) {
        lighting = sdf3DLightDefault;
    }
    secondaryLighting = model->secondaryLighting;
    if (secondaryLighting == NULL) {
        secondaryLighting = sdf3DFogDefault;
    }
    globalByte = corePktSoftenBias;
    modelColor = model->color;
    nodeMask = model->drawFlags;
    if (model->flags & SDF_MODEL_USE_SCALAR_OVERRIDES) {
        firstScalar = model->scalarOverrideFirst;
        secondScalar = model->scalarOverrideSecond;
    } else {
        firstScalar = sdfFaceDivSoften;
        secondScalar = sdfFaceDivBias;
    }
    skipCommands = 0;
    if (model->flags & 0x08) {
        skipCommands = 1;
    }

    appended = 0;
    for (nodeIndex = 0; nodeIndex < model->list->usedCount; nodeIndex++) {
        sdfModelNode_t *node;
        SdfCommandNode *entry;
        struct sdfDrawTag_t *packets;
        struct sdfDrawTag_t *variantLists[4];
        s32 color;
        s32 variant;
        f32 depth;

        globalModeBits = globalByte;
        globalModeBits <<= 16;
        node = ((sdfModelNode_t **)model->list->buffer)[nodeIndex];
        entry = node->lists[bufferIndex];
        if (entry == NULL ||
            (node->flags & (SDF_DRAW_NODE_FLAG_SKIP_RENDER | SDF_DRAW_NODE_FLAG_MOTION_HIDDEN)) != 0) {
            continue;
        }

        VU0_LOAD_MATRIX(node->worldMatrix);
        sdfPostmultiplyVuMatrixFromMemory((const struct Matrix4 *)sdf3DScreenTm);
        if (node->boundsCorners != NULL &&
            sdfModelCheckBounds(node->boundsCorners) != 0) {
            continue;
        }
        SDF_VU0_GET_ROW3();
        SDF_VU0_GETVECTOR_W(depth);

        color = modelColor;
        if (node->flags & SDF_DRAW_NODE_FLAG_USE_NODE_COLOR) {
            color = node->color;
        }
        nodeFlags = nodeMask & node->drawFlagMask;
        for (variant = 0; variant != 4; variant++) {
            variantLists[variant] = NULL;
        }
        sdfCoreDrawSetZBias(model->modelScale);

        negativeColor = 0;
        if (color < 0) {
            negativeColor = 1;
        }
        secondHeader = ((u64)(nodeFlags | globalModeBits) << 32) | 0x6102C000;
        do {
            SdfModelLightingCallPacket *lightingPacket;

            variant = (u8)entry->packetSelector;
            if ((variant >> 4) != model->lodIndex) {
                continue;
            }
            variant &= 0x0F;
            variant = variant == 0 ? negativeColor == 0 : variant;
            packets = variantLists[variant];
            if (packets == NULL) {
                packets = sdfPrimAllocDrawTag(NULL);
                variantLists[variant] = packets;
                *(f32 *)&packets->unkC = depth;
                packets->firstReferenceSource = (u32)lighting;
                packets->secondReferenceSource = (u32)secondaryLighting;
                lightingPacket = (SdfModelLightingCallPacket *)sdfGrAllocPacketBuff(0x40);
                sdfModelSetTmRefPacket(node, &lightingPacket->address, bufferIndex);
                lightingPacket->firstHeader = 0x20000002;
                lightingPacket->secondHeader = secondHeader;
                lightingPacket->lightingCommand = 0x6E01C002;
                lightingPacket->color = color;
                lightingPacket->command = 0x6003C003;
                *(u32 *)&lightingPacket->modelScale = *(u32 *)&model->modelScale;
                lightingPacket->firstScalar = firstScalar;
                lightingPacket->secondScalar = secondScalar;
                lightingPacket->finalCommand = 0x04000004;
                lightingPacket->finalAddress = 0x14000000;
                sdfAppendReferencePacket(packets, (u32)lightingPacket);
            }

            switch (entry->kind) {
            case SDF_COMMAND_NODE_KIND_PACKET_DATA: {
                if (!skipCommands) {
                    u8 *referencePacket = (u8 *)sdfGrAllocPacketBuff(0x20);
                    *(u16 *)referencePacket = 0;
                    *(u32 *)(referencePacket + 4) = (u32)(entry + 1) & 0x0FFFFFFF;
                    *(u64 *)(referencePacket + 8) = 0;
                    *(u128 *)(referencePacket + 16) = 0;
                    sdfDrawTagLinkCall(packets, (u32)referencePacket);
                }
                break;
            }
            case SDF_COMMAND_NODE_KIND_SHAPED_COMMANDS:
                if (!skipCommands) {
                    sdfModelLinkShapedDrawPacket(node, entry->payload, packets, bufferIndex);
                }
                break;
            case SDF_COMMAND_NODE_KIND_CORE_COMMANDS: {
                u32 packet;
                sdfCoreDrawSetLight(lighting);
                sdfCoreDrawSetFog(secondaryLighting);
                packet = sdfCorePacketGen(node, entry->payload, nodeFlags, color,
                                       firstScalar, secondScalar,
                                       bufferIndex, skipCommands);
                if (packet != 0) {
                    sdfDrawTagLink(packets, packet);
                }
                break;
            }
            }
        } while ((entry = entry->next) != NULL);

        variant = 0;
        do {
            packets = variantLists[variant];
            if (packets != NULL) {
                SdfPoolNode **slot = surfaces + variant;
                (*slot)->append(*slot, packets);
                appended = 1;
            }
            variantLists[variant] = NULL;
            variant++;
        } while (variant != 4);
    }
    if (appended) {
        model->flags |= SDF_MODEL_HAS_ANCHOR_RECORDS;
    }
}

/* Copy the destination's declared number of 16-byte object entries. */
void sdfModelMorphListCopyStat(DevRequest *destination, const DevRequest *source) {
    s16 count;

    if (destination == NULL) {
        return;
    }
    count = destination->usedCount;
    if (count > 0) {
        memcpy(destination->buffer, source->buffer, count * 16);
    }
}
