#ifndef BTL_COMMAND_BLOCK_REASON_H
#define BTL_COMMAND_BLOCK_REASON_H

#include "common.h"
#include "btl_command_id.h"

typedef s32 BtlCommandBlockReason;

enum {
    BTL_COMMAND_BLOCK_NONE = 0,
    BTL_COMMAND_BLOCK_NO_RESTRICTED_UNIT = 2,
};

#if defined(VERSION_DDS1)
enum {
    BTL_COMMAND_BLOCK_EVENT_61_NOT_ACTIVE = 6,
    BTL_COMMAND_BLOCK_EMPTY_OR_ALL_FLAGGED_TARGETS = 9,
};
#elif defined(VERSION_DDS2)
enum {
    BTL_COMMAND_BLOCK_EMPTY_OR_ALL_FLAGGED_TARGETS = 10,
};
#endif

#ifdef VERSION_DDS1
struct BtlTask;
BtlCommandBlockReason btlGetCommandBlockReason(
    struct BtlTask *actionTask, BtlCommandId commandId);
#elif defined(VERSION_DDS2)
struct ActionStateLink;
BtlCommandBlockReason btlGetCommandBlockReason(
    struct ActionStateLink *actionTask, BtlCommandId commandId);
#endif

#endif /* BTL_COMMAND_BLOCK_REASON_H */
