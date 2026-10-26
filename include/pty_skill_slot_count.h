#ifndef PTY_SKILL_SLOT_COUNT_H
#define PTY_SKILL_SLOT_COUNT_H

#include "common.h"

typedef u8 PtySkillSlotCount;

/* Capacities at the level thresholds, plus the no-new-slots result. */
enum {
    PTY_SKILL_SLOT_COUNT_NONE = 0,
    PTY_SKILL_SLOT_COUNT_INITIAL = 4,
    PTY_SKILL_SLOT_COUNT_AFTER_FIRST_UNLOCK = 6,
    PTY_SKILL_SLOT_COUNT_AFTER_SECOND_UNLOCK = 8
};

#endif /* PTY_SKILL_SLOT_COUNT_H */
