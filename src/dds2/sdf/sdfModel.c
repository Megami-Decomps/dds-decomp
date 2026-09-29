#include "common.h"

/* 16-byte packet, written at quadword, word, halfword and byte granularity. */
typedef struct {
    union {
        s64 q;
        struct {
            u16 h0;
            u8 b2;
            u8 b3;
            u32 w4;
        } p;
    } u0;
    union {
        s64 q;
        struct {
            s32 w8;
            s32 wC;
        } p;
    } u8;
} SdfPacket; /* 0x10 */

struct SdfDrawNode;

typedef struct {
    u8 pad_0x00[0x04];
    s16 unk4;
    u8 pad_0x06[0x06];
    struct SdfDrawNode **entries;
} SdfList;

typedef struct {
    SdfList *list;     /* 0x00 */
    u8 pad_0x04[0x0C]; /* 0x04 */
    void *unk10;       /* 0x10 */
    u8 pad_0x14[0x05]; /* 0x14 */
    u8 unk19;          /* 0x19 */
    u8 pad_0x1A[0x06]; /* 0x1A */
    u8 transformStart; /* 0x20: COP2 reads four vectors across following fields */
    u8 pad_0x21[0x0F];
    s32 packetAddressBase; /* 0x30: base of 128-byte indexed address packets */
    u8 pad_0x34[0x04]; /* 0x34 */
    s32 unk38;         /* 0x38 */
    u8 pad_0x3C[0x34];
    u8 scaleVector[0x10]; /* 0x70 */
} SdfModel;

/* A draw node owns a circular child list and five COP2 input vectors. */
typedef struct SdfDrawNode {
    u8 pad00[4];
    struct SdfDrawNode *next; /* 0x04 */
    u8 pad08[4];
    struct SdfDrawNode *children; /* 0x0C */
    u8 pad10[0x20];
    u32 address; /* 0x30 */
    u8 pad34[0x2C];
    u8 vectors[5][0x10]; /* 0x60-0xAF */
    u8 padB0[0x10];
    u8 transformed[0x40]; /* 0xC0: four COP2 output vectors */
} SdfDrawNode;

extern void sdfFreeNodeLists(void);

extern void sdfEnsureFreeRootWorkspace(void *arg0);

extern void func_003314B0(void *arg0);

extern void func_003312A8(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

extern void func_00331590(void *arg0, void *arg1);

typedef struct {
    s32 count;           /* 0x00: entry count */
    u8 pad_0x04[0x0C];   /* 0x04 */
    u8 firstItem;        /* 0x10: entries, 0x50 stride */
} SdfItemList;

typedef struct {
    SdfItemList *items;   /* 0x00 */
} SdfItemListRef;

extern SdfModel *func_003317C8(void *arg0, void *arg1);

extern void func_00331500(void *arg0, void *arg1);

extern void func_00336B00(void);

extern void func_0033AEA8(u32 arg0);

extern void *memcpy(void *dst, const void *src, u32 n);

typedef struct {
    u8 bytes[0x10];
} SdfObjectEntry;

typedef struct {
    u8 pad_0x00[0x04];
    s16 count;
    u8 pad_0x06[0x06];
    SdfObjectEntry *entries;
} SdfObj;

extern vu8 D_004389DA;

INCLUDE_ASM(const s32, "sdf/sdfModel", func_00330C18);

/* Emit the model's indexed address and fixed packet command words. */
SdfPacket *sdfModelWriteAddressPacket(SdfModel *model, SdfPacket *packet, s32 index) {
    u32 address = (model->packetAddressBase + (index << 7)) & 0x0FFFFFFF;

    packet->u0.q = ((s64)address << 32) | 0x30000008;
    packet->u8.p.wC = 0x6C07C000;
    packet->u8.p.w8 = 0;
    return packet + 1;
}

/* Append a zeroed packet with its fixed first command word. */
SdfPacket *sdfModelWriteFixedPacket(SdfPacket *packet) {
    packet->u8.q = 0;
    packet->u0.q = 0x60000000;
    return packet + 1;
}

typedef struct SdfAssetTable {
    u8 pad00[0x0C];
    u32 **entries;
} SdfAssetTable;

typedef struct SdfChunk {
    u8 pad00[0x0C];
    SdfAssetTable *assets;
} SdfChunk;

/* Find a resource word by index in the chunk's asset table. */
void sdfModelWriteIndexedAssetPacket(SdfChunk *chunk, s32 index, u32 packet, u32 frame) {
    sdfInitNodeHeaderFromWords(chunk->assets->entries[index], packet, frame);
}

INCLUDE_ASM(const s32, "sdf/sdfModel", func_00330D30);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_00330DF0);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_00330E60);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_00330FE0);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_00331238);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_003312A8);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_003314B0);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_00331500);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_00331590);

/* Reset the node lists and initialize both per-model slots. */
void sdfModelResetAndInitNodes(SdfModel *model, s32 arg1, s32 arg2) {
    s32 i = 0;
    s32 j = 0;

    model->unk38 = arg1;
    sdfFreeNodeLists();
    sdfEnsureFreeRootWorkspace(model);
    func_003314B0(model);
    /* Required to match: reinitialize both loop counters after setting up the model. */
    i = 0;
    j = 0;
    do {
        i++;
        func_003312A8(model, arg1, arg2, 0, j);
        j = i;
    } while (i != 2);
}

INCLUDE_ASM(const s32, "sdf/sdfModel", func_00331740);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_003317C8);

/* Attach each 0x50-byte item to the corresponding model draw node. */
SdfModel *sdfModelCreateWithItems(void *data, SdfItemListRef *listRef) {
    s32 i = 0;
    SdfModel *model = func_003317C8(data, listRef);
    SdfItemList *list = listRef->items;
    s32 count = list->count;
    u8 *item = &list->firstItem;

    if (count != i) {
        do {
            func_00331590(model->list->entries[i], item);
            item += 0x50;
            i++;
        } while (i != count);
    }
    return model;
}

/* Attach each item using the alternate draw-node setup path. */
SdfModel *sdfModelCreateWithAlternateItems(void *data, SdfItemListRef *listRef) {
    s32 i = 0;
    SdfModel *model = func_003317C8(data, listRef);
    SdfItemList *list;
    s32 count;
    u8 *item;

    model->unk19 |= 4;
    list = listRef->items;
    count = list->count;
    item = &list->firstItem;
    if (count != i) {
        do {
            func_00331500(model->list->entries[i], item);
            item += 0x50;
            i++;
        } while (i != count);
    }
    return model;
}

/* Compose each draw node's VU transform and visit its circular child list. */
void sdfModelUpdateDrawNodeTransforms(SdfDrawNode *drawNode, void *parentMatrix, s32 frame) {
    u8 *xAxis = drawNode->vectors[2];
    u8 *yAxis;
    u8 *zAxis;
    u8 *scale;
    u8 *translation;
    u8 *transformed;
    u32 address;
    SdfDrawNode *child;

    __asm__ volatile ("lqc2 vf28, 0(%0)" :: "r" (xAxis) : "memory");
    yAxis = drawNode->vectors[3];
    __asm__ volatile ("lqc2 vf29, 0(%0)" :: "r" (yAxis) : "memory");
    zAxis = drawNode->vectors[4];
    __asm__ volatile ("lqc2 vf30, 0(%0)" :: "r" (zAxis) : "memory");
    scale = drawNode->vectors[1];
    __asm__ volatile ("lqc2 vf10, 0(%0)" :: "r" (scale) : "memory");
    __asm__ volatile (
        ".set noreorder                   \n"
        "vmulx.xyzw vf28, vf28, vf10x     \n"
        "vmuly.xyzw vf29, vf29, vf10y     \n"
        "vmulz.xyzw vf30, vf30, vf10z     \n"
        ".set reorder"
        :
        :
        : "memory"
    );
    translation = drawNode->vectors[0];
    __asm__ volatile ("lqc2 vf31, 0(%0)" :: "r" (translation) : "memory");
    __asm__ volatile (
        ".set noreorder          \n"
        "lqc2 vf24, 0(%0)        \n"
        "lqc2 vf25, 16(%0)       \n"
        "lqc2 vf26, 32(%0)       \n"
        "lqc2 vf27, 48(%0)       \n"
        ".set reorder"
        :
        : "r" (parentMatrix)
        : "memory"
    );
    func_00336B00();
    transformed = drawNode->transformed;
    __asm__ volatile (
        ".set noreorder          \n"
        "sqc2 vf28, 0(%0)        \n"
        "sqc2 vf29, 16(%0)       \n"
        "sqc2 vf30, 32(%0)       \n"
        "sqc2 vf31, 48(%0)       \n"
        ".set reorder"
        :
        : "r" (transformed)
        : "memory"
    );
    address = drawNode->address;
    if (address != 0) {
        func_0033AEA8(address + (frame << 7));
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

    __asm__ volatile (
        ".set noreorder          \n"
        "lqc2 vf28, 0(%0)        \n"
        "lqc2 vf29, 16(%0)       \n"
        "lqc2 vf30, 32(%0)       \n"
        "lqc2 vf31, 48(%0)       \n"
        ".set reorder"
        :
        : "r" (transform)
        : "memory"
    );
    scale = model->scaleVector;
    __asm__ volatile (
        "lqc2 vf10, 0(%0)"
        :
        : "r" (scale)
        : "memory"
    );
    __asm__ volatile (
        ".set noreorder               \n"
        "vmul.xyz vf28, vf28, vf10    \n"
        "vmul.xyz vf29, vf29, vf10    \n"
        "vmul.xyz vf30, vf30, vf10    \n"
        ".set reorder"
        :
        :
        : "memory"
    );
    __asm__ volatile (
        ".set noreorder          \n"
        "sqc2 vf28, %0           \n"
        "sqc2 vf29, %1           \n"
        "sqc2 vf30, %2           \n"
        "sqc2 vf31, %3           \n"
        ".set reorder"
        :
        : "m" (rootMatrix[0]), "m" (rootMatrix[1]), "m" (rootMatrix[2]), "m" (rootMatrix[3])
        : "memory"
    );
    list = model->list;
    sdfModelUpdateDrawNodeTransforms(list->entries[0], rootMatrix, frame);
}

/* Update the model with the engine's current signed frame index. */
void sdfModelUpdateCurrentFrameTransforms(SdfModel *model) {
    sdfModelUpdateRootTransforms(model, (s8)D_004389DA);
}

/* Store four message words, then notify the consumer of the second word. */
void func_00331B38(u32 *arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4) {
    arg0[3] = arg4;
    *arg0 = arg1;
    arg0[2] = arg3;
    arg0[1] = (s32)arg2;
    func_00348BD8(arg2);
}

INCLUDE_ASM(const s32, "sdf/sdfModel", func_00331B68);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_00331C80);

INCLUDE_ASM(const s32, "sdf/sdfModel", func_003320E8);

/* Copy the destination's declared number of 16-byte object entries. */
void sdfModelCopyData(SdfObj *destination, SdfObj *source) {
    s16 count;

    if (destination == NULL) {
        return;
    }
    count = destination->count;
    if (count > 0) {
        memcpy(destination->entries, source->entries, count * 16);
    }
}
