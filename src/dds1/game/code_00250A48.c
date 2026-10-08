#include "common.h"
#include "mnu_scene_work.h"

extern u32 *mnuGetSelectedNodeValue(void);
extern void mnuStopResourceAnimation(void);
extern void mnuResetResourceAnimation(void);
extern void mnuReinitializeSceneGrid(MenuSceneWork *work);

void mnuBeginTransition(MenuSceneWork *work, s32 mode) {
    MnuSceneListNode *transition = mnuAppendDisplayListNode(&work->sceneTransitionList);

    if (transition != NULL) {
        if (work->transitionBlendCounter >= 6) {
            mnuStopResourceAnimation();
            mnuReinitializeSceneGrid(work);
        } else if (work->transitionBlendCounter < -5) {
            mnuResetResourceAnimation();
            mnuReinitializeSceneGrid(work);
        }
        transition->payload.transition.mode = mode;
        if (mode == 1) {
            work->transitionBlendCounter = 10;
            transition->payload.transition.fromValue = *mnuGetSelectedNodeValue();
            mnuStopResourceAnimation();
            transition->payload.transition.toValue = *mnuGetSelectedNodeValue();
            mnuResetResourceAnimation();
            return;
        }
        if (mode == 2) {
            work->transitionBlendCounter = -10;
            transition->payload.transition.fromValue = *mnuGetSelectedNodeValue();
            mnuResetResourceAnimation();
            transition->payload.transition.toValue = *mnuGetSelectedNodeValue();
            mnuStopResourceAnimation();
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00250A48", func_00250B60);


