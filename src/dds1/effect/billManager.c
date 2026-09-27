#include "common.h"

extern u64 func_00151D88(u64, u32);
extern u64 func_002EB028(u64, u32 *, u64);

INCLUDE_ASM(const s32, "effect/billManager", func_001502B0);

INCLUDE_ASM(const s32, "effect/billManager", func_00150750);

INCLUDE_ASM(const s32, "effect/billManager", func_00150840);

INCLUDE_ASM(const s32, "effect/billManager", func_00150EB0);

INCLUDE_ASM(const s32, "effect/billManager", func_00151010);

INCLUDE_ASM(const s32, "effect/billManager", func_00151178);

INCLUDE_ASM(const s32, "effect/billManager", func_001511C0);

void func_00151210(u32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 0x30);
    if (temp_v0 != 0) {
        func_00150260(temp_v0);
    }
    func_002CFF98(arg0);
}

void func_00151248(s32 arg0) {
    func_001502B0(arg0, *(u32 *)(arg0 + 0x30));
}

INCLUDE_ASM(const s32, "effect/billManager", func_00151260);

INCLUDE_ASM(const s32, "effect/billManager", func_001512E8);

void func_00151368(u32 arg0) {
    func_00151C58(*(u32 *)((s32)arg0 + 0x30));
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "effect/billManager", func_00151398);

INCLUDE_ASM(const s32, "effect/billManager", func_00151568);

INCLUDE_ASM(const s32, "effect/billManager", func_001515E8);

void func_001518A0(s32 arg0, s32 arg1, s32 arg2) {
    s16 temp_v0;
    s32 temp_v1;
    s32 *piVar3;
    s32 temp_v2;

    temp_v2 = *(s32 *)(arg0 + 4);
    piVar3 = (s32 *)(*(s32 *)(arg0 + 8) + arg1 * 0x14);
    temp_v1 = *piVar3;
    *(s32 **)(arg2 + 0xc) = piVar3;
    temp_v2 = temp_v2 + temp_v1;
    *(u32 *)(arg2 + 4) = 0;
    temp_v0 = *(s16 *)(temp_v2 + 0x12);
    *(s32 *)(arg2 + 0x10) = temp_v2;
    *(s32 *)(arg2 + 8) = (s32)temp_v0;
}

INCLUDE_ASM(const s32, "effect/billManager", func_001518D8);

INCLUDE_ASM(const s32, "effect/billManager", func_00151A88);

INCLUDE_ASM(const s32, "effect/billManager", func_00151C58);

INCLUDE_ASM(const s32, "effect/billManager", func_00151CE8);

INCLUDE_ASM(const s32, "effect/billManager", func_00151D88);

u64 func_00151E08(u64 arg0, u64 arg1) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_002EB028(arg1, temp_v2, 0);
    temp_v1 = func_00151D88(arg0, temp_v2[0]);
    func_002D0918(temp_v0);
    return temp_v1;
}

INCLUDE_ASM(const s32, "effect/billManager", func_00151E60);

INCLUDE_ASM(const s32, "effect/billManager", func_00151F00);

INCLUDE_ASM(const s32, "effect/billManager", func_00151F38);
