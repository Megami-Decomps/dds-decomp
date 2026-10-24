#include "common.h"
#include "dds3obj.h"
#include "dds3_world_selection.h"
#include "dds3_world_value.h"
#include "sdf.h"
#include "dds3_world_counter.h"

extern dds3Basic_t *fldLocalMapCameraObject;
struct dds3Basic_t;

extern SdfQuad fldLocalMapFirstCameraVector;

extern SdfQuad fldLocalMapSecondCameraVector;




extern void dds3GetRadiusPoseSet(dds3Basic_t *object, f32 value);


extern dds3Basic_t *dds3AddCameraObjectBasicTypeFree(s32, u128 *, u128 *);

void fldCreateLocalMapCamera(void) {
    fldLocalMapCameraObject = dds3AddCameraObjectBasicTypeFree(dds3AllocObjectId(), (u128 *)&fldLocalMapFirstCameraVector, (u128 *)&fldLocalMapSecondCameraVector);
    dds3SetUnitObjectId(fldLocalMapCameraObject, "Lmap_Cam");
    dds3GetRadiusPoseSet(fldLocalMapCameraObject, 2.0f);
    dds3SetActiveCameraSceneBasic(dds3GetControlScene(), fldLocalMapCameraObject);
}

INCLUDE_RODATA(const s32, "game/code_002C3800", D_003B3D48);

INCLUDE_SDATA(const s32, "game/code_002C3800", fldLocalMapCameraObject);
