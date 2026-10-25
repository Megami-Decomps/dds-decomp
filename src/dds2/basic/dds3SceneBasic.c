#include "common.h"
#include "sdf_resource.h"
#include "dds3obj.h"
#include "evt_world_object_api.h"
#include "evt_world.h"
#include "kwln.h"
#include "kwln_task_lifecycle.h"
#include "bf_flw0_header.h"

struct sdfMemHandle_t;

extern dds3ProcessID_t *scrStartScript2(s32 priority, BfFlw0Header *script, s32 option);

extern void fldReleaseFieldResources(void);

/* Release field resources before clearing the scene object's state word.
 * The scene resource handle/address are released separately. */
void dds3SceneBasicFreeScene(dds3Basic_t *worldNode) {
    EvtWorldTable *worldData;

    worldData = (EvtWorldTable *)worldNode->data;
    fldReleaseFieldResources();
    worldData->indexedHandle = 0;
}

/* Load another scene's named resource and give the scene object ownership. */
s32 evtLoadSceneResourceFrom(dds3Basic_t *worldNode, const char *resourceName) {
    EvtWorldTable *worldData;
    struct sdfMemHandle_t *resourceHandle;
    u32 resolvedAddress;
    void *resourceAddress;

    worldData = (EvtWorldTable *)worldNode->data;
    if (resourceName == NULL) {
        return 0;
    }
    if (worldData->sceneResourceHandle != 0) {
        dds3SceneBasicReleaseScriptFile(worldNode);
    }
    resourceHandle = sdfDevLoadFileEx(resourceName, &resolvedAddress, 0);
    resourceAddress = (void *)resolvedAddress;
    if (resourceAddress == NULL) {
        return 0;
    }
    worldData->sceneResourceHandle = resourceHandle;
    worldData->sceneScript = (struct BfFlw0Header *)resourceAddress;
    return 1;
}

/* Retain an allocation handle and attach its resolved address; return 1 on
 * attachment, 0 otherwise. NULL preserves the current resource. A non-NULL
 * handle releases the old resource first, so failure does not restore it;
 * a NULL resolved address fails attachment without undoing the retain. */
s32 dds3SceneBasicLoadScriptFile_FromMem(dds3Basic_t *worldNode, struct sdfMemHandle_t *resourceHandle) {
    s32 notAttachedStatus = 0;
    void *resourceAddress;
    EvtWorldTable *worldData = (EvtWorldTable *)worldNode->data;

    if (resourceHandle == NULL) {
        return notAttachedStatus;
    }
    if (worldData->sceneResourceHandle != 0) {
        dds3SceneBasicReleaseScriptFile(worldNode);
    }
    resourceAddress = sdfMemLock(resourceHandle);
    if (resourceAddress != 0) {
        worldData->sceneResourceHandle = resourceHandle;
        worldData->sceneScript = (struct BfFlw0Header *)resourceAddress;
        return 1;
    }
    return notAttachedStatus;
}

/* Release a stored allocation handle when present, then always clear both
 * handle and resolved address. Safe for an already-cleared resource state. */
void dds3SceneBasicReleaseScriptFile(dds3Basic_t *worldNode) {
    EvtWorldTable *worldData = (EvtWorldTable *)worldNode->data;

    if (worldData->sceneResourceHandle != 0) {
        sdfMemFreeDirect(worldData->sceneResourceHandle);
    }
    worldData->sceneResourceHandle = 0;
    worldData->sceneScript = NULL;
}

/* Starts the named script task on the scene object's resource. */
dds3ProcessID_t *evtStartSceneResourceTask(dds3Basic_t *worldNode, const char *taskName) {
    EvtWorldTable *worldData = (EvtWorldTable *)worldNode->data;
    struct BfFlw0Header *sceneScript = worldData->sceneScript;
    ScrProcedureIndex scriptIndex;

    if (sceneScript == NULL) {
        return 0;
    }
    if (taskName == NULL) {
        return 0;
    }
    scriptIndex = scrSearchProcedureName(sceneScript, taskName);
    if (scriptIndex < 0) {
        return 0;
    }
    if (dds3SearchProcessName(taskName) != NULL) {
        return 0;
    }
    return scrStartScript2(0x3EA, sceneScript, scriptIndex);
}

/* Destroy the task selected by taskName; NULL or an absent task is a no-op.
 * unusedContext is not read. The nested early return retains the matched
 * jal/epilogue rather than turning the destruction into a sibling call. */
void dds3SceneBasicEndScript(void *unusedContext, const char *taskName) {
    dds3ProcessID_t *task;

    if (taskName != NULL) {
        task = dds3SearchProcessName(taskName);
        if (task == NULL) {
            return;
        }
        dds3KillProcess(task, 0);
    }
}
