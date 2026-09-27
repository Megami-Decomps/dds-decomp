#include "common.h"

extern u8 D_003BD42F;

extern u32 D_003BDA6C;

extern u32 D_003BDA68;

extern u32 D_003BD3F4;

extern s32 func_002E5158(u32, u8 *, u32);

extern s32 D_003BDA48;
extern u32 D_003BDA58;

extern s32 D_003BD3D8;
extern u32 D_003BDA64;

extern u32 D_003BDA44;

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4720);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4908);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4960);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E49E8);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4A00);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4A28);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4A80);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4AE0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4B80);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4C28);

u32 func_002E4CA8(void) {
    return D_003BDA44;
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4CB0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4D48);

void func_002E4DF8(void) {
    if (D_003BD3D8 != 0) {
        SignalSema(D_003BDA64);
        D_003BD3D8 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4E20);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4EE0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E4FB0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5158);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5310);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5398);

void func_002E55E0(void) {
    if (D_003BDA48 != 0) {
        WaitSema(D_003BDA58);
        func_002F4620();
        SignalSema(D_003BDA58);
        D_003BDA48 = 0;
    }
}

u8 func_002E5618(u32 arg0) {
    s64 temp_v0;
    u8 temp_v1 [16];

    temp_v0 = func_002E5158(arg0, temp_v1, 0);
    return temp_v0 != 0;
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5640);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5670);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E56A0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5738);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E57B8);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5808);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5880);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5918);

u32 func_002E5958(void) {
    return 0;
}

u32 func_002E5960(void) {
    return 0;
}

u32 func_002E5968(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5970);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5B10);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5B50);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5BB8);

void func_002E5C38(u32 arg0) {
    func_002E6E38();
    WaitSema(D_003BD3F4);
    func_002E6E88(arg0);
}

void func_002E5C68(void) {
    func_002E6D48();
    WaitSema(D_003BD3F4);
}

u32 func_002E5C88(void) {
    func_002E6CF0();
    WaitSema(D_003BD3F4);
    return D_003BDA68;
}

u32 func_002E5CB0(void) {
    func_002E6C90();
    WaitSema(D_003BD3F4);
    return D_003BDA6C;
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5CD8);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5D70);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5D80);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5D98);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5DA0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5E90);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E5F08);

void func_002E6020(u32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 0x28);
    *(u32 *)((s32)arg0 + 0x28) = 0xffffffff;
    if (-1 < temp_v0) {
        func_0030EB78(temp_v0);
    }
    func_002E5F08(arg0);
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E6060);

INCLUDE_RODATA(const s32, "game/code_002E4720", D_003B4578);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E60B0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E67A8);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E69F0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E6B28);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E6BA8);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E6C10);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E6C90);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E6CF0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E6D48);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E6DA8);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E6E08);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E6E38);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E6E88);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E6EC8);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E6F58);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E7000);

void func_002E7078(void) {
    D_003BD42F = 3;
    func_002E7000(0x78);
}

void func_002E7098(void) {
    func_002E7000(0x48);
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E70B0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E70F0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E71A0);

void func_002E7210(void) {
    iSignalSema();
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E7228);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E7370);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E73F0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E7480);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E74F8);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E7568);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E75A0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E75F0);

void func_002E7680(u32 arg0) {
    func_002D0918(*(u32 *)arg0);
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E76B0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E7730);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E77F8);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E78F8);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E7918);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E79C0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E79F0);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E7AA8);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E7B20);

INCLUDE_ASM(const s32, "game/code_002E4720", func_002E7BA8);
