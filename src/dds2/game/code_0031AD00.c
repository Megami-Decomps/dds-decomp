#include "common.h"

extern u32 *D_00438940;

void mdlBroadcastMasked(u32 sprite);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031AD00);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031ADD8);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031AE48);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031AEB8);

void func_0031AF58(s32 arg0) {
    *(u32 *)(arg0 + 0x1d4) = 0;
}

void func_0031AF60(void) {
}

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031AF68);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031B080);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031B0F8);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031B188);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031B1F8);

void func_0031B268(void) {
    if (D_00438940 != (u32 *)0x0) {
        func_003297C8(*D_00438940);
        D_00438940 = (u32 *)0x0;
    }
}

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031B290);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031B2E0);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031B328);

void func_0031B3B0(s32 arg0) {
    *(u16 *)(arg0 + 0x1da) = 0;
}

void func_0031B3B8(s32 arg0) {
    *(u16 *)(arg0 + 0x1da) = 1;
}

void func_0031B3C8(void) {
}

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031B3D0);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031B4F0);

void func_0031B5F8(s32 *arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *arg0;
    if (0 < arg0[1]) {
        do {
            memset(temp_v0, 0, 0x20);
            temp_v1 = (temp_v1 + 1) & 0xffff;
            temp_v0 = temp_v0 + 0x20;
        } while ((s32)temp_v1 < arg0[1]);
    }
}

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031B668);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031B6D0);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031B748);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031B838);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031B960);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031BA28);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031BB80);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031BBB0);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031BC10);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031BDE8);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031BFA0);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031BFC0);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031BFE0);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031C0F8);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031C1A0);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031C208);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031C280);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031C348);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031C3C8);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031C458);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031C4A0);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031C4E8);

void func_0031C578(s32 arg0) {
    *(u32 *)(arg0 + 0x44) = 0;
    **(u32 **)(arg0 + 0x40) = **(u32 **)(arg0 + 0x40) | 1;
}

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031C590);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031C5A0);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031C5B8);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031C5E8);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031C630);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031C688);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031C850);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031C888);

void func_0031C8A8(void) {
}

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031C8B0);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031C900);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031C940);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031CA10);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031CAE8);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031CBC8);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031CDE8);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031CE60);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031CF68);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031CF88);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031D120);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031D260);

void func_0031D380(s32 *arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *arg0;
    if (0 < arg0[1]) {
        do {
            memset(temp_v0, 0, 0x34);
            temp_v1 = (temp_v1 + 1) & 0xffff;
            temp_v0 = temp_v0 + 0x34;
        } while ((s32)temp_v1 < arg0[1]);
    }
}

void func_0031D3F0(s32 *arg0, u32 arg1) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *arg0;
    if (0 < arg0[1]) {
        do {
            temp_v1 = temp_v1 + 1;
            *(u32 *)(temp_v0 + 0x20) = (*(u32 *)(temp_v0 + 0x20) & 0xfffff807) | ((arg1 & 0xff) << 3);
            temp_v0 = temp_v0 + 0x34;
        } while (temp_v1 < arg0[1]);
    }
}

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031D440);

s32 func_0031D4A8(s32 *arg0) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *arg0;
    if (0 < arg0[1]) {
        do {
            if ((*(u32 *)(temp_v0 + 0x20) & 1) == 0) {
                *(u16 *)(temp_v0 + 0x2a) = 10;
                *(u32 *)(temp_v0 + 0x20) = *(u32 *)(temp_v0 + 0x20) | 1;
                *(u16 *)(temp_v0 + 0x28) = 0;
                *(u16 *)(temp_v0 + 0x24) = 0;
                *(u16 *)(temp_v0 + 0x26) = 0;
                return temp_v0;
            }
            temp_v1 = temp_v1 + 1;
            temp_v0 = temp_v0 + 0x34;
        } while (temp_v1 < arg0[1]);
    }
    return 0;
}

void func_0031D508(s32 arg0) {
    *(u32 *)(arg0 + 0x20) = *(u32 *)(arg0 + 0x20) & 0xfffffffe;
}

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031D520);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031D530);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031D540);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031D558);

INCLUDE_ASM(const s32, "game/code_0031AD00", func_0031D680);

INCLUDE_SDATA(const s32, "game/code_0031AD00", D_00438938);

INCLUDE_SDATA(const s32, "game/code_0031AD00", D_00438940);

INCLUDE_SDATA(const s32, "game/code_0031AD00", D_00438944);

INCLUDE_SDATA(const s32, "game/code_0031AD00", D_00438948);

INCLUDE_SDATA(const s32, "game/code_0031AD00", D_0043894C);

INCLUDE_SDATA(const s32, "game/code_0031AD00", D_00438950);

