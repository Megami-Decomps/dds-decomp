#ifndef BTL_DEFERRED_UNIT_STATUS_ARGS_H
#define BTL_DEFERRED_UNIT_STATUS_ARGS_H

#include "common.h"

struct BtlUnit;

typedef struct BtlDeferredUnitStatusArgs {
    struct BtlUnit *unit;
    u32 minimumValue;
} BtlDeferredUnitStatusArgs;

typedef char BtlDeferredUnitStatusArgsSizeCheck[
    sizeof(BtlDeferredUnitStatusArgs) == 8 ? 1 : -1];
typedef char BtlDeferredUnitStatusArgsUnitOffsetCheck[
    (u32)&((BtlDeferredUnitStatusArgs *)0)->unit == 0 ? 1 : -1];
typedef char BtlDeferredUnitStatusArgsMinimumValueOffsetCheck[
    (u32)&((BtlDeferredUnitStatusArgs *)0)->minimumValue == 4 ? 1 : -1];

#endif /* BTL_DEFERRED_UNIT_STATUS_ARGS_H */
