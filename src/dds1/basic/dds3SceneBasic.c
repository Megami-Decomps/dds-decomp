#include "common.h"
#include "dds3obj.h"

extern void sdfReleaseResourceAllocation(void *);
extern void *sdfResourceRetainAddress(void *);
extern void *sdfReadNamedResource(const char *, void **, s32);
extern s32 bfFindScriptIndexByName(void *, const char *);
extern void *kwlnTaskGetTaskByName(const char *);
extern void kwlnTaskDestroyWithHierarchy(void *, s32);
extern s32 scrCreateTaskForProcessId(s32, void *, s32);
extern void evtReleaseSceneResource(Scene *);

/* Resource ownership and resolved address stored in the scene object. */
typedef struct {
    u8 pad0[0x18];
    void *resourceHandle;
    void *resourceAddress;
} SceneObjectResourceState;

/* Release field resources before clearing the scene object's state word.
 * The scene resource handle/address are released separately. */
void dds3ClearSceneObjectState(Scene *scene) {
    SceneObject *sceneObject;

    sceneObject = scene->object;
    fldReleaseFieldResources();
    sceneObject->state = 0;
}

s32 evtLoadSceneResourceFrom(Scene *scene, const char *resourceName) {
    SceneObjectResourceState *object;
    void *resourceHandle;
    void *resourceAddress;

    object = (SceneObjectResourceState *)scene->object;
    if (resourceName == NULL) {
        return 0;
    }
    if (object->resourceHandle != NULL) {
        evtReleaseSceneResource(scene);
    }
    resourceHandle = sdfReadNamedResource(resourceName, &resourceAddress, 0);
    if (resourceAddress == NULL) {
        return 0;
    }
    object->resourceHandle = resourceHandle;
    object->resourceAddress = resourceAddress;
    return 1;
}

/* Retain an allocation handle and attach its resolved address; return 1 on
 * attachment, 0 otherwise. NULL preserves the current resource. A non-NULL
 * handle releases the old resource first, so failure does not restore it;
 * a NULL resolved address fails attachment without undoing the retain. */
s32 evtRetainSceneResource(Scene *scene, void *resourceHandle) {
    s32 result = 0;
    void *resourceAddress;
    SceneObjectResourceState *sceneObject = (SceneObjectResourceState *)scene->object;

    if (resourceHandle == NULL) {
        return result;
    }
    if (sceneObject->resourceHandle != NULL) {
        evtReleaseSceneResource(scene);
    }
    resourceAddress = sdfResourceRetainAddress(resourceHandle);
    if (resourceAddress != NULL) {
        sceneObject->resourceHandle = resourceHandle;
        sceneObject->resourceAddress = resourceAddress;
        return 1;
    }
    return result;
}

/* Release a stored allocation handle when present, then always clear both
 * handle and resolved address. Safe for an already-cleared resource state. */
void evtReleaseSceneResource(Scene *scene) {
    SceneObjectResourceState *sceneObject = (SceneObjectResourceState *)scene->object;

    if (sceneObject->resourceHandle != NULL) {
        sdfReleaseResourceAllocation(sceneObject->resourceHandle);
    }
    sceneObject->resourceHandle = NULL;
    sceneObject->resourceAddress = NULL;
}

/* Starts the named script task on the scene object's resource; stays asm:
   retail's beqz/b merge of the two exit paths has no plain-C shape. */
INCLUDE_ASM(const s32, "basic/dds3SceneBasic", evtStartSceneResourceTask);

/* Destroy the task selected by taskName; NULL or an absent task is a no-op.
 * unusedContext is not read. The nested early return retains the matched
 * jal/epilogue rather than turning the destruction into a sibling call. */
void evtDestroyNamedTask(void *unusedContext, const char *taskName) {
    void *task;

    if (taskName != NULL) {
        task = kwlnTaskGetTaskByName(taskName);
        if (task == NULL) {
            return;
        }
        kwlnTaskDestroyWithHierarchy(task, 0);
    }
}
