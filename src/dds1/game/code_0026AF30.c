#include "common.h"
#include "evt_world_object_api.h"
#include "dds3obj.h"
#include "eff_transform.h"

extern f32 D_003DC1C0[4];

extern f32 D_003DC1D0[4];



extern EffWorldNode *mnuTitleCameraObject;

extern void fldReleaseCameraColorEffect(void);

extern void sdfDisableFlagListViewTargetRelativeMode(void);

extern void evtDestroySecondaryWorldNode(void);

extern void dds3SetWorldObject(void *);
#include "dds3_world_secondary.h"
extern void mnuCreateTitleCameraWorldEntry(void);

extern u32 D_003BA8EC;

extern void fldInitializeCameraColorResource(void);

extern void sdfEnableFlagListViewTargetRelativeMode(void);

extern void fldApplyLightSetIndex(s32);

s32 mnuApplyInnerEffectVectorsAndTickObject(void) {
    effObjSetInnerPosition(mnuTitleCameraObject, (u128 *)D_003DC1C0);
    effObjSetInnerRotation(mnuTitleCameraObject, (u128 *)D_003DC1D0);
    return mnuTitleCameraObject->ops->update(mnuTitleCameraObject);
}

void mnuInitializeTitleCameraScene(void) {
    evtCreateWorldObjectForKey(1, 1);
    dds3SetWorldObject(NULL);
    mnuCreateTitleCameraWorldEntry();
    dds3SetWorldObject(dds3GetWorldSecondaryObject());

    D_003BA8EC = 0x80000000;
    D_003DC1C0[0] = 0.0f;
    D_003DC1C0[1] = -4000.0f;
    D_003DC1C0[2] = -4000.0f;
    D_003DC1C0[3] = 1.0f;
    D_003DC1D0[0] = -0.32f;
    D_003DC1D0[1] = 0.0f;
    D_003DC1D0[2] = 0.0f;
    D_003DC1D0[3] = -0.94f;

    fldInitializeCameraColorResource();
    sdfEnableFlagListViewTargetRelativeMode();
    fldApplyLightSetIndex(1);
}

void mnuMovieShutdownA(void) {
    fldReleaseCameraColorEffect();
    sdfDisableFlagListViewTargetRelativeMode();
    evtDestroySecondaryWorldNode();
}
