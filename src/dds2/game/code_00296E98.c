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
    s32 data[0x69];
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

void func_00298E20(void) {
    mnuStaffCreateTasks();
    D_0043798A = 0;
    D_0043798B = 1;
}

s8 func_00298E48(void) {
    return D_0043798B;
}

u32 func_00298E50(void) {
    D_0043798A = 1;
    return 1;
}

void func_00298E60(void) {
    func_00299868();
}

s8 func_00298E78(void) {
    return D_00437989;
}

INCLUDE_ASM(const s32, "game/code_00296E98", func_00298E80);

void func_00298EA8(MenuIconRef *refs) {
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

void func_00298F08(MenuIconBatch *batch) {
    func_0011A0D0(batch->resource);
}

INCLUDE_ASM(const s32, "game/code_00296E98", ptyClampExp);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00299018);

void brsApplyRewardBundle(u32 arg0, u32 arg1, u32 arg2) {
    func_00298EA8(arg1);
    func_00298F08(arg1);
    func_00299018(arg0, arg2);
}

INCLUDE_ASM(const s32, "game/code_00296E98", func_002991D0);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00299280);

INCLUDE_RODATA(const s32, "game/code_00296E98", D_00428358);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00299320);

INCLUDE_ASM(const s32, "game/code_00296E98", func_002993D0);

INCLUDE_ASM(const s32, "game/code_00296E98", brsMarkPartyRows);

void func_00299518(u32 arg0, u32 arg1, u32 arg2) {
    brsMarkPartyRows(arg0, arg1, 2);
    brsMarkPartyRows(arg0, arg2, 1);
}

void brsTaskLatchPendingRows(s32 arg0) {
    if (*(s32 *)(arg0 + 0x368) == 0) {
        D_00437989 = 0;
    } else {
        D_00437989 = 1;
    }
}

INCLUDE_ASM(const s32, "game/code_00296E98", func_00299578);

INCLUDE_ASM(const s32, "game/code_00296E98", func_002996B8);

INCLUDE_ASM(const s32, "game/code_00296E98", mnuStaffCreateTasks);

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

s32 func_00299868(void) {
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

u32 func_002998A0(void) {
    if (D_00437988 == 1) {
        mnuStaffDestroyTasks();
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00296E98", func_002998D8);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00299988);

s32 func_00299A00(void) {
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

INCLUDE_ASM(const s32, "game/code_00296E98", mnuStaffCopyPanelBlock);

INCLUDE_ASM(const s32, "game/code_00296E98", func_00299B20);

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

