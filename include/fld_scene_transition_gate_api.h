#ifndef FLD_SCENE_TRANSITION_GATE_API_H
#define FLD_SCENE_TRANSITION_GATE_API_H

#include "common.h"

/* Nonzero delays next-floor work while the scene becomes ready or hands off. */
typedef u8 FldSceneTransitionGateResult;
enum {
    FLD_SCENE_TRANSITION_GATE_PROCEED = 0,
    FLD_SCENE_TRANSITION_GATE_SCENE_READY = 1,
    FLD_SCENE_TRANSITION_GATE_HANDOFF_DELAY = 2
};

FldSceneTransitionGateResult fldProcAmChk(void);

#endif /* FLD_SCENE_TRANSITION_GATE_API_H */
