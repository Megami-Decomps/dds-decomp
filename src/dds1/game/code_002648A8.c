#include "common.h"

typedef struct SceneItem {
    u8 pad00[6];
    u16 left;
    u16 sourceLeft;
    u16 top;
    u16 sourceTop;
    u16 active;
} SceneItem;

typedef struct {
    u8 pad00[0x98];
    SceneItem **itemSlot; /* 0x98 */
    u8 pad9C[0x14EC];
    s32 displayState; /* 0x1588 */
} TitleItemScene;

extern void func_00265C28(void *, SceneItem *);
extern void sndSetSequenceVolumePan(s32, s32, s32);
extern void func_00262AC0(SceneItem *, void *);

void kwlnItemUpdateDisplay(TitleItemScene *scene) {
    SceneItem *item = *scene->itemSlot;
    switch (scene->displayState) {
    case 4:
        func_00265C28((u8 *)scene + 0x3D0, item);
        sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    case 1:
        item->left = item->sourceLeft;
        item->top = item->sourceTop;
        item->active = 0;
        sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    case 2:
        item->left = item->sourceLeft;
        sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    case 3:
        item->top = item->sourceTop;
        sndSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    }
    func_00262AC0(item, scene);
}
