#ifndef BTL_ACTION_PRESS_RESULT_H
#define BTL_ACTION_PRESS_RESULT_H

#include "common.h"

struct BtlUnit;

/* A hit's press result is one flag; a target group ORs these flags together. */
typedef s32 BtlActionPressResult;

enum {
    BTL_ACTION_PRESS_RESULT_NONE = 0,     /* No hit outcome has been accumulated. */
    BTL_ACTION_PRESS_RESULT_NORMAL = 1,   /* Ordinary non-critical result. */
    BTL_ACTION_PRESS_RESULT_CRITICAL = 2, /* Critical result. */
    BTL_ACTION_PRESS_RESULT_WEAKNESS = 4, /* The target's affinity is weak. */
};

BtlActionPressResult btlDetermineActionPressResult(
    struct BtlUnit *attacker, struct BtlUnit *target, u32 command);

#endif /* BTL_ACTION_PRESS_RESULT_H */
