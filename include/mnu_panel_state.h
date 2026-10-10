#ifndef MNU_PANEL_STATE_H
#define MNU_PANEL_STATE_H

#include "common.h"

struct MenuPanelState;
struct EffectSlotSet;
struct EffMappedResource;

/* Store the selected panel state in a title-specific allocated panel owner. */
void mnuSetPanelState(struct MenuPanelState *panel, u32 state);
#ifndef VERSION_DDS2
/* Bind the four panel grid slots used by the DDS1 skill-page initializer. */
void mnuSetPanelFirstGridSlot(struct MenuPanelState *panel, u32 valueA,
                              u32 valueB, struct EffectSlotSet *resource,
                              u32 index);
void mnuSetPanelSecondGridSlot(struct MenuPanelState *panel, u32 valueA,
                               u32 valueB, struct EffectSlotSet *resource,
                               u32 index);
void mnuSetPanelThirdGridSlot(struct MenuPanelState *panel, u32 valueA,
                              u32 valueB, struct EffectSlotSet *resource,
                              u32 index);
void mnuSetPanelFourthGridSlot(struct MenuPanelState *panel, u32 valueA,
                               u32 valueB, struct EffectSlotSet *resource,
                               u32 index, u32 additionalValue);
void mnuInitializePanelResource(struct MenuPanelState *panel,
                                struct EffectSlotSet *resource,
                                struct EffMappedResource *target);
#else
/* DDS2 forwards both resource words to its panel constructor. */
void mnuInitializePanelResource(struct MenuPanelState *panel,
                                s32 resource, s32 material);
#endif

#endif
