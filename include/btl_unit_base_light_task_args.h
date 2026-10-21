#ifndef BTL_UNIT_BASE_LIGHT_TASK_ARGS_H
#define BTL_UNIT_BASE_LIGHT_TASK_ARGS_H

#include "common.h"

struct BtlUnit;

/* Eight-byte callback arguments allocated by btlCreateUnitBaseLightTask. */
typedef struct BtlUnitBaseLightArgs {
    struct BtlUnit *unit;
    s32 delay;
} BtlUnitBaseLightArgs;

typedef char BtlUnitBaseLightArgs_size[(sizeof(BtlUnitBaseLightArgs) == 0x08) ? 1 : -1];
typedef char BtlUnitBaseLightArgs_unit_offset[((u32)&((BtlUnitBaseLightArgs *)0)->unit == 0x00) ? 1 : -1];
typedef char BtlUnitBaseLightArgs_delay_offset[((u32)&((BtlUnitBaseLightArgs *)0)->delay == 0x04) ? 1 : -1];

#endif
