#include "common.h"
#include "sdf.h"

extern s32 D_003BD264;

extern SdfQuad D_003900A0;

extern SdfQuad D_003900B0;

extern s32 dds3AdvanceWorldCounter(void);

extern s32 dds3GetWorldSecondaryObject(void);

extern void dds3SetWorldEntryCallbackTarget(s32, char *);

extern void effObjSetInnerFloat(s32, f32);

extern void func_001109B8(s32, s32);

extern s32 func_00112C08(s32, SdfQuad *, SdfQuad *);

void func_002C3800(void) {
    D_003BD264 = func_00112C08(dds3AdvanceWorldCounter(), &D_003900A0, &D_003900B0);
    dds3SetWorldEntryCallbackTarget(D_003BD264, "Lmap_Cam");
    effObjSetInnerFloat(D_003BD264, 2.0f);
    func_001109B8(dds3GetWorldSecondaryObject(), D_003BD264);
}

INCLUDE_RODATA(const s32, "game/code_002C3800", D_003B3D48);

INCLUDE_SDATA(const s32, "game/code_002C3800", D_003BD264);

