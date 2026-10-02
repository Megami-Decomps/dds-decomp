#include "common.h"
#include "dds3obj.h"

extern void func_002D0918(void *);
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

void dds3ClearSceneObjectState(Scene *scene) {
    SceneObject *object;

    object = scene->object;
    fldReleaseFieldResources();
    object->state = 0;
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

/* Retain `name` and hand its address to the scene object. */
s32 evtRetainSceneResource(Scene *scene, void *name) {
    s32 result = 0;
    void *address;
    SceneObjectResourceState *object = (SceneObjectResourceState *)scene->object;

    if (name == NULL) {
        return result;
    }
    if (object->resourceHandle != NULL) {
        evtReleaseSceneResource(scene);
    }
    address = sdfResourceRetainAddress(name);
    if (address != NULL) {
        object->resourceHandle = name;
        object->resourceAddress = address;
        return 1;
    }
    return result;
}

/* Release the resource this scene object currently owns. */
void evtReleaseSceneResource(Scene *scene) {
    SceneObjectResourceState *object = (SceneObjectResourceState *)scene->object;

    if (object->resourceHandle != NULL) {
        func_002D0918(object->resourceHandle);
    }
    object->resourceHandle = NULL;
    object->resourceAddress = NULL;
}

/* Starts the named script task on the scene object's resource; stays asm:
   retail's beqz/b merge of the two exit paths has no plain-C shape. */
INCLUDE_ASM(const s32, "basic/dds3SceneBasic", evtStartSceneResourceTask);

/* Destroys the task named by `name`, if there is one. The `return` inside the
   nested block is what keeps retail's jal+epilogue instead of a sibling call. */
void evtDestroyNamedTask(void *ctx, const char *name) {
    void *task;

    if (name != NULL) {
        task = kwlnTaskGetTaskByName(name);
        if (task == NULL) {
            return;
        }
        kwlnTaskDestroyWithHierarchy(task, 0);
    }
}
