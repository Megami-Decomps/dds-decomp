#include "common.h"

extern s32 func_00101A70();

INCLUDE_ASM(const s32, "game/code_00279328", func_00279328);

INCLUDE_ASM(const s32, "game/code_00279328", func_002793D0);

INCLUDE_ASM(const s32, "game/code_00279328", func_00279568);

INCLUDE_ASM(const s32, "game/code_00279328", func_00279728);

INCLUDE_ASM(const s32, "game/code_00279328", func_00279760);

INCLUDE_ASM(const s32, "game/code_00279328", func_00279860);

INCLUDE_ASM(const s32, "game/code_00279328", func_002798F8);

INCLUDE_ASM(const s32, "game/code_00279328", func_00279A30);

INCLUDE_ASM(const s32, "game/code_00279328", func_00279AF8);

INCLUDE_ASM(const s32, "game/code_00279328", func_00279B30);

s32 func_00279BA8(s32 selection) {
    s32 context = func_00101A70();
    func_00277C80(selection);
    func_002786E8(context);
    func_002807E8(context + 0x15c);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00279328", func_00279BF8);
