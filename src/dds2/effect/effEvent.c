#include "common.h"

extern s32 D_00436530;
extern u32 D_00436534;
extern u32 D_00436538;

void func_00197E40(u32 arg0) {
    func_001686F0(*(u32 *)((s32)arg0 + 0x34));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00197E70);

INCLUDE_ASM(const s32, "effect/effEvent", func_00197ED8);

void func_00197F40(s32 arg0) {
    func_00169168(*(u32 *)(arg0 + 0x34));
}

void func_00197F58(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x30) = arg1;
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00197F60);

INCLUDE_ASM(const s32, "effect/effEvent", func_00198278);

INCLUDE_ASM(const s32, "effect/effEvent", func_00198340);

INCLUDE_ASM(const s32, "effect/effEvent", func_00198380);

void func_00198448(s32 arg0) {
    func_00197F60(*(u32 *)(arg0 + 4));
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00198460);

void func_001984B8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x34) = arg1;
    func_00197E70(*(u32 *)(arg0 + 4), arg0 + 8);
}

INCLUDE_ASM(const s32, "effect/effEvent", func_001984D8);

INCLUDE_RODATA(const s32, "effect/effEvent", D_00414A00);

INCLUDE_ASM(const s32, "effect/effEvent", func_00198710);

void func_00198908(s32 arg0) {
    D_00436530 = D_00436530 - 1;
    if (D_00436530 == 0) {
        func_00159AF0(D_00436534);
        func_00159AF0(D_00436538);
    }
    func_003297C8(*(u32 *)(arg0 + 0x84));
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00198950);

INCLUDE_ASM(const s32, "effect/effEvent", func_00198D00);

INCLUDE_ASM(const s32, "effect/effEvent", func_00199118);

INCLUDE_ASM(const s32, "effect/effEvent", func_00199C50);

void func_00199C60(s32 arg0, u8 arg1) {
    *(u8 *)(arg0 + 0x80) = arg1;
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00199C68);

INCLUDE_ASM(const s32, "effect/effEvent", func_00199D48);

INCLUDE_ASM(const s32, "effect/effEvent", func_00199E68);

INCLUDE_ASM(const s32, "effect/effEvent", func_00199E88);

INCLUDE_ASM(const s32, "effect/effEvent", func_0019A058);


INCLUDE_SDATA(const s32, "effect/effEvent", D_00436530);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436534);

INCLUDE_SDATA(const s32, "effect/effEvent", D_00436538);


INCLUDE_SDATA(const s32, "effect/effEvent", D_0043653C);

