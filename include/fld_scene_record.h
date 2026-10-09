#ifndef FLD_SCENE_RECORD_H
#define FLD_SCENE_RECORD_H

#include "common.h"
#include "sdf_model.h"

/* Relocated rows and nested records in the field scene-record resource. */
typedef struct FldPoint {
    f32 x;
    f32 y;
    f32 z;
} FldPoint;

typedef struct FldIcon {
    u32 kind;
    FldPoint *position;
} FldIcon;

typedef struct FldItem {
    char *name;
    u32 nodeIndex;
    FldIcon *icons;
    u32 iconCount;
    s32 floor;
    FldPoint *pointA;
    FldPoint *pointB;
} FldItem;

typedef struct FldSceneRecord {
    char *name;
    FldItem *items;
    u32 count;
    SdfItemListRef *model;
    FldPoint *pos;
} FldSceneRecord;

typedef char FldPoint_size_must_be_0xC[(sizeof(FldPoint) == 0xC) ? 1 : -1];
typedef char FldPoint_y_at_4[((u32)&((FldPoint *)0)->y == 4) ? 1 : -1];
typedef char FldPoint_z_at_8[((u32)&((FldPoint *)0)->z == 8) ? 1 : -1];

typedef char FldIcon_size_must_be_8[(sizeof(FldIcon) == 8) ? 1 : -1];
typedef char FldIcon_position_at_4[((u32)&((FldIcon *)0)->position == 4) ? 1 : -1];

typedef char FldItem_size_must_be_0x1C[(sizeof(FldItem) == 0x1C) ? 1 : -1];
typedef char FldItem_nodeIndex_at_4[((u32)&((FldItem *)0)->nodeIndex == 4) ? 1 : -1];
typedef char FldItem_icons_at_8[((u32)&((FldItem *)0)->icons == 8) ? 1 : -1];
typedef char FldItem_iconCount_at_C[((u32)&((FldItem *)0)->iconCount == 0xC) ? 1 : -1];
typedef char FldItem_floor_at_10[((u32)&((FldItem *)0)->floor == 0x10) ? 1 : -1];
typedef char FldItem_pointA_at_14[((u32)&((FldItem *)0)->pointA == 0x14) ? 1 : -1];
typedef char FldItem_pointB_at_18[((u32)&((FldItem *)0)->pointB == 0x18) ? 1 : -1];

typedef char FldSceneRecord_size_must_be_0x14[(sizeof(FldSceneRecord) == 0x14) ? 1 : -1];
typedef char FldSceneRecord_items_at_4[((u32)&((FldSceneRecord *)0)->items == 4) ? 1 : -1];
typedef char FldSceneRecord_count_at_8[((u32)&((FldSceneRecord *)0)->count == 8) ? 1 : -1];
typedef char FldSceneRecord_model_at_C[((u32)&((FldSceneRecord *)0)->model == 0xC) ? 1 : -1];
typedef char FldSceneRecord_pos_at_10[((u32)&((FldSceneRecord *)0)->pos == 0x10) ? 1 : -1];

extern FldSceneRecord *fldSceneRecords;
extern s32 fldSceneRecordCount;
extern s32 fldSceneRecordResource;

#endif /* FLD_SCENE_RECORD_H */
