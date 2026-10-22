#ifndef BTL_SKILL_MAX_API_H
#define BTL_SKILL_MAX_API_H

#include "common.h"

struct datUnitWork_t;

u32 btlComputeSkillAdjustedMaxHp(struct datUnitWork_t *record);
u32 btlComputeSkillAdjustedMaxMp(struct datUnitWork_t *record);

#endif
