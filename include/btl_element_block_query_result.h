#ifndef BTL_ELEMENT_BLOCK_QUERY_RESULT_H
#define BTL_ELEMENT_BLOCK_QUERY_RESULT_H

#include "common.h"

struct BtlUnit;

/* Ally/enemy selector bits stored in BtlUnit.status.flags. */
typedef u32 BtlUnitStatusSideMask;

enum {
    BTL_UNIT_STATUS_SIDE_ALLY = 0x200,
    BTL_UNIT_STATUS_SIDE_ENEMY = 0x400
};

/* Result for a unit whose status flags intersect the requested side mask. */
typedef s32 BtlElementBlockQueryResult;

#define BTL_ELEMENT_BLOCK_QUERY_NOT_BLOCKED 0 /* Applicable unit does not block the query. */
#define BTL_ELEMENT_BLOCK_QUERY_BLOCKED 1 /* Applicable unit blocks the query. */
#define BTL_ELEMENT_BLOCK_QUERY_NOT_APPLICABLE 2 /* Unit state or group excludes the query. */

/* Result for an individual unit's binary element-block query. */
typedef s32 BtlUnitElementBlockQueryResult;
#define BTL_UNIT_ELEMENT_BLOCK_QUERY_NOT_BLOCKED 0
#define BTL_UNIT_ELEMENT_BLOCK_QUERY_BLOCKED 1

BtlUnitElementBlockQueryResult btlUnitBlocksElementQuery(
    struct BtlUnit *unit, s32 action, s32 mask);

BtlElementBlockQueryResult btlUnitBlocksElementQueryForGroup(
    struct BtlUnit *unit, s32 action, BtlUnitStatusSideMask sideMask);

#endif /* BTL_ELEMENT_BLOCK_QUERY_RESULT_H */
