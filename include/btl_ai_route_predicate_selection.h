#ifndef BTL_AI_ROUTE_PREDICATE_SELECTION_H
#define BTL_AI_ROUTE_PREDICATE_SELECTION_H

/* Values stored in BtlAiScratchWork.conditionKind after route selection. */
enum BtlAiRoutePredicateSelection {
    BTL_AI_ROUTE_PREDICATE_SET_ALL = 0,
    BTL_AI_ROUTE_PREDICATE_SET_FIRST_TWO = 1,
    BTL_AI_ROUTE_PREDICATE_SET_FIRST_AND_THIRD = 2,
    BTL_AI_ROUTE_PREDICATE_SET_SECOND_AND_THIRD = 3,
    BTL_AI_ROUTE_PREDICATE_SET_FIRST = 4,
    BTL_AI_ROUTE_PREDICATE_SET_SECOND = 5,
    BTL_AI_ROUTE_PREDICATE_SET_THIRD = 6,
    BTL_AI_ROUTE_UNCONDITIONAL = 7,
    BTL_AI_ROUTE_NO_MATCH = 8,
};

/* Preserve the member token for compiler matching; source uses its route role. */
#define routePredicateSelection conditionKind

#endif /* BTL_AI_ROUTE_PREDICATE_SELECTION_H */
