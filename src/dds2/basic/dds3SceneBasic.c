#include "common.h"
#include "dds3obj.h"
#include "kwln.h"

extern void *func_00343ED0(void *resource, u32 *resolvedId, s32 options);
extern void *sdfResourceRetainAddress(void *resource);
extern void func_003297C8(void *arg);
extern s32 bfFindScriptIndexByName(void *, const char *);
extern KwlnTask *func_00101740(const char *name);
extern s32 scrCreateTaskForProcessId(s32, s32, s32);
extern void func_00110EE8(Scene *);

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
INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_00110E08);

/* Retain `name` and hand its address to the scene object. */
s32 func_00110E80(Scene *scene, void *name) {
    s32 result = 0;
    void *address;
    SceneObjectRes *object = (SceneObjectRes *)scene->object;

    if (name == NULL) {
        return result;
    }
    if (object->unk18 != NULL) {
        func_00110EE8(scene);
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
void func_00110EE8(Scene *scene) {
    SceneObjectRes *object = (SceneObjectRes *)scene->object;

    if (object->unk18 != NULL) {
        func_003297C8(object->unk18);
    }
    object->unk18 = NULL;
    object->unk1C = NULL;
}

/* Starts the named script task on the scene object's resource; stays asm:
   retail's beqz/b merge of the two exit paths has no plain-C shape. */
INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_00110F28);

/* Destroys the task named by `name`; stays asm: retail keeps the tail call as
   jal+epilogue, which needs a nosibcall flag this unit does not carry. */
INCLUDE_ASM(const s32, "basic/dds3SceneBasic", func_00110FB0);
