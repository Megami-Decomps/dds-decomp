#ifndef BTL_GROUPED_ENTITY_API_H
#define BTL_GROUPED_ENTITY_API_H

#include "mdl.h"

struct SdfMemBlock;

/* Return the first node of this type in the group, or NULL. */
BattleGroupNode *btlFindGroupedEntity(s32 group, s32 type);

/* Create or retire a group/type node in the model manager's group list. */
void btlCreateGroupNode(s32 group, s32 type, s32 ownershipFlags,
                        DevRequest *resourceList,
                        struct SdfItemListRef *itemList,
                        struct SdfMemBlock *requestAllocation);
void btlDestroyGroupNode(BattleGroupNode *groupNode);
void btlRemoveCurrentGroupedEntity(s32 group, s32 type);

#endif
