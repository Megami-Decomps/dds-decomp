#ifndef EFF_RESOURCE_LIST_REQUEST_H
#define EFF_RESOURCE_LIST_REQUEST_H

#include "common.h"

/* Entry view followed by the effect resource-list completion consumer. */
typedef struct EffLoadedItem {
    struct EffLoadedItem *next; /* 0x00 */
    u8 opaque04[4];             /* 0x04 */
    u32 allocationHandle;       /* 0x08: used as an SdfMemBlock handle. */
    u8 opaque0C[4];             /* 0x0C */
    u8 kind;                    /* 0x10: the consumer accepts kind 1. */
} EffLoadedItem;

/* Consumer-side view of the item chain embedded in a file request. */
typedef struct EffRequest {
    u8 opaque00[0x60];
    EffLoadedItem *items; /* 0x60 */
} EffRequest;

typedef char EffLoadedItem_size_must_be_14[(sizeof(EffLoadedItem) == 0x14) ? 1 : -1];
typedef char EffLoadedItem_allocationHandle_offset_must_be_08[
    ((u32)&((EffLoadedItem *)0)->allocationHandle == 0x08) ? 1 : -1];
typedef char EffLoadedItem_kind_offset_must_be_10[
    ((u32)&((EffLoadedItem *)0)->kind == 0x10) ? 1 : -1];
typedef char EffRequest_items_offset_must_be_60[
    ((u32)&((EffRequest *)0)->items == 0x60) ? 1 : -1];

#endif /* EFF_RESOURCE_LIST_REQUEST_H */
