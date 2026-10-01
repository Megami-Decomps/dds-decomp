#include "common.h"
#include "pcp_vu0.h"

extern void *sdfInitNodeHeaderFromWords(s32 arg0, s32 arg1, s32 arg2);
extern void *func_002CFEB8(s32 arg0);
extern void *sdfDevCreateBufferedRequest(s32 arg0, s32 arg1, s32 arg2);
extern void func_002EFD30(s32 arg0);
extern void *memcpy(void *dst, const void *src, u32 n);
extern void sdfFreeNodeLists(void);
extern void sdfEnsureFreeRootWorkspace(void *arg0);
extern void func_002D83F8(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void func_002D86E0(void *arg0, void *arg1);
extern void func_002DDC50(void);
extern void func_002E1FF8(u32 arg0);
extern vu8 D_003BD2EA;

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

typedef struct {
    u8 pad_0x00[0x18];
    u32 unk18;
} SdfEntry;

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
    u8 transformStart; /* 0x20: COP2 reads 0x40 bytes across following fields */
    u8 pad_0x21[0x07];
    struct SdfNode *nodes[2]; /* 0x28: per-slot node lists */
    s32 packetAddressBase; /* 0x30: base of 128-byte indexed address packets */
    u8 pad_0x34[0x04]; /* 0x34 */
    s32 unk38;         /* 0x38 */
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
    s32 unk18;                    /* 0x18 */
    u8 pad1C[0x14];
    u32 address;                  /* 0x30 */
    s32 unk34;                    /* 0x34 */
    void *unk38;                  /* 0x38: item this node was built from */
    u8 pad3C[0x14];
    u8 quaternion[0x10];          /* 0x50 */
    u8 vectors[5][0x10];         /* 0x60-0xAF: COP2 inputs */
    u8 padB0[0x10];
    u8 transformed[0x40];        /* 0xC0: four COP2 output vectors */
} SdfDrawNode;

/* The indexed resource words are stored at offset 0x0C of the asset table. */
typedef struct SdfAssetTable {
    u8 pad_0x00[0x0C];
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
    void *unk0;       /* 0x0 */
    u8 unk4;          /* 0x4 */
    s8 unk5;          /* 0x5 */
    s16 unk6;         /* 0x6 */
    u8 pad_0x08[0x4]; /* 0x8 */
    s32 unkC;         /* 0xC */
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
    s32 unk8;         /* 0x08 */
    u8 pad0C[4];
    f32 x;            /* 0x10 */
    f32 y;            /* 0x14 */
    f32 z;            /* 0x18 */
    u8 pad1C[4];
    u128 vec20;       /* 0x20 */
    u128 vec30;       /* 0x30 */
    s32 unk40;        /* 0x40 */
} SdfItem;

extern SdfModel *sdfModelCreateFromAssetData(void *arg0, void *arg1);
extern void effMiscQuaternionToMatrixVU(void);
extern void func_002E7F20(f32 x, f32 y, f32 z);
void sdfDrawNodeBuildMatrix(SdfDrawNode *node);
void sdfDrawNodeSetFromItem(SdfDrawNode *node, SdfItem *item);


INCLUDE_ASM(const s32, "sdf/sdfModel", sdfModelFindDrawNode);

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

void sdfDrawNodeBuildMatrix(SdfDrawNode *node) {
    VU0_LOAD_VF(vf10, node->quaternion);
    effMiscQuaternionToMatrixVU();
    VU0_LOAD_VF(vf31, node->vectors[0]);
    VU0_STORE_MATRIX(node->vectors[2]);
}

void sdfDrawNodeSetFromItem(SdfDrawNode *node, SdfItem *item) {
    node->unk38 = item;
    node->unk18 = item->unk8;
    func_002E7F20(item->x, item->y, item->z);
    VU0_STORE_VF(vf10, node->quaternion);
    PCP_COPY_VECTOR(node->vectors[0], &item->vec20);
    PCP_COPY_VECTOR(node->vectors[1], &item->vec30);
    ((f32 *)node->vectors[0])[3] = 1.0f;
    sdfDrawNodeBuildMatrix(node);
    node->unk34 = item->unk40;
}

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D86E0);

/* Reset the node lists and initialize both per-model slots. */
void sdfModelResetAndInitNodes(SdfModel *model, s32 arg1, s32 arg2) {
    s32 i = 0;
    s32 j = 0;

    model->unk38 = arg1;
    sdfFreeNodeLists();
    sdfEnsureFreeRootWorkspace(model);
    sdfDrawNodeBuildMatrix((SdfDrawNode *)model);
    /* The re-initialization below is load-bearing for a byte-identical build. */
    i = 0;
    j = 0;
    do {
        i++;
        func_002D83F8(model, arg1, arg2, 0, j);
        j = i;
    } while (i != 2);
}

INCLUDE_ASM(const s32, "sdf/sdfModel", func_002D8890);

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

    model->unk19 |= 4;
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
    u8 *xAxis = drawNode->vectors[2];
    u8 *yAxis;
    u8 *zAxis;
    u8 *scale;
    u8 *translation;
    u8 *transformed;
    u32 address;
    SdfDrawNode *child;

        VU0_LOAD_VF_MEMORY(vf28, xAxis);
    yAxis = drawNode->vectors[3];
        VU0_LOAD_VF_MEMORY(vf29, yAxis);
    zAxis = drawNode->vectors[4];
        VU0_LOAD_VF_MEMORY(vf30, zAxis);
    scale = drawNode->vectors[1];
        VU0_LOAD_VF_MEMORY(vf10, scale);
    VU0_SCALE_MATRIX_ROWS(vf10);
    translation = drawNode->vectors[0];
    VU0_LOAD_VF_MEMORY(vf31, translation);
    VU0_LOAD_MATRIX_B(parentMatrix);
    func_002DDC50();
    transformed = drawNode->transformed;
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
    sdfModelUpdateRootTransforms(model, (s8)D_003BD2EA);
}

/* Store four message words, then notify the consumer of the second word. */
void func_002D8C88(SdfMsg *msg, s32 unk0, s32 unk4, s32 unk8, s32 unkC) {
    msg->unkC = unkC;
    msg->unk0 = unk0;
    msg->unk8 = unk8;
    msg->unk4 = unk4;
    func_002EFD30(unk4);
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
