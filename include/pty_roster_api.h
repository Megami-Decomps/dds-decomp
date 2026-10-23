#ifndef PTY_ROSTER_API_H
#define PTY_ROSTER_API_H

#include "common.h"

struct datUnitWork_t;

s32 ptyAddUnit(s32 rosterIndex);
s32 ptyRemoveUnit(s32 unitId);
s32 ptyMatchAffinityPermutation(const struct datUnitWork_t *const *actors, s32 affinity);

#endif
