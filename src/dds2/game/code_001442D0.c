#include "common.h"

extern u32 D_00436200;

extern u64 func_00101958(void);

extern u32 D_00436210;

extern u32 D_00436234;

extern u32 D_00436238;

extern u32 D_00436270;

extern u32 D_00436274;

extern s32 D_00436278;

extern u32 D_0043626C;

extern u32 D_00436364;

extern u32 D_00436354;

extern u32 D_0043635C;

extern s32 D_0043637C;

extern u64 func_0013EA78(u32);

extern u64 func_001420F0(u64);

extern u64 func_001421C0(u64);

extern u64 func_001422E8(u64);

extern u64 func_00142478(u64);

extern u64 func_00120858(void);

/* Script VM stack accessors (see script/scrTraceCode.c). */
extern s32 func_0010D650(s32 idx);

extern void func_0010D818(s32 value);

extern s32 func_00140780(s32 value);

extern s32 func_0013FA98(s32 param0, s32 param1);

extern s32 func_0013FFF8(s32 param0, s32 param1);

extern void func_00140238(void);

extern void func_00144EE0(void);

extern void func_00144F60(void);

extern void func_00144F08(void);

extern void func_00144F88(s32 param);

extern void func_00145078(s32 param);

extern s32 func_00145360(s32 param);

extern void func_001453D0(s32 param0, s32 param1);

extern void func_00145400(s32 param0, s32 param1);

extern void func_0014EC00(s32 param0, s32 param1, s32 param2);

extern void *func_0010D8C8(void);

extern s32 func_0013EAD0(u32 key);

extern void func_00140A58(void *entry);

/* Work object queried by func_0014F408; +0xE4 holds the key for func_0013BEE8. */
typedef struct {
    u8 unk00[0xE4]; /* 0x00 */
    u32 key;        /* 0xE4 */
} EffCmdWork;

extern void func_0014BF98(s32 handle);

extern s32 func_0023CC00(s32 param);

extern char *func_0010D7D0(s32 idx);

extern s32 func_0014E570(char *str);

extern u64 func_0014E620(void);

extern u64 func_00342688(void);

extern u64 func_00140750(void);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001442D0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00144400);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00144498);

void func_001444E8(void) {
    u64 temp_v0;

    temp_v0 = func_00101958();
    func_00328E48(temp_v0);
    D_00436200 = 0;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00144510);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00144560);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00144598);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001445D0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001446E8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001447A0);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_004136C0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00144B50);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00144C20);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00144CB8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00144DB0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00144DF8);

void func_00144EE0(void) {
    func_00341C00(D_00436210);
    D_00436210 = 0;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00144F08);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00144F48);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00144F60);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00144F88);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145078);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145118);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145160);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145228);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001452D8);

u32 func_00145358(void) {
    return D_00436234;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145360);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001453D0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145400);

void func_00145428(void) {
    D_00436238 = 0;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145430);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001454B8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001454C0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145548);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145550);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145698);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145730);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145790);

void func_001457E0(void) {
    if (D_00436278 != 0) {
        func_003298C0(D_00436278);
    }
    D_00436278 = 0;
    D_00436270 = 0;
    D_00436274 = 0;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145818);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001458A0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145948);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145A08);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145D48);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001460D8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00146150);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001461F0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00146250);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00148188);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00148488);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00148A98);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00149A00);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00149C38);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00149CE0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00149E08);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00149FE8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014A228);

u32 func_0014A250(void) {
    return D_0043626C;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014A258);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014A880);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014A9F0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014AA28);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014AA90);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014AB38);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014ABA8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014AC18);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014AC40);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014AC68);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014AD38);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014AE08);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014AE58);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B010);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B050);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B088);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B0A8);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413788);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_004137B8);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_004137C8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B0F8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B440);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B520);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B5D8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B6F0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B748);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B788);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B7F8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B860);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B940);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B960);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B980);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B9E0);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413800);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413818);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413830);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413848);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413860);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014BA60);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014BD20);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014BDE0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014BEC0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014BF10);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014BF98);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413AE8);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413AF8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014C2B8);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413B38);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413B48);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014C9B0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014D008);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014D060);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014D0E8);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413B68);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413B78);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014D380);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014D7B8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014D838);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014DB50);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014DDD8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014DEA8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014E308);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014E3A8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014E4B8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014E570);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014E620);

void func_0014E668(u32 arg0) {
    D_00436364 = arg0;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014E670);

void func_0014E698(void) {
    D_0043635C = 0;
    D_00436354 = 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014E6A8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014EBB8);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413C10);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014EC00);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014ED50);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014ED90);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014EDB8);

void func_0014F138(void) {
    if (D_0043637C != 0) {
        func_0032BBB0(D_0043637C);
        D_0043637C = 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413C80);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014F168);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014F2F8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014F318);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014F408);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014F4A8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014F560);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014F5B0);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413DB8);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413DC8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014F5F0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014F788);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014F7E0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014F8B8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014F940);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014F980);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014FB58);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014FC88);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014FD28);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00150080);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00150138);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00150800);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001509E0);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413DE8);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413DF8);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413E48);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413E98);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00150A60);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00150F10);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00150F20);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001512D8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001512E8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00151350);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001513C0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00151400);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00151468);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00151480);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00151498);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001514A8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001514B8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001514C8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001514F8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001515A0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001515E0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00151760);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001517D0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00151868);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00151918);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001519E8);

void func_00151DD8(void) {
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00151DE0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00151E00);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00151FF8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001521F0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00152230);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00152390);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001523B0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001523D0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001523F0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001525F0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001526B8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00152C18);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00152C48);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00152C88);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00152FB8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00153068);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001533D0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00153410);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00153520);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00153560);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413F68);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413F78);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001536B8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00153D40);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00414000);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00153D60);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00153FA0);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00414020);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00414030);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001540E8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001542D8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00154528);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00154540);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00154558);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00154628);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001546F8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00154758);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001547F0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00154828);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00154870);

u32 func_00154880(void) {
    func_00128380(0x10);
    return 1;
}

u32 func_001548A0(void) {
    func_00128390(0x10);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_001548C0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00154940);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00154AE8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00154B00);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00154CE0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00154E08);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00154E48);

void func_00154F18(void) {
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00154F20);

u32 func_00154F28(void) {
    func_0012A078();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00154F48);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00154FA0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00155020);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00155048);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00155090);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00155108);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001552E0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00155498);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00155690);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00155848);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00155A08);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00155BA8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00155D48);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00155ED0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00155F90);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156070);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001560F8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156180);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156208);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156290);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156330);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001563B8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156440);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156538);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156630);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156700);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156738);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156770);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001567A8);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00414090);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001567E0);

u32 func_001568E0(void) {
    s32 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D8C8();
    temp_v1 = func_0013EA78(*(u32 *)(temp_v0 + 0xe4));
    func_001236E8(temp_v1);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156910);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156960);

u32 func_001569B8(void) {
    u64 temp_v0;

    temp_v0 = func_0010D650(0);
    func_001411F8(temp_v0);
    return 1;
}

u32 func_001569E0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_001569E8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156A18);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156A48);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156A78);

u32 func_00156AA8(void) {
    u64 temp_v0;

    temp_v0 = func_0010D650(0);
    func_00142670(temp_v0);
    return 1;
}

u32 func_00156AD0(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D650(0);
    temp_v1 = func_0010D650(1);
    func_001206D0(temp_v0, temp_v1);
    return 1;
}

u32 func_00156B10(void) {
    func_00120820();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156B30);

/* Persona 4 func_002993c0 @ 002993C0 (src/Script/scrCommonCommand.c), recompiled unchanged */
s32 func_00156B58(void) {
    func_0010D818(func_00140780(func_0010D650(0)));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156B88);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156BD0);

s32 func_00156BE8(void) {
    s32 param0 = func_0010D650(0);
    s32 param1 = func_0010D650(1);

    func_0010D818(func_0013FA98(param0, param1));
    return 1;
}

s32 func_00156C30(void) {
    s32 param0 = func_0010D650(0);
    s32 param1 = func_0010D650(1);

    func_0010D818(func_0013FFF8(param0, param1));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156C78);

s32 func_00156C98(void) {
    func_00140238();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156CB8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156CE8);

s32 func_00156D18(void) {
    func_00144EE0();
    return 1;
}

s32 func_00156D38(void) {
    func_00144F60();
    return 1;
}

s32 func_00156D58(void) {
    func_00144F08();
    return 1;
}

s32 func_00156D78(void) {
    func_00144F88(func_0010D650(0));
    return 1;
}

s32 func_00156DA0(void) {
    func_00145078(func_0010D650(0));
    return 1;
}

u8 func_00156DC8(void) {
    return func_00145360(func_0010D650(0)) != 0;
}

s32 func_00156DF0(void) {
    s32 param0 = func_0010D650(0);
    s32 param1 = func_0010D650(1);

    func_001453D0(param0, param1);
    return 1;
}

s32 func_00156E30(void) {
    s32 param0 = func_0010D650(0);
    s32 param1 = func_0010D650(1);

    func_00145400(param0, param1);
    return 1;
}

s32 func_00156E70(void) {
    s32 param0 = func_0010D650(0);
    s32 param1 = func_0010D650(1);

    func_0014EC00(param0, param1, 0x3c);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156EB8);

s32 func_00156EE0(void) {
    EffCmdWork *work = func_0010D8C8();
    void *entry = func_0013EAD0(work->key);

    if (entry != NULL) {
        func_00140A58(entry);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156F18);

s32 func_00156F40(void) {
    func_0014BF98(func_0023CC00(func_0010D650(0)));
    return 1;
}

/* Persona 4 func_001eb2a0 @ 001EB2A0 (src/promoted/code1_001e.c), recompiled unchanged */
s32 func_00156F70(void) {
    char *param = func_0010D7D0(0);

    func_0010D818(func_0014E570(param));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156FA0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00156FC8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001570C0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00157130);

u32 func_00157180(void) {
    func_00342690();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_001571A0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001571C8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00157258);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00157268);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001572A0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00157308);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00157330);
