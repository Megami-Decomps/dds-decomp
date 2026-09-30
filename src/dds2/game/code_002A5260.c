#include "common.h"

/* Sliding menu bar: direction flag and 0..max position */
typedef struct { s32 active; s32 pos; } SlideBar;

extern u64 scrReadIntParameter(u64);

extern u32 D_00437A2C;

extern s32 D_00437A40;

extern u16 D_00435BAC;

extern u32 *D_00437AB0;

extern s32 sdfCheckPendingWorkWithInterrupts(void);

extern u32 D_00437AE8;

extern s32 kwlnFadeIsActive(void);

extern s32 func_00101958();

extern s32 D_004379F8;

extern char D_00429938[]; /* "staffImageProc" */

extern char D_00429968[]; /* "staffProc" */

extern u8 D_003E5608[];

extern u8 D_003803C8[];

extern u32 D_00437ACC;

extern u8 D_00457E60[];

extern char D_0042A418[];

extern u32 D_00457E48[];

extern s32 func_002A88A0();

extern u8 D_00437AC0[];

extern char D_00437B78[]; /* "camp" */

extern char D_0042AA08[]; /* "camp_draw" */

extern char D_0042AA18[]; /* "camp_update" */

extern s8 D_00437B72;

extern u8 D_00437A00[];

extern u8 D_00437A08[];

extern u8 *D_00435DD0;

extern s8 D_00437B73;

extern u32 D_004379C8[];

extern s8 D_004379FC;

extern s32 D_00437A18;

extern s32 D_00437A1C;

extern u8 D_00437A10[];

extern u32 D_00437A20[2];

extern u32 D_00437A28;

extern u32 D_00437A38;

extern void func_002A5F40(void);

extern void func_003458E8(u32);

extern void func_003297C8(u32);

extern void func_002A5A78();

extern u8 D_00437A48[];

void func_002A2408(void);

void func_002A2550(void);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A5260);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A55B8);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A5890);

void func_002A58C0(void) {
    u8 *state = (u8 *)D_00437A40;

    *(u32 *)(state + 0x28) = 0;
    *(u32 *)(state + 0x14) = 0;
    *(u32 *)(state + 0x1C) = 0;
}

void func_002A58D8(void) {
    u8 *state = (u8 *)D_00437A40;

    *(u32 *)(state + 0x28) = 0;
    *(u32 *)(state + 0x14) = 0;
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A58E8);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A5A20);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A5A78);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A5B08);

void mnuTitleResetSequenceTimers(void) {
    u32 *title = (u32 *)D_00437A40;
    title[0x28 / 4] = 1;
    title[0x1C / 4] = title[0x14 / 4] = 0;
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A5C58);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A5E00);

void func_002A5EE8(u32 arg0, s32 arg1) {
    s32 temp_v0;

    func_002A7AF0();
    temp_v0 = D_00437A40;
    if (D_00437A40 != 0) {
        *(u32 *)(D_00437A40 + 0x10c) = 1;
        if (arg1 == 0) {
            *(u32 *)(temp_v0 + 0x110) = 0x80;
        }
        else {
            *(u32 *)(temp_v0 + 0x110) = 0;
        }
        *(u32 *)(D_00437A40 + 0x114) = 0;
    }
}

void func_002A5F40(void) {
    func_002A7FD0();
    if (D_00437A40 != 0) {
        *(u32 *)(D_00437A40 + 0x10c) = 0;
    }
}

u32 func_002A5F68(void) {
    u32 temp_v0;

    temp_v0 = 0;
    if (D_00437A40 != 0) {
        temp_v0 = *(u32 *)(D_00437A40 + 0x10c);
    }
    return temp_v0;
}

extern void func_003458F0(u32, u32, u32, u32, u32);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A5F80);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004287E0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004287F8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428810);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428820);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428840);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428860);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428870);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428880);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428898);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004288A8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004288B8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004288C8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004288D8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004288E8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004288F8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428908);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428920);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428930);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428948);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428958);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428968);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428978);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428998);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004289A8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004289C0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004289D8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004289E8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004289F8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428A10);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428A28);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428A38);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428A48);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428A60);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428A70);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428A80);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428A90);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428AA0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428AB8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428AC8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428AD8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428AE8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428AF8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428B08);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428B20);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428B38);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428B50);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428B68);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428B78);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428B90);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428BA0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428BC0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428BD8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428BF0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428C08);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428C18);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428C28);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428C38);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428C48);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428C58);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428C68);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428C78);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428C88);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428C98);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428CB0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428CD0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428CE0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428CF8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428D10);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428D30);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428D40);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428D60);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428D70);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428D80);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428D90);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428DA0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428DB0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428DC8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428DD8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428DE8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428E00);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428E18);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428E30);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428E40);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428E58);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428E70);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428E88);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428EA0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428EB8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428EC8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428ED8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428EE8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428F00);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428F18);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428F28);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428F38);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428F50);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428F60);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428F70);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428F80);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428F90);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428FA0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428FB0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428FC8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428FD8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428FE8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00428FF8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429008);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429020);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429030);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429040);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429050);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429068);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429078);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429088);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429098);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004290A8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004290C0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004290D0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004290E0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004290F0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429100);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429118);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429130);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429140);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429150);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429160);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429170);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429180);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004291A0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004291B0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004291C8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004291D8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004291E8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004291F8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429208);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429218);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429228);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429238);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429250);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429260);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429278);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429288);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004292A0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004292B8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004292C8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004292E8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004292F8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429318);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429330);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429350);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429368);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429390);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004293A0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004293B0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004293C0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004293E0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429408);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429420);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429430);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429448);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429458);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429468);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429480);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429498);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004294A8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004294C0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004294D0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004294E8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004294F8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429508);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429518);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429530);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429540);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429560);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429570);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429588);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004295A0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004295B0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004295C8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004295E0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004295F0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429600);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429620);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429638);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429650);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429660);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429678);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429688);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004296A0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004296B0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004296C8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004296D8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004296F0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429708);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429720);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429730);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429740);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429750);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429760);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429778);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429788);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429798);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004297A8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004297C0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004297D8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004297E8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004297F8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429808);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429830);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429840);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429858);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429868);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429878);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429888);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429898);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004298B0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004298C0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004298D8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004298E8);

void mnuLoadMovieRollSprite(void) {
    D_00437AB0[1] = effLoadIndexedResource(D_00437AC0, "staff_01.spr", 0);
}

void func_002A6000(void) {
    mnuLoadStaffFonts();
}

void func_002A6018(void) {
    mnuUnloadStaffFonts();
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A6030);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A6180);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A6480);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A6580);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429938);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A6858);

void mnuFadeSetState(u32 *state, u32 mode) {
    switch (mode) {
    case 2: state[1] = 0; mode = 0; break;
    case 3: mode = 1; state[1] = 0x200; break;
    }
    state[0] = mode;
}

extern void func_00306CD0(s32, s32, s32, s32, s32, s32, s32, s32);

void mnuAdvanceSpriteSlideBar(SlideBar *bar) {
    u32 sprite = D_00437AB0[1];

    if (bar->active == 0 && bar->pos == 0) {
        return;
    }
    func_00306CD0(0, 0, 0, bar->pos / 2, 0, sprite, 9, 0x53);
    if (bar->active == 0) {
        bar->pos -= 8;
    } else {
        bar->pos += 8;
    }
    if (bar->pos < 0) {
        bar->pos = 0;
    }
    if (bar->pos > 0x200) {
        bar->pos = 0x200;
    }
}

void mnuFadeSetStateOff(u32 *state, u32 mode) {
    switch (mode) {
    case 2: state[1] = 0; mode = 0; break;
    case 3: state[1] = 0; mode = 1; break;
    }
    state[0] = mode;
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A6D68);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A6F88);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A7260);

void mnuTitleSetPaletteTransition(u32 *state, s32 mode) {
    switch (mode) {
    case 2:
        state[1] = 0;
        mode = 0;
        state[2] = 0;
        func_002A7260(state, 0);
        break;
    case 3:
        state[1] = 0x200;
        mode = 1;
        break;
    }
    state[0] = mode;
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A73C0);

void mnuFadeSetStateB(u32 *state, u32 mode) {
    switch (mode) {
    case 2: state[1] = 0; mode = 0; break;
    case 3: mode = 1; state[1] = 0x200; break;
    }
    state[0] = mode;
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A75A8);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A7730);

void func_002A78B0(void) {
    s64 temp_v0;

    D_00435BAC = 2;
    func_002A2408();
    func_002A2550();
    func_002A6018();
    do {
        temp_v0 = sdfCheckPendingWorkWithInterrupts();
    } while (temp_v0 != 0);
    func_003298C0(*D_00437AB0);
    D_00437AB0 = (u32 *)0x0;
}

void func_002A7900(void) {
    func_003054E8(D_00437AB0[1]);
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    func_003458E8(0);
}

extern u32 D_00437AB4;

extern u32 D_00437AB8;

void func_002A7938(void) {
    D_00437AB8 = 4;
    D_00437AB4 = 0;
    mnuLoadMovieRollSprite();
    func_003458E8(1);
    func_003458F0(0x80, 0x60, 0x180, 0x100, 0x80808080);
}

extern u32 D_00435CBC;

extern void func_002A6580();

typedef struct {
    u32 handle;
    u32 unk4;
    u32 unk8;
    u32 unkC;
} StaffTaskState;

void mnuCreateStaffTask(void) {
    u32 handle;

    D_00435CBC = 0x80000000;
    handle = func_003292A8(0xD8);
    D_00437AB0 = sdfResourceRetainAddress(handle);
    memset(D_00437AB0, 0, 0xD8);
    ((StaffTaskState *)D_00437AB0)->handle = handle;
    ((StaffTaskState *)D_00437AB0)->unk8 = 0;
    ((StaffTaskState *)D_00437AB0)->unkC = 0;
    D_00435BAC = 1;
    func_0019BD48();
    kwlnTaskCreate(D_00429968, 0x408, 0, 0, func_002A6580, func_002A78B0, 0);
}

u32 mnuStartStaffMovieRequest(void) {
    mnuCreateStaffTask();
    return 0xffffffff;
}

s32 mnuStopStaffTasks(void) {
    kwlnTaskDestroyWithHierarchyByName(D_00429938, 0);
    kwlnTaskDestroyWithHierarchyByName(D_00429968, 1);
    return 0;
}

s32 mnuMovieDraw(void) {
    func_00345BA0(D_003E5608, D_003803C8);
    return 0;
}

extern char D_0042A338[]; /* "mnuMovieDraw" */

void func_002A7A98(u32 resource, void *data) {
    if (D_00437ACC == 0) {
        func_00346778(D_003E5608, data, resource);
        D_00437ACC = kwlnTaskCreate(D_0042A338, 0x2afb, 1, 1, mnuMovieDraw, 0, 0);
    }
}

extern struct {
    u32 handle;
    u8 data[20];
} D_003E4C48[];

void func_002A7AF0(index)
s32 index;
{
    u32 *entry = (u32 *)&D_003E4C48[index];
    func_002A7A98(*entry, entry + 1);
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A7B28);

extern char D_0042A348[];

extern char D_0042A380[];

extern u32 D_00437AD0;

s32 mnuMovieDrawNextProc(s32 procedure) {
    if (D_00437ACC == 0) {
        func_0035B6E0(D_0042A348);
        return -1;
    }
    func_002A7B28(D_003E5608, D_003803C8);
    D_00437AD0++;
    if (D_00437AD0 == 0x1E) {
        func_0035B6E0(D_0042A380, procedure);
        return -1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A7DB0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429968);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429978);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429998);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004299B8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004299D8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_004299F8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429A18);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429A38);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429A58);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429A78);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429A98);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429AB8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429AD8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429AF8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429B18);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429B38);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429B58);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429B78);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429B98);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429BB8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429BD8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429BF8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429C18);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429C38);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429C58);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429C78);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429C98);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429CB8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429CD8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429CF8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429D18);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429D38);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429D58);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429D78);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429D98);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429DB8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429DD8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429DF8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429E18);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429E38);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429E58);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429E78);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429E98);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429EB8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429ED8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429EF8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429F18);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429F38);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429F58);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429F78);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429F98);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429FB8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429FD8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_00429FF8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A018);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A038);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A058);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A078);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A098);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A0B8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A0D8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A0F8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A118);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A138);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A158);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A178);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A198);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A1B8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A1D8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A1F8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A218);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A238);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A258);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A278);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A298);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A2B8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A2D8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A2F8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A318);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A338);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A348);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A380);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A7E60);

void func_002A7F98(s32 index) {
    u32 *entry = (u32 *)&D_003E4C48[index];
    func_002A7E60(*entry, entry + 1);
}

void func_002A7FD0(void) {
    if (D_00437ACC == 0) {
        return;
    }
    func_00346988(D_003E5608);
    kwlnTaskDestroyWithHierarchy(D_00437ACC, 0);
    D_00437ACC = 0;
}

s32 func_002A8008(void) {
    return func_00346A60(D_003E5608);
}

extern u8 D_003E563C[];

u8 func_002A8028(void) {
    return D_003E563C[0];
}

u8 func_002A8038(void) {
    return D_003E5608[0];
}

s32 mnuSetFrameDivisor(void) {
    func_00345488(0x3c / D_00435BAC);
    return 0;
}

void mnuCreateMovieManagerTask(void) {
    sdfSoundInitIpuStream();
    kwlnTaskCreate("movieMan", 0x385, 1, 0, mnuSetFrameDivisor, 0, 0);
}

u32 func_002A80C0(void) {
    u64 temp_v0;

    temp_v0 = scrReadIntParameter(0);
    func_002A7AF0(temp_v0);
    D_00437AE8 = 0;
    return 1;
}

u32 func_002A80F0(void) {
    func_002A7FD0();
    D_00437AE8 = 0;
    kwlnDrawEnableDc8(0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A8120);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A81C8);

typedef struct MovieList {
    u32 task;
    u32 *head;
    s16 count;
    s16 offset;
    s16 unkC;
} MovieList;

void mnuClearMovieList(void) {
    u32 *node = ((MovieList *)D_00457E48)->head;
    if (node != NULL) {
        do {
            u32 *next = (u32 *)*node;
            func_00328E48(node);
            node = next;
        } while (node != NULL);
        ((MovieList *)D_00457E48)->head = NULL;
        ((MovieList *)D_00457E48)->count = 0;
        ((MovieList *)D_00457E48)->offset = 0;
        ((MovieList *)D_00457E48)->unkC = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A8268);

u32 mnuGetMovieListNodeAtOffset(void) {
    u8 *state = (u8 *)D_00457E48;
    u32 entry = D_00457E48[1];
    s32 remaining = *(s16 *)(state + 0xA);
    if (entry != 0 && remaining > 0) {
        do {
            entry = *(u32 *)entry;
            remaining--;
        } while (entry != 0 && remaining > 0);
    }
    return entry;
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A8610);

typedef struct MovieStatus {
    u8 pad00[0x64];
    s32 total;
    s32 pad68;
    s32 current;
} MovieStatus;

extern s32 D_00457E58[];

extern s32 func_0011F250(s32, s32, s32, s32, s32, s32, s32);

extern s32 func_0033D810(s32, s32, s32, s32, char *, s32, s32);

extern void sdfAppendPacket(s32, s32);

void func_002A87F0(void) {
    s32 list;
    if (func_002A8008() == 0) {
        list = D_00457E58[0];
        sdfAppendPacket(list, func_0011F250(0x8810, 0x85E8, 0xFF0080, 0x720, 0x90, 0x30000000, 0x60404040));
        sdfAppendPacket(list, func_0033D810(0x8840, 0x8600, 0xFF0080, 0, "%04d/%04d", ((MovieStatus *)D_003E5608)->current, ((MovieStatus *)D_003E5608)->total));
    }
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A88A0);

void mnuCreateMovieViewerTask(void) {
    func_002A8268();
    D_00457E48[0] = kwlnTaskCreate(D_0042A418, 0x2b02, 1, 0, func_002A88A0, 0, 0);
}

void mnuDestroyMovieViewerTask(void) {
    s32 movieTask = func_00101740(D_0042A418);
    if (movieTask != 0) {
        kwlnTaskDestroyWithHierarchy(movieTask, 0);
        D_00457E48[0] = 0;
        func_002A7FD0();
    }
    mnuClearMovieList();
}

void func_002A8B78(void) {
    D_00457E60[2] = 1;
    D_00457E60[3] = 1;
}

void func_002A8B90(void) {
    u32 *task = (u32 *)D_00457E60;
    task[1] = 0x10002010;
    task[2] = (u32)D_003E5608;
    func_002A8B78();
}

/* Copy a source word and a 0x40-byte block when their pending flags are set. */
typedef struct MnuMovieTransfer {
    u8 pad00[2];
    u8 wordPending;      /* 0x02 */
    u8 blockPending;     /* 0x03 */
    s32 *wordSource;     /* 0x04 */
    void *blockSource;   /* 0x08 */
    s32 word;            /* 0x0C */
    u8 block[0x40];      /* 0x10 */
} MnuMovieTransfer;

void func_002A8BC8(void) {
    MnuMovieTransfer *state = (MnuMovieTransfer *)D_00457E60;

    if (state->wordPending != 0) {
        state->word = *state->wordSource;
    }
    if (state->blockPending != 0) {
        memcpy(state->block, state->blockSource, 0x40);
    }
}

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A418);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A8C80);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A9068);

extern u32 D_00438FF8[2];

extern u32 D_003E6848[];

extern char D_0042A950[];

extern u32 effLoadIndexedResource(char *, u32, u32);

void mnuLoadCampResources(void) {
    s32 i;
    for (i = 0; i < 2; i++) {
        D_00438FF8[i] = effLoadIndexedResource(D_0042A950, D_003E6848[i * 2], 1);
    }
}

extern void func_00305068(u32);

extern void effResolveAndReleaseResource(u32);

void func_002A91A0(u32 *destination) {
    s32 i;
    for (i = 0; i < 2; i++) {
        effResolveAndReleaseResource(D_00438FF8[i]);
        destination[i] = D_00438FF8[i];
    }
}

void func_002A9200(u32 *destination) {
    s32 remaining = 1;
    u32 offset = 0;
    do {
        func_00305068(*(u32 *)((u8 *)D_00438FF8 + offset));
        *(u32 *)((u8 *)destination + offset) = 0;
        offset += 4;
    } while (--remaining >= 0);
}

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A440);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A450);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A460);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A470);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A480);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A490);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A4A8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A4C0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A4D8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A4F0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A500);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A518);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A530);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A540);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A558);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A570);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A588);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A598);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A5B0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A5C8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A5E0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A5F8);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A608);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A620);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A638);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A650);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A668);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A680);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A690);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A6B0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A6C0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A6D0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A6E0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A6F0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A700);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A710);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A720);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A730);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A740);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A750);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A760);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A770);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A780);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A790);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A7A0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A7B0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A7D0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A7E0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A7F0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A800);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A810);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A820);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A830);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A840);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A850);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A870);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A888);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A8A0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A8B0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A8C0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A8D0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A8E0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A8F0);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A900);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A910);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A920);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A930);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A940);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042A950);

u8 *mnuGetStaffCategoryEntries(s32 kind, s32 *count, u8 *work) {
    switch (kind) {
    case 1:
        *count = 1;
        return work + 0xC8;
    case 2:
        *count = 1;
        return work + 0xC4;
    case 3:
        *count = 2;
        return work + 0x68;
    case 4:
        *count = 9;
        return work + 0xCC;
    case 5:
        *count = 1;
        return work + 0xF0;
    default:
        *count = 0;
        return 0;
    }
}

void func_002A92D8(s32 list, s32 count, u8 *work) {
    s32 i;

    effResolveAndReleaseResource(*(u32 *)list);
    for (i = 0; i < 5; i++) {
        u8 *slot = D_00435DD0 + 0xA60 + i * 0x1C4;

        if ((*(u16 *)slot & 1) != 0) {
            s32 index = *(u16 *)(slot + 4) + D_00437B73;

            effResolveAndReleaseResource(*(u32 *)(list + index * 4 - 4));
        }
    }
}

void movReleaseCategoryModels(s32 kind, u8 *work) {
    s32 count;
    s32 *entries = (s32 *)mnuGetStaffCategoryEntries(kind, &count, work);
    if (kind != 4) {
        s32 i;
        for (i = 0; i < count; i++) {
            effResolveAndReleaseResource(entries[i]);
        }
    } else {
        func_002A92D8(entries, count, work);
    }
}

void func_002A93F8(s32 kind, u8 *work) {
    s32 count;
    s32 i = 0;
    u8 *buffer = mnuGetStaffCategoryEntries(kind, &count, work);

    if (count > 0) {
        u32 *handles = (u32 *)buffer;
        do {
            func_00305068(*handles++);
        } while (++i < count);
    }
}

void func_002A9460(s32 kind, u8 *work) {
    s32 old = *(s32 *)(work + 0xAA4C);
    if (kind == old) {
        return;
    }
    if (old != 0) {
        func_002A93F8(old, work);
    }
    if (kind != 0) {
        movReleaseCategoryModels(kind, work);
    }
    *(s32 *)(work + 0xAA4C) = kind;
}

extern u32 D_003E6970[];

typedef struct { u8 pad0[0x20]; s32 *data; } MotSub;
typedef struct { u8 pad0[8]; MotSub *sub; } MotRes;

void movLoadTitleEffects(u8 *work) {
    s32 *data;

    *(u32 *)(work + 0x100) = effLoadMappedResource("/camp/mot/", D_003E6970[0]);
    *(MotRes **)(work + 0x110) = func_00304998(6);
    data = (*(MotRes **)(work + 0x110))->sub->data;
    data[0] = 0xF;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;
    data[4] = 0;
    *(MotRes **)(work + 0x114) = func_00304998(1);
    data = (*(MotRes **)(work + 0x114))->sub->data;
    data[0] = 0xF;
    data[1] = 0;
}

void movReleaseTitleEffects(u32 *state) {
    u32 *handles = state + 0x110 / 4;
    u32 i;
    effDestroyPackedBatch(state[0x100 / 4]);
    for (i = 0; i < 2; i++) {
        effDestroyPackedBatch(*handles++);
    }
}

void func_002A95B0(u32 arg0, u32 *arg1, u32 arg2, u32 arg3) {
    func_002BCD90(arg0, arg3, *arg1, 1, arg1[1], 0x2d, arg1[1], 0x1d);
    func_002BC498(arg0, arg1[1]);
    func_002BC5D0(arg0, arg1 + 4);
    func_002BC600(arg0, arg1 + 0xc);
    mnuRegisterResourceHandles(arg0, arg1 + 0x14);
    func_002BCA98(arg0);
    func_002BE6E8(arg0, arg1[1]);
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A9640);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A9788);

s32 movAreTitleEffectsReady(s32 mode, u32 *state) {
    u32 *entry;
    u32 *tail;
    s32 i;
    func_00303E88(mode);
    i = 0;
    entry = state;
    for (; i < 2; i++) {
        if (*entry++ == 0) {
            return 0;
        }
    }
    i = 0;
    entry = state + 4;
    for (; i < 16; i++) {
        if (*entry++ == 0) {
            return 0;
        }
    }
    i = 0;
    entry = state + 0x50 / 4;
    for (; i < 5; i++) {
        if (*entry++ == 0) {
            return 0;
        }
    }
    tail = state + 2;
    i = 0;
    for (; i < 2; i++) {
        if (*tail++ == 0) {
            return 0;
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A9908);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A9A40);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A9AB8);

typedef struct StaffResourceHeader {
    u8 pad00[0x64];
    s32 resourceSource;         /* 0x064 */
    u8 pad68[0x8C];
    u32 baseHandles[3];        /* 0x0F4 */
    s32 resourceOptions;        /* 0x100 */
    u32 resourceLists[3];      /* 0x104 */
} StaffResourceHeader;

void func_002A9BC8(s32 arg0, u32 arg1, u32 arg2, s32 arg3, u32 arg4,
                                    u32 arg5) {
    func_00306F80(arg0 + 0x60, arg1, arg2, 1, *(u32 *)(*(s32 *)(arg3 + 0x30) + 100), 10,
                                arg5);
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A9BF8);

extern s32 func_002B9FF8(s32, s32, s32);

extern s32 func_002A9BF8(void *, s32, s32, s32, u8 *, void *);

extern void mnuSetWindowContainerState(s32, s32);

extern void func_002BAF10(u8 *);

extern void func_002BAF50(s32, u8 *);

extern u8 D_003E56D0[], D_003E56F0[], D_003E5708[], D_003E6978[], D_003E6998[];

void mnuStaffInitResourceLists(u8 *work) {
    u8 *ctx = work + 0xB10C;
    s32 list;

    ((StaffResourceHeader *)work)->baseHandles[0] = func_002B9FF8(0, ((StaffResourceHeader *)work)->resourceSource, ((StaffResourceHeader *)work)->resourceOptions);
    ((StaffResourceHeader *)work)->baseHandles[1] = func_002B9FF8(1, ((StaffResourceHeader *)work)->resourceSource, ((StaffResourceHeader *)work)->resourceOptions);
    ((StaffResourceHeader *)work)->baseHandles[2] = func_002B9FF8(3, ((StaffResourceHeader *)work)->resourceSource, ((StaffResourceHeader *)work)->resourceOptions);
    ((StaffResourceHeader *)work)->resourceLists[0] = func_002A9BF8(D_003E56D0, 8, 0x1C0, 0x10, work, D_003E6978);
    list = func_002A9BF8(D_003E56F0, 5, 0x1C0, 0x10, work, D_003E6998);
    ((StaffResourceHeader *)work)->resourceLists[1] = list;
    mnuSetWindowContainerState(list, 0x100);
    list = func_002A9BF8(D_003E5708, 2, 0x1C0, 0x10, work, 0);
    ((StaffResourceHeader *)work)->resourceLists[2] = list;
    mnuSetWindowContainerState(list, 0x100);
    func_002BAF10(ctx);
    func_002BAF50(((StaffResourceHeader *)work)->resourceLists[0], ctx);
}

extern void mnuDestroyWindowContainer(u32);

extern void mnuReleaseResourceList(u32);

void func_002A9F08(u8 *work) {
    u32 *handles = ((StaffResourceHeader *)work)->resourceLists;
    u32 i;

    for (i = 0; i < 3; i++) {
        mnuDestroyWindowContainer(*handles++);
    }
    mnuReleaseResourceList(((StaffResourceHeader *)work)->baseHandles[0]);
    mnuReleaseResourceList(((StaffResourceHeader *)work)->baseHandles[1]);
    mnuReleaseResourceList(((StaffResourceHeader *)work)->baseHandles[2]);
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A9F78);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002A9FF0);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002AA068);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002AA0D8);

void mnuDestroyStaffMenuTask(u32 task) {
    u8 *work = (u8 *)func_00101958(task);
    if (work == NULL) {
        return;
    }
    func_002C3FC8(work + 8, task);
    func_002A9F08(work);
    func_002BB418(*(u32 *)(work + 0x118));
    mnuShutdownContext(work + 0x284);
    func_0026C728();
    mnuDestroyEffectResources(work + 0x11c);
    func_002A9A40(work);
    movReleaseTitleEffects(work);
    func_00303D58(*(u32 *)(work + 0x5c));
    func_003297C8(*(u32 *)work);
    D_00437B72 = 2;
    func_003425D8();
}

u32 func_002AA278(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    func_002C1B70(temp_v0 + 0xaa50, 0x53);
    return 0;
}

extern u32 func_002C44E8(s32);

extern s32 func_002A9AB8(s32);

extern s32 func_0026C768(void);

extern s32 func_002C6CE8(void);

extern s32 fileConsumeConfigTaskReady(void);

extern void mnuDestroyCampTasks(void);

extern void mnuPlayInputSound();

/* On button 8, exit the camp only when both nested guards permit it;
 * otherwise play the alternate sound without destroying its tasks. */
s32 mnuStaffCampCancelCheck(s32 menu) {
    u32 buttons = func_002C44E8(8);
    s32 result;

    if (func_002A9AB8(menu) == 0) {
        return 0;
    }
    result = 0;
    if (func_0026C768() == 0) {
        if (buttons & 8) {
            if (func_002C6CE8() != 1) {
                if (fileConsumeConfigTaskReady() == 0) {
                    mnuDestroyCampTasks();
                    mnuPlayInputSound(0, 2, 0);
                    return -1;
                }
            }
            mnuPlayInputSound(0, 0x8000, 0);
        }
    }
    return result;
}

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042AA08);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042AA18);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002AA360);

void mnuDestroyCampTasks(void) {
    kwlnTaskDestroyWithHierarchyByName(D_00437B78, 0);
    kwlnTaskDestroyWithHierarchyByName(D_0042AA08, 0);
    kwlnTaskDestroyWithHierarchyByName(D_0042AA18, 0);
}

s32 mnuAcknowledgeCampState(void) {
    s8 state = D_00437B72;
    if (state == 1) {
        return 1;
    }
    if (state < 2) {
        return 0;
    }
    if (state == 2) {
        D_00437B72 = 0;
    }
    return 0;
}

u8 mnuIsFadeIdle(void) {
    s64 temp_v0;

    temp_v0 = kwlnFadeIsActive();
    return temp_v0 == 0;
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002AA530);

extern u32 D_003E5710[];

void mnuCreateStaffImageSprite(s32 index) {
    u32 *object = (u32 *)func_0019F460(0x340, 0x148, 0, 0xa09dc35a,
                                      D_003E5710[index], 0);
    func_0019D550(object, 1, 0x54);
    func_0019C5B0(object);
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002AA7A0);

INCLUDE_ASM(const s32, "game/code_002A5260", func_002AA9D8);

extern void func_002AA9D8(u32, u32, u32, u32, u32, u32, u32, u32, u32);

void func_002AAC70(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g) {
    func_002AA9D8(a, b, c, d, e, f, 0, 0, g);
}

void func_002AAC98(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f) {
    func_002AAC70(a, b, c, d, e, 0, f);
}

INCLUDE_ASM(const s32, "game/code_002A5260", func_002AACB8);

void func_002AAE80(u32 arg0) {
    func_002AACB8(0, arg0);
}

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042AA48);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042AC40);

INCLUDE_RODATA(const s32, "game/code_002A5260", D_0042AC70);

