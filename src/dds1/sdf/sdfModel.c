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
#include "sdf_model_command_views.h"

void sdfDrawNodeBuildMatrix(SdfDrawNode *node);
void sdfDrawNodeSetFromItem(SdfDrawNode *node, SdfItem *item);





extern void *memcpy(void *dst, const void *src, u32 n);


extern vu8 sdfCurrentBufferIndex;

/* Retail selects by nodeId when SDF_MODEL_FIND_DRAW_NODE_BY_ID is set,
 * otherwise by array index. */
SdfDrawNode *sdfModelFindDrawNode(SdfModel *model, s32 id) {
    DevRequest *list = model->list;
    s16 count;
    SdfDrawNode **entries;
    s32 i;

    count = list->usedCount;
    entries = list->buffer;
    if (model->flags & SDF_MODEL_FIND_DRAW_NODE_BY_ID) {
        for (i = 0; i < count; i++) {
            SdfDrawNode *node = entries[i];
            if (node->nodeId == id) {
                return node;
            }
        }
        return 0;
    }
    if ((u32)id >= (u32)count) {
        return 0;
    }
    return entries[id];
}

/* Append a DMA REF for eight quadwords and a VIF V4-32 UNPACK for seven vectors. */
SdfModelPacket *sdfModelWriteAddressPacket(SdfDrawNode *node, SdfModelPacket *packet, s32 index) {
    u32 address = (u32)(node->workspace + (index << 7)) & 0x0FFFFFFF;

    packet->dmaTag.bits = ((s64)address << 32) | 0x30000008;
    packet->vifCodes.fields.secondCode = 0x6C07C000;
    packet->vifCodes.fields.firstCode = 0;
    return packet + 1;
}

/* Append a DMA RET tag with no transferred quadwords and zero VIF codes. */
SdfModelPacket *sdfModelWriteFixedPacket(SdfModelPacket *packet) {
    packet->vifCodes.bits = 0;
    packet->dmaTag.bits = 0x60000000;
    return packet + 1;
}

/* Select an asset packet from the root model's parsed resource table. */
SdfDmaTagHeader *sdfModelWriteIndexedAssetPacket(SdfModel *model, s32 index, SdfDmaTagHeader *packet, s32 frame) {
    return (SdfDmaTagHeader *)sdfInitAssetDrawEntryReferenceNode(
        ((struct SdfAsset **)model->resources->buffer)[index], packet, frame);
}

/* Walk a command list and report its total packet byte size, the number of
 * kind-3 entries, and the kind 4-8 quadword total. */
s32 sdfCommandListMeasure(u32 *list, s32 *outCount, s32 *outQuadwords) {
    s32 remainingCommandCount;
    SdfMeasureCommandPrefix *command;
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
            command = (SdfMeasureCommandPrefix *)*list;
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

SdfDmaTagHeader *sdfModelBuildIndexedCommandPayload(SdfDrawNode *node, SdfIndexedCommandView *command,
                                                    SdfDmaTagHeader *packet, s32 frame) {
    u32 packed = command->assetIndexAndCount;
    u16 assetIndex = packed >> 16;
    u16 quadwordCount = packed;
    SdfDmaTagHeader *payload;

    payload = sdfModelWriteIndexedAssetPacket(node->root, assetIndex, packet, frame);
    payload->control = 0x30;
    payload->quadwordCount = quadwordCount;
    payload->address = command->address & 0x0FFFFFFF;
    payload->firstVifCode = 0;
    payload->secondVifCode = 0;
    return payload + 1;
}

INCLUDE_ASM(const s32, "sdf/sdfModel", sdfModelBuildIndexedCommandPackets);

INCLUDE_ASM(const s32, "sdf/sdfModel", sdfModelBuildClippedCommandPackets);

/* Prepend a chip-allocated command node to one buffered draw list. */
SdfCommandNode *sdfDrawNodePrependCommandNode(SdfDrawNode *drawNode, s32 packetSelector, s32 listIndex) {
    SdfCommandNode *node = sdfAllocSizeClassBlock(sizeof(*node));
    SdfCommandNode **head = (SdfCommandNode **)((u32)listIndex * sizeof(*drawNode->lists) + (u32)drawNode +
                                               (u32)&((SdfDrawNode *)0)->lists);
    SdfCommandNode *next = *head;

    node->packetSelector = packetSelector;
    *head = node;
    node->quadwordCount = 1;
    node->allocation = NULL;
    node->next = next;
    return node;
}

INCLUDE_ASM(const s32, "sdf/sdfModel", sdfDrawNodeBuildCommandList);

/* Build the local rotation basis and append its translation as the fourth row. */
void sdfDrawNodeBuildMatrix(SdfDrawNode *node) {
    VU0_LOAD_VF(vf10, node->quaternion);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf31, node->translation);
    VU0_STORE_MATRIX(node->localMatrix);
}

/* Copy item identity, Euler rotation, translation, scale and optional clipping bounds. */
void sdfDrawNodeSetFromItem(SdfDrawNode *node, SdfItem *item) {
    node->sourceItem = item;
    node->nodeId = item->nodeId;
    sdfConvertEulerAnglesToQuaternionVU(item->rotationX, item->rotationY, item->rotationZ);
    VU0_STORE_VF(vf10, node->quaternion);
    PCP_COPY_VECTOR(node->translation, &item->translation);
    PCP_COPY_VECTOR(node->scale, &item->scale);
    node->translation[3] = 1.0f;
    sdfDrawNodeBuildMatrix(node);
    node->boundsCorners = item->boundsCorners;
}

void sdfDrawNodeBuildFromItemAndCommands(SdfDrawNode *node, SdfItem *item) {
    s32 pass;
    s32 slot;
    u32 **cursor;
    u32 *commandList;

    sdfDrawNodeSetFromItem(node, item);
    switch (item->commandSetupMode) {
    case SDF_ITEM_COMMAND_SETUP_INLINE_LISTS:
        for (pass = 0; pass != 2; pass++) {
            for (slot = 0; slot != 3; slot++) {
                sdfDrawNodeBuildCommandList(node, item->commandData.inlineCommandLists[slot],
                              slot, 0, pass);
            }
        }
        break;
    case SDF_ITEM_COMMAND_SETUP_NULL_TERMINATED_LIST_TABLE:
        if (item->commandData.commandList.commandLists != NULL) {
            for (pass = 0; pass != 2; pass++) {
                cursor = item->commandData.commandList.commandLists;
                while ((commandList = *cursor++) != NULL) {
                    sdfDrawNodeBuildCommandList(node, commandList, 0, 1, pass);
                }
            }
        }
        break;
    }
}

/* Reset a draw node's command lists and initialize both buffered passes. */
void sdfModelResetAndInitNodes(SdfDrawNode *node, u32 *commandList, s32 packetSelector) {
    s32 i;

    node->sourceItem = commandList;
    sdfFreeNodeLists(node);
    sdfEnsureFreeRootWorkspace(node);
    sdfDrawNodeBuildMatrix(node);
    for (i = 0; i != 2; i++) {
        sdfDrawNodeBuildCommandList(node, commandList, packetSelector, 0, i);
    }
}


/* Allocate `count` zeroed two-pair slot entries and attach them to the model. */
void sdfModelAllocateSlotPairs(SdfModel *model, s32 count) {
    DevRequest *buf;
    SdfSlotEntry *entries;
    s32 i;
    s32 j;

    if (count > 0) {
        buf = sdfDevCreateBufferedRequest(count, 0x10, 1);
        model->slotPairs = buf;
        entries = buf->buffer;
        for (i = 0; i != count; i++) {
            for (j = 0; j != 2; j++) {
                entries[i].pair[j].index = 0;
                entries[i].pair[j].weight = 0.0f;
            }
        }
        buf->usedCount = count;
    }
}

SdfDrawNode *sdfCreateDrawNode(void);
SdfModel *sdfCreateBufferedTransformSlot(void);
void sdfResizeBufferedSlotRequest(SdfModel *model, s32 count);
void sdfAppendBufferedRouteNode(SdfModel *list, SdfDrawNode *owner, SdfDrawNode *node);
DevRequest *sdfAssetListParse(DevRequest *lookup, u32 *data);

/* Build the buffered model, parse its asset records and create one draw node per
 * item, linking each node under its parent item's node when it has one. */
SdfModel *sdfModelCreateFromAssetData(void *data, SdfItemListRef *listRef) {
    s32 i = 0;
    SdfModel *model = sdfCreateBufferedTransformSlot();
    SdfItemList *list;
    s32 count;
    SdfItem *item;
    SdfDrawNode *node;
    SdfDrawNode *parent;

    model->assetData = data;
    model->resources = sdfAssetListParse(data, listRef->serializedAssetData);
    model->chunkTable = listRef->chunkTable;
    sdfModelAllocateSlotPairs(model, listRef->slotPairCount);
    list = listRef->items;
    count = list->count;
    list++;
    sdfResizeBufferedSlotRequest(model, count);
    item = (SdfItem *)list;
    if (count != i) {
        do {
            node = sdfCreateDrawNode();
            parent = NULL;
            if (item->parentNodeIndex >= 0) {
                parent = ((SdfDrawNode **)model->list->buffer)[item->parentNodeIndex];
            }
            sdfAppendBufferedRouteNode(model, parent, node);
            i++;
            node->nodeId = item->nodeId;
            item++;
        } while (i != count);
    }
    return model;
}

/* Attach each 0x50-byte item to the corresponding model draw node. */
SdfModel *sdfModelCreateWithItems(void *data, SdfItemListRef *listRef) {
    s32 i = 0;
    SdfModel *model = sdfModelCreateFromAssetData(data, listRef);
    SdfItemList *list = listRef->items;
    s32 count = list->count;
    SdfItem *item = list->firstItem;

    if (count != i) {
        do {
            sdfDrawNodeBuildFromItemAndCommands(((SdfDrawNode **)model->list->buffer)[i], item);
            item++;
            i++;
        } while (i != count);
    }
    return model;
}

/* Attach each item using the alternate draw-node setup path. */
SdfModel *sdfModelCreateWithAlternateItems(void *data, SdfItemListRef *listRef) {
    s32 i = 0;
    SdfModel *model = sdfModelCreateFromAssetData(data, listRef);
    SdfItemList *list;
    s32 count;
    SdfItem *item;

    model->flags |= SDF_MODEL_ALTERNATE_ITEM_SETUP;
    list = listRef->items;
    count = list->count;
    item = list->firstItem;
    if (count != i) {
        do {
            sdfDrawNodeSetFromItem(((SdfDrawNode **)model->list->buffer)[i], item);
            item++;
            i++;
        } while (i != count);
    }
    return model;
}

/* Compose each draw node's VU transform and visit its circular child list. */
void sdfModelUpdateDrawNodeTransforms(SdfDrawNode *drawNode, const f32 (*parentMatrix)[4], s32 frame) {
    f32 *xAxis = drawNode->localMatrix[0];
    f32 *yAxis;
    f32 *zAxis;
    f32 *scale;
    f32 *translation;
    f32 (*transformed)[4];
    u8 *workspace;
    SdfDrawNode *child;

    VU0_LOAD_VF_MEMORY(vf28, xAxis);
    yAxis = drawNode->localMatrix[1];
    VU0_LOAD_VF_MEMORY(vf29, yAxis);
    zAxis = drawNode->localMatrix[2];
    VU0_LOAD_VF_MEMORY(vf30, zAxis);
    scale = drawNode->scale;
    VU0_LOAD_VF_MEMORY(vf10, scale);
    VU0_SCALE_MATRIX_ROWS(vf10);
    translation = drawNode->translation;
    VU0_LOAD_VF_MEMORY(vf31, translation);
    VU0_LOAD_MATRIX_B(parentMatrix);
    sdfMultiplyVuMatrixInPlace();
    transformed = drawNode->worldMatrix;
    VU0_STORE_MATRIX(transformed);
    workspace = drawNode->workspace;
    if (workspace != NULL) {
        sdfWriteVuLightingPacket((VuLightingPacket *)(workspace + (frame << 7)));
    }
    child = drawNode->children;
    if (child == 0) {
        return;
    }
    do {
        sdfModelUpdateDrawNodeTransforms(child, transformed, frame);
        child = child->next;
    } while (child != drawNode->children);
}

/* Scale the root transform and propagate it into the first draw node. */
void sdfModelUpdateRootTransforms(SdfModel *model, s32 frame) {
    u128 rootMatrix[4];
    void *transform = model->matrix;
    f32 *scale;
    DevRequest *list;

    VU0_LOAD_MATRIX(transform);
    scale = model->scaleVector;
    VU0_LOAD_VF_MEMORY(vf10, scale);
    VU0_MUL_MATRIX_ROWS_VF10();
    VU0_STORE_MATRIX_M(rootMatrix[0], rootMatrix[1], rootMatrix[2], rootMatrix[3]);
    list = model->list;
    sdfModelUpdateDrawNodeTransforms(((SdfDrawNode **)list->buffer)[0], (const f32 (*)[4])rootMatrix, frame);
}

/* Update the model with the engine's current signed frame index. */
void sdfModelUpdateCurrentFrameTransforms(SdfModel *model) {
    sdfModelUpdateRootTransforms(model, (s8)sdfCurrentBufferIndex);
}

/* Store the message words and install release callbacks on its work pointer. */
void sdfStoreMessageWordsAndNotifyConsumer(SdfMsg *message, s32 firstWord, u32 *work, s32 thirdWord, s32 fourthWord) {
    message->fourthWord = fourthWord;
    message->firstWord = firstWord;
    message->thirdWord = thirdWord;
    message->work = work;
    sdfInstallPoolNodeReleaseCallbacks(work);
}

/* Nonzero when the bounds box lies entirely outside one clip plane: each
 * plane's bit must be set in all four 6-bit clip-flag groups. */
s32 sdfModelTestTransformedClipVectors(const f32 *bounds) {
    s32 flags;
    s32 result;

    VU0_CLIP_TEST_BOX_CORNERS(flags, bounds);
    result = flags & 0x3F;
    result &= (flags >> 12) & (flags >> 6);
    result &= flags >> 18;
    return result;
}

INCLUDE_ASM(const s32, "sdf/sdfModel", sdfModelBuildVisibleNodePackets);

#include "sdf.h"
extern u8 D_003BD344;
extern void *D_003BD374;
extern void *D_003BD378;
extern f32 D_003BD358, D_003BD35C;
extern f32 D_00398470[16];
extern void *sdfAllocPacketAligned(s32);
extern void sdfVuSetGlobalScale(f32);
extern u32 func_002E0820(SdfDrawNode *, void *, s32, s32, f32, f32, s32, s32);
typedef struct {
    SdfModelPacket address;
    u64 firstHeader;
    u64 secondHeader;
    u32 lightingCommand;
    s32 color;
    u32 command;
    u32 modelScalar;
    f32 firstScalar;
    f32 secondScalar;
    u32 finalCommand;
    u32 finalAddress;
} SdfModelLightingCallPacket;
typedef char SdfModelLightingCallPacket_size[(sizeof(SdfModelLightingCallPacket) == 0x40) ? 1 : -1];

/* Build one per-frame packet list for each visible node resource variant. */
void sdfModelBuildNodeResourcePackets(SdfPoolNode **surfaces, SdfModel *model) {
    s32 frame;
    s32 nodeIndex;
    u32 modelColor;
    u32 nodeMask;
    s32 nodeFlags;
    void *lighting;
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
        sdfModelBuildVisibleNodePackets(surfaces, model);
    }

    frame = (s8)sdfCurrentBufferIndex;
    if (model->flags & 0x20) {
        sdfSetForcedAssetTextureMode(1);
        sdfApplyResourceListEntriesWithForcedTexture(model->resources, frame);
        sdfSetForcedAssetTextureMode(0);
    } else if (model->flags & 0x40) {
        sdfSetForcedAssetTextureMode(2);
        sdfApplyResourceListEntriesWithForcedTexture(model->resources, frame);
        sdfSetForcedAssetTextureMode(0);
    } else {
        sdfResourceListApplyEntryChanges(model->resources, frame);
    }

    lighting = model->lighting;
    if (lighting == NULL) {
        lighting = D_003BD374;
    }
    secondaryLighting = model->secondaryLighting;
    if (secondaryLighting == NULL) {
        secondaryLighting = D_003BD378;
    }
    globalByte = D_003BD344;
    modelColor = model->color;
    nodeMask = model->drawFlags;
    if (model->flags & SDF_MODEL_USE_SCALAR_OVERRIDES) {
        firstScalar = model->scalarOverrideFirst;
        secondScalar = model->scalarOverrideSecond;
    } else {
        firstScalar = D_003BD358;
        secondScalar = D_003BD35C;
    }
    skipCommands = 0;
    if (model->flags & 0x08) {
        skipCommands = 1;
    }

    appended = 0;
    for (nodeIndex = 0; nodeIndex < model->list->usedCount; nodeIndex++) {
        SdfDrawNode *node;
        SdfCommandNode *entry;
        struct SdfListHead *packets;
        struct SdfListHead *variantLists[4];
        s32 color;
        s32 variant;
        f32 depth;

        globalModeBits = globalByte;
        globalModeBits <<= 16;
        node = ((SdfDrawNode **)model->list->buffer)[nodeIndex];
        entry = node->lists[frame];
        if (entry == NULL || (node->flags & 0x11) != 0) {
            continue;
        }

        VU0_LOAD_MATRIX(node->worldMatrix);
        sdfPostmultiplyVuMatrixFromMemory(D_00398470);
        if (node->boundsCorners != NULL &&
            sdfModelTestTransformedClipVectors(node->boundsCorners) != 0) {
            continue;
        }
        VU0_MOVE_VF(vf10, vf31);
        VU0_GET_VF10_W(depth);

        color = modelColor;
        if (node->flags & SDF_DRAW_NODE_FLAG_USE_NODE_COLOR) {
            color = node->color;
        }
        nodeFlags = nodeMask & (u16)node->unk16;
        for (variant = 0; variant != 4; variant++) {
            variantLists[variant] = NULL;
        }
        sdfVuSetGlobalScale(model->unk94);

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
                packets = sdfAllocatePacketList(NULL);
                variantLists[variant] = packets;
                *(f32 *)&packets->unkC = depth;
                packets->firstReferenceSource = (u32)lighting;
                packets->secondReferenceSource = (u32)secondaryLighting;
                lightingPacket = (SdfModelLightingCallPacket *)sdfAllocPacketAligned(0x40);
                sdfModelWriteAddressPacket(node, &lightingPacket->address, frame);
                lightingPacket->firstHeader = 0x20000002;
                lightingPacket->secondHeader = secondHeader;
                lightingPacket->lightingCommand = 0x6E01C002;
                lightingPacket->color = color;
                lightingPacket->command = 0x6003C003;
                lightingPacket->modelScalar = (*(u32 *)&model->unk94);
                lightingPacket->firstScalar = firstScalar;
                lightingPacket->secondScalar = secondScalar;
                lightingPacket->finalCommand = 0x04000004;
                lightingPacket->finalAddress = 0x14000000;
                sdfAppendReferencePacket(packets, (u32)lightingPacket);
            }

            switch (entry->kind) {
            case 1: {
                if (!skipCommands) {
                    u8 *referencePacket = sdfAllocPacketAligned(0x20);
                    *(u16 *)referencePacket = 0;
                    *(u32 *)(referencePacket + 4) = ((u32)entry + 0x10) & 0x0FFFFFFF;
                    *(u64 *)(referencePacket + 8) = 0;
                    *(u128 *)(referencePacket + 16) = 0;
                    sdfAppendCallPacket(packets, (u32)referencePacket);
                }
                break;
            }
            case 2:
                if (!skipCommands) {
                    sdfModelBuildClippedCommandPackets(node, entry->payload, packets, frame);
                }
                break;
            case 3: {
                u32 packet;
                sdfVuSelectTransformMatrix((u32)lighting);
                sdfVuCacheObjectVector((u8 *)secondaryLighting);
                packet = func_002E0820(node, entry->payload, nodeFlags, color,
                                       firstScalar, secondScalar,
                                       frame, skipCommands);
                if (packet != 0) {
                    sdfAppendPacket(packets, packet);
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
void sdfModelCopyData(DevRequest *destination, DevRequest *source) {
    s16 count;

    if (destination == NULL) {
        return;
    }
    count = destination->usedCount;
    if (count > 0) {
        memcpy(destination->buffer, source->buffer, count * 16);
    }
}
