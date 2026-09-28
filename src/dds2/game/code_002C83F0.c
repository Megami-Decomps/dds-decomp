#include "common.h"

/* File request entry: D_003DC698 table, 0x64 bytes per entry. */
typedef struct FileReqEntry {
    u32 unk0;      /* 0x00 */
    u32 unk4;      /* 0x04 */
    u32 unk8;      /* 0x08 */
    u32 unkC;      /* 0x0C */
    u8 unk10;      /* 0x10 */
    u8 unk11;      /* 0x11 */
    u8 unk12;      /* 0x12 */
    s8 unk13;      /* 0x13 */
    u32 unk14[20]; /* 0x14 */
} FileReqEntry;

extern FileReqEntry D_00457F68[];

extern s32 D_00439000;

extern s32 (*D_00438BC0)(void);

void func_002C8F88(s32 arg0);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C83F0);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C85B0);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C8638);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C8878);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C8900);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C8AC0);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C8CB8);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C8D20);

u32 fileMan(void) {
    func_002C8D20();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C8EF0);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C8F88);

void func_002C8FD8(s32 arg0) {
    D_00439000 = arg0;
    func_002C8F88(arg0);
    D_00457F68[arg0].unk10 = 0;
}

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C9020);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C9118);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C9140);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C9168);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C9190);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C91B8);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C91E8);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C9218);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C9250);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C9280);

INCLUDE_ASM(const s32, "game/code_002C83F0", func_002C92A8);

void func_002C92D0(u32 arg0) {
    func_0034FCE0(arg0, 0);
}
