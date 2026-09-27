#include "common.h"

void func_0018FE60(u32 arg0) {
    effTrackPolyFreeData(*(u32 *)((s32)arg0 + 0x38));
    func_00328E48(arg0);
}

void func_0018FE90(s32 arg0) {
    *(u32 *)(arg0 + 0x34) = 0;
    effTrackPolyInitData(*(u32 *)(arg0 + 0x38));
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_0018FEB0);

void func_0018FF10(s32 arg0) {
    func_001906B0(*(u32 *)(arg0 + 0x38));
}

void func_0018FF28(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x38) + 4) = arg1;
}

void func_0018FF38(s32 arg0) {
    func_00190A48(*(u32 *)(arg0 + 0x38));
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_0018FF50);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00190018);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001900B8);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00190120);

void func_00190148(s32 *arg0, s32 arg1) {
    func_0018FE90(*(u32 *)(arg1 * 4 + *arg0));
}

void func_00190170(s32 *arg0, s32 arg1, u32 arg2) {
    func_0018FF28(*(u32 *)(arg1 * 4 + *arg0), arg2);
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00190198);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001901F8);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00190370);

void effTrackPolyFreeData(s32 arg0) {
    func_00333918(*(u32 *)(arg0 + 0x20));
    func_003297C8(*(u32 *)(arg0 + 0x24));
}

void effTrackPolyInitData(s32 arg0) {
    *(u32 *)(arg0 + 0x10) = 2;
    *(u32 *)(arg0 + 4) = 0x80808080;
    *(u32 *)(arg0 + 0xc) = 0;
}

void func_001904A8(u32 *arg0, u32 arg1) {
    *arg0 = arg1;
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001904B0);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00190590);

void func_001905F0(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x10) + ((*(s32 *)(arg0 + 0x14) - 1) * (arg1 - 1) + arg1) * -2
    ;
    if (temp_v0 < 2) {
        temp_v0 = (temp_v0 + *(s32 *)(arg0 + 8)) - 2;
    }
    *(s32 *)(arg0 + 0x10) = temp_v0;
}

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00190638);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_001906B0);

INCLUDE_ASM(const s32, "effect/effModelTrackPoly", func_00190A48);
