#include "common.h"

extern s32 func_0028A018(s32);
extern s32 func_00315FC8(u16);
extern s32 func_00314990(s32, u16);
extern s32 func_00314B78(s32);
extern s32 func_0026CF70(s16);
extern void func_0028D070(s32, s32, s32);
extern void func_00291118(void);
extern void mnuReleaseMiddleMantraSpriteSlots(void);
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

u32 mnuGetSelectedNodeValue(MenuContainer *object) {
    return object->list->selected->value;
}

s32 mnuGetNodeValueByIndex(MenuContainer *object, s32 index) {
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

u32 func_002890B8(MenuContainer *object) {
    func_002B8D10(object->list);
    func_002B8F98(object->list);
    return 1;
}

u32 func_002890F0(MenuContainer *object) {
    func_002B8CF0(object->list);
    func_002B8F98(object->list);
    return 1;
}

typedef struct MenuListCursor {
    u8 pad00[0x1C];
    s32 *index;
} MenuListCursor;

/* Steps the list cursor to `target` one entry at a time. */
s32 func_00289128(MenuContainer *object, s8 target) {
    s32 diff;
    s32 current;

    current = *((MenuListCursor *)object->list)->index;
    func_0010AE38("Jump!! %d to %d\n", current, target);
    diff = current - target;
    while (diff != 0) {
        if (diff > 0) {
            func_002B8D10(object->list);
            diff--;
            func_002B8F98(object->list);
        } else {
            func_002B8CF0(object->list);
            diff++;
            func_002B8F98(object->list);
        }
    }
    return 1;
}

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

/* Bit layout of the flag word at state+0x554 (only the fields this file sets). */
typedef struct MantraMenuBits {
    u32 pad0 : 2;
    u32 mode : 8;
    u32 pad10 : 4;
    u32 flag14 : 1;
    u32 pad15 : 17;
} MantraMenuBits;

extern void func_0028E858(s32 object);
extern void evtStageTestInit(s32 a);
extern void func_002A2200(s32 a);
extern void func_002A2388(void);

/* Opens the mantra menu: collects the list's node ids and starts the AT3 load. */
void func_00289DC8(s32 object) {
    s32 state = object + 0x240;
    MenuNode *node;
    s32 count;
    MenuNodeList *list;

    ((MantraMenuBits *)(state + 0x554))->flag14 = 0;
    ((MantraMenuBits *)(state + 0x554))->mode = 1;
    *(s32 *)(state + 0x560) = func_0026CF70(func_00314B78(*(s32 *)(*(s32 *)(*(s32 *)(object + 4) + 0x1C) + 0x70)));
    func_0028E858(object);
    list = ((MenuContainer *)object)->list;
    node = list->head;
    count = 0;
    for (; node != 0; node = node->next) {
        ((u32 *)(object + 0x7F4))[count++] = node->value;
    }
    *(s32 *)(state + 0x5D8) = count;
    *(s32 *)(state + 0x5D4) = **(s32 **)((s32)list + 0x1C);
    func_0028D070(object, 5, 0);
    evtStageTestInit(0);
    kwlnFadeOutStart(0, 0, 0, 0);
    func_0010AE38("AT3 LOAD!!\n");
    func_002A2408();
    func_002A2550();
    func_002A2200(0x10);
    func_002A2388();
}

extern void mnuDestroyMantraDrawPool(u32 address);
extern s64 func_0026D148(u32 *p);
extern void func_00279C38(u32 *sprite);

void func_00289ED0(s32 object) {
    s32 state;
    u32 *handle;
    s32 i;

    if (*(u32 *)(object + 0xC00) != 0) {
        mnuDestroyMantraDrawPool(*(u32 *)(object + 0xC00));
    }
    state = object + 0x240;
    handle = (u32 *)(object + 0x7AC);
    for (i = 5; i >= 0; i--) {
        if (*handle != 0) {
            func_0026D148((u32 *)*handle);
        }
        *handle = 0;
        handle++;
    }
    if (*(u32 *)(state + 0x9AC) != 0) {
        func_00279C38((u32 *)*(u32 *)(state + 0x9AC));
    }
}

void func_00289F58(void) {
    func_00291118();
    mnuReleaseMiddleMantraSpriteSlots();
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

s32 func_0028A178(MenuContainer *object) {
    MenuNode *node = object->list->head;
    s32 index = 0;
    while (node != 0) {
        if (func_0028A018(node->value) != 0) {
            return index;
        }
        node = node->next;
        index++;
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_00289058", func_0028A1D0);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028B1B0);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028B318);

INCLUDE_ASM(const s32, "game/code_00289058", func_0028B738);

typedef struct MantraMenuSrc {
    u16 bits;
} MantraMenuSrc;

typedef struct MantraMenuSrcState {
    u8 pad000[0x554];
    u32 low : 18;
    u32 kind : 8;
    u32 high : 6;
    u8 pad558[0x40C];
    MantraMenuSrc *src;
    u16 field968;
    u16 flag96A;
} MantraMenuSrcState;

extern MantraMenuSrc *func_0028FD10(void);

/* Binds the source record (or the default one) and unpacks its two bit fields. */
s32 func_0028BAF0(s32 object, MantraMenuSrc *src) {
    MantraMenuSrcState *state = (MantraMenuSrcState *)(object + 0x240);

    if (src != 0) {
        state->src = src;
    } else {
        state->src = func_0028FD10();
    }
    if (state->src != 0) {
        state->kind = 4;
        state->field968 = (state->src->bits >> 8) & 0xF;
        state->flag96A = (state->src->bits >> 12) & 1;
        return 1;
    }
    return 0;
}

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

typedef struct MantraMenuTimer {
    u8 pad00[0x5E1];
    s8 mode;
    u16 timer;
} MantraMenuTimer;

/* Advances the 6-frame countdown of mode 1 (-> 3) or mode 2 (-> 0). */
void func_0028CCC8(s32 object) {
    MantraMenuTimer *state = (MantraMenuTimer *)(object + 0x240);

    switch (state->mode) {
    case 0:
        break;
    case 1:
        state->timer++;
        if ((s16)state->timer >= 6) {
            state->mode = 3;
            state->timer = 0;
        }
        break;
    case 2:
        state->timer++;
        if ((s16)state->timer >= 6) {
            state->timer = 0;
            state->mode = 0;
        }
        break;
    }
}

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

