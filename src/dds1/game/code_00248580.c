#include "common.h"

extern s32 func_0027B888(u32);

extern u8 D_003BC3E1;

extern s32 dds3GetWorldObject(void);

extern s32 mdlFlagTest(u32);

typedef struct {
    u8 pad00[0x64];
    u32 firstResource;   /* 0x64 */
    u32 secondResource;  /* 0x68 */
    u8 pad6C[0x7B4];
    u32 panelGroup;      /* 0x820 */
    u32 displayResource; /* 0x824 */
    u32 effectResource;  /* 0x828 */
} MenuVisualWork;

extern s32 D_003BAA00;

typedef struct MenuProgressNode {
    u8 pad00[0x48];
    u32 flags;
    u8 pad4C[0xC];
    struct MenuProgressNode *next;
    u8 pad5C[4];
    u32 itemIndex; /* 0x60: party entry index */
    u32 requiredAmount; /* 0x64 */
    u8 pad68[8];
    s32 panel; /* 0x70: allocated panel resource */
} MenuProgressNode;

typedef struct {
    u8 pad00[0x10];
    MenuProgressNode *firstProgressNode; /* 0x10 */
    u8 pad14[8];
    MenuProgressNode *selectedNode;      /* 0x1C */
    s32 selectionState;                   /* 0x20 */
} MenuProgressOwner;

typedef struct MenuProgressWork {
    u8 pad00[4];
    s32 groupResource;       /* 0x04 */
    u8 pad08[0x68];
    s32 listResource;        /* 0x70 */
    MenuProgressOwner *list; /* 0x74 */
    MenuProgressOwner *owner;/* 0x78 */
    s32 mode;                /* 0x7C */
    s32 initState;           /* 0x80 */
} MenuProgressWork;

extern s32 func_00248BA0(s32, s32);

extern s32 func_002CFEB8(s32);

extern s32 func_0027FA70(u16, u16);

extern void func_00284258(s32, s32, s32, s32, s32, s32);

extern s32 mnuCreateListState(s32, s32, s32, s32);

extern s32 mnuListAppendNode(s32, s32);

extern void func_002491B8(void);

extern u8 D_003BC3F8[];

extern s32 mnuWalkNodeList(s32, s32);

extern s32 func_003014F0(char *, const char *, ...);

extern u32 func_002C1630(u32, u32, s32);

extern s32 func_001978E8(s32, s32, s32, s32, s32, s32);

extern char D_003BC3E8[];

extern void func_001958A0(s32, s32, s32);

extern void func_00194920(s32);

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

extern EffectObject *func_002BD258(s32);

extern s32 effDestroyPackedBatch(s32);

void mnuReleaseVisualResources(MenuVisualWork *work) {
    effResolveAndReleaseResource(work->firstResource);
    effResolveAndReleaseResource(work->secondResource);
}

void func_002485B0(MenuVisualWork *work) {
    func_002BD870(work->firstResource);
    func_002BD870(work->secondResource);
}

INCLUDE_ASM(const s32, "game/code_00248580", func_002485E0);

INCLUDE_ASM(const s32, "game/code_00248580", func_00248658);

/* Mark entries whose required amount exceeds the current profile amount. */
void mnuRefreshThresholdNodeFlags(MenuProgressOwner *owner) {
    MenuProgressNode *node = owner->firstProgressNode;
    if (node != 0) {
        s32 base = D_003BAA00;
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

void func_00248750(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, u32 color, s32 priority) {
    char buf[16];
    s32 handle;

    func_003014F0(buf, D_003BC3E8, a4);
    handle = func_001978E8(a0, a1, a2, func_002C1630(color, color & ~0xFF, a3), (s32)buf, 0);
    func_001958A0(handle, 1, priority);
    func_00194920(handle);
}

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF5A8);

INCLUDE_ASM(const s32, "game/code_00248580", func_00248810);

extern s32 func_00248BA0(s32, s32);

extern s32 func_002CFEB8(s32);

extern s32 func_0027FA70(u16, u16);

extern void func_00284258(s32, s32, s32, s32, s32, s32);

s32 func_00248BA0(s32 resource, s32 context) {
    s32 panel = func_002CFEB8(0xa8);
    func_00284258(panel, 0, 0, 0x1e,
        func_0027FA70(*(u16 *)(resource + 6), *(u16 *)(resource + 8)),
        *(s32 *)(context + 0xe0));
    func_00284258(panel + 0x54, 1, 0, 0x1e,
        func_0027FA70(*(u16 *)(resource + 0xa), *(u16 *)(resource + 0xc)),
        *(s32 *)(context + 0xe0));
    return panel;
}

void func_00248C38(s32 arg0) {
    if (arg0 != 0) {
        mnuReleaseSpriteTextures();
        mnuReleaseSpriteTextures((s32)arg0 + 0x54);
        func_002CFF98(arg0);
        return;
    }
}

/* Rebuild each node's panel from its corresponding party entry. */
void mnuUpdateGroupResources(u8 *scene) {
    MenuProgressNode *node = *(MenuProgressNode **)(*(u8 **)(scene + 0x74) + 0x10);

    while (node != NULL) {
        node->panel = func_00248BA0(D_003BAA00 + node->itemIndex * 420 + 0xA60, (s32)scene);
        node = node->next;
    }
}

void mnuDestroyThresholdNodePanels(s32 owner) {
    s32 entry;

    for (entry = (s32)((MenuProgressWork *)owner)->list->firstProgressNode; entry != 0; entry = (s32)((MenuProgressNode *)entry)->next) {
        func_00248C38(((MenuProgressNode *)entry)->panel);
    }
}

INCLUDE_ASM(const s32, "game/code_00248580", func_00248D40);

void func_00248E18(s32 arg0) {
    func_0027B368((u32)((MenuProgressWork *)arg0)->list);
}

void func_00248E30(s32 arg0) {
    func_00248C38(((MenuProgressWork *)arg0)->list->selectedNode->panel);
    func_0027B888((u32)((MenuProgressWork *)arg0)->list);
}

INCLUDE_ASM(const s32, "game/code_00248580", func_00248E68);

INCLUDE_ASM(const s32, "game/code_00248580", func_00249010);

INCLUDE_ASM(const s32, "game/code_00248580", func_00249058);

u8 func_00249198(void) {
    s64 flagSet;

    flagSet = mdlFlagTest(0x902);
    return flagSet == 0;
}

INCLUDE_ASM(const s32, "game/code_00248580", func_002491B8);

extern s32 mnuCreateListState(s32, s32, s32, s32);

extern s32 mnuListAppendNode(s32, s32);

extern void func_002491B8(void);

extern u8 D_003BC3F8[];

/* Omit the selected entry when building the progress list. */
s32 mnuBuildThresholdNodeList(s32 *items, s32 count, s32 excluded, s32 callback) {
    s32 list = mnuCreateListState(0, count, 0x15, callback);
    s32 i;
    *(s32 *)(list + 0x30) = callback;
    *(s32 *)(list + 0x2c) = (s32)func_002491B8;
    *(s32 *)(list + 0x3c) = 0;
    for (i = 0; i < count; i++) {
        if (i != excluded) {
            s32 node = mnuListAppendNode(list, (s32)D_003BC3F8);
            *(s32 *)(node + 0x60) = items[i];
        }
    }
    return list;
}

extern s32 mnuWalkNodeList(s32, s32);

void func_002493B0(s32 object) {
    s32 state = ((MenuProgressWork *)object)->mode;
    if (state < 2) {
        if (state < 0) {
            return;
        }
        if (((MenuProgressWork *)object)->owner->selectionState == 0) {
            s32 selected = mnuWalkNodeList(2 - func_00249198(),
                                              ((MenuProgressWork *)object)->listResource);
            ((MenuProgressNode *)selected)->flags |= 1;
        }
    }
}

extern s32 mnuWalkNodeList(s32, s32);

void func_00249420(s32 object) {
    s32 state = ((MenuProgressWork *)object)->mode;
    s32 selectedIndex;
    if (state != 0) {
        if (state != 2) {
            return;
        }
        selectedIndex = 0;
    } else {
        selectedIndex = 3 - func_00249198();
    }
    if (((MenuProgressWork *)object)->list->selectionState == 0) {
        s32 node = mnuWalkNodeList(selectedIndex, ((MenuProgressWork *)object)->listResource);
        ((MenuProgressNode *)node)->flags |= 1;
    }
}

INCLUDE_ASM(const s32, "game/code_00248580", func_00249498);

extern void func_0027B368(u32);

extern void mnuReleaseStaffImageHandles(u8 *);

void mnuReleaseWorkResources(u8 *work) {
    u32 i;

    for (i = 0; i < 1; i++) {
        func_0027B368(*(u32 *)(work + 0x70 + i * 4));
    }
    mnuDestroyThresholdNodePanels((s32)work);
    mnuReleaseStaffImageHandles(work + 0xE0);
    func_00248E18((s32)work);
    func_0027B368((u32)((MenuProgressWork *)work)->owner);
}

extern void kwlnFadeOutStart(s32, s32, s32, s32);

extern void func_00220110(s32);

extern void evtClearActiveFlag(s32);

extern void func_0024DEF8(s32, s32);

void mnuFadeOrPlayCloseSfx(s32 skip, u8 *work) {
    if (skip == 0) {
        s32 mode = ((MenuProgressWork *)work)->mode;

        if (mode < 3) {
            if (mode > 0) {
                kwlnFadeOutStart(0, 0, 0, 15);
            } else {
                func_00220110(0x322);
            }
        } else {
            func_00220110(0x322);
        }
    } else {
        func_00220110(0x322);
    }
    evtClearActiveFlag(0);
    func_0024DEF8(1, 1);
}

void func_002496D8(u8 *work) {
    u8 *owner = (u8 *)((MenuProgressWork *)work)->owner;
    ((MenuProgressWork *)work)->mode = 0;
    ((MenuProgressWork *)work)->initState = *(s32 *)(*(u8 **)(owner + 0x1C) + 0x60);
}

extern s32 func_002D03F8(s32);

extern s32 sdfResourceRetainAddress(s32);

extern void *memset(void *, s32, u32);

extern s32 func_002BC5C0(s32);

extern void mnuInitPartyPanelSlots(s32);

extern void func_00271500(s32, s32);

u8 *mnuCreateWorkBlock(void) {
    s32 handle = func_002D03F8(0x82C);
    u8 *work = (u8 *)sdfResourceRetainAddress(handle);

    memset(work, 0, 0x82C);
    *(s32 *)work = handle;
    ((MenuProgressWork *)work)->groupResource = func_002BC5C0(1);
    mnuInitPartyPanelSlots((s32)(work + 0x84));
    func_00271500(((MenuProgressWork *)work)->groupResource, (s32)(work + 8));
    ((MenuProgressWork *)work)->initState = 1;
    return work;
}

void func_00249770(u32 *arg0) {
    mnuShutdownContext(arg0 + 100);
    func_00276320(arg0 + 2);
    mnuReleaseStaffResourceGroups(arg0 + 2);
    func_002BC618(arg0[1]);
    func_002D0918(*arg0);
}

extern s32 mnuStaffSlotsAllFilled(s32, s32 *);

extern void func_002762D8(s32 *);

extern void func_00271480(s32, s32 *, s32, s32);

s32 mnuTickInitState(u8 *work) {
    s32 state = ((MenuProgressWork *)work)->initState;
    s32 *group;

    if (state == 0) {
        return 1;
    }
    if (state == 2) {
        return 0;
    }
    group = (s32 *)(work + 8);
    if (mnuStaffSlotsAllFilled(((MenuProgressWork *)work)->groupResource, group) == 0) {
        return 1;
    }
    func_002762D8(group);
    func_00271480((s32)(work + 0x190), group, 0, (s32)(work + 0x84));
    ((MenuProgressWork *)work)->initState = 2;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00248580", func_00249850);

void func_00249930(s32 arg0) {
    mnuClearEntries(arg0 + 400);
    func_0027FA20(arg0 + 400);
    mnuDestroyPanelGroup(((MenuVisualWork *)arg0)->panelGroup);
    func_00283820(((MenuVisualWork *)arg0)->displayResource);
    func_00285160(((MenuVisualWork *)arg0)->effectResource);
}

void effUpdateAttached(s32 arg0, s32 arg1, s32 arg2, MenuVisualWork *work) {
    func_00285440(arg0, arg1, arg2, work->effectResource);
}

INCLUDE_ASM(const s32, "game/code_00248580", func_00249998);

INCLUDE_ASM(const s32, "game/code_00248580", func_00249A60);

void func_00249C08(s8 enabled) {
    s64 worldObject;

    if (enabled == '\x01') {
        worldObject = dds3GetWorldObject();
        if (worldObject != 0) {
            func_00110860(worldObject, 1);
        }
        D_003BC3E1 = 1;
    }
    else {
        worldObject = dds3GetWorldObject();
        if (worldObject != 0) {
            func_00110860(worldObject, 0);
        }
        D_003BC3E1 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00248580", func_00249C78);

INCLUDE_ASM(const s32, "game/code_00248580", func_00249D80);

INCLUDE_ASM(const s32, "game/code_00248580", func_00249DD0);

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF5E0);

INCLUDE_RODATA(const s32, "game/code_00248580", D_003AF620);

INCLUDE_ASM(const s32, "game/code_00248580", func_00249E20);

INCLUDE_ASM(const s32, "game/code_00248580", func_00249F08);

INCLUDE_SDATA(const s32, "game/code_00248580", D_003BC3E0);

INCLUDE_SDATA(const s32, "game/code_00248580", D_003BC3E1);

INCLUDE_SDATA(const s32, "game/code_00248580", D_003BC3E4);

INCLUDE_SDATA(const s32, "game/code_00248580", D_003BC3E8);

INCLUDE_SDATA(const s32, "game/code_00248580", D_003BC3F0);

INCLUDE_SDATA(const s32, "game/code_00248580", D_003BC3F8);

