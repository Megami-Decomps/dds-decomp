#ifndef BTL_COMMAND_STATUS_API_H
#define BTL_COMMAND_STATUS_API_H

#include "btl_action_press_result.h"

struct BtlUnit;

/* 32-bit masks of status bits applied to a party record during action resolution. */
typedef u32 BtlPartyStatusMask;

/* Status bits the command adds to the target's party record. */
#if defined(VERSION_DDS1)
BtlPartyStatusMask btlGetCommandAddedStatus(
    struct BtlUnit *attacker, struct BtlUnit *target, s32 command,
    BtlActionPressResult pressResult, s32 mode);
#elif defined(VERSION_DDS2)
BtlPartyStatusMask btlGetCommandAddedStatus(
    struct BtlUnit *attacker, struct BtlUnit *target, u32 command,
    BtlActionPressResult pressResult, s32 mode);
#else
#error "btl_command_status_api.h requires a DDS title version"
#endif

/* Status bits to clear from the target's party record. */
BtlPartyStatusMask btlQueryUnitChannelFlags(
    struct BtlUnit *source, struct BtlUnit *target, s32 command,
    BtlActionPressResult pressResult, s32 kind);

#endif /* BTL_COMMAND_STATUS_API_H */
