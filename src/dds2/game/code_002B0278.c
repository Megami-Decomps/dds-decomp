#include "common.h"

extern s32 D_00435E5C;

extern s32 func_002C6CE8(void);

extern s32 func_002B86E8(u32);

extern u32 func_002BC120(u32);

extern u32 func_002B9FF8(u32);

extern s32 D_00435DD0;

extern s64 func_002C4038(s32, s32 *, u32, s32);

extern s32 func_00101958();

extern void func_002C21F8(s32);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B0278);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B0578);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B05C8);

u32 func_002B0610(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = func_00101958();
    temp_v0 = *(s32 *)(temp_v1 + 0xaa48);
    func_002C1B68(temp_v1 + 0xaa50, 1);
    func_0026C918(0, D_00435E5C +
                                    *(s32 *)(*(s32 *)(*(s32 *)(*(s32 *)(temp_v0 + 0x18) + 0x18) + 0x1c) + 100) * 0x19);
    func_0026C5B8(8);
    func_0026C648(0);
    func_0026C618(0xf);
    return 1;
}

u32 func_002B06A0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B06A8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B0898);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B09C8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B0A18);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B0A60);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B0AD8);

u32 func_002B0B88(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B0B90);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B0CB0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B0D00);

u32 func_002B0D48(void) {
    return 1;
}

void func_002B0D50(u32 arg0) {
    func_002A9460(4, arg0);
}

void func_002B0D70(void) {
}

s32 func_002B0D78(s32 arg0, s32 arg1) {
    if (arg0 < (*(s32 *)(arg1 + 0x20) - 1)) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B0D90);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B0FA0);

void func_002B1150(s32 arg0) {
    func_002B9520(*(u32 *)(*(s32 *)(arg0 + 0xaa48) + 8));
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B1178);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B12B0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B15F8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B1780);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B17C0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B18A0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B18E8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B1B90);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B1BF0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B1C68);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B1EA8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B2338);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B2408);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B2698);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B2790);

u8 func_002B27C8(void) {
    s64 temp_v0;

    temp_v0 = func_002C6CE8();
    return temp_v0 != 1;
}

void func_002B27F0(u32 arg0) {
    func_002A9460(3, arg0);
}

void func_002B2810(void) {
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B2818);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B2860);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B28A8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B2970);

void func_002B29C8(u32 arg0) {
    func_002B28A8(arg0, 1);
}

void func_002B29E0(void) {
    func_002B2970();
}

void func_002B29F8(u32 arg0) {
    func_002B28A8(arg0, 0);
}

void func_002B2A10(void) {
    func_002B2970();
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B2A28);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B2B48);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B2C50);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B2C88);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B2E38);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B2FF0);

void func_002B3120(s32 arg0) {
    *(u32 *)
      (*(s32 *)(**(s32 **)(*(s32 *)(arg0 + 0xa914) + 0x1c) * 0x2138 + arg0 + 0x3d8) + 0x60) =
              0x100;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B3150);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B3260);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B3400);

void func_002B34E0(s32 arg0, u32 *arg1) {
    s32 temp_v0;

    temp_v0 = 0x100 - *(s32 *)(*(s32 *)(**(s32 **)(*(s32 *)(arg0 + 0xa690) + 0x1c) * 0x2138 + arg0
                                                                      + 0x154) + 0x60);
    func_00306CD0(0xa0, 0xa30, 0, temp_v0, 1, arg1[1], 0x55, 0x53);
    func_00306CD0(0x30, 0xaf8, 0, temp_v0, 1, *arg1, 0x1a, 0x53);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B3580);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B3648);

void func_002B3720(u32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4, u32 arg5) {
    func_002C16F0(0, 0, 0, arg0, *(u8 *)((s32)arg0 + 0x55), arg2, arg5);
    func_002C3E08(0xe80, 0x5b8, 0, arg3, arg5);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B3788);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B3940);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B39F0);

u32 func_002B3A58(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B3A60);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B3CA0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B3E80);

u32 func_002B40B8(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    temp_v0 = *(s32 *)(temp_v0 + 0xaa48);
    func_002B9520(*(u32 *)(temp_v0 + 0x24));
    *(u32 *)(temp_v0 + 0x24) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B40F8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B4180);

void func_002B4270(u32 arg0) {
    func_002A9460(1, arg0);
}

void func_002B4290(void) {
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B4298);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B45D8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B4730);

void func_002B47F8(void) {
    func_00328E48();
}

s32 func_002B4810(s32 arg0, u32 *arg1) {
    s32 temp_v0 = (arg0 < 0) ? arg0 + 0x1f : arg0;

    return (arg1[temp_v0 >> 5] & (1 << arg0)) != 0;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B4848);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B4C48);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B4CC8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B4DE8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B4E58);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B5028);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B5128);

void func_002B5160(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    *(u32 *)(*(s32 *)(temp_v0 + 0xaa48) + 0x34) = 0xffffffff;
}

u32 func_002B5190(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    return ~*(u32 *)(*(s32 *)(temp_v0 + 0xaa48) + 0x34) >> 0x1f;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B51C8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B5240);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B5358);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B53B8);

void func_002B5430(s32 arg0, s32 arg1) {
    *(u16 *)(arg1 * 2 + arg0 + 0x22) = 0;
    func_003144E8();
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B5450);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B5580);

void func_002B5778(s32 arg0, s32 arg1, s32 arg2) {
    u8 *temp_v0 = (u8 *)(arg0 + 2);
    s32 temp_v1 = arg1 * 2 + 32;
    s32 temp_v2 = arg2 * 2 + 32;
    u16 temp_v3 = *(u16 *)(temp_v0 + temp_v1);
    u16 temp_v4 = *(u16 *)(temp_v0 + temp_v2);

    *(u16 *)(temp_v0 + temp_v1) = temp_v4;
    *(u16 *)(temp_v0 + temp_v2) = temp_v3;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B57A8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B5980);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B5A30);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B5BE8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B5DB0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B5DE8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B5F00);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B5FA8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B60E8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B61C0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B61F8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B62A8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B6308);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B63F0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B6498);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B66D8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B6800);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B6838);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B6898);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B6B00);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B6C70);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B6D08);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B6D78);

u32 func_002B6FA8(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    temp_v0 = *(s32 *)(temp_v0 + 0xaa48);
    func_002B9520(*(u32 *)(temp_v0 + 0x24));
    *(u32 *)(temp_v0 + 0x24) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B6FE8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B7060);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B70C0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B7228);

void func_002B7588(s32 arg0) {
    s32 temp_v0;

    for (temp_v0 = **(s32 **)(arg0 + 0x28c); temp_v0 < 3; temp_v0 = temp_v0 + 1) {
    }
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B75C8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B76B0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B76E8);

void func_002B7730(u32 arg0, u32 *arg1) {
    *arg1 = *arg1 | arg0;
}

void func_002B7740(s32 arg0, s32 arg1) {
    u32 *puVar1;
    s32 temp_v0;
    u32 *puVar3;
    s32 temp_v1;
    s32 temp_v2;
    u32 temp_v3;

    temp_v3 = 0;
    temp_v2 = 0;
    do {
        puVar3 = (u32 *)(arg1 + 0x40);
        temp_v0 = temp_v2 << 2;
        temp_v1 = 3;
        do {
            puVar1 = (u32 *)(temp_v0 + arg0);
            temp_v0 = temp_v0 + 4;
            temp_v1 = temp_v1 - 1;
            *puVar3 = *puVar1;
            puVar3 = puVar3 + 1;
        } while (-1 < temp_v1);
        temp_v3 = temp_v3 + 1;
        arg1 = arg1 + 0x10;
        temp_v2 = temp_v2 + 4;
    } while (temp_v3 < 2);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B7790);

void func_002B77A0(s32 arg0) {
    func_003059E0(*(u32 *)(arg0 + 8), *(u32 *)(arg0 + 0x1c),
                                *(u32 *)(arg0 + 0x3c), 0, 4);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B77D0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B7850);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B78C8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B7908);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B7958);

void func_002B7A80(s32 arg0, s32 arg1) {
    *(u32 *)(arg1 * 4 + arg0 + 0x60) = 0;
    *(s32 *)(arg0 + 0x160) = *(s32 *)(arg0 + 0x160) - 1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B7AA0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B7C10);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B7E60);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B7F80);

void func_002B8140(u32 *arg0) {
    *arg0 = *arg0 & 0xfffffffb;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B8158);

u32 func_002B81C8(u32 arg0) {
    s64 temp_v0;

    do {
        temp_v0 = func_002B86E8(arg0);
    } while (temp_v0 != 0);
    func_00328E48(arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B8208);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B82A0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B8350);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B83A0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B86E8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B8860);

void *func_002B88A8(s32 arg0, void *arg1) {
    void *temp_node = *(void **)((s32)arg1 + 0x10);
    s32 temp_i = 0;

    if (temp_node != NULL && arg0 != temp_i) {
        do {
            temp_node = *(void **)((s32)temp_node + 0x58);
            temp_i++;
        } while (temp_node != NULL && temp_i != arg0);
    }
    return temp_node;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B88F0);

void func_002B8968(u32 arg0) {
    func_002B88F0(0, arg0);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B8988);

s32 func_002B89A8(s32 *arg0) {
    s32 temp_1C = arg0[7];
    s32 temp_14 = arg0[5];
    s32 *temp_18 = (s32 *)arg0[6];

    if (temp_1C == temp_14) {
        return temp_1C;
    }
    temp_18 = (s32 *)temp_18[22];
    if (temp_18 == NULL) {
        return temp_1C;
    }
    arg0[6] = (s32)temp_18;
    arg0[9]--;
    return temp_1C;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B89E0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B8A50);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B8BA8);

void func_002B8CF0(u32 arg0) {
    func_002B8A50(arg0, 0, 0);
}

void func_002B8D10(u32 arg0) {
    func_002B8BA8(arg0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B8D30);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B8E30);

void func_002B8F98(u32 *arg0) {
    *arg0 &= ~1;
    *arg0 &= ~2;
}

u32 func_002B8FB8(u32 *arg0) {
    return *arg0 & 2;
}

s32 func_002B8FC8(s32 arg0) {
    return *(s32 *)(arg0 + 0x28) * *(s32 *)(arg0 + 0xc);
}

void func_002B8FD8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x10);
    if (temp_v0 != 0) {
        *(u32 *)(temp_v0 + 0x50) = 0;
        while (temp_v0 = *(s32 *)(temp_v0 + 0x58), temp_v0 != 0) {
            *(u32 *)(temp_v0 + 0x50) = 0;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9010);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9058);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9138);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9188);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9218);

void func_002B9460(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_002B9218(arg0, arg1, arg2, 0, 0, 0x100, 0, arg3, arg4);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9490);

void func_002B9520(u32 arg0) {
    s32 temp_v0;

    func_002B81C8(*(u32 *)((s32)arg0 + 0x18));
    temp_v0 = *(s32 *)((s32)arg0 + 0x90);
    if (temp_v0 != 0) {
        func_002B99D8(temp_v0);
    }
    func_00328E48(arg0);
}

void func_002B9560(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x3c) = arg1;
}

void func_002B9568(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x94) = arg1;
}

void func_002B9570(s32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4, u32 arg5, u32 arg6, u32 arg7,
                                    u32 arg8) {
    *(u32 *)(arg0 + 0x2c) = arg1;
    *(u32 *)(arg0 + 0x4c) = arg8;
    *(u32 *)(arg0 + 0x30) = arg2;
    *(u32 *)(arg0 + 0x34) = arg3;
    *(u32 *)(arg0 + 0x38) = arg5;
    *(u32 *)(arg0 + 0x48) = arg4;
    *(u32 *)(arg0 + 0x3c) = arg6;
    *(u32 *)(arg0 + 0x40) = arg7;
    *(u32 *)(arg0 + 0x44) = 0;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B95A0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B95D0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B95E8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9678);

void func_002B96D8(s32 arg0) {
    *(u32 *)(arg0 + 4) = *(u32 *)(arg0 + 4) & 0xfffffffb;
}

void func_002B96F0(s32 arg0) {
    func_002B82A0(*(u32 *)(arg0 + 0x18));
}

void func_002B9708(s32 arg0) {
    func_002B83A0(*(u32 *)(arg0 + 0x18));
}

void func_002B9720(s32 arg0) {
    func_002B86E8(*(u32 *)(arg0 + 0x18));
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9738);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9788);

void func_002B97D8(u32 arg0) {
    func_002B9738(arg0, 0);
}

void func_002B97F0(u32 arg0) {
    func_002B9788(arg0, 0);
}

void func_002B9808(s32 arg0) {
    func_002B8F98(*(u32 *)(arg0 + 0x18));
}

void func_002B9820(s32 arg0) {
    func_002B8FB8(*(u32 *)(arg0 + 0x18));
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9838);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9918);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B99D8);

void func_002B9A38(void) {
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9A40);

void func_002B9BB0(s32 arg0, s32 arg1, u32 arg2, s32 arg3, u32 arg4) {
    func_002B9A40(arg0 - 0xf0, arg1 - 8, arg2, *(u32 *)(arg3 + 0x94),
                                *(u32 *)(arg3 + 0x18), *(u32 *)(arg3 + 0x90), arg4);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9BE0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9CD8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9CF8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9DD8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9EA0);

void func_002B9FB8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0x18) + 0x20);
    if (0 < temp_v0) {
        do {
            temp_v0 = temp_v0 - 1;
        } while (temp_v0 != 0);
    }
}

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AD38);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AD78);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AD88);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AD98);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042ADA8);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042ADB8);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042ADC8);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042ADD8);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042ADE8);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042ADF8);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AE08);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AE18);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AE28);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AE48);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AE58);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AE68);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AE80);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AE90);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AEA0);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AED0);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AEE8);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AF00);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9FF8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BA268);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BA308);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BA378);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AF48);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BA518);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BA660);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BA738);

void func_002BA7A8(void) {
    func_002BA738();
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BA7C0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BA7E8);

void func_002BA890(s32 arg0) {
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v2;

    temp_v2 = *(s32 *)(arg0 + 0x1c);
    temp_v1 = *(s32 *)(arg0 + 0x1c);
    while (temp_v0 = temp_v2, temp_v0 != 0) {
        temp_v1 = temp_v0;
        temp_v2 = *(s32 *)(temp_v0 + 0x5c);
    }
    *(s32 *)(arg0 + 0x10) = temp_v1;
}

void func_002BA8C8(s32 arg0) {
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v2;

    temp_v2 = *(s32 *)(arg0 + 0x1c);
    temp_v1 = *(s32 *)(arg0 + 0x1c);
    while (temp_v0 = temp_v2, temp_v0 != 0) {
        temp_v1 = temp_v0;
        temp_v2 = *(s32 *)(temp_v0 + 0x58);
    }
    *(s32 *)(arg0 + 0x14) = temp_v1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BA900);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BA978);

s32 func_002BAA28(s32 *arg0, s32 *arg1) {
    u32 temp_A = *(u32 *)(*arg0 + 0x60);
    u32 temp_B = *(u32 *)(*arg1 + 0x60);

    if (temp_B < temp_A) {
        return -1;
    }
    return temp_A < temp_B;
}

s32 func_002BAA50(s32 *arg0, s32 *arg1) {
    u32 temp_A = *(u32 *)(*arg0 + 0x60);
    u32 temp_B = *(u32 *)(*arg1 + 0x60);

    if (temp_B < temp_A) {
        return 1;
    }
    return (temp_A < temp_B) ? -1 : 0;
}

s32 func_002BAA80(s32 *arg0, s32 *arg1) {
    u32 temp_A = *(u32 *)(*arg0 + 0x64);
    u32 temp_B = *(u32 *)(*arg1 + 0x64);

    if (temp_B < temp_A) {
        return -1;
    }
    return temp_A < temp_B;
}

s32 func_002BAAA8(s32 *arg0, s32 *arg1) {
    u32 temp_A = *(u32 *)(*arg0 + 0x64);
    u32 temp_B = *(u32 *)(*arg1 + 0x64);

    if (temp_B < temp_A) {
        return 1;
    }
    return (temp_A < temp_B) ? -1 : 0;
}

s32 func_002BAAD8(s32 *arg0, s32 *arg1) {
    u32 temp_A = *(u32 *)(*arg0 + 0x68);
    u32 temp_B = *(u32 *)(*arg1 + 0x68);

    if (temp_B < temp_A) {
        return -1;
    }
    return temp_A < temp_B;
}

s32 func_002BAB00(s32 *arg0, s32 *arg1) {
    u32 temp_A = *(u32 *)(*arg0 + 0x68);
    u32 temp_B = *(u32 *)(*arg1 + 0x68);

    if (temp_B < temp_A) {
        return 1;
    }
    return (temp_A < temp_B) ? -1 : 0;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BAB30);

void func_002BAC58(s32 *list) {
    u32 i;
    for (i = 0; i < 4; i++) {
        list[i + 1] = func_00328E18(0x18);
    }
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BACA0);

void func_002BACF0(s32 arg0, s32 arg1, s32 *arg2) {
    u32 temp_v0 = *arg2;
    s32 *temp_v1 = arg2 + temp_v0;
    s32 *temp_v2;

    if (temp_v0 < 5) {
        return;
    }
    temp_v2 = (s32 *)temp_v1[1];
    *arg2 = temp_v0 + 1;
    temp_v2[0] = arg0;
    temp_v2[4] = arg1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BAD20);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BAE08);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BAE98);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BAF10);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BAF50);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BB0D0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BB0E8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BB290);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BB320);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BB370);

void func_002BB418(u32 arg0) {
    func_002BB320();
    func_00328E48(arg0);
}

void func_002BB440(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x34);
    *(s32 *)(arg0 + 0x2c) = temp_v0;
    *(u32 *)(arg0 + 0x30) = *(u32 *)(arg0 + 0x38);
    *(u32 *)(arg0 + 0x34) = 0;
    if (temp_v0 != 0) {
        func_00305B00(temp_v0, *(u32 *)(arg0 + 0x38), *(u32 *)(arg0 + 0x44), 0, 10, 2);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BB498);

u8 func_002BB500(s32 arg0) {
    return *(s32 *)(arg0 + 0x2c) != 0;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BB510);

void func_002BB850(s32 arg0, u32 arg1, u32 arg2, u32 arg3, s32 arg4
                                    ) {
    u32 temp_v0;

    temp_v0 = func_00305348(arg1, arg2, 1);
    *(u32 *)(arg0 + 0xe4) = temp_v0;
    temp_v0 = func_00305348(arg1, arg3, 1);
    *(u32 *)(arg0 + 0xe8) = temp_v0;
    if (-1 < arg4) {
        temp_v0 = func_00305348(arg1, arg4, 1);
        *(u32 *)(arg0 + 0xec) = temp_v0;
    }
}

void func_002BB8D8(s32 arg0) {
    u32 temp_v0;
    u32 *puVar2;
    u32 *puVar3;
    u32 *puVar4;
    u32 temp_v1;

    puVar2 = (u32 *)(arg0 + 0x7c);
    puVar3 = (u32 *)(arg0 + 0x164);
    puVar4 = (u32 *)(arg0 + 0x160);
    temp_v1 = 0;
    do {
        if (puVar2[0x38] != 0) {
            func_003054E8(puVar2[0x38]);
        }
        if (puVar2[0x39] != 0) {
            func_003054E8(puVar2[0x39]);
        }
        if (puVar2[0x3a] != 0) {
            func_003054E8(puVar2[0x3a]);
        }
        temp_v0 = *puVar2;
        temp_v1 = temp_v1 + 1;
        puVar2[0x38] = 0;
        *puVar4 = 0;
        *puVar2 = temp_v0 & 0xffffffbf;
        puVar2 = puVar2 + 0x84e;
        *puVar3 = 0;
        puVar3 = puVar3 + 0x84e;
        puVar4 = puVar4 + 0x84e;
    } while (temp_v1 < 5);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BB998);

void func_002BB9C8(u32 *arg0, u32 arg1) {
    *arg0 = arg1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BB9D0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BBA38);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BBE78);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BBF38);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BBFC8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BC078);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BC0A8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BC120);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BC258);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BC2B8);

void func_002BC3C8(s32 arg0, s32 arg1, u32 arg2) {
    u32 temp_v0;

    temp_v0 = func_002BC120(arg2);
    *(u32 *)(arg0 * 0x2138 + arg1 + 0x158) = temp_v0;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BC410);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BC460);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BC498);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BC580);

void func_002BC5D0(s32 arg0, u32 *arg1) {
    u32 temp_v0;
    u32 *puVar2;
    u32 temp_v1;

    puVar2 = (u32 *)(arg0 + 0x24);
    temp_v1 = 0;
    do {
        temp_v0 = *arg1;
        arg1 = arg1 + 1;
        temp_v1 = temp_v1 + 1;
        *puVar2 = temp_v0;
        puVar2 = puVar2 + 1;
    } while (temp_v1 < 8);
}

void func_002BC600(s32 arg0, u32 *arg1) {
    u32 temp_v0;
    u32 *puVar2;
    u32 temp_v1;

    puVar2 = (u32 *)(arg0 + 0x44);
    temp_v1 = 0;
    do {
        temp_v0 = *arg1;
        arg1 = arg1 + 1;
        temp_v1 = temp_v1 + 1;
        *puVar2 = temp_v0;
        puVar2 = puVar2 + 1;
    } while (temp_v1 < 8);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BC630);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BC690);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BC9A8);

void func_002BCA98(u32 arg0) {
    func_002BC9A8(arg0, 0);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BCAB0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BCBD8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BCCB0);

void func_002BCCF0(u32 arg0, u32 arg1) {
    func_002BCCB0();
    func_002BCBD8(arg0, arg1);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BCD28);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BCD90);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BCE50);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BCF60);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BCFC8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BD090);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BD1D0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BD2E0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BD358);

void func_002BD3A8(s32 arg0) {
    u32 *puVar1;
    u32 temp_v0;

    puVar1 = (u32 *)(arg0 + 0x7c);
    temp_v0 = 0;
    do {
        temp_v0 = temp_v0 + 1;
        *puVar1 = *puVar1 & 0xfffffffe;
        puVar1 = puVar1 + 0x84e;
    } while (temp_v0 < 5);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BD3E0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BD480);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AF90);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AFA0);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AFB8);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AFD8);

INCLUDE_SDATA(const s32, "game/code_002B0278", D_00437BE0);

INCLUDE_SDATA(const s32, "game/code_002B0278", D_00437BE8);

INCLUDE_SDATA(const s32, "game/code_002B0278", D_00437BF0);

INCLUDE_SDATA(const s32, "game/code_002B0278", D_00437BF8);

INCLUDE_SDATA(const s32, "game/code_002B0278", D_00437C00);

INCLUDE_SDATA(const s32, "game/code_002B0278", D_00437C08);

INCLUDE_SDATA(const s32, "game/code_002B0278", D_00437C10);

INCLUDE_SDATA(const s32, "game/code_002B0278", D_00437C18);

INCLUDE_SDATA(const s32, "game/code_002B0278", D_00437C20);

INCLUDE_SDATA(const s32, "game/code_002B0278", D_00437C28);

INCLUDE_SDATA(const s32, "game/code_002B0278", D_00437C30);
