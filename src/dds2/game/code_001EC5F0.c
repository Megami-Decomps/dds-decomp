#include "common.h"
#include "btl.h"
#include "btl_command.h"
#include "btl_state.h"

extern void btlAdvanceSkillCameraCursor(BtlLinkedCommand *, BtlCamState *);
extern void btlInitializeRandomCameraCursor(BtlLinkedCommand *, BtlCamState *);
extern void btlAdvanceCommandCursor(BtlLinkedCommand *, BtlCamState *);
extern void btlResetUnitEffectVector(BtlLinkedCommand *, BtlCamState *);
extern void btlBuildGroupFramingCameraPose(BtlCamState *, BtlCamState *);
extern void btlFrameEarliestAnimatedUnitCameraPair(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

/* Advance the action's skill camera unless its native override flag is set. */
void btlAdvanceActionSkillCamera(BtlLinkedCommand *unit) {
    if (!(unit->flags & 0x10000)) {
        btlAdvanceSkillCameraCursor(unit, &unit->camera);
    }
}

/* Initialize the action's camera from the randomized global cursor choice. */
void btlInitializeRandomActionCamera(BtlLinkedCommand *action) {
    btlInitializeRandomCameraCursor(action, &action->camera);
}

void btlAdvanceCommandCursorTask(BtlLinkedCommand *action) {
    btlAdvanceCommandCursor(action, &action->camera);
}

/* Initialize front/back framing for the native marked-unit selection path. */
void btlInitializeMarkedUnitCameraPair(BtlLinkedCommand *action) {
    btlFrameEarliestAnimatedUnitCameraPair(action, &action->frontCamera, &action->backCamera);
}

extern void btlStepUnitHeightFramedCamera(BtlLinkedCommand *);

/* Advance the linked action's unit-height-framed camera. */
void btlUpdateLinkedHeightCamera(BtlLinkedCommand *action) {
    btlStepUnitHeightFramedCamera(action);
}

extern void btlBuildSpeciesFramedCameraPair(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

void btlAppendLinkedUnitToActorIndices(BtlLinkedCommand *action) {
    btlAppendIndexListEntry(action->targetList, action->link->unit);
    btlBuildSpeciesFramedCameraPair(action, &action->frontCamera, &action->backCamera);
}

void btlUpdateDevilizeCameraNoOp(void) {
}

void btlUpdateLinkedActionEffectVectorByTarget(BtlLinkedCommand *action) {
    if ((action->link->unit->status.flags & 0x200) != 0) {
        btlBuildGroupFramingCameraPose(&action->camera, &action->camera);
        return;
    }
    if (action->actionKind != 0x10) {
        btlResetUnitEffectVector(action, &action->camera);
        return;
    }
}

void btlUpdateEscapeCameraNoOp(void) {
}

void btlInitLinkedUnitActionCursor(ActionStateLink *linkState);

void btlInitializeCursorForLinkedAction(s32 action) {
    if (((BtlLinkedCommand *)action)->link != 0) {
        btlInitLinkedUnitActionCursor(((BtlLinkedCommand *)action)->link);
        return;
    }
}

void btlUpdateSingleActionCameraNoOp(void) {
}

extern void btlBuildActorRotatedCameraPresets(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

/* Initialize the action's front/back pair from presets rotated with its actor. */
void btlInitializeRotatedPresetCameraPair(BtlLinkedCommand *action) {
    btlBuildActorRotatedCameraPresets(action, &action->frontCamera, &action->backCamera);
}

typedef struct {
    u8 unk00[0x674];
    s32 (*allowDefaultSound)(void *);
} SoundEventCallbacks;

extern void btlInitializeActorClearanceCameraPair(BtlLinkedCommand *, BtlCamState *, BtlCamState *);

void btlRunDefaultActionPoseUnlessHooked(void *actor) {
    SoundEventCallbacks *callbacks = (SoundEventCallbacks *)btlGetRuntime();
    if (callbacks->allowDefaultSound && callbacks->allowDefaultSound(actor)) {
        return;
    }
    btlInitializeActorClearanceCameraPair(actor, &((BtlLinkedCommand *)actor)->frontCamera, &((BtlLinkedCommand *)actor)->backCamera);
}

/* Run the timed camera setup hook; return its result, or zero when unset. */
s32 btlRunTimedCameraSetupHook(BtlUnit *unit) {
    s32 (*hook)(BtlUnit *) = ((BtlState *)btlGetRuntime())->unk650;
    s32 result = 0;
    if (hook != 0) {
        result = hook(unit);
    }
    return result;
}

/* Run the timed camera finish hook; return its result, or zero when unset. */
s32 btlRunTimedCameraFinishHook(BtlUnit *unit) {
    s32 (*hook)(BtlUnit *) = ((BtlState *)btlGetRuntime())->unk658;
    s32 result = 0;
    if (hook != 0) {
        result = hook(unit);
    }
    return result;
}
