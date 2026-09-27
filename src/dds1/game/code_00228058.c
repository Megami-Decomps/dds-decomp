#include "common.h"

extern u8 D_003BD88C;
extern u32 D_003BD890;
extern u32 D_003BD894;

extern u32 D_003BBDC0;
extern u64 func_00101A70(void);

extern u32 D_003BBDB8;

extern s32 D_003BAA00;

extern s64 func_001019C8(u64);
extern u64 func_0010D428(u64);

INCLUDE_ASM(const s32, "game/code_00228058", func_00228058);

INCLUDE_ASM(const s32, "game/code_00228058", func_002280B0);

INCLUDE_ASM(const s32, "game/code_00228058", func_00228180);

u32 func_00228210(void) {
    u64 temp_v0;
    s64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_001019C8(temp_v0);
    if (temp_v1 != 0) {
        func_00235088(temp_v0);
    }
    return 1;
}

u32 func_00228258(void) {
    u64 temp_v0;
    s64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_001019C8(temp_v0);
    if (temp_v1 != 0) {
        func_002350B0(temp_v0);
    }
    return 1;
}

u32 func_002282A0(void) {
    u64 temp_v0;
    s64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_001019C8(temp_v0);
    if (temp_v1 != 0) {
        func_002350E0(temp_v0);
    }
    return 1;
}

u32 func_002282E8(void) {
    u64 temp_v0;
    u64 temp_v1;
    u64 temp_v2;

    func_00132B60(0);
    func_00132B70(0x80);
    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    temp_v2 = func_0010D428(2);
    func_00132AE8(temp_v0, temp_v1, temp_v2);
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00228058", D_003ACAE0);

INCLUDE_ASM(const s32, "game/code_00228058", func_00228350);

u32 func_002283C8(void) {
    func_00235340(1);
    return 1;
}

u32 func_002283E8(void) {
    func_00235340(0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00228058", func_00228408);

u32 func_00228460(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    func_00241938(temp_v0, temp_v1);
    return 1;
}

u32 func_002284A0(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    func_00241990(temp_v0, temp_v1);
    return 1;
}

u32 func_002284E0(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    func_00241A50(temp_v0, temp_v1);
    return 1;
}

u32 func_00228520(void) {
    func_002E9708();
    func_001F3448();
    return 1;
}

u32 func_00228548(void) {
    func_002E9730();
    func_001F3448();
    return 1;
}

u32 func_00228570(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    func_00241A98(temp_v0, temp_v1);
    return 1;
}

u32 func_002285B0(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    func_00241AE8(temp_v0, temp_v1);
    return 1;
}

u32 func_002285F0(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    func_00241CA0(temp_v0, temp_v1);
    return 1;
}

u32 func_00228630(void) {
    func_0018F570();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00228058", func_00228650);

INCLUDE_ASM(const s32, "game/code_00228058", func_002286C0);

u8 func_002286E8(void) {
    return *(u8 *)(D_003BAA00 + 0xa41);
}

void func_002286F8(u8 arg0) {
    *(u8 *)(D_003BAA00 + 0xa41) = arg0 & 0xf;
    *(u32 *)(D_003BAA00 + 0xa44) = 0;
}

void func_00228710(void) {
    *(u8 *)(D_003BAA00 + 0xa40) = *(u8 *)(D_003BAA00 + 0xa40) | 1;
}

void func_00228728(void) {
    *(u8 *)(D_003BAA00 + 0xa40) = *(u8 *)(D_003BAA00 + 0xa40) & 0xfe;
}

INCLUDE_ASM(const s32, "game/code_00228058", func_00228740);

void func_00228778(void) {
    *(u8 *)(D_003BAA00 + 0xa40) = *(u8 *)(D_003BAA00 + 0xa40) & 0xfd;
    D_003BBDB8 = 0;
}

void func_00228790(void) {
    *(u8 *)(D_003BAA00 + 0xa40) = *(u8 *)(D_003BAA00 + 0xa40) | 2;
}

void func_002287A8(void) {
    *(u8 *)(D_003BAA00 + 0xa40) = *(u8 *)(D_003BAA00 + 0xa40) & 0xfd;
}

INCLUDE_ASM(const s32, "game/code_00228058", func_002287C0);

INCLUDE_ASM(const s32, "game/code_00228058", func_00228930);

INCLUDE_ASM(const s32, "game/code_00228058", func_00228A00);

void func_00228A58(void) {
    u64 temp_v0;

    temp_v0 = func_00101A70();
    func_00228B78(temp_v0);
    func_002CFF98(temp_v0);
    D_003BBDC0 = 0;
}

INCLUDE_ASM(const s32, "game/code_00228058", func_00228A98);

INCLUDE_ASM(const s32, "game/code_00228058", func_00228B00);

INCLUDE_ASM(const s32, "game/code_00228058", func_00228B38);

INCLUDE_ASM(const s32, "game/code_00228058", func_00228B78);

INCLUDE_ASM(const s32, "game/code_00228058", func_00228B98);

INCLUDE_ASM(const s32, "game/code_00228058", func_00228BF8);

INCLUDE_ASM(const s32, "game/code_00228058", func_00228C38);

INCLUDE_ASM(const s32, "game/code_00228058", func_00228CA0);

INCLUDE_ASM(const s32, "game/code_00228058", func_00228E20);

INCLUDE_ASM(const s32, "game/code_00228058", func_00228FB0);

INCLUDE_ASM(const s32, "game/code_00228058", func_00229100);

INCLUDE_ASM(const s32, "game/code_00228058", func_00229230);

INCLUDE_ASM(const s32, "game/code_00228058", func_00229320);

INCLUDE_ASM(const s32, "game/code_00228058", func_00229410);

INCLUDE_ASM(const s32, "game/code_00228058", func_002294C0);

INCLUDE_ASM(const s32, "game/code_00228058", func_00229500);

INCLUDE_ASM(const s32, "game/code_00228058", func_00229540);

INCLUDE_ASM(const s32, "game/code_00228058", func_00229750);

INCLUDE_ASM(const s32, "game/code_00228058", func_00229A10);

INCLUDE_ASM(const s32, "game/code_00228058", func_00229B70);

INCLUDE_ASM(const s32, "game/code_00228058", func_00229CD0);

INCLUDE_ASM(const s32, "game/code_00228058", func_00229D28);

INCLUDE_ASM(const s32, "game/code_00228058", func_00229D80);

INCLUDE_ASM(const s32, "game/code_00228058", func_00229E28);

INCLUDE_ASM(const s32, "game/code_00228058", func_00229ED8);

INCLUDE_ASM(const s32, "game/code_00228058", func_0022A248);

INCLUDE_ASM(const s32, "game/code_00228058", func_0022A850);

INCLUDE_ASM(const s32, "game/code_00228058", func_0022A8D8);

INCLUDE_ASM(const s32, "game/code_00228058", func_0022A960);

INCLUDE_ASM(const s32, "game/code_00228058", func_0022AAF0);

void func_0022AB00(s32 arg0) {
    if (arg0 == 0) {
        D_003BD88C = 0;
        D_003BD890 = 0;
        D_003BD894 = 0;
        return;
    }
    D_003BD894 = (s32)arg0;
    D_003BD88C = 3;
    D_003BD890 = 0;
}

INCLUDE_ASM(const s32, "game/code_00228058", func_0022AB28);

void func_0022AB58(void) {
}

u32 func_0022AB60(void) {
    func_0022AB58();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00228058", func_0022AB80);

INCLUDE_RODATA(const s32, "game/code_00228058", D_003ACBF8);

INCLUDE_ASM(const s32, "game/code_00228058", func_0022AB90);

void func_0022AEB8(void) {
    func_0010BDB8();
}

INCLUDE_ASM(const s32, "game/code_00228058", func_0022AED0);

INCLUDE_RODATA(const s32, "game/code_00228058", D_003ACD18);

INCLUDE_ASM(const s32, "game/code_00228058", func_0022AF18);

INCLUDE_RODATA(const s32, "game/code_00228058", D_003ACD38);

INCLUDE_RODATA(const s32, "game/code_00228058", D_003ACD48);

INCLUDE_RODATA(const s32, "game/code_00228058", D_003ACD58);

INCLUDE_RODATA(const s32, "game/code_00228058", D_003ACD68);

INCLUDE_RODATA(const s32, "game/code_00228058", D_003ACD78);

INCLUDE_ASM(const s32, "game/code_00228058", func_0022AF50);

INCLUDE_ASM(const s32, "game/code_00228058", func_0022B618);

INCLUDE_ASM(const s32, "game/code_00228058", func_0022B620);

INCLUDE_ASM(const s32, "game/code_00228058", func_0022B6C0);

INCLUDE_ASM(const s32, "game/code_00228058", func_0022B710);

INCLUDE_ASM(const s32, "game/code_00228058", func_0022B7A0);
