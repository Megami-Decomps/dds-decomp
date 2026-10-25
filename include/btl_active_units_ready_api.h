#ifndef BTL_ACTIVE_UNITS_READY_API_H
#define BTL_ACTIVE_UNITS_READY_API_H

#include "common.h"

typedef s32 BtlActiveUnitsReadiness;
enum {
    BTL_ACTIVE_UNITS_NOT_READY = 0,
    BTL_ACTIVE_UNITS_READY = 1,
};

BtlActiveUnitsReadiness btlAllActiveUnitsReady(void);

#endif /* BTL_ACTIVE_UNITS_READY_API_H */
