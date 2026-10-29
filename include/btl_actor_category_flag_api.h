#ifndef BTL_ACTOR_CATEGORY_FLAG_API_H
#define BTL_ACTOR_CATEGORY_FLAG_API_H

#include "common.h"

struct BtlLinkedCommand;

typedef s32 BtlActorCategoryFlagUseResult;
enum {
    BTL_ACTOR_CATEGORY_FLAG_USE_UNAVAILABLE = 0,
    BTL_ACTOR_CATEGORY_FLAG_USE_AVAILABLE = 1,
};

BtlActorCategoryFlagUseResult btlCanUseActorCategoryFlag2(struct BtlLinkedCommand *actor);
BtlActorCategoryFlagUseResult btlCanUseActorCategoryFlag4(struct BtlLinkedCommand *actor);

#endif /* BTL_ACTOR_CATEGORY_FLAG_API_H */
