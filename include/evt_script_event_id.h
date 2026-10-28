#ifndef EVT_SCRIPT_EVENT_ID_H
#define EVT_SCRIPT_EVENT_ID_H

#include "common.h"

/* Script-pack event ID queued for follow-up dispatch; nonpositive means none. */
typedef s32 EvtScriptEventId;

extern EvtScriptEventId evtPendingEventSelection;

#endif /* EVT_SCRIPT_EVENT_ID_H */
