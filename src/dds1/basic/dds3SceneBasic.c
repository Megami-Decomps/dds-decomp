#include "common.h"
#include "dds3obj.h"

extern void func_002D0918(void *);
extern void *sdfResourceRetainAddress(void *);
extern u32 func_002EB028(const char *, u32 *, s32);
extern s32 bfFindScriptIndexByName(void *, const char *);
extern void *kwlnTaskGetTaskByName(const char *);
extern void kwlnTaskDestroyWithHierarchy(void *, s32);
extern s32 scrCreateTaskForProcessId(s32, void *, s32);
extern void evtReleaseSceneResource(Scene *);

/* Scene object fields used by the resource helpers (0x20). */
typedef struct {
    u8 pad0[0x18];
    void *unk18;
    void *unk1C;
} SceneObjectRes;

void dds3ClearSceneObjectState(Scene *scene) {
    SceneObject *object;

    object = scene->object;
    fldReleaseFieldResources();
    object->state = 0;
}

/* Loads another scene's resource into this one; stays asm: a $16/$17
   saved-register priority swap no natural declaration order produces. */
INCLUDE_ASM(const s32, "basic/dds3SceneBasic", evtLoadSceneResourceFrom);

/* Retain `name` and hand its address to the scene object. */
s32 evtRetainSceneResource(Scene *scene, void *name) {
    s32 result = 0;
    void *address;
    SceneObjectRes *object = (SceneObjectRes *)scene->object;

    if (name == NULL) {
        return result;
    }
    if (object->unk18 != NULL) {
        evtReleaseSceneResource(scene);
    }
    address = sdfResourceRetainAddress(name);
    if (address != NULL) {
        object->unk18 = name;
        object->unk1C = address;
        return 1;
    }
    return result;
}

/* Release the resource this scene object currently owns. */
void evtReleaseSceneResource(Scene *scene) {
    SceneObjectRes *object = (SceneObjectRes *)scene->object;

    if (object->unk18 != NULL) {
        func_002D0918(object->unk18);
    }
    object->unk18 = NULL;
    object->unk1C = NULL;
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
