#include "common.h"

extern u32 D_00438A3C;

extern u32 func_00333300(void);

INCLUDE_ASM(const s32, "game/code_003325F8", func_003325F8);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332860);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332920);

INCLUDE_ASM(const s32, "game/code_003325F8", func_003329B8);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332A00);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332A18);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332AD8);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332B78);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332BB0);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332C30);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332C88);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332D08);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332D48);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332D88);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332DB8);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332DE8);

void func_00332E00(s32 arg0) {
    *(u8 *)(arg0 + 0x19) = *(u8 *)(arg0 + 0x19) & 0xfd;
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332E10);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332E30);

void func_00332E50(u32 arg0) {
    D_00438A3C = arg0;
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332E58);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332E78);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332F08);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332F70);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332FC8);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333060);

INCLUDE_ASM(const s32, "game/code_003325F8", func_003330F0);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333120);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333140);

INCLUDE_ASM(const s32, "game/code_003325F8", func_003331A8);

INCLUDE_ASM(const s32, "game/code_003325F8", func_003331F0);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333208);

void func_00333270(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
    *(u8 *)(arg0 + 6) = *(u8 *)(arg0 + 6) | 3;
}

void func_00333288(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
    *(u8 *)(arg0 + 6) = *(u8 *)(arg0 + 6) | 3;
}

void func_003332A0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x20) = arg1;
    *(u8 *)(arg0 + 6) = *(u8 *)(arg0 + 6) | 3;
}

void func_003332B8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x28) = arg1;
    *(u8 *)(arg0 + 6) = *(u8 *)(arg0 + 6) | 3;
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_003332D0);

void func_003332E8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x2c) = arg1;
    *(u8 *)(arg0 + 6) = *(u8 *)(arg0 + 6) | 3;
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333300);

void func_00333340(s32 arg0) {
    u32 temp_v0;

    if (*(s32 *)(arg0 + 0x38) == 0) {
        temp_v0 = func_00333300();
        *(u32 *)(arg0 + 0x38) = temp_v0;
    }
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333378);

INCLUDE_ASM(const s32, "game/code_003325F8", func_003333F8);

void func_00333460(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
    *(u8 *)(arg0 + 6) = *(u8 *)(arg0 + 6) | 0x30;
}

void func_00333478(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x34) = arg1;
    *(u8 *)(arg0 + 6) = *(u8 *)(arg0 + 6) | 0x30;
}

void func_00333490(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x30) = arg1;
    *(u8 *)(arg0 + 6) = *(u8 *)(arg0 + 6) | 0x30;
}

void func_003334A8(s32 arg0) {
    u32 temp_v0;

    if (*(s32 *)(arg0 + 0x3c) == 0) {
        temp_v0 = func_00333300();
        *(u32 *)(arg0 + 0x3c) = temp_v0;
    }
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_003334E0);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333560);

INCLUDE_ASM(const s32, "game/code_003325F8", func_003335C8);

INCLUDE_ASM(const s32, "game/code_003325F8", func_003335E0);

INCLUDE_ASM(const s32, "game/code_003325F8", func_003336E0);

INCLUDE_ASM(const s32, "game/code_003325F8", func_003338B0);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333918);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333950);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333990);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333A30);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333B18);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333B38);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333BC8);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333BE0);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333CB0);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333D98);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333E38);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333E98);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333EF8);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00334008);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00334078);
