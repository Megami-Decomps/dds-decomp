#include "common.h"

extern u32 func_00265E68(u32, s32);

extern s32 mdlFlagTest(u32);

typedef struct LevelStep {
    u8 threshold;
    u8 value;
} LevelStep;

extern LevelStep D_00370D08[];

extern s32 D_003BAA00;

INCLUDE_ASM(const s32, "game/code_002653A0", func_002653A0);

extern void func_00264E90();
extern void func_00264EF0();
extern void func_00264B08();
extern void func_00264D90();

void func_00265478(u8 *work) {
    s32 y = 0x100 - *(s32 *)(work + 0x1574);

    func_00264E90(work);
    func_00264EF0(work);
    func_00264B08(work);
    func_00264D90(work);
    func_00266250(0x2C0, 0x3D8, 0, y, work + 0x3E4, 0x53);
    func_002653A0(work);
}

typedef struct {
    u8 pad00[4];
    u16 kind;       /* 0x04 */
    u8 pad06[0xE];
    u16 animation;  /* 0x14 */
} TitleEntry;

void func_002654E8(s32 arg0) {
    func_00265088(arg0);
}

u8 func_00265500(s32 value) {
    s32 i;

    for (i = 2; i >= 0; i--) {
        if (value >= D_00370D08[i].threshold) {
            return D_00370D08[i].value;
        }
    }
    return D_00370D08[0].value;
}

u8 func_00265540(s32 position, s32 increment) {
    u8 *table = (u8 *)D_00370D08;
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

extern s32 D_003BAA00;

s32 func_00265968(void) {
    s32 offset = 0;
    s32 count = 0;
    s32 remaining = 4;
    do {
        s32 step = func_002658B8(D_003BAA00 + 0xa60 + offset);
        count += step > 0;
        offset += 0x1a4;
    } while (--remaining >= 0);
    return count;
}

INCLUDE_ASM(const s32, "game/code_002653A0", func_002659C8);

INCLUDE_ASM(const s32, "game/code_002653A0", func_00265AB8);

s32 mnuAdvanceTitleEntryAnimation(TitleEntry *entry) {
    s32 step = func_002658B8(entry);
    entry->animation += step;
    func_002CD0C0(entry);
    return step;
}

s32 func_00265C28(u8 *src, u8 *obj) {
    s32 i;

    for (i = 0; i < 5; i++) {
        s8 *stat = (s8 *)(obj + 0x16 + i);

        *stat += src[i * 4];
        if (*stat >= 100) {
            *stat = 99;
        }
    }
    func_002CD0C0(obj);
    return 1;
}

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

extern u32 func_002C1630(u32, u32, s32);

u32 func_00266168(u32 a, u32 b, u32 c, s32 blend, u8 *resource) {
    func_002CD7B8(*(u32 *)(resource + 8));
    return func_002C1630(0x80808080, 0x80808000, blend);
}

INCLUDE_ASM(const s32, "game/code_002653A0", func_002661A8);

INCLUDE_ASM(const s32, "game/code_002653A0", func_00266250);

INCLUDE_RODATA(const s32, "game/code_002653A0", D_003AFBA0);
