#include "common.h"

extern s32 func_002877A8(void);

extern s32 kwlnFadeIsActive(void);

extern s8 D_003BC529;

extern s8 D_003BC52A;

extern s8 D_003BC52B;

extern s8 D_003BC528;

INCLUDE_ASM(const s32, "game/code_00260208", func_00260208);

INCLUDE_ASM(const s32, "game/code_00260208", func_00260370);

typedef struct {
    u32 value;
    u32 mode;
} MenuCommand;

typedef struct {
    u8 pad00[0x30];
    MenuCommand *command; /* 0x30 */
    u8 pad34[0x74];
    s32 frames;           /* 0xA8 */
    s32 mode;             /* 0xAC: command phase, 0–3 */
} MenuCommandWork;

void func_00260530(MenuCommandWork *work, u32 value) {
    MenuCommand *command;

    command = work->command;
    if (command != (MenuCommand *)0x0) {
        command->value = value;
        command->mode = 1;
    }
}

void func_00260550(MenuCommandWork *work, u32 value) {
    MenuCommand *command;

    command = work->command;
    if (command != (MenuCommand *)0x0) {
        command->value = value;
        command->mode = 2;
    }
}

void func_00260570(MenuCommandWork *work, u32 value) {
    MenuCommand *command;

    command = work->command;
    if (command != (MenuCommand *)0x0) {
        command->value = value;
        command->mode = 1;
    }
}

void func_00260590(MenuCommandWork *work, u32 value) {
    MenuCommand *command;

    command = work->command;
    if (command != (MenuCommand *)0x0) {
        command->value = value;
        command->mode = 2;
    }
}

void func_002605B0(MenuCommandWork *work, u32 mode) {
    work->mode = mode;
    work->frames = 0;
}

s32 func_002605C0(MenuCommandWork *work) {
    s32 count;

    switch (work->mode) {
    case 0:
        count = work->frames + 1;
        work->frames = count;
        if ((f32)count > 10.0f) {
            return 0;
        }
        return -1;
    case 1:
        return 1;
    case 2:
        count = work->frames + 1;
        work->frames = count;
        if ((f32)count > 10.0f) {
            return 2;
        }
        return -1;
    case 3:
        return 3;
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_00260208", func_00260670);

INCLUDE_ASM(const s32, "game/code_00260208", func_002609D8);

INCLUDE_ASM(const s32, "game/code_00260208", func_00260AB0);

INCLUDE_ASM(const s32, "game/code_00260208", func_00261688);

INCLUDE_ASM(const s32, "game/code_00260208", func_00261760);

void func_00261F58(void) {
    func_00262818();
    D_003BC52A = 0;
    D_003BC52B = 1;
}

s8 func_00261F80(void) {
    return D_003BC52B;
}

u32 func_00261F88(void) {
    D_003BC52A = 1;
    return 1;
}

void func_00261F98(void) {
    func_00262938();
}

s8 func_00261FB0(void) {
    return D_003BC529;
}

s8 func_00261FB8(s32 arg0) {
    if (*(s32 *)(arg0 + 0xd44) != 0) {
        D_003BC52B = 0;
    }
    return D_003BC52B ? 0 : D_003BC52A;
}

typedef struct MenuIconRef {
    u16 id;
    u8 param;
    u8 pad3;
} MenuIconRef;

extern void func_00119900(s32, s32);

void func_00261FD8(MenuIconRef *refs) {
    u32 i;

    for (i = 0; i < 3; i++) {
        u16 id = refs->id;
        u8 param = refs->param;

        refs++;
        if (id != 0) {
            func_00119900(id, param);
        }
    }
}

void func_00262038(s32 arg0) {
    func_001198B8(*(u32 *)(arg0 + 0xc));
}

INCLUDE_ASM(const s32, "game/code_00260208", func_00262050);

INCLUDE_ASM(const s32, "game/code_00260208", func_00262148);

void func_002622B0(u32 arg0, u32 arg1, u32 arg2) {
    func_00261FD8(arg1);
    func_00262038(arg1);
    func_00262148(arg0, arg2);
}

extern void func_002762D8(s32 *);
extern void func_00271480(s32, s32 *, s32, s32);
extern s32 mnuCreatePanelGroup(s32);
extern void mnuUpdateFiveListEntries(s32, s32);
extern s32 mnuCreateSpriteState(s32, s32, s32);
extern void func_00287450(s32);
extern void mnuForwardTableByte(s32);

void func_00262300(s32 work) {
    s32 *group = (s32 *)(work + 0x4F8);
    s32 panel;

    func_002762D8(group);
    func_00271480(work + 0x680, group, 0, work + 0x574);
    panel = mnuCreatePanelGroup(*(s32 *)(work + 0x514));
    *(s32 *)(work + 0xD10) = panel;
    mnuUpdateFiveListEntries(panel, *(s32 *)(work + 0x90));
    *(s32 *)(work + 0xD14) = mnuCreateSpriteState(*(s32 *)(work + 0x50C), *(s32 *)(work + 0x500), *(s32 *)(work + 0x514));
    func_00287450(0);
    mnuForwardTableByte(*(u16 *)(*(s32 *)(work + *(s32 *)(work + 0x240) * 24 + 0x2CC) + 4));
}

void func_00262398(s32 arg0) {
    s32 panelContext;

    panelContext = arg0 + 0x680;
    func_002BDD60(*(u32 *)(arg0 + 0x90));
    mnuClearEntries(panelContext);
    func_0027FA20(panelContext);
    mnuShutdownContext(panelContext);
    mnuDestroyPanelGroup(*(u32 *)(arg0 + 0xd10));
    func_002832F8(*(u32 *)(arg0 + 0xd14));
    mnuReleaseAssets(arg0 + 0xd1c);
    func_00276320(arg0 + 0x4f8);
    func_00271648(arg0 + 0x4f8);
    mnuResetWorkFloats();
}

INCLUDE_RODATA(const s32, "game/code_00260208", D_003AFA88);

extern char D_003AFA88[];
extern char D_003AFA98[];
extern void func_002E9340(s32);
extern void initPartyPanelSlots(s32);
extern void func_00271500(s32, s32);
extern void effRequestResourceByMode(char *, char *, s32, s32);
extern void func_0027AEA8(s32);
extern void kwlnFadeInStart(s32, s32, s32, s32);

s32 func_00262418(s32 work) {
    if (*(s32 *)(work + 0x570) != 0) {
        return 0;
    }
    func_002E9340(0x50000);
    initPartyPanelSlots(work + 0x574);
    func_00271500(*(s32 *)(work + 0x58), work + 0x4F8);
    effRequestResourceByMode(D_003AFA88, D_003AFA98, 0, work + 0x90);
    func_0027AEA8(work + 0xD1C);
    *(s32 *)(work + 0x570) = 1;
    kwlnFadeInStart(0, 0, 0, 1);
    kwlnFadeInStart(0, 0, 0, 0);
    return 1;
}

extern s32 func_002716E8(s32, s32);
extern s32 func_0027AF28(s32);
extern void func_00262300(s32);
extern void kwlnFadeOutStart(s32, s32, s32, s32);

s32 func_002624C0(s32 work) {
    s32 state = *(s32 *)(work + 0x570);

    if (state == 0) {
        return 1;
    }
    if (state == 2) {
        return 0;
    }
    if (func_002716E8(*(s32 *)(work + 0x58), work + 0x4F8) == 0) {
        return 1;
    }
    if (func_002877A8() == 1) {
        return 1;
    }
    if (*(s32 *)(work + 0x90) == 0) {
        return 1;
    }
    if (func_0027AF28(work + 0xD1C) == 0) {
        return 1;
    }
    func_00262300(work);
    *(s32 *)(work + 0x570) = 2;
    kwlnFadeOutStart(0, 0, 0, 15);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00260208", func_00262570);

void func_00262600(u32 arg0, u32 arg1, u32 arg2) {
    func_00262570(arg0, arg1, 2);
    func_00262570(arg0, arg2, 1);
}

void func_00262640(s32 arg0) {
    if (*(s32 *)(arg0 + 0x344) == 0) {
        D_003BC529 = 0;
    } else {
        D_003BC529 = 1;
    }
}

INCLUDE_ASM(const s32, "game/code_00260208", func_00262660);

extern void func_00285600(s32, s32);
extern s32 func_002624C0(s32);
extern void func_00262398(s32);
extern void func_002BC618(s32);
extern void func_0024DBC8(void);
extern void func_002D0918(s32);

void func_00262790(s32 arg0) {
    s32 context = func_00101A70();

    if (*(s32 *)(context + 0xD44) != 0) {
        func_002BDD60(*(s32 *)(context + 0xD44));
    }
    func_00285600(context + 8, arg0);
    if (func_002624C0(context) == 0) {
        func_00262398(context);
    }
    func_002BC618(*(s32 *)(context + 0x58));
    func_0024DBC8();
    func_002D0918(*(s32 *)context);
    D_003BC528 = 2;
}

extern char D_003BC530[];
extern char D_003AFAB8[];
extern char D_003AFAC8[];
extern void func_00262EB8(void);
extern void func_00262F90(void);
extern void func_00262FF0(void);
extern s32 kwlnTaskCreate(void *name, s32 arg1, s32 arg2, s32 arg3, void *update, void *destroy, void *data);
extern void *func_00262660(void);

s32 func_00262818(void) {
    s32 result;
    void *work = func_00262660();

    kwlnTaskCreate(D_003BC530, 0x405, 1, 0, func_00262EB8, 0, work);
    kwlnTaskCreate(D_003AFAB8, 0x2B15, 1, 0, func_00262F90, 0, work);
    result = kwlnTaskCreate(D_003AFAC8, 0x5211, 1, 0, func_00262FF0, func_00262790, work);
    D_003BC528 = 1;
    return result;
}

extern char D_003BC530[];
extern char D_003AFAB8[];
extern char D_003AFAC8[];
extern void kwlnTaskDestroyWithHierarchyByName(char *, s32);

u32 func_002628C8(void) {
    s8 state = D_003BC528;

    if (state == 1) {
        kwlnTaskDestroyWithHierarchyByName(D_003BC530, 0);
        kwlnTaskDestroyWithHierarchyByName(D_003AFAB8, 0);
        kwlnTaskDestroyWithHierarchyByName(D_003AFAC8, 0);
        D_003BC52B = state;
        return 1;
    }
    return 0;
}

s32 func_00262938(void) {
    s32 state = D_003BC528;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        D_003BC528 = 0;
    }
    return 0;
}

u32 func_00262970(void) {
    if (D_003BC528 == 1) {
        func_002628C8();
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00260208", func_002629A8);

INCLUDE_ASM(const s32, "game/code_00260208", func_00262A30);

s32 func_00262A88(void) {
    if (kwlnFadeIsActive() != 0) {
        return 0;
    }
    return func_002877A8() != 1;
}

void func_00262AC0(u32 arg0, s32 menu) {
    initPartyPanelSlots(menu + 0x574);
    menuUpdateHandleStates(menu + 0x680);
    func_00280048(menu + 0x680);
}

typedef struct MenuPanelBlock {
    s32 data[0x69];
} MenuPanelBlock;

void func_00262AF8(MenuPanelBlock *src, u8 *base) {
    *(MenuPanelBlock *)(base + 0x9C) = *src;
}

extern s32 D_0036D3C0[];
extern s32 effMiscRand(s32);

s32 func_00262BA8(void) {
    u32 roll = effMiscRand(0) & 0xFF;
    u32 i;

    for (i = 0; i < 4; i++) {
        if ((s32)roll < D_0036D3C0[i]) {
            return i + 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00260208", func_00262C08);

INCLUDE_ASM(const s32, "game/code_00260208", func_00262CE8);

INCLUDE_RODATA(const s32, "game/code_00260208", D_003AFAB8);

INCLUDE_RODATA(const s32, "game/code_00260208", D_003AFAC8);

INCLUDE_RODATA(const s32, "game/code_00260208", D_003AFAD8);

INCLUDE_RODATA(const s32, "game/code_00260208", D_003AFAE8);

INCLUDE_SDATA(const s32, "game/code_00260208", D_003BC510);

INCLUDE_SDATA(const s32, "game/code_00260208", D_003BC518);

INCLUDE_SDATA(const s32, "game/code_00260208", D_003BC520);

INCLUDE_SDATA(const s32, "game/code_00260208", D_003BC528);

INCLUDE_SDATA(const s32, "game/code_00260208", D_003BC529);

INCLUDE_SDATA(const s32, "game/code_00260208", D_003BC52A);

INCLUDE_SDATA(const s32, "game/code_00260208", D_003BC52B);

INCLUDE_SDATA(const s32, "game/code_00260208", D_003BC530);

INCLUDE_SDATA(const s32, "game/code_00260208", D_003BC538);

INCLUDE_SDATA(const s32, "game/code_00260208", D_003BC540);

INCLUDE_SDATA(const s32, "game/code_00260208", D_003BC548);

