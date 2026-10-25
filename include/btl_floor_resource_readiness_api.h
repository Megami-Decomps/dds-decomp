#ifndef BTL_FLOOR_RESOURCE_READINESS_API_H
#define BTL_FLOOR_RESOURCE_READINESS_API_H

#include "common.h"

/* Scene setup waits until both retained front- and side-floor resources exist. */
typedef s32 BtlFloorResourceReadiness;
enum {
    BTL_FLOOR_RESOURCES_NOT_READY = 0,
    BTL_FLOOR_RESOURCES_READY = 1,
};

BtlFloorResourceReadiness btlAreFrontAndSideFloorResourcesReady(void);

#endif /* BTL_FLOOR_RESOURCE_READINESS_API_H */
