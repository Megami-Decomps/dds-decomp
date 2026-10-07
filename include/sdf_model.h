#ifndef SDF_MODEL_H
#define SDF_MODEL_H

#include "sdf_draw.h"

/* Native model constructor reference: AMB roots and the FLD model wrapper's
 * four-word subrecord. The item list has a variable-length private body. */
typedef struct SdfItemList SdfItemList;
typedef struct SdfItemListRef {
    SdfItemList *items; /* 0x00 */
    void *assets;      /* 0x04: serialized material/asset list */
    s32 slotPairCount; /* 0x08: passed to sdfModelAllocateSlotPairs */
    u32 parameter;     /* 0x0C: retained at SdfModel.unk90 */
} SdfItemListRef;

typedef char SdfItemListRef_size_must_be_0x10[(sizeof(SdfItemListRef) == 0x10) ? 1 : -1];
typedef char SdfItemListRef_assets_at_4[((u32)&((SdfItemListRef *)0)->assets == 4) ? 1 : -1];
typedef char SdfItemListRef_slotPairCount_at_8[((u32)&((SdfItemListRef *)0)->slotPairCount == 8) ? 1 : -1];
typedef char SdfItemListRef_parameter_at_C[((u32)&((SdfItemListRef *)0)->parameter == 0xC) ? 1 : -1];

SdfModel *sdfModelCreateFromAssetData(void *data, SdfItemListRef *listRef);
SdfModel *sdfModelCreateWithItems(void *data, SdfItemListRef *listRef);
SdfModel *sdfModelCreateWithAlternateItems(void *data, SdfItemListRef *listRef);

#endif /* SDF_MODEL_H */
