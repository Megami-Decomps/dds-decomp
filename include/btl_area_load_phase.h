#ifndef BTL_AREA_LOAD_PHASE_H
#define BTL_AREA_LOAD_PHASE_H

#include "common.h"

/* Signed word stored by the battle scene while a field area load advances. */
typedef s32 BtlAreaLoadPhase;

enum {
    BTL_AREA_LOAD_PHASE_START = 0,
    BTL_AREA_LOAD_PHASE_POLLING = 1,
    BTL_AREA_LOAD_PHASE_COMPLETE = 2,
    BTL_AREA_LOAD_PHASE_COUNT = 3
};

typedef char BtlAreaLoadPhaseSizeCheck[
    (sizeof(BtlAreaLoadPhase) == 4) ? 1 : -1];

#endif
