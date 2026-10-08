#include "common.h"
#include "mnu_mantra_grid.h"

extern void mnuDrawMantraPanelSprite();
extern void mnuDrawMantraCostCounter(s32, s32, s32, s32, u16, s32);
extern void func_00255368(s32, s32, s32, s32, u16, s32);

typedef struct DspEntryLink {
    u8 pad00[4];
    MnuMantraGridEntry *entry;
} DspEntryLink;

typedef struct DspEntryContainer {
    u8 pad00[8];
    DspEntryLink *link;
} DspEntryContainer;

typedef struct DspDisplayObject {
    u8 pad00[0x484];
    DspEntryContainer *entries;
    u8 pad488[0xC0];
    s32 busy;
} DspDisplayObject;

void mnuDrawSelectedMantraEntry(DspDisplayObject *object, s32 alpha, s32 context) {
    MnuMantraGridEntry *entry = object->entries->link->entry;

    if (entry != NULL) {
        if (object->busy == 0) {
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


