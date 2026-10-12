#include "common.h"
#include "mnu_scene_work.h"

extern u32 *mnuGetSelectedNodeValue(void);
extern void mnuStopResourceAnimation(void);
extern void mnuResetResourceAnimation(void);
extern void mnuRestoreMantraGridSelection(MenuSceneWork *work);

void mnuBeginTransitionAlt(MenuSceneWork *work, s32 mode) {
    MnuSceneListNode *transition = mnuAppendNodeToDisplayList(&work->costTransitionList);

    if (transition != NULL) {
        if (work->transitionBlendCounter >= 6) {
            mnuStopResourceAnimation();
            mnuRestoreMantraGridSelection(work);
        } else if (work->transitionBlendCounter < -5) {
            mnuResetResourceAnimation();
            mnuRestoreMantraGridSelection(work);
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
