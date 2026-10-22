#include "common.h"
#include "dds3obj.h"
#include "dds3_world_selection.h"
#include "dds3_world_value.h"
#include "sdf.h"
#include "dds3_world_counter.h"

extern EffWorldNode *fldLocalMapCameraObject;
struct EffWorldNode;

extern SdfQuad fldLocalMapFirstCameraVector;

extern SdfQuad fldLocalMapSecondCameraVector;




extern void dds3GetRadiusPoseSet(EffWorldNode *object, f32 value);


extern EffWorldNode *dds3AddCameraObjectBasicTypeFree(s32, SdfQuad *, SdfQuad *);

void fldCreateLocalMapCamera(void) {
    fldLocalMapCameraObject = dds3AddCameraObjectBasicTypeFree(dds3AllocObjectId(), &fldLocalMapFirstCameraVector, &fldLocalMapSecondCameraVector);
    dds3SetUnitObjectId(fldLocalMapCameraObject, (u32)"Lmap_Cam");
    dds3GetRadiusPoseSet(fldLocalMapCameraObject, 2.0f);
    dds3SetActiveCameraSceneBasic(dds3GetControlScene(), fldLocalMapCameraObject);
}

INCLUDE_RODATA(const s32, "game/code_002C3800", D_003B3D48);

INCLUDE_SDATA(const s32, "game/code_002C3800", fldLocalMapCameraObject);

