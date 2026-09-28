#include "common.h"

extern s32 func_0030AC10(void);

extern u32 D_004388AC;

extern u32 D_004388B0;

extern u32 D_0043908C;

extern u32 D_00439090;

extern s32 mdlFlagTest(u32);

extern s32 func_00101740(u32);

extern char D_0042D240[]; /* "LmapMain" */

extern void func_0030AB88(void);

extern void kwlnTaskDestroyWithHierarchyByName(char *, s32);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A128);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A160);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A1D0);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A270);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A300);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A378);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A3E8);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A528);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A700);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A7C8);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A8A8);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A970);

void func_0030AA10(void) {
    func_0030AB88();
    kwlnTaskDestroyWithHierarchyByName(D_0042D240, 1);
}

s32 func_0030AA40(void) {
    return func_00101740((u32)D_0042D240) != 0;
}

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030AA68);

INCLUDE_RODATA(const s32, "game/code_0030A128", D_0042D240);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030AAB0);

void func_0030AB20(s32 arg0) {
    s64 temp_v0;
    u32 temp_v1;

    D_0043908C = 0;
    D_004388AC = 1;
    D_00439090 = 0;
    D_004388B0 = 0;
    temp_v0 = mdlFlagTest(0x1c);
    temp_v1 = 3;
    if (temp_v0 == 0) {
        temp_v0 = mdlFlagTest(0x13);
        temp_v1 = 2;
        if (temp_v0 == 0) {
            temp_v1 = 1;
        }
    }
    *(u32 *)(arg0 + 8) = temp_v1;
    *(u32 *)(arg0 + 4) = 0;
}

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030AB88);

u8 func_0030ABF0(void) {
    s64 temp_v0;

    temp_v0 = func_0030AC10();
    return temp_v0 != 0;
}

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030AC10);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030B078);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030B1E8);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030B470);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030B568);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030B600);

u32 func_0030B678(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030B680);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030B6D8);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030B728);

INCLUDE_RODATA(const s32, "game/code_0030A128", D_0042D348);

INCLUDE_RODATA(const s32, "game/code_0030A128", D_0042D358);

INCLUDE_RODATA(const s32, "game/code_0030A128", D_0042D368);

INCLUDE_RODATA(const s32, "game/code_0030A128", D_0042D3D8);

INCLUDE_SDATA(const s32, "game/code_0030A128", D_00438888);

INCLUDE_SDATA(const s32, "game/code_0030A128", D_0043888C);

INCLUDE_SDATA(const s32, "game/code_0030A128", D_00438890);

INCLUDE_SDATA(const s32, "game/code_0030A128", D_00438894);

INCLUDE_SDATA(const s32, "game/code_0030A128", D_00438898);

INCLUDE_SDATA(const s32, "game/code_0030A128", D_004388A0);

INCLUDE_SDATA(const s32, "game/code_0030A128", D_004388A4);

INCLUDE_SDATA(const s32, "game/code_0030A128", D_004388A8);

INCLUDE_SDATA(const s32, "game/code_0030A128", D_004388AC);

INCLUDE_SDATA(const s32, "game/code_0030A128", D_004388B0);

