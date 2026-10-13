#ifndef BTL_AFFINITY_ROW_H
#define BTL_AFFINITY_ROW_H

#include "common.h"

typedef struct BtlAffinityRow {
    s32 value[19];
} BtlAffinityRow;

typedef char BtlAffinityRow_size_must_be_0x4C[
    (sizeof(BtlAffinityRow) == 0x4C) ? 1 : -1];
typedef char BtlAffinityRow_value_offset_must_be_0x00[
    ((u32)&((BtlAffinityRow *)0)->value == 0x00) ? 1 : -1];

#endif
