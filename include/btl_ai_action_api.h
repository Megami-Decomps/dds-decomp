#ifndef BTL_AI_ACTION_API_H
#define BTL_AI_ACTION_API_H

#include "btl_task.h"

/* Execute the selected AI action for an action-state actor. */
s32 btlExecuteAiTableAction(ActionStateLink *task, u16 actionId, u32 actionArgument);

#endif /* BTL_AI_ACTION_API_H */
