#ifndef SDF_MODEL_H
#define SDF_MODEL_H

#include "sdf_draw.h"

/* Native model constructor reference: AMB roots and the FLD model wrapper's
 * four-word subrecord. The item list has a variable-length body of SdfItem
 * records beginning at the firstItem marker. */
typedef struct SdfItemList SdfItemList;
struct SdfItemList {
    s32 count;           /* 0x00: item count */
    u8 pad_0x04[0x0C];   /* 0x04 */
    u8 firstItem;        /* 0x10: first item, records have a 0x50 stride */
};

/* Serialized per-item data applied to a draw node by sdfDrawNodeSetFromItem. */
typedef struct SdfItem {
    u16 commandSetupMode; /* 0x00: selects the optional command-list form */
    u16 reserved02;       /* 0x02 */
    u32 unk04;            /* 0x04 */
    s32 nodeId;       /* 0x08 */
    s32 parentNodeIndex; /* 0x0C: negative means this node has no parent */
    f32 rotationX;    /* 0x10: Euler angles supplied to the quaternion builder */
    f32 rotationY;    /* 0x14 */
    f32 rotationZ;    /* 0x18 */
    u8 pad1C[4];
    u128 translation; /* 0x20 */
    u128 scale;       /* 0x30 */
    s32 boundsAddress; /* 0x40: optional local-box corners used by clipping */
    union {
        u32 inlineCommandAddresses[3]; /* mode 0 */
        struct {
            u32 *commandAddresses; /* mode 1: zero-terminated address words */
            u32 unk48;
            u32 unk4C;
        } commandList;
    } commandData; /* 0x44: other modes leave the command tail unused */
} SdfItem;

typedef char SdfItem_size_must_be_0x50[(sizeof(SdfItem) == 0x50) ? 1 : -1];

typedef struct SdfItemListRef {
    SdfItemList *items; /* 0x00 */
    void *assets;      /* 0x04: serialized material/asset list */
    s32 slotPairCount; /* 0x08: passed to sdfModelAllocateSlotPairs */
    u32 chunkTable;    /* 0x0C: retained at SdfModel.chunkTable */
} SdfItemListRef;

typedef char SdfItemListRef_size_must_be_0x10[(sizeof(SdfItemListRef) == 0x10) ? 1 : -1];
typedef char SdfItemListRef_assets_at_4[((u32)&((SdfItemListRef *)0)->assets == 4) ? 1 : -1];
typedef char SdfItemListRef_slotPairCount_at_8[((u32)&((SdfItemListRef *)0)->slotPairCount == 8) ? 1 : -1];
typedef char SdfItemListRef_chunkTable_at_C[((u32)&((SdfItemListRef *)0)->chunkTable == 0xC) ? 1 : -1];

SdfModel *sdfModelCreateFromAssetData(void *data, SdfItemListRef *listRef);
SdfModel *sdfModelCreateWithItems(void *data, SdfItemListRef *listRef);
SdfModel *sdfModelCreateWithAlternateItems(void *data, SdfItemListRef *listRef);

#endif /* SDF_MODEL_H */
