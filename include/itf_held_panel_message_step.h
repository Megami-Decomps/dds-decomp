#ifndef ITF_HELD_PANEL_MESSAGE_STEP_H
#define ITF_HELD_PANEL_MESSAGE_STEP_H

#include "common.h"

/* Script polling stays pending until the final held message has elapsed. */
typedef s32 ItfHeldPanelMessageStepResult;
enum {
    ITF_HELD_PANEL_MESSAGES_PENDING = 0,
    ITF_HELD_PANEL_MESSAGES_COMPLETE = 1,
};

ItfHeldPanelMessageStepResult itfStepHeldPanelMessages(void);

#endif /* ITF_HELD_PANEL_MESSAGE_STEP_H */
