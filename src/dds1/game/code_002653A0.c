#include "common.h"

extern u32 func_00265E68(u32, s32);

extern s32 mdlFlagTest(u32);

INCLUDE_ASM(const s32, "game/code_002653A0", func_002653A0);

INCLUDE_ASM(const s32, "game/code_002653A0", func_00265478);

typedef struct {
    u8 pad00[4];
    u16 kind;       /* 0x04 */
    u8 pad06[0xE];
    u16 animation;  /* 0x14 */
} TitleEntry;

void func_002654E8(s32 arg0) {
    func_00265088(arg0);
}

INCLUDE_ASM(const s32, "game/code_002653A0", func_00265500);

INCLUDE_ASM(const s32, "game/code_002653A0", func_00265540);

u32 func_00265590(void) {
    return 1;
}

u32 func_00265598(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002653A0", func_002655A0);

INCLUDE_ASM(const s32, "game/code_002653A0", func_00265610);

INCLUDE_ASM(const s32, "game/code_002653A0", func_00265648);

s32 isTitleEntryAvailable(TitleEntry *entry) {
    if (mdlFlagTest(0x902) == 0 && entry->kind == 4) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002653A0", func_00265700);

INCLUDE_ASM(const s32, "game/code_002653A0", func_002658B8);

INCLUDE_ASM(const s32, "game/code_002653A0", func_00265968);

INCLUDE_ASM(const s32, "game/code_002653A0", func_002659C8);

INCLUDE_ASM(const s32, "game/code_002653A0", func_00265AB8);

s32 advanceTitleEntryAnimation(TitleEntry *entry) {
    s32 step = func_002658B8(entry);
    entry->animation += step;
    func_002CD0C0(entry);
    return step;
}

INCLUDE_ASM(const s32, "game/code_002653A0", func_00265C28);

INCLUDE_ASM(const s32, "game/code_002653A0", func_00265C90);

INCLUDE_ASM(const s32, "game/code_002653A0", func_00265E68);

INCLUDE_ASM(const s32, "game/code_002653A0", func_00265FD8);

INCLUDE_ASM(const s32, "game/code_002653A0", func_00266048);

void func_00266130(u32 arg0) {
    func_001953D8(arg0, 0xc, 0x10);
    func_001953A8(arg0, 0xfffffffffffffffc);
}

INCLUDE_ASM(const s32, "game/code_002653A0", func_00266168);

INCLUDE_ASM(const s32, "game/code_002653A0", func_002661A8);

INCLUDE_ASM(const s32, "game/code_002653A0", func_00266250);

INCLUDE_RODATA(const s32, "game/code_002653A0", D_003AFBA0);

