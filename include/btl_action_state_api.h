#ifndef BTL_ACTION_STATE_API_H
#define BTL_ACTION_STATE_API_H

#include "btl_task.h"

/* Per-actor AI task has dispatched its initial action route. */
#define BTL_AI_TASK_FLAG_ACTION_DISPATCHED 0x80u

/* The action-state table is indexed by the state stored on its owner record. */
typedef s32 BtlActionStateIndex;

#ifdef VERSION_DDS1
typedef BtlTask BtlActionStateOwner;
#else
typedef ActionStateLink BtlActionStateOwner;
#endif

void btlDispatchStateHandler(void *owner, BtlActionStateIndex index);

#endif
