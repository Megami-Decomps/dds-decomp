#ifndef EVT_WORLD_OBJECT_API_H
#define EVT_WORLD_OBJECT_API_H

#include "common.h"

/* Create and select the world object for an area/room pair; returns 1 after setup. */
s32 evtLoadArea(s32 area, s32 room);

struct dds3Basic_t;
/* Release the node's scene allocation and clear its handle and address. */
void dds3SceneBasicReleaseScriptFile(struct dds3Basic_t *worldNode);

#endif /* EVT_WORLD_OBJECT_API_H */
