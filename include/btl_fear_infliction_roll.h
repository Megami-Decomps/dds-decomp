#ifndef BTL_FEAR_INFLICTION_ROLL_H
#define BTL_FEAR_INFLICTION_ROLL_H

#include "btl_action_press_result.h"

struct BtlUnit;

/* Success authorizes the hit executor to add fear; failure includes rejected gates. */
typedef s32 BtlFearInflictionRollResult;
enum {
    BTL_FEAR_INFLICTION_ROLL_FAILED = 0,
    BTL_FEAR_INFLICTION_ROLL_SUCCEEDED = 1
};

#if defined(VERSION_DDS1)
BtlFearInflictionRollResult btlRollFearChance(
    s32 unused, struct BtlUnit *unit, u32 flags,
    BtlActionPressResult pressFlags);
#elif defined(VERSION_DDS2)
BtlFearInflictionRollResult btlRollFearChance(
    s32 unused, struct BtlUnit *unit, s32 flags,
    BtlActionPressResult pressFlags);
#else
#error "btl_fear_infliction_roll.h requires a DDS title version"
#endif

#endif /* BTL_FEAR_INFLICTION_ROLL_H */
