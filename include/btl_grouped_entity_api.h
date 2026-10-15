#ifndef BTL_GROUPED_ENTITY_API_H
#define BTL_GROUPED_ENTITY_API_H

#include "mdl.h"

/* Return the first node of this type in the group, or NULL. */
BattleGroupNode *btlFindGroupedEntity(s32 group, s32 type);

#endif
