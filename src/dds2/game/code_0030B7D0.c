#include "common.h"
#include "fpu.h"
#include "sdf.h"

extern s32 fldLocalMapCameraObject;
struct EffWorldNode;

extern SdfQuad fldLocalMapFirstCameraVector;

extern SdfQuad fldLocalMapSecondCameraVector;

extern u32 dds3AdvanceWorldCounter(void);

extern s32 dds3GetWorldSecondaryObject(void);

extern void dds3SetWorldNodeValue(struct EffWorldNode *node, u32 value);

extern void effObjSetInnerFloat(s32, f32);

extern void dds3SetWorldCameraObject(s32, s32);

extern s32 dds3CreateCameraObject(s32, SdfQuad *, SdfQuad *);

void fldCreateLocalMapCamera(void) {
    fldLocalMapCameraObject = dds3CreateCameraObject(dds3AdvanceWorldCounter(), &fldLocalMapFirstCameraVector, &fldLocalMapSecondCameraVector);
    dds3SetWorldNodeValue((struct EffWorldNode *)fldLocalMapCameraObject, (u32)"Lmap_Cam");
    effObjSetInnerFloat(fldLocalMapCameraObject, 2.0f);
    dds3SetWorldCameraObject(dds3GetWorldSecondaryObject(), fldLocalMapCameraObject);
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

