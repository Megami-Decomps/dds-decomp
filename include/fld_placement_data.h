#ifndef FLD_PLACEMENT_DATA_H
#define FLD_PLACEMENT_DATA_H

#include "common.h"

/* Type-10 placement records contain a four-word serialized header. */
typedef struct FldPlacementData {
    u32 kind;            /* 0x00 */
    s32 event;            /* 0x04 */
    u8 opaque08[4];
    void *parameters;     /* 0x0C */
} FldPlacementData;

typedef char FldPlacementDataSizeCheck[
    sizeof(FldPlacementData) == 0x10 ? 1 : -1];

#endif
