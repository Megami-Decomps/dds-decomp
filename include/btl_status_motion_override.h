#ifndef BTL_STATUS_MOTION_OVERRIDE_H
#define BTL_STATUS_MOTION_OVERRIDE_H

#include "common.h"

struct BtlUnit;

/* A nonnegative result replaces the selected status motion; -1 keeps it. */
typedef s32 BtlStatusMotionOverrideResult;
enum {
    BTL_STATUS_MOTION_OVERRIDE_NONE = -1,
};

typedef BtlStatusMotionOverrideResult (*BtlStatusMotionOverrideCallback)(
    struct BtlUnit *unit);

#endif /* BTL_STATUS_MOTION_OVERRIDE_H */
