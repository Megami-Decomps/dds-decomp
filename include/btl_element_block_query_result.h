#ifndef BTL_ELEMENT_BLOCK_QUERY_RESULT_H
#define BTL_ELEMENT_BLOCK_QUERY_RESULT_H

#include "common.h"

struct BtlUnit;

/* Result of testing whether a unit blocks an element query for its group. */
typedef s32 BtlElementBlockQueryResult;

#define BTL_ELEMENT_BLOCK_QUERY_NOT_BLOCKED 0 /* Applicable unit does not block the query. */
#define BTL_ELEMENT_BLOCK_QUERY_BLOCKED 1 /* Applicable unit blocks the query. */
#define BTL_ELEMENT_BLOCK_QUERY_NOT_APPLICABLE 2 /* Unit state or group excludes the query. */

BtlElementBlockQueryResult btlUnitBlocksElementQueryForGroup(
    struct BtlUnit *unit, s32 action, u32 mask);

#endif /* BTL_ELEMENT_BLOCK_QUERY_RESULT_H */
