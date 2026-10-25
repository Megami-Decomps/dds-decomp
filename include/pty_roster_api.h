#ifndef PTY_ROSTER_API_H
#define PTY_ROSTER_API_H

#include "common.h"

struct datUnitWork_t;

typedef s32 PtyAddUnitResult;
enum {
    PTY_ADD_UNIT_RESULT_ALREADY_PRESENT = -1,
    PTY_ADD_UNIT_RESULT_PARTY_FULL = -2,
};

/* Return the assigned party slot, or a PTY_ADD_UNIT_RESULT_* error. */
PtyAddUnitResult ptyAddUnit(s32 unitId);

typedef s32 PtyRemoveUnitResult;
enum {
    PTY_REMOVE_UNIT_RESULT_NOT_FOUND = 0,
    PTY_REMOVE_UNIT_RESULT_REMOVED = 1,
};

/* Return whether a matching roster record was removed. */
PtyRemoveUnitResult ptyRemoveUnit(s32 unitId);
s32 ptyMatchAffinityPermutation(const struct datUnitWork_t *const *actors, s32 affinity);

#endif
