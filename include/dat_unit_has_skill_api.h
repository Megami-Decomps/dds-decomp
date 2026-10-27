#ifndef DAT_UNIT_HAS_SKILL_API_H
#define DAT_UNIT_HAS_SKILL_API_H

#include "common.h"
#include "dat_skill_id.h"

struct datUnitWork_t;

u8 datUnitHasSkill(const struct datUnitWork_t *unit, DatSkillId skillId);

#endif /* DAT_UNIT_HAS_SKILL_API_H */
