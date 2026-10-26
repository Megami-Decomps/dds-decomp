#ifndef BTL_COMMAND_USE_ERROR_H
#define BTL_COMMAND_USE_ERROR_H

#include "common.h"

struct BtlUnit;

typedef s32 BtlCommandUseError;

/* Zero allows use; nonzero values identify why the command is blocked. */
enum {
    BTL_COMMAND_USE_ERROR_NONE = 0,
    BTL_COMMAND_USE_ERROR_INSUFFICIENT_HP = 1,
    BTL_COMMAND_USE_ERROR_INSUFFICIENT_MP = 2,
    BTL_COMMAND_USE_ERROR_ACTOR_STATE = 3,
    BTL_COMMAND_USE_ERROR_LINKAGE_STATE = 4,
    BTL_COMMAND_USE_ERROR_SELECTOR_STATE = 5,
    BTL_COMMAND_USE_ERROR_INSUFFICIENT_SCENE_SLOTS = 6,
    /* DDS2-only actor/command restriction. */
    BTL_COMMAND_USE_ERROR_RESTRICTED_ACTOR_COMMAND = 7,
};

BtlCommandUseError btlGetCommandFailureReason(struct BtlUnit *unit, s32 command);
BtlCommandUseError btlGetLinkedPartyCommandFailureReason(
    struct BtlUnit *base, struct BtlUnit *first, struct BtlUnit *second,
    struct BtlUnit *third, s32 command);
BtlCommandUseError btlGetCommandUseError(struct BtlUnit *unit, s32 command);
BtlCommandUseError btlGetPartnerCommandUseError(
    struct BtlUnit *base, struct BtlUnit *first, struct BtlUnit *second,
    struct BtlUnit *third, s32 command);

#endif /* BTL_COMMAND_USE_ERROR_H */
