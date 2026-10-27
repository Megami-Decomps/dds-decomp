#ifndef PTY_SKILL_RECOVERY_API_H
#define PTY_SKILL_RECOVERY_API_H

#include "dat_skill_id.h"

struct datUnitWork_t;

void ptyApplySkillRecovery(struct datUnitWork_t *unit, DatSkillId skillId);

#endif /* PTY_SKILL_RECOVERY_API_H */
