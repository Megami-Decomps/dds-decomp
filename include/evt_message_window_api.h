#ifndef EVT_MESSAGE_WINDOW_API_H
#define EVT_MESSAGE_WINDOW_API_H

#include "common.h"
#include "itf_mes_entry_index.h"

struct ItfMesSub;

s32 fclInitMessage(struct ItfMesSub *definition);

u32 evtFinishMessageWindowAndNotify(void);

typedef s32 EvtMessageWindowControlResult;

#define EVT_MESSAGE_WINDOW_CONTROL_RESULT_NO_ACTIVE_CONTROL 0
#define EVT_MESSAGE_WINDOW_CONTROL_RESULT_ACTIVE 1
#define EVT_MESSAGE_WINDOW_CONTROL_RESULT_GATED 2
#define EVT_MESSAGE_WINDOW_CONTROL_RESULT_CLEANUP 3

/* Return the signed control-state byte as a query result, not a boolean or a
 * stored phase. NO_ACTIVE_CONTROL (0) covers a missing window and control state
 * two hidden by the gate; stored zero also passes through. Visible stored states
 * one, two and three report ACTIVE, GATED and CLEANUP. Other stored values pass
 * through unchanged. */
EvtMessageWindowControlResult fclChkMessage(void);

s32 evtSetBoundedDisplayValue(s32 displayValueIndex, s32 displayValue);
/* Store an option-list entry index for deferred construction and snapshot the
 * second panel value while a window is open. Returns 1 on success or 0 when
 * no window is open. */
s32 evtStoreValueAndCaptureWindowPanelValue(ItfMesEntryIndex entryIndex);

typedef s32 EvtMessageWindowPageValueResult;

#define EVT_MESSAGE_WINDOW_PAGE_VALUE_NO_WINDOW 0
#define EVT_MESSAGE_WINDOW_PAGE_VALUE_UPDATED 1

/* Apply page mode zero and the caller's page value when a window is open.
 * Returns UPDATED when applied, or NO_WINDOW when no window is open. */
EvtMessageWindowPageValueResult evtSetMessageWindowPageValue(s32 pageValue);

void evtSetMessageWindowOptionWhenOpen(s32 option);

#endif
