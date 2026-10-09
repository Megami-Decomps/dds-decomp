#ifndef EVT_ACTION_OBJECT_H
#define EVT_ACTION_OBJECT_H

#include "common.h"

struct EffWorldNode;

/* Appends a linked kind-2 world node; both inputs remain generic words. */
struct EffWorldNode *evtSpawnActionObj2(s32 firstValue, s32 secondValue);

#endif /* EVT_ACTION_OBJECT_H */
