#include "common.h"

typedef struct SceneItem {
    u8 pad00[6];
    u16 left;
    u16 sourceLeft;
    u16 top;
    u16 sourceTop;
    u16 active;
} SceneItem;

extern void func_00265C28(void *, SceneItem *);
extern void soundSetSequenceVolumePan(s32, s32, s32);
extern void func_00262AC0(SceneItem *, void *);

void kwlnItemUpdateDisplay(u8 *scene) {
    SceneItem *item = *(SceneItem **)(*(s32 *)(scene + 0x98));
    switch (*(s32 *)(scene + 0x1588)) {
    case 4:
        func_00265C28(scene + 0x3D0, item);
        soundSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    case 1:
        item->left = item->sourceLeft;
        item->top = item->sourceTop;
        item->active = 0;
        soundSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    case 2:
        item->left = item->sourceLeft;
        soundSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    case 3:
        item->top = item->sourceTop;
        soundSetSequenceVolumePan(0x10, 0x7F, 0x3F);
        break;
    }
    func_00262AC0(item, scene);
}
