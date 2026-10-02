#include "common.h"
#include "sdf.h"

extern s32 fldLocalMapCameraObject;

extern SdfQuad fldLocalMapFirstCameraVector;

extern SdfQuad fldLocalMapSecondCameraVector;

extern s32 dds3AdvanceWorldCounter(void);

extern s32 dds3GetWorldSecondaryObject(void);

extern void dds3SetWorldEntryCallbackTarget(s32, char *);

extern void effObjSetInnerFloat(s32, f32);

extern void dds3SetWorldCameraObject(s32, s32);

extern s32 func_00112C08(s32, SdfQuad *, SdfQuad *);

void fldCreateLocalMapCamera(void) {
    fldLocalMapCameraObject = func_00112C08(dds3AdvanceWorldCounter(), &fldLocalMapFirstCameraVector, &fldLocalMapSecondCameraVector);
    dds3SetWorldEntryCallbackTarget(fldLocalMapCameraObject, "Lmap_Cam");
    effObjSetInnerFloat(fldLocalMapCameraObject, 2.0f);
    dds3SetWorldCameraObject(dds3GetWorldSecondaryObject(), fldLocalMapCameraObject);
}

INCLUDE_RODATA(const s32, "game/code_002C3800", D_003B3D48);

INCLUDE_SDATA(const s32, "game/code_002C3800", fldLocalMapCameraObject);

