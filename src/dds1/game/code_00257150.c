#include "common.h"
#include "mnu_mantra_grid.h"
#include "mnu_scene_work.h"

extern void mnuDrawMantraPanelSprite(s32, s32, s32, s32, u16, s32);
extern void mnuDrawMantraCostCounter(s32, s32, s32, s32, u16, s32);
extern void func_00255368(s32, s32, s32, s32, u16, s32);

void mnuDrawSelectedMantraEntry(MenuSceneWork *object, s32 alpha, s32 context) {
    MnuMantraGridEntry *entry =
        (MnuMantraGridEntry *)(u32)object->gridHandle->cursor->value;

    if (entry != NULL) {
        if (object->unk548 == 0) {
            if (entry->state != 3) {
                if (entry->profileFlag != 0) {
                    func_00255368(0, 0, 1, alpha, entry->sceneId, context);
                } else {
                    mnuDrawMantraCostCounter(0, 0, 1, alpha, entry->sceneId, context);
                }
                return;
            }
            mnuDrawMantraPanelSprite(0, 0, 1, alpha, entry->sceneId, context);
        }
    }
}


