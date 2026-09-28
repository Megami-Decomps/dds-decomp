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
    u32 elapsed;          /* 0xA8 */
    u32 duration;         /* 0xAC */
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

void func_002605B0(MenuCommandWork *work, u32 duration) {
    work->duration = duration;
    work->elapsed = 0;
}

INCLUDE_ASM(const s32, "game/code_00260208", func_002605C0);

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

INCLUDE_ASM(const s32, "game/code_00260208", func_00261FD8);

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

INCLUDE_ASM(const s32, "game/code_00260208", func_00262300);

void func_00262398(s32 arg0) {
    s32 temp_v0;

    temp_v0 = arg0 + 0x680;
    func_002BDD60(*(u32 *)(arg0 + 0x90));
    clearMenuEntries(temp_v0);
    func_0027FA20(temp_v0);
    shutdownMenuContext(temp_v0);
    destroyPanelGroup(*(u32 *)(arg0 + 0xd10));
    func_002832F8(*(u32 *)(arg0 + 0xd14));
    releaseMenuAssets(arg0 + 0xd1c);
    func_00276320(arg0 + 0x4f8);
    func_00271648(arg0 + 0x4f8);
    menuResetWorkFloats();
}

INCLUDE_RODATA(const s32, "game/code_00260208", D_003AFA88);

INCLUDE_ASM(const s32, "game/code_00260208", func_00262418);

INCLUDE_ASM(const s32, "game/code_00260208", func_002624C0);

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

INCLUDE_ASM(const s32, "game/code_00260208", func_00262790);

INCLUDE_ASM(const s32, "game/code_00260208", func_00262818);

INCLUDE_ASM(const s32, "game/code_00260208", func_002628C8);

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

INCLUDE_ASM(const s32, "game/code_00260208", func_00262970);

INCLUDE_ASM(const s32, "game/code_00260208", func_002629A8);

INCLUDE_ASM(const s32, "game/code_00260208", func_00262A30);

s32 func_00262A88(void) {
    if (kwlnFadeIsActive() != 0) {
        return 0;
    }
    return func_002877A8() != 1;
}

void func_00262AC0(u32 arg0, s32 arg1) {
    func_00285A68(arg1 + 0x574);
    func_0027FF60(arg1 + 0x680);
    func_00280048(arg1 + 0x680);
}

INCLUDE_ASM(const s32, "game/code_00260208", func_00262AF8);

INCLUDE_ASM(const s32, "game/code_00260208", func_00262BA8);

INCLUDE_ASM(const s32, "game/code_00260208", func_00262C08);

INCLUDE_ASM(const s32, "game/code_00260208", func_00262CE8);

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

INCLUDE_RODATA(const s32, "game/code_00260208", D_003AFAB8);

INCLUDE_RODATA(const s32, "game/code_00260208", D_003AFAC8);

INCLUDE_RODATA(const s32, "game/code_00260208", D_003AFAD8);

INCLUDE_RODATA(const s32, "game/code_00260208", D_003AFAE8);

