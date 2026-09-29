#include "common.h"

extern s32 func_00101A70(void);
extern s64 func_00285670(s32, s32 *, u32, s32);

extern s32 func_00101A70();

INCLUDE_ASM(const s32, "game/code_00279328", func_00279328);

INCLUDE_ASM(const s32, "game/code_00279328", ptySkillMenuCopyPageState);

INCLUDE_ASM(const s32, "game/code_00279328", func_00279568);

void func_00279728(s32 selection) {
    s32 context = func_00101A70();
    func_00285670(context + 8, (s32 *)(context + 0x54), 2, selection);
}

INCLUDE_ASM(const s32, "game/code_00279328", ptySkillMenuUseSelectedInField);

INCLUDE_ASM(const s32, "game/code_00279328", func_00279860);

INCLUDE_ASM(const s32, "game/code_00279328", func_002798F8);

INCLUDE_ASM(const s32, "game/code_00279328", func_00279A30);

void func_00279AF8(s32 selection) {
    s32 context = func_00101A70();
    func_00285670(context + 8, (s32 *)(context + 0x54), 2, selection);
}

extern void func_002806E8(void *, u32);
extern void ptySkillMenuBuildEquippedSlots(s32, s32);
extern void ptySkillMenuInitPages(void *);

s32 func_00279B30(s32 arg0) {
    u8 *ctx = (u8 *)func_00101A70();
    u32 *p = (u32 *)(ctx + 0x15C);

    func_002806E8(p, *(u32 *)(*(u32 *)(*(u32 *)(ctx + 0x7D8) + 0x1C)));
    *p |= 0x400;
    ptySkillMenuBuildEquippedSlots(0, arg0);
    ptySkillMenuInitPages(ctx);
    return 1;
}

s32 func_00279BA8(s32 selection) {
    s32 context = func_00101A70();
    func_00277C80(selection);
    func_002786E8(context);
    func_002807E8(context + 0x15c);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00279328", ptySkillMenuHandlePageSwitch);
