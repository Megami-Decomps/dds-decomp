#include "common.h"

extern s32 D_00437C9C;

extern s32 func_002C4BA8(u16);

extern u64 func_0011D360(u64, s32);

extern s64 func_002C4038(s32, s32 *, u64, u64);

extern s32 func_002C6CE8(void);

extern u32 func_002B9FF8(u32);

extern u32 func_00304998(u32);

extern s32 D_00435E20;

extern s32 D_00435E1C;

extern void func_002C42B0(s32, s32);

extern u8 D_00457EBC[];

extern u32 D_00457EB8[];

extern u32 D_00457ED0[];

extern f32 D_0037F5E0[];

extern void func_002C6A20(void);

extern u32 D_00457EB4[];

extern u32 D_00457EB0[];

extern u8 D_003E7940[];

extern u8 D_003E7950[];

extern u32 D_00457EE0[];

extern u32 D_00457F1C[];

extern u32 D_00457F18[];

extern s32 func_00328D68(u32);

extern void func_002C2920(s32, s32, s32);

extern s8 D_003E7928[];

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BE628);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BE6E8);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BE730);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BED10);

void func_002BEE38(u32 *arg0) {
    arg0[3] = 0x18;
    arg0[4] = 5;
    *arg0 = 0;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BEE50);

void func_002BEF90(u32 *entry) {
    entry[0] = 0;
    entry[1] = 0;
    entry[2] = 0;
    entry[3] = 0;
    entry[4] = 0;
    entry[5] = 0;
    entry[6] = 0;
}

void func_002BEFB0(s32 arg0, s32 arg1) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = 1;
    temp_v0 = arg1 * 0x2138 + arg0 + 0x168;
    do {
        temp_v1 = temp_v1 - 1;
        func_002BEF90(temp_v0);
        temp_v0 = temp_v0 + 0x1024;
    } while (-1 < temp_v1);
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BF000);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BF238);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BF478);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BF660);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B0D0);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BF830);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BFEA0);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C0330);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C03B0);

void func_002C04C0(s32 arg0) {
    if (*(s32 *)(arg0 + 4) < 0x100) {
        *(s32 *)(arg0 + 4) = *(s32 *)(arg0 + 4) + 8;
    }
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C04E0);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C0630);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C0718);

INCLUDE_ASM(const s32, "game/code_002BE628", createPanelState);

void func_002C07A0(u32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 0x88);
    if (temp_v0 != 0) {
        releaseResourceList(temp_v0);
    }
    func_00328E48(arg0);
}

void func_002C07D8(s32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4) {
    *(u32 *)(arg0 + 0x18) = arg1;
    *(u32 *)(arg0 + 0x1c) = arg2;
    func_00307388(arg0 + 0x20, arg3, arg4);
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C0800);

void func_002C08B0(s32 arg0) {
    u32 temp_v0;

    temp_v0 = func_002B9FF8(5);
    *(u32 *)(arg0 + 0x88) = temp_v0;
}

void func_002C08E0(s32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4) {
    *(u32 *)(arg0 + 0x68) = arg1;
    *(u32 *)(arg0 + 0x6c) = arg2;
    func_00307388(arg0 + 0x60, arg3, arg4);
}

void func_002C0908(s32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4) {
    *(u32 *)(arg0 + 0x78) = arg1;
    *(u32 *)(arg0 + 0x7c) = arg2;
    func_00307388(arg0 + 0x70, arg3, 0);
    *(u32 *)(arg0 + 0x80) = arg4;
}

void func_002C0950(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C0958);

extern s32 func_002C2680(void);

extern void func_002C26D8(s32, s32, s32, s32, s32);

extern void func_002C0D00(s32);

s32 func_002C0B80(s32 owner, s32 texture, s32 mode) {
    s32 group = func_00328D68(0x2C);
    s32 *slot = (s32 *)(group + 0x10);
    s32 index;
    for (index = 0; index < 5; index++) {
        s32 entry = func_002C2680();
        func_002C26D8(entry, owner, texture, mode, index);
        *slot++ = entry;
    }
    func_002C0D00(group);
    *(s32 *)(group + 0xC) = texture;
    *(s32 *)(group + 0x28) = 0x100;
    return group;
}

INCLUDE_ASM(const s32, "game/code_002BE628", destroyPanelGroup);

INCLUDE_ASM(const s32, "game/code_002BE628", updateFiveListEntries);

void func_002C0CF8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002C0D00(s32 arg0) {
    *(u32 *)(arg0 + 0x24) = 0xffffffff;
}

u32 func_002C0D10(s32 arg0) {
    return *(u32 *)(arg0 + 0x24);
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C0D18);

void func_002C0F00(u32 *group, s32 index, s32 selected, u32 flags) {
    s32 offset = index * 4 + 0x10;
    u32 *entry = (u32 *)((u8 *)group + offset);
    func_002C2AB0(*entry, selected);
    func_002C2AC8(*entry, flags);
}

void func_002C0F48(s32 arg0, s32 arg1, u32 arg2) {
    func_002C2AA0(*(u32 *)(arg1 * 4 + arg0 + 0x10), arg2);
}

void func_002C0F70(s32 arg0, u64 arg1) {
    u32 temp_v0;
    u64 temp_v1;
    s32 temp_v2;
    u32 *puVar5;
    s32 temp_v3;

    puVar5 = (u32 *)(arg0 + 0x10);
    temp_v3 = 0;
    do {
        temp_v2 = temp_v3 + 1;
        temp_v1 = func_0011D360(arg1, temp_v3);
        temp_v0 = *puVar5;
        puVar5 = puVar5 + 1;
        func_002C2AA0(temp_v0, temp_v1);
        temp_v3 = temp_v2;
    } while (temp_v2 < 5);
}

void *createSpriteState(s32 x, s32 y, s32 z) {
    u8 *item = func_00328D68(0x20);
    memset(item, 0, 0x20);
    *(s32 *)(item + 0x10) = x;
    *(s32 *)(item + 0x14) = y;
    *(s32 *)(item + 0x18) = z;
    *(s32 *)(item + 0x1C) = 0x100;
    return item;
}

void func_002C1050(void) {
    func_00328E48();
}

void func_002C1068(u32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u16 arg4, u32 arg5, u32 arg6) {
    s32 temp_v0;

    temp_v0 = func_002C4BA8(arg4);
    func_00306CD0(arg0, arg1, arg2, arg3, 1, arg5, temp_v0 + 8, arg6);
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C10F0);

void *func_002C1660(s32 x, s32 y, s32 z) {
    u8 *item = func_00328D68(0x20);
    memset(item, 0, 0x20);
    *(s32 *)(item + 0x10) = x;
    *(s32 *)(item + 0x14) = y;
    *(s32 *)(item + 0x18) = z;
    *(s32 *)(item + 0x1C) = 0x100;
    return item;
}

void func_002C16D8(void) {
    func_00328E48();
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C16F0);

void func_002C1B58(s32 arg0, s32 arg1) {
    *(s32 *)(arg0 + 4) = arg1;
    *(s32 *)arg0 = 0;
    *(s32 *)(arg0 + 8) = 0;
}

void func_002C1B68(u32 *arg0, u32 arg1) {
    *arg0 = arg1;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C1B70);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C1C20);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C1CD0);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C1D10);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C1DC8);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C1E48);

void func_002C1F68(s32 arg0) {
    u32 temp_v0;

    temp_v0 = func_00304998(3);
    *(u32 *)(arg0 + 0x38) = temp_v0;
    temp_v0 = func_00304998(3);
    *(u32 *)(arg0 + 0x3c) = temp_v0;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C1FA0);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C1FF0);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B118);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B130);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B140);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B150);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B160);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B180);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B220);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B270);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C2128);

void func_002C21F8(u32 *group) {
    u32 *entry = group + 7;
    u32 index = 0;
    do {
        func_003054E8(*entry++);
        index++;
    } while (index < 7);
    func_002C1FA0(group);
}

u32 func_002C2258(s32 useDefault, s32 index, s32 option) {
    u32 color = 0xA09DC380;
    if (!useDefault) {
        switch (func_002C5CD0(index, option)) {
        case 1:
            color = 0xB4A06480;
            break;
        case 2:
            color = 0x89554780;
            break;
        }
    }
    return color;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C22D0);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C24E8);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C2680);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C26D8);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C2920);

void func_002C2A88(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

void func_002C2A90(u32 *entry, u32 left, u32 right) {
    entry[6] = left;
    entry[7] = right;
}

void func_002C2AA0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void func_002C2AA8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x28) = arg1;
}

void func_002C2AB0(s32 arg0, s32 arg1) {
    if (*(s32 *)(arg0 + 0x20) != arg1) {
        *(u32 *)(arg0 + 0xa4) = 0x100;
    }
    *(s32 *)(arg0 + 0x20) = arg1;
}

void func_002C2AC8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x1c) = arg1;
}

void func_002C2AD0(void) {
    func_00328E48();
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C2AE8);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C3010);

void func_002C32A0(s32 arg0, s32 arg1, s32 arg2) {
    *(s32 *)(arg0 + 0x10) = arg1;
    *(s32 *)(arg0 + 0x14) = arg2;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C32B0);

void func_002C3390(void) {
    func_00328E48();
}

void func_002C33A8(u32 *entry, u32 first, u32 second, u32 third, u32 fourth) {
    entry[6] = first;
    entry[7] = second;
    entry[9] = third;
    entry[10] = fourth;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C33C0);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C3E08);

void func_002C3E58(u32 arg0) {
    memset(arg0, 0, 0x4c);
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C3E78);

void func_002C3FC8(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)arg0;
    while (temp_v0 != 0) {
        func_002C3E78(1, 0, arg0, arg1);
        temp_v0 = *(s32 *)arg0;
    }
}

s32 func_002C4020(s32 *arg0) {
    return (*arg0 & 0x200000) > 0;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C4038);

u8 func_002C42A0(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 0x44) == arg1;
}

void func_002C42B0(s32 arg0, s32 arg1) {
    u16 temp_v0;

    temp_v0 = *(u16 *)arg1;
    *(s32 *)arg0 = arg1;
    *(s32 *)arg1 = temp_v0;
}

void func_002C42C0(s32 arg0, s32 arg1) {
    u16 temp_v0;

    temp_v0 = *(u16 *)arg1;
    *(s32 *)arg0 = arg1;
    *(s32 *)arg1 = temp_v0 | 0x20000;
}

void func_002C42D8(s32 arg0, s32 arg1) {
    u16 temp_v0;

    temp_v0 = *(u16 *)arg1;
    *(s32 *)arg0 = arg1;
    *(s32 *)arg1 = temp_v0 | 0x60000;
}

void func_002C42F0(s32 arg0, u32 arg1) {
    if (*(s32 *)(arg0 + 0x44) != 0) {
        func_002C42C0(arg1, *(s32 *)(arg0 + 0x44));
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C4328);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C4430);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C44E8);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C47C8);

void func_002C48C8(u32 arg0, u32 arg1) {
    func_002C47C8(*(u32 *)((s32)arg0 + 0x90), arg0, arg1);
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C48F0);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C49F0);

void func_002C4B40(u32 arg0) {
    func_002C49F0(arg0, 0);
}

INCLUDE_ASM(const s32, "game/code_002BE628", findMatchingPartyEntryIndex);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C4BA8);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C4C28);

s32 getIndexedNonzeroEffect(s32 index) {
    s32 count = 0;
    s32 i;
    s8 *entry = D_003E7928;
    for (i = 0; i < 6; i++, entry += 3) {
        s32 value = *entry;
        if (value != 0) count++;
        if (index == count - 1) return value;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C4CA8);

u16 func_002C4D78(s32 arg0) {
    s32 temp_v0 = (arg0 & 0xffff) * 56 + D_00435E20;

    if (*(u8 *)(temp_v0 + 0x24) != 2) {
        return 0;
    }
    return *(u16 *)(temp_v0 + 0x26);
}

u8 func_002C4DB0(u32 arg0) {
    return *(u8 *)((arg0 & 0xffff) * 0x38 + D_00435E20 + 3);
}

u16 getAdjustedEntryValue(s32 id, s32 object) {
    s32 entry = (id & 0xFFFF) * 0x38 + D_00435E20;
    u16 base = *(u16 *)(entry + 4);
    u16 addition = *(u16 *)(entry + 6);
    if (func_002C4DB0(id & 0xFFFF) == 1) {
        base = addition + *(u16 *)(object + 8) * base / 100;
    }
    return base;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C4E58);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C4EB8);

extern s32 func_002C4EB8(u16, s32);

s32 func_002C4F50(s32 context, u16 id) {
    if (func_002C4EB8(id, context) == 0) return -1;
    if ((*(u8 *)(D_00435E20 + id * 56 + 1) & 1) == 0) return 1;
    if (id < 0x220) return 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C4FB8);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C5030);

s32 func_002C50C0(u16 ability) {
    u8 value;
    if (ability == 0) {
        return 1;
    }
    value = *(u8 *)(D_00435E20 + ability * 56 + 8);
    switch (value) {
    case 0:
        return 1;
    case 1:
        return 2;
    case 2:
        return 3;
    }
    return 0;
}

void func_002C5128(u16 arg0) {
    func_00119548(arg0);
}

u32 func_002C5140(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C5148);

u8 func_002C5338(u32 arg0) {
    return *(s8 *)((arg0 & 0xffff) * 2 + D_00435E1C) == '\x01';
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C5358);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C5428);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C5480);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C5498);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C54B0);

extern u8 *D_00435DD0;

typedef struct MenuPanelEntry {
    u16 flags;
    u8 unknown02[0x12];
    u16 marker;
    u8 unknown16[0x19C];
    u16 currentId;
    u8 unknown1B4[0x10];
} MenuPanelEntry;

extern u16 func_002C5580(s32);

u32 func_002C54C8(s32 id) {
    u32 value;
    s32 index;
    if (id < 0xA0) return 0;
    if (id >= 0xBF) return 0;
    value = *(u8 *)(id + (s32)D_00435DD0 + 0x1340);
    for (index = 0; index < 5; index++) {
        MenuPanelEntry *entry = (MenuPanelEntry *)(D_00435DD0 + 0xA60) + index;
        if (id == func_002C5580((s32)entry)) {
            value++;
        }
    }
    return value;
}

u32 func_002C5570(s32 arg0, u16 arg1) {
    *(u16 *)(arg0 + 0x52) = arg1;
    return 1;
}

u16 func_002C5580(s32 arg0) {
    return *(u16 *)(arg0 + 0x52);
}

u32 func_002C5588(u32 arg0, u32 arg1) {
    *(s16 *)((s32)arg0 + 0x1b2) = (s16)arg1;
    func_002C5678(arg1);
    func_003144E8(arg0);
    return 1;
}

u16 func_002C55C0(s32 arg0) {
    return *(u16 *)(arg0 + 0x1b2);
}

MenuPanelEntry *func_002C55C8(u32 id) {
    s32 index;
    MenuPanelEntry *entry = (MenuPanelEntry *)(D_00435DD0 + 0xA60);
    for (index = 0; index < 5; index++, entry++) {
        if ((entry->flags & 1) && id == entry->currentId) {
            return entry;
        }
    }
    return 0;
}

MenuPanelEntry *func_002C5618(u32 id) {
    s32 index;
    MenuPanelEntry *entry = (MenuPanelEntry *)(D_00435DD0 + 0x1CA10);
    for (index = 0; index < 16; index++, entry++) {
        if (entry->marker != 0 && (entry->flags & 1) && id == entry->currentId) {
            return entry;
        }
    }
    return 0;
}

void func_002C5678(s32 index) {
    s32 offset = 0x1E730 + index;
    if (index != 0) {
        D_00435DD0[offset] |= 1;
    }
}

void func_002C56A8(s32 index) {
    s32 offset = 0x1E730 + index;
    if (index != 0) {
        D_00435DD0[offset] &= ~1;
    }
}

s32 func_002C56D8(s32 index) {
    if (index == 0) {
        return 1;
    }
    return *(u8 *)(index + (s32)D_00435DD0 + 0x1E730) & 1;
}

s32 func_002C5700(void) {
    s32 index;
    for (index = 0xC0; index < 0x100; index++) {
        if (!func_002C56D8(index) && *(u8 *)(index + (s32)D_00435DD0 + 0x1340) != 0) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C5758);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B2E8);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B300);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B350);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B370);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B3D0);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B440);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B4C0);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C57D8);

s32 func_002C59B0(void) {
    if (func_0011B260(1)) return 1;
    if (func_0011B260(2)) return 2;
    if (func_0011B260(5)) return 5;
    return func_0011B260(8) ? 8 : 1;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C5A28);

s32 func_002C5C28(s32 entry) {
    u16 flags = *(u16 *)(entry + 0xe);
    if (flags & 0x400) return 0;
    if (flags & 0x100) return 1;
    if (flags & 0x80) return 2;
    if (flags & 0x40) return 3;
    if (flags & 0x10) return 4;
    return -1;
}

INCLUDE_ASM(const s32, "game/code_002BE628", getMatchingPartyEntryMask);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C5CD0);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C5D20);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C5DE0);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C5EA8);

s32 func_002C5F78(u32 *left, u32 *right) {
    u8 *a = (u8 *)*left;
    u8 *b = (u8 *)*right;
    s32 leftRatio = (*(u16 *)(a + 6) << 10) / *(u16 *)(a + 8);
    s32 rightRatio = (*(u16 *)(b + 6) << 10) / *(u16 *)(b + 8);
    if (*(u16 *)a & 2) {
        if (!(*(u16 *)b & 2)) {
            return -1;
        }
    } else if (*(u16 *)b & 2) {
        return 1;
    }
    if (rightRatio < leftRatio) return 1;
    if (leftRatio < rightRatio) return -1;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C6008);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C6348);

u8 func_002C6480(void) {
    return D_00437C9C != 0;
}

void func_002C6490(s32 arg0) {
    if (arg0 == 0) {
        D_00457EBC[0] = 0;
    } else {
        D_00457EBC[0] = 1;
    }
}

u32 func_002C64B8(void) {
    return D_00457EB8[0];
}

u32 func_002C64C8(void) {
    return D_00457ED0[0];
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C64D8);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C6578);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C6610);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C6670);

extern u8 *D_00457EC0[];

u8 func_002C66D0(u16 index) {
    return D_00457EC0[0][index * 60 + 1];
}

f32 func_002C66F8(u16 index) {
    return *(f32 *)(D_00457EC0[0] + index * 60 + 0x18);
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C6720);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C6758);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C6790);

void func_002C6958(f32 value) {
    D_0037F5E0[5] = 2048.0f;
    D_0037F5E0[4] = value + 2048.0f;
    func_002C6790();
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C6988);

void func_002C6A20(void) {
    u32 *state = D_00457EB0;
    if (state[0] == 1) {
        return;
    }
    state[27] = -1;
    if (state[26] != 0) {
        func_002C76F0();
    }
    if (state[2] != 0) {
        func_002322E8(state[2]);
        state[2] = 0;
        state[28] = 0;
    }
}

void menuResetWorkFloats(void) {
    func_002C6A20();
    D_0037F5E0[4] = 2048.0f;
    D_0037F5E0[5] = 2048.0f;
}

void func_002C6AC0(s32 arg0, s32 arg1, s32 arg2) {
    D_00457EB4[0] = func_00231B80();
}

INCLUDE_ASM(const s32, "game/code_002BE628", menuForwardTableByte);

u32 func_002C6B28(u32 *arg0) {
    return *arg0 & 1;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C6B38);

void func_002C6BB8(void) {
    D_00457EB0[5] &= ~1;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C6BD8);

void func_002C6CC8(u16 arg0, u32 arg1) {
    func_002C6BD8(arg0, 0xffffffffffffffff, arg1);
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C6CE8);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C6E20);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C6EE0);

void menuResetWorkPair(void) {
    *(s32 *)(D_003E7950 + 0) = 0;
    *(s32 *)(D_003E7950 + 4) = 0;
    *(f32 *)(D_003E7950 + 8) = -400.0f;
    *(s32 *)(D_003E7940 + 0) = 0;
    *(s32 *)(D_003E7940 + 4) = 0;
    *(s32 *)(D_003E7940 + 8) = 0;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C6F98);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C70D0);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C7168);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C72E0);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C73C0);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C7428);

extern u32 D_00457EC8[];

void func_002C7508(u32 menuId, f32 x, f32 y) {
    D_00457EC8[6] = 1;
    D_00457EC8[7] = menuId;
    D_00457EC8[8] = (s32)x;
    D_00457EC8[9] = (s32)y;
}

void func_002C7530(void) {
    D_00457EE0[0] = 4;
}

s32 func_002C7540(void) {
    s32 temp_v0 = D_00457EE0[0];

    if ((temp_v0 == 0) || (temp_v0 == 3)) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C7568);

void func_002C76A8(void) {
    if (D_00457EB0[26] != 0) {
        func_002C76F0();
    }
    D_00457EB0[26] = func_00203DA8(D_00457EB0[2], 0x30);
}

void func_002C76F0(void) {
    u32 *temp_v0 = D_00457EB0;
    u32 temp_v1 = temp_v0[26];

    if (temp_v1 == 0) {
        return;
    }
    func_00203E90(temp_v1);
    temp_v0[26] = 0;
}

void func_002C7730(u32 arg0) {
    D_00457F1C[0] = arg0;
}

s32 func_002C7740(void) {
    return D_00457F18[0] != 0;
}

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B500);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B518);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B528);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B538);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B548);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B558);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C7750);

u32 func_002C79B8(void) {
    func_002C7750();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C79D8);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B5A8);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B5B8);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C7A60);

void func_002C7C00(void) {
    func_0023A9A8();
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C7C18);

INCLUDE_ASM(const s32, "game/code_002BE628", createBattleStageTestTask);

s32 destroyBattleStageTask(object)
    s32 object;
{
    if (*(u8 *)(object + 1) == 6) {
        s32 resource = *(s32 *)(object + 0xC);
        if (resource != 0) {
            func_0033FD30(resource);
        }
        func_00346AF8(object + 0x30);
        func_00328E48(*(void **)(object + 8));
        func_00328E48((void *)object);
        return 0;
    }
    return 1;
}

void func_002C7CE8(void) {
    destroyBattleStageTask();
}

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B610);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437C58);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437C60);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437C68);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437C70);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437C78);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437C80);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437C88);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437C90);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437C98);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437C9C);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437CA0);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437CA4);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437CA8);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437CB0);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437CB8);

INCLUDE_SDATA(const s32, "game/code_002BE628", D_00437CBC);

