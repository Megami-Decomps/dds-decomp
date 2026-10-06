#include "common.h"
#include "pcp_vu0.h"
#include "sdf_draw.h"

#define SDF_MODEL_ALTERNATE_ITEM_SETUP 4

extern void *sdfInitNodeHeaderFromWords(u32 *words, void *node, s32 wordIndex);
extern void *sdfAllocSizeClassBlock(s32 arg0);
extern void sdfInstallPoolNodeReleaseCallbacks(s32 arg0);
extern void *memcpy(void *dst, const void *src, u32 n);
extern void sdfFreeNodeLists(SdfDrawNode *node);
extern void *sdfEnsureFreeRootWorkspace(SdfDrawNode *node);
extern void func_002D83F8(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void func_002D86E0(void *arg0, void *arg1);
extern void sdfMultiplyVuMatrixInPlace(void);
extern void sdfWriteVuLightingPacket(u32 arg0);
extern vu8 sdfCurrentBufferIndex;

/* One DMA tag followed by two VIF codes; all aliases retain the 16-byte packet layout. */
typedef struct {
    union {
        s64 bits;
        struct {
            u16 quadwordCount;
            u8 reservedByte;
            u8 control; /* PCE, tag ID and IRQ in the high tag byte */
            u32 address;
        } fields;
    } dmaTag;
    union {
        s64 bits;
        struct {
            s32 firstCode;
            s32 secondCode;
        } fields;
    } vifCodes;
} SdfPacket; /* 0x10 */


typedef struct SdfSlotPair {
    s32 index;
    f32 weight;
} SdfSlotPair;


typedef struct SdfSlotEntry {
    SdfSlotPair pair[2];
} SdfSlotEntry;



typedef struct {
    s32 firstWord;
    s32 work;
    s32 thirdWord;
    s32 fourthWord;
} SdfMsg;

typedef struct {
    s32 count;           /* 0x00: entry count */
    u8 pad_0x04[0x0C];   /* 0x04 */
    u8 firstItem;        /* 0x10: entries, 0x50 stride */
} SdfItemList;

typedef struct {
    SdfItemList *items;   /* 0x00 */
} SdfItemListRef;

/* Per-item record applied to a draw node by sdfDrawNodeSetFromItem (0x50 bytes). */
typedef struct {
    u8 pad00[8];
    s32 nodeId;       /* 0x08 */
    u8 pad0C[4];      /* 0x0C: parent-node index used during construction */
    f32 rotationX;    /* 0x10: Euler angles supplied to the quaternion builder */
    f32 rotationY;    /* 0x14 */
    f32 rotationZ;    /* 0x18 */
    u8 pad1C[4];
    u128 translation; /* 0x20 */
    u128 scale;       /* 0x30 */
    s32 boundsAddress; /* 0x40: optional local-box corners used by clipping */
} SdfItem;

extern SdfModel *sdfModelCreateFromAssetData(void *arg0, void *arg1);
extern void effMiscQuaternionToMatrixVU(void);
extern void func_002E7F20(f32 x, f32 y, f32 z);
void sdfDrawNodeBuildMatrix(SdfDrawNode *node);
void sdfDrawNodeSetFromItem(SdfDrawNode *node, SdfItem *item);


/* Retail selects by nodeId when flags bit 0 is set, otherwise by array index. */
u8 *sdfModelFindDrawNode(void *chunk, s32 id) {
    SdfModel *model = (SdfModel *)chunk;
    DevRequest *list = model->list;
    s16 count;
    SdfDrawNode **entries;
    s32 i;

    count = list->usedCount;
    entries = list->buffer;
    if (model->flags & 1) {
        for (i = 0; i < count; i++) {
            SdfDrawNode *node = entries[i];
            if (node->nodeId == id) {
                return (u8 *)node;
            }
        }
        return 0;
    }
    if ((u32)id >= (u32)count) {
        return 0;
    }
    return (u8 *)entries[id];
}

/* Append a DMA REF for eight quadwords and a VIF V4-32 UNPACK for seven vectors. */
SdfPacket *sdfModelWriteAddressPacket(SdfDrawNode *node, SdfPacket *packet, s32 index) {
    u32 address = (node->address + (index << 7)) & 0x0FFFFFFF;

    packet->dmaTag.bits = ((s64)address << 32) | 0x30000008;
    packet->vifCodes.fields.secondCode = 0x6C07C000;
    packet->vifCodes.fields.firstCode = 0;
    return packet + 1;
}

/* Append a DMA RET tag with no transferred quadwords and zero VIF codes. */
SdfPacket *sdfModelWriteFixedPacket(SdfPacket *packet) {
    packet->vifCodes.bits = 0;
    packet->dmaTag.bits = 0x60000000;
    return packet + 1;
}

/* Select an asset packet from the root model's parsed resource table. */
void *sdfModelWriteIndexedAssetPacket(SdfModel *model, s32 index, void *packet, s32 frame) {
    return sdfInitNodeHeaderFromWords(((u32 **)model->resources->buffer)[index], packet, frame);
}

/* A command-list entry: a kind byte followed by per-kind payload words. */
typedef struct SdfMeasureCommand {
    u8 kind;
    u8 pad01[0xB];
    u16 quadwordCount; /* 0x0C */
} SdfMeasureCommand;

/* Walk a command list and report its total packet byte size, the number of
 * kind-3 entries, and the kind 4-8 quadword total. */
s32 sdfCommandListMeasure(u32 *list, s32 *outCount, s32 *outQuadwords) {
    s32 count;
    SdfMeasureCommand *command;
    s32 bytes;
    s32 count3;
    s32 quadwords;

    if (list == 0) {
        *outCount = 0;
        *outQuadwords = 0;
        return 0;
    }
    count = list[0];
    list++;
    bytes = 0;
    count3 = 0;
    count &= 0xFFFF;
    quadwords = 0;
    if (count != 0) {
        do {
            command = (SdfMeasureCommand *)*list;
            list++;
            switch (command->kind) {
            case 1:
                bytes += 0x20;
                break;
            case 2:
                bytes += (command->quadwordCount << 4) + 0x40;
                break;
            case 3:
                count3++;
                break;
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
                quadwords += 0x10;
                break;
            }
            count--;
        } while (count != 0);
    }
    *outCount += count3;
    *outQuadwords = quadwords;
    return bytes != 0 ? bytes + 0x20 : 0;
}

typedef struct {
    u8 kind;
    u8 pad01[3];
    u32 assetIndexAndCount;
    u32 address;
} SdfIndexedCommand;

typedef struct {
    u16 quadwordCount;
    u8 reservedByte;
    u8 control;
    u32 address;
    u32 firstVifCode;
    u32 secondVifCode;
} SdfIndexedPayload;

SdfIndexedPayload *func_002D7F40(SdfDrawNode *node, SdfIndexedCommand *command, void *packet, s32 frame) {
    u32 packed = command->assetIndexAndCount;
    u16 assetIndex = packed >> 16;
    u16 quadwordCount = packed;
    SdfIndexedPayload *payload;

    payload = sdfModelWriteIndexedAssetPacket(node->root, assetIndex, packet, frame);
    payload->control = 0x30;
    payload->quadwordCount = quadwordCount;
    payload->address = command->address & 0x0FFFFFFF;
    payload->firstVifCode = 0;
    payload->secondVifCode = 0;
    return payload + 1;
}

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D7FB0);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D8130);

extern SdfCommandNode *func_002D8388(SdfDrawNode *drawNode, s32 packetSelector, s32 listIndex);
INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D8388);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D83F8);

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
    func_002E7F20(item->rotationX, item->rotationY, item->rotationZ);
    VU0_STORE_VF(vf10, node->quaternion);
    PCP_COPY_VECTOR(node->translation, &item->translation);
    PCP_COPY_VECTOR(node->scale, &item->scale);
    node->translation[3] = 1.0f;
    sdfDrawNodeBuildMatrix(node);
    node->boundsAddress = item->boundsAddress;
}

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D86E0);

/* Reset a draw node's command lists and initialize both buffered passes. */
void sdfModelResetAndInitNodes(SdfDrawNode *node, s32 commandList, s32 packetSelector) {
    s32 i;

    node->sourceItem = (void *)commandList;
    sdfFreeNodeLists(node);
    sdfEnsureFreeRootWorkspace(node);
    sdfDrawNodeBuildMatrix(node);
    for (i = 0; i != 2; i++) {
        func_002D83F8(node, commandList, packetSelector, 0, i);
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

INCLUDE_ASM(const s32, "sdf/sdfModel", sdfModelCreateFromAssetData);

/* Attach each 0x50-byte item to the corresponding model draw node. */
SdfModel *sdfModelCreateWithItems(void *data, SdfItemListRef *listRef) {
    s32 i = 0;
    SdfModel *model = sdfModelCreateFromAssetData(data, listRef);
    SdfItemList *list = listRef->items;
    s32 count = list->count;
    u8 *item = &list->firstItem;

    if (count != i) {
        do {
            func_002D86E0(((SdfDrawNode **)model->list->buffer)[i], item);
            item += 0x50;
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
    u8 *item;

    model->flags |= SDF_MODEL_ALTERNATE_ITEM_SETUP;
    list = listRef->items;
    count = list->count;
    item = &list->firstItem;
    if (count != i) {
        do {
            sdfDrawNodeSetFromItem(((SdfDrawNode **)model->list->buffer)[i], (SdfItem *)item);
            item += 0x50;
            i++;
        } while (i != count);
    }
    return model;
}

/* Compose each draw node's VU transform and visit its circular child list. */
void sdfModelUpdateDrawNodeTransforms(SdfDrawNode *drawNode, void *parentMatrix, s32 frame) {
    f32 *xAxis = drawNode->localMatrix[0];
    f32 *yAxis;
    f32 *zAxis;
    f32 *scale;
    f32 *translation;
    f32 (*transformed)[4];
    u32 address;
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
    address = drawNode->address;
    if (address != 0) {
        sdfWriteVuLightingPacket(address + (frame << 7));
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
    sdfModelUpdateDrawNodeTransforms(((SdfDrawNode **)list->buffer)[0], rootMatrix, frame);
}

/* Update the model with the engine's current signed frame index. */
void sdfModelUpdateCurrentFrameTransforms(SdfModel *model) {
    sdfModelUpdateRootTransforms(model, (s8)sdfCurrentBufferIndex);
}

/* Store the message words and install release callbacks on its work pointer. */
void sdfStoreMessageWordsAndNotifyConsumer(SdfMsg *message, s32 firstWord, s32 work, s32 thirdWord, s32 fourthWord) {
    message->fourthWord = fourthWord;
    message->firstWord = firstWord;
    message->thirdWord = thirdWord;
    message->work = work;
    sdfInstallPoolNodeReleaseCallbacks(work);
}

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D8CB8);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D8DD0);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D9238);

/* Copy the destination's declared number of 16-byte object entries. */
void sdfModelCopyData(DevRequest *dst, DevRequest *src) {
    s16 count;

    if (dst == NULL) {
        return;
    }
    count = dst->usedCount;
    if (count > 0) {
        memcpy(dst->buffer, src->buffer, count * 16);
    }
}
