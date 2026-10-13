#ifndef BTL_FLOOR_LOAD_TASK_ARGS_H
#define BTL_FLOOR_LOAD_TASK_ARGS_H

#include "common.h"

/* Complete 0x14-byte payload allocated by the paired floor-load task creators. */
typedef struct BtlFloorLoadArgs {
    s32 frontHandle; /* 0x00: pending front-floor file request */
    s32 sideHandle;  /* 0x04: pending side-floor file request */
    s32 stage;       /* 0x08: stage used to form both resource paths */
    s32 variant;     /* 0x0C: floor variant used to form both paths */
    s32 state;       /* 0x10: callback phase */
} BtlFloorLoadArgs;

typedef char BtlFloorLoadArgs_size[(sizeof(BtlFloorLoadArgs) == 0x14) ? 1 : -1];
typedef char BtlFloorLoadArgs_frontHandle_offset[((unsigned long)&((BtlFloorLoadArgs *)0)->frontHandle == 0x00) ? 1 : -1];
typedef char BtlFloorLoadArgs_sideHandle_offset[((unsigned long)&((BtlFloorLoadArgs *)0)->sideHandle == 0x04) ? 1 : -1];
typedef char BtlFloorLoadArgs_stage_offset[((unsigned long)&((BtlFloorLoadArgs *)0)->stage == 0x08) ? 1 : -1];
typedef char BtlFloorLoadArgs_variant_offset[((unsigned long)&((BtlFloorLoadArgs *)0)->variant == 0x0C) ? 1 : -1];
typedef char BtlFloorLoadArgs_state_offset[((unsigned long)&((BtlFloorLoadArgs *)0)->state == 0x10) ? 1 : -1];

#endif
