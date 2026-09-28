#include "common.h"

extern u32 D_00438FC8;

extern u32 *D_00437960;

extern u64 func_00279DC8(u32, u64, u64, u64, u64, u64);

extern u64 func_0027A628(u32, u64, u64);

extern s32 func_00291400(u64, u64);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028E350);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028E568);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028E638);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028E858);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028EB38);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028EF50);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028F128);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427330);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427340);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427360);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427380);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028F380);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028F570);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028F770);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427428);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028F7B8);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028F8A8);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427488);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_004274B0);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028F9A0);

u32 func_0028FD08(void) {
    return 0;
}

void func_0028FD10(u32 arg0) {
    func_0028FD30(arg0, 0xffffffffffffffff, 0xffffffffffffffff);
}

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427560);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028FD30);

INCLUDE_ASM(const s32, "game/code_0028E350", func_0028FEF0);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_004275B0);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_004275E8);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00290240);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00290328);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00290410);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427668);

INCLUDE_ASM(const s32, "game/code_0028E350", func_002906E0);

u32 func_00290A70(s32 arg0) {
    return *(u32 *)(arg0 + 0x7a0);
}

INCLUDE_ASM(const s32, "game/code_0028E350", func_00290A78);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00290B28);

u16 func_00290B98(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(*(s32 *)(**(s32 **)(*(s32 *)(arg0 + 4) + 0x1c) * 4 + arg0 + 0x7ac) + 8);
    if (*(s32 *)(arg0 + 0x7a4) != 0) {
        return *(u16 *)(*(s16 *)(*(s32 *)(arg0 + 0x7a4) + 2) * 2 + temp_v0);
    }
    return *(u16 *)(*(s16 *)(*(s32 *)(arg0 + 0x7a0) + 2) * 2 + temp_v0);
}

u16 func_00290BF0(s32 arg0, s32 arg1) {
    return *(u16 *)
                    (((arg1 << 0x10) >> 0xf) +
                    *(s32 *)(*(s32 *)(**(s32 **)(*(s32 *)(arg0 + 4) + 0x1c) * 4 + arg0 + 0x7ac) + 8));
}

INCLUDE_ASM(const s32, "game/code_0028E350", func_00290C20);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00290E48);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00291038);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_004276C0);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00291118);

INCLUDE_ASM(const s32, "game/code_0028E350", func_002911D0);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00291288);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00291338);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00291400);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00291460);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00291510);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00291590);

INCLUDE_ASM(const s32, "game/code_0028E350", func_002917C0);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00291A20);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00291C68);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00291DD0);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00292458);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427720);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427740);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00292478);

void func_00292998(s32 arg0) {
    u32 temp_v0;
    s32 temp_v1;
    u64 temp_v2;

    temp_v0 = *(u32 *)(arg0 + 0xbec);
    func_0026D098(0);
    temp_v1 = func_00291400(0, 8);
    temp_v2 = func_0027A628(temp_v0, 8, 0);
    func_0027A798(temp_v2, 7, 0);
    temp_v2 = func_00279DC8(temp_v0, 8, 1, 0, 0, 0);
    func_00279F30(temp_v2, 0, 0, 0, 0x80, 0x53, 0, 0);
    func_0027A798(temp_v2, 8, 0);
    *(u16 *)(temp_v1 + 2) = (*(u16 *)(temp_v1 + 2) & 0xfff0) | 1;
}

INCLUDE_ASM(const s32, "game/code_0028E350", func_00292A60);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00292B90);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00292BB0);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00292C58);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00292CF0);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00292EA8);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00292FF0);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00293148);

INCLUDE_ASM(const s32, "game/code_0028E350", func_002932B0);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00293360);

INCLUDE_ASM(const s32, "game/code_0028E350", func_002933A8);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_004277A0);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427858);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_00427888);

INCLUDE_RODATA(const s32, "game/code_0028E350", D_004278C0);

INCLUDE_ASM(const s32, "game/code_0028E350", func_002933F0);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00293DB0);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00293FD0);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00294060);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00294420);

void func_00294488(void) {
    if (D_00437960 != (u32 *)0x0) {
        func_003297C8(*D_00437960);
    }
    D_00437960 = (u32 *)0x0;
}

INCLUDE_ASM(const s32, "game/code_0028E350", func_002944B0);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00294538);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00294580);

void func_002945B8(u32 arg0) {
    D_00438FC8 = arg0;
}

INCLUDE_ASM(const s32, "game/code_0028E350", func_002945C0);

INCLUDE_ASM(const s32, "game/code_0028E350", func_00294680);
INCLUDE_SDATA(const s32, "game/code_0028E350", D_00437940);

INCLUDE_SDATA(const s32, "game/code_0028E350", D_00437948);

INCLUDE_SDATA(const s32, "game/code_0028E350", D_00437950);

INCLUDE_SDATA(const s32, "game/code_0028E350", D_00437958);


INCLUDE_SDATA(const s32, "game/code_0028E350", D_00437960);

