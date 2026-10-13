#ifndef BTL_APPROACH_TASK_ARGS_H
#define BTL_APPROACH_TASK_ARGS_H

#include "common.h"

struct BtlUnit;

/* Cleared task argument block for moving one unit toward a selected target. */
typedef struct BtlApproachTaskArgs {
    struct BtlUnit *unit;   /* 0x00: unit being moved */
    struct BtlUnit *target; /* 0x04: selected target unit */
    f32 offset;             /* 0x08: target reach offset, initialized on first tick */
    f32 scale;              /* 0x0C: interpolation scale, with zero selecting 1.0 */
    u32 opaque10;            /* 0x10: cleared by allocation; not accessed by callback */
    s32 count;               /* 0x14: callback frame count */
} BtlApproachTaskArgs;

typedef char BtlApproachTaskArgsSizeCheck[
    (sizeof(BtlApproachTaskArgs) == 0x18) ? 1 : -1];
typedef char BtlApproachTaskArgsUnitOffsetCheck[
    ((u32)&((BtlApproachTaskArgs *)0)->unit == 0x00) ? 1 : -1];
typedef char BtlApproachTaskArgsTargetOffsetCheck[
    ((u32)&((BtlApproachTaskArgs *)0)->target == 0x04) ? 1 : -1];
typedef char BtlApproachTaskArgsOffsetOffsetCheck[
    ((u32)&((BtlApproachTaskArgs *)0)->offset == 0x08) ? 1 : -1];
typedef char BtlApproachTaskArgsScaleOffsetCheck[
    ((u32)&((BtlApproachTaskArgs *)0)->scale == 0x0C) ? 1 : -1];
typedef char BtlApproachTaskArgsOpaqueOffsetCheck[
    ((u32)&((BtlApproachTaskArgs *)0)->opaque10 == 0x10) ? 1 : -1];
typedef char BtlApproachTaskArgsCountOffsetCheck[
    ((u32)&((BtlApproachTaskArgs *)0)->count == 0x14) ? 1 : -1];

#endif /* BTL_APPROACH_TASK_ARGS_H */
