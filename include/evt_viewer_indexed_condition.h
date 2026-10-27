#ifndef EVT_VIEWER_INDEXED_CONDITION_H
#define EVT_VIEWER_INDEXED_CONDITION_H

#include "common.h"

/* Sign-extended 32-bit view of the signed timeline-key parameter halfword:
 * bits 12-15 select a script integer, and bits 0-11 hold its required value.
 * Selector zero disables the condition. */
typedef s32 EvtViewerIndexedCondition;

/* The predicate returns only these two outcomes. */
typedef enum EvtViewerIndexedConditionResult {
    EVT_VIEWER_INDEXED_CONDITION_NOT_MATCHED = 0,
    EVT_VIEWER_INDEXED_CONDITION_MATCHED = 1
} EvtViewerIndexedConditionResult;

/* A zero selector matches unconditionally; otherwise the integer at
 * script.ints[199 + selector] must equal the encoded 12-bit value. */
EvtViewerIndexedConditionResult evtViewerTestIndexedCondition(
    EvtViewerIndexedCondition condition);

#endif /* EVT_VIEWER_INDEXED_CONDITION_H */
