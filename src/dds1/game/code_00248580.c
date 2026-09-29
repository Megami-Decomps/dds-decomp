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
    u8 pad5C[8];
    u32 requiredAmount;
} MenuProgressNode;

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

void func_00248700(s32 object) {
    MenuProgressNode *node = *(MenuProgressNode **)(object + 0x10);
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

INCLUDE_ASM(const s32, "game/code_00248580", func_00248BA0);

void func_00248C38(s32 arg0) {
    if (arg0 != 0) {
        mnuReleaseSpriteTextures();
        mnuReleaseSpriteTextures((s32)arg0 + 0x54);
        func_002CFF98(arg0);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_00248580", func_00248C80);

void func_00248CF8(s32 owner) {
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

INCLUDE_ASM(const s32, "game/code_00248580", func_002492F8);

INCLUDE_ASM(const s32, "game/code_00248580", func_002493B0);

INCLUDE_ASM(const s32, "game/code_00248580", func_00249420);

INCLUDE_ASM(const s32, "game/code_00248580", func_00249498);

INCLUDE_ASM(const s32, "game/code_00248580", func_002495F8);

INCLUDE_ASM(const s32, "game/code_00248580", func_00249668);

INCLUDE_ASM(const s32, "game/code_00248580", func_002496D8);

INCLUDE_ASM(const s32, "game/code_00248580", func_002496F0);

void func_00249770(u32 *arg0) {
    mnuShutdownContext(arg0 + 100);
    func_00276320(arg0 + 2);
    func_00271648(arg0 + 2);
    func_002BC618(arg0[1]);
    func_002D0918(*arg0);
}

INCLUDE_ASM(const s32, "game/code_00248580", func_002497C0);

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

void func_00249C08(s8 arg0) {
    s64 temp_v0;

    if (arg0 == '\x01') {
        temp_v0 = dds3GetWorldObject();
        if (temp_v0 != 0) {
            func_00110860(temp_v0, 1);
        }
        D_003BC3E1 = 1;
    }
    else {
        temp_v0 = dds3GetWorldObject();
        if (temp_v0 != 0) {
            func_00110860(temp_v0, 0);
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
