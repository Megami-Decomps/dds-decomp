#ifndef BTL_OPTION_SKILL_API_H
#define BTL_OPTION_SKILL_API_H

#include "btl_command_id.h"

struct BtlUnit;

/* Count eligible IDs, optionally writing them to the caller's buffer. */
s32 btlCollectAvailableOptionSkills(struct BtlUnit *actor, BtlCommandId *choices);

/* Return an available option-skill command ID, or the invalid-ID sentinel. */
BtlCommandId btlChooseRandomAvailableOption(struct BtlUnit *actor);

#endif /* BTL_OPTION_SKILL_API_H */
