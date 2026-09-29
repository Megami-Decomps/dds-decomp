#include "common.h"

extern s32 D_00437C9C;

extern s32 mnuLookupRangeEntry(u16);

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

typedef struct MenuSelectionEntry {
    u8 pad0[0xE];
    u16 flags;
} MenuSelectionEntry;

extern u16 D_003E7900[];

extern u16 D_003E7902[];

extern s8 D_003E792A[];

typedef struct RangeEntry {
    u8 pad00;
    u8 flags;
    u8 pad02;
    u8 kind;
    u16 value;
    u16 addition;
    u8 pad08[0x1C];
    u8 secondaryKind; /* 0x24 */
    u8 pad25;
    u16 secondaryValue; /* 0x26 */
    u8 pad28[0x10];
} RangeEntry;

typedef struct AffinityRow {
    s32 affinity[4];
} AffinityRow;

extern u32 D_00435E24;

extern void mdlAddEntryFlaggedEx(s32, s32, s32, f32, f32);

extern void mdlAddEntryPlainEx(s32, s32, s32, f32, f32);

extern void func_002C7568(void);

extern s32 mdlGetNodeRefHalf(u32 node, s32 index);

typedef struct StageCameraTarget {
    u8 pad[8];
    void *unk8;
} StageCameraTarget;

extern StageCameraTarget *func_0023B018(f32 *, f32 *);

extern u8 D_00437CB0[];

extern void func_002C0330(s32, s32, s32, s32, s32, s32);

extern void func_002C2AD0();

extern void func_00328E48();

extern u32 effMiscRand(s32);

extern u8 *D_00435DD0;

extern u16 D_003E78D8[];

extern u8 *D_00457EC0[];

extern u32 D_00457EC8[];

extern void func_00340DC8(f32, f32, f32);

extern char D_0042B610[];

extern void func_002C7A60();

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

void mnuClearSpriteRecord(u32 *entry) {
    entry[0] = 0;
    entry[1] = 0;
    entry[2] = 0;
    entry[3] = 0;
    entry[4] = 0;
    entry[5] = 0;
    entry[6] = 0;
}

void mnuClearPairedSpriteRecords(s32 menu, s32 index) {
    s32 record;
    s32 remaining;

    remaining = 1;
    record = index * 0x2138 + menu + 0x168;
    do {
        remaining = remaining - 1;
        mnuClearSpriteRecord(record);
        record = record + 0x1024;
    } while (-1 < remaining);
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BF000);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BF238);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BF478);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BF660);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B0D0);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BF830);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002BFEA0);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C0330);

typedef struct MenuSpacing {
    s32 step;
    s32 gap;
    s32 tail;
} MenuSpacing;

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B118);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B130);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B140);

typedef struct MenuListNode {
    u8 pad00[0xC];
    s32 overrideValue;
} MenuListNode;

typedef struct MenuListPanel {
    u8 pad00[4];
    u32 layoutFlags; /* 0x04: bit 0x40 forces the regular list spacing */
    u8 pad08[0xD4];
    MenuListNode *node;
} MenuListPanel;

typedef struct MenuListState {
    u32 flags;
    u8 pad04[4];
    s32 *entryCount; /* 0x08 */
    u8 pad0C[0xA68C];
    s32 selectedPanel; /* 0xA698 */
    s32 scrollOffset; /* 0xA69C */
} MenuListState;

void mnuCalcListEntryOffset(s32 *out, MenuListState *menu, s32 index) {
    MenuSpacing spacing = {0x310, 0x370, 0x190};
    s32 count = *menu->entryCount;
    s32 mode;

    if (!(((MenuListPanel *)((s32)menu + index * 0x2138 + 0x78))->layoutFlags & 0x40)) {
        mode = index < count ? 1 : 2;
    } else {
        mode = 1;
    }
    if (menu->selectedPanel >= 0) {
        out[0] = 0xC80;
        out[1] = 0x20;
    } else {
        out[0] = 0xF00;
        out[1] = 0x28;
    }
    switch (mode) {
    case 1:
        out[1] += index * spacing.step;
        break;
    case 2:
        out[1] += (count - 1) * spacing.step;
        if (index >= count) {
            out[1] += spacing.gap;
        }
        out[1] += (index - count) * spacing.tail;
        break;
    }
}

void func_002C04C0(s32 arg0) {
    if (*(s32 *)(arg0 + 4) < 0x100) {
        *(s32 *)(arg0 + 4) = *(s32 *)(arg0 + 4) + 8;
    }
}

void func_002C04E0(s32 x, s32 y, s32 z, s32 overrideValue, MenuListState *menu, s32 param) {
    s32 offset[2];
    MenuListPanel *panel = (MenuListPanel *)((s32)menu + menu->selectedPanel * 0x2138 + 0x78);
    MenuListNode *node;

    mnuCalcListEntryOffset(offset, menu, 0);
    node = panel->node;
    if (node != NULL) {
        node->overrideValue = overrideValue;
    }
    x += menu->scrollOffset * 0x10;
    menu->scrollOffset = (s32)((f32)menu->scrollOffset / 1.19999993f);
    if (menu->flags & 0x80) {
        func_002C0330(x + offset[0], y + offset[1], z, menu, menu->selectedPanel, param);
    } else {
        func_002C0330(x + offset[0], y + offset[1], z, menu, menu->selectedPanel, param);
    }
    node = panel->node;
    if (node != NULL) {
        node->overrideValue = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C0630);

void func_002C0718(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    func_002C0630(arg0, arg1, arg2, 0, arg3, arg4);
}

typedef struct MenuPoint {
    s32 x;
    s32 y;
} MenuPoint;

typedef struct MenuPanelState {
    u8 pad00[0xC];
    s32 width;
    s32 height;
    u32 state; /* 0x14 */
    u32 firstValueA; /* 0x18 */
    u32 firstValueB; /* 0x1C */
    MenuPoint firstPosition; /* 0x20 */
    MenuPoint corners[5]; /* 0x28 */
    MenuPoint guideStart; /* 0x50 */
    MenuPoint guideEnd; /* 0x58 */
    MenuPoint secondPosition; /* 0x60 */
    u32 secondValueA; /* 0x68 */
    u32 secondValueB; /* 0x6C */
    MenuPoint thirdPosition; /* 0x70 */
    u32 thirdValueA; /* 0x78 */
    u32 thirdValueB; /* 0x7C */
    u32 thirdValueC; /* 0x80 */
    u8 pad84[4];
    u32 resourceHandle; /* 0x88 */
} MenuPanelState;

void *mnuCreatePanelState(s32 width, s32 height) {
    MenuPanelState *panel = (MenuPanelState *)func_00328D68(0x8C);

    memset(panel, 0, 0x8C);
    panel->width = width;
    panel->height = height;
    return panel;
}

void mnuDestroyPanelState(MenuPanelState *panel) {
    s32 resourceHandle;

    resourceHandle = panel->resourceHandle;
    if (resourceHandle != 0) {
        mnuReleaseResourceList(resourceHandle);
    }
    func_00328E48(panel);
}

void func_002C07D8(MenuPanelState *panel, u32 valueA, u32 valueB, u32 x,
                                    u32 y) {
    panel->firstValueA = valueA;
    panel->firstValueB = valueB;
    func_00307388(&panel->firstPosition, x, y);
}

void func_002C0800(MenuPanelState *panel, s32 x, s32 y, s32 guideX, s32 guideTopY, s32 guideBottomY) {
    func_00307388(&panel->guideStart, guideX, guideTopY);
    func_00307388(&panel->guideEnd, guideX, guideBottomY);
    panel->corners[0].x = x;
    panel->corners[0].y = y;
    panel->corners[1].x = x + 0xC0;
    panel->corners[1].y = y - 0x20;
    panel->corners[2].x = x - 0x50;
    panel->corners[2].y = y + 0x68;
    panel->corners[3].x = x + 0x1D0;
    panel->corners[3].y = y + 0x68;
    panel->corners[4].x = x + 0xC0;
    panel->corners[4].y = y + 0xF0;
}

void mnuInitializePanelResource(MenuPanelState *panel) {
    u32 resourceHandle;

    resourceHandle = func_002B9FF8(5);
    panel->resourceHandle = resourceHandle;
}

void func_002C08E0(MenuPanelState *panel, u32 valueA, u32 valueB, u32 x,
                                    u32 y) {
    panel->secondValueA = valueA;
    panel->secondValueB = valueB;
    func_00307388(&panel->secondPosition, x, y);
}

void func_002C0908(MenuPanelState *panel, u32 valueA, u32 valueB, u32 x,
                                    u32 additionalValue) {
    panel->thirdValueA = valueA;
    panel->thirdValueB = valueB;
    func_00307388(&panel->thirdPosition, x, 0);
    panel->thirdValueC = additionalValue;
}

void func_002C0950(MenuPanelState *panel, u32 state) {
    panel->state = state;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C0958);

extern s32 func_002C2680(void);

extern void func_002C26D8(s32, s32, s32, s32, s32);

typedef struct MenuPanelGroup {
    u8 pad00[0x0C];
    s32 texture;       /* 0x0C */
    s32 entries[5];    /* 0x10 */
    u32 selection;     /* 0x24 */
    s32 initialValue;  /* 0x28: initialized to 0x100 */
} MenuPanelGroup;

extern void func_002C0D00(MenuPanelGroup *);

s32 func_002C0B80(s32 owner, s32 texture, s32 mode) {
    MenuPanelGroup *group = (MenuPanelGroup *)func_00328D68(0x2C);
    s32 *slot = group->entries;
    s32 index;
    for (index = 0; index < 5; index++) {
        s32 entry = func_002C2680();
        func_002C26D8(entry, owner, texture, mode, index);
        *slot++ = entry;
    }
    func_002C0D00(group);
    group->texture = texture;
    group->initialValue = 0x100;
    return (s32)group;
}

void mnuDestroyPanelGroup(MenuPanelGroup *group) {
    s32 i;

    for (i = 0; i < 5; i++) {
        func_002C2AD0(group->entries[i]);
    }
    func_00328E48(group);
}

void mnuUpdateFiveListEntries(MenuPanelGroup *group, s32 data) {
    s32 i;

    for (i = 0; i < 5; i++) {
        func_002C2920(group->entries[i], data, i);
    }
}

void func_002C0CF8(MenuPanelGroup *group, u32 selection) {
    group->selection = selection;
}

void func_002C0D00(MenuPanelGroup *group) {
    group->selection = 0xffffffff;
}

u32 func_002C0D10(MenuPanelGroup *group) {
    return group->selection;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C0D18);

void mnuSetGroupSelection(MenuPanelGroup *group, s32 index, s32 selected, u32 flags) {
    /* Required to match: retain byte-offset indexing for the child slot. */
    s32 offset = index * 4 + 0x10;
    u32 *entry = (u32 *)((u8 *)group + offset);
    func_002C2AB0(*entry, selected);
    func_002C2AC8(*entry, flags);
}

void func_002C0F48(MenuPanelGroup *group, s32 index, u32 value) {
    func_002C2AA0(group->entries[index], value);
}

void func_002C0F70(MenuPanelGroup *group, u64 value) {
    u32 temp_v0;
    u64 temp_v1;
    s32 temp_v2;
    u32 *puVar5;
    s32 temp_v3;

    puVar5 = (u32 *)group->entries;
    temp_v3 = 0;
    do {
        temp_v2 = temp_v3 + 1;
        temp_v1 = func_0011D360(value, temp_v3);
        temp_v0 = *puVar5;
        puVar5 = puVar5 + 1;
        func_002C2AA0(temp_v0, temp_v1);
        temp_v3 = temp_v2;
    } while (temp_v2 < 5);
}

typedef struct MenuSpriteState {
    u8 pad00[0x10];
    s32 x;
    s32 y;
    s32 z;
    s32 initialValue; /* 0x1C: initialized to 0x100 */
} MenuSpriteState;

void *mnuCreateSpriteState(s32 x, s32 y, s32 z) {
    MenuSpriteState *item = func_00328D68(0x20);
    memset(item, 0, 0x20);
    item->x = x;
    item->y = y;
    item->z = z;
    item->initialValue = 0x100;
    return item;
}

void func_002C1050(void) {
    func_00328E48();
}

void func_002C1068(u32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u16 arg4, u32 arg5, u32 arg6) {
    s32 temp_v0;

    temp_v0 = mnuLookupRangeEntry(arg4);
    func_00306CD0(arg0, arg1, arg2, arg3, 1, arg5, temp_v0 + 8, arg6);
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C10F0);

void *func_002C1660(s32 x, s32 y, s32 z) {
    MenuSpriteState *item = func_00328D68(0x20);
    memset(item, 0, 0x20);
    item->x = x;
    item->y = y;
    item->z = z;
    item->initialValue = 0x100;
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

void func_002C1C20(u8 *object, s32 arg1, s32 arg2, s32 arg3, s32 count, s32 arg5, s32 arg6, s32 arg7) {
    s32 i;

    for (i = 0; i < count; i++) {
        func_00306CD0(object, arg1, arg2, arg3, 1, arg5, arg6, arg7);
        object += 0xA0;
    }
}

void func_002C1CD0(s32 object) {
    s32 first = *(s32 *)(object + 0x38);
    s32 second = *(s32 *)(object + 0x3C);
    s32 firstData = *(s32 *)(first + 8);
    s32 secondData = *(s32 *)(second + 8);
    s32 *pos = *(s32 **)(firstData + 0x20);

    pos[0] = 10;
    pos[1] = *(s8 *)(object + 0x19);
    pos[2] = 10;
    pos = *(s32 **)(secondData + 0x20);
    pos[1] = 5;
    pos[0] = 10;
    pos[2] = 10;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C1D10);

void func_002C1DC8(s32 object) {
    s32 *table;
    s32 value;

    func_002C1D10(object);
    func_002C1CD0(object);
    table = *(s32 **)(object + 0x14);
    value = 0;
    if (table != 0) {
        value = table[*(s8 *)(object + 0x18)];
    }
    effConfigureWithDefaultSetting(*(s32 *)(object + 0x28), 0, *(s32 *)(object + 0x38), 0, value, 0);
    *(u8 *)(object + 0x18) += 1;
    if ((s8)*(u8 *)(object + 0x18) >= 4) {
        *(u8 *)(object + 0x18) = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C1E48);

void func_002C1F68(s32 arg0) {
    u32 temp_v0;

    temp_v0 = func_00304998(3);
    *(u32 *)(arg0 + 0x38) = temp_v0;
    temp_v0 = func_00304998(3);
    *(u32 *)(arg0 + 0x3c) = temp_v0;
}

void func_002C1FA0(s32 *list) {
    u32 i;

    for (i = 0; i < 2; i++) {
        effDestroyPackedBatch(list[i + 14]);
    }
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C1FF0);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B160);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B180);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B220);

INCLUDE_RODATA(const s32, "game/code_002BE628", D_0042B270);

void func_002C2128(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 tableA[7] = {0x29, 0x23, 0x24, 0x25, 0x25, 0x2A, 0x2C};
    s32 tableB[7] = {0x29, 0x26, 0x27, 0x28, 0x28, 0x2B, 0x2C};

    if (arg1 == 1) {
        func_002C1FF0(arg0, arg1, arg2, arg3, arg4, arg5, tableB, 7);
    } else {
        func_002C1FF0(arg0, arg1, arg2, arg3, arg4, arg5, tableA, 7);
    }
}

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

s32 func_002C2680(void) {
    s32 item = func_00328D68(0xAC);

    memset(item, 0, 0xAC);
    *(s32 *)(item + 0x14) = 0x63;
    *(s32 *)(item + 0x10) = 0x8c;
    *(s32 *)(item + 0xA0) = 0x100;
    return item;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C26D8);

void func_002C2920(s32 obj, s32 param, s32 index) {
    s32 table[5] = {0, 4, 1, 2, 3};

    func_00307388(obj + 0x74, param, 7);
    func_003071D0(*(s32 *)(obj + 0x74), *(s32 *)(obj + 0x78), -0x50, -0x50, 0, 0);
    func_00307388(obj + 0x7C, param, 5);
    func_003071D0(*(s32 *)(obj + 0x7C), *(s32 *)(obj + 0x80), 0x390, -8, 0, 0);
    func_00307388(obj + 0x84, param, 6);
    func_003071D0(*(s32 *)(obj + 0x84), *(s32 *)(obj + 0x88), 0x390, -8, 0, 0);
    func_00307388(obj + 0x8C, param, 9);
    func_003071D0(*(s32 *)(obj + 0x8C), *(s32 *)(obj + 0x90), 0x5D0, 0, 0, 0);
    func_00307388(obj + 0x94, param, table[index]);
    func_003071D0(*(s32 *)(obj + 0x94), *(s32 *)(obj + 0x98), 0x130, -0x30, 0, 0);
}

void func_002C2A88(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

void mnuSetGroupPair(u32 *entry, u32 left, u32 right) {
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

u32 *func_002C32B0(s32 source) {
    u32 *item = (u32 *)func_00328D68(0x48);
    s32 first;
    u32 second;
    u32 i;

    memset(item, 0, 0x48);
    first = func_00314B78(source);
    second = func_00314BE0(source);
    func_002C32A0(item, func_00314690((u16)first), *(u32 *)second);
    for (i = 0; i < 5; i++) {
        item[11 + i] = effMiscRand(0) % 0xC0 + 0x40;
    }
    item[17] = 0x100;
    return item;
}

void func_002C3390(void) {
    func_00328E48();
}

void mnuSetGroupProperties(u32 *entry, u32 first, u32 second, u32 third, u32 fourth) {
    entry[6] = first;
    entry[7] = second;
    entry[9] = third;
    entry[10] = fourth;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C33C0);

void func_002C3E08(s32 x, s32 y, s32 z, u32 *item, s32 option) {
    s32 previous;
    s32 next;

    func_002C33C0(x, y, z, item, option);
    previous = item[16];
    next = previous + 1;
    if (previous < 0x200) {
        item[16] = next;
        if (next < 0x200) {
            return;
        }
        previous = next;
    }
    item[16] = previous - 0x200;
}

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

typedef struct PartyPanelSlot {
    s32 unk0;
    s32 index;
    s32 unk8;
    s32 pad[10];
} PartyPanelSlot;

typedef struct PartyPanel {
    s32 unk0;
    s32 unk4;
    PartyPanelSlot slots[5];
} PartyPanel;

extern void func_002C4328(u8 *entry, s32 arg1, u32 index, PartyPanel *panel);

void mnuInitPartyPanelSlots(PartyPanel *panel) {
    u32 i;
    u8 *entry;
    s32 offset = 0;

    memset(panel, 0, 0x10C);
    panel->unk0 = 0;
    panel->unk4 = 0;
    for (i = 0; i < 5; i++) {
        entry = D_00435DD0 + offset + 0xA60;
        offset += 0x1C4;
        if (*(u16 *)entry & 1) {
            func_002C4328(entry, 0, i, panel);
            panel->slots[i].index = i;
        } else {
            panel->slots[i].unk8 = -1;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C44E8);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C47C8);

void func_002C48C8(u32 arg0, u32 arg1) {
    func_002C47C8(*(u32 *)((s32)arg0 + 0x90), arg0, arg1);
}

void mnuPlayInputSound(s32 arg0, s32 buttons, s32 *state) {
    if (buttons & 0x8000) {
        sndSetSequenceVolumePan(0xD, 0x7F, 0x3F);
        return;
    }
    if (buttons & 0x4000) {
        sndSetSequenceVolumePan(0xC, 0x7F, 0x3F);
        return;
    }
    if (buttons != 0) {
        if (buttons & 1) {
            sndSetSequenceVolumePan(8, 0x7F, 0x3F);
        }
        if (buttons & 2) {
            sndSetSequenceVolumePan(0xA, 0x7F, 0x3F);
        }
        if (buttons & 0xCF0) {
            if (state != NULL) {
                if ((*state & 3) != 2) {
                    sndSetSequenceVolumePan(0, 0x7F, 0x3F);
                }
            } else {
                sndSetSequenceVolumePan(0, 0x7F, 0x3F);
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C49F0);

void func_002C4B40(u32 arg0) {
    func_002C49F0(arg0, 0);
}

s32 mnuFindMatchingPartyEntryIndex(s32 object) {
    s32 i;
    u8 *entry = D_00435DD0 + 0xA60;
    for (i = 0; i < 5; i++, entry += 0x1C4) {
        if ((*(u16 *)entry & 1) && *(u16 *)(object + 4) == *(u16 *)(entry + 4)) {
            return i;
        }
    }
    return 0;
}

s32 mnuLookupRangeEntry(u16 arg0) {
    u16 *table = D_003E7900;
    s8 *entries = D_003E7928;
    u32 key = arg0 & 0xffff;
    u32 i;
    for (i = 0; i < 0x14; i += 2, table += 2) {
        if (key < table[0]) {
            continue;
        }
        if (key >= table[1]) {
            continue;
        }
        {
            u32 j = 0;
            s8 *entry = entries + 1;
            for (; j < 6; j++, entry += 3) {
                if (i != entry[1]) {
                    continue;
                }
                return entry[0];
            }
        }
    }
    return 0;
}

u16 mnuPickPairedTableValue(s32 index, s32 alt) {
    return (alt == 0) ? D_003E7900[index] : D_003E7902[index];
}

s32 mnuGetIndexedNonzeroEffect(s32 index) {
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

u16 mnuLookupPartyTableValue(u32 count, s32 base, s32 which) {
    u32 i;
    s32 sum = 0;

    for (i = 0; i < count; i++) {
        sum += mnuGetIndexedNonzeroEffect(i);
    }
    if (which == 0) {
        return D_003E7900[D_003E792A[(base + sum) * 3]];
    }
    return D_003E7902[D_003E792A[(base + sum) * 3]];
}

u16 func_002C4D78(s32 arg0) {
    s32 temp_v0 = (arg0 & 0xffff) * 56 + D_00435E20;

    if (((RangeEntry *)temp_v0)->secondaryKind != 2) {
        return 0;
    }
    return ((RangeEntry *)temp_v0)->secondaryValue;
}

u8 func_002C4DB0(u32 arg0) {
    return ((RangeEntry *)((arg0 & 0xffff) * 0x38 + D_00435E20))->kind;
}

u16 mnuGetAdjustedEntryValue(s32 id, s32 object) {
    s32 entry = (id & 0xFFFF) * 0x38 + D_00435E20;
    u16 base = ((RangeEntry *)entry)->value;
    u16 addition = ((RangeEntry *)entry)->addition;
    if (func_002C4DB0(id & 0xFFFF) == 1) {
        base = addition + *(u16 *)(object + 8) * base / 100;
    }
    return base;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C4E58);

s32 func_002C4EB8(u16 id, s32 item) {
    u16 minimum = ((RangeEntry *)D_00435E20)[id].value;
    s32 kind = func_002C4DB0(id);

    switch (kind) {
    case 1:
        if (*(u16 *)(item + 6) < minimum) {
            return 0;
        }
        break;
    case 2:
        if (*(u16 *)(item + 0xA) < minimum) {
            return 0;
        }
        break;
    }
    return 1;
}

extern s32 func_002C4EB8(u16, s32);

s32 func_002C4F50(s32 context, u16 id) {
    if (func_002C4EB8(id, context) == 0) return -1;
    if ((((RangeEntry *)(D_00435E20 + id * 56))->flags & 1) == 0) return 1;
    if (id < 0x220) return 0;
    return 1;
}

s32 func_002C4FB8(u16 id, s32 object) {
    s32 kind = ((RangeEntry *)D_00435E20)[id].kind;
    u16 value = ((RangeEntry *)D_00435E20)[id].value;

    switch (kind) {
    case 1:
        if (*(u16 *)(object + 6) < value) {
            return 1;
        }
        break;
    case 2:
        if (*(u16 *)(object + 0xA) < value) {
            return 1;
        }
        break;
    }
    return 0;
}

s32 func_002C5030(s32 arg0, u8 *cursor) {
    u8 *record = (u8 *)((arg0 & 0xFFFF) * 0x38 + D_00435E20);
    u16 amount = *(u16 *)(record + 4);

    switch (record[3]) {
    case 1:
        if (*(u16 *)(cursor + 6) < amount) {
            return 0;
        }
        datMoveCursorX(cursor, -amount);
        return 1;
    case 2:
        if (*(u16 *)(cursor + 0xA) < amount) {
            return 0;
        }
        datMoveCursorY(cursor, -amount);
        return 1;
    default:
        return 1;
    }
}

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

s32 func_002C5358(s32 arg0, s32 arg1) {
    s32 flags = ((AffinityRow *)D_00435E24)[(arg0 - 0x1AB) & 0xFFFF].affinity[arg1];

    if (flags == -1) {
        return -1;
    }
    if (flags == 0x20000000) {
        return 0;
    }
    if (flags == 0x20000002) {
        return 1;
    }
    if (flags == 0x20000004) {
        return 2;
    }
    if (flags == 0x20000008) {
        return 3;
    }
    if (flags == 0x20000010) {
        return 4;
    }
    if (flags == 0x20000020) {
        return 5;
    }
    if (flags == 0x20000040) {
        return 6;
    }
    if (flags == 0x20000080) {
        return 7;
    }
    if (flags == 0x20000100) {
        return 8;
    }
    if (flags & 0x40000000) {
        return -2;
    }
    return -1;
}

s32 func_002C5428(s32 arg0, s32 arg1) {
    s32 value = ((AffinityRow *)D_00435E24)[(arg0 - 0x1AB) & 0xFFFF].affinity[arg1];

    if (value == -1) {
        return 0;
    }
    if (value & 0x40000000) {
        return value & ~0x40000000;
    }
    return value;
}

s32 func_002C5480(s32 arg0) {
    if (arg0 < 0xA0) {
        return 0;
    }
    return arg0 < 0xC0;
}

s32 func_002C5498(s32 arg0) {
    if (arg0 < 0x60) {
        return 0;
    }
    return arg0 < 0x6D;
}

s32 func_002C54B0(s32 arg0) {
    if (arg0 < 0xC0) {
        return 0;
    }
    return arg0 < 0x100;
}

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
    mnuMarkEntryBlocked(arg1);
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

void mnuMarkEntryBlocked(s32 index) {
    s32 offset = 0x1E730 + index;
    if (index != 0) {
        D_00435DD0[offset] |= 1;
    }
}

void mnuClearEntryBlocked(s32 index) {
    s32 offset = 0x1E730 + index;
    if (index != 0) {
        D_00435DD0[offset] &= ~1;
    }
}

s32 mnuIsEntryBlocked(s32 index) {
    if (index == 0) {
        return 1;
    }
    return *(u8 *)(index + (s32)D_00435DD0 + 0x1E730) & 1;
}

s32 func_002C5700(void) {
    s32 index;
    for (index = 0xC0; index < 0x100; index++) {
        if (!mnuIsEntryBlocked(index) && *(u8 *)(index + (s32)D_00435DD0 + 0x1340) != 0) {
            return 1;
        }
    }
    return 0;
}

s32 func_002C5758(u32 id, s32 mode) {
    u16 *entry = D_003E78D8;
    u32 i;

    for (i = 0; i < 5; i++, entry += 4) {
        if (id == entry[0]) {
            switch (mode) {
            case 0:
                id = entry[1];
                id += 0x10;
                break;
            case 1:
                id = entry[2];
                id += 0x17;
                break;
            default:
                id = entry[3];
                id += 0x17;
                break;
            }
            func_0026C5B8(id);
            return 1;
        }
    }
    return 0;
}

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

s32 mnuGetSelectionFromFlags(MenuSelectionEntry *entry) {
    u16 flags = entry->flags;
    if (flags & 0x400) return 0;
    if (flags & 0x100) return 1;
    if (flags & 0x80) return 2;
    if (flags & 0x40) return 3;
    if (flags & 0x10) return 4;
    return -1;
}

s32 mnuGetMatchingPartyEntryMask(s32 object) {
    s32 i;
    u8 *entry = D_00435DD0 + 0xA60;
    for (i = 0; i < 5; i++, entry += 0x1C4) {
        if ((*(u16 *)entry & 1) && *(u16 *)(entry + 4) == *(u16 *)(object + 4)) {
            return 1 << i;
        }
    }
    return 0;
}

s32 func_002C5CD0(s32 amount, s32 divisor) {
    s32 percent;

    if (divisor != 0) {
        percent = amount * 100 / divisor;
        if (percent < 25) {
            return 2;
        }
        if (percent < 50) {
            return 1;
        }
    }
    return 0;
}

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

void evtStageTestSetEntryIndex(s32 arg0, s32 value) {
    s32 index = arg0 & 0xFFFF;

    if (value < 0) {
        value = 0;
    }
    if (D_00457EB0[2] != 0 && value >= mdlGetNodeRefHalf(D_00457EB0[2], 0)) {
        value = mdlGetNodeRefHalf(D_00457EB0[2], 0) - 1;
    }
    *(s8 *)(index * 60 + D_00457EB0[4] + 1) = value;
    func_002C6E20(-1);
}

void evtStageTestAddEntryValue(s32 arg0, f32 delta) {
    s32 index = arg0 & 0xFFFF;
    f32 *entry;

    if (delta < 0.0f && ((f32 *)(index * 60 + D_00457EB0[4]))[6] - delta < 0.0f) {
        return;
    }
    entry = (f32 *)(index * 60 + D_00457EB0[4]);
    entry[6] += delta;
    func_002C6E20(-1);
}

void func_002C6610(s32 index, s32 dx, s32 dy, s32 dz) {
    s32 offset = (index & 0xFFFF) * 60;
    f32 *position = (f32 *)(offset + (s32)D_00457EC0[0]);
    f32 x = position[7] + (f32)dx;
    f32 y = position[8] + (f32)dy;
    f32 z = position[9] + (f32)dz;

    position[7] = x;
    position[8] = y;
    position[9] = z;
}

void func_002C6670(s32 index, s32 dx, s32 dy, s32 dz) {
    s32 offset = (index & 0xFFFF) * 60;
    f32 *position = (f32 *)(offset + (s32)D_00457EC0[0]);
    f32 x = position[11] + (f32)dx;
    f32 y = position[12] + (f32)dy;
    f32 z = position[13] + (f32)dz;

    position[11] = x;
    position[12] = y;
    position[13] = z;
}

u8 func_002C66D0(u16 index) {
    return D_00457EC0[0][index * 60 + 1];
}

f32 func_002C66F8(u16 index) {
    return *(f32 *)(D_00457EC0[0] + index * 60 + 0x18);
}

void func_002C6720(s32 index, f32 *out) {
    s32 offset = (index & 0xFFFF) * 60;
    f32 *position = (f32 *)(offset + (s32)D_00457EC0[0]);

    out[0] = position[7];
    out[1] = position[8];
    out[2] = position[9];
}

void func_002C6758(s32 index, f32 *out) {
    s32 offset = (index & 0xFFFF) * 60;
    f32 *position = (f32 *)(offset + (s32)D_00457EC0[0]);

    out[0] = position[11];
    out[1] = position[12];
    out[2] = position[13];
}

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

void mnuResetWorkFloats(void) {
    func_002C6A20();
    D_0037F5E0[4] = 2048.0f;
    D_0037F5E0[5] = 2048.0f;
}

void func_002C6AC0(s32 arg0, s32 arg1, s32 arg2) {
    D_00457EB4[0] = func_00231B80();
}

void mnuForwardTableByte(s32 arg0) {
    func_002C6AC0(D_00457EC8[1], *(u8 *)(D_00457EC8[-2] + (arg0 & 0xffff) * 60), 0);
}

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

#define VU_LOAD10(p) __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0(%0)\n\t.set reorder" : : "r"(p))

extern s32 D_00435DF0;
extern void mdlStoreTertiaryVectorVU(s32);

f32 mnuSetModelScaleVector(s32 arg0, s32 useTable) {
    f32 scale = 1.0f;
    f32 vec[4];

    if (useTable != 0) {
        scale = *(f32 *)(D_00435DF0 + D_00457ED0[0] * 0x270 + 0x10);
    }
    vec[0] = scale;
    vec[1] = scale;
    vec[2] = scale;
    vec[3] = 1.0f;
    VU_LOAD10(vec);
    mdlStoreTertiaryVectorVU(arg0);
    return scale;
}

void mnuResetWorkPair(void) {
    *(s32 *)(D_003E7950 + 0) = 0;
    *(s32 *)(D_003E7950 + 4) = 0;
    *(f32 *)(D_003E7950 + 8) = -400.0f;
    *(s32 *)(D_003E7940 + 0) = 0;
    *(s32 *)(D_003E7940 + 4) = 0;
    *(s32 *)(D_003E7940 + 8) = 0;
}

extern void mdlStorePrimaryVectorVU(s32);

void mnuApplyModelCamera(s32 arg0) {
    f32 vec[4];
    u8 *entry;
    f32 scale;

    memset(vec, 0, sizeof(vec));
    vec[3] = 1.0f;
    entry = (u8 *)(D_00457EB0[6] * 60 + D_00457EB0[4]);
    vec[0] = *(f32 *)(entry + 0x1C);
    vec[1] = *(f32 *)(entry + 0x20);
    if (*(s8 *)((u8 *)D_00457EB0 + 0xC) != 1) {
        mnuSetModelScaleVector(arg0, 0);
        vec[2] = *(f32 *)((u8 *)(D_00457EB0[6] * 60 + D_00457EB0[4]) + 0x24);
        mnuResetWorkPair();
    } else {
        scale = mnuSetModelScaleVector(arg0, 1);
        entry = (u8 *)(D_00457EB0[6] * 60 + D_00457EB0[4]);
        vec[0] -= *(f32 *)(entry + 0x1C) - *(f32 *)(entry + 0x1C) * scale;
        vec[1] -= *(f32 *)(entry + 0x20) - *(f32 *)(entry + 0x20) * scale;
        vec[2] = 0.0f;
        *(f32 *)(D_003E7950 + 8) = (-400.0f - *(f32 *)(entry + 0x24)) * scale;
    }
    VU_LOAD10(vec);
    mdlStorePrimaryVectorVU(arg0);
}

void evtStageTestApplyEntryRotation(s32 arg0) {
    f32 *entry = (f32 *)(D_00457EB0[6] * 60 + D_00457EB0[4]);

    func_00340DC8(entry[11] * 3.14159265f / 180.0f, entry[12] * 3.14159265f / 180.0f,
                  entry[13] * 3.14159265f / 180.0f);
    func_00232AD0(arg0);
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C7168);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C72E0);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C73C0);

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C7428);

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

void func_002C7568(void) {
    s32 index;
    s32 node;

    if (D_00457EC8[6] != 0 && D_00457EC8[6] != 3 && (node = func_002C64B8()) != 0) {
        if (D_00457EC8[6] == 1) {
            index = D_00457EC8[7];

            if (index < mdlGetNodeRefHalf(node, 0)) {
                mdlAddEntryPlainEx(node, 0, index, (s32)D_00457EC8[8], (s32)D_00457EC8[9]);
                D_00457EC8[6] = 2;
            }
        } else if (!(D_00457EC8[5] & 1) && (*(u8 *)(*(s32 *)(node + 0x1C) + 0x30) == 5 || D_00457EC8[6] == 4)) {
            index = *(u8 *)(D_00457EC8[0] * 0x3C + D_00457EC8[-2] + 1);

            if (index < mdlGetNodeRefHalf(node, 0)) {
                mdlAddEntryFlaggedEx(node, 0, index, (s32)D_00457EC8[8], (s32)D_00457EC8[9]);
                D_00457EC8[6] = 3;
            }
        }
    }
}

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

void *evtCreateBattleStageTestCamera(void) {
    f32 position[4] = {401.0f, -593.0f, -1208.25f, 0.0f};
    f32 orientation[4] = {0.22f, 0.12f, 0.03f, 1.0f};
    StageCameraTarget *target = func_0023B018(position, orientation);

    target->unk8 = D_00437CB0;
    return func_002C79B8;
}

INCLUDE_ASM(const s32, "game/code_002BE628", func_002C7A60);

void func_002C7C00(void) {
    evtDestroyWorldSecondaryNode();
}

void func_002C7C18(void) {
    kwlnTaskDestroyWithHierarchyByName(D_0042B610, 1);
    kwlnDebugGraphSetEnabled(0);
}


void btlCreateStageTestTask(void) {
    kwlnDebugGraphSetEnabled(1);
    kwlnTaskCreate(D_0042B610, 0x2B0C, 1, 1, func_002C7A60, func_002C7C00, 0);
}

s32 btlDestroyStageTask(object)
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
    btlDestroyStageTask();
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

