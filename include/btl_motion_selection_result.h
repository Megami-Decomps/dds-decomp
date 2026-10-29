#ifndef BTL_MOTION_SELECTION_RESULT_H
#define BTL_MOTION_SELECTION_RESULT_H

#include "common.h"

/* Nonnegative results select a motion-table slot; -1 keeps the current motion. */
typedef s32 BtlMotionSelectionResult;
enum {
    BTL_MOTION_SELECTION_KEEP_CURRENT = -1,
};

#endif /* BTL_MOTION_SELECTION_RESULT_H */
