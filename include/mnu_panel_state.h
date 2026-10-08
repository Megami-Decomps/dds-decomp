#ifndef MNU_PANEL_STATE_H
#define MNU_PANEL_STATE_H

#include "common.h"

struct MenuPanelState;

/* Store the selected panel state in a title-specific allocated panel owner. */
void mnuSetPanelState(struct MenuPanelState *panel, u32 state);

#endif
