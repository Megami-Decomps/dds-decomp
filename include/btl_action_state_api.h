#ifndef BTL_ACTION_STATE_API_H
#define BTL_ACTION_STATE_API_H

#include "btl_task.h"

/* The action-state table is indexed by the state stored on its owner record. */
typedef s32 BtlActionStateIndex;

#ifdef VERSION_DDS1
typedef BtlTask BtlActionStateOwner;
#else
typedef ActionStateLink BtlActionStateOwner;
#endif

void btlDispatchStateHandler(void *owner, BtlActionStateIndex index);

#endif
