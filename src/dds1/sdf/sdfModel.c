#include "common.h"
#include "pcp_vu0.h"

#define SDF_DRAW_TRANSLATION_VECTOR 0
#define SDF_DRAW_SCALE_VECTOR 1
#define SDF_DRAW_X_AXIS_VECTOR 2
#define SDF_DRAW_Y_AXIS_VECTOR 3
#define SDF_DRAW_Z_AXIS_VECTOR 4
#define SDF_MODEL_ALTERNATE_ITEM_SETUP 4

extern void *sdfInitNodeHeaderFromWords(s32 arg0, s32 arg1, s32 arg2);
extern void *func_002CFEB8(s32 arg0);
extern void *sdfDevCreateBufferedRequest(s32 arg0, s32 arg1, s32 arg2);
extern void sdfInstallPoolNodeReleaseCallbacks(s32 arg0);
extern void *memcpy(void *dst, const void *src, u32 n);
extern void sdfFreeNodeLists(void);
extern void sdfEnsureFreeRootWorkspace(void *arg0);
extern void func_002D83F8(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void func_002D86E0(void *arg0, void *arg1);
extern void sdfMultiplyVuMatrixInPlace(void);
extern void func_002E1FF8(u32 arg0);
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

typedef struct {
    u8 pad_0x00[0x18];
    u32 unk18;
} SdfEntry;

typedef struct {
    u8 pad_0x00[0x04];
    s16 count; /* 0x04: draw-node entries */
    u8 pad_0x06[0x06];
    struct SdfDrawNode **entries;
} SdfList;

typedef struct {
    SdfList *list;     /* 0x00 */
    u8 pad_0x04[0x0C]; /* 0x04 */
    void *slotPairs;   /* 0x10: buffer allocated by sdfModelAllocateSlotPairs */
    u8 pad_0x14[0x05]; /* 0x14 */
    u8 flags;          /* 0x19: bit 0 looks up node IDs; bit 2 selects alternate item setup */
    u8 pad_0x1A[0x06]; /* 0x1A */
    u8 transformStart; /* 0x20: COP2 reads 0x40 bytes across following fields */
    u8 pad_0x21[0x07];
    struct SdfNode *nodes[2]; /* 0x28: per-slot node lists */
    s32 packetAddressBase; /* 0x30: base of 128-byte indexed address packets */
    u8 pad_0x34[0x04]; /* 0x34 */
    s32 commandList;   /* 0x38: source measured and compiled into both slot lists */
    u8 pad_0x3C[0x34];
    u8 scaleVector[0x10]; /* 0x70 */
} SdfModel;

/* Each model draw node owns a circular list of child draw nodes. */
typedef struct SdfDrawNode {
    u8 pad00[4];
    struct SdfDrawNode *next;     /* 0x04 */
    u8 pad08[4];
    struct SdfDrawNode *children; /* 0x0C */
    u8 pad10[8];
    s32 nodeId;                   /* 0x18: lookup key when model flags bit 0 is set */
    u8 pad1C[0x14];
    u32 address;                  /* 0x30 */
    s32 boundsAddress;            /* 0x34: optional address of two local xyz box corners */
    void *sourceItem;             /* 0x38: item this node was built from */
    u8 pad3C[0x14];
    u8 quaternion[0x10];          /* 0x50 */
    u8 vectors[5][0x10];         /* 0x60-0xAF: COP2 inputs */
    u8 localTranslationRow[0x10]; /* 0xB0: fourth row written by sdfDrawNodeBuildMatrix */
    u8 worldMatrix[0x40];         /* 0xC0: local transform composed with its parent */
} SdfDrawNode;

/* The indexed resource words are stored at offset 0x0C of the asset table. */
typedef struct SdfAssetTable {
    u8 pad00[0x0C];
    s32 *entries;
} SdfAssetTable;

typedef struct SdfChunk {
    u8 pad_0x00[0x0C];
    SdfAssetTable *assets;
} SdfChunk;

typedef struct {
    u8 pad_0x00[0x04];
    u32 unk4;
    u32 unk8;
} SdfInfo;

typedef struct SdfNode {
    void *next;        /* 0x00: next node in the per-slot command list */
    u8 kind;          /* 0x04: packed payload or routed command */
    s8 packetSelector; /* 0x05: high nibble filters the pass, low nibble selects a packet list */
    s16 quadwordCount; /* 0x06: payload size rounded up to 16-byte units */
    u8 pad_0x08[0x4]; /* 0x8 */
    s32 resourceHandle; /* 0x0C: retained backing allocation, zero for standalone nodes */
} SdfNode; /* 0x10 */

typedef struct {
    u8 pad_0x00[0x28];
    void *unk28;
} SdfLink;

typedef struct {
    u8 pad_0x00[0x04];
    s16 unk4;
    u8 pad_0x06[0x06];
    void *unkC;
} SdfBuf;

/* The copy descriptor counts fixed-width, 16-byte entries. */
typedef struct {
    u8 bytes[0x10];
} SdfObjectEntry;

typedef struct {
    u8 pad_0x00[0x04];
    s16 count;
    u8 pad_0x06[0x06];
    SdfObjectEntry *entries;
} SdfObj;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
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
INCLUDE_ASM(const s32, "sdf/sdfModel", sdfModelFindDrawNode);

/* Append a DMA REF for eight quadwords and a VIF V4-32 UNPACK for seven vectors. */
SdfPacket *sdfModelWriteAddressPacket(SdfModel *model, SdfPacket *packet, s32 index) {
    u32 address = (model->packetAddressBase + (index << 7)) & 0x0FFFFFFF;

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

/* Find a resource word by index in the chunk's asset table. */
void *sdfModelWriteIndexedAssetPacket(SdfChunk *chunk, s32 index, s32 packet, s32 frame) {
    return sdfInitNodeHeaderFromWords(chunk->assets->entries[index], packet, frame);
}

INCLUDE_ASM(const s32, "sdf/sdfModel", sdfCommandListMeasure);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D7F40);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D7FB0);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D8130);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D8388);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D83F8);

/* Build the local rotation basis and append its translation as the fourth row. */
void sdfDrawNodeBuildMatrix(SdfDrawNode *node) {
    VU0_LOAD_VF(vf10, node->quaternion);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf31, node->vectors[SDF_DRAW_TRANSLATION_VECTOR]);
    VU0_STORE_MATRIX(node->vectors[SDF_DRAW_X_AXIS_VECTOR]);
}

/* Copy item identity, Euler rotation, translation, scale and optional clipping bounds. */
void sdfDrawNodeSetFromItem(SdfDrawNode *node, SdfItem *item) {
    node->sourceItem = item;
    node->nodeId = item->nodeId;
    func_002E7F20(item->rotationX, item->rotationY, item->rotationZ);
    VU0_STORE_VF(vf10, node->quaternion);
    PCP_COPY_VECTOR(node->vectors[SDF_DRAW_TRANSLATION_VECTOR], &item->translation);
    PCP_COPY_VECTOR(node->vectors[SDF_DRAW_SCALE_VECTOR], &item->scale);
    ((f32 *)node->vectors[SDF_DRAW_TRANSLATION_VECTOR])[3] = 1.0f;
    sdfDrawNodeBuildMatrix(node);
    node->boundsAddress = item->boundsAddress;
}

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D86E0);

/* Reset the node lists and initialize both per-model slots. */
void sdfModelResetAndInitNodes(SdfModel *model, s32 commandList, s32 packetSelector) {
    s32 i = 0;
    s32 j = 0;

    model->commandList = commandList;
    sdfFreeNodeLists();
    sdfEnsureFreeRootWorkspace(model);
    sdfDrawNodeBuildMatrix((SdfDrawNode *)model);
    /* The re-initialization below is load-bearing for a byte-identical build. */
    i = 0;
    j = 0;
    do {
        i++;
        func_002D83F8(model, commandList, packetSelector, 0, j);
        j = i;
    } while (i != 2);
}

typedef struct SdfSlotPair {
    s32 first;
    s32 second;
} SdfSlotPair;

typedef struct SdfSlotEntry {
    SdfSlotPair pair[2];
} SdfSlotEntry;

typedef struct SdfSlotBuf {
    u8 pad00[4];
    s16 count;             /* 0x04 */
    u8 pad06[6];
    SdfSlotEntry *entries; /* 0x0C */
} SdfSlotBuf;

/* Allocate `count` zeroed two-pair slot entries and attach them to the model. */
void sdfModelAllocateSlotPairs(SdfModel *model, s32 count) {
    SdfSlotBuf *buf;
    SdfSlotEntry *entries;
    s32 i;
    s32 j;

    if (count > 0) {
        buf = sdfDevCreateBufferedRequest(count, 0x10, 1);
        model->slotPairs = buf;
        entries = buf->entries;
        for (i = 0; i != count; i++) {
            for (j = 0; j != 2; j++) {
                entries[i].pair[j].first = 0;
                entries[i].pair[j].second = 0;
            }
        }
        buf->count = count;
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
            func_002D86E0(model->list->entries[i], item);
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
            sdfDrawNodeSetFromItem(model->list->entries[i], (SdfItem *)item);
            item += 0x50;
            i++;
        } while (i != count);
    }
    return model;
}

/* Compose each draw node's VU transform and visit its circular child list. */
void sdfModelUpdateDrawNodeTransforms(SdfDrawNode *drawNode, void *parentMatrix, s32 frame) {
    u8 *xAxis = drawNode->vectors[SDF_DRAW_X_AXIS_VECTOR];
    u8 *yAxis;
    u8 *zAxis;
    u8 *scale;
    u8 *translation;
    u8 *transformed;
    u32 address;
    SdfDrawNode *child;

        VU0_LOAD_VF_MEMORY(vf28, xAxis);
    yAxis = drawNode->vectors[SDF_DRAW_Y_AXIS_VECTOR];
        VU0_LOAD_VF_MEMORY(vf29, yAxis);
    zAxis = drawNode->vectors[SDF_DRAW_Z_AXIS_VECTOR];
        VU0_LOAD_VF_MEMORY(vf30, zAxis);
    scale = drawNode->vectors[SDF_DRAW_SCALE_VECTOR];
        VU0_LOAD_VF_MEMORY(vf10, scale);
    VU0_SCALE_MATRIX_ROWS(vf10);
    translation = drawNode->vectors[SDF_DRAW_TRANSLATION_VECTOR];
    VU0_LOAD_VF_MEMORY(vf31, translation);
    VU0_LOAD_MATRIX_B(parentMatrix);
    sdfMultiplyVuMatrixInPlace();
    transformed = drawNode->worldMatrix;
    VU0_STORE_MATRIX(transformed);
    address = drawNode->address;
    if (address != 0) {
        func_002E1FF8(address + (frame << 7));
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
    u8 *transform = &model->transformStart;
    u8 *scale;
    SdfList *list;

    VU0_LOAD_MATRIX(transform);
    scale = model->scaleVector;
    VU0_LOAD_VF_MEMORY(vf10, scale);
    VU0_MUL_MATRIX_ROWS_VF10();
    VU0_STORE_MATRIX_M(rootMatrix[0], rootMatrix[1], rootMatrix[2], rootMatrix[3]);
    list = model->list;
    sdfModelUpdateDrawNodeTransforms(list->entries[0], rootMatrix, frame);
}

/* Update the model with the engine's current signed frame index. */
void sdfModelUpdateCurrentFrameTransforms(SdfModel *model) {
    sdfModelUpdateRootTransforms(model, (s8)sdfCurrentBufferIndex);
}

/* Store the message words and install release callbacks on its work pointer. */
void sdfStoreMessageWordsAndNotifyConsumer(SdfMsg *message, s32 firstWord, s32 work, s32 thirdWord, s32 fourthWord) {
    message->unkC = fourthWord;
    message->unk0 = firstWord;
    message->unk8 = thirdWord;
    message->unk4 = work;
    sdfInstallPoolNodeReleaseCallbacks(work);
}

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D8CB8);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D8DD0);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D9238);

/* Copy the destination's declared number of 16-byte object entries. */
void sdfModelCopyData(SdfObj *dst, SdfObj *src) {
    s16 count;

    if (dst == NULL) {
        return;
    }
    count = dst->count;
    if (count > 0) {
        memcpy(dst->entries, src->entries, count * 16);
    }
}
