#include "common.h"

extern void func_00266C08();
extern void kwlnFadeOutStart(s32, s32, s32, s32);
extern s32 func_0026BC80();
extern void func_0023AC80(s32);
extern void evtClearActiveFlag(s32);
extern void func_0026CA80(s32, s32);
extern s8 D_00437859;

typedef struct MenuSlotState {
    u8 pad00[0x64];
    s32 batch;     /* 0x64 */
    u8 pad68[0x40];
    s32 effect[7]; /* 0xA8 */
    s32 cur;       /* 0xC4 */
    s32 prev;      /* 0xC8 */
    u8 padCC[0x18];
    s32 mode;      /* 0xE4 */
} MenuSlotState;

extern void evtLoadResourcePair(const char *, u8 *);
extern void func_0026C538(s32);
extern void func_002680E0(s32);
extern void func_002A91A0(u8 *);
extern void func_002673B8();
extern void func_002C3E58(u8 *);
extern void func_002C1B58(u8 *, s32);

extern s32 func_0035C860(char *, const char *, ...);
extern u32 func_00309138(u32, u32, s32);
extern s32 func_0019F5E8(s32, s32, s32, s32, s32, s32);
extern char D_00437860[];

typedef struct EffectPair {
    s32 a;
    s32 b;
} EffectPair;

typedef struct EffectInner {
    u8 pad00[0x20];
    EffectPair *pair; /* 0x20 */
} EffectInner;

typedef struct EffectObject {
    u8 pad00[8];
    EffectInner *inner; /* 0x08 */
} EffectObject;

extern EffectObject *func_00304998(s32);

extern s32 mdlFlagTest(u32);

extern s64 func_0026C768(void);

extern s32 fldGetModeFrameRecordIndex(s32);

extern s32 kwlnFadeIsActive(void);

extern s32 func_002B86E8(u32);

extern void func_002686F0(s32);

extern s8 D_00437858;

extern s32 func_00101958();

extern s32 func_002C3E08(s32, s32, s32, s32);

extern void func_002C1B70(s32, s32);

extern void func_002C1B68(s32, s32);

extern s32 movAreTitleEffectsReady(s32, s32);

extern void func_002B2818(s32);

extern s32 D_00435DD0;

extern s32 func_003292A8(s32);

extern s32 sdfResourceRetainAddress(s32);

extern s32 func_00303D00(s32);

extern void mnuInitPartyPanelSlots(s32);

extern void func_002A9640(s32, s32);

extern s32 func_002C32B0(void);

extern void mnuSetGroupProperties(s32, s32, s32, s32, s32);

extern void func_002C0630(s32, s32, s32, s32, s32, s32);

extern void func_002C16F0(s32, s32, s32, s32, s32, s32, s32);

extern void func_00267EA0(s8, s8);

extern s32 mnuWalkNodeList(s32, s32);

extern void kwlnTaskDestroyWithHierarchyByName(const char *, s32);

extern const char D_00424F00[];

extern const char D_00424F10[];

extern const char D_00424F20[];

extern s32 D_0043785C;

extern s32 mnuCreateListState();

extern s32 mnuListAppendNode(s32, s32);

extern u8 D_00437870[];

extern s32 func_00328D68(s32);

extern s32 func_002BC460(u16, u16);

extern void func_002C2128(s32, s32, s32, s32, s32, s32);

extern s32 effDestroyPackedBatch(s32);
extern void func_002C3390(s32);

typedef struct MenuResourceGroup {
    u8 pad0[0x64];
    u32 primary;
    u32 secondary;
    u32 tertiary;
    u32 quaternary;
    u8 pad74[0x380];
    s32 reducedMode;
} MenuResourceGroup;

typedef struct MenuProgressNode {
    u8 pad00[0x48];
    u32 flags;
    u8 pad4C[0xC];
    struct MenuProgressNode *next;
    u8 pad5C[4];
    s32 entryId;
    u32 requiredAmount;
    u8 pad68[8];
    s32 childPanel;
} MenuProgressNode;

typedef struct MenuProgressList {
    u8 pad00[0x10];
    MenuProgressNode *head;
    u8 pad14[8];
    MenuProgressNode *selected;
    s32 busy;
    u8 pad24[8];
    s32 updateCallback;
    s32 callback;
    u8 pad34[8];
    s32 visible;
} MenuProgressList;

typedef struct MenuTitleResource {
    u8 pad00[6];
    u16 firstA;
    u16 firstB;
    u16 secondA;
    u16 secondB;
} MenuTitleResource;

typedef struct MenuProgressHost {
    s32 heapHandle;
    s32 titleEffectHandle;
    u8 pad08[0x64];
    s32 loadState;
    u8 pad70[8];
    s32 menuList;
    MenuProgressList *progressList;
    MenuProgressList *secondaryList;
    s32 state;
    s32 selectedSlot;
    u8 pad8C[0x60];
    s32 panelStyle;
} MenuProgressHost;

extern void func_002C42B0(s32 *, void *);

extern u8 D_003CE944[];

extern void func_002B81C8(u32);

extern void func_002A9200(u8 *);

extern s64 func_002C4038(s32, s32 *, u64, u64);

void func_002665B0(s32 arg0) {
    effDestroyPackedBatch(*(u32 *)(arg0 + 0x3c));
}

u8 func_002665C8() {
    s64 unlocked;

    unlocked = mdlFlagTest(0x31);
    return unlocked != 0;
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_002665E8);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00266808);

void mnuReleaseResourceGroup(s32 address) {
    MenuResourceGroup *group = (MenuResourceGroup *)address;
    if (group->reducedMode == 0) {
        effResolveAndReleaseResource(group->primary);
        effResolveAndReleaseResource(group->secondary);
        effResolveAndReleaseResource(group->tertiary);
        effResolveAndReleaseResource(group->quaternary);
        return;
    }
    effResolveAndReleaseResource(group->primary);
    effResolveAndReleaseResource(group->secondary);
}

void func_00266928(s32 address, u32 value) {
    MenuResourceGroup *group = (MenuResourceGroup *)address;
    if (group->reducedMode == 0) {
        func_00304FB0(group->primary);
        func_00304FB0(group->secondary, value);
        func_00304FB0(group->tertiary, value);
        func_00304FB0(group->quaternary, value);
        return;
    }
    func_00304FB0(group->primary);
    func_00304FB0(group->secondary, value);
}

void func_002669B0(u32 arg0) {
    func_00266928(arg0, 0);
}

extern s32 func_0019F460(s32, s32, s32, s32, s32, s32);
extern void func_0019D550(s32, s32, s32);
extern void func_0019C5B0(s32);
extern u8 D_003A41A8[];
extern u8 D_003A47E8[];

void func_002669C8(s32 a0, s32 a1, s32 a2, s32 a3, s8 slot, s8 alternate) {
    u8 *entry;
    s32 handle;

    if (alternate == 0) {
        entry = D_003A41A8 + slot * 32;
    } else {
        entry = D_003A47E8 + slot * 32;
    }
    handle = func_0019F460(a0 - 0x120, a1, a2, a3, (s32)entry, 0);
    func_0019D550(handle, 1, 0x52);
    func_0019C5B0(handle);
}

typedef struct BoxRecord {
    u16 status; /* 0x00: bit 0 set when the record is in use */
    u8 pad02[4];
    u16 y0; /* 0x06 */
    u16 y1; /* 0x08 */
    u16 x0; /* 0x0A */
    u16 x1; /* 0x0C */
    u16 flags; /* 0x0E */
} BoxRecord;

s32 func_00266A48(BoxRecord *box) {
    f32 w = box->x1 - box->x0;
    f32 h = box->y1 - box->y0;
    s32 bonus = 0;

    if (box->flags & 0x400) {
        bonus = 100;
    }
    if (box->flags & 0x100) {
        bonus += 50;
    }
    if (box->flags & 0x80) {
        bonus += 100;
    }
    if (box->flags & 0x40) {
        bonus += 100;
    }
    if (box->flags & 0x10) {
        bonus += 100;
    }
    return (s32)(h * 1.8f) + (s32)(w * (w / 40.0f + 5.0f)) + bonus;
}

/* Mark entries whose required amount exceeds the current profile amount. */
void mnuRefreshThresholdNodeFlags(MenuProgressList *list) {
    MenuProgressNode *node = list->head;
    if (node != 0) {
        s32 base = D_00435DD0;
        do {
            u32 amount = *(u32 *)(base + 0x3c);
            if (amount < node->requiredAmount) {
                node->flags |= 1;
            } else {
                node->flags &= ~1u;
            }
            node = node->next;
        } while (node != 0);
    }
}

void func_00266B48(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, u32 color, s32 priority) {
    char buf[16];
    s32 handle;

    func_0035C860(buf, D_00437860, a4);
    handle = func_0019F5E8(a0, a1, a2, func_00309138(color, color & ~0xFF, a3), (s32)buf, 0);
    func_0019D550(handle, 1, priority);
    func_0019C5B0(handle);
}

INCLUDE_RODATA(const s32, "game/code_002665B0", D_00424E60);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00266C08);

s32 func_00266F70(MenuTitleResource *resource, MenuProgressHost *host) {
    s32 panel = func_00328D68(0xa0);
    func_002C2128(panel, 0, 0, 0x1e,
        func_002BC460(resource->firstA, resource->firstB),
        host->panelStyle);
    func_002C2128(panel + 0x50, 1, 0, 0x1e,
        func_002BC460(resource->secondA, resource->secondB),
        host->panelStyle);
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

void mnuCreateThresholdNodePanels(MenuProgressHost *host) {
    MenuProgressNode *node = host->progressList->head;
    while (node != 0) {
        s32 id = node->entryId;
        node->childPanel =
            func_00266F70((MenuTitleResource *)(D_00435DD0 + id * 0x1c4 + 0xa60), host);
        node = node->next;
    }
}

void mnuDestroyThresholdNodePanels(MenuProgressHost *host) {
    MenuProgressNode *node;

    for (node = host->progressList->head; node != 0; node = node->next) {
        func_00267008(node->childPanel);
    }
}

typedef struct ThresholdEntry {
    s32 entryId;
    s32 requiredAmount;
} ThresholdEntry;

void func_00267110(MenuProgressHost *host) {
    MenuProgressList *list;
    s32 i;

    list = (MenuProgressList *)mnuCreateListState(0, 5, 0x24);
    list->callback = (s32)host;
    *(s32 *)&host->progressList = (s32)list;
    list->updateCallback = (s32)func_00266C08;
    list->visible = 0;
    for (i = 0; i < 5; i++) {
        BoxRecord *box = (BoxRecord *)(D_00435DD0 + i * 0x1C4 + 0xA60);

        if ((u16)(box->status & 1)) {
            s32 score = func_00266A48(box);

            if (score != 0) {
                MenuProgressNode *node = (MenuProgressNode *)mnuListAppendNode((s32)host->progressList, (s32)D_00437870);
                ThresholdEntry *entry = (ThresholdEntry *)&node->entryId;

                node->childPanel = 0;
                entry->requiredAmount = score;
                entry->entryId = i;
            }
        }
    }
    mnuRefreshThresholdNodeFlags(host->progressList);
}

void func_002671E8(MenuProgressHost *host) {
    func_002B81C8((s32)host->progressList);
}

void func_00267200(MenuProgressHost *host) {
    func_00267008(host->progressList->selected->childPanel);
    func_002B86E8((u32)host->progressList);
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_00267238);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00267358);

INCLUDE_ASM(const s32, "game/code_002665B0", func_002673B8);

u32 func_002674F8(void) {
    return 0;
}

void func_00267500(s32 x, s32 y, s32 unused, MenuProgressList *list, MenuProgressNode *node, s32 arg5) {
    s32 width = list->visible;
    s32 host = list->callback;
    s32 index = node->entryId;
    s32 isCurrent = node == list->selected;

    if (node->flags & 1) {
        width /= 2;
    }
    if (isCurrent) {
        func_00306CD0(x, y, 0, width, 0, *(s32 *)(host + 0x64), 0x16, arg5);
        index += 1;
    }
    func_00306CD0(x + 0x50, y - 0x10, 0, width, 0, *(s32 *)(host + 0x64), index, arg5);
}

/* Omit the selected entry when building the progress list. */
s32 mnuBuildThresholdNodeList(s32 *items, s32 count, s32 excluded, s32 callback) {
    MenuProgressList *list = (MenuProgressList *)mnuCreateListState(0, count, 0x16, callback);
    s32 i;
    list->callback = callback;
    list->updateCallback = (s32)func_00267500;
    list->visible = 0;
    for (i = 0; i < count; i++) {
        if (i != excluded) {
            MenuProgressNode *node = (MenuProgressNode *)mnuListAppendNode((s32)list, (s32)D_00437870);
            node->entryId = items[i];
        }
    }
    return (s32)list;
}

void func_00267680(MenuProgressHost *host) {
    s32 state = host->state;
    if (state < 2) {
        if (state < 0) {
            return;
        }
        if (host->secondaryList->busy == 0) {
            MenuProgressNode *selected = (MenuProgressNode *)mnuWalkNodeList(2 - func_002674F8(),
                                              host->menuList);
            selected->flags |= 1;
        }
    }
}

void func_002676F0(MenuProgressHost *host) {
    s32 state = host->state;
    s32 selectedIndex;
    if (state != 0) {
        if (state != 2) {
            return;
        }
        selectedIndex = 0;
    } else {
        selectedIndex = 3 - func_002674F8();
    }
    if (host->progressList->busy == 0) {
        MenuProgressNode *node = (MenuProgressNode *)mnuWalkNodeList(selectedIndex, host->menuList);
        node->flags |= 1;
    }
}

void func_00267768(MenuProgressHost *host) {
    s32 table[3][5] = {
        {0xB, 0xD, 0xF, 0x11, 0x13},
        {0xB, 0xD, 0x2E, 0x30, 0},
        {0x11, 0x30, 0, 0, 0},
    };
    s32 row;
    s32 count;
    s32 excluded = -1;

    switch (host->state) {
    case 0:
        row = 0;
        count = 5;
        if (func_002674F8() != 0) {
            excluded = 1;
        }
        break;
    case 1:
        row = 1;
        count = 4;
        if (func_002674F8() != 0) {
            excluded = 1;
        }
        break;
    default:
        row = 2;
        count = 2;
        break;
    }
    host->menuList = mnuBuildThresholdNodeList(table[row], count, excluded, (s32)host);
    func_00267110(host);
    func_002A91A0((u8 *)host + 0xE8);
    mnuCreateThresholdNodePanels(host);
    func_002673B8(host);
    func_00267680(host);
    func_002676F0(host);
}

void mnuReleaseWorkResources(u8 *work) {
    u32 i;

    for (i = 0; i < 1; i++) {
        func_002B81C8(*(u32 *)(work + 0x78 + i * 4));
    }
    mnuDestroyThresholdNodePanels((s32)work);
    func_002A9200(work + 0xE8);
    func_002671E8((s32)work);
    func_002B81C8(*(u32 *)(work + 0x80));
}

void func_00267938(s32 flag, s32 scene) {
    if (flag == 0) {
        s32 state = *(s32 *)(scene + 0x84);

        if (state < 3) {
            if (state > 0) {
                kwlnFadeOutStart(0, 0, 0, 0xF);
            } else if (mdlFlagTest(0x429) != 0 || func_0026BC80() != 0 || func_002665C8(scene) != 0) {
                kwlnFadeOutStart(0, 0, 0, 0xF);
            } else {
                func_0023AC80(0x322);
            }
        } else if (mdlFlagTest(0x429) != 0 || func_0026BC80() != 0 || func_002665C8(scene) != 0) {
            kwlnFadeOutStart(0, 0, 0, 0xF);
        } else {
            func_0023AC80(0x322);
        }
    } else {
        func_0023AC80(0x322);
    }
    evtClearActiveFlag(0);
    func_0026CA80(1, 1);
}

void func_002679E8(u8 *work) {
    u8 *owner = *(u8 **)(work + 0x80);
    *(s32 *)(work + 0x84) = 0;
    *(s32 *)(work + 0x88) = *(s32 *)(*(u8 **)(owner + 0x1C) + 0x60);
}

s32 mnuCreateProgressHost(void) {
    s32 heap = func_003292A8(0xa82c);
    MenuProgressHost *host = (MenuProgressHost *)sdfResourceRetainAddress(heap);
    memset((void *)host, 0, 0xa82c);
    host->heapHandle = heap;
    host->titleEffectHandle = func_00303D00(1);
    mnuInitPartyPanelSlots((s32)host + 0x70);
    func_002A9640(host->titleEffectHandle, (s32)host + 8);
    host->loadState = 1;
    return (s32)host;
}

void func_00267A80(u32 *arg0) {
    func_002B2860(arg0 + 2);
    func_002A9788(arg0 + 2);
    func_00303D58(arg0[1]);
    func_003297C8(*arg0);
}

s32 mnuPollTitleEffectsReady(MenuProgressHost *host) {
    s32 state = host->loadState;
    if (state == 0) {
        return 1;
    }
    if (state == 2) {
        return 0;
    }
    if (movAreTitleEffectsReady(host->titleEffectHandle, (s32)host + 8) == 0) {
        return 1;
    }
    func_002B2818((s32)host + 8);
    host->loadState = 2;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_00267B40);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00267C48);

void func_00267D30(s32 unused, s32 object) {
    if (*(s32 *)(object + 0xa828) == 0) {
        s32 effect = func_002C32B0();
        *(s32 *)(object + 0xa828) = effect;
        mnuSetGroupProperties(effect, *(s32 *)(object + 8), *(s32 *)(object + 0x14), 1, 2);
    }
}

void func_00267DA0(s32 object) {
    func_002C3390(*(s32 *)(object + 0xa828));
    *(s32 *)(object + 0xa828) = 0;
}

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

typedef struct MenuFadeWork {
    u8 pad00[0x84];
    s32 reduced;      /* 0x84 */
    u8 pad88[0xCC];
    s32 fadeColor;    /* 0x154 */
} MenuFadeWork;

extern void sndStartTrackExtended(s32);
extern void func_00342580(s32);

void func_00267EA0(s8 mode, s8 enable) {
    s32 address = func_00101958(D_0043785C);
    MenuFadeWork *work = (MenuFadeWork *)address;

    if (mode == 1) {
        if (enable == 1) {
            if (work->reduced == 0) {
                sndStartTrackExtended(work->fadeColor);
            } else {
                sndStartTrackExtended(work->fadeColor);
            }
        }
        D_00437859 = 1;
        mnuReleaseResourceGroup(address);
    } else {
        if (enable == 1) {
            func_00342580(work->fadeColor);
        }
        D_00437859 = 0;
        func_00266928(address, 1);
    }
}

void func_00267F68(s8 index) {
    func_00267EA0(index, 1);
}

void func_00267F88(MenuSlotState *state) {
    EffectObject *obj;

    obj = func_00304998(1);
    state->effect[0] = (s32)obj;
    obj->inner->pair->a = 0x14;
    obj->inner->pair->b = 1;
    obj = func_00304998(1);
    state->effect[1] = (s32)obj;
    obj->inner->pair->a = 0xF;
    obj->inner->pair->b = 0;
    obj = func_00304998(8);
    state->effect[2] = (s32)obj;
    obj->inner->pair->a = 6;
    obj->inner->pair->b = 1;
    obj = func_00304998(8);
    state->effect[3] = (s32)obj;
    obj->inner->pair->a = 6;
    obj->inner->pair->b = 0;
    obj = func_00304998(1);
    state->effect[4] = (s32)obj;
    obj->inner->pair->a = 6;
    obj->inner->pair->b = 1;
    obj = func_00304998(1);
    state->effect[5] = (s32)obj;
    obj->inner->pair->a = 6;
    obj->inner->pair->b = 0;
    obj = func_00304998(1);
    state->effect[6] = (s32)obj;
    obj->inner->pair->a = 0x78;
    obj->inner->pair->b = 0;
}

void func_00268090(s32 object) {
    s32 *batch = (s32 *)(object + 0xA8);
    u32 i;

    for (i = 0; i < 7; i++) {
        effDestroyPackedBatch(batch[i]);
    }
}

typedef struct MenuFadeHost {
    u8 pad00[0x84];
    s32 reduced;      /* 0x84 */
    u8 pad88[0xCC];
    s32 fadeColor;    /* 0x154 */
} MenuFadeHost;

INCLUDE_ASM(const s32, "game/code_002665B0", func_002680E0);

extern void sndStartTrackExtended(s32);
extern void func_003425B0(void);
extern void func_00342580(s32);
extern void func_003425D8(void);

void func_00268128(s32 mode, MenuFadeHost *host) {
    if (mode == 0) {
        if (host->reduced == 0) {
            sndStartTrackExtended(host->fadeColor);
        } else {
            func_003425B0();
        }
    } else if (host->reduced == 0) {
        func_00342580(host->fadeColor);
    } else {
        func_003425D8();
    }
}

u8 *func_00268178(s32 reduced, s32 slot) {
    s32 handle;
    u8 *obj;
    u32 i;

    handle = func_003292A8(0x3F8);
    obj = (u8 *)sdfResourceRetainAddress(handle);
    memset(obj, 0, 0x3F8);
    *(s32 *)obj = handle;
    func_002C3E58(obj + 8);
    func_00267F88((MenuSlotState *)obj);
    *(s32 *)(obj + 0x84) = reduced;
    *(s32 *)(obj + 0xE4) = reduced;
    *(s32 *)(obj + 0x88) = slot;
    *(s32 *)(obj + 0xE0) = slot;
    func_00267768((MenuProgressHost *)obj);
    evtLoadResourcePair("/facility/msg/terminal/mes_data.bmd", obj + 0x5C);
    func_0026C538(*(s32 *)(obj + 0x60));
    for (i = 0; i < 2; i++) {
        *(s32 *)(obj + 0xC4 + i * 4) = -1;
    }
    *(s32 *)(obj + 0x150) = 0xF;
    func_002680E0((s32)obj);
    func_00268128(0, (MenuFadeHost *)obj);
    func_002C1B58(obj + 0x3E8, 0x60);
    return obj;
}

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

void fldStopSceneTasks(void) {
    kwlnTaskDestroyWithHierarchyByName(D_00424F00, 0);
    kwlnTaskDestroyWithHierarchyByName(D_00424F10, 0);
    kwlnTaskDestroyWithHierarchyByName(D_00424F20, 0);
    D_0043785C = 0;
}

s32 fldPollSceneState(void) {
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

static inline s64 menuSetHandler(s32 context, u64 mode, s32 callback) {
    return func_002C4038(context + 8, (s32 *)(context + 0x54), mode, callback);
}

s64 func_002684F0(s32 callback) {
    s32 context = func_00101958();

    func_002C42B0((s32 *)(context + 0x54), D_003CE944);
    return menuSetHandler(context, 0, callback);
}

s64 func_00268550(s32 callback) {
    return menuSetHandler(func_00101958(), 1, callback);
}

s64 func_00268588(s32 callback) {
    return menuSetHandler(func_00101958(), 2, callback);
}

s32 func_002685C0(void) {
    s32 fadeActive = kwlnFadeIsActive();

    if (fadeActive != 0) {
        return 0;
    }
    return func_0026C768() == 0;
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_002685F0);

typedef struct {
    u8 pad00[6];
    u16 previousA; /* 0x06 */
    u16 currentA; /* 0x08 */
    u16 previousB; /* 0x0A */
    u16 currentB; /* 0x0C */
    u16 flags; /* 0x0E */
} SceneOptionRecord;

void fldSaveSceneOptionsAndClearFlags(SceneOptionRecord *option) {
    u16 flags = option->flags;
    u16 currentA = option->currentA;
    u16 currentB = option->currentB;
    u16 retainedFlags = flags & 0xfa2f;

    option->previousA = currentA;
    option->previousB = currentB;
    option->flags = retainedFlags;
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_002686F0);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00268838);

/* The sequel stores this countdown eight bytes later than DDS1. */
typedef struct {
    u8 pad00[0xA4];
    s32 remainingFrames; /* 0xA4 */
} SceneTimerView;

s32 fldClassifyRemainingFrames(SceneTimerView *timer) {
    s32 frames = timer->remainingFrames;
    if (frames == 0) {
        return 0;
    }
    return frames >= 60 ? 2 : 1;
}

void func_002689D0(u32 mode, MenuSlotState *state) {
    s32 *slot = &state->cur;

    if (*slot < 0) {
        return;
    }
    switch (mode) {
    case 1:
        effConfigureWithDefaultSetting(state->batch, *slot, state->effect[4], 0, 5, 2);
        break;
    case 2:
        effConfigureWithDefaultSetting(state->batch, *slot, state->effect[5], 0, 0, 2);
        break;
    case 3:
        effConfigureWithDefaultSetting(state->batch, *slot, state->effect[4], 0, 0, 2);
        if (slot[1] >= 0) {
            effConfigureWithDefaultSetting(state->batch, slot[1], state->effect[5], 0, 0, 2);
        }
        break;
    }
}

void func_00268AA0(s32 ctx, s32 index, MenuSlotState *state) {
    s32 table[5] = {3, 1, 2, 0x2D, 0x52};

    if (state->mode == 1 && index == state->mode) {
        index = 3;
    }
    if (index >= 0) {
        state->prev = state->cur;
        state->cur = table[index];
    } else if (index == -2) {
        state->prev = -1;
    }
    func_002689D0(ctx, state);
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_00268B48);

typedef struct {
    u8 pad00[0x14];
    u8 unk14;
    u8 pad15[0x8B];
} SceneFrameRecord;

typedef struct {
    u8 pad00[0x18];
    SceneFrameRecord *records;
} SceneFrameTable;

typedef struct {
    u8 pad00[0x64];
    SceneFrameTable *frameTable;
    u8 pad68[0x7C];
    s32 mode; /* 0xE4 */
} SceneFrameOwner;

/* Scene modes 1 and 2 select different entries from the same frame table. */
s32 fldGetModeFrameRecordIndex(s32 object) {
    switch (((SceneFrameOwner *)object)->mode) {
    case 1:
        return 0x32;
    case 2:
        return 0x36;
    default:
        return 0;
    }
}

u8 func_00268C08(SceneFrameOwner *scene) {
    s32 index;

    index = fldGetModeFrameRecordIndex((s32)scene);
    return scene->frameTable->records[index].unk14;
}

typedef struct MenuEffHost {
    u8 pad00[0x64];
    s32 batch;        /* 0x64 */
    u8 pad68[0x40];
    s32 effectA;      /* 0xA8 */
    u8 padAC[8];
    s32 effectB;      /* 0xB4 */
} MenuEffHost;

extern void effConfigureWithDefaultSetting(s32, s32, s32, s32, s32, s32);

void func_00268C48(s32 mode, MenuEffHost *host) {
    switch (mode) {
    case 1:
        effConfigureWithDefaultSetting(host->batch, 6, host->effectA, 0, 0, 2);
        return;
    case 2:
        effConfigureWithDefaultSetting(host->batch, 6, host->effectB, 0, 0, 2);
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_002665B0", func_00268CC0);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00268EC8);

INCLUDE_ASM(const s32, "game/code_002665B0", func_002690A8);

INCLUDE_ASM(const s32, "game/code_002665B0", func_00269230);

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

