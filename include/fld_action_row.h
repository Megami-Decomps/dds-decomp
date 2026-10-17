#ifndef FLD_ACTION_ROW_H
#define FLD_ACTION_ROW_H

#include "fld.h"
#include "fld_trigger_shape.h"

typedef struct FldActionTransformFields {
    f32 position[4];
    f32 rotation[4];
    f32 scale[4];
} FldActionTransformFields;

typedef union FldActionTransform {
    FldActionTransformFields fields;
    struct {
        f32 position[4];
        f32 rotation[4];
        f32 scale[4];
    };
    f32 values[12];
} FldActionTransform;

typedef struct FldActionRow {
    union {
        struct {
            FldTriggerSphere shape;
            FldTaskInfo info;
        } task;
        FldTriggerShape shape;
    } prefix;
    FldActionTransform transform;
    FldFileResource childResource;
    FldFileResource displayResource;
} FldActionRow;

typedef char FldActionTransform_size_must_be_0x30[
    (sizeof(FldActionTransform) == 0x30) ? 1 : -1];
typedef char FldActionTransformFields_size_must_be_0x30[
    (sizeof(FldActionTransformFields) == 0x30) ? 1 : -1];
typedef char FldActionTransform_position_offset_must_be_0[
    ((u32)&((FldActionTransform *)0)->position == 0) ? 1 : -1];
typedef char FldActionTransform_rotation_offset_must_be_0x10[
    ((u32)&((FldActionTransform *)0)->rotation == 0x10) ? 1 : -1];
typedef char FldActionTransform_scale_offset_must_be_0x20[
    ((u32)&((FldActionTransform *)0)->scale == 0x20) ? 1 : -1];
typedef char FldActionTransform_values_offset_must_be_0[
    ((u32)&((FldActionTransform *)0)->values == 0) ? 1 : -1];

typedef char FldActionRow_size_must_be_0x90[
    (sizeof(FldActionRow) == 0x90) ? 1 : -1];
typedef char FldActionRow_prefix_offset_must_be_0[
    ((u32)&((FldActionRow *)0)->prefix == 0) ? 1 : -1];
typedef char FldActionRow_transform_offset_must_be_0x18[
    ((u32)&((FldActionRow *)0)->transform == 0x18) ? 1 : -1];
typedef char FldActionRow_child_resource_offset_must_be_0x48[
    ((u32)&((FldActionRow *)0)->childResource == 0x48) ? 1 : -1];
typedef char FldActionRow_display_resource_offset_must_be_0x6c[
    ((u32)&((FldActionRow *)0)->displayResource == 0x6C) ? 1 : -1];

#endif
