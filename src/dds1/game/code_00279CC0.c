#include "common.h"

extern void func_0027CA90();

extern void func_002807E8();

extern s32 mnuGetSelectionFromFlags(s32);

extern void func_002BD870(s32);

extern void func_002BF9E0(s32, s32, s32, s32, s32, s32);

extern void func_00284258(s32, s32, s32, s32, s32, s32);

extern u8 D_0037CD30[][0x10];

extern void func_0027D850(s32, s32, s32, s32, u32 *, s32);
extern void func_0027DA80(s32, s32, s32, s32, u32 *, s32);
extern void func_0027DBD0(s32, s32, s32, s32, u32 *, s32);

extern s32 D_003BAA98;

extern void func_0027C140();

extern void func_0027D318();

extern s32 ptyGetCurrentProfileId(s32);

extern s32 func_002CD240(s32, s32 *);

extern s32 func_00197760(s32, s32, s32, s32, s32, s32);

extern void func_00196088(s32, s32, s32);

extern void func_001958A0(s32, s32, s32);

extern void func_00194920(s32);

extern s32 D_003BAA00;

typedef struct MenuListNode MenuListNode;
extern void func_00300508(MenuListNode **, s32, s32, s32 (*)(MenuListNode **, MenuListNode **));

extern s32 func_00101A70();

extern void func_00272778(s32);

extern void mnuCreateStaffImageSprite(s32);

extern void func_002723B0(s32, s32);

typedef struct MenuSlot {
    s32 resources[3];
    u16 unused;
    u16 flags;
} MenuSlot;

extern MenuSlot *D_003BAA54;

typedef struct MenuPageParams {
    u8 unk0[0x64];
    s32 field64;
    s32 field68;
    s32 field6C;
    s32 field70;
} MenuPageParams;

/* Page records of a menu window, stride 0x134, starting at window + 0x20. */
typedef struct MenuPage {
    u8 pad0[0xC0];
    s32 scaleA;
    u8 padC4[0x50];
    s32 scaleB;
    u8 pad118[0x1C];
} MenuPage;

typedef struct MenuSourceEntry {
    u8 pad0[0x40];
    u32 flags;
    u8 pad44[0x3C];
} MenuSourceEntry;

typedef struct MenuSource {
    u8 pad0[0x10];
    MenuSourceEntry *entries;
} MenuSource;

typedef struct MenuGauge {
    s32 id;
    u8 pad4[4];
    s32 hp;
    s32 mp;
    s32 maxHp;
    s32 maxMp;
    u8 pad18[0xC];
} MenuGauge;

typedef struct MenuRecord {
    u8 pad0[0xC];
    s32 partyIndex;
    MenuGauge gauge;
} MenuRecord;

typedef struct MenuWindow {
    u32 flags;
    u8 pad4[4];
    MenuRecord *records;
    s32 source;
    s32 slot;
    s32 field14;
    s32 field18;
    s32 field1C;
    s32 field20;
    s32 handlesA[8];
    s32 handlesB[8];
    s32 handlesC[5];
    u8 pad78[0x604];
    void *listA;
    void *listB;
    s32 selected;
    s32 pad688;
    s32 fade;
} MenuWindow;

typedef struct ScrollParams {
    s32 a;
    s32 b;
    s32 c;
} ScrollParams;

typedef struct ScrollInner {
    u8 unk0[0x20];
    ScrollParams *params;
} ScrollInner;

typedef struct ScrollHandle {
    u8 unk0[8];
    ScrollInner *inner;
} ScrollHandle;

extern ScrollHandle *func_002BD258(s32);

static inline s64 menuRunPanel(s32 context, u64 mode, u64 arg) {
    return func_00285670(context + 8, (s32 *)(context + 0x54), mode, arg);
}

extern void func_0027FCA0(s32, s32, s32);

extern void func_0027CDD0(s32, s32, s32, s32, s32);

extern void mnuReleaseSpriteTextures(s32);

extern void func_002CFF98(void *);

extern u32 func_0027D4A0(u32);

extern u32 func_0027F730(u32);

extern s32 func_002CFEB8(u32);

extern u32 mnuCreateWindowState(u32, u32, u32, u32);

extern s32 func_0027B888(u32);

extern s32 func_00101A70();

extern s64 func_00285670(s32, s32 *, u64, u64);

void func_00279CC0(s32 menu, s32 target) {
    s32 *entry = (s32 *)menu + **(s32 **)(*(s32 *)(menu + 4) + 0x1C);
    s32 list = entry[2];
    s32 delta = target - *(s32 *)(*(s32 *)(list + 0x14) + 0x24);
    s32 dir;
    s32 n;

    if (delta < 0) {
        dir = -1;
        delta = -delta;
    } else {
        dir = 1;
    }
    if (delta > 0) {
        n = delta;
        do {
            if (dir < 0) {
                mnuReverseListSelection(list, 1);
            }
            if (dir > 0) {
                mnuAdvanceListSelection(list, 1);
            }
            n--;
        } while (n != 0);
    }
    func_0027BF10(*(s32 *)(list + 0x14));
}

INCLUDE_ASM(const s32, "game/code_00279CC0", ptySkillMenuBrowseCandidatePages);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_00279F88);

s64 func_0027A0A8(s32 callback) {
    return menuRunPanel(func_00101A70(), 2, callback);
}

void mnuDrawSelectionLabel(s32 selection) {
    s32 item = func_00197E08(0xCB0, 0xA80, 0, 0, selection & 0xFFFF, 1);
    frFontSetChildColors(item, 0xA09DC366);
    func_001958A0(item, 1, 0x53);
    func_00194920(item);
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027A140);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027A300);

typedef struct PtyFrontlineSlot {
    u16 flags;          /* 0x00: bit 0 present, bit 1 frontline */
} PtyFrontlineSlot;

/* Collect up to max pointers to occupied, frontline party slots. */
void func_0027A468(s32 **out, s32 max) {
    s32 count = 0;
    s32 i = 0;

    while (count < max) {
        s32 entry = D_003BAA00 + i * 0x1A4 + 0xA60;

        *out = 0;
        if ((((PtyFrontlineSlot *)entry)->flags & 1) != 0 &&
            (((PtyFrontlineSlot *)entry)->flags & 2) != 0) {
            *out = (s32 *)entry;
            out++;
            count++;
        }
        i++;
        if (i >= 5) {
            break;
        }
    }
}

s32 mnuHasAvailableSlotResource(s32 id) {
    MenuSlot *entry;
    s32 i;
    id -= 0x1ab;
    entry = (MenuSlot *)((id << 4) + (s32)D_003BAA54);
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

INCLUDE_ASM(const s32, "game/code_00279CC0", ptySkillMenuBuildLinkageSkills);

u32 func_0027A778() {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    temp_v0 = *(s32 *)(temp_v0 + 0x90c);
    func_0027C430(*(u32 *)(temp_v0 + 0x24));
    *(u32 *)(temp_v0 + 0x24) = 0;
    return 1;
}

s32 mnuResetSelection(s32 selection) {
    s32 context;
    s32 menu;
    campMenuInit();
    context = func_00101A70(selection);
    menu = *(s32 *)(context + 0x90C);
    ptySkillMenuBuildLinkageSkills(selection);
    mnuFlagActiveWindows(context + 0x15C);
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

extern void func_002BF4E0(s32, s32, s32, s32, s32, s32, s32, s32);

void mnuDrawListFrames(s32 menu) {
    s32 i;

    for (i = **(s32 **)(menu + 0x164); i < 3; i++) {
        s32 x = 0xF10;

        if (i == 1) {
            x = 0xE70;
        }
        func_002BF4E0(x, i * 0x320 + 0xC8, 0, 0x100, 1, *(s32 *)(menu + 0x6C), 7, 0x53);
    }
}

void campMenuDrawStatus(s32 param) {
    s32 context = func_00101A70();
    s32 menu = *(s32 *)(context + 0x90C);
    s32 slots;
    s32 node;
    s32 label;

    func_00272778(param);
    mnuDrawListFrames(context);
    mnuCreateStaffImageSprite(0x10);
    slots = menu + 4;
    func_0027CDD0(0x1C0, 0x3D0, 0, *(s32 *)(slots + *(s32 *)(menu + 0x2C) * 4 + 0x20), 0x53);
    node = *(s32 *)(*(s32 *)(slots + *(s32 *)(menu + 0x2C) * 4 + 0x20) + 0x14);
    label = *(s32 *)(*(s32 *)(node + 0x1C) + 0x60);
    func_0027A9A8(context);
    if (label != 0) {
        if (label != 0xFFFF) {
            label &= 0xFFFF;
            mnuDrawSelectionLabel(label);
            func_0027A140(label, *(s32 *)(context + 0x68), *(s32 *)(context + 0xE0));
        }
    }
    func_002723B0(2, *(s32 *)(context + 0x78));
    menuRunPanel(context, 1, param);
}

s64 func_0027AC00(s32 callback) {
    return menuRunPanel(func_00101A70(), 2, callback);
}

typedef struct MenuEffectPayload {
    u8 pad0[8];
    u8 *data;
} MenuEffectPayload;

typedef struct MenuAssets {
    u32 sprites[5];
    u32 material;
    MenuEffectPayload *layerA;
    MenuEffectPayload *layerB;
} MenuAssets;

void func_0027AC38(MenuAssets *assets) {
    s32 packet;
    MenuEffectPayload *first;
    MenuEffectPayload *second;

    first = (MenuEffectPayload *)effCreatePayload(2);
    assets->layerA = first;
    second = (MenuEffectPayload *)effCreatePayload(2);
    packet = (s32)second->data;
    assets->layerB = second;
    func_002BDE18(packet + 0x28, assets->material, 0, 0xc);
    func_002BDE18((s32)assets->layerB->data + 0x94, assets->material, 1, 0xc);
    effSetMaterialSlots(assets->sprites[4], 0, 0, (u32)assets->layerB->data);
    effSetMaterialSlots(assets->sprites[4], 1, 0, (s32)assets->layerB->data + 0x6c);
    effSetMaterialSlots(assets->sprites[4], 2, 0, (s32)assets->layerB->data + 0x6c);
    effSetMaterialSlots(assets->sprites[4], 3, 0, (u32)assets->layerB->data);
    effSetMaterialSlots(assets->sprites[4], 4, 0, (u32)assets->layerB->data);
    func_002BDE18((s32)assets->layerA->data + 0x28, assets->material, 2, 0xd);
    func_002BE0C0(assets->sprites[1], 0, (u32)assets->layerA->data);
    func_002BE258(assets->sprites[2], 0, assets->material, 3, 4);
    func_002BE258(assets->sprites[3], 0, assets->material, 4, 4);
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027AD80);

extern char D_003B2330[];

extern char D_003B2348[];

extern u32 D_0037CD18[];

extern u32 D_0037CD20[];

void func_0027AEA8(MenuAssets *assets) {
    effRequestResourceByMode(D_003B2330, D_0037CD18[1], 0, &assets->sprites[0]);
    effRequestResourceByMode(D_003B2330, D_0037CD18[0], 0, &assets->sprites[4]);
    effRequestMappedResource(D_003B2348, D_0037CD20[0], &assets->material);
}

typedef struct MenuColorEntry {
    u8 pad0[0x84];
    u32 color[4];
    u8 pad94[0xC];
} MenuColorEntry;

typedef struct MenuColorSet {
    u8 pad0[0x18];
    MenuColorEntry *entries;
} MenuColorSet;

static inline void menuFillColors(MenuColorEntry *entries) {
    s32 i;
    s32 j;

    for (i = 0; i < 5; i++) {
        u32 *color = entries[i].color;

        for (j = 0; j < 4; j++) {
            *color++ = 0x8080805A;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027AF28);

void mnuReleaseAssets(MenuAssets *assets) {
    u32 i;
    for (i = 0; i < 4; i++) {
        func_002BDD60(assets->sprites[i]);
    }
    func_002BDD60(assets->sprites[4]);
    effDestroyPackedBatch(assets->material);
    func_002BD988(assets->layerA);
    func_002BD988(assets->layerB);
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

extern void func_002BF970(s32, s32);

void mnuDrawCursorIcons(s32 menu, s32 arg) {
    s32 icon = *(s32 *)(menu + 8);
    s32 *state = *(s32 **)(icon + 0x18);

    state[3] = 0x9000;
    state[4] = 0x3F00;
    func_002BF4E0(-0x4800, -0x1F80, 0, 0x50, 0, icon, 0, arg);
    func_002BF970(*(s32 *)(menu + 8), 0);
    icon = *(s32 *)(menu + 0xC);
    state = *(s32 **)(icon + 0x18);
    state[3] = 0x9000;
    state[4] = 0x3F00;
    func_002BF4E0(-0x2800, -0x1180, 0, 0x50, 0, icon, 0, arg);
    func_002BF970(*(s32 *)(menu + 0xC), 0);
}

void mnuDrawBackdrop(s32 *assets, s32 option) {
    func_002C0950(0x30000);
    func_002C0DD8(0, 0, 0, 0x2000, 0xE00, 0x80808080, option);
    func_002BF790(0, 0, 0, 0, assets[0], 0, option);
    mnuDrawCursorIcons(assets, option);
    func_0027B088((s32)assets, option);
}

s32 func_0027B2F8(s32 left, s32 right, s32 size) {
    s32 item = func_002CFF68(0x40);
    *(s32 *)(item + 8) = left;
    *(s32 *)(item + 0xC) = right;
    *(s32 *)(item + 0x28) = size * 8;
    *(s32 *)(item + 0x3C) = 0x100;
    *(s32 *)(item + 0x18) = 0;
    *(s32 *)(item + 0x10) = 0;
    *(s32 *)(item + 0x24) = 0;
    *(s32 *)(item + 0x1C) = 0;
    return item;
}

u32 func_0027B368(u32 arg0) {
    s64 temp_v0;

    do {
        temp_v0 = func_0027B888(arg0);
    } while (temp_v0 != 0);
    func_002CFF98(arg0);
    return 1;
}

struct MenuListNode {
    s32 index;
    s32 value;
    u8 pad8[0x48];
    s32 animationTimer; /* 0x50: stepped down to zero while a list is visible */
    u8 pad54[4];
    struct MenuListNode *next;
    struct MenuListNode *prev;
    u32 sortKeyPrimary;   /* 0x60 */
    u32 sortKeySecondary; /* 0x64 */
    u32 sortKeyTertiary;  /* 0x68 */
    u8 pad6C[8];
};

typedef struct MenuList {
    u32 unk0;
    u32 flags;
    u8 pad8[4];
    s32 visibleCount;
    MenuListNode *first;
    MenuListNode *last;
    MenuListNode *head;
    MenuListNode *cursor;
    s32 count;
    s32 windowOffset;
} MenuList;

void mnuUpdateListScrollFlags(MenuList *list) {
    MenuListNode *node = list->head;
    s32 i;

    if (node == NULL) {
        list->flags = 0;
        return;
    }
    if (node->prev != NULL) {
        list->flags |= 1;
    } else {
        list->flags &= ~1;
    }
    for (i = 0; i < list->visibleCount; i++) {
        node = node->next;
        if (node == NULL) {
            list->flags &= ~2;
            return;
        }
    }
    list->flags |= 2;
}

extern MenuListNode *func_002CFF68(s32);

MenuListNode *mnuListAppendNode(list, value)
    MenuList *list;
    s32 value;
{
    MenuListNode *node = func_002CFF68(0x74);
    s32 index = list->count;
    MenuListNode *last;

    if (index == 0) {
        list->head = node;
        list->cursor = node;
        list->first = node;
    }
    node->prev = list->last;
    node->next = NULL;
    node->value = value;
    last = list->last;
    node->prev = last;
    if (last != NULL) {
        last->next = node;
    }
    node->index = index;
    list->last = node;
    list->count++;
    mnuUpdateListScrollFlags(list);
    if (list->visibleCount < 3) {
        if (list->visibleCount < list->count) {
            list->visibleCount = list->count;
        }
    }
    return node;
}

s32 mnuListContainsFinalNode(MenuList *list) {
    MenuListNode *node = list->head;
    s32 index = 0;
    if (node != NULL) {
        s32 count = list->visibleCount;
        do {
            if (index >= count) {
                return 0;
            }
            if (node == list->last) {
                return 1;
            }
            node = node->next;
            index++;
        } while (node != NULL);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027B540);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027B888);

typedef struct MenuSpriteRef {
    s32 sprite;
    s32 effect;
} MenuSpriteRef;

typedef struct MenuSpriteGrid {
    s32 pad0[2];
    MenuSpriteRef slots[8];
} MenuSpriteGrid;

void func_0027BA00(MenuSpriteGrid *grid, s32 row, s32 col, s32 x, s32 y, s32 sprite, s32 effect) {
    grid->slots[row * 4 + col].sprite = sprite;
    grid->slots[row * 4 + col].effect = effect;
    func_002BF9E0(sprite, effect, x, y, x, y);
}

void *mnuWalkNodeList(s32 targetIndex, MenuList *list) {
    MenuListNode *node = list->first;
    s32 index = 0;

    if (node != NULL && targetIndex != index) {
        do {
            node = node->next;
            index++;
        } while (node != NULL && index != targetIndex);
    }
    return node;
}

s32 mnuSeekListNode(s32 index, MenuList *list) {
    s32 size = list->count;

    if (index >= size) {
        return 0;
    }
    list->windowOffset = 0;
    list->head = list->first;
    list->cursor = list->first;
    if (index > 0) {
        do {
            if (size - list->head->index <= list->visibleCount) {
                list->windowOffset += 1;
            } else {
                list->head = list->head->next;
            }
            list->cursor = list->cursor->next;
            index--;
        } while (index != 0);
    }
    return 1;
}

void func_0027BB08(MenuList *list) {
    mnuSeekListNode(0, list);
}

void func_0027BB28(MenuList *list) {
    mnuSeekListNode(list->count - 1, list);
}

/* Advance the visible head when a prior window offset can be reduced. */
s32 func_0027BB48(MenuList *list) {
    MenuListNode *cursor = list->cursor;
    MenuListNode *last = list->last;
    MenuListNode *head = list->head;

    if (cursor == last) {
        return (s32)cursor;
    }
    head = head->next;
    if (head == NULL) {
        return (s32)cursor;
    }
    list->head = head;
    list->windowOffset--;
    return (s32)cursor;
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

/* Cancel the pending animation on every node in this list. */
void func_0027BF10(MenuList *list) {
    MenuListNode *node;

    node = list->first;
    if (node != 0) {
        node->animationTimer = 0;
        while (node = node->next, node != 0) {
            node->animationTimer = 0;
        }
    }
}

/* Tick every node's animation down in units of 16, clamping at zero. */
void func_0027BF48(MenuList *list) {
    MenuListNode *node = list->first;
    if (node != NULL) {
        do {
            s32 timer = node->animationTimer;
            s32 reduced = timer - 0x10;
            if (timer > 0) {
                node->animationTimer = reduced;
                timer = reduced;
            }
            if (timer < 0) {
                node->animationTimer = 0;
            }
            node = node->next;
        } while (node != NULL);
    }
}

void mnuDrawFourEntries(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 base8 = arg4 + 8;
    s32 baseC = arg4 + 0xC;
    u32 i = 0;
    do {
        s32 eq = arg4 == *(s32 *)(arg3 + 0x1C);
        s32 off = (eq * 4 + i) * 8;
        s32 value = *(s32 *)(base8 + off);
        if (value != 0) {
            func_002BF790(arg0, arg1, arg2, 0, value, *(s32 *)(baseC + off), arg5);
        }
        i++;
    } while (i < 4);
}

s32 mnuDispatchByFlag(s32 arg0, s32 arg1) {
    return func_002C1630((*(s32 *)(arg1 + 0x48) & 1) ? 0x89BDC940 : 0x89BDC980,
                         arg0, *(s32 *)(arg1 + 0x50));
}

typedef struct MenuSlotEntry {
    u8 pad0[0x14];
    s32 word[4];
    u8 pad24[0x7C];
} MenuSlotEntry;

typedef struct MenuSlotSet {
    u8 pad0[0x18];
    MenuSlotEntry *entries;
} MenuSlotSet;

void func_0027C0B0(MenuSlotSet *menu, s32 index, s32 node) {
    s32 j;

    for (j = 0; j < 4; j++) {
        s32 word = menu->entries[index].word[j];

        menu->entries[index].word[j] = mnuDispatchByFlag(word, node);
    }
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027C140);

void mnuCallInitWide(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_0027C140(arg0, arg1, arg2, 0, 0, 0x100, 0, arg3, arg4);
}

s32 mnuCreateWindowContainer(s32 id, s32 width, s32 height, s32 left, s32 right) {
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
        mnuReleaseWindowTextures(temp_v0);
    }
    func_002CFF98(arg0);
}

void func_0027C470(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x3c) = arg1;
}

void func_0027C478(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x88) = arg1;
}

void func_0027C480(u8 *panel, s32 x, s32 y, u32 sprite,
                   u32 effect, u32 color) {
    *(s32 *)(panel + 0x2c) = x;
    *(s32 *)(panel + 0x30) = y;
    func_002BF9E0(x, y, -0x70, -0x68, -0x70, -0x68);
    *(u32 *)(panel + 0x34) = sprite;
    *(u32 *)(panel + 0x38) = effect;
    if (sprite != 0) {
        func_002BF9E0(sprite, effect, 0x60, -0xd0, 0, 0);
    }
    *(u32 *)(panel + 0x3c) = sprite;
    *(u32 *)(panel + 0x40) = color;
    if (sprite != 0) {
        func_002BF9E0(sprite, color, 0x60, -0xd0, 0, 0);
    }
}

void mnuForwardDupArg(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_0027C480(arg0, arg1, arg2, arg3, arg4, arg4);
}

void func_0027C570(s32 arg0, s32 *arg1, s32 arg2, s32 arg3, s32 arg4) {
    arg1[6] = arg0;
    arg1[7] = arg2;
    arg1[9] = arg3 + 3;
    arg1[8] = arg3;
    arg1[10] = arg4;
}

void func_0027C590(u8 *panel, const void *layout, u32 left, u32 top,
                   u32 right, u32 bottom) {
    memcpy(panel + 0x4c, layout, 0x38);
    *(u32 *)(panel + 0x70) = left;
    *(u32 *)(panel + 0x74) = top;
    *(u32 *)(panel + 0x78) = right;
    *(u32 *)(panel + 0x7c) = bottom;
    *(u32 *)(panel + 4) |= 4;
}

void func_0027C620(s32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4) {
    u32 temp_v0;

    temp_v0 = mnuCreateWindowState(arg1, arg2, arg3, arg4);
    *(u32 *)(arg0 + 0x84) = temp_v0;
}

void func_0027C658(s32 arg0) {
    *(u32 *)(arg0 + 4) = *(u32 *)(arg0 + 4) & 0xfffffffb;
}

void func_0027C670(s32 arg0) {
    mnuListAppendNode(*(u32 *)(arg0 + 0x14));
}

void func_0027C688(s32 arg0) {
    func_0027B540(*(u32 *)(arg0 + 0x14));
}

void func_0027C6A0(s32 arg0) {
    func_0027B888(*(u32 *)(arg0 + 0x14));
}

s32 mnuAdvanceListSelection(s32 window, s32 direction) {
    s32 item = func_0027BBF0(*(s32 *)(window + 0x14), direction, 0);
    if (item != 0) {
        *(u8 *)(item + 0x54) = 0;
        mnuClearEntryFlags(window + 0x4c);
    }
    return item;
}

s32 mnuReverseListSelection(s32 window, s32 direction) {
    s32 item = func_0027BD48(*(s32 *)(window + 0x14), direction, 0);
    if (item != 0) {
        *(u8 *)(item + 0x54) = 0;
        mnuClearEntryFlags(window + 0x4c);
    }
    return item;
}

void func_0027C758(u32 arg0) {
    mnuAdvanceListSelection(arg0, 0);
}

void func_0027C770(u32 arg0) {
    mnuReverseListSelection(arg0, 0);
}

void func_0027C788(s32 arg0) {
    func_0027BED0(*(u32 *)(arg0 + 0x14));
}

void func_0027C7A0(s32 arg0) {
    func_0027BEF0(*(u32 *)(arg0 + 0x14));
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027C7B8);

void func_0027CA78(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_0027C7B8();
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027CA90);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027CCD0);

void func_0027CDD0(s32 arg0, s32 arg1, s32 arg2, s32 menu, s32 param) {
    s32 texture = *(s32 *)(menu + 0x88);
    s32 count;

    *(s32 *)(*(s32 *)(menu + 0x14) + 0x3C) = texture;
    func_0027CA90();
    func_0027CA78(arg0, arg1, arg2, menu, param);
    if (*(s32 *)(*(s32 *)(menu + 0x14) + 0x20) != 0) {
        func_0027CCD0(arg0, arg1, arg2, menu, param);
    }
    func_0027C140(arg0, arg1, arg2, *(s32 *)(menu + 8), *(s32 *)(menu + 0xC), texture, *(s32 *)(menu + 4),
                  *(s32 *)(menu + 0x14), param);
    count = *(s32 *)(menu + 0x84);
    if (count != 0) {
        func_0027D318(arg0, arg1, arg2, *(s32 *)(*(s32 *)(menu + 0x14) + 4), count, param);
    }
    count = *(s32 *)(menu + 0x88);
    if (count < 0x100) {
        *(s32 *)(menu + 0x88) = count + 0x20;
    }
    *(u32 *)(menu + 4) |= 4;
}

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

extern s32 func_002D03F8(s32);

extern s32 *sdfResourceRetainAddress(s32);

extern void func_0027CF28(s32 *, u32, u32, u32, u32);

typedef struct MenuWindowSpriteGroup {
    s32 resourceHandle;
    u8 pad4[8];
    s32 sprites[7];
} MenuWindowSpriteGroup;

u32 mnuCreateWindowState(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    s32 handle = func_002D03F8(sizeof(MenuWindowSpriteGroup));
    MenuWindowSpriteGroup *group = (MenuWindowSpriteGroup *)sdfResourceRetainAddress(handle);

    memset(group, 0, sizeof(MenuWindowSpriteGroup));
    group->resourceHandle = handle;
    func_0027CF28((s32 *)group, arg0, arg1, arg2, arg3);
    return (u32)group;
}

void mnuReleaseWindowTextures(MenuWindowSpriteGroup *group) {
    u32 i;
    for (i = 0; i < 7; i++) {
        func_002BDD60(group->sprites[i]);
    }
    func_002D0918(group->resourceHandle);
}

void func_0027D248(MenuWindowSpriteGroup *group, u32 arg1) {
    effConfigureWithDefaultSetting(group->sprites[1], 0, arg1, 0, 0x14, 0xc);
    effConfigureWithDefaultSetting(group->sprites[2], 0, arg1, 1, 10, 0xc);
    effConfigureWithDefaultSetting(group->sprites[3], 0, arg1, 2, 0, 0xc);
    effConfigureWithDefaultSetting(group->sprites[4], 0, arg1, 2, 0, 0xc);
    effConfigureWithDefaultSetting(group->sprites[5], 0, arg1, 1, 10, 0xc);
    effConfigureWithDefaultSetting(group->sprites[6], 0, arg1, 0, 0x14, 0xc);
}

void func_0027D318(s32 x, s32 y, s32 z, s32 mask, MenuWindowSpriteGroup *group, s32 param) {
    func_002BF790(x, y, z, 0, group->sprites[0], 0, param);
    if (mask & 1) {
        func_002BF790(x, y, z, 0, group->sprites[1], 0, param);
        func_002BF790(x, y, z, 0, group->sprites[2], 0, param);
        func_002BF790(x, y, z, 0, group->sprites[3], 0, param);
    }
    if (mask & 2) {
        func_002BF790(x, y, z, 0, group->sprites[4], 0, param);
        func_002BF790(x, y, z, 0, group->sprites[5], 0, param);
        func_002BF790(x, y, z, 0, group->sprites[6], 0, param);
    }
    func_002BF970(group->sprites[1], 0);
    func_002BF970(group->sprites[2], 0);
    func_002BF970(group->sprites[3], 0);
    func_002BF970(group->sprites[4], 0);
    func_002BF970(group->sprites[5], 0);
    func_002BF970(group->sprites[6], 0);
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027D4A0);

extern void func_002BD5A0(void *, s32);

void mnuClearEntryFlags(u32 *group) {
    s32 i;

    if (group[3] != 0 && group[0] < 3) {
        for (i = 0; i < (s32)group[2]; i++) {
            u32 *entry = (u32 *)group[3 + i];
            u32 *flags = (u32 *)(entry[6] + 0x28);

            *flags &= ~1;
            func_002BD5A0(entry, 0);
        }
    }
}

void mnuReleaseResourceList(s32 *object) {
    s32 i;
    s32 count = object[2];
    for (i = 0; i < count; i++) {
        if (object[i + 3] != 0) {
            func_002BDD60(object[i + 3]);
            count = object[2];
        }
    }
    func_002CFF98(object);
}

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

void menuHideWindowHandles(u32 obj) {
    switch (*(u32 *)obj) {
    case 0:
        func_002BF970(*(s32 *)(obj + 0x1C), 0);
        func_002BF970(*(s32 *)(obj + 0x20), 0);
        return;
    case 1:
        func_002BF970(*(s32 *)(obj + 0xC), 0);
        func_002BF970(*(s32 *)(obj + 0x10), 0);
        func_002BF970(*(s32 *)(obj + 0x14), 0);
        func_002BF970(*(s32 *)(obj + 0x18), 0);
        return;
    case 2:
        func_002BF970(*(s32 *)(obj + 0x14), 0);
        func_002BF970(*(s32 *)(obj + 0x18), 0);
        break;
    }
}

/* Rebuild the first-node pointer by walking backward from the cursor. */
void func_0027DE60(MenuList *list) {
    MenuListNode *node;
    MenuListNode *first;
    MenuListNode *previous;

    previous = list->cursor;
    first = list->cursor;
    while (node = previous, node != 0) {
        first = node;
        previous = node->prev;
    }
    list->first = first;
}

/* Rebuild the last-node pointer by walking forward from the cursor. */
void func_0027DE98(MenuList *list) {
    MenuListNode *node;
    MenuListNode *last;
    MenuListNode *next;

    next = list->cursor;
    last = list->cursor;
    while (node = next, node != 0) {
        last = node;
        next = node->next;
    }
    list->last = last;
}

/* Reset the cursor to the beginning, optionally replaying its old position. */
void func_0027DED0(MenuList *list, s32 restoreOffset) {
    MenuListNode *first;
    MenuListNode *oldCursor;
    list->windowOffset = 0;
    first = list->first;
    oldCursor = list->cursor;
    list->head = first;
    list->cursor = first;
    if (restoreOffset == 1) {
        MenuListNode *node = first;
        if (node == NULL) {
            return;
        }
        do {
            if (node == oldCursor) {
                return;
            }
            func_0027BE90((u32)list);
            node = node->next;
        } while (node != NULL);
    }
}

void menuLinkItemList(MenuListNode **items, s32 count) {
    s32 i;

    items[0]->prev = NULL;
    items[0]->next = items[1];
    for (i = 1; i < count - 1; i++) {
        items[i]->prev = items[i - 1];
        items[i]->next = items[i + 1];
    }
    items[count - 1]->prev = items[count - 2];
    items[count - 1]->next = NULL;
    for (i = 0; i < count; i++) {
        items[i]->index = i;
    }
}

s32 mnuComparePrimaryKeyDescending(MenuListNode **left, MenuListNode **right) {
    u32 leftKey = (*left)->sortKeyPrimary;
    u32 rightKey = (*right)->sortKeyPrimary;

    if (rightKey < leftKey) {
        return -1;
    }
    return leftKey < rightKey;
}

s32 mnuComparePrimaryKeyAscending(MenuListNode **left, MenuListNode **right) {
    u32 leftKey = (*left)->sortKeyPrimary;
    u32 rightKey = (*right)->sortKeyPrimary;

    if (rightKey < leftKey) {
        return 1;
    }
    return (leftKey < rightKey) ? -1 : 0;
}

s32 mnuCompareSecondaryKeyDescending(MenuListNode **left, MenuListNode **right) {
    u32 leftKey = (*left)->sortKeySecondary;
    u32 rightKey = (*right)->sortKeySecondary;

    if (rightKey < leftKey) {
        return -1;
    }
    return leftKey < rightKey;
}

s32 mnuCompareSecondaryKeyAscending(MenuListNode **left, MenuListNode **right) {
    u32 leftKey = (*left)->sortKeySecondary;
    u32 rightKey = (*right)->sortKeySecondary;

    if (rightKey < leftKey) {
        return 1;
    }
    return (leftKey < rightKey) ? -1 : 0;
}

s32 mnuCompareTertiaryKeyDescending(MenuListNode **left, MenuListNode **right) {
    u32 leftKey = (*left)->sortKeyTertiary;
    u32 rightKey = (*right)->sortKeyTertiary;

    if (rightKey < leftKey) {
        return -1;
    }
    return leftKey < rightKey;
}

s32 mnuCompareTertiaryKeyAscending(MenuListNode **left, MenuListNode **right) {
    u32 leftKey = (*left)->sortKeyTertiary;
    u32 rightKey = (*right)->sortKeyTertiary;

    if (rightKey < leftKey) {
        return 1;
    }
    return (leftKey < rightKey) ? -1 : 0;
}

INCLUDE_RODATA(const s32, "game/code_00279CC0", D_003B2330);

INCLUDE_RODATA(const s32, "game/code_00279CC0", D_003B2348);

INCLUDE_RODATA(const s32, "game/code_00279CC0", D_003B2358);

INCLUDE_RODATA(const s32, "game/code_00279CC0", D_003B2368);

INCLUDE_RODATA(const s32, "game/code_00279CC0", D_003B2380);

INCLUDE_RODATA(const s32, "game/code_00279CC0", D_003B23A0);

void menuSortItems(s32 menu, s32 sortKey, s32 descending) {
    s32 (*comparators[6])(MenuListNode **, MenuListNode **) = {
        mnuComparePrimaryKeyDescending, mnuCompareSecondaryKeyDescending, mnuCompareTertiaryKeyDescending,
        mnuComparePrimaryKeyAscending, mnuCompareSecondaryKeyAscending, mnuCompareTertiaryKeyAscending
    };
    s32 count = 0;
    s32 handle = func_002D03F8(((MenuList *)menu)->count * 4);
    MenuListNode **items = (MenuListNode **)sdfResourceRetainAddress(handle);
    MenuListNode **out = items;
    MenuListNode *node;

    for (node = ((MenuList *)menu)->first; node != NULL; node = node->next) {
        *out++ = node;
        count++;
    }
    if (descending != 0) {
        sortKey += 3;
    }
    func_00300508(items, count, 4, comparators[sortKey]);
    menuLinkItemList(items, count);
    func_0027DE60(menu);
    func_0027DE98(menu);
    func_0027DED0((s32 *)menu, 0);
    func_002D0918(handle);
}

void mnuAllocateListEntries(s32 *list) {
    u32 i;
    for (i = 0; i < 4; i++) {
        list[i + 1] = func_002CFF68(0x18);
    }
}

void freeMenuListEntries(s32 *list) {
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

typedef struct MenuFadeEntry {
    u32 active;
    u32 pad4[3];
    void *handle;
    u32 pad14;
} MenuFadeEntry;

void func_0027E2F0(s32 *list, u32 index) {
    s32 *entries;
    s32 *slot;
    u32 i;

    if (index >= 5) {
        entries = list + 1;
        slot = entries + index;
        if (((MenuFadeEntry *)*slot)->active != 0) {
            func_0027C430(((MenuFadeEntry *)*slot)->handle);
        }
        i = list[0] - 1;
        ((MenuFadeEntry *)*slot)->handle = 0;
        for (; index < i; i--) {
            *(MenuFadeEntry *)entries[i - 1] = *(MenuFadeEntry *)entries[i];
            ((MenuFadeEntry *)entries[i])->handle = 0;
        }
        list[0]--;
    }
}

void func_0027E3D8(s32 image, s32 *list, s32 option) {
    u32 i;
    for (i = 0; i < (u32)list[0]; i++) {
        s32 *entry = (s32 *)list[i + 1];
        func_0027CDD0(entry[2], entry[3], image, entry[4], option);
    }
}

void mnuUpdateFade(s32 *list) {
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

void mnuInitScrollHandles(u8 *menu) {
    ScrollHandle *handle;

    handle = func_002BD258(1);
    *(ScrollHandle **)(menu + 0x40) = handle;
    handle->inner->params->a = 10;
    handle->inner->params->b = 0;

    handle = func_002BD258(3);
    *(ScrollHandle **)(menu + 0x44) = handle;
    handle->inner->params->a = 8;
    handle->inner->params->b = 4;
    handle->inner->params->c = 8;

    handle = func_002BD258(1);
    *(ScrollHandle **)(menu + 0x48) = handle;
    handle->inner->params->a = 10;
    handle->inner->params->b = 0;
}

void func_0027E570(s32 *list) {
    u32 i;
    for (i = 0; i < 3; i++) {
        effDestroyPackedBatch(list[i + 16]);
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
    mnuInitScrollHandles(temp_v0);
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

void func_0027E6F0(u32 *menu) {
    u32 *source = menu + 9;
    u32 *dest = menu + 12;
    s32 remaining = 1;
    do {
        u32 previous = *dest;
        u32 next = source[4];
        dest[-4] = previous;
        *source = next;
        *dest = 0;
        source += 2;
        dest += 2;
    } while (--remaining >= 0);
    if (menu[8] != 0) {
        func_002BF9E0(menu[8], menu[9], 0, 0, 0x400, 0);
        effConfigureWithDefaultSetting(menu[8], menu[9], menu[18],
                                          0, 10, 2);
    }
}

void func_0027E790(u32 *menu, s32 x, s32 y, s32 color) {
    func_0027E6F0(menu);
    menu[12] = x;
    menu[13] = y;
    menu[14] = x;
    menu[15] = color;
    func_002BF9E0(x, y, 0, 0, -0x400, 0);
    func_002BE258(x, y, menu[16], 0, 3);
    func_002BF9E0(x, color, 0, 0, 0, 0);
    effConfigureWithDefaultSetting(x, color, menu[17], 0, 10, 0);
}

u8 func_0027E850(s32 arg0) {
    return *(s32 *)(arg0 + 0x20) != 0;
}

void func_0027E860(u8 *panel, s32 y, s32 unknown,
                   u32 *sprite, s32 flag) {
    func_002C1380(flag);
    func_002BF790((s32)(panel + 0x10), y + 0xf8, 0xffffff, 1,
                   sprite[6], sprite[7], flag);
    func_002C1430(flag);
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027E8D8);

void *func_0027EAF0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    u8 *item = (u8 *)func_002CFEB8(0x3C);

    memset(item, 0, 0x3C);
    *(s32 *)(item + 0xC) = effCreateResourceSlotSet(arg1, 0, 1);
    *(s32 *)(item + 0x10) = effCreateResourceSlotSet(arg1, 1, 1);
    *(s32 *)(item + 0x14) = effCreateResourceSlotSet(arg0, 2, 1);
    *(s32 *)(item + 0x18) = effCreateResourceSlotSet(arg0, 5, 1);
    *(s32 *)(item + 0x1C) = effCreateResourceSlotSet(arg0, 6, 1);
    *(s32 *)(item + 0x20) = effCreateResourceSlotSet(arg0, 7, 1);
    *(s32 *)(item + 0x24) = effCreateResourceSlotSet(arg0, 8, 1);
    *(s32 *)(item + 0x28) = effCreateResourceSlotSet(arg0, 0xA, 1);
    *(s32 *)(item + 0x30) = effCreateResourceSlotSet(arg2, arg3, 1);
    *(s32 *)(item + 0x2C) = effCreateResourceSlotSet(arg4, arg5, 1);
    return item;
}

void mnuDestroyResources(s32 *object) {
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

void func_0027EF18(u8 *menu, s32 x, s32 y, s32 style, s32 color) {
    u32 *output = (u32 *)(menu + 0x154);
    u32 i = 0;
    s32 offset = 0;
    for (; i < 5; i++) {
        s32 entry = *(s32 *)(*(u32 *)(menu + 8) + offset + 0x10);
        offset += 0x34;
        if (entry >= 0) {
            *output = func_0027EAF0(x, y, style, 0, color, entry);
        }
        output = (u32 *)((u8 *)output + 0x134);
    }
}

void mnuReleaseSlotResources(s32 context) {
    s32 *slot = (s32 *)(context + 0x154);
    u32 i = 0;
    s32 offset = 0;
    for (; i < 5; i++) {
        s32 node = *(s32 *)(*(s32 *)(context + 8) + offset + 0x10);
        offset += 0x34;
        if (node >= 0 && *slot != 0) {
            mnuDestroyResources(*slot);
            *slot = 0;
        }
        slot = (s32 *)((u8 *)slot + 0x134);
    }
}

void func_0027F050(s32 arg0, u32 arg1, u32 arg2, u32 arg3, s32 arg4
                                    ) {
    u32 temp_v0;

    temp_v0 = effCreateResourceSlotSet(arg1, arg2, 1);
    *(u32 *)(arg0 + 0xe8) = temp_v0;
    temp_v0 = effCreateResourceSlotSet(arg1, arg3, 1);
    *(u32 *)(arg0 + 0xec) = temp_v0;
    if (-1 < arg4) {
        temp_v0 = effCreateResourceSlotSet(arg1, arg4, 1);
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

typedef struct MenuPageMotion {
    u8 unk0[0x40];
    s32 field40;
    s32 field44;
    s32 field48;
    s32 field4C;
} MenuPageMotion;

void mnuSetPageParams(MenuPageMotion *page, s32 mode) {
    switch (mode) {
    case 0:
        page->field44 = 0;
        page->field40 = 0;
        page->field48 = 0x40;
        page->field4C = 0x100;
        break;
    case 1:
        page->field44 = 1;
        page->field40 = 0x100;
        page->field48 = 0;
        page->field4C = 0x1000;
        break;
    default:
        page->field44 = 0;
        page->field40 = 0x100;
        page->field48 = 0;
        page->field4C = 0;
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027F230);

void func_0027F4B0(u32 *menu) {
    u32 *entry;
    u32 *base;
    u32 i;
    entry = menu + 4;
    for (i = 0; i < 2; i++) {
        func_002BDD60(*entry++);
    }
    i = 0;
    base = menu + 2;
    entry = base + 4;
    for (; i < 4; i++) {
        func_002BDD60(*entry++);
    }
    entry = base + 8;
    for (i = 0; i < 3; i++) {
        func_002BDD60(*entry++);
    }
    i = 0;
    entry = menu + 13;
    for (; i < 2; i++) {
        func_002BDD60(*entry++);
    }
    func_002CFF98(menu);
}

void func_0027F588(s32 arg0, s32 arg1, s32 arg2, s32 skip, s32 *menu, s32 param) {
    u32 i;

    if (skip == 0) {
        func_002BF4E0(0, 0, arg2, menu[15], 0, menu[4], 0, param);
        for (i = 0; i < 4; i++) {
            func_002BF4E0(0, 0, arg2, menu[15], 0, menu[6 + i], 0, param);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027F638);

void mnuClearEntries(s32 *menu) {
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

u32 func_0027F730(u32 arg0) {
    u8 *item = (u8 *)func_002CFEB8(0x24);
    s32 sprite;

    memset(item, 0, 0x24);
    sprite = effCreateResourceSlotSet(arg0, 0x1F, 1);
    *(s32 *)(item + 0xC) = sprite;
    func_002BF9E0(sprite, 0, 0, 0x40, 0, 0);
    sprite = effCreateResourceSlotSet(arg0, 0x1E, 1);
    *(s32 *)(item + 0x10) = sprite;
    func_002BF9E0(sprite, 0, 0x5E0, -0x20, 0, 0);
    sprite = effCreateResourceSlotSet(arg0, 7, 1);
    *(s32 *)(item + 0x14) = sprite;
    func_002BF9E0(sprite, 0, 0x5E0, -0x18, 0, 0);
    sprite = effCreateResourceSlotSet(arg0, 0x1D, 1);
    *(s32 *)(item + 0x18) = sprite;
    func_002BF9E0(sprite, 0, 0xB40, 0x40, 0, 0);
    return (u32)item;
}

void mnuReleaseFourResourceList(s32 *list) {
    u32 i;
    for (i = 0; i < 4; i++) {
        func_002BDD60(list[i + 3]);
    }
    func_002CFF98(list);
}

typedef struct MenuFadeSpriteSet {
    u8 pad0[0xC];
    s32 sprites[4];
    s32 alpha;
    s32 fadeOut;
} MenuFadeSpriteSet;

void func_0027F898(s32 x, s32 y, s32 z, s32 unused, MenuFadeSpriteSet *sprites, s32 param) {
    s32 px = x + 0x120;
    s32 py = y + 0x30;
    s32 alpha = sprites->alpha;
    s32 fade;
    s32 nextAlpha;
    s32 lowerAlpha;

    func_002BF4E0(px, py, z, alpha, 0, sprites->sprites[0], 0, param);
    func_002BF4E0(px, py, z, alpha, 0, sprites->sprites[1], 0, param);
    func_002BF4E0(px, py, z, alpha, 0, sprites->sprites[2], 0, param);
    func_002BF4E0(px, py, z, alpha, 0, sprites->sprites[3], 0, param);
    if (sprites->fadeOut == 0) {
        fade = sprites->alpha;
        nextAlpha = fade + 0x10;
        if (fade < 0x100) {
            sprites->alpha = nextAlpha;
            fade = nextAlpha;
        }
        if (fade >= 0x101) {
            sprites->alpha = 0x100;
        }
    } else {
        fade = sprites->alpha;
        lowerAlpha = fade - 0x10;
        if (fade > 0) {
            sprites->alpha = lowerAlpha;
            fade = lowerAlpha;
        }
        if (fade < 0) {
            sprites->alpha = 0;
        }
    }
}

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
            mnuReleaseFourResourceList(*item);
            *item = 0;
        }
    }
}

s32 func_0027FA70(s32 value, s32 total) {
    if (total > 0) {
        return value * 100 / total;
    }
    return 100;
}

typedef struct MenuGaugeRow {
    s32 id;
    u8 pad4[4];
    s32 hp;
    s32 mp;
    s32 maxHp;
    s32 maxMp;
    u8 pad18[0x1C];
} MenuGaugeRow;

static inline MenuGaugeRow *menuGauges(MenuWindow *window) {
    return (MenuGaugeRow *)((u8 *)window->records + 0x10);
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027FAA8);

void func_0027FB90(s32 window) {
    u32 i;
    for (i = 0; i < 5; i++, window += 0x134) {
        mnuReleaseSpriteTextures(window + 0x94);
        mnuReleaseSpriteTextures(window + 0xe8);
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

extern void effResolveAndReleaseResource(s32);

void mnuRegisterResourceHandles(s32 destination, s32 *source) {
    u32 i;
    for (i = 0; i < 5; i++) {
        effResolveAndReleaseResource(source[i]);
        *(s32 *)(destination + 0x64 + 4 * i) = source[i];
    }
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_0027FCA0);

void menuUpdateHandleStates(s32 obj) {
    s32 *handle = (s32 *)(obj + 0x24);
    s32 i;
    s32 offset;

    for (i = 0; i < 8U; i++, handle++) {
        if (func_002BD8F8(*handle) != 0) {
            func_002BD870(*handle);
            func_002BD870(handle[8]);
        }
    }
    for (i = 0, offset = 0; i < 5U; i++) {
        s32 entry = *(s32 *)(obj + 8) + offset;

        offset += 0x34;
        if (*(s32 *)(entry + 0x10) >= 0) {
            if (i < **(s32 **)(obj + 8)) {
                func_0027FCA0(obj, i, 1);
            } else {
                func_0027FCA0(obj, i, 2);
            }
        } else {
            func_0027FCA0(obj, i, 0);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_00280048);

extern char D_003BC728[];

void mnuFillPanelLists(s32 menu, s32 *counts) {
    s32 i = 0;

    *(void **)(menu + 0x67C) = (void *)func_0027B2F8(0, 1, 1);
    *(void **)(menu + 0x680) = (void *)func_0027B2F8(0, 1, 1);
    for (i = 0; i < counts[0] + counts[1]; i++) {
        mnuListAppendNode(*(void **)(menu + 0x67C), D_003BC728);
        mnuListAppendNode(*(void **)(menu + 0x680), D_003BC728);
    }
}

void func_00280228(s32 arg0) {
    func_0027B368(*(u32 *)(arg0 + 0x67c));
    func_0027B368(*(u32 *)(arg0 + 0x680));
}

void func_00280258(s32 arg0, s32 arg1) {
    func_00280228(arg0);
    mnuFillPanelLists(arg0, arg1);
}

void func_00280290(MenuWindow *window) {
    if (window->selected >= 0) {
        MenuPage *pages = (MenuPage *)((u8 *)window + 0x20);

        pages[window->selected].scaleA = 0x100;
        pages[window->selected].scaleB = 0x100;
        window->selected = -1;
    }
    window->flags &= ~0x400;
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_002802E0);

void menuReleaseHandles(s32 obj) {
    s32 *handle = (s32 *)(obj + 0x10);
    u32 i;

    for (i = 0; i < 3; i++, handle++) {
        if (*handle != 0) {
            func_002BDD60(*handle);
        }
    }
    if (*(s32 *)(obj + 0xC4) != 0) {
        func_002BDD60(*(s32 *)(obj + 0xC4));
    }
    if (*(s32 *)(obj + 0xC8) != 0) {
        func_002BDD60(*(s32 *)(obj + 0xC8));
    }
    if (*(s32 *)(obj + 0xCC) != 0) {
        func_002BDD60(*(s32 *)(obj + 0xCC));
    }
    if (*(s32 *)(obj + 0xD0) != 0) {
        func_002BDD60(*(s32 *)(obj + 0xD0));
    }
    if (*(s32 *)(obj + 0xD4) != 0) {
        func_002BDD60(*(s32 *)(obj + 0xD4));
    }
    if (*(s32 *)(obj + 0xD8) != 0) {
        func_002BDD60(*(s32 *)(obj + 0xD8));
    }
}

extern void menuReleaseHandles(s32);

void mnuShutdownContext(s32 context) {
    u32 i;
    for (i = 0; i < 5; i++) {
        menuReleaseHandles(context + 0x78 + i * 0x134);
    }
    func_0027FB90(context);
    func_00280228(context);
}

void func_002804F0(MenuWindow *window) {
    s32 selected = window->selected;
    s32 offset = 0;
    u32 i;

    for (i = 0; i < 5; i++, offset += sizeof(MenuRecord)) {
        if (i != selected) {
            s32 id = ((MenuRecord *)((u8 *)window->records + offset))->gauge.id;

            if (id >= 0) {
                if (func_002BD8F8(window->handlesA[id]) == 0) {
                    effResolveAndReleaseResource(window->handlesA[id]);
                    effResolveAndReleaseResource(window->handlesB[id]);
                }
            }
        }
    }
}

void func_002805B0(MenuWindow *window) {
    s32 selected = window->selected;
    u32 i;
    s32 id;
    MenuRecord *record;

    for (i = 0; i < 5; i++) {
        record = &window->records[i];
        id = record->gauge.id;
        if (id >= 0) {
            if (func_002BD8F8(window->handlesA[id]) != 0) {
                func_002BD870(window->handlesA[id]);
                func_002BD870(window->handlesB[id]);
            }
        }
    }
    record = &window->records[selected];
    id = record->gauge.id;
    if (id >= 0) {
        if (func_002BD8F8(window->handlesA[id]) == 0) {
            effResolveAndReleaseResource(window->handlesA[id]);
            effResolveAndReleaseResource(window->handlesB[id]);
        }
    }
}

typedef struct MenuHandleSet {
    u8 pad0[0x20];
    s32 a[8];
    s32 b[8];
    s32 c[5];
} MenuHandleSet;

void func_002806E8(MenuWindow *window, s32 selected) {
    s32 *resource = window->handlesC;
    u32 i;
    MenuHandleSet *handles = (MenuHandleSet *)((u8 *)window + 4);
    MenuRecord *record;
    s32 active;

    for (i = 0; i < 5; i++) {
        func_002BD870(*resource++);
    }
    record = &window->records[selected];
    active = mnuGetSelectionFromFlags(D_003BAA00 + record->partyIndex * 0x1A4 + 0xA60);
    for (i = 0; i < 5; i++) {
        if (i == active) {
            effResolveAndReleaseResource(handles->c[i]);
        }
    }
    if (window->selected >= 0) {
        func_002804F0(window);
    }
    window->selected = selected;
    func_002805B0(window);
}

void func_002807E8(window)
    MenuWindow *window;
{
    s32 *resource = window->handlesC;
    u32 i;

    for (i = 0; i < 5; i++) {
        effResolveAndReleaseResource(*resource++);
    }
    if (window->selected >= 0) {
        func_002804F0(window);
    }
    func_00280290(window);
}

void mnuFlagActiveWindows(u8 *menu) {
    u8 *kind = menu + 8;
    u8 *flags = menu + 12;
    u32 i = 0;
    s32 activeKind = 2;
    s32 offset = 0x70;
    do {
        if (*(s32 *)(kind + offset) == activeKind) {
            *(u32 *)(flags + offset) |= 1;
        }
        i++;
        offset += 0x134;
    } while (i < 5);
}

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

void mnuClearListFlags(s32 which, s32 *menu) {
    mnuSeekListNode(0, menu[0x67C / 4 + which]);
    if (which == 0) {
        *menu &= ~2;
        *menu &= ~4;
        *menu &= ~8;
        *menu &= ~0x10;
        *menu &= ~0x20;
    } else {
        *menu &= ~2;
        *menu &= ~8;
        *menu &= ~0x10;
        *menu &= ~0x20;
    }
}

extern u32 func_00285B20(u32);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_00280978);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_00280A90);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_00280BC0);

s32 func_00280D98(s32 arg0, s32 arg1, s32 arg2, u32 *window, s32 arg4) {
    s32 result = func_00280A90(window, arg4);

    switch (result) {
    case 1:
        *window &= ~2;
        return 1;
    case 2:
        *window &= ~2;
        return 1;
    default:
        return 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_00280E08);

extern void func_002CD0D8(u32 textId, s32 arg1, char *out);

extern s32 func_001951C8(char *text, s32, s32, s32, s32);

extern s32 frFontMeasureGlyphChain(s32 item);

extern void func_00195450(s32 item, s32 x, s32 y);

void mnuDrawCenteredLabel(s32 x, s32 y, s32 unused, s32 color, s32 textId, s32 param) {
    char text[0x40];
    s32 item;
    s32 width;

    func_002CD0D8(textId & 0xFFFF, 1, text);
    item = func_001951C8(text, 0, 0, 0, 0);
    frFontSetChildColors(item, color);
    width = frFontMeasureGlyphChain(item) + 8;
    func_00195450(item, x - (width * 0x10 >> 1) + 0x5F0, y);
    func_001958A0(item, 1, param);
    func_00194920(item);
}

void func_002811D0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 partyIndex, s32 param) {
    s32 outValue;
    s32 cost = ptyGetCurrentProfileId(D_003BAA00 + partyIndex * 0x1A4 + 0xA60);
    s32 code;
    s32 texture;
    s32 item;

    texture = func_002C1630(0xA09DC380, 0xA09DC300, arg3);
    code = arg4 != 0 ? arg4 : cost;
    if (code != 0) {
        if (func_002CD240(code & 0xFFFF, &outValue) != 0) {
            mnuDrawCenteredLabel(0x1120, 0x5F0, arg2, texture, code, param);
            return;
        }
        item = func_00197760(0, 0, arg2, texture, outValue, 0);
        func_00196088(0x1710, 0x5F0, item);
        func_001958A0(item, 1, param);
        func_00194920(item);
    }
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_002812E8);

INCLUDE_SDATA(const s32, "game/code_00279CC0", D_003BC718);

INCLUDE_SDATA(const s32, "game/code_00279CC0", D_003BC720);

INCLUDE_SDATA(const s32, "game/code_00279CC0", D_003BC728);

INCLUDE_SDATA(const s32, "game/code_00279CC0", D_003BC730);

INCLUDE_SDATA(const s32, "game/code_00279CC0", D_003BC738);

INCLUDE_SDATA(const s32, "game/code_00279CC0", D_003BC740);

void func_002814D0(s32 x, s32 y, s32 z, s32 obj, s32 mode, s32 param) {
    s32 pos[2] = {0x180, 0xE0};

    if (mode == 0) {
        func_002BF4E0(x, y, z, 0x100, 1, *(s32 *)(obj + 0x10), 0, param);
        x += pos[0];
        y += pos[1];
        func_002BF4E0(x, y, z, 0x100, 1, *(s32 *)(obj + 0xE8), 0, param);
        if (*(s32 *)(obj + 0xF0) == 0) {
            func_002BF4E0(x + 0x360, y + 0x68, z, 0x100, 1, *(s32 *)(obj + 0xEC), 0, param);
        } else {
            func_002BF4E0(x + 0x360, y + 0x68, z, 0x100, 1, *(s32 *)(obj + 0xEC), 0, param);
            func_002BF4E0(x + 0x790, y + 0x70, z, 0x100, 1, *(s32 *)(obj + 0xF0), 0, param);
        }
    }
}

void mnuBlendPanelSlots(s32 dst, s32 src, u32 amount) {
    s32 i;
    s32 ctx = *(s32 *)(dst + 0x18);

    for (i = 0; i < 4; i++) {
        s32 off = i * 4 + 0x80;
        s32 result = func_002C1630(*(s32 *)(ctx + off + 4),
                                   *(s32 *)(*(s32 *)(src + 0x18) + off + 4),
                                   (s32)amount / 2 + 0x80, ctx);
        s32 current = *(s32 *)(dst + 0x18);
        ctx = current;
        *(s32 *)(current + i * 4 + 0x14) = result;
    }
}

INCLUDE_ASM(const s32, "game/code_00279CC0", func_00281688);

INCLUDE_ASM(const s32, "game/code_00279CC0", func_00281780);

void func_00281898(u32 arg0) {
    memset(arg0, 0, 0x20);
}
