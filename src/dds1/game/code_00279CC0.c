#include "common.h"

extern void func_00284340(s32);

extern void func_002CFF98(void *);

extern u32 func_0027D4A0(u32);

extern u32 func_0027F730(u32);

extern s32 func_002CFEB8(u32);

extern u32 func_0027D148(u32, u32, u32, u32);

extern s32 func_0027B888(u32);

extern s32 func_00101A70();

INCLUDE_ASM(const s32, "game/code_00279CC0", func_00279CC0);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_00279D68);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_00279F88);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027A0A8);

void func_0027A0E0(s32 selection) {
    s32 item = func_00197E08(0xCB0, 0xA80, 0, 0, selection & 0xFFFF, 1);
    func_001954C8(item, 0xA09DC366);
    func_001958A0(item, 1, 0x53);
    func_00194920(item);
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027A140);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027A300);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027A468);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027A4D0);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027A540);

u32 func_0027A778() {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    temp_v0 = *(s32 *)(temp_v0 + 0x90c);
    func_0027C430(*(u32 *)(temp_v0 + 0x24));
    *(u32 *)(temp_v0 + 0x24) = 0;
    return 1;
}

s32 func_0027A7B0(s32 selection) {
    s32 context;
    s32 menu;
    func_00278760();
    context = func_00101A70(selection);
    menu = *(s32 *)(context + 0x90C);
    func_0027A540(selection);
    func_00280858(context + 0x15C);
    *(s32 *)(menu + 0x2C) = 0;
    return 1;
}

s32 func_0027A810(s32 selection) {
    s32 context = func_00101A70();
    func_0027A778(selection);
    func_002808A8(context + 0x15c);
    func_00278868(selection);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027A860);

void func_0027A9A8(s32 arg0) {
    func_002BF790(0x1c0, 0xa60, 0, 1, *(u32 *)(arg0 + 0x74), 0x1f, 0x53);
    func_002BF790(0x150, 0xa00, 0, 1, *(u32 *)(arg0 + 0x74), 0, 0x53);
    func_002BF790(0xbb0, 0xa00, 0, 1, *(u32 *)(arg0 + 0x74), 0, 0x53);
    func_002BF790(0x250, 0x9c0, 0, 1, *(u32 *)(arg0 + 0xe4), 0x18, 0x53);
    func_002BF790(0xce0, 0x9e0, 0, 1, *(u32 *)(arg0 + 100), 2, 0x53);
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027AA68);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027AB10);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027AC00);

void func_0027AC38(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;
    s32 temp_v2;

    temp_v1 = func_002BD908(2);
    *(u32 *)(arg0 + 0x18) = temp_v1;
    temp_v2 = func_002BD908(2);
    temp_v0 = *(s32 *)(temp_v2 + 8);
    *(s32 *)(arg0 + 0x1c) = temp_v2;
    func_002BDE18(temp_v0 + 0x28, *(u32 *)(arg0 + 0x14), 0, 0xc);
    func_002BDE18(*(s32 *)(*(s32 *)(arg0 + 0x1c) + 8) + 0x94, *(u32 *)(arg0 + 0x14), 1, 0xc)
    ;
    func_002BE128(*(u32 *)(arg0 + 0x10), 0, 0, *(u32 *)(*(s32 *)(arg0 + 0x1c) + 8));
    func_002BE128(*(u32 *)(arg0 + 0x10), 1, 0, *(s32 *)(*(s32 *)(arg0 + 0x1c) + 8) + 0x6c);
    func_002BE128(*(u32 *)(arg0 + 0x10), 2, 0, *(s32 *)(*(s32 *)(arg0 + 0x1c) + 8) + 0x6c);
    func_002BE128(*(u32 *)(arg0 + 0x10), 3, 0, *(u32 *)(*(s32 *)(arg0 + 0x1c) + 8));
    func_002BE128(*(u32 *)(arg0 + 0x10), 4, 0, *(u32 *)(*(s32 *)(arg0 + 0x1c) + 8));
    func_002BDE18(*(s32 *)(*(s32 *)(arg0 + 0x18) + 8) + 0x28, *(u32 *)(arg0 + 0x14), 2, 0xd)
    ;
    func_002BE0C0(*(u32 *)(arg0 + 4), 0, *(u32 *)(*(s32 *)(arg0 + 0x18) + 8));
    func_002BE258(*(u32 *)(arg0 + 8), 0, *(u32 *)(arg0 + 0x14), 3, 4);
    func_002BE258(*(u32 *)(arg0 + 0xc), 0, *(u32 *)(arg0 + 0x14), 4, 4);
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027AD80);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027AEA8);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027AF28);

void func_0027B010(s32 *assets) {
    u32 i;
    for (i = 0; i < 4; i++) {
        func_002BDD60(assets[i]);
    }
    func_002BDD60(assets[4]);
    func_002BD2F8(assets[5]);
    func_002BD988(assets[6]);
    func_002BD988(assets[7]);
}

void func_0027B088(s32 arg0, u32 arg1) {
    func_002C14E0(arg1);
    func_002BF790(0xffffffffffffff90, 0xa0, 0, 0x61, *(u32 *)(arg0 + 0x10), 0, arg1);
    func_002BF790(0xfffffffffffffb90, 0x808, 0, 0x61, *(u32 *)(arg0 + 0x10), 1, arg1);
    func_002BF790(0x1050, 0xfffffffffffffc18, 0, 0x61, *(u32 *)(arg0 + 0x10), 2, arg1);
    func_002BF790(0x10b0, 0x3c0, 0, 0x61, *(u32 *)(arg0 + 0x10), 3, arg1);
    func_002BF790(0x1300, 0xb70, 0, 0x61, *(u32 *)(arg0 + 0x10), 4, arg1);
    func_002C1548(0, arg1);
    func_002BF970(*(u32 *)(arg0 + 0x10), 0);
    func_002BF970(*(u32 *)(arg0 + 0x10), 1);
    func_002BF790(0, 0, 0, 0x60, *(u32 *)(arg0 + 4), 0, arg1);
    func_002BF970(*(u32 *)(arg0 + 4), 0);
    func_002C1588(arg1);
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027B1B0);

void func_0027B268(s32 *assets, s32 option) {
    func_002C0950(0x30000);
    func_002C0DD8(0, 0, 0, 0x2000, 0xE00, 0x80808080, option);
    func_002BF790(0, 0, 0, 0, assets[0], 0, option);
    func_0027B1B0(assets, option);
    func_0027B088((s32)assets, option);
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027B2F8);

u32 func_0027B368(u32 arg0) {
    s64 temp_v0;

    do {
        temp_v0 = func_0027B888(arg0);
    } while (temp_v0 != 0);
    func_002CFF98(arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027B3A8);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027B440);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027B4F0);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027B540);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027B888);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027BA00);

void *func_0027BA48(s32 arg0, void *arg1) {
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

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027BA90);

void func_0027BB08(u32 arg0) {
    func_0027BA90(0, arg0);
}

void func_0027BB28(s32 *arg0) {
    func_0027BA90(arg0[8] - 1, arg0);
}

s32 func_0027BB48(s32 *arg0) {
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

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027BB80);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027BBF0);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027BD48);

void func_0027BE90(u32 arg0) {
    func_0027BBF0(arg0, 0, 0);
}

void func_0027BEB0(u32 arg0) {
    func_0027BD48(arg0, 0, 0);
}

void func_0027BED0(u32 *arg0) {
    *arg0 &= ~1;
    *arg0 &= ~2;
}

u32 func_0027BEF0(u32 *arg0) {
    return *arg0 & 2;
}

s32 func_0027BF00(s32 arg0) {
    return *(s32 *)(arg0 + 0x28) * *(s32 *)(arg0 + 0xc);
}

void func_0027BF10(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x10);
    if (temp_v0 != 0) {
        *(u32 *)(temp_v0 + 0x50) = 0;
        while (temp_v0 = *(s32 *)(temp_v0 + 0x58), temp_v0 != 0) {
            *(u32 *)(temp_v0 + 0x50) = 0;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027BF48);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027BF90);

void func_0027C070(s32 arg0, s32 arg1) {
    func_002C1630((*(s32 *)(arg1 + 0x48) & 1) ? 0x89BDC940 : 0x89BDC980, arg0, *(s32 *)(arg1 + 0x50));
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027C0B0);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027C140);

void func_0027C370(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_0027C140(arg0, arg1, arg2, 0, 0, 0x100, 0, arg3, arg4);
}

s32 func_0027C3A0(s32 id, s32 width, s32 height, s32 left, s32 right) {
    s32 item = func_002CFF68(0x8C);
    s32 child;
    *(s32 *)(item + 8) = width;
    *(s32 *)(item + 0xC) = height;
    *(s32 *)item = id;
    child = func_0027B2F8(id, left, right);
    *(s32 *)(item + 0x88) = 0;
    *(s32 *)(item + 0x14) = child;
    return item;
}

void func_0027C430(u32 arg0) {
    s32 temp_v0;

    func_0027B368(*(u32 *)((s32)arg0 + 0x14));
    temp_v0 = *(s32 *)((s32)arg0 + 0x84);
    if (temp_v0 != 0) {
        func_0027D1E8(temp_v0);
    }
    func_002CFF98(arg0);
}

void func_0027C470(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x3c) = arg1;
}

void func_0027C478(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x88) = arg1;
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027C480);

void func_0027C558(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_0027C480(arg0, arg1, arg2, arg3, arg4, arg4);
}

void func_0027C570(s32 arg0, s32 *arg1, s32 arg2, s32 arg3, s32 arg4) {
    arg1[6] = arg0;
    arg1[7] = arg2;
    arg1[9] = arg3 + 3;
    arg1[8] = arg3;
    arg1[10] = arg4;
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027C590);

void func_0027C620(s32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4) {
    u32 temp_v0;

    temp_v0 = func_0027D148(arg1, arg2, arg3, arg4);
    *(u32 *)(arg0 + 0x84) = temp_v0;
}

void func_0027C658(s32 arg0) {
    *(u32 *)(arg0 + 4) = *(u32 *)(arg0 + 4) & 0xfffffffb;
}

void func_0027C670(s32 arg0) {
    func_0027B440(*(u32 *)(arg0 + 0x14));
}

void func_0027C688(s32 arg0) {
    func_0027B540(*(u32 *)(arg0 + 0x14));
}

void func_0027C6A0(s32 arg0) {
    func_0027B888(*(u32 *)(arg0 + 0x14));
}

s32 func_0027C6B8(s32 window, s32 direction) {
    s32 item = func_0027BBF0(*(s32 *)(window + 0x14), direction, 0);
    if (item != 0) {
        *(u8 *)(item + 0x54) = 0;
        func_0027D740(window + 0x4c);
    }
    return item;
}

s32 func_0027C708(s32 window, s32 direction) {
    s32 item = func_0027BD48(*(s32 *)(window + 0x14), direction, 0);
    if (item != 0) {
        *(u8 *)(item + 0x54) = 0;
        func_0027D740(window + 0x4c);
    }
    return item;
}

void func_0027C758(u32 arg0) {
    func_0027C6B8(arg0, 0);
}

void func_0027C770(u32 arg0) {
    func_0027C708(arg0, 0);
}

void func_0027C788(s32 arg0) {
    func_0027BED0(*(u32 *)(arg0 + 0x14));
}

void func_0027C7A0(s32 arg0) {
    func_0027BEF0(*(u32 *)(arg0 + 0x14));
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027C7B8);

void func_0027CA78(void) {
    func_0027C7B8();
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027CA90);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027CCD0);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027CDD0);

void func_0027CEE8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0x14) + 0x20);
    if (0 < temp_v0) {
        do {
            temp_v0 = temp_v0 - 1;
        } while (temp_v0 != 0);
    }
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027CF28);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027D148);

void func_0027D1E8(s32 *object) {
    u32 i;
    for (i = 0; i < 7; i++) {
        func_002BDD60(object[i + 3]);
    }
    func_002D0918(object[0]);
}

void func_0027D248(s32 arg0, u32 arg1) {
    func_002BE378(*(u32 *)(arg0 + 0x10), 0, arg1, 0, 0x14, 0xc);
    func_002BE378(*(u32 *)(arg0 + 0x14), 0, arg1, 1, 10, 0xc);
    func_002BE378(*(u32 *)(arg0 + 0x18), 0, arg1, 2, 0, 0xc);
    func_002BE378(*(u32 *)(arg0 + 0x1c), 0, arg1, 2, 0, 0xc);
    func_002BE378(*(u32 *)(arg0 + 0x20), 0, arg1, 1, 10, 0xc);
    func_002BE378(*(u32 *)(arg0 + 0x24), 0, arg1, 0, 0x14, 0xc);
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027D318);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027D4A0);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027D740);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027D7E0);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027D850);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027DA80);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027DBD0);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027DCE8);

void func_0027DD58(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    func_0027DCE8(arg0, arg1, arg2, arg3, arg4, 0, arg5);
}

void func_0027DD78(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_0027DD58(arg0, arg1, arg2, 0x100, arg3, arg4);
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027DDA0);

void func_0027DE60(s32 arg0) {
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

void func_0027DE98(s32 arg0) {
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

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027DED0);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027DF48);

s32 func_0027DFF8(s32 *arg0, s32 *arg1) {
    u32 temp_A = *(u32 *)(*arg0 + 0x60);
    u32 temp_B = *(u32 *)(*arg1 + 0x60);

    if (temp_B < temp_A) {
        return -1;
    }
    return temp_A < temp_B;
}

s32 func_0027E020(s32 *arg0, s32 *arg1) {
    u32 temp_A = *(u32 *)(*arg0 + 0x60);
    u32 temp_B = *(u32 *)(*arg1 + 0x60);

    if (temp_B < temp_A) {
        return 1;
    }
    return (temp_A < temp_B) ? -1 : 0;
}

s32 func_0027E050(s32 *arg0, s32 *arg1) {
    u32 temp_A = *(u32 *)(*arg0 + 0x64);
    u32 temp_B = *(u32 *)(*arg1 + 0x64);

    if (temp_B < temp_A) {
        return -1;
    }
    return temp_A < temp_B;
}

s32 func_0027E078(s32 *arg0, s32 *arg1) {
    u32 temp_A = *(u32 *)(*arg0 + 0x64);
    u32 temp_B = *(u32 *)(*arg1 + 0x64);

    if (temp_B < temp_A) {
        return 1;
    }
    return (temp_A < temp_B) ? -1 : 0;
}

s32 func_0027E0A8(s32 *arg0, s32 *arg1) {
    u32 temp_A = *(u32 *)(*arg0 + 0x68);
    u32 temp_B = *(u32 *)(*arg1 + 0x68);

    if (temp_B < temp_A) {
        return -1;
    }
    return temp_A < temp_B;
}

s32 func_0027E0D0(s32 *arg0, s32 *arg1) {
    u32 temp_A = *(u32 *)(*arg0 + 0x68);
    u32 temp_B = *(u32 *)(*arg1 + 0x68);

    if (temp_B < temp_A) {
        return 1;
    }
    return (temp_A < temp_B) ? -1 : 0;
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027E100);

void func_0027E228(s32 *list) {
    u32 i;
    for (i = 0; i < 4; i++) {
        list[i + 1] = func_002CFF68(0x18);
    }
}

void func_0027E270(s32 *list) {
    u32 i;
    s32 *entry = list + 1;
    for (i = 0; i < 4; i++, entry++) {
        func_002CFF98((void *)*entry);
    }
}

void func_0027E2C0(s32 arg0, s32 arg1, s32 *arg2) {
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

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027E2F0);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027E3D8);

void func_0027E468(s32 *list) {
    u32 i;
    for (i = 0; i < 4; i++) {
        s32 *entry = (s32 *)list[i + 1];
        if (entry[5] != 0) {
            entry[5] -= 0x40;
        } else {
            func_0027E2F0(list, i);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027E4E0);

void func_0027E570(s32 *list) {
    u32 i;
    for (i = 0; i < 3; i++) {
        func_002BD2F8(list[i + 16]);
    }
}

s32 func_0027E5C0(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v2;

    temp_v0 = func_002CFEB8(0x4c);
    memset(temp_v0, 0, 0x4c);
    *(u32 *)(temp_v0 + 8) = 0;
    *(u32 *)(temp_v0 + 0xc) = 0;
    temp_v2 = 1;
    func_002BFB98(temp_v0 + 0x10, arg0, arg1);
    func_002BFB98(temp_v0 + 0x18, arg2, arg3);
    temp_v1 = temp_v0;
    do {
        temp_v2 = temp_v2 - 1;
        func_002BFB98(temp_v1 + 0x20, 0, 0);
        func_002BFB98(temp_v1 + 0x30, 0, 0);
        temp_v1 = temp_v1 + 8;
    } while (-1 < temp_v2);
    func_0027E4E0(temp_v0);
    return temp_v0;
}

void func_0027E690(u32 arg0) {
    func_0027E570((s32 *)arg0);
    func_002CFF98(arg0);
}

void func_0027E6B8(s32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    func_002BFB98(arg0 + 0x10);
    *(u32 *)(arg0 + 4) = arg3;
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027E6F0);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027E790);

u8 func_0027E850(s32 arg0) {
    return *(s32 *)(arg0 + 0x20) != 0;
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027E860);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027E8D8);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027EAF0);

void func_0027EC40(s32 *object) {
    u32 i;
    for (i = 0; i < 2; i++) {
        func_002BDD60(object[i + 3]);
    }
    for (i = 0; i < 6; i++) {
        func_002BDD60(object[i + 5]);
    }
    func_002BDD60(object[12]);
    func_002BDD60(object[11]);
    func_002CFF98(object);
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027ECD8);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027EF18);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027EFD0);

void func_0027F050(s32 arg0, u32 arg1, u32 arg2, u32 arg3, s32 arg4
                                    ) {
    u32 temp_v0;

    temp_v0 = func_002BDBC0(arg1, arg2, 1);
    *(u32 *)(arg0 + 0xe8) = temp_v0;
    temp_v0 = func_002BDBC0(arg1, arg3, 1);
    *(u32 *)(arg0 + 0xec) = temp_v0;
    if (-1 < arg4) {
        temp_v0 = func_002BDBC0(arg1, arg4, 1);
        *(u32 *)(arg0 + 0xf0) = temp_v0;
    }
}

void func_0027F0D8(s32 arg0) {
    u32 temp_v0;
    s32 *piVar2;
    u32 *puVar3;
    u32 *puVar4;
    u32 temp_v1;

    piVar2 = (s32 *)(arg0 + 0x168);
    puVar3 = (u32 *)(arg0 + 0x7c);
    puVar4 = (u32 *)(arg0 + 0x164);
    temp_v1 = 0;
    do {
        if (piVar2[-2] != 0) {
            func_002BDD60(piVar2[-2]);
        }
        if (piVar2[-1] != 0) {
            func_002BDD60(piVar2[-1]);
        }
        if (*piVar2 != 0) {
            func_002BDD60(*piVar2);
        }
        temp_v0 = *puVar3;
        temp_v1 = temp_v1 + 1;
        piVar2[-2] = 0;
        *puVar4 = 0;
        *puVar3 = temp_v0 & 0xffffff7f;
        puVar3 = puVar3 + 0x4d;
        *piVar2 = 0;
        piVar2 = piVar2 + 0x4d;
        puVar4 = puVar4 + 0x4d;
    } while (temp_v1 < 5);
}

void func_0027F198(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_v0 = arg0 + arg1 * 0x134 + 0x78;

    *(s32 *)(temp_v0 + 0x6C) = 0;
    *(s32 *)(temp_v0 + 0xC0) = 0;
    if (arg3 != 0) {
        return;
    }
    *(s32 *)(temp_v0 + 0x68) = 0x100;
    *(s32 *)(temp_v0 + 0xBC) = 0x100;
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027F1C8);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027F230);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027F4B0);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027F588);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027F638);

void func_0027F6B8(s32 *menu) {
    u32 i;
    s32 *entry = menu + 0x56;
    func_002807E8();
    for (i = 0; i < 5; i++, entry += 0x4D) {
        if (*entry != 0) {
            func_0027F4B0(*entry);
            *entry = 0;
        }
    }
    *menu &= ~0x100;
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027F730);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027F838);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027F898);

void func_0027F9D8(s32 arg0, s32 arg1, u32 arg2) {
    u32 temp_v0;

    temp_v0 = func_0027F730(arg2);
    *(u32 *)(arg0 * 0x134 + arg1 + 0x15c) = temp_v0;
}

void func_0027FA20(s32 window) {
    u32 i;
    for (i = 0; i < 5; i++) {
        s32 *item = (s32 *)(window + 0x15c + i * 0x134);
        if (*item != 0) {
            func_0027F838(*item);
            *item = 0;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027FA70);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027FAA8);

void func_0027FB90(s32 window) {
    u32 i;
    for (i = 0; i < 5; i++, window += 0x134) {
        func_00284340(window + 0x94);
        func_00284340(window + 0xe8);
    }
}

void func_0027FBE0(s32 arg0, u32 *arg1) {
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

void func_0027FC10(s32 arg0, u32 *arg1) {
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

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027FC40);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027FCA0);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027FF60);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_00280048);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_00280170);

void func_00280228(s32 arg0) {
    func_0027B368(*(u32 *)(arg0 + 0x67c));
    func_0027B368(*(u32 *)(arg0 + 0x680));
}

void func_00280258(s32 arg0, s32 arg1) {
    func_00280228(arg0);
    func_00280170(arg0, arg1);
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_00280290);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_002802E0);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_002803A0);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_00280488);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_002804F0);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_002805B0);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_002806E8);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_002807E8);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_00280858);

void func_002808A8(s32 arg0) {
    u32 *puVar1;
    u32 temp_v0;

    puVar1 = (u32 *)(arg0 + 0x7c);
    temp_v0 = 0;
    do {
        temp_v0 = temp_v0 + 1;
        *puVar1 = *puVar1 & 0xfffffffe;
        puVar1 = puVar1 + 0x4d;
    } while (temp_v0 < 5);
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_002808E0);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_00280978);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_00280A90);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_00280BC0);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_00280D98);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_00280E08);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_00281108);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_002811D0);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_002812E8);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_002814D0);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_002815F0);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_00281688);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_00281780);

void func_00281898(u32 arg0) {
    memset(arg0, 0, 0x20);
}







INCLUDE_RODATA(const s32, "game/code_00279CC0", D_003B2330);

INCLUDE_RODATA(const s32, "game/code_00279CC0", D_003B2348);

INCLUDE_RODATA(const s32, "game/code_00279CC0", D_003B2358);

INCLUDE_RODATA(const s32, "game/code_00279CC0", D_003B2368);

INCLUDE_RODATA(const s32, "game/code_00279CC0", D_003B2380);

INCLUDE_RODATA(const s32, "game/code_00279CC0", D_003B23A0);

INCLUDE_RODATA(const s32, "game/code_00279CC0", D_003B23B0);

INCLUDE_SDATA(const s32, "game/code_00279CC0", D_003BC718);

INCLUDE_SDATA(const s32, "game/code_00279CC0", D_003BC720);

INCLUDE_SDATA(const s32, "game/code_00279CC0", D_003BC728);

INCLUDE_SDATA(const s32, "game/code_00279CC0", D_003BC730);

INCLUDE_SDATA(const s32, "game/code_00279CC0", D_003BC738);

INCLUDE_SDATA(const s32, "game/code_00279CC0", D_003BC740);


INCLUDE_SDATA(const s32, "game/code_00279CC0", D_003BC748);

