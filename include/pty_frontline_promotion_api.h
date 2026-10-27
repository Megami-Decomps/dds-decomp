#ifndef PTY_FRONTLINE_PROMOTION_API_H
#define PTY_FRONTLINE_PROMOTION_API_H

#include "common.h"
#include "dat_unit_id.h"

/* Whether a roster entry moved into the frontline. */
typedef s32 PtyFrontlinePromotionResult;
enum {
    PTY_FRONTLINE_PROMOTION_NOT_PERFORMED = 0,
    PTY_FRONTLINE_PROMOTION_PERFORMED = 1,
};

PtyFrontlinePromotionResult ptyRebalanceFrontline(DatUnitId unitId);

#endif /* PTY_FRONTLINE_PROMOTION_API_H */
