#ifndef PTY_FRONTLINE_PROMOTION_API_H
#define PTY_FRONTLINE_PROMOTION_API_H

#include "common.h"

/* Whether a roster entry moved into the frontline. */
typedef s32 PtyFrontlinePromotionResult;
enum {
    PTY_FRONTLINE_PROMOTION_NOT_PERFORMED = 0,
    PTY_FRONTLINE_PROMOTION_PERFORMED = 1,
};

PtyFrontlinePromotionResult ptyRebalanceFrontline(s32 unitId);

#endif /* PTY_FRONTLINE_PROMOTION_API_H */
