#include "mnu.h"

extern void func_0024DD78(void);

extern s32 kwlnTaskGetUserValue();

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00273AB0);

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00273B98);

u32 func_00273C40(void) {
    return 1;
}

u32 func_00273C48(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00273C50);

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00273D40);

s64 func_00273DE8(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    return menuRunPanel(context, 2, callback);
}

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00273E20);

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00273F20);

s64 func_00274008(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    return menuRunPanel(context, 2, callback);
}

u32 func_00274040(void) {
    return 1;
}

u32 func_00274048(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00274050);

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00274228);

s64 func_00274310(s32 callback) {
    s32 context = kwlnTaskGetUserValue();
    return menuRunPanel(context, 2, callback);
}

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00274348);

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00274430);

extern u16 mnuGetPartyEntryMenuValue(s32);
extern void func_0024DD90(s32, void *);
extern void dspStartEntry(s32);
extern void func_00283BF0(s32, s32);
extern void func_00119900(s32, s32);
extern u8 *D_003BAA70;
extern u8 *D_003BAA84;

typedef struct MnuEquipUnit {
    u8 pad00[4];
    u16 unitId;              /* 0x04 */
} MnuEquipUnit;

typedef struct MnuEquipContext {
    u8 pad00[0x1C];
    s32 previousItem;        /* 0x1C */
    s32 selectedItem;        /* 0x20 */
} MnuEquipContext;

typedef struct MnuEquipScene {
    u8 pad00[0x90C];
    MnuEquipContext *context; /* 0x90C */
} MnuEquipScene;

/* Swap the equipped bullet item: update the actor/old/new message tokens,
 * adjust inventory counts, and latch the old/new IDs in the menu context. */
void mnuSwapEquippedBullet(s32 scene, u8 *unit, s32 itemId) {
    MnuEquipContext *equipContext = ((MnuEquipScene *)scene)->context;
    s32 equipped = mnuGetPartyEntryMenuValue((s32)unit);

    func_00283BF0(scene + 0x914, 1);
    if (equipped != itemId) {
        /* Actor names use 17-byte records; item names use 25-byte records. */
        func_0024DD90(0, D_003BAA70 + ((MnuEquipUnit *)unit)->unitId * 17);
        func_0024DD90(1, D_003BAA84 + equipped * 25);
        func_0024DD90(2, D_003BAA84 + itemId * 25);
        dspStartEntry(0);
        if (equipped != 0) {
            func_00119900(equipped, 1);
        }
        func_00119900(itemId, -1);
        equipContext->previousItem = equipped;
        equipContext->selectedItem = itemId;
    } else {
        func_0024DD90(0, D_003BAA84 + equipped * 25);
        dspStartEntry(1);
        equipContext->previousItem = 0;
        equipContext->selectedItem = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00274610);

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00274768);

INCLUDE_ASM(const s32, "game/code_00273AB0", func_00274978);

s64 func_00274B30(s32 selection) {
    s32 context = kwlnTaskGetUserValue();

    func_0024DD78();
    return menuRunPanel(context, 2, selection);
}

u32 func_00274B78(void) {
    return 1;
}
