#include "common.h"
#include "dds3obj.h"
#include "dds3_world_selection.h"
#include "dds3_world_value.h"
#include "fpu.h"
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
    dds3ReplaceActiveCameraSceneBasic(dds3GetControlScene(), fldLocalMapCameraObject);
}

INCLUDE_SDATA(const s32, "game/code_0030B7D0", fldLocalMapCameraObject);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", sdfSelectedCounterIndex);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", sdfCounterSelectionCount);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388C0);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388C1);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", sdfActiveCounterRuntime);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388D0);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388D1);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388D8);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388E0);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388E8);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388F0);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388F8);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_00438900);
