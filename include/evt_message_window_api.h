#ifndef EVT_MESSAGE_WINDOW_API_H
#define EVT_MESSAGE_WINDOW_API_H

#include "common.h"

u32 evtFinishMessageWindowAndNotify(void);

/* Returns the signed control-state byte as s32. Returns 0 when no window is
 * open, or when a nonzero state gate hides control state 2. This is a state
 * value, not a boolean; other stored values pass through. */
s32 evtGetMessageWindowControlState(void);

#endif
