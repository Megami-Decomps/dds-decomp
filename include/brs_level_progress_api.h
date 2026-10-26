#ifndef BRS_LEVEL_PROGRESS_API_H
#define BRS_LEVEL_PROGRESS_API_H

#include "common.h"
#include "pty_skill_slot_count.h"

/* Level thresholds 1, 10, and 20 provide 4, 6, and 8 skill slots. */
PtySkillSlotCount brsGetLevelStepForValue(s32 level);

/* Return the newly reached slot capacity, or NONE if the gain crosses no
 * threshold. */
PtySkillSlotCount brsGetLevelStepCrossedBy(s32 startingLevel, s32 levelGain);

#endif /* BRS_LEVEL_PROGRESS_API_H */
