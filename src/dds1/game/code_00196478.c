#include "common.h"

extern u64 func_002D3288(u32);
extern u64 func_002EB028(u64, u32 *, u64);

extern u32 D_003BB190;
extern s64 func_00101818(u32);

extern u32 D_003BB18C;

extern u32 D_003BD81C;

extern u64 func_001951C8(u64, u64, u64, u64, u64);

extern u64 func_00195160(u64, u64, u64, u64, u64);

extern s32 D_003BB168;

extern u32 D_003BB170;

extern u32 D_003BB16C;

extern u32 D_003BB15C;

INCLUDE_ASM(const s32, "game/code_00196478", func_00196478);

INCLUDE_ASM(const s32, "game/code_00196478", func_001964A0);

INCLUDE_ASM(const s32, "game/code_00196478", func_001964F8);

INCLUDE_ASM(const s32, "game/code_00196478", func_001968C0);

INCLUDE_ASM(const s32, "game/code_00196478", func_00196AB0);

INCLUDE_ASM(const s32, "game/code_00196478", func_00196AE8);

INCLUDE_ASM(const s32, "game/code_00196478", func_00196B30);

INCLUDE_ASM(const s32, "game/code_00196478", func_00196B88);

u32 func_00196BB0(u32 arg0) {
    return D_003BB15C & arg0;
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00196BC0);

u32 func_00196BD8(void) {
    return D_003BB16C;
}

u32 func_00196BE0(void) {
    return D_003BB170;
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00196BE8);

INCLUDE_ASM(const s32, "game/code_00196478", func_00196ED0);

INCLUDE_ASM(const s32, "game/code_00196478", func_00197068);

INCLUDE_ASM(const s32, "game/code_00196478", func_00197190);

INCLUDE_ASM(const s32, "game/code_00196478", func_001971E0);

INCLUDE_ASM(const s32, "game/code_00196478", func_00197200);

void func_00197220(s32 arg0) {
    if (arg0 < 1) {
        arg0 = 0x14;
    }
    D_003BB168 = arg0;
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00197238);

INCLUDE_ASM(const s32, "game/code_00196478", func_00197348);

void func_00197378(void) {
    func_001945A8(4);
    func_001945A8(5);
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00197398);

INCLUDE_ASM(const s32, "game/code_00196478", func_001973E8);

void func_001974B8(u64 arg0, u64 arg1, s32 arg2, u64 arg3,
                                    u64 arg4, u64 arg5) {
    u64 temp_v0;

    temp_v0 = func_00195160(arg4, 0, 0, 0, 0);
    func_001953D8(temp_v0, 0x10, 0x12);
    func_00195450(temp_v0, arg0, arg1);
    func_00195460(temp_v0, arg2 << 4);
    func_001954C8(temp_v0, arg3);
    func_001953A8(temp_v0, 0xfffffffffffffffc);
    func_00195B78(arg5, temp_v0, 0);
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00197580);

INCLUDE_ASM(const s32, "game/code_00196478", func_00197708);

void func_00197748(void) {
    func_00197708();
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00197760);

INCLUDE_ASM(const s32, "game/code_00196478", func_001978E8);

INCLUDE_ASM(const s32, "game/code_00196478", func_001979C8);

INCLUDE_ASM(const s32, "game/code_00196478", func_00197A98);

void func_00197B78(u64 arg0, u64 arg1, s32 arg2, u64 arg3,
                                    u64 arg4, u64 arg5) {
    u64 temp_v0;

    temp_v0 = func_001951C8(arg4, 0, 0, 0, 0);
    func_001953D8(temp_v0, 0xc, 0x10);
    func_001953A8(temp_v0, 3);
    func_00195450(temp_v0, arg0, arg1);
    func_00195460(temp_v0, arg2 << 4);
    func_001954C8(temp_v0, arg3);
    func_00195B78(arg5, temp_v0, 0);
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00197C40);

INCLUDE_ASM(const s32, "game/code_00196478", func_00197E08);

INCLUDE_ASM(const s32, "game/code_00196478", func_00197EC8);

u32 func_00198008(void) {
    return 0;
}

void func_00198010(void) {
}

void func_00198018(void) {
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00198020);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198028);

u32 func_00198030(u32 arg0) {
    return arg0;
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00198038);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198068);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198088);

INCLUDE_ASM(const s32, "game/code_00196478", func_001981A8);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198248);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198270);

u32 func_001982A0(s32 arg0) {
    func_002D0918(*(u32 *)(arg0 - 4));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00196478", func_001982C0);

void func_00198308(void) {
    func_002D2CB8(D_003BD81C);
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00198320);

INCLUDE_ASM(const s32, "game/code_00196478", func_001983A8);

void func_00198408(void) {
    func_00194800(D_003BB18C);
    func_00198308();
}

u32 func_00198428(void) {
    s64 temp_v0;
    u32 temp_v1;

    func_00198320();
    temp_v0 = func_00101818(D_003BB190);
    temp_v1 = 0xffffffff;
    if (temp_v0 != 3) {
        temp_v1 = 0;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00198460);

INCLUDE_ASM(const s32, "game/code_00196478", func_001984C0);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198528);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198580);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198600);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198658);

INCLUDE_ASM(const s32, "game/code_00196478", func_001986A8);

u64 func_001986E0(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_002EB028(arg0, temp_v2, 0);
    temp_v1 = func_002D3288(temp_v2[0]);
    func_002D0918(temp_v0);
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_00196478", func_00198730);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198858);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198990);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198B30);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198C70);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198DF0);

INCLUDE_ASM(const s32, "game/code_00196478", func_00198F58);

INCLUDE_ASM(const s32, "game/code_00196478", func_00199080);

INCLUDE_ASM(const s32, "game/code_00196478", func_001991D0);

INCLUDE_ASM(const s32, "game/code_00196478", func_00199318);

INCLUDE_ASM(const s32, "game/code_00196478", func_00199460);

INCLUDE_ASM(const s32, "game/code_00196478", func_001994D8);

INCLUDE_ASM(const s32, "game/code_00196478", func_00199560);

INCLUDE_ASM(const s32, "game/code_00196478", func_00199638);

u32 func_001997E0(void) {
    return 0;
}

u32 func_001997E8(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00196478", func_001997F0);

INCLUDE_ASM(const s32, "game/code_00196478", func_00199828);
