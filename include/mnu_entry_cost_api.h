#ifndef MNU_ENTRY_COST_API_H
#define MNU_ENTRY_COST_API_H

#include "common.h"

struct datUnitWork_t;

typedef s32 MnuEntryCostAffordability;
enum {
    MNU_ENTRY_COST_UNAFFORDABLE = 0,
    MNU_ENTRY_COST_AFFORDABLE = 1
};

/* Compares stored raw HP/MP percentages; equality and other cost kinds pass. */
MnuEntryCostAffordability mnuCanAffordEntryCost(
    u16 commandId, const struct datUnitWork_t *actorEntry);

#endif /* MNU_ENTRY_COST_API_H */
