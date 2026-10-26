#ifndef EVT_WORLD_OBJECT_API_H
#define EVT_WORLD_OBJECT_API_H

#include "common.h"

/* Create and select the world object for an area/room pair; returns 1 after setup. */
s32 evtLoadArea(s32 area, s32 room);

struct dds3Basic_t;
struct sdfMemHandle_t;

typedef s32 Dds3SceneScriptAttachResult;
enum Dds3SceneScriptAttachResult {
    DDS3_SCENE_SCRIPT_ATTACH_FAILED = 0,
    DDS3_SCENE_SCRIPT_ATTACH_SUCCEEDED = 1
};

/* Attach a memory-backed scene script to worldNode. A NULL handle returns
 * FAILED and leaves the current resource untouched. A non-NULL handle replaces
 * the current resource before locking: SUCCEEDED transfers ownership to the
 * scene, while FAILED leaves the handle at the caller (the previous resource
 * has already been released). */
Dds3SceneScriptAttachResult dds3SceneBasicLoadScriptFile_FromMem(
    struct dds3Basic_t *worldNode, struct sdfMemHandle_t *resourceHandle);

/* Load a named scene script. NULL leaves the current resource untouched. A
 * non-NULL name releases it before loading, so FAILED leaves it cleared;
 * SUCCEEDED stores the loaded handle and address on worldNode. */
Dds3SceneScriptAttachResult evtLoadSceneResourceFrom(
    struct dds3Basic_t *worldNode, const char *resourceName);

/* Release the node's scene allocation and clear its handle and address. */
void dds3SceneBasicReleaseScriptFile(struct dds3Basic_t *worldNode);

#endif /* EVT_WORLD_OBJECT_API_H */
