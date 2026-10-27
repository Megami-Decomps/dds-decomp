#ifndef BTL_ACTOR_COMMAND_SELECTOR_VALUE_API_H
#define BTL_ACTOR_COMMAND_SELECTOR_VALUE_API_H

#include "common.h"
#include "btl_command_id.h"

struct BtlUnit;

/* Reads the command selector stat, with enemy command zero using its record override. */
s8 btlGetActorIndexedSignedValue(const struct BtlUnit *actor, BtlCommandId commandId);

#endif /* BTL_ACTOR_COMMAND_SELECTOR_VALUE_API_H */
