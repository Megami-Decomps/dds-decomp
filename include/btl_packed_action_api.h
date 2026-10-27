#ifndef BTL_PACKED_ACTION_API_H
#define BTL_PACKED_ACTION_API_H

#include "btl_task.h"
#include "btl_ai_scratch.h"

/* Evaluate a packed action for the current command actor and its owning unit.
 * The task pointer is retained in BtlAiScratchWork for the duration of dispatch. */
s32 btlDispatchPackedActionWithScratch(
    BtlAiScratchActor actorTask, BtlUnit *owner, s32 packedActionMask);

#endif /* BTL_PACKED_ACTION_API_H */
