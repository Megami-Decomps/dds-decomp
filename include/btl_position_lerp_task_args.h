#ifndef BTL_POSITION_LERP_TASK_ARGS_H
#define BTL_POSITION_LERP_TASK_ARGS_H

#include "common.h"

struct BtlUnit;

/* Complete 0x30-byte payload allocated by the position-lerp task creators. */
typedef struct BtlPosLerpTaskArgs {
    s128 from;             /* 0x00: interpolation start position */
    s128 to;               /* 0x10: interpolation target position */
    f32 rate;              /* 0x20: interpolation rate */
    f32 t;                 /* 0x24: current interpolation fraction */
    s32 count;             /* 0x28: callback invocation count */
    struct BtlUnit *unit;  /* 0x2C: actor whose position is updated */
} BtlPosLerpTaskArgs;

typedef char BtlPosLerpTaskArgs_size[(sizeof(BtlPosLerpTaskArgs) == 0x30) ? 1 : -1];
typedef char BtlPosLerpTaskArgs_from_offset[((unsigned long)&((BtlPosLerpTaskArgs *)0)->from == 0x00) ? 1 : -1];
typedef char BtlPosLerpTaskArgs_to_offset[((unsigned long)&((BtlPosLerpTaskArgs *)0)->to == 0x10) ? 1 : -1];
typedef char BtlPosLerpTaskArgs_rate_offset[((unsigned long)&((BtlPosLerpTaskArgs *)0)->rate == 0x20) ? 1 : -1];
typedef char BtlPosLerpTaskArgs_t_offset[((unsigned long)&((BtlPosLerpTaskArgs *)0)->t == 0x24) ? 1 : -1];
typedef char BtlPosLerpTaskArgs_count_offset[((unsigned long)&((BtlPosLerpTaskArgs *)0)->count == 0x28) ? 1 : -1];
typedef char BtlPosLerpTaskArgs_unit_offset[((unsigned long)&((BtlPosLerpTaskArgs *)0)->unit == 0x2C) ? 1 : -1];

#endif
