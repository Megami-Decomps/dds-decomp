#ifndef FLD_TRIGGER_SHAPE_H
#define FLD_TRIGGER_SHAPE_H

#include "common.h"

typedef struct FldTriggerSphere {
    u32 kind;
    f32 radius;
} FldTriggerSphere;

typedef struct FldTriggerPlane {
    u32 kind;
    f32 width;
    f32 height;
} FldTriggerPlane;

typedef struct FldTriggerBox {
    u32 kind;
    f32 size[3];
} FldTriggerBox;

/* Trigger data is selected by kind: the serialized variants are 8, 12, and
 * 16 bytes, with one, two, or three float words after kind. Runtime probes
 * read kind through this union before using it. */
typedef union FldTriggerShape {
    u32 kind;
    FldTriggerSphere sphere;
    FldTriggerPlane plane;
    FldTriggerBox box;
} FldTriggerShape;

/* The task callback data retains two opaque words around its slot and shape. */
typedef struct FldTaskInfo {
    s32 unk0;
    s32 slot;
    u32 unk08;
    FldTriggerShape *shape;
} FldTaskInfo;

typedef char FldTriggerSphereSizeCheck[(sizeof(FldTriggerSphere) == 0x08) ? 1 : -1];
typedef char FldTriggerSphereKindOffsetCheck[
    ((u32)&((FldTriggerSphere *)0)->kind == 0x00) ? 1 : -1];
typedef char FldTriggerSphereRadiusOffsetCheck[
    ((u32)&((FldTriggerSphere *)0)->radius == 0x04) ? 1 : -1];
typedef char FldTriggerPlaneSizeCheck[(sizeof(FldTriggerPlane) == 0x0C) ? 1 : -1];
typedef char FldTriggerPlaneKindOffsetCheck[
    ((u32)&((FldTriggerPlane *)0)->kind == 0x00) ? 1 : -1];
typedef char FldTriggerPlaneWidthOffsetCheck[
    ((u32)&((FldTriggerPlane *)0)->width == 0x04) ? 1 : -1];
typedef char FldTriggerPlaneHeightOffsetCheck[
    ((u32)&((FldTriggerPlane *)0)->height == 0x08) ? 1 : -1];
typedef char FldTriggerBoxSizeCheck[(sizeof(FldTriggerBox) == 0x10) ? 1 : -1];
typedef char FldTriggerBoxKindOffsetCheck[
    ((u32)&((FldTriggerBox *)0)->kind == 0x00) ? 1 : -1];
typedef char FldTriggerBoxSizeOffsetCheck[
    ((u32)&((FldTriggerBox *)0)->size == 0x04) ? 1 : -1];
typedef char FldTriggerShapeSizeCheck[(sizeof(FldTriggerShape) == 0x10) ? 1 : -1];
typedef char FldTriggerShapeKindOffsetCheck[
    ((u32)&((FldTriggerShape *)0)->kind == 0x00) ? 1 : -1];
typedef char FldTaskInfoSizeCheck[(sizeof(FldTaskInfo) == 0x10) ? 1 : -1];
typedef char FldTaskInfoUnk0OffsetCheck[
    ((u32)&((FldTaskInfo *)0)->unk0 == 0x00) ? 1 : -1];
typedef char FldTaskInfoSlotOffsetCheck[
    ((u32)&((FldTaskInfo *)0)->slot == 0x04) ? 1 : -1];
typedef char FldTaskInfoUnk08OffsetCheck[
    ((u32)&((FldTaskInfo *)0)->unk08 == 0x08) ? 1 : -1];
typedef char FldTaskInfoShapeOffsetCheck[
    ((u32)&((FldTaskInfo *)0)->shape == 0x0C) ? 1 : -1];

#endif /* FLD_TRIGGER_SHAPE_H */
