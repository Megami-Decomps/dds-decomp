#include "common.h"
#include "dds3_world_selection.h"
#include "evt_world_object_api.h"
#include "dds3obj.h"
#include "eff_transform.h"

extern f32 mnuTitleCameraPosition[4];

extern f32 mnuTitleCameraRotation[4];



extern EffWorldNode *mnuTitleCameraObject;

extern void fldReleaseCameraColorEffect(void);

extern void sdfDisableFlagListViewTargetRelativeMode(void);

extern void evtDestroySecondaryWorldNode(void);

#include "dds3_world_secondary.h"
extern void mnuCreateTitleCameraWorldEntry(void);

extern u32 D_003BA8EC;

extern void fldInitializeCameraColorResource(void);

extern void sdfEnableFlagListViewTargetRelativeMode(void);

extern void fldApplyLightSetIndex(s32);

s32 mnuApplyInnerEffectVectorsAndTickObject(void) {
    dds3SetPosPoseSet(mnuTitleCameraObject, (u128 *)mnuTitleCameraPosition);
    dds3SetRotPoseSet(mnuTitleCameraObject, (u128 *)mnuTitleCameraRotation);
    return mnuTitleCameraObject->ops->update(mnuTitleCameraObject);
}

void mnuInitializeTitleCameraScene(void) {
    evtCreateWorldObjectForKey(1, 1);
    dds3SetWorldObject(NULL);
    mnuCreateTitleCameraWorldEntry();
    dds3SetWorldObject(dds3GetWorldSecondaryObject());

    D_003BA8EC = 0x80000000;
    mnuTitleCameraPosition[0] = 0.0f;
    mnuTitleCameraPosition[1] = -4000.0f;
    mnuTitleCameraPosition[2] = -4000.0f;
    mnuTitleCameraPosition[3] = 1.0f;
    mnuTitleCameraRotation[0] = -0.32f;
    mnuTitleCameraRotation[1] = 0.0f;
    mnuTitleCameraRotation[2] = 0.0f;
    mnuTitleCameraRotation[3] = -0.94f;

    fldInitializeCameraColorResource();
    sdfEnableFlagListViewTargetRelativeMode();
    fldApplyLightSetIndex(1);
}

void mnuMovieShutdownA(void) {
    fldReleaseCameraColorEffect();
    sdfDisableFlagListViewTargetRelativeMode();
    evtDestroySecondaryWorldNode();
}
