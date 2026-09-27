#include "common.h"

extern s32 D_003BB140;
extern u32 D_003BB144;
extern u32 D_003BB148;

void func_00190208(u32 arg0) {
    func_00160B00(*(u32 *)((s32)arg0 + 0x34));
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00190238);

INCLUDE_ASM(const s32, "effect/effEvent", func_001902A0);

void func_00190308(s32 arg0) {
    func_00161588(*(u32 *)(arg0 + 0x34));
}

void func_00190320(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x30) = arg1;
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00190328);

INCLUDE_ASM(const s32, "effect/effEvent", func_00190640);

INCLUDE_ASM(const s32, "effect/effEvent", func_00190708);

INCLUDE_ASM(const s32, "effect/effEvent", func_00190748);

void func_00190810(s32 arg0) {
    func_00190328(*(u32 *)(arg0 + 4));
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00190828);

void func_00190880(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x34) = arg1;
    func_00190238(*(u32 *)(arg0 + 4), arg0 + 8);
}

INCLUDE_ASM(const s32, "effect/effEvent", func_001908A0);

INCLUDE_RODATA(const s32, "effect/effEvent", D_003A12E0);

INCLUDE_ASM(const s32, "effect/effEvent", func_00190AD8);

void func_00190CD0(s32 arg0) {
    D_003BB140 = D_003BB140 - 1;
    if (D_003BB140 == 0) {
        func_00151F00(D_003BB144);
        func_00151F00(D_003BB148);
    }
    func_002D0918(*(u32 *)(arg0 + 0x84));
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00190D18);

INCLUDE_ASM(const s32, "effect/effEvent", func_001910C8);

INCLUDE_ASM(const s32, "effect/effEvent", func_001914E0);

INCLUDE_ASM(const s32, "effect/effEvent", func_00192018);

void func_00192028(s32 arg0, u8 arg1) {
    *(u8 *)(arg0 + 0x80) = arg1;
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00192030);

INCLUDE_ASM(const s32, "effect/effEvent", func_00192110);

INCLUDE_ASM(const s32, "effect/effEvent", func_00192230);

INCLUDE_ASM(const s32, "effect/effEvent", func_00192250);

INCLUDE_ASM(const s32, "effect/effEvent", func_00192420);
