#include "common.h"

typedef struct MenuActionOwner MenuActionOwner;

extern u8 D_0043798A;

extern s32 kwlnFadeIsActive(void);

extern s32 func_002C6CE8(void);

extern void mnuSetCommandPhase(MenuActionOwner *, u32);

extern void func_00297200(MenuActionOwner *, u32);

extern void func_002971C0(MenuActionOwner *, u32);

extern s8 D_0043798B;

extern s8 D_00437989;

extern s8 D_00437988;

typedef struct MenuAction {
    u32 value;
    u32 mode;
} MenuAction;

struct MenuActionOwner {
    u8 pad00[0x30];
    MenuAction *action;
    u8 pad34[0x88];
    u32 counter;
    u32 valueC0;
};

typedef struct MenuIconRef {
    u16 id;
    u8 param;
    u8 pad3;
} MenuIconRef;

typedef struct MenuIconBatch {
    MenuIconRef icons[3];
    u32 resource;
} MenuIconBatch;

extern void func_0011A118(s32, s32);

extern char D_00437990[];

extern char D_00428388[];

extern char D_00428398[];

extern char D_00437990[];

extern char D_00428388[];

extern char D_00428398[];

extern void kwlnTaskDestroyWithHierarchyByName(char *, s32);

typedef struct MenuPanelBlock {
    s32 data[0x71];
} MenuPanelBlock;

INCLUDE_ASM(const s32, "game/code_00296E98", func_00296E98);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00297000);

void func_002971C0(MenuActionOwner *owner, u32 value) {
    MenuAction *action;

    action = owner->action;
    if (action != (MenuAction *)0x0) {
        action->value = value;
        action->mode = 1;
    }
}

void func_002971E0(MenuActionOwner *owner, u32 value) {
    MenuAction *action;

    action = owner->action;
    if (action != (MenuAction *)0x0) {
        action->value = value;
        action->mode = 2;
    }
}

void func_00297200(MenuActionOwner *owner, u32 value) {
    MenuAction *action;

    action = owner->action;
    if (action != (MenuAction *)0x0) {
        action->value = value;
        action->mode = 1;
    }
}

void func_00297220(MenuActionOwner *owner, u32 value) {
    MenuAction *action;

    action = owner->action;
    if (action != (MenuAction *)0x0) {
        action->value = value;
        action->mode = 2;
    }
}

void mnuSetCommandPhase(MenuActionOwner *owner, u32 value) {
    owner->valueC0 = value;
    owner->counter = 0;
}

INCLUDE_ASM(const s32, "game/code_00296E98", func_00297250);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00297320);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00297898);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00297970);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00298570);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00298648);

void brsTaskStart(void) {
    mnuStaffCreateTasks();
    D_0043798A = 0;
    D_0043798B = 1;
}

s8 brsTaskIsUpdateBlocked(void) {
    return D_0043798B;
}

u32 brsTaskAllowUpdate(void) {
    D_0043798A = 1;
    return 1;
}

void brsTaskPollDone(void) {
    brsTaskConsumeDone();
}

s8 brsTaskHasPendingRows(void) {
    return D_00437989;
}

s8 func_00298E80(s32 context) {
    if (*(s32 *)(context + 0xAEB0) != 0) {
        D_0043798B = 0;
    }
    return D_0043798B ? 0 : D_0043798A;
}

void gstApplyCounterDeltaTable(MenuIconRef *refs) {
    u32 i;

    for (i = 0; i < 3; i++) {
        u16 id = refs->id;
        u8 param = refs->param;

        refs++;
        if (id != 0) {
            func_0011A118(id, param);
        }
    }
}

void gstApplyBundleMacca(MenuIconBatch *batch) {
    func_0011A0D0(batch->resource);
}

extern s32 ptyComputeTotalExp(u8 *, s32);

void ptyClampExp(u32 *unit) {
    u8 buf[0x1C4];
    u32 exp;

    memcpy(buf, unit, 0x1C4);
    *(u16 *)(buf + 0x14) = 0x63;
    exp = ptyComputeTotalExp(buf, 0);
    if (exp < unit[4]) {
        unit[4] = exp;
    }
}

INCLUDE_ASM(const s32, "game/code_00296E98", func_00299018);

void brsApplyRewardBundle(u32 partyWork, MenuIconBatch *batch, u32 rewardState) {
    gstApplyCounterDeltaTable(batch->icons);
    gstApplyBundleMacca(batch);
    func_00299018(partyWork, rewardState);
}

extern void func_002B2818(s32);
extern void func_002A95B0(s32, s32, s32, s32);
extern s32 func_002C0B80(s32, s32, s32);
extern s32 mnuCreateSpriteState(s32, s32, s32);
extern void func_002C6988(s32);

void brsOpenSkillPackagePanel(s32 work) {
    s32 *group = (s32 *)(work + 0x51C);
    s32 panel;

    func_002B2818((s32)group);
    func_002A95B0(work + 0x690, (s32)group, 0, work + 0x584);
    panel = func_002C0B80(*(s32 *)(work + 0x520), *(s32 *)(work + 0x524), 0);
    *(s32 *)(work + 0xAD34) = panel;
    mnuUpdateFiveListEntries(panel, *(s32 *)(work + 0x94));
    *(s32 *)(work + 0xAD38) = mnuCreateSpriteState(*(s32 *)(work + 0x520), *(s32 *)(work + 0x524), *(s32 *)(work + 0x51C));
    func_002C6988(0);
    mnuForwardTableByte(*(u16 *)(*(s32 *)(work + *(s32 *)(work + 0x264) * 24 + 0x2F0) + 4));
}

INCLUDE_ASM(const s32, "game/code_00296E98", func_00299280);

INCLUDE_RODATA(const s32, "game/code_00296E98", D_00428358);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00299320);

INCLUDE_ASM(const s32, "game/code_00296E98", func_002993D0);

extern s32 D_00435DD0;

void brsMarkPartyRows(u8 *dst, u8 *state, s32 flags) {
    s32 i;

    for (i = 0; i < *(s32 *)(state + 0x78); i++) {
        u8 *d = dst;
        u8 *unit = *(u8 **)&D_00435DD0 + 0xA60;
        s32 j;

        for (j = 4; j >= 0; j--) {
            if ((*(u16 *)unit & 1) != 0) {
                s32 *row = (s32 *)(state + i * 24);
                if (*row == (s32)unit) {
                    *(s32 *)d |= flags;
                    if (flags & 2) {
                        *(s32 *)(d + 4) = row[1];
                    }
                }
            }
            d += 0x2C;
            unit += 0x1C4;
        }
    }
}

void brsMarkPartyRowsFromLists(u32 arg0, u32 arg1, u32 arg2) {
    brsMarkPartyRows(arg0, arg1, 2);
    brsMarkPartyRows(arg0, arg2, 1);
}

void brsTaskLatchPendingRows(s32 context) {
    if (*(s32 *)(context + 0x368) == 0) {
        D_00437989 = 0;
    } else {
        D_00437989 = 1;
    }
}

INCLUDE_ASM(const s32, "game/code_00296E98", func_00299578);

extern s32 func_00101958(void);
extern void func_003054E8(s32);
extern void func_002C3FC8(s32, s32);
extern s32 func_002993D0(s32);
extern void func_00299280(s32);
extern void func_00303D58(s32);
extern void func_0026C728(void);
extern void func_003297C8(s32);

void brsStaffTaskDestroy(s32 arg0) {
    s32 context = func_00101958();

    if (*(s32 *)(context + 0xAEB0) != 0) {
        func_003054E8(*(s32 *)(context + 0xAEB0));
    }
    func_002C3FC8(context + 8, arg0);
    if (func_002993D0(context) == 0) {
        func_00299280(context);
    }
    func_00303D58(*(s32 *)(context + 0x58));
    func_0026C728();
    func_003297C8(*(s32 *)context);
    D_00437988 = 2;
}

extern s32 kwlnTaskCreate(void *name, s32 arg1, s32 arg2, s32 arg3, void *update, void *destroy, void *data);
extern void *func_00299578(void);
extern void brsMessageInputStep(void);
extern void mnuStaffRunPanel1(void);
extern void mnuStaffRunPanel2(void);
extern void brsStaffTaskDestroy(s32);

s32 mnuStaffCreateTasks(void) {
    s32 result;
    void *work = func_00299578();

    kwlnTaskCreate(D_00437990, 0x405, 1, 0, brsMessageInputStep, 0, work);
    kwlnTaskCreate(D_00428388, 0x2B15, 1, 0, mnuStaffRunPanel1, 0, work);
    result = kwlnTaskCreate(D_00428398, 0x5211, 1, 0, mnuStaffRunPanel2, brsStaffTaskDestroy, work);
    D_00437988 = 1;
    return result;
}

u32 mnuStaffDestroyTasks(void) {
    s8 state = D_00437988;

    if (state == 1) {
        kwlnTaskDestroyWithHierarchyByName(D_00437990, 0);
        kwlnTaskDestroyWithHierarchyByName(D_00428388, 0);
        kwlnTaskDestroyWithHierarchyByName(D_00428398, 0);
        D_0043798B = state;
        return 1;
    }
    return 0;
}

s32 brsTaskConsumeDone(void) {
    s32 state = D_00437988;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        D_00437988 = 0;
    }
    return 0;
}

u32 brsTaskTryDestroy(void) {
    if (D_00437988 == 1) {
        mnuStaffDestroyTasks();
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00296E98", func_002998D8);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00299988);

s32 brsTaskIsFadeIdle(void) {
    if (kwlnFadeIsActive() != 0) {
        return 0;
    }
    return func_002C6CE8() != 1;
}

void mnuRefreshSelectedUnitPanels(u32 arg0, s32 arg1) {
    mnuInitPartyPanelSlots(arg1 + 0x584);
    func_002BCA98(arg1 + 0x690);
    func_002BCAB0(arg1 + 0x690);
}

void mnuStaffCopyPanelBlock(MenuPanelBlock *src, u8 *base) {
    *(MenuPanelBlock *)(base + 0xA0) = *src;
}

extern s32 effMiscRand(s32);
extern s32 D_003D6308[];

s32 func_00299B20(u32 arg0) {
    u32 roll = effMiscRand(0) & 0xFF;
    u32 i;

    for (i = 0; i < 5; i++) {
        if ((s32)roll < D_003D6308[arg0 * 5 + i]) {
            return i + 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00296E98", func_00299B98);

INCLUDE_RODATA(const s32, "game/code_00296E98", D_00428388);

INCLUDE_RODATA(const s32, "game/code_00296E98", D_00428398);

INCLUDE_RODATA(const s32, "game/code_00296E98", D_004283B0);

INCLUDE_RODATA(const s32, "game/code_00296E98", D_004283C0);

INCLUDE_SDATA(const s32, "game/code_00296E98", D_00437988);

INCLUDE_SDATA(const s32, "game/code_00296E98", D_00437989);

INCLUDE_SDATA(const s32, "game/code_00296E98", D_0043798A);

INCLUDE_SDATA(const s32, "game/code_00296E98", D_0043798B);

INCLUDE_SDATA(const s32, "game/code_00296E98", D_00437990);

INCLUDE_SDATA(const s32, "game/code_00296E98", D_00437998);

INCLUDE_SDATA(const s32, "game/code_00296E98", D_004379A0);

INCLUDE_SDATA(const s32, "game/code_00296E98", D_004379A8);

