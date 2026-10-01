#include "common.h"
#include "fpu.h"
#include "sdf.h"

extern s32 D_004388B4;

extern SdfQuad D_00400970;

extern SdfQuad D_00400980;

extern s32 dds3AdvanceWorldCounter(void);

extern s32 dds3GetWorldSecondaryObject(void);

extern void dds3SetWorldEntryCallbackTarget(s32, char *);

extern void effObjSetInnerFloat(s32, f32);

extern void dds3SetWorldCameraObject(s32, s32);

extern s32 func_00112E30(s32, SdfQuad *, SdfQuad *);

void fldCreateLocalMapCamera(void) {
    D_004388B4 = func_00112E30(dds3AdvanceWorldCounter(), &D_00400970, &D_00400980);
    dds3SetWorldEntryCallbackTarget(D_004388B4, "Lmap_Cam");
    effObjSetInnerFloat(D_004388B4, 2.0f);
    dds3SetWorldCameraObject(dds3GetWorldSecondaryObject(), D_004388B4);
}

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388B4);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", sdfSelectedCounterIndex);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388BC);

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

