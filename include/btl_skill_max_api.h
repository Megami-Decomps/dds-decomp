#ifndef BTL_SKILL_MAX_API_H
#define BTL_SKILL_MAX_API_H

#include "common.h"

struct DatPartyRecord;

u32 btlComputeSkillAdjustedMaxHp(struct DatPartyRecord *record);
u32 btlComputeSkillAdjustedMaxMp(struct DatPartyRecord *record);

#endif
