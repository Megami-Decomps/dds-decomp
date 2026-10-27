#ifndef BTL_TARGET_LIST_BUILDER_H
#define BTL_TARGET_LIST_BUILDER_H

#include "common.h"

struct BtlIndexList;
struct BtlTask;
struct ActionStateLink;

/* The low two area bits select allies and the opposing side. */
typedef s32 BtlTargetListArea;
enum {
    BTL_TARGET_LIST_AREA_NONE = 0,
    BTL_TARGET_LIST_AREA_ALLIES = 1,
    BTL_TARGET_LIST_AREA_OPPOSING_SIDE = 2,
};

/* Rule bits govern the acting unit and living/defeated candidate paths. */
typedef s32 BtlTargetListRule;
enum {
    BTL_TARGET_LIST_RULE_NONE = 0,
    BTL_TARGET_LIST_RULE_SELF_ONLY = 1, /* Appends the actor before area filtering. */
    BTL_TARGET_LIST_RULE_LIVING_NON_ACTOR = 2, /* Accepts living units except the actor. */
    BTL_TARGET_LIST_RULE_INCLUDE_DEFEATED = 4, /* Adds defeated units to the normal scan. */
    BTL_TARGET_LIST_RULE_DEFEATED_ONLY = 8, /* Skips living units. */
};

/* Zero disables party-status masking but still rejects state flags 0xE0;
 * -1 bypasses both checks. */
typedef s32 BtlTargetStatusExclusion;
enum {
    BTL_TARGET_STATUS_EXCLUSION_NO_MASK = 0,
};
#define BTL_TARGET_STATUS_EXCLUSION_ANY ((BtlTargetStatusExclusion)-1)

/* The output list is empty or contains at least one selected unit. */
typedef s32 BtlTargetListBuildResult;
enum {
    BTL_TARGET_LIST_BUILD_EMPTY = 0,
    BTL_TARGET_LIST_BUILD_NONEMPTY = 1,
};

#if defined(VERSION_DDS1)
BtlTargetListBuildResult btlBuildEligibleTargetIndexList(
    struct BtlTask *action, struct BtlIndexList *targets, BtlTargetListArea area,
    BtlTargetListRule rule, BtlTargetStatusExclusion excludedStatus);
#elif defined(VERSION_DDS2)
BtlTargetListBuildResult btlBuildUnitIndexList(
    struct ActionStateLink *action, struct BtlIndexList *targets, BtlTargetListArea area,
    BtlTargetListRule rule, BtlTargetStatusExclusion excludedStatus);
#else
#error "btl_target_list_builder.h requires a DDS title version"
#endif

#endif /* BTL_TARGET_LIST_BUILDER_H */
