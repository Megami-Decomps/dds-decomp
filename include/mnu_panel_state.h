#ifndef MNU_PANEL_STATE_H
#define MNU_PANEL_STATE_H

#include "common.h"

struct MenuPanelState;
struct EffectSlotSet;
struct EffMappedResource;

/* Store the selected panel state in a title-specific allocated panel owner. */
void mnuSetPanelState(struct MenuPanelState *panel, u32 state);
#ifndef VERSION_DDS2
void mnuInitializePanelResource(struct MenuPanelState *panel,
                                struct EffectSlotSet *resource,
                                struct EffMappedResource *target);
#endif

#endif
