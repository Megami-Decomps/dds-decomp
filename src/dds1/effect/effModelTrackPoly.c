#include "common.h"

void func_00188228(u32 arg0) {
    func_00188820(*(u32 *)((s32)arg0 + 0x38));
    func_002CFF98(arg0);
}

void func_00188258(s32 arg0) {
    *(u32 *)(arg0 + 0x34) = 0;
    func_00188850(*(u32 *)(arg0 + 0x38));
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188278);

void func_001882D8(s32 arg0) {
    func_00188A78(*(u32 *)(arg0 + 0x38));
}

void func_001882F0(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x38) + 4) = arg1;
}

void func_00188300(s32 arg0) {
    func_00188E10(*(u32 *)(arg0 + 0x38));
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188318);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001883E0);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188480);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001884E8);

void func_00188510(s32 *arg0, s32 arg1) {
    func_00188258(*(u32 *)(arg1 * 4 + *arg0));
}

void func_00188538(s32 *arg0, s32 arg1, u32 arg2) {
    func_001882F0(*(u32 *)(arg1 * 4 + *arg0), arg2);
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188560);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001885C0);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188738);

void func_00188820(s32 arg0) {
    func_002DAA68(*(u32 *)(arg0 + 0x20));
    func_002D0918(*(u32 *)(arg0 + 0x24));
}

void func_00188850(s32 arg0) {
    *(u32 *)(arg0 + 0x10) = 2;
    *(u32 *)(arg0 + 4) = 0x80808080;
    *(u32 *)(arg0 + 0xc) = 0;
}

void func_00188870(u32 *arg0, u32 arg1) {
    *arg0 = arg1;
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188878);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188958);

void func_001889B8(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x10) + ((*(s32 *)(arg0 + 0x14) - 1) * (arg1 - 1) + arg1) * -2
    ;
    if (temp_v0 < 2) {
        temp_v0 = (temp_v0 + *(s32 *)(arg0 + 8)) - 2;
    }
    *(s32 *)(arg0 + 0x10) = temp_v0;
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188A00);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188A78);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00188E10);
