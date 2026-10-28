#ifndef DAT_UNIT_HAS_SKILL_API_H
#define DAT_UNIT_HAS_SKILL_API_H

#include "common.h"
#include "dat_skill_id.h"

struct datUnitWork_t;

typedef u8 DatUnitHasSkillResult;

#define DAT_UNIT_SKILL_RESULT_ABSENT ((DatUnitHasSkillResult)0)
#define DAT_UNIT_SKILL_RESULT_PRESENT ((DatUnitHasSkillResult)1)

DatUnitHasSkillResult datUnitHasSkill(
    const struct datUnitWork_t *unit, DatSkillId skillId);

#endif /* DAT_UNIT_HAS_SKILL_API_H */
