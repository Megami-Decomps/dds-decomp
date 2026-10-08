#include "common.h"
#include "mnu_scene_work.h"

extern u32 *mnuGetSelectedNodeValue(void);
extern void mnuStopResourceAnimation(void);
extern void mnuResetResourceAnimation(void);
extern void func_00253558(MenuSceneWork *work);

void mnuBeginTransitionAlt(MenuSceneWork *work, s32 mode) {
    MnuSceneListNode *transition = mnuAppendNodeToDisplayList(&work->displayList590);

    if (transition != NULL) {
        if (work->transitionState >= 6) {
            mnuStopResourceAnimation();
            func_00253558(work);
        } else if (work->transitionState < -5) {
            mnuResetResourceAnimation();
            func_00253558(work);
        }
        transition->value0C = mode;
        if (mode == 1) {
            work->transitionState = 10;
            transition->value04 = *mnuGetSelectedNodeValue();
            mnuStopResourceAnimation();
            transition->value08 = *mnuGetSelectedNodeValue();
            mnuResetResourceAnimation();
            return;
        }
        if (mode == 2) {
            work->transitionState = -10;
            transition->value04 = *mnuGetSelectedNodeValue();
            mnuResetResourceAnimation();
            transition->value08 = *mnuGetSelectedNodeValue();
            mnuStopResourceAnimation();
        }
    }
}
