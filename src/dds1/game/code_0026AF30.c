#include "common.h"

extern f32 D_003DC1C0[4];

extern f32 D_003DC1D0[4];

typedef struct WorldCallbackTable {
    u8 pad00[8];
    s32 (*onFirst)(void *);
    s32 (*onSecond)(void *);
} WorldCallbackTable;

typedef struct WorldCallbackHolder {
    u8 pad00[0x10];
    WorldCallbackTable *callbacks;
} WorldCallbackHolder;

extern WorldCallbackHolder *mnuTitleCameraObject;

extern void fldReleaseCameraColorEffect(void);

extern void func_002CF430(void);

extern void evtDestroySecondaryWorldNode(void);

extern void effObjSetInnerFirstVec(void *, void *);

extern void effObjSetInnerSecondVec(void *, void *);

extern s32 evtCreateWorldObjectForKey(s32, s32);

extern void dds3SetWorldObject(void *);

extern void *dds3GetWorldSecondaryObject(void);

extern void mnuCreateTitleCameraWorldEntry(void);

extern u32 D_003BA8EC;

extern void fldInitializeCameraColorResource(void);

extern void func_002CF420(void);

extern void fldApplyLightSetIndex(s32);

s32 mnuApplyInnerEffectVectorsAndTickObject(void) {
    effObjSetInnerFirstVec(mnuTitleCameraObject, D_003DC1C0);
    effObjSetInnerSecondVec(mnuTitleCameraObject, D_003DC1D0);
    return mnuTitleCameraObject->callbacks->onFirst(mnuTitleCameraObject);
}

void func_0026AF78(void) {
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
    func_002CF420();
    fldApplyLightSetIndex(1);
}

void mnuMovieShutdownA(void) {
    fldReleaseCameraColorEffect();
    func_002CF430();
    evtDestroySecondaryWorldNode();
}
