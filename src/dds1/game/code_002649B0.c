#include "common.h"

INCLUDE_ASM(const s32, "game/code_002649B0", func_002649B0);

extern void func_002639E0(s32);
extern void func_00263B78(s32, s32);

s64 func_00264A60(u64 request) {
    s32 context = func_00101A70();

    func_002639E0(context);
    func_00263B78(context, 0);
    return func_00285670(context + 8, (s32 *)(context + 0x54), 1, request);
}

extern s32 func_00285670(s32, s32 *, u64, u64);
extern s32 func_00101A70();

extern void func_0024DC98(s32);

s64 func_00264AB8(u64 request) {
    s32 context = func_00101A70();

    func_0024DC98(0);
    return func_00285670(context + 8, (s32 *)(context + 0x54), 2, request);
}

INCLUDE_ASM(const s32, "game/code_002649B0", func_00264B08);

INCLUDE_ASM(const s32, "game/code_002649B0", func_00264D90);

extern u32 func_002C1630(u32, u32, s32);

void func_00264E90(u8 *work) {
    s32 remaining = 0x100 - *(s32 *)(work + 0x1574);

    if (*(s8 *)(work + 0xD3C) == 0) {
        u32 color = func_002C1630(0x80808080, 0x80808000, remaining) & 0xFF;

        *(u32 *)(work + 0xD48) = color;
        if (color >= 0x80) {
            *(s8 *)(work + 0xD3C) = 1;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002649B0", func_00264EF0);

void func_00265078(void) {
}

void func_00265080(void) {
}

INCLUDE_ASM(const s32, "game/code_002649B0", func_00265088);

typedef struct {
    u8 pad00[0x1574];
    u32 state; /* 0x1574 */
} TitleWork;

u32 func_002650B0(TitleWork *work) {
    return work->state;
}

void func_002650B8(TitleWork *work) {
    work->state = 0;
}

void func_002650C0(void) {
}

INCLUDE_ASM(const s32, "game/code_002649B0", func_002650C8);

INCLUDE_ASM(const s32, "game/code_002649B0", func_00265220);

INCLUDE_ASM(const s32, "game/code_002649B0", func_002652E0);

INCLUDE_RODATA(const s32, "game/code_002649B0", D_003AFB20);

INCLUDE_RODATA(const s32, "game/code_002649B0", D_003AFB30);

INCLUDE_RODATA(const s32, "game/code_002649B0", D_003AFB40);

INCLUDE_SDATA(const s32, "game/code_002649B0", D_003BC560);

INCLUDE_SDATA(const s32, "game/code_002649B0", D_003BC568);

