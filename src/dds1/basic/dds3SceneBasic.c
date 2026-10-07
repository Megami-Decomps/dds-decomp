#include "common.h"
#include "dds3obj.h"
#include "evt_world.h"
#include "kwln.h"

extern void sdfReleaseResourceAllocation(void *);
extern void *sdfResourceRetainAddress(void *);
extern void *sdfReadNamedResource(const char *, void **, s32);
extern s32 bfFindScriptIndexByName(void *, const char *);
extern KwlnTask *kwlnTaskGetTaskByName(const char *name);
extern s32 kwlnTaskDestroyWithHierarchy(KwlnTask *task, s32 delayTicks);
extern s32 scrCreateTaskForProcessId(s32, void *, s32);
extern void evtReleaseSceneResource(EffWorldNode *worldNode);

/* Release field resources before clearing the scene object's state word.
 * The scene resource handle/address are released separately. */
void dds3ClearSceneObjectState(EffWorldNode *worldNode) {
    EvtWorldTable *worldData;

    worldData = (EvtWorldTable *)worldNode->data;
    fldReleaseFieldResources();
    worldData->indexedHandle = 0;
}

s32 evtLoadSceneResourceFrom(EffWorldNode *worldNode, const char *resourceName) {
    EvtWorldTable *worldData;
    void *resourceHandle;
    void *resourceAddress;

    worldData = (EvtWorldTable *)worldNode->data;
    if (resourceName == NULL) {
        return 0;
    }
    if (worldData->unk18 != 0) {
        evtReleaseSceneResource(worldNode);
    }
    resourceHandle = sdfReadNamedResource(resourceName, &resourceAddress, 0);
    if (resourceAddress == NULL) {
        return 0;
    }
    worldData->unk18 = (u32)resourceHandle;
    worldData->unk1C = (u32)resourceAddress;
    return 1;
}

/* Retain an allocation handle and attach its resolved address; return 1 on
 * attachment, 0 otherwise. NULL preserves the current resource. A non-NULL
 * handle releases the old resource first, so failure does not restore it;
 * a NULL resolved address fails attachment without undoing the retain. */
s32 evtRetainSceneResource(EffWorldNode *worldNode, void *resourceHandle) {
    s32 result = 0;
    void *resourceAddress;
    EvtWorldTable *worldData = (EvtWorldTable *)worldNode->data;

    if (resourceHandle == NULL) {
        return result;
    }
    if (worldData->unk18 != 0) {
        evtReleaseSceneResource(worldNode);
    }
    resourceAddress = sdfResourceRetainAddress(resourceHandle);
    if (resourceAddress != NULL) {
        worldData->unk18 = (u32)resourceHandle;
        worldData->unk1C = (u32)resourceAddress;
        return 1;
    }
    return result;
}

/* Release a stored allocation handle when present, then always clear both
 * handle and resolved address. Safe for an already-cleared resource state. */
void evtReleaseSceneResource(EffWorldNode *worldNode) {
    EvtWorldTable *worldData = (EvtWorldTable *)worldNode->data;

    if (worldData->unk18 != 0) {
        sdfReleaseResourceAllocation((void *)worldData->unk18);
    }
    worldData->unk18 = 0;
    worldData->unk1C = 0;
}

/* Starts the named script task on the scene object's resource; stays asm:
   retail's beqz/b merge of the two exit paths has no plain-C shape. */
INCLUDE_ASM(const s32, "basic/dds3SceneBasic", evtStartSceneResourceTask);

/* Destroy the task selected by taskName; NULL or an absent task is a no-op.
 * unusedContext is not read. The nested early return retains the matched
 * jal/epilogue rather than turning the destruction into a sibling call. */
void evtDestroyNamedTask(void *unusedContext, const char *taskName) {
    KwlnTask *task;

    if (taskName != NULL) {
        task = kwlnTaskGetTaskByName(taskName);
        if (task == NULL) {
            return;
        }
        kwlnTaskDestroyWithHierarchy(task, 0);
    }
}
