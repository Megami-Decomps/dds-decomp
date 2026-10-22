#ifndef MNU_PARTY_PANEL_API_H
#define MNU_PARTY_PANEL_API_H

#include "common.h"
#include "mnu.h"

struct datUnitWork_t;

/* DDS2 party panels drawn from a sprite state's resource sets and fade. */
void mnuDrawPartySkillListPanel(s32 x, s32 y, s32 depth, struct datUnitWork_t *record, MenuSpriteState *panel,
                                s32 surface);
void mnuDrawPartyProfilePanelContents(s32 x, s32 y, s32 depth, struct datUnitWork_t *actor, s32 profile,
                                      MenuSpriteState *panel, s32 surface);

#endif /* MNU_PARTY_PANEL_API_H */
