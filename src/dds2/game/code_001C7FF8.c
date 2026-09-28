#include "common.h"

extern u64 func_001D4160(u64, u64, u64);

extern s32 func_001AA6F8(void);

extern s64 func_00101740(u32);

extern u64 func_0019F5E8(s32, s32, u64, u64, u64, u64);

extern u32 func_001CCBB8(void);

extern s32 D_00435DEC;

extern u32 D_004367BC;

extern void func_001C35F0(s32, s32, s32);

extern u32 func_00101958(s64);

extern void func_00230960(s32);

extern s32 D_003B6940[];

extern s32 func_00230978(void);

extern void kwlnFadeInStart(s32, s32, s32, s32);

typedef struct {
    void (*initialize)(s32);
    s32 (*update)(s32);
    s32 flags;
} SceneInitializer;

extern SceneInitializer D_003B6938[];

extern u32 func_001B57B0(void);

extern s32 func_001AC750(s32, void *);

extern s32 D_004367C0;

extern char D_003B5D10[];

extern s32 getEntryFlagsUnlessDisabled(void *);

extern void func_001D3978(void);

extern u32 D_00435E64;

extern u32 D_00435E5C;

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C7FF8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C8078);

void func_001C80C0(void) {
}

void func_001C80C8(void) {
}

void func_001C80D0(s32 arg0, s32 arg1, u64 arg2, u64 arg3) {
    u64 temp_v0;

    func_0019B8B0(0x13);
    temp_v0 = func_0019F5E8(arg0 << 4, arg1 << 3, 0, arg2, arg3, 0);
    func_0019D550(temp_v0, 1, 0x53);
    func_0019C5B0(temp_v0);
    func_0019B8B0(0xffffffffffffffff);
}

void func_001C8158(s32 x, s32 y, s32 z, s32 w, u16 index) {
    s32 handle;
    func_0019B8B0(0x13);
    handle = func_0019F460(x << 4, y << 3, z, w, D_00435E64 + index * 17, 0);
    func_0019D530(handle, 1);
    func_0019C5B0(handle);
    func_0019B8B0(-1);
}

void func_001C81F8(s32 x, s32 y, s32 z, s32 w, u16 index) {
    s32 handle;
    func_0019B8B0(0x13);
    handle = func_0019F460(x << 4, y << 3, z, w, D_00435E5C + index * 25, 0);
    func_0019D530(handle, 1);
    func_0019C5B0(handle);
    func_0019B8B0(-1);
}

s32 func_001C82A0(s32 unused, u32 limit) {
    func_001AA6F8();
    if (limit < func_001B57B0()) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C82D8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C83D0);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C8518);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C8768);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C89A0);

char *func_001C8A28(s32 object, s16 *value) {
    s32 cached = D_004367C0;
    if (cached == 0) {
        cached = func_001AC750(*(s32 *)(*(s32 *)(object + 0x2C) + 0x18), D_003B5D10);
        D_004367C0 = cached;
    }
    *value = cached;
    return D_003B5D10;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C8A80);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C91D8);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_004169C0);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C92A0);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C98E8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C9BE8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C9DC8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001C9EA0);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CA390);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_004169F0);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416A00);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416A10);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CA490);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CA760);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CA790);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CA7E0);

s64 func_001CA820(void) {
    s64 temp_v0;

    temp_v0 = func_00101740(D_004367BC);
    if (temp_v0 == 0) {
        return temp_v0;
    }
    return *(s32 *)func_001CA7E0();
}

s32 func_001CA858(void) {
    s32 actor = *(s32 *)(func_001AA6F8() + 0x24C);
    while (actor != 0) {
        if ((*(u64 *)(actor + 0x110) & 0x421) == 0x401) {
            u16 kind = *(u16 *)(actor + 0x124);
            if (kind == 0x4C || kind == 0x3C) {
                return 1;
            }
        }
        actor = *(s32 *)(actor + 0x364);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CA8D8);

s32 func_001CA958(void) {
    s32 actor = *(s32 *)(func_001AA6F8() + 0x24C);
    while (actor != 0) {
        if ((*(u64 *)(actor + 0x110) & 0x421) == 0x401 &&
            (*(u16 *)(actor + 0x12E) & 1) != 0) {
            return 1;
        }
        actor = *(s32 *)(actor + 0x364);
    }
    return 0;
}

s32 func_001CA9C0(void) {
    s32 actor = *(s32 *)(func_001AA6F8() + 0x24C);
    while (actor != 0) {
        if ((*(u64 *)(actor + 0x110) & 0x421) == 0x401 &&
            getEntryFlagsUnlessDisabled((void *)(actor + 0x120)) != 0) {
            return 0;
        }
        actor = *(s32 *)(actor + 0x364);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CAA30);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CAB60);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CB158);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CB190);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CB278);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416BB8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CB498);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416C98);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416CC8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CB7A8);

u32 func_001CC018(void) {
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416D00);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CC020);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CC438);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CC7C8);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416D28);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CC8D0);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CC9C0);

void func_001CCB50(u32 arg0) {
    func_001E8018(*(u32 *)((s32)arg0 + 0x10));
    func_001E8018(*(u32 *)((s32)arg0 + 0xc));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CCB88);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CCBB8);

u32 func_001CCBF8(void) {
    u32 *puVar1;

    puVar1 = (u32 *)func_001CCBB8();
    return *puVar1;
}

u32 func_001CCC18(void) {
    u32 *puVar1;

    puVar1 = (u32 *)(func_001CCBB8() + 0x10);
    return *puVar1;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CCC38);

void func_001CCD50(void) {
    u32 *temp_v0;

    temp_v0 = (u32 *)func_001CCBB8();
    if (temp_v0 != 0) {
        *temp_v0 = 6;
    }
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CCD80);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416D58);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416D88);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CCEB8);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416DE0);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416DF0);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416E50);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CD6E0);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CDD38);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416ED0);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CDEC0);

void func_001CE3E8(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = arg1 * 0xa0 + *(s32 *)(arg0 + 0x18);
    *(s32 *)(temp_v0 + 0xc) = *(s32 *)(temp_v0 + 0x7c) << 4;
    *(s32 *)(temp_v0 + 0x10) = *(s32 *)(temp_v0 + 0x80) << 3;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CE418);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CE548);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CE5C8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CE838);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CF0B0);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CF500);

INCLUDE_ASM(const s32, "game/code_001C7FF8", findBattleSceneSlotById);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CF630);

INCLUDE_ASM(const s32, "game/code_001C7FF8", fadeStaleBattleSceneSlots);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CF720);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CF800);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CFA60);

void func_001CFAC0(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    *(u32 *)(temp_v0 + 0x2d8) = 0;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CFAE0);

void func_001CFB20(void) {
    func_001AA6F8();
    func_001C7DB8(0, 8);
}

void func_001CFB48(void) {
    func_001B7238();
}

u32 func_001CFB60(s32 arg0) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    return *(u32 *)(arg0 * 4 + temp_v0 + 0x4e4);
}

void func_001CFB90(void) {
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CFB98);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CFC40);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CFCA8);

void func_001CFEF8(void) {
}

u32 func_001CFF00(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001CFF08);

s32 func_001CFFC8(void) {
    if (func_00203FD8() != 0 &&
        func_0022B108() != 0 &&
        func_0022E460() != 0) {
        loadBattleSoundBank();
        func_0022B288();
        return 3;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D0020);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D0078);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D0140);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D0710);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D08A8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D0FE0);

void func_001D1120(void) {
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D1128);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D1190);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D1200);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D14B0);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D1700);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416EF8);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416F08);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416F18);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416F28);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00416FA0);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D18D8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D22D8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D2798);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D28D8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D2A20);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D2A78);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D2C10);

u32 func_001D2C40(void) {
    if (func_00230978() != 0) {
        kwlnFadeInStart(0, 0, 0, 0);
        return 2;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", setBattleScene);

void func_001D2CD0(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    *(u32 *)(temp_v0 + 0x230) = arg0;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", updateBattleScene);

void func_001D2D78(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    setBattleScene(1);
    *(u32 *)(temp_v0 + 0x230) = 0;
}

void func_001D2DB0(void) {
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D2DB8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D2DF0);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D2E68);

s32 func_001D2EC8(s32 *object) {
    u32 flags;
    func_001AA6F8();
    if (object[2] & 0x40) {
        return 8;
    }
    flags = *(u32 *)(object[6] + 0x110) & 0xE00;
    switch (flags) {
    case 0x200: return 0x14;
    case 0x400: return 0x2D;
    case 0x800: return 0xF;
    default: return 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D2F40);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D3018);

s32 func_001D3098(u8 *object) {
    u32 flags = *(u32 *)(*(u8 **)(object + 0x18) + 0x110) & 0xE00;
    s32 result;
    switch (flags) {
    case 0x200:
        result = 1;
        break;
    case 0x400:
        result = 2;
        break;
    case 0x800:
        result = 3;
        break;
    default:
        result = 0;
        break;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D30E0);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D34B8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D3520);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D3690);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D37E8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D3898);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D38F0);

void func_001D3978(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    func_001D3018(temp_v0 + 0x318, 0x14);
    func_001D2F40(temp_v0 + 0x368, 0x2d);
    func_001D2F40(temp_v0 + 0x41c, 0xf);
    func_001D30E0();
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D39C8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D3A78);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D3B38);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D3BA8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D3C00);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D3D40);

void func_001D3E00(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    *(u32 *)(temp_v0 + 0x218) = *(u32 *)(temp_v0 + 0x218) | 0xc;
}

void func_001D3E28(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    *(u32 *)(temp_v0 + 0x218) = *(u32 *)(temp_v0 + 0x218) & 0xfffffffb;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D3E58);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D3E90);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D3ED8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D4020);

u32 func_001D4120(s32 *arg0) {
    u8 temp_v0;

    if (*arg0 == 0) {
        temp_v0 = (u8)arg0[2];
    }
    else {
        if ((*(u32 *)(*arg0 + 8) & 0x40) != 0) {
            return 1;
        }
        temp_v0 = (u8)arg0[2];
    }
    func_001D3520(arg0[1], temp_v0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D4160);

void func_001D4200(void) {
    s32 temp_v0;

    temp_v0 = func_001AA6F8();
    *(u32 *)(temp_v0 + 0x218) = *(u32 *)(temp_v0 + 0x218) & 0xfffffffb;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D4230);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D4290);

u32 func_001D4328(u32 *arg0) {
    func_001D37E8(*arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D4348);

void func_001D43B0(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) | 1;
}

void func_001D43C0(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) & 0xfffffffe;
}

void func_001D43D8(s32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = *(u32 *)(arg1 + 0x110);
    *(s32 *)(arg0 + 0x18) = arg1;
    if ((temp_v0 & 0x400) != 0) {
        if (0x17f < *(u16 *)(arg1 + 0x124)) {
            temp_v0 = *(u32 *)(arg0 + 8);
            goto LAB_001c8880;
        }
        *(u16 *)(arg0 + 4) =
                  (u16)*(u8 *)(((u32)*(u16 *)(arg1 + 0x124) * 0x14 -
                                                      (u32)*(u16 *)(arg1 + 0x124)) * 4 + D_00435DEC + 0x15);
    }
    temp_v0 = *(u32 *)(arg0 + 8);
LAB_001c8880:
    *(u32 *)(arg0 + 8) = temp_v0 | 8;
}

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00417278);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00417288);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D4438);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D4590);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D46A8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D4820);

void func_001D48E0(void) {
}

void func_001D48E8(void) {
}

void func_001D48F0(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) & 0xfffffdff;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D4908);

void func_001D4A30(s32 arg0) {
    *(u32 *)(arg0 + 8) = (*(u32 *)(arg0 + 8) | 0x10) & ~0x200;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D4A48);

void func_001D4B90(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) & 0xffffffef;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D4BA8);

void func_001D4C98(void) {
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D4CA0);

void func_001D4FE0(void) {
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D4FE8);

void func_001D5330(u32 arg0) {
    func_001AA6F8();
    *(u32 *)((s32)arg0 + 8) = *(u32 *)((s32)arg0 + 8) & 0xfffffffb;
    func_001CAB60(arg0);
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D5368);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D54B8);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D5668);

void func_001D5938(s32 arg0) {
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) & 0xffffff7f;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D5950);

void func_001D5B38(s32 arg0) {
    func_001C35F0(arg0, 0, 0);
    *(u32 *)(arg0 + 8) = *(u32 *)(arg0 + 8) | 0x20;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D5B70);

void func_001D5BD8(void) {
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D5BE0);

void func_001D5CE8(s32 arg0) {
    func_001B7830();
    func_001DF700(arg0 + 0x20);
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x314) = 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D5D20);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D5E50);

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D5EE8);

void func_001D5FA8(void) {
}

INCLUDE_ASM(const s32, "game/code_001C7FF8", func_001D5FB0);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00417348);

INCLUDE_RODATA(const s32, "game/code_001C7FF8", D_00417360);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436858);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436860);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436868);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436870);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436878);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436880);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436888);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436890);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436898);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368A0);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368A8);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368B0);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368BC);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368BE);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368C0);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368C8);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368D0);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368D8);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368E0);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368E8);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368F0);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004368F8);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436900);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436908);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436910);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436918);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436920);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436928);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436930);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436938);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436940);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436948);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436950);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436958);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436960);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436968);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436970);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436978);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436980);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436988);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436990);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436998);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004369A0);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004369A8);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004369B0);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004369B8);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004369C0);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004369C8);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004369D0);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004369D8);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004369E0);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004369E8);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004369F0);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_004369F8);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436A00);

INCLUDE_SDATA(const s32, "game/code_001C7FF8", D_00436A08);

