#include "common.h"

extern s32 func_0028A018(s32);
extern s32 func_00315FC8(u16);
extern s32 func_00314990(s32, u16);
extern s32 func_00314B78(s32);
extern s32 func_0026CF70(s16);
extern void func_0028D070(s32, s32, s32);
extern void func_00291118(void);
extern void func_0026E560(void);
extern void kwlnFadeOutStart(s32, s32, s32, s32);
extern void func_002A2408(void);
extern void func_002A2550(void);
extern void mdlFlagSet(u16);

typedef struct MenuNode {
    u8 pad0[0x58];
    struct MenuNode *next;
    u8 pad5C[0x14];
    u32 value;
} MenuNode;

typedef struct MenuNodeList {
    u8 pad0[0x10];
    MenuNode *head;
    u8 pad14[8];
    MenuNode *selected;
} MenuNodeList;

typedef struct MenuContainer {
    u8 pad0[4];
    MenuNodeList *list;
} MenuContainer;

u32 func_00289058(MenuContainer *object) {
    return object->list->selected->value;
}

s32 func_00289068(MenuContainer *object, s32 index) {
    MenuNode *node = object->list->head;
    s32 current = 0;
    while (node != 0) {
        if (current == index) {
            return node->value;
        }
        node = node->next;
        current++;
    }
    return 0;
}

u32 func_002890A8(MenuContainer *object) {
    return *(u32 *)object->list->selected;
}

u32 func_002890B8(s32 arg0) {
    func_002B8D10(*(u32 *)(arg0 + 4));
    func_002B8F98(*(u32 *)(arg0 + 4));
    return 1;
}

u32 func_002890F0(s32 arg0) {
    func_002B8CF0(*(u32 *)(arg0 + 4));
    func_002B8F98(*(u32 *)(arg0 + 4));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00289058", func_00289128);

INCLUDE_ASM(const s32, "game/code_00289058", func_002891C0);

INCLUDE_ASM(const s32, "game/code_00289058", func_002893A0);

INCLUDE_ASM(const s32, "game/code_00289058", func_00289550);

INCLUDE_ASM(const s32, "game/code_00289058", func_00289710);

INCLUDE_ASM(const s32, "game/code_00289058", func_00289928);

void func_00289B40(s32 object) {
    s32 state = object + 0x240;
    s16 id = func_00314B78(*(s32 *)(*(s32 *)(*(s32 *)(object + 4) + 0x1c) + 0x70));
    *(s32 *)(state + 0x560) = func_0026CF70(id);
    func_0028D070(object, 5, 0);
}

INCLUDE_ASM(const s32, "game/code_00289058", func_00289BA0);

INCLUDE_ASM(const s32, "game/code_00289058", func_00289DC8);

INCLUDE_ASM(const s32, "game/code_00289058", func_00289ED0);

void func_00289F58(void) {
    func_00291118();
    func_0026E560();
    kwlnFadeOutStart(0, 0, 0, 0);
    func_002A2408();
    func_002A2550();
}

s32 func_00289FA0(s32 object) {
    s32 index;
    for (index = 1; index < 0xb0; index++) {
        u16 id = index;
        if ((func_00315FC8(id) & 1) == 0 &&
            func_00314990(object, id) == 0) {
            return 0;
        }
    }
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00289058", D_00426370);

INCLUDE_RODATA(const s32, "game/code_00289058", D_00426380);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028A018);

s32 func_0028A0F8(s32 object) {
    u16 flagIds[6] = {0x9a0, 0x9a1, 0x9a2, 0x9a3, 0x9a4, 0x9a5};
    u8 flagIndices[9] = {0, 0, 1, 2, 3, 4, 5, 2, 1};
    mdlFlagSet(flagIds[flagIndices[*(u16 *)(object + 4)]]);
    return 1;
}

s32 func_0028A178(s32 object) {
    s32 node = *(s32 *)(*(s32 *)(object + 4) + 0x10);
    s32 index = 0;
    while (node != 0) {
        if (func_0028A018(*(s32 *)(node + 0x70)) != 0) {
            return index;
        }
        node = *(s32 *)(node + 0x58);
        index++;
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_00289058", func_0028A1D0);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028B1B0);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028B318);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028B738);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028BAF0);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028BB80);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028C8F8);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028CBF8);

void func_0028CCA8(s32 arg0) {
    *(u16 *)(arg0 + 0x822) = 0;
    *(u8 *)(arg0 + 0x821) = 1;
}

void func_0028CCB8(s32 arg0) {
    *(u16 *)(arg0 + 0x822) = 0;
    *(u8 *)(arg0 + 0x821) = 2;
}

INCLUDE_ASM(const s32, "game/code_00289058", func_0028CCC8);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028CD50);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028D070);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028D2F8);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028D7C8);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028DC08);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028DE10);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028DFA0);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028E0E8);

INCLUDE_SDATA(const s32, "game/code_00289058", D_00437930);

INCLUDE_SDATA(const s32, "game/code_00289058", D_00437938);

