#ifndef DAT_UNIT_DYING_API_H
#define DAT_UNIT_DYING_API_H

#include "common.h"

typedef u8 DatUnitDyingResult;
enum {
    DAT_UNIT_DYING_RESULT_NO = 0,
    DAT_UNIT_DYING_RESULT_YES = 1
};

struct datUnitWork_t;

/* Return YES when truncated current HP percent is below 25.
 * maxHp must be nonzero. */
DatUnitDyingResult datCheckDying(const struct datUnitWork_t *unit);

#endif /* DAT_UNIT_DYING_API_H */
