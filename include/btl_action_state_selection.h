#ifndef BTL_ACTION_STATE_SELECTION_H
#define BTL_ACTION_STATE_SELECTION_H

#include "btl_task.h"

/* A mode hook selects an action-state handler or defers to the usual path. */
typedef s32 BtlActionStateSelectionResult;
#define BTL_ACTION_STATE_SELECTION_USE_DEFAULT ((BtlActionStateSelectionResult)-1)

#ifdef VERSION_DDS1
typedef BtlActionStateSelectionResult (*BtlActionStateSelectionHook)(BtlTask *);
#else
typedef BtlActionStateSelectionResult (*BtlActionStateSelectionHook)(struct ActionStateLink *);
#endif

#endif
