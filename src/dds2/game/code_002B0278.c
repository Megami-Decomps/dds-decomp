#include "common.h"

extern s32 D_00435E5C;

extern s32 func_002C6CE8(void);

extern s32 func_002B86E8(u32);
extern void func_002AAE80();
extern char D_00380788[];
extern void func_002C72E0();
extern void func_002B96D8();
extern void func_002BD3E0();
extern char D_003E7790[];
extern char D_003E7720[];
extern void func_002C48C8();
extern void func_002BD480();
extern char D_003E7758[];
extern s32 func_002B5DE8();
extern u32 func_002C44E8();
extern s32 func_002C50C0();
extern void func_002C42B0();
extern void func_002C48F0();
extern u32 D_003E7828[];
extern u32 D_003E7858[];
extern char *D_003E7818[];
extern char *D_003E7820[];
extern u32 func_00304030(char *, char *, s32);
extern u32 loadMappedEffectResource(char *, char *);
extern void requestEffectResourceByMode(char *, char *, s32, u32 *);
extern void requestMappedEffectResource(char *, char *, u32 *);
extern void func_002BCE50();
extern u8 *func_002B8A50();
extern u8 *func_002B8BA8();
extern void func_002BA268();
extern s32 func_0026C6A0();
extern void func_002C42C0();
extern void func_002C4430();
extern s32 func_002B06A8();
extern void func_002AFB38();
extern char D_003E7530[];
extern void func_002AAC70();
extern s32 D_00435E6C;
extern void func_002AACB8();
extern s32 func_00305080();
extern void func_002AAC98();
extern char D_003E69B0[];
extern void func_002AA740();
extern void func_002AA7A0();
extern void func_002BB0E8();
extern void func_002B3940();
extern void func_002B2408();

extern u32 func_002BC120(u32);

extern u32 func_002B9FF8(u32);

extern s32 D_00435DD0;
typedef struct MenuSlot {
    s32 resources[3];
    u16 unused;
    u16 flags;
} MenuSlot;
extern MenuSlot *D_00435E24;

extern void func_002C21F8(s32);

extern s32 func_00101958();

extern void func_002B9EA0(s32, s32, s32, s32, s32);

extern void func_00304EE0(s32);

extern s64 func_002C4038(s32, s32 *, u64, u64);

/* Menu state handler installer: the call is inlined at each use, so callers
 * return its result through a real call rather than a sibcall. */
static inline s64 menuSetHandler(s32 context, u64 mode, s32 callback) {
    return func_002C4038(context + 8, (s32 *)(context + 0x54), mode, callback);
}

extern void func_0026C900(void);
extern void func_002B9520(u32);
extern void *memset(void *, s32, u32);

u32 *func_002B9918(u32 first, u32 second, u32 third,
                    u32 fourth, u32 fifth, u32 sixth);

typedef struct MenuListDefaults {
    u32 fields[3];
} MenuListDefaults;
extern MenuListDefaults D_0042AF00;


extern void func_002B0278(s32);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B0278);

s64 func_002B0578(s32 callback) {
    s32 context = func_00101958();
    func_002B0278(callback);
    return menuSetHandler(context, 1, callback);
}

s64 func_002B05C8(s32 callback) {
    s32 context = func_00101958();
    func_0026C900();
    return menuSetHandler(context, 2, callback);
}

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

s64 func_002B09C8(s32 callback) {
    s32 context = func_00101958();
    func_002B0278(callback);
    return menuSetHandler(context, 1, callback);
}

s64 func_002B0A18(s32 callback) {
    s32 context = func_00101958();
    func_0026C900();
    return menuSetHandler(context, 2, callback);
}

void func_002B0A60(u32 context, s32 selection) {
    s32 offset;
    s32 i;
    if (selection == 0) {
        return;
    }
    selection -= 0xc0;
    offset = 0x1e670 + selection * 5;
    i = 4;
    do {
        ((u8 *)D_00435DD0)[offset++] = 0;
    } while (--i >= 0);
    ((u8 *)(selection + D_00435DD0))[0x1e7b0] = 0;
    func_003144E8(context);
}

u32 func_002B0AD8(void) {
    s32 context = func_00101958();
    s32 slot = D_00435DD0 + **(s32 **)(*(s32 *)(context + 0xa914) + 0x1c) * 0x1c4 + 0xa60;
    s32 sel;
    func_002C1B68(context + 0xaa50, 1);
    sel = func_002C55C0(slot);
    func_0026C918(0, D_00435E5C + sel * 0x19);
    func_0026C5B8(0xd);
    func_0026C648(0);
    func_0026C618(0xf);
    return 1;
}

u32 func_002B0B88(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B0B90);

s64 func_002B0CB0(s32 callback) {
    s32 context = func_00101958();
    func_002B0278(callback);
    return menuSetHandler(context, 1, callback);
}

s64 func_002B0D00(s32 callback) {
    s32 context = func_00101958();
    func_0026C900();
    return menuSetHandler(context, 2, callback);
}

u32 func_002B0D48(void) {
    return 1;
}

void func_002B0D50(u32 arg0) {
    func_002A9460(4, arg0);
}

void func_002B0D70(u32 callback) {
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

u32 func_002B1B90(void) {
    s32 context = func_00101958();
    u32 *selection = *(u32 **)(context + 0xaa48);
    func_002B18A0(context);
    func_002B1150(context);
    func_002B0D70(context);
    func_003297C8(*selection);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B1BF0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B1C68);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B1EA8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B2338);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B2408);

s64 func_002B2698(s32 callback) {
    s32 context = func_00101958();
    u8 *menu = *(u8 **)(context + 0xaa48);
    s32 list;
    func_002AAE80(callback);
    func_002AA740(0x17);
    list = *(s32 *)(*(s32 *)(menu + 8) + 0x18);
    if (func_002B0D78(**(s32 **)(list + 0x1c), list)) {
        **(u32 **)(*(s32 *)(menu + 8) + 0x18) |= 0x10;
    } else {
        **(u32 **)(*(s32 *)(menu + 8) + 0x18) &= ~0x10;
    }
    func_002BB0E8(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    if (*(s32 *)(menu + 0x1dd8) == 0) {
        func_002B2408(context);
    }
    func_002AA7A0(0, *(s32 *)(context + 0x60));
    return menuSetHandler(context, 1, callback);
}

/*W13D2:func_002B2790*/
s64 func_002B2790(s32 callback) {
    s32 context = func_00101958();
    return menuSetHandler(context, 2, callback);
}
/*W13D2END*/

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

void func_002B2818(s32 menu) {
    u32 *handles = (u32 *)(menu + 8);
    s32 remaining = 1;
    do {
        func_00304EE0(*handles++);
    } while (--remaining >= 0);
}

void func_002B2860(s32 menu) {
    u32 *handles = (u32 *)(menu + 8);
    s32 remaining = 1;
    do {
        func_00305068(*handles++);
    } while (--remaining >= 0);
}

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

s64 func_002B2B48(s32 callback) {
    s32 context = func_00101958();
    s32 *menu = *(s32 **)(context + 0xaa48);
    if (func_00305080(*(s32 *)(context + 0x64))) {
        func_002AACB8(0, callback);
    } else {
        func_002AACB8(1, callback);
    }
    if (menu[5] == 0) {
        func_002AA740(0x16);
    } else {
        func_002AA740(0x15);
    }
    func_002BB0E8(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    func_002AA7A0(0, *(s32 *)(context + 0x60));
    if (menu[9] != 0) {
        func_002AAC98(0, **(s32 **)(*(s32 *)(*(s32 *)(context + 0x104) + 0x18) + 0x1c), D_003E69B0, context, 1, 0x53);
    }
    return menuSetHandler(context, 1, callback);
}

s64 func_002B2C50(s32 callback) {
    s32 context = func_00101958();
    return menuSetHandler(context, 2, callback);
}

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

s64 func_002B39F0(s32 callback) {
    s32 context = func_00101958();
    s32 *menu = *(s32 **)(context + 0xaa48);
    if (menu[4] == 0) {
        func_002B3940(menu);
    }
    return menuSetHandler(context, 2, callback);
}

u32 func_002B3A58(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B3A60);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B3CA0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B3E80);

u32 func_002B40B8(u32 callback) {
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

void func_002B4290(s32 context) {
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

void func_002B4C48(s32 context) {
    u32 *menu = *(u32 **)(context + 0xaa48);
    if (menu[2] != 0) {
        u32 i = 0;
        u32 *resource = menu + 4;
        func_002C07A0(menu[8]);
        func_002B81C8(menu[3]);
        do {
            func_002B9520(*resource++);
            i++;
        } while (i < 4);
        menu[2] = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B4CC8);

s32 func_002B4DE8(s32 selection) {
    s32 context = func_00101958();
    s32 *menu = *(s32 **)(context + 0xaa48);
    if (menu[9] != 0) {
        func_002B40B8(selection);
    }
    func_002B4290(context);
    func_003297C8(menu[0]);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B4E58);

s64 func_002B5028(s32 callback) {
    s32 context = func_00101958();
    func_002AAE80(callback);
    if (**(s32 **)(*(s32 *)(*(s32 *)(context + 0x104) + 0x18) + 0x1c) == 0) {
        func_002AA740(1);
    } else {
        func_002AA740(0xe);
    }
    func_002AAC98(0, **(s32 **)(*(s32 *)(*(s32 *)(context + 0x104) + 0x18) + 0x1c), D_003E69B0, context, 1, 0x53);
    if (**(s32 **)(*(s32 *)(*(s32 *)(context + 0x104) + 0x18) + 0x1c) == 0) {
        func_002BB0E8(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    } else {
        func_002BB0E8(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    }
    func_002AA7A0(0, *(s32 *)(context + 0x60));
    return menuSetHandler(context, 1, callback);
}

s64 func_002B5128(s32 callback) {
    s32 context = func_00101958();
    return menuSetHandler(context, 2, callback);
}

void func_002B5160(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    *(u32 *)(*(s32 *)(temp_v0 + 0xaa48) + 0x34) = 0xffffffff;
}

u32 func_002B5190(s32 callback) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    return ~*(u32 *)(*(s32 *)(temp_v0 + 0xaa48) + 0x34) >> 0x1f;
}

void func_002B51C8(void) {
    u8 *state = *(u8 **)(func_00101958() + 0xaa48);
    u8 *node = *(u8 **)(*(u8 **)(*(u8 **)(state + 0x24) + 0x18) + 0x10);
    while (node != NULL) {
        if (*(u32 *)node == *(u32 *)(state + 0x34)) {
            *(u32 *)(node + 0x48) |= 2;
        } else {
            *(u32 *)(node + 0x48) &= ~2;
        }
        node = *(u8 **)(node + 0x58);
    }
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B5240);

u32 func_002B5358(void) {
    s32 context = func_00101958();
    u32 *state = *(u32 **)(context + 0xaa48);
    s32 image = *(s32 *)(context + 0x104);
    if (**(s32 **)(*(s32 *)(image + 0x18) + 0x1c) == 0) {
        func_002BAF50(image, context + 0xb10c);
    }
    state[12] = 0;
    return 1;
}

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

s64 func_002B5980(s32 callback) {
    s32 context = func_00101958();
    s32 *menu = *(s32 **)(context + 0xaa48);
    s64 state = func_002C4038(context + 8, (s32 *)(context + 0x54), 0, callback);
    if (state != 0) {
        return state;
    }
    if (**(s32 **)(*(s32 *)(*(s32 *)(context + 0x104) + 0x18) + 0x1c) == 0) {
        func_002B5450(callback);
    } else if (menu[12] == 0) {
        func_002B5580(callback);
    } else {
        func_002B57A8(callback);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B5A30);

s64 func_002B5BE8(s32 callback) {
    s32 context = func_00101958();
    s32 *menu = *(s32 **)(context + 0xaa48);
    s32 label;
    if (**(s32 **)(*(s32 *)(*(s32 *)(context + 0x104) + 0x18) + 0x1c) == 0) {
        func_002BB0E8(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    } else {
        func_002BB0E8(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
        func_002B5A30(context);
    }
    func_002AAE80(callback);
    if (**(s32 **)(*(s32 *)(*(s32 *)(context + 0x104) + 0x18) + 0x1c) == 0) {
        func_002AA740(2);
    } else if (menu[12] != 0) {
        if (func_002B5190(callback) == 0) {
            func_002AA740(0x12);
        } else {
            func_002AA740(0x13);
        }
    } else if (**(s32 **)(*(s32 *)(*(s32 *)((s32)menu + 0x10 + (**(s32 **)(menu[3] + 0x1c) << 2)) + 0x18) + 0x1c) == 0) {
        func_002AA740(0x11);
    } else {
        func_002AA740(0x10);
    }
    label = *(s32 *)(*(s32 *)(*(s32 *)(menu[9] + 0x18) + 0x1c) + 0x60);
    if (label != 0xffff && label != 0) {
        func_002AAC70(0, label, D_00435E6C, context, 1, 1, 0x53);
    } else {
        func_002AAC98(0, 0, 0, context, 1, 0x53);
    }
    func_002AA7A0(0, *(s32 *)(context + 0x60));
    return menuSetHandler(context, 1, callback);
}

s64 func_002B5DB0(s32 callback) {
    s32 context = func_00101958();
    return menuSetHandler(context, 2, callback);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B5DE8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B5F00);

s64 func_002B5FA8(s32 callback) {
    s32 context = func_00101958();
    u8 *menu = *(u8 **)(context + 0xaa48);
    s32 *popup = (s32 *)(context + 0x54);
    u32 buttons = func_002C44E8(3);
    s64 state;
    s32 label;
    u16 code;
    u8 *window;
    state = func_002C4038(context + 8, popup, 0, callback);
    if (state != 0) {
        return state;
    }
    label = *(s32 *)(*(s32 *)(*(s32 *)(*(s32 *)(menu + 0x24) + 0x18) + 0x1c) + 0x60);
    code = label;
    if (func_002C50C0(code) == 2) {
        *(u32 *)(context + 0x284) |= 0x10;
    }
    if (func_002C50C0(code) == 3) {
        *(u32 *)(context + 0x284) |= 0x20;
    }
    window = (u8 *)(context + 0x284);
    func_002BD480(8, window);
    if (buttons & 1) {
        buttons = func_002B5DE8(label, context) == 0 ? 0x8000 : 0;
        func_002B5F00(context);
    }
    if (buttons & 2) {
        func_002C42B0(popup, D_003E7758);
        func_002BD3E0(1, window);
    }
    func_002C48F0(0, buttons, 0);
    return 0;
}

s64 func_002B60E8(s32 callback) {
    s32 context = func_00101958();
    u8 *menu = *(u8 **)(context + 0xaa48);
    func_002AAE80(callback);
    func_002AA740(3);
    func_002AAC70(0, *(s32 *)(*(s32 *)(*(s32 *)(*(s32 *)(menu + 0x24) + 0x18) + 0x1c) + 0x60), D_00435E6C, context, 1, 1, 0x53);
    **(u32 **)(*(s32 *)(menu + 0x24) + 0x18) &= ~8;
    func_002BB0E8(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    func_002AA7A0(0, *(s32 *)(context + 0x60));
    return menuSetHandler(context, 1, callback);
}

s64 func_002B61C0(s32 callback) {
    s32 context = func_00101958();
    return menuSetHandler(context, 2, callback);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B61F8);

u32 func_002B62A8(u32 callback) {
    s32 context = func_00101958();
    func_002BAF50(*(u32 *)(context + 0x104), context + 0xb10c);
    func_002B40B8(callback);
    func_002B4C48(context);
    func_002BD2E0(context + 0x284);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B6308);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B63F0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B6498);

s64 func_002B66D8(s32 callback) {
    s32 context = func_00101958();
    s32 *menu = *(s32 **)(context + 0xaa48);
    s32 index = **(s32 **)(menu[3] + 0x1c);
    s32 label = *(s32 *)(*(s32 *)(*(s32 *)(*(s32 *)((s32)menu + 0x10 + (index << 2)) + 0x18) + 0x1c) + 0x60);
    func_002B5A30(context);
    func_002AAE80(callback);
    func_002AA740(0xf);
    if (label != 0xffff && label != 0) {
        func_002AAC70(0, label, D_00435E6C, context, 1, 1, 0x53);
    } else {
        func_002AAC98(0, 0, 0, context, 1, 0x53);
    }
    **(u32 **)(menu[9] + 0x18) |= 8;
    func_002BB0E8(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    func_002AA7A0(3, *(s32 *)(context + 0x60));
    return menuSetHandler(context, 1, callback);
}

s64 func_002B6800(s32 callback) {
    s32 context = func_00101958();
    return menuSetHandler(context, 2, callback);
}

INCLUDE_ASM(const s32, "game/code_002B0278", drawSelectionLabel);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B6898);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B6B00);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B6C70);

s32 func_002B6D08(s32 id) {
    MenuSlot *entry;
    s32 i;
    id -= 0x1ab;
    entry = (MenuSlot *)((id << 4) + (s32)D_00435E24);
    if ((entry->flags & 2) != 0) {
        return 0;
    }
    for (i = 0; i < 3; i++) {
        if (entry->resources[i] != -1) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B6D78);

u32 func_002B6FA8(u32 callback) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    temp_v0 = *(s32 *)(temp_v0 + 0xaa48);
    func_002B9520(*(u32 *)(temp_v0 + 0x24));
    *(u32 *)(temp_v0 + 0x24) = 0;
    return 1;
}

u32 func_002B6FE8(u32 callback) {
    s32 context;
    u32 *state;
    func_002B4CC8(callback);
    context = func_00101958(callback);
    state = *(u32 **)(context + 0xaa48);
    func_002B6D78(callback);
    func_002BD358(context + 0x284);
    state[11] = 0;
    func_002BAF50(state[9], context + 0xb10c);
    return 1;
}

u32 func_002B7060(u32 callback) {
    s32 context = func_00101958();
    func_002BAF50(*(u32 *)(context + 0x104), context + 0xb10c);
    func_002B6FA8(callback);
    func_002BD3A8(context + 0x284);
    func_002B4DE8(callback);
    return 1;
}

s64 func_002B70C0(s32 callback) {
    s32 context = func_00101958();
    s32 *menu = *(s32 **)(context + 0xaa48);
    s32 *popup = (s32 *)(context + 0x54);
    u32 buttons = func_002C44E8(0xc32);
    s64 state;
    s32 *list;
    state = func_002C4038(context + 8, popup, 0, callback);
    if (state != 0) {
        return state;
    }
    if ((buttons & 0x300000) == 0) {
        list = menu + 1;
        func_002B9808(list[8 + menu[11]]);
    }
    list = menu + 1;
    if (buttons & 0x10) {
        func_002B97F0(list[8 + menu[11]]);
    }
    if (buttons & 0x20) {
        func_002B97D8(list[8 + menu[11]]);
    }
    func_002C48C8(list[8 + menu[11]], &buttons);
    func_002B96D8(list[8 + menu[11]]);
    func_002C48F0(0, buttons, *(s32 *)(list[8 + menu[11]] + 0x18));
    if (buttons & 2) {
        func_002C42C0(popup, D_003E7720);
        func_002BB498(*(s32 *)(context + 0x118), *(s32 *)(context + 0x60), 0, 1);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B7228);

void func_002B7588(s32 arg0) {
    s32 temp_v0;

    for (temp_v0 = **(s32 **)(arg0 + 0x28c); temp_v0 < 3; temp_v0 = temp_v0 + 1) {
    }
}

s64 func_002B75C8(s32 callback) {
    s32 context = func_00101958();
    u8 *menu = *(u8 **)(context + 0xaa48);
    u32 label;
    func_002AAE80(callback);
    func_002B7588(context);
    func_002AA740(0x14);
    func_002BB0E8(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    label = *(u32 *)(*(s32 *)(*(s32 *)(*(s32 *)(menu + 0x24 + *(s32 *)(menu + 0x2c) * 4) + 0x18) + 0x1c) + 0x60);
    func_002B7228(context);
    if (label != 0 && label != 0xffff) {
        label = (u16)label;
        drawSelectionLabel(label);
        func_002B6898(label, *(s32 *)(context + 0x64), *(s32 *)(context + 0xc8));
    }
    func_002AA7A0(2, *(s32 *)(context + 0x60));
    return menuSetHandler(context, 1, callback);
}

s64 func_002B76B0(s32 callback) {
    s32 context = func_00101958();
    return menuSetHandler(context, 2, callback);
}

typedef struct MapPacket {
    u32 type;
    u32 value;
    u32 unk_08;
    u32 items[11];
    s32 count;
} MapPacket;

void func_002B76E8(u32 value, u32 *values, s32 count, MapPacket *packet) {
    s32 index = 0;
    packet->value = value;
    packet->type = 4;
    packet->count = count;
    if (count > 0) {
        do {
            packet->items[index] = values[index];
            index++;
        } while (index < count);
    }
}

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

void func_002B7790(u32 first, u32 second, u32 *menu) {
    menu[2] = first;
    menu[15] = second;
}

void func_002B77A0(s32 arg0) {
    func_003059E0(*(u32 *)(arg0 + 8), *(u32 *)(arg0 + 0x1c),
                                *(u32 *)(arg0 + 0x3c), 0, 4);
}

void func_002B77D0(u8 *effect) {
    func_002B76E8(0, D_003E7828, 0xb, (MapPacket *)effect);
    func_002B7740((s32)D_003E7858, (s32)effect);
    *(u32 *)(effect + 8) = func_00304030("/camp/spr/n_min/", D_003E7818[0], 0);
    *(u32 *)(effect + 0x3c) = loadMappedEffectResource("/camp/mot/", D_003E7820[0]);
    func_002B77A0((s32)effect);
}

void func_002B7850(u8 *effect) {
    func_002B76E8(0, D_003E7828, 0xb, (MapPacket *)effect);
    func_002B7740((s32)D_003E7858, (s32)effect);
    requestEffectResourceByMode("/camp/spr/n_min/", D_003E7818[0], 0, (u32 *)(effect + 8));
    requestMappedEffectResource("/camp/mot/", D_003E7820[0], (u32 *)(effect + 0x3c));
}

u32 func_002B78C8(u32 *menu) {
    if (menu[2] == 0) {
        return 0;
    }
    if (menu[15] == 0) {
        return 0;
    }
    func_002B77A0(menu);
    return 1;
}

void func_002B7908(u8 *ctx) {
    u32 i;
    for (i = 0; i < 1; i++) {
        func_003054E8(*(u32 *)(ctx + 8 + i * 4));
    }
    destroyPackedEffectBatch(*(u32 *)(ctx + 0x3c));
}

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

u32 *func_002B8158(u32 owner, u32 callback, s32 count) {
    u32 *node = (u32 *)func_00328E18(0x40);
    node[2] = owner;
    node[3] = callback;
    node[10] = count * 8;
    node[15] = 0x100;
    node[6] = 0;
    node[4] = 0;
    node[9] = 0;
    node[7] = 0;
    return node;
}

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

s32 func_002B8350(u32 *menu) {
    u8 *node = (u8 *)menu[6];
    s32 index = 0;
    if (node != 0) {
        s32 count = menu[3];
        do {
            if (index >= count) {
                return 0;
            }
            if (node == (u8 *)menu[5]) {
                return 1;
            }
            node = *(u8 **)(node + 0x58);
            index++;
        } while (node != 0);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B83A0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B86E8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B8860);

void *menuWalkNodeList(s32 arg0, void *arg1) {
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

void func_002B8988(u32 *entry) {
    func_002B88F0(entry[8] - 1, entry);
}

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

void func_002B9010(u8 *menu) {
    u8 *node = *(u8 **)(menu + 0x10);
    if (node != NULL) {
        do {
            s32 timer = *(s32 *)(node + 0x50);
            s32 reduced = timer - 0x10;
            if (timer > 0) {
                *(s32 *)(node + 0x50) = reduced;
                timer = reduced;
            }
            if (timer < 0) {
                *(s32 *)(node + 0x50) = 0;
            }
            node = *(u8 **)(node + 0x58);
        } while (node != NULL);
    }
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9058);

extern u32 func_00309138(u32 color, u32 previous, s32 blend);

u32 func_002B9138(u32 backup, u8 *node) {
    u32 flags = *(u32 *)(node + 0x48);
    u32 color = 0x89bdc940;
    if (!(flags & 1)) {
        color = (flags & 4) ? 0xbbefab80 : 0x89bdc980;
    }
    return func_00309138(color, backup, *(s32 *)(node + 0x50));
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9188);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9218);

void menuCallInitWide(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_002B9218(arg0, arg1, arg2, 0, 0, 0x100, 0, arg3, arg4);
}

INCLUDE_ASM(const s32, "game/code_002B0278", createWindowContainer);

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

void func_002B95A0(s32 menu, u32 first, u32 second) {
    func_002B9570(menu, first, second, 0, 0, 0, 0, 0, 0);
}

void func_002B95D0(u32 first, u32 *menu, u32 second, u32 third, u32 fourth) {
    menu[7] = first;
    menu[8] = second;
    menu[9] = third;
    menu[10] = fourth;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B95E8);

void func_002B9678(u8 *menu, u32 first, u32 second, u32 third, u32 fourth) {
    MenuListDefaults defaults = D_0042AF00;
    *(u32 **)(menu + 0x90) =
        func_002B9918(first, second, third, fourth, (u32)&defaults, 3);
}

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

u8 *advanceMenuListSelection(u8 *menu, s32 arg1) {
    u8 *item = func_002B8A50(*(s32 *)(menu + 0x18), arg1, 0);
    if (item != NULL) {
        item[0x54] = 0;
        func_002BA268(menu + 0x58);
    }
    return item;
}

u8 *reverseMenuListSelection(u8 *menu, s32 arg1) {
    u8 *item = func_002B8BA8(*(s32 *)(menu + 0x18), arg1, 0);
    if (item != NULL) {
        item[0x54] = 0;
        func_002BA268(menu + 0x58);
    }
    return item;
}

void func_002B97D8(u32 arg0) {
    advanceMenuListSelection(arg0, 0);
}

void func_002B97F0(u32 arg0) {
    reverseMenuListSelection(arg0, 0);
}

void func_002B9808(s32 arg0) {
    func_002B8F98(*(u32 *)(arg0 + 0x18));
}

void func_002B9820(s32 arg0) {
    func_002B8FB8(*(u32 *)(arg0 + 0x18));
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9838);

u32 *func_002B9918(u32 first, u32 second, u32 third,
                    u32 fourth, u32 fifth, u32 sixth) {
    u32 handle = func_003292A8(0x18);
    u32 *resource = (u32 *)func_003298F8(handle);
    memset(resource, 0, 0x18);
    resource[0] = handle;
    func_002B9838(resource, first, second, third, fourth, fifth, sixth);
    return resource;
}

void func_002B99D8(u32 *menu) {
    u32 i = 0;
    do {
        func_003054E8(menu[i + 3]);
        i++;
    } while (i < 3);
    func_003297C8(menu[0]);
}

void func_002B9A38(void) {
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9A40);

void func_002B9BB0(s32 arg0, s32 arg1, u32 arg2, s32 arg3, u32 arg4) {
    func_002B9A40(arg0 - 0xf0, arg1 - 8, arg2, *(u32 *)(arg3 + 0x94),
                                *(u32 *)(arg3 + 0x18), *(u32 *)(arg3 + 0x90), arg4);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002B9BE0);

void func_002B9CD8(u32 x, u32 y, u32 flags, u8 *entry, u32 option) {
    u8 *data = *(u8 **)(entry + 0x18);
    func_002B9BE0(x, y, flags, entry, *(u32 *)(data + 0xc), option);
}

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

INCLUDE_ASM(const s32, "game/code_002B0278", releaseResourceList);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BA378);

INCLUDE_RODATA(const s32, "game/code_002B0278", D_0042AF48);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BA518);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BA660);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BA738);

void func_002BA7A8(u32 a0, u32 a1, u32 a2, u32 a3, u32 a4, u32 a5, u32 a6) {
    func_002BA738(a0, a1, a2, a3, a4, a5, a6);
}

void func_002BA7C0(u32 a0, u32 a1, u32 a2, s32 arg3, s32 arg4) {
    func_002BA7A8(a0, a1, a2, 0x100, arg3, 0, arg4);
}

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

void allocateMenuListEntries(s32 *list) {
    u32 i;
    for (i = 0; i < 4; i++) {
        list[i + 1] = func_00328E18(0x18);
    }
}

extern void func_00328E48();

void freeMenuListEntries(s32 *list) {
    s32 *entries = list + 1;
    u32 i = 0;
    do {
        func_00328E48(*entries++);
        i++;
    } while (i < 4);
}

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

void func_002BAE08(s32 image, s32 *list, s32 option) {
    u32 i;
    for (i = 0; i < (u32)list[0]; i++) {
        s32 *entry = (s32 *)list[i + 1];
        func_002B9EA0(entry[2], entry[3], image, entry[4], option);
    }
}

void updateMenuFade(s32 *list) {
    u32 i;
    for (i = 0; i < 4; i++) {
        s32 *entry = (s32 *)list[i + 1];
        if (entry[5] != 0) {
            entry[5] -= 0x40;
        } else {
            func_002BAD20(list, i);
        }
    }
}

void func_002BAF10(u8 *menu) {
    memset(menu, 0, 0x98);
    *(u32 *)(menu + 0xb4) = 0;
    *(u32 *)(menu + 0xb8) = 0x200;
    *(u32 *)(menu + 0xbc) = 0;
    *(u32 *)(menu + 0xc0) = 0;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BAF50);

void func_002BB0D0(u8 *menu) {
    *(u32 *)(menu + 0xbc) = 0;
    *(u32 *)(menu + 0xb8) = 0x200;
    *(u32 *)(menu + 0xc0) = 0;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BB0E8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BB290);

void func_002BB320(menu)
    u32 *menu;
{
    u32 *handles = menu + 15;
    u32 i = 0;
    do {
        destroyPackedEffectBatch(*handles++);
        i++;
    } while (i < 3);
}

u8 *func_002BB370(u32 owner) {
    u8 *menu = (u8 *)func_00328D68(0x48);
    memset(menu, 0, 0x48);
    *(u32 *)(menu + 8) = 0;
    *(u32 *)(menu + 0xc) = 0;
    func_00307388(menu + 0x14, owner, 0x40);
    func_00307388(menu + 0x1c, owner, 0x41);
    func_00307388(menu + 0x24, owner, 0x44);
    func_00307388(menu + 0x2c, 0, 0);
    func_00307388(menu + 0x34, 0, 0);
    func_002BB290(menu);
    return menu;
}

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
        configureEffectWithDefaultSetting(temp_v0, *(u32 *)(arg0 + 0x38), *(u32 *)(arg0 + 0x44), 0, 10, 2);
        return;
    }
}

void func_002BB498(u32 *menu, u32 model, u32 value, u32 color) {
    func_002BB440(menu);
    menu[1] = color;
    menu[13] = model;
    menu[14] = value;
    func_003059E0(model, value, menu[15], 0, 3);
}

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

void func_002BB998(u8 *menu, s32 index, u32 unused, u32 preserve) {
    u8 *entry = menu + index * 0x2138 + 0x78;
    *(u32 *)(entry + 0x64) = 0;
    *(u32 *)(entry + 0xb4) = 0;
    if (preserve == 0) {
        *(u32 *)(entry + 0x60) = 0x100;
        *(u32 *)(entry + 0xb0) = 0x100;
    }
}

void func_002BB9C8(u32 *arg0, u32 arg1) {
    *arg0 = arg1;
}

typedef struct MenuPageParams {
    u8 unk0[0x64];
    s32 field64;
    s32 field68;
    s32 field6C;
    s32 field70;
} MenuPageParams;

void func_002BB9D0(MenuPageParams *page, s32 mode) {
    switch (mode) {
    case 0:
        page->field68 = 0;
        page->field64 = 0;
        page->field6C = 0x40;
        page->field70 = 0x100;
        break;
    case 1:
        page->field68 = 1;
        page->field64 = 0x100;
        page->field6C = 0;
        page->field70 = 0x1000;
        break;
    default:
        page->field68 = 0;
        page->field64 = 0x100;
        page->field6C = 0;
        page->field70 = 0;
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BBA38);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BBE78);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BBF38);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BBFC8);

void func_002BC078(s32 index, u8 *menu, u32 first, u32 second) {
    u8 **slot = (u8 **)(menu + index * 0x2138 + 0x154);
    if (*slot != NULL) {
        (*slot)[0x74] = first;
        (*slot)[0x75] = second;
    }
}

void clearMenuEntries(u8 *menu) {
    u8 *entry = menu + 0x154;
    u32 i = 0;
    func_002BD2E0(menu);
    do {
        if (*(u32 *)entry != 0) {
            func_002BBE78(*(u32 *)entry);
            *(u32 *)entry = 0;
        }
        i++;
        entry += 0x2138;
    } while (i < 5);
    *(u32 *)menu &= ~0x80;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BC120);

void func_002BC258(u32 *menu) {
    u32 i = 0;
    do {
        func_003054E8(menu[i + 3]);
        i++;
    } while (i < 3);
    func_00328E48(menu);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BC2B8);

void func_002BC3C8(s32 arg0, s32 arg1, u32 arg2) {
    u32 temp_v0;

    temp_v0 = func_002BC120(arg2);
    *(u32 *)(arg0 * 0x2138 + arg1 + 0x158) = temp_v0;
}

void func_002BC410(u8 *menu) {
    u8 *slot = menu + 0x158;
    u32 i = 0;
    do {
        u32 resource = *(u32 *)slot;
        i++;
        if (resource != 0) {
            func_002BC258((u32 *)resource);
            *(u32 *)slot = 0;
        }
        slot += 0x2138;
    } while (i < 5);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BC460);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BC498);

void func_002BC580(u8 *menu) {
    u32 i = 0;
    do {
        func_002C21F8((s32)(menu + 0x94));
        func_002C21F8((s32)(menu + 0xe4));
        menu += 0x2138;
        i++;
    } while (i < 5);
}

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

void registerMenuResourceHandles(s32 destination, s32 *source) {
    u32 i;
    for (i = 0; i < 5; i++) {
        func_00304EE0(source[i]);
        *(s32 *)(destination + 0x64 + 4 * i) = source[i];
    }
}

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

typedef struct MenuSlotWindow {
    u8 unk0[0xC0];
    u32 fieldC0;
    u8 unkC4[0x110 - 0xC4];
    u32 field110;
    u8 unk114[0x2138 - 0x114];
} MenuSlotWindow;

typedef struct MenuWindowSet {
    u32 flags;
    u8 unk4[0x14];
    MenuSlotWindow slots[5];
    u8 unkA630[0xA698 - 0xA630];
    s32 selected;
} MenuWindowSet;

void func_002BCD28(MenuWindowSet *set) {
    if (set->selected >= 0) {
        MenuSlotWindow *slots = set->slots;
        slots[set->selected].fieldC0 = 0x100;
        slots[set->selected].field110 = 0x100;
        set->selected = -1;
    }
    set->flags &= ~0x200;
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BCD90);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BCE50);

void shutdownMenuContext(u8 *ctx) {
    u8 *slot = ctx + 0x78;
    u32 i;
    for (i = 0; i < 5; i++, slot += 0x2138) {
        func_002BCE50(slot);
    }
    func_002BC580(ctx);
    func_002BCCB0(ctx);
}

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BCFC8);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BD090);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BD1D0);

INCLUDE_ASM(const s32, "game/code_002B0278", func_002BD2E0);

void func_002BD358(u8 *menu) {
    u8 *kind = menu + 8;
    u8 *flags = menu + 0xc;
    u32 i;
    for (i = 0; i < 5; i++) {
        s32 offset = 0x70 + i * 0x2138;
        if (*(u32 *)(kind + offset) == 2) {
            *(u32 *)(flags + offset) |= 1;
        }
    }
}

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

void func_002BD3E0(s32 kind, u8 *ctx) {
    func_002B88F0(0, *(s32 *)(ctx + kind * 4 + 0xa690));
    if (kind == 0) {
        *(u32 *)ctx &= ~2;
        *(u32 *)ctx &= ~4;
        *(u32 *)ctx &= ~8;
        *(u32 *)ctx &= ~0x10;
        *(u32 *)ctx &= ~0x20;
    } else {
        *(u32 *)ctx &= ~2;
        *(u32 *)ctx &= ~8;
        *(u32 *)ctx &= ~0x10;
        *(u32 *)ctx &= ~0x20;
    }
}

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

