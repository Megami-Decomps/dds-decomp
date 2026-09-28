#include "common.h"

extern s32 mdlFlagTest(u32);

extern s64 func_0026C768(void);

extern s32 func_00268BE0(s32);

extern s32 kwlnFadeIsActive(void);

extern s32 func_002B86E8(u32);

extern void func_002686F0(s32);

extern s8 D_00437858;

extern s64 func_002C4038(s32, s32 *, u64, u64);

extern s32 func_00101958();
extern s32 func_002C3E08(s32, s32, s32, s32);
extern void func_002C1B70(s32, s32);
extern void func_002C1B68(s32, s32);
extern s32 func_002A9820(s32, s32);
extern void func_002B2818(s32);
extern s32 D_00435DD0;
extern s32 func_00266F70(s32, s32);
extern s32 func_003292A8(s32);
extern s32 func_003298F8(s32);
extern s32 func_00303D00(s32);
extern void func_002C4430(s32);
extern void func_002A9640(s32, s32);
extern s32 func_002C32B0(void);
extern void func_002C33A8(s32, s32, s32, s32, s32);
extern void func_002C0630(s32, s32, s32, s32, s32, s32);
extern void func_002C16F0(s32, s32, s32, s32, s32, s32, s32);
extern s32 func_00267EA0(s8, s32);
extern s32 menuWalkNodeList(s32, s32);
extern void kwlnTaskDestroyWithHierarchyByName(const char *, s32);
extern const char D_00424F00[];
extern const char D_00424F10[];
extern const char D_00424F20[];
extern s32 D_0043785C;
extern s32 func_002B8158(s32, s32, s32, s32);
extern s32 func_002B82A0(s32, s32);
extern void func_00267500(void);
extern u8 D_00437870[];
extern s32 func_00328D68(s32);
extern s32 func_002BC460(u16, u16);
extern void func_002C2128(s32, s32, s32, s32, s32, s32);
extern void destroyPackedEffectBatch(s32);

void func_002665B0(s32 arg0) {
    destroyPackedEffectBatch(*(u32 *)(arg0 + 0x3c));
}

u8 func_002665C8(void) {
    s64 temp_v0;

    temp_v0 = mdlFlagTest(0x31);
    return temp_v0 != 0;
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_002665E8);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00266808);

void func_002668C0(s32 arg0) {
    if (*(s32 *)(arg0 + 0x3f4) == 0) {
        func_00304EE0(*(u32 *)(arg0 + 100));
        func_00304EE0(*(u32 *)(arg0 + 0x68));
        func_00304EE0(*(u32 *)(arg0 + 0x6c));
        func_00304EE0(*(u32 *)(arg0 + 0x70));
        return;
    }
    func_00304EE0(*(u32 *)(arg0 + 100));
    func_00304EE0(*(u32 *)(arg0 + 0x68));
}

void func_00266928(s32 arg0, u32 arg1) {
    if (*(s32 *)(arg0 + 0x3f4) == 0) {
        func_00304FB0(*(u32 *)(arg0 + 100));
        func_00304FB0(*(u32 *)(arg0 + 0x68), arg1);
        func_00304FB0(*(u32 *)(arg0 + 0x6c), arg1);
        func_00304FB0(*(u32 *)(arg0 + 0x70), arg1);
        return;
    }
    func_00304FB0(*(u32 *)(arg0 + 100));
    func_00304FB0(*(u32 *)(arg0 + 0x68), arg1);
}

void func_002669B0(u32 arg0) {
    func_00266928(arg0, 0);
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_002669C8);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00266A48);

void func_00266AF8(s32 object) {
    s32 node = *(s32 *)(object + 0x10);
    if (node != 0) {
        s32 base = D_00435DD0;
        do {
            u32 limit = *(u32 *)(base + 0x3c);
            if (limit < *(u32 *)(node + 0x64)) {
                *(u32 *)(node + 0x48) |= 1;
            } else {
                *(u32 *)(node + 0x48) &= ~1u;
            }
            node = *(s32 *)(node + 0x58);
        } while (node != 0);
    }
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_00266B48);

INCLUDE_RODATA(const s32, "game/code_002665B0", D_00424E60);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00266C08);

s32 func_00266F70(s32 resource, s32 context) {
    s32 panel = func_00328D68(0xa0);
    func_002C2128(panel, 0, 0, 0x1e,
        func_002BC460(*(u16 *)(resource + 6), *(u16 *)(resource + 8)),
        *(s32 *)(context + 0xec));
    func_002C2128(panel + 0x50, 1, 0, 0x1e,
        func_002BC460(*(u16 *)(resource + 0xa), *(u16 *)(resource + 0xc)),
        *(s32 *)(context + 0xec));
    return panel;
}

void func_00267008(s32 arg0) {
    if (arg0 != 0) {
        func_002C21F8();
        func_002C21F8((s32)arg0 + 0x50);
        func_00328E48(arg0);
        return;
    }
}

void func_00267050(s32 object) {
    s32 node = *(s32 *)(*(s32 *)(object + 0x7c) + 0x10);
    while (node != 0) {
        s32 id = *(s32 *)(node + 0x60);
        *(s32 *)(node + 0x70) =
            func_00266F70(D_00435DD0 + id * 0x1c4 + 0xa60, object);
        node = *(s32 *)(node + 0x58);
    }
}

void func_002670C8(s32 arg0) {
    s32 temp_v0;

    for (temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0x7c) + 0x10); temp_v0 != 0; temp_v0 = *(s32 *)(temp_v0 + 0x58)
            ) {
        func_00267008(*(u32 *)(temp_v0 + 0x70));
    }
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_00267110);

void func_002671E8(s32 arg0) {
    func_002B81C8(*(u32 *)(arg0 + 0x7c));
}

void func_00267200(s32 arg0) {
    func_00267008(*(u32 *)(*(s32 *)(*(s32 *)(arg0 + 0x7c) + 0x1c) + 0x70));
    func_002B86E8(*(u32 *)(arg0 + 0x7c));
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_00267238);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00267358);

INCLUDE_ASM(const s32, "game/code_002665B0", func_002673B8);

u32 func_002674F8(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_00267500);

s32 func_002675C8(s32 *items, s32 count, s32 excluded, s32 callback) {
    s32 list = func_002B8158(0, count, 0x16, callback);
    s32 i;
    *(s32 *)(list + 0x30) = callback;
    *(s32 *)(list + 0x2c) = (s32)func_00267500;
    *(s32 *)(list + 0x3c) = 0;
    for (i = 0; i < count; i++) {
        if (i != excluded) {
            s32 node = func_002B82A0(list, (s32)D_00437870);
            *(s32 *)(node + 0x60) = items[i];
        }
    }
    return list;
}

void func_00267680(s32 object) {
    s32 state = *(s32 *)(object + 0x84);
    if (state < 2) {
        if (state < 0) {
            return;
        }
        if (*(s32 *)(*(s32 *)(object + 0x80) + 0x20) == 0) {
            s32 selected = menuWalkNodeList(2 - func_002674F8(),
                                              *(s32 *)(object + 0x78));
            *(u32 *)(selected + 0x48) |= 1;
        }
    }
}

void func_002676F0(s32 object) {
    s32 state = *(s32 *)(object + 0x84);
    s32 selectedIndex;
    if (state != 0) {
        if (state != 2) {
            return;
        }
        selectedIndex = 0;
    } else {
        selectedIndex = 3 - func_002674F8();
    }
    if (*(s32 *)(*(s32 *)(object + 0x7c) + 0x20) == 0) {
        s32 node = menuWalkNodeList(selectedIndex, *(s32 *)(object + 0x78));
        *(u32 *)(node + 0x48) |= 1;
    }
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_00267768);

INCLUDE_ASM(const s32, "game/code_002665B0", func_002678C8);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00267938);

INCLUDE_ASM(const s32, "game/code_002665B0", func_002679E8);

s32 func_00267A00(void) {
    s32 heap = func_003292A8(0xa82c);
    s32 object = func_003298F8(heap);
    memset((void *)object, 0, 0xa82c);
    *(s32 *)object = heap;
    *(s32 *)(object + 4) = func_00303D00(1);
    func_002C4430(object + 0x70);
    func_002A9640(*(s32 *)(object + 4), object + 8);
    *(s32 *)(object + 0x6c) = 1;
    return object;
}

void func_00267A80(u32 *arg0) {
    func_002B2860(arg0 + 2);
    func_002A9788(arg0 + 2);
    func_00303D58(arg0[1]);
    func_003297C8(*arg0);
}

s32 func_00267AC8(s32 object) {
    s32 state = *(s32 *)(object + 0x6c);
    if (state == 0) {
        return 1;
    }
    if (state == 2) {
        return 0;
    }
    if (func_002A9820(*(s32 *)(object + 4), object + 8) == 0) {
        return 1;
    }
    func_002B2818(object + 8);
    *(s32 *)(object + 0x6c) = 2;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_00267B40);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00267C48);

void func_00267D30(s32 unused, s32 object) {
    if (*(s32 *)(object + 0xa828) == 0) {
        s32 effect = func_002C32B0();
        *(s32 *)(object + 0xa828) = effect;
        func_002C33A8(effect, *(s32 *)(object + 8), *(s32 *)(object + 0x14), 1, 2);
    }
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_00267DA0);

s32 func_00267DE0(s32 arg0, s32 arg1, s32 arg2, s32 object) {
    return func_002C3E08(arg0, arg1, arg2, *(s32 *)(object + 0xa828));
}

s32 func_00267E00(s32 resource, s32 object, s32 mode) {
    if (*(s32 *)(object + 0x6c) != 2) {
        return 0;
    }
    *(s32 *)(object + 0x17c) |= 0x280;
    func_002C0630(0, 0, 0, *(u8 *)(resource + 0x55), object + 0x17c, mode);
    func_002C16F0(0, 0, 0, resource, *(u8 *)(resource + 0x55),
                   *(s32 *)(object + 0xa824), mode);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_00267EA0);

s32 func_00267F68(s8 index) {
    return func_00267EA0(index, 1);
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_00267F88);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00268090);

INCLUDE_ASM(const s32, "game/code_002665B0", func_002680E0);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00268128);

INCLUDE_RODATA(const s32, "game/code_002665B0", D_00424E98);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00268178);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00268278);

s32 func_00268318(void) {
    s32 context = func_00101958() + 0x3e8;
    func_002C1B70(context, 0x53);
    if (func_0026C768() != 0) {
        func_002C1B68(context, 1);
    } else {
        func_002C1B68(context, 0);
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_002665B0", D_00424F00);

INCLUDE_RODATA(const s32, "game/code_002665B0", D_00424F10);

INCLUDE_RODATA(const s32, "game/code_002665B0", D_00424F20);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00268380);

void func_00268470(void) {
    kwlnTaskDestroyWithHierarchyByName(D_00424F00, 0);
    kwlnTaskDestroyWithHierarchyByName(D_00424F10, 0);
    kwlnTaskDestroyWithHierarchyByName(D_00424F20, 0);
    D_0043785C = 0;
}

s32 pollSceneState(void) {
    s32 state = D_00437858;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        D_00437858 = 0;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_002684F0);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00268550);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00268588);

s32 func_002685C0(void) {
    s32 temp_v0 = kwlnFadeIsActive();

    if (temp_v0 != 0) {
        return 0;
    }
    return func_0026C768() == 0;
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_002685F0);

void func_002686D0(s32 arg0) {
    u16 temp_E = *(u16 *)(arg0 + 0xe);
    u16 temp_8 = *(u16 *)(arg0 + 8);
    u16 temp_C = *(u16 *)(arg0 + 0xc);
    u16 temp_M = temp_E & 0xfa2f;

    *(u16 *)(arg0 + 6) = temp_8;
    *(u16 *)(arg0 + 0xa) = temp_C;
    *(u16 *)(arg0 + 0xe) = temp_M;
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_002686F0);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00268838);

INCLUDE_ASM(const s32, "game/code_002665B0", classifyRemainingFrames);

INCLUDE_ASM(const s32, "game/code_002665B0", func_002689D0);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00268AA0);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00268B48);

s32 func_00268BE0(s32 object) {
    switch (*(s32 *)(object + 0xe4)) {
    case 1:
        return 0x32;
    case 2:
        return 0x36;
    default:
        return 0;
    }
}

u8 func_00268C08(s32 arg0) {
    s32 temp_v0;

    temp_v0 = func_00268BE0(arg0);
    return *(u8 *)(temp_v0 * 0xa0 + *(s32 *)(*(s32 *)(arg0 + 100) + 0x18) + 0x14);
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_00268C48);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00268CC0);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00268EC8);

INCLUDE_ASM(const s32, "game/code_002665B0", func_002690A8);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00269230);

INCLUDE_RODATA(const s32, "game/code_002665B0", D_00424F40);

INCLUDE_RODATA(const s32, "game/code_002665B0", D_00424F58);

INCLUDE_RODATA(const s32, "game/code_002665B0", D_00424F88);

s32 func_00269418(s32 object) {
    switch (*(s32 *)(object + 0x20)) {
    case 2: return 0x3a;
    case 3: return 0x3b;
    case 4: return 0x3c;
    case 5: return 0x3d;
    case 6: return 0x3e;
    default: return 0x3f;
    }
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_00269478);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00269638);

INCLUDE_ASM(const s32, "game/code_002665B0", func_002698A0);

INCLUDE_RODATA(const s32, "game/code_002665B0", D_00424FC8);

INCLUDE_SDATA(const s32, "game/code_002665B0", D_00437858);

INCLUDE_SDATA(const s32, "game/code_002665B0", D_00437859);

INCLUDE_SDATA(const s32, "game/code_002665B0", D_0043785C);

INCLUDE_SDATA(const s32, "game/code_002665B0", D_00437860);

INCLUDE_SDATA(const s32, "game/code_002665B0", D_00437868);

INCLUDE_SDATA(const s32, "game/code_002665B0", D_00437870);

INCLUDE_SDATA(const s32, "game/code_002665B0", D_00437878);

