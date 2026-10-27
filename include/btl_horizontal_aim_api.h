#ifndef BTL_HORIZONTAL_AIM_API_H
#define BTL_HORIZONTAL_AIM_API_H

#include "common.h"

/* Reports whether horizontal aiming produced a directional quaternion.
 * A zero XZ delta loads the identity quaternion and returns NO_XZ_DELTA. */
typedef s32 BtlHorizontalAimResult;

enum {
    BTL_HORIZONTAL_AIM_NO_XZ_DELTA = 0,
    BTL_HORIZONTAL_AIM_DIRECTION_BUILT = 1,
};

BtlHorizontalAimResult btlAimHorizontalDirectionVU(
    const f32 *origin, const f32 *targetPosition);

#endif /* BTL_HORIZONTAL_AIM_API_H */
