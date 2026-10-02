#include "common.h"

extern u8 D_003DC1C0[];

extern u8 D_003DC1D0[];

extern void *mnuTitleCameraObject;

extern void fldReleaseCameraColorEffect(void);

extern void func_002CF430(void);

extern void evtDestroySecondaryWorldNode(void);

extern void effObjSetInnerFirstVec(void *, void *);

extern void effObjSetInnerSecondVec(void *, void *);

s32 mnuApplyInnerEffectVectorsAndTickObject(void) {
    effObjSetInnerFirstVec(mnuTitleCameraObject, D_003DC1C0);
    effObjSetInnerSecondVec(mnuTitleCameraObject, D_003DC1D0);
    return (*(s32 (**)(void *))(*(s32 *)((u8 *)mnuTitleCameraObject + 0x10) + 8))(mnuTitleCameraObject);
}

INCLUDE_ASM(const s32, "game/code_0026AF30", func_0026AF78);

void mnuMovieShutdownA(void) {
    fldReleaseCameraColorEffect();
    func_002CF430();
    evtDestroySecondaryWorldNode();
}
