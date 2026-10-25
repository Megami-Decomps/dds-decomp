#ifndef EVT_MESSAGE_WINDOW_API_H
#define EVT_MESSAGE_WINDOW_API_H

#include "common.h"

struct ItfMesSub;

s32 fclInitMessage(struct ItfMesSub *definition);

u32 evtFinishMessageWindowAndNotify(void);

/* Returns the signed control-state byte as s32. Returns 0 when no window is
 * open, or when a nonzero state gate hides control state 2. This is a state
 * value, not a boolean; other stored values pass through. */
s32 fclChkMessage(void);

s32 evtSetBoundedDisplayValue(s32 displayValueIndex, s32 displayValue);
/* Store the opaque value and snapshot the panel field when a window is open.
 * Returns 1 on success, or 0 when no window is open. */
s32 evtStoreValueAndCaptureWindowPanelValue(s32 requestedValue);

typedef s32 EvtMessageWindowPageValueResult;

#define EVT_MESSAGE_WINDOW_PAGE_VALUE_NO_WINDOW 0
#define EVT_MESSAGE_WINDOW_PAGE_VALUE_UPDATED 1

/* Apply page mode zero and the caller's page value when a window is open.
 * Returns UPDATED when applied, or NO_WINDOW when no window is open. */
EvtMessageWindowPageValueResult evtSetMessageWindowPageValue(s32 pageValue);

void evtSetMessageWindowOptionWhenOpen(s32 option);

#endif
