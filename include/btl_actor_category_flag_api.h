#ifndef BTL_ACTOR_CATEGORY_FLAG_API_H
#define BTL_ACTOR_CATEGORY_FLAG_API_H

#include "common.h"

struct BtlLinkedCommand;

typedef s32 BtlActorCategoryFlagUseResult;

BtlActorCategoryFlagUseResult btlCanUseActorCategoryFlag2(struct BtlLinkedCommand *actor);
BtlActorCategoryFlagUseResult btlCanUseActorCategoryFlag4(struct BtlLinkedCommand *actor);

#endif /* BTL_ACTOR_CATEGORY_FLAG_API_H */
