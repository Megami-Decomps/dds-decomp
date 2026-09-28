#include "common.h"

extern s32 D_004363C4;

extern u32 D_004363C8;

extern u32 D_004363CC;

extern u32 D_004363D0;

extern u32 D_004363D4;

extern u32 D_00436380;

extern u32 D_00436384;

extern u32 D_00436388;

extern u32 D_0043638C;

extern u32 D_00436390;

extern u32 D_00436394;

extern u32 D_00436398;

extern u32 D_0043639C;

extern s32 D_004363AC;

extern s32 D_004363B0;

extern s32 D_004363B4;

extern u32 D_00436370;

extern s32 D_00436374;

extern s32 D_00436368;

extern s32 D_0043636C;

extern u32 D_00436324;

extern u32 D_00436328;

extern s32 D_0043632C;

extern s32 D_00435DD0;

extern u32 D_0043623C;

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

extern u64 func_00120858(void);

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

extern void *func_00328D68(s32 size);

extern void func_00101950(s32 arg0, void *arg1);

extern void func_00144400(void);

extern s32 kwlnTaskCreate(void *name, s32 arg1, s32 arg2, s32 arg3, void *update, void *destroy, void *data);

extern u8 D_00436208[];

extern s32 D_0043621C;

typedef struct {
    s32 unk0;
    s32 unk4;
    f32 unk8;
    f32 unkC;
    f32 unk10;
    f32 unk14;
} FldClear18; /* 0x18 bytes */

extern FldClear18 D_0044F730[];

extern s32 D_00389770[];

extern void func_00342580(s32 arg0);

extern s32 D_00389798[];

extern void func_00341C78();

extern void func_00129E50(u32 arg0, s32 arg1);

extern s32 func_00129D60(s32 arg0);

extern s32 D_00389780[];

extern s32 D_00436244;

extern s32 D_00436248;

extern s32 D_00389834[];

extern s32 D_00389838[];

extern s32 D_00436340;

extern f32 D_003A8E90[];

extern s32 D_00436344;

extern f32 D_003A8EA0[];

extern char D_00413C10[]; /* "fldTitle" */

extern s32 func_00101740(void *name);

extern char D_00413C80[]; /* "fldTitleMini" */

extern void func_001576A0(u32 arg0);

extern void func_001132F0(s32 arg0);

typedef struct {
    s32 unk0;
    u8 pad4[0x10];
} FldEnt14; /* 0x14 bytes */

extern FldEnt14 D_00451BE0[];

extern s32 D_00389884[];

extern u8 D_003A9EB0[];

extern void func_0026C538(void *arg0);

extern void func_0026C5B8(s32 arg0);

extern void func_00126000(void);

extern void *D_00451B94[];

extern s32 D_003897C0[];

extern u64 func_0010FFE8(void);

extern u32 *func_001111A8(u64 world, const char *name);

extern void func_0035B6E0(const char *fmt, ...);

extern s32 func_0010D650(s32);

extern s32 D_003897C8[];

extern s32 D_00389988[];

extern s32 func_0010D8C8(void);

extern s32 func_001420F0(s32);

extern s32 func_001421C0(s32);

extern s32 func_001422E8(s32);

extern s32 func_00142478(s32);

extern u32 func_00140750(void);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001442D0);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00144400);

void *func_00144498(s32 arg0) {
    s32 *temp_v0;

    temp_v0 = func_00328D68(0x10);
    temp_v0[0] = 0;
    temp_v0[1] = 0;
    temp_v0[2] = 0;
    temp_v0[3] = 0;
    func_00101950(arg0, temp_v0);
    return func_00144400;
}

void func_001444E8(void) {
    u64 temp_v0;

    temp_v0 = func_00101958();
    func_00328E48(temp_v0);
    D_00436200 = 0;
}

void func_00144510(void) {
    if (D_00436200 == 0) {
        D_00436200 = kwlnTaskCreate(D_00436208, 0x2B0B, 1, 1, func_00144498, func_001444E8, 0);
    }
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00144560);

void func_00144598(void) {
    FldClear18 *temp_v0 = D_0044F730;
    s32 temp_v1 = 7;

    do {
        temp_v1 -= 1;
        temp_v0->unk0 = 0;
        temp_v0->unk8 = 0;
        temp_v0->unk4 = 0;
        temp_v0 += 1;
    } while (temp_v1 >= 0);
    D_0043621C = 0;
}

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

void func_00144F08(void) {
    func_00342580(D_00436210);
    if (D_00389770[10] >= 0x80) {
        D_00389770[10] = 1;
    }
    D_00436210 = 0;
}

void func_00144F48(void) {
    func_00341CA8();
}

void func_00144F60(void) {
    func_00341C78(D_00436210);
    D_00389798[0] = 0;
}

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

void func_001453D0(s32 arg0, s32 arg1) {
    func_00341E20(arg0 * 0x10000 + arg1 + 0x30000000, 0x7f, 0x3f);
}

void func_00145400(s32 arg0, s32 arg1) {
    func_00341C78(arg0 * 0x10000 + arg1 + 0x30000000);
}

void func_00145428(void) {
    D_00436238 = 0;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145430);

void func_001454B8(void) {
    D_0043623C = 0;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_001454C0);

u32 func_00145548(void) {
    return D_00436210;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145550);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00145698);

void func_00145730(s32 arg0, s32 arg1) {
    if (D_00389780[0] < 0xC8) {
        s32 temp_a0 = arg0;
        s32 temp_a1 = arg1;
        s32 temp_v0 = temp_a0 + 8;

        D_00436278 = temp_a1;
        func_00129E50(arg0, temp_v0);
        {
            s32 temp_v1 = func_00129D60(temp_v0);
            s32 temp_4 = *(s32 *)(temp_v1 + 4);
            s32 temp_8 = *(s32 *)(temp_v1 + 8);

            D_00436274 = temp_8;
            D_00436270 = temp_4;
        }
    }
}

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

void func_0014A9F0(s32 arg0, s32 arg1, s32 arg2) {
    D_00389770[4] = arg0;
    D_00389770[6] = arg2;
    D_00436248 = D_00389770[5] = arg1;
    D_00436244 = arg0 % 100;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014AA28);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014AA90);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014AB38);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014ABA8);

void func_0014AC18(s32 arg0) {
    if (arg0 >= 0x40) {
        D_00389834[0] = -1;
    } else {
        D_00389834[0] = arg0;
    }
}

void func_0014AC40(s32 arg0) {
    if (arg0 >= 0x40) {
        D_00389838[0] = -1;
    } else {
        D_00389838[0] = arg0;
    }
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014AC68);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014AD38);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014AE08);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014AE58);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B010);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B050);

void func_0014B088(void) {
    func_0014A880();
    func_00148A98();
}

void func_0014B0A8(void) {
    u64 *puVar1;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v2;

    temp_v2 = 0;
    temp_v1 = D_00435DD0;
    do {
        puVar1 = (u64 *)(temp_v1 + 0xfcd0);
        temp_v0 = 0x3f;
        do {
            temp_v0 = temp_v0 - 1;
            *puVar1 = 0;
            puVar1 = puVar1 + 1;
        } while (-1 < temp_v0);
        temp_v2 = temp_v2 + 1;
        temp_v1 = temp_v1 + 0x200;
    } while (temp_v2 < 10);
}

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413788);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_004137B8);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_004137C8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B0F8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B440);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B520);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B5D8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B6F0);

void func_0014B748(void) {
    if (D_0043632C != 0) {
        func_00157658(D_0043632C);
        D_0043632C = 0;
        func_003298C0(D_00436324);
        D_00436324 = 0;
        D_00436328 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B788);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B7F8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014B860);

void func_0014B940(f32 arg0, f32 arg1, f32 arg2) {
    D_00436340 = 1;
    D_003A8E90[0] = arg0;
    D_003A8E90[1] = arg1;
    D_003A8E90[2] = arg2;
}

void func_0014B960(f32 arg0, f32 arg1, f32 arg2) {
    D_00436344 = 1;
    D_003A8EA0[0] = arg0;
    D_003A8EA0[1] = arg1;
    D_003A8EA0[2] = arg2;
}

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

s32 func_0014E670(void) {
    return func_00101740(D_00413C10) != 0;
}

void func_0014E698(void) {
    D_0043635C = 0;
    D_00436354 = 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014E6A8);

void func_0014EBB8(void) {
    if (D_00436368 != 0) {
        func_0032BBB0(D_00436368);
        D_00436368 = 0;
    }
    if (D_0043636C != 0) {
        func_0032BBB0(D_0043636C);
        D_0043636C = 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413C10);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014EC00);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014ED50);

s32 func_0014ED90(void) {
    return func_00101740(D_00413C80) != 0;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014EDB8);

void func_0014F138(void) {
    if (D_0043637C != 0) {
        func_0032BBB0(D_0043637C);
        D_0043637C = 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413C80);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014F168);

void func_0014F2F8(void) {
    if (D_00436374 == 0) {
        D_00436370 = 0;
        D_00436374 = 1;
    }
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014F318);

void func_0014F408(void) {
    if (D_004363AC != 0) {
        func_0032BBB0(D_004363AC);
        D_004363AC = 0;
    }
    if (D_004363B0 != 0) {
        func_0032BBB0(D_004363B0);
        D_004363B0 = 0;
    }
    if (D_004363B4 != 0) {
        func_0032BBB0(D_004363B4);
        D_004363B4 = 0;
    }
    func_00157658(D_00436388);
    D_00436388 = 0;
    D_0043638C = 0;
    func_003298C0(D_00436380);
    D_00436380 = 0;
    D_00436384 = 0;
    func_00157658(D_00436398);
    D_00436398 = 0;
    D_0043639C = 0;
    func_003298C0(D_00436390);
    D_00436390 = 0;
    D_00436394 = 0;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014F4A8);

void func_0014F560(void) {
    if (D_00436388 != 0 && D_0043638C != 0) {
        func_001576A0(D_00436388);
    }
    if (D_00436398 != 0 && D_0043639C != 0) {
        func_001576A0(D_00436398);
    }
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014F5B0);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413DB8);

INCLUDE_RODATA(const s32, "game/code_001442D0", D_00413DC8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_0014F5F0);

void func_0014F788(void) {
    FldEnt14 *entry = D_00451BE0;
    s32 i = 0x10;

    do {
        s32 temp = entry->unk0;

        i--;
        if (temp != 0) {
            func_001132F0(temp);
            entry->unk0 = 0;
        }
        entry++;
    } while (i >= 0);
}

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

void func_00151480(void) {
    func_0014F560();
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00151498);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001514A8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001514B8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001514C8);

INCLUDE_ASM(const s32, "game/code_001442D0", func_001514F8);

void func_001515A0(void) {
    if (D_004363C4 != 0) {
        func_003298C0(D_004363C4);
        D_004363C4 = 0;
        D_004363C8 = 0;
        D_004363CC = 0;
        D_004363D0 = 0;
        D_004363D4 = 0;
    }
}

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

s32 func_00154AE8(void) {
    D_003897C0[0] = 3;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00154B00);

INCLUDE_ASM(const s32, "game/code_001442D0", func_00154CE0);

s32 func_00154E08(void) {
    char *temp_v0;

    temp_v0 = func_0010D7D0(0);
    D_00389770[0x14] = 5;
    strcpy((char *)D_00389770 + 0x40, temp_v0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00154E48);

void func_00154F18(void) {
}

u32 func_00154F20(void) {
    return 1;
}

u32 func_00154F28(void) {
    func_0012A078();
    return 1;
}

s32 func_00154F48(const char *name) {
    u32 *entry = func_001111A8(func_0010FFE8(), name);
    if (entry != 0) {
        return entry[1];
    }
    func_0035B6E0("field SEARCH_ID NotFound:[%s]\n", name);
    return -1;
}

INCLUDE_ASM(const s32, "game/code_001442D0", func_00154FA0);

s32 func_00155020(void) {
    s32 temp_v0;

    temp_v0 = func_0010D650(0);
    D_003897C8[0] = temp_v0;
    return 1;
}

s32 func_00155048(void) {
    s32 temp_v0;

    D_00389988[11] = func_0010D650(0);
    temp_v0 = func_0010D650(1);
    func_00135A68(D_00389988[11], temp_v0);
    return 1;
}

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

u32 func_00156910(void) {
    if (func_00123788(func_0013EA78(*(s32 *)(func_0010D8C8() + 0xE4))) != 0) {
        func_0010D818(1);
    } else {
        func_0010D818(0);
    }
    return 1;
}

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

u32 func_001569E8(void) {
    s32 temp_v0;

    temp_v0 = func_0010D650(0);
    temp_v0 = func_001420F0(temp_v0);
    func_0010D818(temp_v0);
    return 1;
}

u32 func_00156A18(void) {
    s32 temp_v0;

    temp_v0 = func_0010D650(0);
    temp_v0 = func_001421C0(temp_v0);
    func_0010D818(temp_v0);
    return 1;
}

u32 func_00156A48(void) {
    s32 temp_v0;

    temp_v0 = func_0010D650(0);
    temp_v0 = func_001422E8(temp_v0);
    func_0010D818(temp_v0);
    return 1;
}

u32 func_00156A78(void) {
    s32 temp_v0;

    temp_v0 = func_0010D650(0);
    temp_v0 = func_00142478(temp_v0);
    func_0010D818(temp_v0);
    return 1;
}

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

void func_00156BD0(void) {
    func_00151350();
}

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

s32 func_00156F18(void) {
    func_0010D818(func_00140750());
    return 1;
}

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
INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436208);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436210);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436214);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043621C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436220);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436224);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436228);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043622C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436230);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436234);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436238);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043623C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436240);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436244);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436248);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043624C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436250);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436254);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436258);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043625C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436260);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436264);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436268);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043626C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436270);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436274);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436278);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043627C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436280);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436284);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436288);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043628C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436290);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436294);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436298);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043629C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362A0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362A4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362A8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362AC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362B0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362B4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362B8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362BC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362C0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362C4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362C8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362CC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362D0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362D4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362D8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362DC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362E0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362E4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362E8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362EC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362F0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362F4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362F8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004362FC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436300);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436304);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436308);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043630C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436310);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436314);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436318);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043631C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436320);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436324);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436328);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043632C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436330);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436338);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043633C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436340);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436344);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436348);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043634C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436350);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436354);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436358);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043635C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436360);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436364);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436368);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043636C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436370);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436374);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436378);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043637C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436380);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436384);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436388);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043638C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436390);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436394);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_00436398);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_0043639C);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363A0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363A4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363A8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363AC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363B0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363B4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363B8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363BC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363BE);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363C0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363C2);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363C4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363C8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363CC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363D0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363D4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363D8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363DC);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363E0);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363E4);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363E8);

INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363F0);


INCLUDE_SDATA(const s32, "game/code_001442D0", D_004363F8);

