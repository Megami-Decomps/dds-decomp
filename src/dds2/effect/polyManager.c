#include "common.h"

void func_001655D0(u32 arg0) {
    func_001634A8(*(u32 *)((s32)arg0 + 0xdc));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/polyManager", func_00165600);

void func_00165670(float arg0, s32 arg1) {
    *(float *)(arg1 + 0xcc) = *(float *)(arg1 + 0xcc) * arg0;
    *(float *)(arg1 + 0xd0) = *(float *)(arg1 + 0xd0) * arg0;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_00165690);

void func_00165838(s32 arg0) {
    func_00165690();
    func_00163508(*(u32 *)(arg0 + 0xdc));
}

INCLUDE_ASM(const s32, "effect/polyManager", func_00165860);

INCLUDE_ASM(const s32, "effect/polyManager", func_001659B8);

INCLUDE_ASM(const s32, "effect/polyManager", func_00165A78);

INCLUDE_ASM(const s32, "effect/polyManager", func_00165B98);

void func_00165CC0(s32 arg0) {
    func_001634A8(*(u32 *)(arg0 + 0xf0));
    func_003297C8(*(u32 *)(arg0 + 0xf8));
}

INCLUDE_ASM(const s32, "effect/polyManager", func_00165CF0);

INCLUDE_ASM(const s32, "effect/polyManager", func_00165D38);

INCLUDE_ASM(const s32, "effect/polyManager", func_00165E28);

INCLUDE_ASM(const s32, "effect/polyManager", func_00165FC8);

void func_00166190(float arg0, s32 arg1) {
    *(float *)(arg1 + 200) = *(float *)(arg1 + 200) * arg0;
    *(float *)(arg1 + 0xdc) = *(float *)(arg1 + 0xdc) * arg0;
    *(float *)(arg1 + 0xcc) = *(float *)(arg1 + 0xcc) * arg0;
    *(float *)(arg1 + 0xd0) = *(float *)(arg1 + 0xd0) * arg0;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_001661C8);

INCLUDE_ASM(const s32, "effect/polyManager", func_00166350);

void func_00166478(s32 arg0) {
    func_001634A8(*(u32 *)(arg0 + 0xe0));
    func_003297C8(*(u32 *)(arg0 + 0xe8));
}

INCLUDE_ASM(const s32, "effect/polyManager", func_001664A8);

INCLUDE_ASM(const s32, "effect/polyManager", func_001664F0);

INCLUDE_ASM(const s32, "effect/polyManager", func_00166590);

void func_001667E8(float arg0, s32 arg1) {
    *(float *)(arg1 + 0xcc) = *(float *)(arg1 + 0xcc) * arg0;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_001667F8);

INCLUDE_ASM(const s32, "effect/polyManager", func_00166980);

void func_00166AB0(s32 arg0) {
    func_001634A8(*(u32 *)(arg0 + 0xf4));
    func_003297C8(*(u32 *)(arg0 + 0xfc));
}

INCLUDE_ASM(const s32, "effect/polyManager", func_00166AE0);

INCLUDE_ASM(const s32, "effect/polyManager", func_00166B40);

INCLUDE_ASM(const s32, "effect/polyManager", func_00166CB0);

void func_00166EA0(float arg0, s32 arg1) {
    *(float *)(arg1 + 0xcc) = *(float *)(arg1 + 0xcc) * arg0;
    *(float *)(arg1 + 0xd0) = *(float *)(arg1 + 0xd0) * arg0;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_00166EC0);

void func_00167058(s32 arg0) {
    u32 temp_v0;
    s32 *piVar2;
    u32 temp_v1;

    temp_v0 = *(u32 *)(arg0 + 0x10);
    temp_v1 = 0;
    *(u32 *)(arg0 + 0x14) = 0xfffffff;
    *(u32 *)(arg0 + 0x6c) = 0;
    *(u32 *)(arg0 + 0x68) = 0;
    piVar2 = *(s32 **)(arg0 + 0xf8);
    if (temp_v0 != 0) {
        do {
            if (*piVar2 != -0xffffff) {
                *piVar2 = 0xffffff0;
            }
            temp_v1 = temp_v1 + 1;
            piVar2 = piVar2 + 5;
        } while (temp_v1 < temp_v0);
    }
}
