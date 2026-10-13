#ifndef FLD_BILL_CONFIG_H
#define FLD_BILL_CONFIG_H

#include "common.h"

/* Shared FLD1 type-11 billboard descriptor. The final two words have no
 * confirmed interpretation and remain opaque. */
typedef struct BillConfig {
    u32 flags;
    u32 kind;
    u32 resourceIndex;
    f32 width;
    f32 height;
    f32 depth;
    u32 projectionDistance;
    s32 mapSelector;
    union {
        u32 entry;
        u32 fadeStart;
    };
    u32 fadeEnd;
    u8 opaque28[8];
} BillConfig;

typedef char BillConfig_size_must_be_0x30[
    (sizeof(BillConfig) == 0x30) ? 1 : -1
];
typedef char BillConfig_resourceIndex_at_0x08[
    ((u32)&((BillConfig *)0)->resourceIndex == 0x08) ? 1 : -1
];
typedef char BillConfig_width_at_0x0C[
    ((u32)&((BillConfig *)0)->width == 0x0C) ? 1 : -1
];
typedef char BillConfig_projectionDistance_at_0x18[
    ((u32)&((BillConfig *)0)->projectionDistance == 0x18) ? 1 : -1
];
typedef char BillConfig_mapSelector_at_0x1C[
    ((u32)&((BillConfig *)0)->mapSelector == 0x1C) ? 1 : -1
];
typedef char BillConfig_entry_at_0x20[
    ((u32)&((BillConfig *)0)->entry == 0x20) ? 1 : -1
];
typedef char BillConfig_fadeEnd_at_0x24[
    ((u32)&((BillConfig *)0)->fadeEnd == 0x24) ? 1 : -1
];
typedef char BillConfig_opaque28_at_0x28[
    ((u32)&((BillConfig *)0)->opaque28 == 0x28) ? 1 : -1
];

#endif
