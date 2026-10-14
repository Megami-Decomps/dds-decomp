#ifndef PRF_SKILL_LIST_BUFFER_H
#define PRF_SKILL_LIST_BUFFER_H

#include "dat_state.h"

/* 0x40-byte local storage used by menu callers of prfBuildRawSkillList. The
 * producer writes only the 0x34-byte PrfSkillList prefix; retain caller tail
 * capacity without assigning it a producer-owned meaning. */
typedef struct PrfSkillListBuffer {
    PrfSkillList list;
    u8 opaque34[0x0C];
} PrfSkillListBuffer;
typedef char PrfSkillListBufferSizeCheck[
    (sizeof(PrfSkillListBuffer) == 0x40) ? 1 : -1];

s32 prfBuildRawSkillList(u16 profile, PrfSkillList *output);

#endif
