#include "common.h"

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





extern void mdlAddEntryFlaggedEx(s32, s32, s32, f32, f32);
extern void mdlAddEntryPlainEx(s32, s32, s32, f32, f32);














extern void func_00284108(s32, s32, s32, s32, s32, s32, s32 *);


extern void func_00288148(s32);
extern void func_001F3188(s32);
extern void func_00217878(s32, s32);
extern void mnuApplyModelCamera(s32);
extern void stageTestApplyEntryRotation(s32);
extern void func_00287C20(void);
extern void func_00288008(void);

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
extern s32 D_003BAA00;
extern void func_00285960(u8 *entry, s32 arg1, u32 index, PartyPanel *panel);

typedef struct AffinityRow {
    s32 affinity[4];
} AffinityRow;

extern void func_002E7F20(f32, f32, f32);
extern void func_00217FB8(s32);

typedef struct StageCameraTarget {
    u8 pad[8];
    void *unk8;
} StageCameraTarget;
extern StageCameraTarget *func_002204A8(f32 *, f32 *);
extern u8 D_003BC7C8[];

extern s32 mdlGetNodeRefHalf(u32 node, s32 index);

extern void func_002878D8(s32 arg0);

extern s32 D_003BC7B4;

extern s32 D_003BAA4C;

extern s32 D_003BAA50;

extern u32 func_002BD258(u32);

extern s32 mnuLookupRangeEntry(u16);

extern u32 func_0027D4A0(u32);

extern s32 func_002CFEB8(u32);

extern s32 func_002877A8(void);

typedef struct MenuPanelGroup {
    u8 pad00[0x0C];
    s32 children[5]; /* 0x0C */
    u32 selection;    /* 0x20 */
    s32 initialValue; /* 0x24: initialized to 0x100 */
} MenuPanelGroup;

extern void func_002830F8(MenuPanelGroup *);

extern char D_003B2608[]; /* "battle stage test" */

extern u8 D_0037CE60[];

extern u8 D_0037CE70[];

extern f32 D_003245E0[];

extern u32 D_003DC5F0[];

extern u32 D_003DC608[];

extern u32 D_003DC618[];

extern u32 D_003DC650[];

extern u32 D_003DC654[];

extern u32 D_003DC5E8[];

extern u32 D_003DC5F8[];

extern u32 D_003DC600[];

extern u8 D_003DC5F4[];

extern u32 D_003DC5EC[];

extern void btlStopStage(void);

extern void func_00288500(void);

INCLUDE_ASM(const s32, "game/code_00282850", func_00282850);
INCLUDE_ASM(const s32, "game/code_00282850", func_002828D0);

/* Advance the panel's current transition value toward its 0x100 limit. */
void func_002829C0(s32 arg0) {
    if (*(s32 *)(arg0 + 4) < 0x100) {
        *(s32 *)(arg0 + 4) = *(s32 *)(arg0 + 4) + 8;
    }
}


typedef struct MenuStageTestState {
    u32 flags;
    u8 pad04[0x680];
    s32 selectedPanel; /* 0x684 */
    s32 scrollOffset;  /* 0x688 */
} MenuStageTestState;

typedef struct MenuStageNode {
    u8 pad00[0xC];
    s32 overrideValue;
} MenuStageNode;

typedef struct MenuStagePanel {
    u8 pad00[0xE0];
    MenuStageNode *node;
} MenuStagePanel;

/* Apply a temporary override to the selected node while drawing its panel. */
void func_002829E0(s32 x, s32 y, s32 z, s32 overrideValue, MenuStageTestState *menu, s32 param) {
    s32 positionOffset[2];
    MenuStagePanel *panel = (MenuStagePanel *)((s32)menu + menu->selectedPanel * 0x134 + 0x78);
    MenuStageNode *node;

    func_002828D0(positionOffset, menu, 0);
    node = panel->node;
    if (node != NULL) {
        node->overrideValue = overrideValue;
    }
    x += menu->scrollOffset * 0x10;
    menu->scrollOffset = (s32)((f32)menu->scrollOffset / 1.19999993f);
    if (menu->flags & 0x100) {
        func_00282850(x + positionOffset[0], y + positionOffset[1], z, menu, menu->selectedPanel, param);
    } else {
        func_00282850(x + positionOffset[0], y + positionOffset[1], z, menu, menu->selectedPanel, param);
    }
    node = panel->node;
    if (node != NULL) {
        node->overrideValue = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00282B08);

void func_00282BE8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    func_00282B08(arg0, arg1, arg2, 0, arg3, arg4);
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
    u32 secondValueA; /* 0x28 */
    u32 secondValueB; /* 0x2C */
    MenuPoint secondPosition; /* 0x30 */
    MenuPoint thirdPosition; /* 0x38 */
    u32 thirdValueA; /* 0x40 */
    u32 thirdValueB; /* 0x44 */
    MenuPoint fourthPosition; /* 0x48 */
    u32 fourthValueA; /* 0x50 */
    u32 fourthValueB; /* 0x54 */
    u32 fourthValueC; /* 0x58 */
    u8 pad5C[4];
    u32 resourceHandle; /* 0x60 */
} MenuPanelState;

void *mnuCreatePanelState(s32 width, s32 height) {
    MenuPanelState *panel = func_002CFEB8(0x64);
    memset(panel, 0, 0x64);
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
    func_002CFF98(panel);
}

void func_00282CA8(MenuPanelState *panel, u32 valueA, u32 valueB, u32 x,
                                    u32 y) {
    panel->firstValueA = valueA;
    panel->firstValueB = valueB;
    func_002BFB98(&panel->firstPosition, x, y);
}

void func_00282CD0(MenuPanelState *panel, u32 valueA, u32 valueB, u32 x,
                                    u32 y) {
    panel->secondValueA = valueA;
    panel->secondValueB = valueB;
    func_002BFB98(&panel->secondPosition, x, y);
}

void mnuInitializePanelResource(MenuPanelState *panel) {
    u32 resourceHandle;

    resourceHandle = func_0027D4A0(2);
    panel->resourceHandle = resourceHandle;
}

void func_00282D28(MenuPanelState *panel, u32 valueA, u32 valueB, u32 x,
                                    u32 y) {
    panel->thirdValueA = valueA;
    panel->thirdValueB = valueB;
    func_002BFB98(&panel->thirdPosition, x, y);
}

void func_00282D50(MenuPanelState *panel, u32 valueA, u32 valueB, u32 x,
                                    u32 y, u32 additionalValue) {
    panel->fourthValueA = valueA;
    panel->fourthValueB = valueB;
    func_002BFB98(&panel->fourthPosition, x, y);
    panel->fourthValueC = additionalValue;
}

void func_00282D98(MenuPanelState *panel, u32 state) {
    panel->state = state;
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00282DA0);

s32 mnuCreatePanelGroup(s32 parent) {
    MenuPanelGroup *group = func_002CFEB8(0x28);
    s32 i;
    for (i = 0; i < 5; i++) {
        s32 child = func_00284888();
        func_002848E0(child, parent, i);
        group->children[i] = child;
    }
    func_002830F8(group);
    group->initialValue = 0x100;
    return (s32)group;
}

void mnuDestroyPanelGroup(MenuPanelGroup *group) {
    s32 i;
    for (i = 0; i < 5; i++) {
        func_00284C30(group->children[i]);
    }
    func_002CFF98(group);
}

extern void func_00284A90(s32, s32, s32);
void mnuUpdateFiveListEntries(MenuPanelGroup *group, s32 data) {
    s32 i;
    for (i = 0; i < 5; i++) {
        func_00284A90(group->children[i], data, i);
    }
}

void func_002830F0(MenuPanelGroup *group, u32 selection) {
    group->selection = selection;
}

void func_002830F8(MenuPanelGroup *group) {
    group->selection = 0xffffffff;
}

u32 func_00283108(MenuPanelGroup *group) {
    return group->selection;
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00283110);

void func_00283238(MenuPanelGroup *group, s32 index, s32 value, s32 option) {
    /* Required to match: retain byte-offset indexing for the child slot. */
    s32 offset = index * 4;
    s32 base = (s32)group + 0xC;
    s32 *item = (s32 *)(base + offset);
    func_00284C10(*item, value);
    func_00284C28(*item, option);
}

typedef struct MenuSpriteState {
    u8 pad00[0x10];
    s32 x;
    s32 y;
    s32 z;
    s32 initialValue; /* 0x1C: initialized to 0x100 */
} MenuSpriteState;

void *mnuCreateSpriteState(s32 x, s32 y, s32 z) {
    MenuSpriteState *item = func_002CFEB8(0x20);
    memset(item, 0, 0x20);
    item->x = x;
    item->y = y;
    item->z = z;
    item->initialValue = 0x100;
    return item;
}

void func_002832F8(void) {
    func_002CFF98();
}

/* Select the range entry's sprite variant before submitting its draw request. */
void func_00283310(u32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u16 rangeId, s32 alternate, u32 arg6, u32 arg7) {
    s32 rangeIndex;
    s32 variant;

    variant = 0x11;
    if (alternate != 0) {
        variant = 0x12;
    }
    rangeIndex = mnuLookupRangeEntry(rangeId);
    func_002BF4E0(arg0, arg1, arg2, arg3, 1, arg6, rangeIndex * 2 + variant, arg7);
}

INCLUDE_ASM(const s32, "game/code_00282850", func_002833B0);

u32 *func_00283788(u32 x, u32 y, u32 z, u32 color, u32 texture) {
    u32 *sprite = func_002CFEB8(0x28);
    memset(sprite, 0, 0x28);
    sprite[4] = x;
    sprite[5] = y;
    sprite[6] = z;
    sprite[7] = color;
    sprite[8] = texture;
    sprite[9] = 0x100;
    return sprite;
}

void func_00283820(void) {
    func_002CFF98();
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00283838);

void func_00283BE0(s32 arg0, s32 arg1) {
    *(s32 *)(arg0 + 4) = arg1;
    *(s32 *)arg0 = 0;
    *(s32 *)(arg0 + 8) = 0;
}

void func_00283BF0(u32 *arg0, u32 arg1) {
    *arg0 = arg1;
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00283BF8);

typedef struct MenuEffectPosition {
    u8 pad00[0x20];
    s32 *coordinates;
} MenuEffectPosition;

typedef struct MenuEffectNode {
    u8 pad00[8];
    MenuEffectPosition *position;
} MenuEffectNode;

typedef struct MenuEffectPair {
    u8 pad00[0x19];
    s8 positionY; /* 0x19 */
    u8 pad1A[0x26];
    MenuEffectNode *first;  /* 0x40 */
    MenuEffectNode *second; /* 0x44 */
} MenuEffectPair;

/* Feed two mirrored effect positions from the active menu entry. */
void func_00283CA8(MenuEffectPair *pair) {
    MenuEffectNode *first = pair->first;
    MenuEffectNode *second = pair->second;
    MenuEffectPosition *firstData = first->position;
    MenuEffectPosition *secondData = second->position;
    s32 *pos = firstData->coordinates;

    pos[0] = 10;
    pos[1] = pair->positionY;
    pos[2] = 10;
    pos = secondData->coordinates;
    pos[1] = 5;
    pos[0] = 10;
    pos[2] = 10;
}

s32 mnuRateByThreshold(s32 object) {
    s32 value = *(s32 *)(object + 0x10);

    if (value < 0x32) {
        return (value >= 0x14) ? 1 : 2;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00283D10);

/* Cycle through four indexed settings while refreshing the paired effects. */
void func_00283E60(s32 object) {
    s32 *settings;
    s32 setting;

    func_00283D10(object);
    func_00283CA8(object);
    settings = *(s32 **)(object + 0x14);
    setting = 0;
    if (settings != 0) {
        setting = settings[*(s8 *)(object + 0x18)];
    }
    effConfigureWithDefaultSetting(*(s32 *)(object + 0x38), 0, *(s32 *)(object + 0x40), 0, setting, 0);
    *(u8 *)(object + 0x18) += 1;
    if ((s8)*(u8 *)(object + 0x18) >= 4) {
        *(u8 *)(object + 0x18) = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00283EE0);

void func_00284080(s32 object) {
    u32 effectHandle;

    effectHandle = func_002BD258(3);
    *(u32 *)(object + 0x40) = effectHandle;
    effectHandle = func_002BD258(3);
    *(u32 *)(object + 0x44) = effectHandle;
}

void func_002840B8(s32 *list) {
    u32 i;
    for (i = 0; i < 2; i++) {
        effDestroyPackedBatch(list[i + 16]);
    }
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00284108);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B2420);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B2430);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B2450);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B2468);

void func_00284258(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 tableA[9] = {6, 3, 0xE, 0xE, 5, 4, 0xC, 0xA, 8};
    s32 tableB[9] = {6, 0, 0xD, 0xD, 2, 1, 0xB, 9, 7};

    if (arg1 == 1) {
        func_00284108(arg0, arg1, arg2, arg3, arg4, arg5, tableA);
    } else {
        func_00284108(arg0, arg1, arg2, arg3, arg4, arg5, tableB);
    }
}

void mnuReleaseSpriteTextures(s32 *object) {
    u32 i;
    for (i = 0; i < 9; i++) {
        func_002BDD60(object[i + 7]);
    }
    func_002840B8(object);
}

u32 func_002843A0(s32 useDefault, s32 index, s32 option) {
    u32 color = 0xA09DC380;
    if (!useDefault) {
        switch (func_00286EF8(index, option)) {
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

INCLUDE_ASM(const s32, "game/code_00282850", func_00284418);

INCLUDE_ASM(const s32, "game/code_00282850", func_002845F8);

void func_00284880(void) {
}

typedef struct MenuPanelItem {
    u8 pad00[0x10];
    u32 value10;
    s32 value14;
    s32 value18;
    u32 option;
    s32 selection;
    u8 pad24[0x38];
    MenuPoint points[5]; /* 0x5C */
    u8 pad84[4];
    u32 initialValue; /* 0x88 */
    u32 selectionRamp; /* 0x8C */
} MenuPanelItem;

s32 func_00284888(void) {
    MenuPanelItem *item = func_002CFEB8(0x90);
    memset(item, 0, 0x90);
    item->value14 = 0x63;
    item->value10 = 0x8c;
    item->initialValue = 0x100;
    return (s32)item;
}

INCLUDE_ASM(const s32, "game/code_00282850", func_002848E0);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B24D0);

void func_00284A90(s32 obj, s32 param, s32 index) {
    s32 table[5] = {0, 4, 1, 2, 3};
    MenuPanelItem *item = (MenuPanelItem *)obj;

    func_002BFB98(&item->points[0], param, 7);
    func_002BF9E0(item->points[0].x, item->points[0].y, 0, 0, 0, 0);
    func_002BFB98(&item->points[1], param, 5);
    func_002BF9E0(item->points[1].x, item->points[1].y, 0x440, 0x58, 0, 0);
    func_002BFB98(&item->points[2], param, 6);
    func_002BF9E0(item->points[2].x, item->points[2].y, 0x440, 0x58, 0, 0);
    func_002BFB98(&item->points[3], param, 9);
    func_002BF9E0(item->points[3].x, item->points[3].y, 0x620, 0x50, 0, 0);
    func_002BFB98(&item->points[4], param, table[index]);
    func_002BF9E0(item->points[4].x, item->points[4].y, 0x100, -0x48, 0, 0);
}

void func_00284BF8(MenuPanelItem *item, u32 value) {
    item->value10 = value;
}

void func_00284C00(MenuPanelItem *item, s32 value, s32 option) {
    item->value18 = value;
    item->option = option;
}

void func_00284C10(MenuPanelItem *item, s32 selection) {
    if (item->selection != selection) {
        item->selectionRamp = 0x100;
    }
    item->selection = selection;
}

void func_00284C28(MenuPanelItem *item, u32 option) {
    item->option = option;
}

void func_00284C30(void) {
    func_002CFF98();
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00284C48);

INCLUDE_ASM(const s32, "game/code_00282850", func_00284EB8);

void func_002850C8(s32 arg0, s32 arg1, s32 arg2) {
    *(s32 *)(arg0 + 0x10) = arg1;
    *(s32 *)(arg0 + 0x14) = arg2;
}

u32 *func_002850D8(s32 source) {
    u32 *item = (u32 *)func_002CFEB8(0x3c);
    s32 first;
    u32 second;
    memset(item, 0, 0x3c);
    first = scrGetSelectedOperandIndex(source);
    second = ptyGetCurrentProfileRecord(source);
    func_002850C8(item, prfGetCapValue((u16)first), *(u32 *)second);
    item[14] = 0x100;
    return item;
}

void func_00285160(void) {
    func_002CFF98();
}

void func_00285178(s32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4) {
    func_002BFB98((u32 *)(arg0 + 0x18));
    func_002BF9E0(*(u32 *)(arg0 + 0x18), *(u32 *)(arg0 + 0x1c), 0, 0, 0, 0);
    func_002BFB98(arg0 + 0x20, arg1, arg3);
    func_002BFB98(arg0 + 0x28, arg1, arg4);
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00285208);

void func_00285440(s32 x, s32 y, s32 z, u32 *item, s32 option) {
    s32 previous;
    s32 next;
    func_00285208(x, y, z, item, option);
    previous = item[13];
    next = previous + 12;
    if (previous < 0x200) {
        item[13] = next;
        if (next < 0x200) {
            return;
        }
        previous = next;
    }
    item[13] = previous - 0x200;
}

void func_00285490(u32 arg0) {
    memset(arg0, 0, 0x4c);
}

INCLUDE_ASM(const s32, "game/code_00282850", func_002854B0);

void func_00285600(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)arg0;
    while (temp_v0 != 0) {
        func_002854B0(1, 0, arg0, arg1);
        temp_v0 = *(s32 *)arg0;
    }
}

s32 func_00285658(s32 *arg0) {
    return (*arg0 & 0x200000) > 0;
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00285670);

u8 func_002858D8(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 0x44) == arg1;
}

void func_002858E8(s32 arg0, s32 arg1) {
    u16 temp_v0;

    temp_v0 = *(u16 *)arg1;
    *(s32 *)arg0 = arg1;
    *(s32 *)arg1 = temp_v0;
}

void func_002858F8(s32 arg0, s32 arg1) {
    u16 temp_v0;

    temp_v0 = *(u16 *)arg1;
    *(s32 *)arg0 = arg1;
    *(s32 *)arg1 = temp_v0 | 0x20000;
}

void func_00285910(s32 arg0, s32 arg1) {
    u16 temp_v0;

    temp_v0 = *(u16 *)arg1;
    *(s32 *)arg0 = arg1;
    *(s32 *)arg1 = temp_v0 | 0x60000;
}

void func_00285928(s32 arg0, u32 arg1) {
    if (*(s32 *)(arg0 + 0x44) != 0) {
        func_002858F8(arg1, *(s32 *)(arg0 + 0x44));
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00285960);

void initPartyPanelSlots(PartyPanel *panel) {
    u32 i;
    u8 *entry;
    s32 offset = 0;

    memset(panel, 0, 0x10C);
    panel->unk0 = 0;
    panel->unk4 = 0;
    for (i = 0; i < 5; i++) {
        entry = (u8 *)(D_003BAA00 + offset + 0xA60);
        offset += 0x1A4;
        if (*(u16 *)entry & 1) {
            func_00285960(entry, 0, i, panel);
            panel->slots[i].index = i;
        } else {
            panel->slots[i].unk8 = -1;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00285B20);

void menuPlayInputSound(s32 arg0, s32 buttons, s32 *state) {
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
        if (buttons & 0xF0) {
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

INCLUDE_ASM(const s32, "game/code_00282850", func_00285F00);

void func_00286050(u32 arg0) {
    func_00285F00(arg0, 0);
}

extern s32 D_003BAA00;
s32 mnuFindMatchingPartyEntryIndex(s32 object) {
    s32 i;
    u8 *entry = (u8 *)(D_003BAA00 + 0xA60);
    for (i = 0; i < 5; i++, entry += 0x1A4) {
        if ((*(u16 *)entry & 1) && *(u16 *)(object + 4) == *(u16 *)(entry + 4)) {
            return i;
        }
    }
    return 0;
}

extern s8 D_0037CE40[];
extern u16 D_0037CE18[];
extern u16 D_0037CE1A[];

s32 mnuLookupRangeEntry(u16 arg0) {
    u16 *table = D_0037CE18;
    s8 *entries = D_0037CE40;
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
    return (alt == 0) ? D_0037CE18[index] : D_0037CE1A[index];
}

s32 mnuGetIndexedNonzeroEffect(s32 index) {
    s32 count = 0;
    s32 i;
    s8 *entry = D_0037CE40;
    for (i = 0; i < 6; i++, entry += 3) {
        s32 value = *entry;
        if (value != 0) count++;
        if (index == count - 1) return value;
    }
    return 0;
}

extern s8 D_0037CE42[];

u16 lookupPartyTableValue(u32 count, s32 base, s32 which) {
    u32 i;
    s32 sum = 0;

    for (i = 0; i < count; i++) {
        sum += mnuGetIndexedNonzeroEffect(i);
    }
    if (which == 0) {
        return D_0037CE18[D_0037CE42[(base + sum) * 3]];
    }
    return D_0037CE1A[D_0037CE42[(base + sum) * 3]];
}

/* Only secondary-kind-2 entries expose the paired value. */
u16 func_00286288(s32 entryId) {
    RangeEntry *entry = (RangeEntry *)((entryId & 0xffff) * 56 + D_003BAA50);

    if (entry->secondaryKind != 2) {
        return 0;
    }
    return entry->secondaryValue;
}

u8 func_002862C0(u32 arg0) {
    return ((RangeEntry *)((arg0 & 0xffff) * 0x38 + D_003BAA50))->kind;
}

u16 mnuGetAdjustedEntryValue(s32 id, s32 object) {
    RangeEntry *entry = (RangeEntry *)((id & 0xFFFF) * 0x38 + D_003BAA50);
    u16 base = entry->value;
    u16 addition = entry->addition;
    if (func_002862C0(id & 0xFFFF) == 1) {
        base = addition + *(u16 *)(object + 8) * base / 100;
    }
    return base;
}

u16 func_00286368(s32 arg0) {
    s32 index = arg0 & 0xFFFF;
    RangeEntry *record = (RangeEntry *)(index * 0x38 + D_003BAA50);
    u16 scale = record->value;
    u16 addition = record->addition;

    if (func_002862C0(index) == 1) {
        s32 count = 0;
        s32 sum = 0;
        u16 *entry = (u16 *)(D_003BAA00 + 0xA60);
        s32 i;

        for (i = 0; i < 5; i++) {
            u16 flags = entry[0];

            if ((flags & 1) && (flags & 2)) {
                count++;
                sum += entry[4];
            }
            entry += 0x1A4 / 2;
        }
        if (count == 0) {
            return 0;
        }
        scale = addition + sum / count * scale / 100;
    }
    return scale;
}

s32 func_00286440(u16 id, s32 item) {
    u16 minimum = ((RangeEntry *)D_003BAA50)[id].value;
    s32 kind = func_002862C0(id);

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

s32 func_002864D8(s32 object, u16 id) {
    if (func_00286440(id, object) == 0) {
        return -1;
    }
    if (!(((RangeEntry *)D_003BAA50)[id].flags & 1)) {
        return 1;
    }
    if (id < 0x200) {
        return 0;
    }
    return 1;
}

s32 func_00286540(u16 id, s32 object) {
    s32 kind = ((RangeEntry *)D_003BAA50)[id].kind;
    u16 value = ((RangeEntry *)D_003BAA50)[id].value;

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

s32 func_002865B8(s32 arg0, u8 *cursor) {
    u8 *record = (u8 *)((arg0 & 0xFFFF) * 0x38 + D_003BAA50);
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

s32 func_00286648(u16 ability) {
    u8 value;
    if (ability == 0) {
        return 1;
    }
    value = *(u8 *)(D_003BAA50 + ability * 56 + 8);
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

void func_002866B0(u16 arg0) {
    func_00118E38(arg0);
}

u32 func_002866C8(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00282850", skillApplyFieldUseEffect);

u8 func_002868C0(u32 arg0) {
    return *(s8 *)((arg0 & 0xffff) * 2 + D_003BAA4C) == '\x01';
}

extern u32 D_003BAA54;

s32 getAffinityKind(s32 arg0, s32 arg1) {
    s32 flags = ((AffinityRow *)D_003BAA54)[(arg0 - 0x1AB) & 0xFFFF].affinity[arg1];

    if (flags == -1) {
        return -1;
    }
    if (flags == 0x20000000) {
        return 0;
    }
    if (flags == 0x20000002) {
        return 1;
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
    if (flags == 0x20008000) {
        return 7;
    }
    if (flags & 0x40000000) {
        return -2;
    }
    return -1;
}

s32 func_00286990(s32 arg0, s32 arg1) {
    s32 value = ((AffinityRow *)D_003BAA54)[(arg0 - 0x1AB) & 0xFFFF].affinity[arg1];

    if (value == -1) {
        return 0;
    }
    if (value & 0x40000000) {
        return value & ~0x40000000;
    }
    return value;
}

s32 func_002869E8(s32 arg0) {
    if (arg0 < 0xa0) {
        return 0;
    }
    return arg0 < 0xbf;
}

s32 func_00286A00(s32 arg0) {
    if (arg0 < 0x60) {
        return 0;
    }
    return arg0 < 0x7f;
}

u16 func_00286AD0(s32 arg0);

typedef struct PtyBulletInventory {
    u8 pad00[0x12A0];
    u8 itemCount[0xBF];      /* 0x12A0: indexed by item ID */
} PtyBulletInventory;

/* Count a bullet item in the active party: inventory plus one per unit
 * that has the item equipped. Only IDs 0xA0..0xBE are bullet items. */
s32 ptyCountBulletItem(s32 bulletId) {
    s32 count;
    s32 i;

    if (bulletId < 0xA0) {
        return 0;
    }
    if (bulletId >= 0xBF) {
        return 0;
    }
    count = ((PtyBulletInventory *)D_003BAA00)->itemCount[bulletId];
    for (i = 0; i < 5; i++) {
        if (bulletId == func_00286AD0(D_003BAA00 + i * 0x1A4 + 0xA60)) {
            count++;
        }
    }
    return count;
}

u32 func_00286AC0(s32 arg0, u16 arg1) {
    *(u16 *)(arg0 + 0x52) = arg1;
    return 1;
}

u16 func_00286AD0(s32 arg0) {
    return *(u16 *)(arg0 + 0x52);
}

extern u16 D_0037CE00[];
INCLUDE_ASM(const s32, "game/code_00282850", func_00286AD8);

INCLUDE_ASM(const s32, "game/code_00282850", battleItemApplyPermanentBonus);

INCLUDE_ASM(const s32, "game/code_00282850", battleItemApplyDirectEffect);

s32 mnuGetSelectionFromFlags(s32 entry) {
    u16 flags = *(u16 *)(entry + 0xe);
    if (flags & 0x400) return 0;
    if (flags & 0x100) return 1;
    if (flags & 0x80) return 2;
    if (flags & 0x40) return 3;
    if (flags & 0x10) return 4;
    return -1;
}

s32 mnuGetMatchingPartyEntryMask(s32 object) {
    s32 i;
    u8 *entry = (u8 *)(D_003BAA00 + 0xA60);
    for (i = 0; i < 5; i++, entry += 0x1A4) {
        if ((*(u16 *)entry & 1) && *(u16 *)(entry + 4) == *(u16 *)(object + 4)) {
            return 1 << i;
        }
    }
    return 0;
}

s32 func_00286EF8(s32 amount, s32 divisor) {
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

u8 func_00286F48(void) {
    return D_003BC7B4 != 0;
}

void func_00286F58(s32 arg0) {
    if (arg0 == 0) {
        D_003DC5F4[0] = 0;
    } else {
        D_003DC5F4[0] = 1;
    }
}

u32 func_00286F80(void) {
    return D_003DC5F0[0];
}

u32 func_00286F90(void) {
    return D_003DC608[0];
}

void stageTestSetEntryIndex(s32 arg0, s32 value) {
    s32 index = arg0 & 0xFFFF;

    if (value < 0) {
        value = 0;
    }
    if (D_003DC5E8[2] != 0 && value >= mdlGetNodeRefHalf(D_003DC5E8[2], 0)) {
        value = mdlGetNodeRefHalf(D_003DC5E8[2], 0) - 1;
    }
    *(s8 *)(index * 60 + D_003DC5E8[4] + 1) = value;
    func_002878D8(-1);
}

void stageTestAddEntryValue(s32 arg0, f32 delta) {
    s32 index = arg0 & 0xFFFF;
    f32 *entry;

    if (delta < 0.0f && ((f32 *)(index * 60 + D_003DC5E8[4]))[6] - delta < 0.0f) {
        return;
    }
    entry = (f32 *)(index * 60 + D_003DC5E8[4]);
    entry[6] += delta;
    func_002878D8(-1);
}

void func_002870D8(s32 index, s32 dx, s32 dy, s32 dz) {
    s32 offset = (index & 0xFFFF) * 60;
    f32 *position = (f32 *)(offset + D_003DC5F8[0]);
    f32 x = position[7] + (f32)dx;
    f32 y = position[8] + (f32)dy;
    f32 z = position[9] + (f32)dz;
    position[7] = x;
    position[8] = y;
    position[9] = z;
}

void func_00287138(s32 index, s32 dx, s32 dy, s32 dz) {
    s32 offset = (index & 0xFFFF) * 60;
    f32 *position = (f32 *)(offset + D_003DC5F8[0]);
    f32 x = position[11] + (f32)dx;
    f32 y = position[12] + (f32)dy;
    f32 z = position[13] + (f32)dz;
    position[11] = x;
    position[12] = y;
    position[13] = z;
}

u8 func_00287198(s32 arg0) {
    return *(u8 *)(D_003DC5F8[0] + (arg0 & 0xffff) * 60 + 1);
}

f32 func_002871C0(s32 arg0) {
    return *(f32 *)(D_003DC5F8[0] + (arg0 & 0xffff) * 60 + 0x18);
}

/* Keep these repeated byte offsets: sharing a typed base changes ee-gcc's
 * instruction sequence. Each 60-byte entry has vectors at +0x1C and +0x2C. */
void func_002871E8(s32 arg0, f32 *arg1) {
    arg1[0] = *(f32 *)(D_003DC5F8[0] + (arg0 & 0xffff) * 60 + 0x1c);
    arg1[1] = *(f32 *)(D_003DC5F8[0] + (arg0 & 0xffff) * 60 + 0x20);
    arg1[2] = *(f32 *)(D_003DC5F8[0] + (arg0 & 0xffff) * 60 + 0x24);
}

void func_00287220(s32 arg0, f32 *arg1) {
    arg1[0] = *(f32 *)(D_003DC5F8[0] + (arg0 & 0xffff) * 60 + 0x2c);
    arg1[1] = *(f32 *)(D_003DC5F8[0] + (arg0 & 0xffff) * 60 + 0x30);
    arg1[2] = *(f32 *)(D_003DC5F8[0] + (arg0 & 0xffff) * 60 + 0x34);
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00287258);

void func_00287420(f32 arg0) {
    D_003245E0[5] = 2048.0f;
    D_003245E0[4] = arg0 + 2048.0f;
    func_00287258();
}

extern s8 D_0037CE90[];
extern u8 D_0037CE98[];

void func_00287450(s32 mode) {
    f32 offset;
    s32 i;
    s32 value;

    D_003DC5E8[0] = mode;
    D_003DC5E8[1] = 0;
    D_003DC5E8[2] = 0;
    *(u8 *)&D_003DC5E8[3] = 0;
    if (mode == 0) {
        D_003DC5E8[4] = (u32)D_0037CE98;
        offset = -140.0f;
    } else {
        D_003DC5E8[4] = 0;
        offset = 140.0f;
    }
    value = D_0037CE90[mode];
    D_003DC5E8[5] = 0;
    for (i = 0; i < 2; i++) {
        D_003DC5E8[i * 10 + 7] = value;
        D_003DC5E8[i * 10 + 8] = -1;
        D_003DC5E8[i * 10 + 9] = 0;
        D_003DC5E8[i * 10 + 11] = 0;
    }
    func_00287420(offset);
}

void btlStopStage(void) {
    if (D_003DC5E8[0] != 1) {
        D_003DC5E8[27] = -1;
        if (D_003DC5E8[26] != 0) {
            func_00288190();
        }
        if (D_003DC5E8[2] != 0) {
            func_002177D0(D_003DC5E8[2]);
            D_003DC5E8[2] = 0;
        }
    }
}

void mnuResetWorkFloats(void) {
    btlStopStage();
    D_003245E0[4] = 2048.0f;
    D_003245E0[5] = 2048.0f;
}

void func_00287580(s32 arg0, s32 arg1, s32 arg2) {
    D_003DC5EC[0] = func_00217068();
}

void mnuForwardTableByte(s32 arg0) {
    func_00287580(D_003DC600[1], *(u8 *)(D_003DC600[-2] + (arg0 & 0xffff) * 60), 0);
}

u32 func_002875E8(u32 *arg0) {
    return *arg0 & 1;
}

typedef struct MenuBlock40 {
    u32 word[10];
} MenuBlock40;

s32 func_002875F8(u8 *object) {
    if (!(*(u32 *)object & 1)) {
        return 0;
    }
    *(MenuBlock40 *)(object + 4) = *(MenuBlock40 *)(object + 0x2C);
    *(u32 *)object &= ~1;
    return 1;
}

void func_00287678(void) {
    D_003DC5E8[5] &= ~1;
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00287698);

void func_00287788(u16 arg0, u32 arg1) {
    func_00287698(arg0, 0xffffffffffffffff, arg1);
}

INCLUDE_ASM(const s32, "game/code_00282850", func_002877A8);

INCLUDE_ASM(const s32, "game/code_00282850", func_002878D8);

extern u8 *D_003BAA20;
extern void mdlStoreTertiaryVectorVU(void *);

f32 mnuSetModelScaleVector(void *model, s32 useTable) {
    f32 scale = 1.0f;
    f32 vector[4];

    if (useTable != 0) {
        scale = *(f32 *)(D_003BAA20 + D_003DC608[0] * 624 + 0x10);
    }
    vector[0] = scale;
    vector[1] = scale;
    vector[2] = scale;
    vector[3] = 1.0f;
    __asm__ volatile(".set noreorder\n\tlqc2 vf10, 0x0(%0)\n\t.set reorder" : : "r"(vector));
    mdlStoreTertiaryVectorVU(model);
    return scale;
}

void mnuResetWorkPair(void) {
    *(s32 *)(D_0037CE70 + 0) = 0;
    *(s32 *)(D_0037CE70 + 4) = 0;
    *(f32 *)(D_0037CE70 + 8) = -400.0f;
    *(s32 *)(D_0037CE60 + 0) = 0;
    *(s32 *)(D_0037CE60 + 4) = 0;
    *(s32 *)(D_0037CE60 + 8) = 0;
}

INCLUDE_ASM(const s32, "game/code_00282850", mnuApplyModelCamera);

void stageTestApplyEntryRotation(s32 arg0) {
    f32 *entry = (f32 *)(D_003DC5E8[6] * 60 + D_003DC5E8[4]);

    func_002E7F20(entry[11] * 3.14159265f / 180.0f, entry[12] * 3.14159265f / 180.0f,
                  entry[13] * 3.14159265f / 180.0f);
    func_00217FB8(arg0);
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00287C20);

s8 stageTestUpdate(s32 arg0) {
    s8 result = func_002877A8();

    if (result == 1) {
        return 1;
    }
    if (D_003DC5E8[0] != 1) {
        if (D_003DC5E8[2] != 0) {
            mnuApplyModelCamera(D_003DC5E8[2]);
            stageTestApplyEntryRotation(D_003DC5E8[2]);
            func_00287C20();
            if ((s32)D_003DC5E8[27] >= 0) {
                func_00288148(D_003DC5E8[27]);
                D_003DC5E8[27] = -1;
            }
            if (D_003DC5E8[26] != 0) {
                func_001F3188(D_003DC5E8[26]);
            }
            func_00217878(D_003DC5E8[2], arg0);
            func_00288008();
        }
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00287E60);

INCLUDE_ASM(const s32, "game/code_00282850", func_00287EC8);

void func_00287FA8(s32 arg0, f32 arg1, f32 arg2) {
    D_003DC600[6] = 1;
    D_003DC600[7] = arg0;
    D_003DC600[8] = (s32)arg1;
    D_003DC600[9] = (s32)arg2;
}

void func_00287FD0(void) {
    D_003DC618[0] = 4;
}

s32 func_00287FE0(void) {
    s32 temp_v0 = D_003DC618[0];

    if ((temp_v0 == 0) || (temp_v0 == 3)) {
        return 0;
    }
    return 1;
}

void func_00288008(void) {
    s32 index;
    s32 node;

    if (D_003DC600[6] != 0 && D_003DC600[6] != 3 && (node = func_00286F80()) != 0) {
        if (D_003DC600[6] == 1) {
            index = D_003DC600[7];

            if (index < mdlGetNodeRefHalf(node, 0)) {
                mdlAddEntryPlainEx(node, 0, index, (s32)D_003DC600[8], (s32)D_003DC600[9]);
                D_003DC600[6] = 2;
            }
        } else if (!(D_003DC600[5] & 1) && (*(u8 *)(*(s32 *)(node + 0x1C) + 0x30) == 5 || D_003DC600[6] == 4)) {
            index = *(u8 *)(D_003DC600[0] * 0x3C + D_003DC600[-2] + 1);

            if (index < mdlGetNodeRefHalf(node, 0)) {
                mdlAddEntryFlaggedEx(node, 0, index, (s32)D_003DC600[8], (s32)D_003DC600[9]);
                D_003DC600[6] = 3;
            }
        }
    }
}

void func_00288148(s32 arg0) {
    if (D_003DC5E8[26] != 0) {
        func_00288190();
    }
    D_003DC5E8[26] = func_001F3118(D_003DC5E8[2], 0x30);
}

void func_00288190(void) {
    u32 *temp_v0 = D_003DC5E8;
    u32 temp_v1 = temp_v0[26];

    if (temp_v1 == 0) {
        return;
    }
    func_001F3200(temp_v1);
    temp_v0[26] = 0;
}

void func_002881D0(u32 arg0) {
    D_003DC654[0] = arg0;
}

s32 func_002881E0(void) {
    return D_003DC650[0] != 0;
}

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B2520);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B2530);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B2540);

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B2550);

INCLUDE_ASM(const s32, "game/code_00282850", func_002881F0);

u32 func_00288458(void) {
    func_002881F0();
    return 0;
}

void *createBattleStageTestCamera(void) {
    f32 position[4] = {401.0f, -593.0f, -1208.25f, 0.0f};
    f32 orientation[4] = {0.22f, 0.12f, 0.03f, 1.0f};
    StageCameraTarget *target = func_002204A8(position, orientation);

    target->unk8 = D_003BC7C8;
    return func_00288458;
}

INCLUDE_ASM(const s32, "game/code_00282850", func_00288500);

void func_002886A0(void) {
    evtDestroySecondaryWorldNode();
}

void func_002886B8(void) {
    kwlnTaskDestroyWithHierarchyByName(D_003B2608, 1);
    kwlnDebugGraphSetEnabled(0);
}

void btlCreateStageTestTask(void) {
    kwlnDebugGraphSetEnabled(1);
    kwlnTaskCreate(D_003B2608, 0x2b0c, 1, 1, func_00288500, func_002886A0, 0);
}

s32 btlDestroyStageTask(object)
    s32 object;
{
    if (*(u8 *)(object + 1) == 6) {
        s32 resource = *(s32 *)(object + 0xC);
        if (resource != 0) {
            func_002E6E88(resource);
        }
        func_002EDC50(object + 0x30);
        func_002CFF98(*(void **)(object + 8));
        func_002CFF98((void *)object);
        return 0;
    }
    return 1;
}

void func_00288788(void) {
    btlDestroyStageTask();
}

INCLUDE_RODATA(const s32, "game/code_00282850", D_003B2608);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC778);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC780);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC788);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC790);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC798);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7A0);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7A8);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7B0);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7B4);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7B8);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7BC);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7C0);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7C8);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7D0);

INCLUDE_SDATA(const s32, "game/code_00282850", D_003BC7D4);

