#ifndef EVT_MESSAGE_WINDOW_API_H
#define EVT_MESSAGE_WINDOW_API_H

#include "common.h"

struct ItfMesSub;

s32 evtCreateMessageWindowIfMissing(struct ItfMesSub *definition);

u32 evtFinishMessageWindowAndNotify(void);

/* Returns the signed control-state byte as s32. Returns 0 when no window is
 * open, or when a nonzero state gate hides control state 2. This is a state
 * value, not a boolean; other stored values pass through. */
s32 evtGetMessageWindowControlState(void);

s32 evtSetBoundedDisplayValue(s32 displayValueIndex, s32 displayValue);
/* Store the opaque value and snapshot the panel field when a window is open.
 * Returns 1 on success, or 0 when no window is open. */
s32 evtStoreValueAndCaptureWindowPanelValue(s32 requestedValue);
void evtSetMessageWindowOptionWhenOpen(s32 option);

#endif
