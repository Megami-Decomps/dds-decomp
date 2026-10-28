#include "common.h"
#include "scr.h"
#include "sdf_resource.h"
#include "dds3obj.h"
#include "evt_world_object_api.h"
#include "evt_world.h"
#include "kwln.h"
#include "kwln_task_lifecycle.h"
#include "bf_flw0_header.h"

struct sdfMemHandle_t;

extern dds3ProcessID_t *scrStartScript2(u32 priority, const BfFlw0Header *script, ScrProcedureIndex procedureIndex);

extern void fldReleaseFieldResources(void);

/* Release globally owned field resources before clearing their borrowed alias.
 * The scene resource handle/address are released separately. */
void dds3SceneBasicReleaseFieldResources(dds3Basic_t *worldNode) {
    EvtWorldTable *worldData;

    worldData = (EvtWorldTable *)worldNode->data;
    fldReleaseFieldResources();
    worldData->fieldResourceHandle = NULL;
}

/* NULL preserves the current resource; a non-NULL replacement releases it
 * before loading, so failure leaves no scene script attached. */
Dds3SceneScriptAttachResult evtLoadSceneResourceFrom(
    dds3Basic_t *worldNode, const char *resourceName) {
    EvtWorldTable *worldData;
    struct sdfMemHandle_t *resourceHandle;
    u32 resolvedAddress;
    void *resourceAddress;

    worldData = (EvtWorldTable *)worldNode->data;
    if (resourceName == NULL) {
        return DDS3_SCENE_SCRIPT_ATTACH_FAILED;
    }
    if (worldData->sceneResourceHandle != 0) {
        dds3SceneBasicReleaseScriptFile(worldNode);
    }
    resourceHandle = sdfDevLoadFileEx(resourceName, &resolvedAddress, 0);
    resourceAddress = (void *)resolvedAddress;
    if (resourceAddress == NULL) {
        return DDS3_SCENE_SCRIPT_ATTACH_FAILED;
    }
    worldData->sceneResourceHandle = resourceHandle;
    worldData->sceneScript = (const struct BfFlw0Header *)resourceAddress;
    return DDS3_SCENE_SCRIPT_ATTACH_SUCCEEDED;
}

/* Attach a memory-backed script. NULL input preserves the current resource. A
 * non-NULL handle releases the old resource before locking; success transfers
 * handle ownership to the scene, while lock failure leaves the new handle at
 * the caller and does not restore the previous resource. */
Dds3SceneScriptAttachResult dds3SceneBasicLoadScriptFile_FromMem(
    dds3Basic_t *worldNode, struct sdfMemHandle_t *resourceHandle) {
    Dds3SceneScriptAttachResult attachResult = DDS3_SCENE_SCRIPT_ATTACH_FAILED;
    void *resourceAddress;
    EvtWorldTable *worldData = (EvtWorldTable *)worldNode->data;

    if (resourceHandle == NULL) {
        return attachResult;
    }
    if (worldData->sceneResourceHandle != 0) {
        dds3SceneBasicReleaseScriptFile(worldNode);
    }
    resourceAddress = sdfMemLock(resourceHandle);
    if (resourceAddress != 0) {
        worldData->sceneResourceHandle = resourceHandle;
        worldData->sceneScript = (const struct BfFlw0Header *)resourceAddress;
        return DDS3_SCENE_SCRIPT_ATTACH_SUCCEEDED;
    }
    return attachResult;
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

/* Borrow the scene's retained script view to start its named task; do not
 * mutate the scene or release its resource. */
dds3ProcessID_t *evtStartSceneResourceTask(const dds3Basic_t *worldNode, const char *taskName) {
    const EvtWorldTable *worldData = worldNode->data;
    const struct BfFlw0Header *sceneScript = worldData->sceneScript;
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
