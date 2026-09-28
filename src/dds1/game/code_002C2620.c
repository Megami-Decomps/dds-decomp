#include "common.h"

extern void func_002C2FF8(void);
extern void kwlnTaskDestroyWithHierarchyByName(char *, s32);

extern char D_003B3CA0[]; /* "LmapMain" */

extern s32 kwlnTaskGetTaskByName(u32);

extern u32 D_003BD25C;

extern u32 D_003BD260;

extern u32 D_003BD970;

extern u32 D_003BD974;

extern s32 mdlFlagTest(u32);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C2620);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C2658);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C26C8);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C2768);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C27F8);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C2870);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C28E0);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C2A20);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C2BF8);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C2CC0);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C2DA0);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C2E38);

void fldStopLmapTask(void) {
    func_002C2FF8();
    kwlnTaskDestroyWithHierarchyByName(D_003B3CA0, 1);
}

s32 fldLmapTaskExists(void) {
    return kwlnTaskGetTaskByName((u32)D_003B3CA0) != 0;
}

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C2EF8);

INCLUDE_RODATA(const s32, "game/code_002C2620", D_003B3CA0);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C2F40);

void func_002C2FC0(void) {
    s64 temp_v0;

    D_003BD25C = 1;
    D_003BD970 = 0;
    D_003BD974 = 0;
    temp_v0 = mdlFlagTest(0x413);
    D_003BD260 = (u32)(temp_v0 == 0);
}

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C2FF8);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C3060);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C30F0);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C3220);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C3420);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C3510);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C35C8);

u32 func_002C3640(void) {
    s64 temp_v0;
    u32 temp_v1;

    temp_v0 = mdlFlagTest(0x412);
    temp_v1 = 2;
    if (temp_v0 == 0) {
        temp_v0 = mdlFlagTest(0x411);
        temp_v1 = 1;
        if (temp_v0 == 0) {
            mdlFlagTest(0x410);
            temp_v1 = 0;
        }
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C3690);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C36E8);

INCLUDE_ASM(const s32, "game/code_002C2620", func_002C3738);

INCLUDE_SDATA(const s32, "game/code_002C2620", D_003BD238);

INCLUDE_SDATA(const s32, "game/code_002C2620", D_003BD23C);

INCLUDE_SDATA(const s32, "game/code_002C2620", D_003BD240);

INCLUDE_SDATA(const s32, "game/code_002C2620", D_003BD248);

INCLUDE_SDATA(const s32, "game/code_002C2620", D_003BD250);

INCLUDE_SDATA(const s32, "game/code_002C2620", D_003BD254);

INCLUDE_SDATA(const s32, "game/code_002C2620", D_003BD258);

INCLUDE_SDATA(const s32, "game/code_002C2620", D_003BD25C);

INCLUDE_SDATA(const s32, "game/code_002C2620", D_003BD260);

