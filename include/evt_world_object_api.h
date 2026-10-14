#ifndef EVT_WORLD_OBJECT_API_H
#define EVT_WORLD_OBJECT_API_H

#include "common.h"

/* Create and select the world object for an area/room pair; returns 1 after setup. */
s32 evtCreateWorldObjectForKey(s32 area, s32 room);

struct EffWorldNode;
/* Release the node's scene allocation and clear its handle and address. */
void evtReleaseSceneResource(struct EffWorldNode *worldNode);

#endif /* EVT_WORLD_OBJECT_API_H */
