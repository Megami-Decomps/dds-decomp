#include "common.h"
#include "dds3obj.h"
#include "kwln.h"

extern void *sdfReadNamedResource(void *resource, u32 *resolvedId, s32 options);
extern void *sdfResourceRetainAddress(void *resource);
extern void sdfReleaseResourceAllocation(void *arg);
extern s32 bfFindScriptIndexByName(void *, const char *);
extern KwlnTask *kwlnTaskGetTaskByName(const char *name);
extern s32 scrCreateTaskForProcessId(s32, s32, s32);
extern void evtReleaseSceneResource(Scene *);
extern s32 kwlnTaskDestroyWithHierarchy(KwlnTask *task, s32 delayTicks);

/* Scene object fields used by the resource helpers (0x20). */
typedef struct {
    u8 pad0[0x18];
    void *unk18;
    void *unk1C;
} SceneObjectRes;

/* Release field resources before clearing the scene object's state word.
 * The scene resource handle/address are released separately. */
void dds3ClearSceneObjectState(Scene *scene) {
    SceneObject *sceneObject;

    sceneObject = scene->object;
    fldReleaseFieldResources();
    sceneObject->state = 0;
}

/* Load another scene's named resource and give the scene object ownership. */
s32 evtLoadSceneResourceFrom(Scene *scene, const char *resourceName) {
    SceneObjectRes *object;
    void *resourceHandle;
    u32 resolvedAddress;
    void *resourceAddress;

    object = (SceneObjectRes *)scene->object;
    if (resourceName == NULL) {
        return 0;
    }
    if (object->unk18 != NULL) {
        evtReleaseSceneResource(scene);
    }
    resourceHandle = sdfReadNamedResource((void *)resourceName, &resolvedAddress, 0);
    resourceAddress = (void *)resolvedAddress;
    if (resourceAddress == NULL) {
        return 0;
    }
    object->unk18 = resourceHandle;
    object->unk1C = resourceAddress;
    return 1;
}

/* Retain an allocation handle and attach its resolved address; return 1 on
 * attachment, 0 otherwise. NULL preserves the current resource. A non-NULL
 * handle releases the old resource first, so failure does not restore it;
 * a NULL resolved address fails attachment without undoing the retain. */
s32 evtRetainSceneResource(Scene *scene, void *resourceHandle) {
    s32 result = 0;
    void *resourceAddress;
    SceneObjectRes *sceneObject = (SceneObjectRes *)scene->object;

    if (resourceHandle == NULL) {
        return result;
    }
    if (sceneObject->unk18 != NULL) {
        evtReleaseSceneResource(scene);
    }
    resourceAddress = sdfResourceRetainAddress(resourceHandle);
    if (resourceAddress != NULL) {
        sceneObject->unk18 = resourceHandle;
        sceneObject->unk1C = resourceAddress;
        return 1;
    }
    return result;
}

/* Release a stored allocation handle when present, then always clear both
 * handle and resolved address. Safe for an already-cleared resource state. */
void evtReleaseSceneResource(Scene *scene) {
    SceneObjectRes *sceneObject = (SceneObjectRes *)scene->object;

    if (sceneObject->unk18 != NULL) {
        sdfReleaseResourceAllocation(sceneObject->unk18);
    }
    sceneObject->unk18 = NULL;
    sceneObject->unk1C = NULL;
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
