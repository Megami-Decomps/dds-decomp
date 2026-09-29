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
    MenuProgressNode *firstProgressNode;
} MenuProgressOwner;

extern s32 func_00248BA0(s32, s32);

extern s32 func_002CFEB8(s32);

extern s32 func_0027FA70(u16, u16);

extern void func_00284258(s32, s32, s32, s32, s32, s32);

extern s32 func_0027B2F8(s32, s32, s32, s32);

extern s32 mnuListAppendNode(s32, s32);

extern void func_002491B8(void);

extern u8 D_003BC3F8[];

extern s32 mnuWalkNodeList(s32, s32);

void func_00248580(MenuVisualWork *work) {
    effResolveAndReleaseResource(work->firstResource);
    effResolveAndReleaseResource(work->secondResource);
}

void func_002485B0(MenuVisualWork *work) {
    func_002BD870(work->firstResource);
    func_002BD870(work->secondResource);
}

INCLUDE_ASM(const s32, "game/code_00248580", func_002485E0);

INCLUDE_ASM(const s32, "game/code_00248580", func_00248658);

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

INCLUDE_ASM(const s32, "game/code_00248580", func_00248750);

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

    for (entry = *(s32 *)(*(s32 *)(owner + 0x74) + 0x10); entry != 0; entry = *(s32 *)(entry + 0x58)) {
        func_00248C38(*(u32 *)(entry + 0x70));
    }
}

INCLUDE_ASM(const s32, "game/code_00248580", func_00248D40);

void func_00248E18(s32 arg0) {
    func_0027B368(*(u32 *)(arg0 + 0x74));
}

void func_00248E30(s32 arg0) {
    func_00248C38(*(u32 *)(*(s32 *)(*(s32 *)(arg0 + 0x74) + 0x1c) + 0x70));
    func_0027B888(*(u32 *)(arg0 + 0x74));
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

extern s32 func_0027B2F8(s32, s32, s32, s32);

extern s32 mnuListAppendNode(s32, s32);

extern void func_002491B8(void);

extern u8 D_003BC3F8[];

s32 mnuBuildThresholdNodeList(s32 *items, s32 count, s32 excluded, s32 callback) {
    s32 list = func_0027B2F8(0, count, 0x15, callback);
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
    s32 state = *(s32 *)(object + 0x7C);
    if (state < 2) {
        if (state < 0) {
            return;
        }
        if (*(s32 *)(*(s32 *)(object + 0x78) + 0x20) == 0) {
            s32 selected = mnuWalkNodeList(2 - func_00249198(),
                                              *(s32 *)(object + 0x70));
            *(u32 *)(selected + 0x48) |= 1;
        }
    }
}

extern s32 mnuWalkNodeList(s32, s32);

void func_00249420(s32 object) {
    s32 state = *(s32 *)(object + 0x7C);
    s32 selectedIndex;
    if (state != 0) {
        if (state != 2) {
            return;
        }
        selectedIndex = 0;
    } else {
        selectedIndex = 3 - func_00249198();
    }
    if (*(s32 *)(*(s32 *)(object + 0x74) + 0x20) == 0) {
        s32 node = mnuWalkNodeList(selectedIndex, *(s32 *)(object + 0x70));
        *(u32 *)(node + 0x48) |= 1;
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
    func_0027B368(*(u32 *)(work + 0x78));
}

extern void kwlnFadeOutStart(s32, s32, s32, s32);
extern void func_00220110(s32);
extern void evtClearActiveFlag(s32);
extern void func_0024DEF8(s32, s32);

void mnuFadeOrPlayCloseSfx(s32 skip, u8 *work) {
    if (skip == 0) {
        s32 mode = *(s32 *)(work + 0x7C);

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
    u8 *owner = *(u8 **)(work + 0x78);
    *(s32 *)(work + 0x7C) = 0;
    *(s32 *)(work + 0x80) = *(s32 *)(*(u8 **)(owner + 0x1C) + 0x60);
}

extern s32 func_002D03F8(s32);
extern s32 sdfResourceRetainAddress(s32);
extern void *memset(void *, s32, u32);
extern s32 func_002BC5C0(s32);
extern void initPartyPanelSlots(s32);
extern void func_00271500(s32, s32);

u8 *mnuCreateWorkBlock(void) {
    s32 handle = func_002D03F8(0x82C);
    u8 *work = (u8 *)sdfResourceRetainAddress(handle);

    memset(work, 0, 0x82C);
    *(s32 *)work = handle;
    *(s32 *)(work + 4) = func_002BC5C0(1);
    initPartyPanelSlots((s32)(work + 0x84));
    func_00271500(*(s32 *)(work + 4), (s32)(work + 8));
    *(s32 *)(work + 0x80) = 1;
    return work;
}

void func_00249770(u32 *arg0) {
    mnuShutdownContext(arg0 + 100);
    func_00276320(arg0 + 2);
    mnuReleaseStaffResourceGroups(arg0 + 2);
    func_002BC618(arg0[1]);
    func_002D0918(*arg0);
}

extern s32 func_002716E8(s32, s32 *);
extern void func_002762D8(s32 *);
extern void func_00271480(s32, s32 *, s32, s32);

s32 mnuTickInitState(u8 *work) {
    s32 state = *(s32 *)(work + 0x80);
    s32 *group;

    if (state == 0) {
        return 1;
    }
    if (state == 2) {
        return 0;
    }
    group = (s32 *)(work + 8);
    if (func_002716E8(*(s32 *)(work + 4), group) == 0) {
        return 1;
    }
    func_002762D8(group);
    func_00271480((s32)(work + 0x190), group, 0, (s32)(work + 0x84));
    *(s32 *)(work + 0x80) = 2;
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

