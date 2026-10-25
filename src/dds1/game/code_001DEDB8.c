#include "common.h"
#include "btl.h"
#include "btl_command.h"
#include "btl_runtime.h"

extern void btlAdvanceSkillCameraCursor(BtlLinkedCommand *, BtlCamState *);
extern void btlInitializeRandomCameraCursor(BtlLinkedCommand *, BtlCamState *);
extern void btlAdvanceCommandCursor(BtlLinkedCommand *, BtlCamState *);
extern void fldCamSetChase(BtlLinkedCommand *, BtlCamState *);
extern void btlInitLinkedUnitActionCursor(BtlTask *);
extern void btlFlagUnitDefeatCandidate(BtlUnit *);

/* Advance the action's skill camera unless its native override flag is set. */
void btlAdvanceActionSkillCamera(BtlLinkedCommand *action) {
    if ((action->flags & 0x10000) != 0) {
        return;
    }
    btlAdvanceSkillCameraCursor(action, &action->camera);
}

/* Initialize the action's camera from the randomized global cursor choice. */
void btlInitializeRandomActionCamera(BtlLinkedCommand *action) {
    btlInitializeRandomCameraCursor(action, &action->camera);
}

void btlAdvanceCommandCursorTask(BtlLinkedCommand *action) {
    btlAdvanceCommandCursor(action, &action->camera);
}

extern void btlFrameEarliestAnimatedUnitCameraPair(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

/* Initialize front/back framing for the native marked-unit selection path. */
void btlInitializeMarkedUnitCameraPair(BtlLinkedCommand *action) {
    btlFrameEarliestAnimatedUnitCameraPair(action, &action->frontCamera, &action->backCamera);
}

extern void btlStepUnitHeightFramedCamera(BtlLinkedCommand *);

/* Advance the linked action's unit-height-framed camera. */
void btlUpdateLinkedHeightCamera(BtlLinkedCommand *action) {
    btlStepUnitHeightFramedCamera(action);
}

extern void btlBuildRandomDefeatCandidateCameraPair(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

extern void btlClearAllUnitDefeatCandidates(void);

void btlStartLinkedDefeatCandidateAction(BtlLinkedCommand *action) {
    BtlTask *task = action->task;
    btlAppendIndexListEntry(action->targetList, task->unit);
    btlBuildRandomDefeatCandidateCameraPair(action, &action->frontCamera, &action->backCamera);
    btlClearAllUnitDefeatCandidates();
    task = action->task;
    btlFlagUnitDefeatCandidate(task->unit);
    action->motionParameter = 50.0f;
    action->flags |= 0x41;
}

void btlUpdateDevilizeCameraNoOp(void) {
}

extern void btlBuildGroupFramingCameraPose(BtlCamState *, BtlCamState *);

void btlUpdateLinkedActionEffectVectorByTarget(BtlLinkedCommand *action) {
    if ((action->task->unit->status.flags & 0x200) != 0) {
        btlBuildGroupFramingCameraPose(&action->camera, &action->camera);
        return;
    }
    if (action->actionKind != 0x10) {
        fldCamSetChase(action, &action->camera);
        return;
    }
}

void btlUpdateEscapeCameraNoOp(void) {
}

void btlInitializeCursorForLinkedAction(BtlLinkedCommand *action) {
    if (action->task != 0) {
        btlInitLinkedUnitActionCursor(action->task);
        return;
    }
}

void btlUpdateSingleActionCameraNoOp(void) {
}

/* Run the timed camera setup hook; return its result, or zero when unset. */
s32 btlRunTimedCameraSetupHook(BtlLinkedCommand *action) {
    s32 context = (s32)(u32)btlGetRuntime();
    s32 (*callback)(BtlLinkedCommand *) = *(void **)(context + 0x618);
    if (callback != 0) {
        return callback(action);
    }
    return 0;
}

/* Run the timed camera finish hook; return its result, or zero when unset. */
s32 btlRunTimedCameraFinishHook(BtlLinkedCommand *action) {
    s32 context = (s32)(u32)btlGetRuntime();
    s32 (*callback)(BtlLinkedCommand *) = *(void **)(context + 0x620);
    if (callback != 0) {
        return callback(action);
    }
    return 0;
}
