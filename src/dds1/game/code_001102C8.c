#include "common.h"

extern u32 func_00128780(u32, u32, u32, u32, u32, u32);

extern u32 func_001281E0(u32);

INCLUDE_ASM(const s32, "game/code_001102C8", func_001102C8);

u16 func_00110400(s32 arg0) {
    u16 temp_v0;

    temp_v0 = 0;
    if (arg0 != 0) {
        temp_v0 = *(u16 *)((s32)arg0 + 6);
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_001102C8", func_00110418);

INCLUDE_ASM(const s32, "game/code_001102C8", func_00110458);

u32 func_00110490(s16 *arg0) {
    arg0[2] = *arg0;
    return (u32)~(s32)*arg0 >> 0x1f;
}

INCLUDE_ASM(const s32, "game/code_001102C8", func_001104B0);

INCLUDE_ASM(const s32, "game/code_001102C8", func_001104F8);

INCLUDE_ASM(const s32, "game/code_001102C8", func_00110578);

INCLUDE_ASM(const s32, "game/code_001102C8", func_00110638);

INCLUDE_ASM(const s32, "game/code_001102C8", func_00110710);

INCLUDE_ASM(const s32, "game/code_001102C8", func_001107C8);

void func_00110860(s32 arg0, s8 arg1) {
    if (*(s32 *)(arg0 + 0x18) != 0) {
        *(s32 *)(*(s32 *)(arg0 + 0x18) + 0x20) = (s32)arg1;
    }
}

INCLUDE_ASM(const s32, "game/code_001102C8", func_00110880);

INCLUDE_ASM(const s32, "game/code_001102C8", func_00110928);

void func_001109B8(s32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x18);
    func_001109F0();
    *(u32 *)(temp_v0 + 0xc) = arg1;
}

u32 func_001109F0(s32 arg0) {
    return *(u32 *)(*(s32 *)(arg0 + 0x18) + 0xc);
}

void func_00110A00(s32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x18);
    func_00110A38();
    *(u32 *)(temp_v0 + 0x10) = arg1;
}

u32 func_00110A38(s32 arg0) {
    return *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x10);
}

INCLUDE_ASM(const s32, "game/code_001102C8", func_00110A48);

INCLUDE_ASM(const s32, "game/code_001102C8", func_00110AB0);

void func_00110B48(s32 arg0, u32 arg1) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v0 = *(s32 *)(arg0 + 0x18);
    temp_v1 = func_001281E0(arg1);
    *(u32 *)(temp_v0 + 0x14) = temp_v1;
}

void func_00110B78(s32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4, u32 arg5, u32 arg6) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v0 = *(s32 *)(arg0 + 0x18);
    temp_v1 = func_00128780(arg1, arg2, arg3, arg4, arg5, arg6);
    *(u32 *)(temp_v0 + 0x14) = temp_v1;
}
