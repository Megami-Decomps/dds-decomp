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




extern void effObjSetInnerFloat(EffWorldNode *object, f32 value);


extern EffWorldNode *dds3CreateCameraObject(s32, SdfQuad *, SdfQuad *);

void fldCreateLocalMapCamera(void) {
    fldLocalMapCameraObject = dds3CreateCameraObject(dds3AdvanceWorldCounter(), &fldLocalMapFirstCameraVector, &fldLocalMapSecondCameraVector);
    dds3SetWorldNodeValue(fldLocalMapCameraObject, (u32)"Lmap_Cam");
    effObjSetInnerFloat(fldLocalMapCameraObject, 2.0f);
    dds3SetWorldCameraObject(dds3GetWorldSecondaryObject(), fldLocalMapCameraObject);
}

INCLUDE_RODATA(const s32, "game/code_002C3800", D_003B3D48);

INCLUDE_SDATA(const s32, "game/code_002C3800", fldLocalMapCameraObject);

