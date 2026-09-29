#include "common.h"

extern u32 func_00265E68(u32, s32);

extern s32 mdlFlagTest(u32);

extern u8 D_00370D08[];

extern s32 D_003BAA00;

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

u8 func_00265540(s32 position, s32 increment) {
    u8 *table = D_00370D08;
    s32 i = 2;
    u8 *limit = table + 4;
    s32 end = position + increment;
    do {
        if (position < *limit && end >= *limit) {
            return limit[1];
        }
        limit -= 2;
    } while (--i >= 0);
    return 0;
}

u32 func_00265590(void) {
    return 1;
}

u32 func_00265598(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002653A0", func_002655A0);

INCLUDE_ASM(const s32, "game/code_002653A0", func_00265610);

INCLUDE_ASM(const s32, "game/code_002653A0", func_00265648);

s32 mnuIsTitleEntryAvailable(TitleEntry *entry) {
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

s32 mnuAdvanceTitleEntryAnimation(TitleEntry *entry) {
    s32 step = func_002658B8(entry);
    entry->animation += step;
    func_002CD0C0(entry);
    return step;
}

INCLUDE_ASM(const s32, "game/code_002653A0", func_00265C28);

INCLUDE_ASM(const s32, "game/code_002653A0", func_00265C90);

INCLUDE_ASM(const s32, "game/code_002653A0", func_00265E68);

void titleInitFourParameters(u32 *state, u32 first, u32 second, u32 third, u32 fourth) {
    memset(state, 0, 0x10);
    state[0] = first;
    state[1] = second;
    state[2] = third;
    state[3] = fourth;
}

INCLUDE_ASM(const s32, "game/code_002653A0", func_00266048);

void func_00266130(u32 fontContext) {
    func_001953D8(fontContext, 0xc, 0x10);
    func_001953A8(fontContext, 0xfffffffffffffffc);
}

INCLUDE_ASM(const s32, "game/code_002653A0", func_00266168);

INCLUDE_ASM(const s32, "game/code_002653A0", func_002661A8);

INCLUDE_ASM(const s32, "game/code_002653A0", func_00266250);

INCLUDE_RODATA(const s32, "game/code_002653A0", D_003AFBA0);
