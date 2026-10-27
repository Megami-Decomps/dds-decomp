#ifndef PTY_SKILL_NIBBLE_STATE_H
#define PTY_SKILL_NIBBLE_STATE_H

#include "common.h"

typedef enum PtySkillNibbleState {
    PTY_SKILL_NIBBLE_STATE_CLEAR = 0,
    PTY_SKILL_NIBBLE_STATE_PRIMARY_BIT_SET = 1,
    PTY_SKILL_NIBBLE_STATE_BIT_1_SET = 2
} PtySkillNibbleState;

struct datUnitWork_t;
PtySkillNibbleState ptyGetSkillNibbleState(const struct datUnitWork_t *partyRecord, u16 flagId);

#endif
