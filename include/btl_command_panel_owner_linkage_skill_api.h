#ifndef BTL_COMMAND_PANEL_OWNER_LINKAGE_SKILL_API_H
#define BTL_COMMAND_PANEL_OWNER_LINKAGE_SKILL_API_H

#include "common.h"

typedef s32 BtlCommandPanelLinkageSkillAvailability;
enum {
    BTL_COMMAND_PANEL_LINKAGE_SKILL_UNAVAILABLE = 0,
    BTL_COMMAND_PANEL_LINKAGE_SKILL_AVAILABLE = 1,
};

/* Clears pending notice flags; reports an unmarked eligible LINKAGE skill. */
BtlCommandPanelLinkageSkillAvailability
btlCommandPanelOwnerHasUnmarkedLinkageSkill(void);

#endif /* BTL_COMMAND_PANEL_OWNER_LINKAGE_SKILL_API_H */
