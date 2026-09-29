#include "common.h"
#include "fpu.h"

extern s32 D_004388B4;

typedef struct SdfQuad {
    u32 word[4];
} SdfQuad;

extern SdfQuad D_00400970;

extern SdfQuad D_00400980;

extern s32 dds3AdvanceWorldCounter(void);

extern s32 dds3GetWorldSecondaryObject(void);

extern void dds3SetWorldEntryCallbackTarget(s32, char *);

extern void effObjSetInnerFloat(s32, f32);

extern void func_00110BE0(s32, s32);

extern s32 func_00112E30(s32, SdfQuad *, SdfQuad *);

extern char D_0042D348[];

void func_0030B7D0(void) {
    D_004388B4 = func_00112E30(dds3AdvanceWorldCounter(), &D_00400970, &D_00400980);
    dds3SetWorldEntryCallbackTarget(D_004388B4, D_0042D348);
    effObjSetInnerFloat(D_004388B4, 2.0f);
    func_00110BE0(dds3GetWorldSecondaryObject(), D_004388B4);
}

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388B4);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388B8);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388BC);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388C0);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388C1);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388C4);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388D0);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388D1);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388D8);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388E0);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388E8);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388F0);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_004388F8);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_00438900);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_00438908);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_00438910);

INCLUDE_SDATA(const s32, "game/code_0030B7D0", D_00438918);

