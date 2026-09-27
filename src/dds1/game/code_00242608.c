#include "common.h"

extern s32 D_003BC7B4;

extern s32 D_003BAA4C;

extern s32 D_003BAA50;

extern u32 func_002BD258(u32);

extern s32 func_002860B8(u16);

extern u32 func_0027D4A0(u32);

extern u32 func_0027F730(u32);

extern s32 func_002CFEB8(u32);

extern u32 func_0027D148(u32, u32, u32, u32);

extern s32 func_0027B888(u32);

extern s32 func_002877A8(void);

extern s64 func_00273750(void);

extern s32 func_00105C48(void);

extern s32 func_00270088(void);

extern u32 D_003BC630;

extern u16 D_003BA72C;
extern u32 *D_003BC610;
extern s32 func_002D3EE8(void);

extern s32 D_003BC5D0;

extern u32 D_003BD8D0;

extern u32 D_003BC5BC;

extern s32 func_0026A720(void);

extern u64 func_002E97E0(void);

extern u64 func_0010D428(u64);

extern u32 func_00265E68(u32, s32);

extern u8 D_003BC52A;
extern u8 D_003BC52B;

extern u32 D_003BAA9C;
extern u64 func_00197C40(u64, u64, u64, u16, u32, u64);

extern s32 func_002CB3B8(u32, u32);

extern u32 D_003BC4CC;

extern s32 D_003BC408;
extern u8 D_003BC40C;

extern u32 func_002EB028(u32, u32 *, u32);

extern s32 func_0024A6C0(void);

extern u8 D_003BC3E1;
extern s32 func_0010FD80(void);

extern s32 func_0021F600(u32);

extern s64 func_0024DC08(void);
extern s64 func_00285670(s32, s32 *, u64, u64);

extern s32 func_00101A70(void);

extern s32 D_003BAA00;

extern s32 func_00101938(u32);

extern s64 func_00241B58(void);

INCLUDE_ASM(const s32, "game/code_00242608", func_00242608);

void func_00242698(void) {
    s64 temp_v0;

    temp_v0 = func_00241B58();
    if (temp_v0 != 0) {
        kwlnTaskDestroyWithHierarchy(temp_v0, 0);
        return;
    }
}

void func_002426D0(void) {
    s64 temp_v0;

    while (temp_v0 = func_00101938(0x3ec), temp_v0 != 0) {
        kwlnTaskDestroyWithHierarchy(temp_v0, 0);
    }
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00242708);

INCLUDE_ASM(const s32, "game/code_00242608", func_00242780);

INCLUDE_ASM(const s32, "game/code_00242608", func_00242950);

INCLUDE_ASM(const s32, "game/code_00242608", func_002429F0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00242BD0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00242C30);

INCLUDE_ASM(const s32, "game/code_00242608", func_00242E28);

INCLUDE_ASM(const s32, "game/code_00242608", func_00242E70);

INCLUDE_ASM(const s32, "game/code_00242608", func_00242F20);

INCLUDE_ASM(const s32, "game/code_00242608", func_00242F78);

INCLUDE_ASM(const s32, "game/code_00242608", func_00243048);

INCLUDE_ASM(const s32, "game/code_00242608", func_002432D0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00243390);

INCLUDE_ASM(const s32, "game/code_00242608", func_00243440);

INCLUDE_ASM(const s32, "game/code_00242608", func_00243460);

INCLUDE_ASM(const s32, "game/code_00242608", func_002434E0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00243558);

INCLUDE_ASM(const s32, "game/code_00242608", func_00243608);

void func_002437E0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x2034);
    if (temp_v0 != 0) {
        *(u32 *)(temp_v0 + 0x28) = 0;
        while (temp_v0 = *(s32 *)(temp_v0 + 0x7c), temp_v0 != 0) {
            *(u32 *)(temp_v0 + 0x28) = 0;
        }
    }
    *(u32 *)(arg0 + 0x240c) = 0;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00243818);

INCLUDE_ASM(const s32, "game/code_00242608", func_00243928);

void func_00243A18(s32 arg0) {
    func_00243818();
    if (*(s32 *)(arg0 + 0x23cc) == 1) {
        func_00134CD8();
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00243A58);

void func_00243AA8(s32 arg0) {
    func_00194920(*(u32 *)(arg0 + 0x2410));
    *(u32 *)(arg0 + 0x2410) = 0;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00243AD8);

void func_00243B00(s32 arg0) {
    if ((*(s32 *)(arg0 + 0x2430) == 0) || (*(s32 *)(arg0 + 0x2430) == 5)) {
        *(u32 *)(arg0 + 0x2430) = 1;
    }
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00243B28);

INCLUDE_ASM(const s32, "game/code_00242608", func_00243BF0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00243CC8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00243D48);

void func_00243EC8(s32 arg0) {
    func_00243D48(*(u32 *)(arg0 + 0x2438));
}

void func_00243EE0(void) {
}

void func_00243EE8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x243c) = (*(u32 *)(arg0 + 0x243c) & 0xfffffffc) | (arg1 & 3);
}

u32 func_00243F08(s32 arg0) {
    return *(u32 *)(arg0 + 0x243c) & 3;
}

void func_00243F18(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x243c) = (*(u32 *)(arg0 + 0x243c) & 0xfffffff3) | ((arg1 & 3) << 2);
}

u32 func_00243F38(s32 arg0) {
    return (*(u32 *)(arg0 + 0x243c) & 0xc) >> 2;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00243F48);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244058);

INCLUDE_ASM(const s32, "game/code_00242608", func_002440A8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244110);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244180);

INCLUDE_ASM(const s32, "game/code_00242608", func_002441E8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244258);

INCLUDE_ASM(const s32, "game/code_00242608", func_002442D0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF3D0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244320);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244360);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244380);

INCLUDE_ASM(const s32, "game/code_00242608", func_002443F8);

INCLUDE_ASM(const s32, "game/code_00242608", func_002444D0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244508);

INCLUDE_ASM(const s32, "game/code_00242608", func_002445E0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244658);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244740);

INCLUDE_ASM(const s32, "game/code_00242608", func_002447D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244848);

s32 func_00244898(void) {
    u16 temp_v0;
    u16 *puVar2;
    s32 temp_v1;
    s32 temp_v2;

    temp_v2 = 0;
    temp_v1 = 4;
    puVar2 = (u16 *)(D_003BAA00 + 0xa60);
    do {
        temp_v0 = *puVar2;
        puVar2 = puVar2 + 0xd2;
        temp_v1 = temp_v1 - 1;
        temp_v2 = temp_v2 + (temp_v0 & 1);
    } while (-1 < temp_v1);
    return temp_v2;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002448D0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244970);

INCLUDE_ASM(const s32, "game/code_00242608", func_002449F0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244AB8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244AF8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244B30);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244B90);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244BC8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244C00);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244D10);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244E08);

INCLUDE_ASM(const s32, "game/code_00242608", func_00244FA0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00245068);

INCLUDE_ASM(const s32, "game/code_00242608", func_00245190);

INCLUDE_ASM(const s32, "game/code_00242608", func_00245208);

INCLUDE_ASM(const s32, "game/code_00242608", func_002453C8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00245628);

INCLUDE_ASM(const s32, "game/code_00242608", func_002457E8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00245A40);

INCLUDE_ASM(const s32, "game/code_00242608", func_00245C00);

INCLUDE_ASM(const s32, "game/code_00242608", func_00245C98);

INCLUDE_ASM(const s32, "game/code_00242608", func_00245CC8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00245D08);

INCLUDE_ASM(const s32, "game/code_00242608", func_00245DA0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF418);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF428);

INCLUDE_ASM(const s32, "game/code_00242608", func_00245DE0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00246088);

INCLUDE_ASM(const s32, "game/code_00242608", func_002460D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00246120);

INCLUDE_ASM(const s32, "game/code_00242608", func_00246160);

INCLUDE_ASM(const s32, "game/code_00242608", func_00246198);

INCLUDE_ASM(const s32, "game/code_00242608", func_00246220);

INCLUDE_ASM(const s32, "game/code_00242608", func_00246460);

INCLUDE_ASM(const s32, "game/code_00242608", func_002464B0);

INCLUDE_ASM(const s32, "game/code_00242608", func_002464F8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00246538);

INCLUDE_ASM(const s32, "game/code_00242608", func_00246570);

INCLUDE_ASM(const s32, "game/code_00242608", func_002465F8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00246838);

INCLUDE_ASM(const s32, "game/code_00242608", func_00246888);

INCLUDE_ASM(const s32, "game/code_00242608", func_002468D0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00246910);

INCLUDE_ASM(const s32, "game/code_00242608", func_00246968);

INCLUDE_ASM(const s32, "game/code_00242608", func_002469F0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00246BE8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00246C38);

u32 func_00246C80(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    *(u32 *)(temp_v0 + 0xa4) = 0;
    func_0027BB28(*(u32 *)(*(s32 *)(temp_v0 + 0x6c) + 0x14));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00246CB0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00246D68);

INCLUDE_ASM(const s32, "game/code_00242608", func_00246DB8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00246E00);

INCLUDE_ASM(const s32, "game/code_00242608", func_00246E38);

INCLUDE_ASM(const s32, "game/code_00242608", func_00246EA0);

INCLUDE_ASM(const s32, "game/code_00242608", func_002470F8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00247148);

INCLUDE_ASM(const s32, "game/code_00242608", func_00247190);

INCLUDE_ASM(const s32, "game/code_00242608", func_002472D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_002473D0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00247420);

INCLUDE_ASM(const s32, "game/code_00242608", func_00247588);

INCLUDE_ASM(const s32, "game/code_00242608", func_00247728);

INCLUDE_ASM(const s32, "game/code_00242608", func_002478B0);

INCLUDE_ASM(const s32, "game/code_00242608", func_002478F8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00247970);

s64 func_002479F0(u64 arg0) {
    s32 temp_v0;
    s64 temp_v1;
    s32 *piVar3;

    temp_v0 = func_00101A70();
    piVar3 = (s32 *)(temp_v0 + 0x54);
    temp_v1 = func_00285670(temp_v0 + 8, piVar3, 0, arg0);
    if (temp_v1 == 0) {
        if ((*piVar3 == 0) && (temp_v1 = func_0024DC08(), temp_v1 == 0)) {
            func_002858F8(piVar3, *(u32 *)(temp_v0 + 0x58));
        }
        temp_v1 = 0;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00247A78);

INCLUDE_ASM(const s32, "game/code_00242608", func_00247CB0);

u32 func_00247CF8(void) {
    func_00220110(0x323);
    func_00105AE0(0, 0, 0, 0xf);
    func_0024DED8(0);
    func_0024DEF8(0, 0);
    func_0024DEF8(1, 1);
    return 1;
}

u32 func_00247D50(void) {
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF528);

INCLUDE_ASM(const s32, "game/code_00242608", func_00247D58);

INCLUDE_ASM(const s32, "game/code_00242608", func_00248088);

INCLUDE_ASM(const s32, "game/code_00242608", func_002480E8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00248130);

u32 func_00248170(void) {
    func_00105AE0(0, 0, 0, 0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002481A0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00248240);

INCLUDE_ASM(const s32, "game/code_00242608", func_00248278);

u32 func_002482B0(void) {
    return 1;
}

u32 func_002482B8(void) {
    return 1;
}

u32 func_002482C0(void) {
    return 0;
}

u32 func_002482C8(void) {
    return 0;
}

u32 func_002482D0(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002482D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_002483C0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00248468);

INCLUDE_ASM(const s32, "game/code_00242608", func_00248508);

void func_00248580(s32 arg0) {
    func_002BD7A0(*(u32 *)(arg0 + 100));
    func_002BD7A0(*(u32 *)(arg0 + 0x68));
}

void func_002485B0(s32 arg0) {
    func_002BD870(*(u32 *)(arg0 + 100));
    func_002BD870(*(u32 *)(arg0 + 0x68));
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002485E0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00248658);

INCLUDE_ASM(const s32, "game/code_00242608", func_00248700);

INCLUDE_ASM(const s32, "game/code_00242608", func_00248750);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF570);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF580);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF590);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF5A8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00248810);

INCLUDE_ASM(const s32, "game/code_00242608", func_00248BA0);

void func_00248C38(s32 arg0) {
    if (arg0 != 0) {
        func_00284340();
        func_00284340((s32)arg0 + 0x54);
        func_002CFF98(arg0);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00248C80);

void func_00248CF8(s32 arg0) {
    s32 temp_v0;

    for (temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0x74) + 0x10); temp_v0 != 0; temp_v0 = *(s32 *)(temp_v0 + 0x58)
            ) {
        func_00248C38(*(u32 *)(temp_v0 + 0x70));
    }
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00248D40);

void func_00248E18(s32 arg0) {
    func_0027B368(*(u32 *)(arg0 + 0x74));
}

void func_00248E30(s32 arg0) {
    func_00248C38(*(u32 *)(*(s32 *)(*(s32 *)(arg0 + 0x74) + 0x1c) + 0x70));
    func_0027B888(*(u32 *)(arg0 + 0x74));
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00248E68);

INCLUDE_ASM(const s32, "game/code_00242608", func_00249010);

INCLUDE_ASM(const s32, "game/code_00242608", func_00249058);

u8 func_00249198(void) {
    s64 temp_v0;

    temp_v0 = func_0021F600(0x902);
    return temp_v0 == 0;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002491B8);

INCLUDE_ASM(const s32, "game/code_00242608", func_002492F8);

INCLUDE_ASM(const s32, "game/code_00242608", func_002493B0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00249420);

INCLUDE_ASM(const s32, "game/code_00242608", func_00249498);

INCLUDE_ASM(const s32, "game/code_00242608", func_002495F8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00249668);

INCLUDE_ASM(const s32, "game/code_00242608", func_002496D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_002496F0);

void func_00249770(u32 *arg0) {
    func_00280488(arg0 + 100);
    func_00276320(arg0 + 2);
    func_00271648(arg0 + 2);
    func_002BC618(arg0[1]);
    func_002D0918(*arg0);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002497C0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00249850);

void func_00249930(s32 arg0) {
    func_0027F6B8(arg0 + 400);
    func_0027FA20(arg0 + 400);
    func_00283038(*(u32 *)(arg0 + 0x820));
    func_00283820(*(u32 *)(arg0 + 0x824));
    func_00285160(*(u32 *)(arg0 + 0x828));
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00249980);

INCLUDE_ASM(const s32, "game/code_00242608", func_00249998);

INCLUDE_ASM(const s32, "game/code_00242608", func_00249A60);

void func_00249C08(s8 arg0) {
    s64 temp_v0;

    if (arg0 == '\x01') {
        temp_v0 = func_0010FD80();
        if (temp_v0 != 0) {
            func_00110860(temp_v0, 1);
        }
        D_003BC3E1 = 1;
    }
    else {
        temp_v0 = func_0010FD80();
        if (temp_v0 != 0) {
            func_00110860(temp_v0, 0);
        }
        D_003BC3E1 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00249C78);

INCLUDE_ASM(const s32, "game/code_00242608", func_00249D80);

INCLUDE_ASM(const s32, "game/code_00242608", func_00249DD0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF5E0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF620);

INCLUDE_ASM(const s32, "game/code_00242608", func_00249E20);

INCLUDE_ASM(const s32, "game/code_00242608", func_00249F08);

INCLUDE_ASM(const s32, "game/code_00242608", func_00249FA8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024A058);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024A0A0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024A0D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024A138);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024A170);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024A1A8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024A1D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024A2B8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024A2D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024A340);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024A478);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024A4A0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024A570);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024A610);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024A6C0);

u8 func_0024A6E8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0024A6C0();
    return *(u8 *)(temp_v0 * 0xa0 + *(s32 *)(*(s32 *)(arg0 + 100) + 0x18) + 0x14);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0024A728);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024A930);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024AB28);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024AB70);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024ACD8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024AE18);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024AF58);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024B090);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024B168);

void func_0024B2D0(u32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = *(u32 *)(arg1 + 0xc4);
    *(u32 *)(arg1 + 0xc4) = arg0;
    *(u32 *)(arg1 + 200) = temp_v0;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0024B2E0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024B358);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024B3A8);

u32 func_0024B470(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0024B478);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024B6C0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024B750);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024B798);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024B7E8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024B868);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024B958);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024B990);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024B9D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024BA78);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024BB00);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024BC18);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024BC88);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024BCD0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024BD48);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024BDB8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024BF48);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024C028);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024C0B0);

u32 func_0024C0F8(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    func_0027BB28(*(u32 *)(temp_v0 + 0x70));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0024C120);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024C1B8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024C298);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024C2E0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024C368);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024C3F8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024C578);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024C638);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024C670);

u32 func_0024C6F8(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0024C700);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024C7E8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024C858);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024C8A0);

s64 func_0024C918(u64 arg0) {
    s32 temp_v0;
    s64 temp_v1;
    s32 *piVar3;

    temp_v0 = func_00101A70();
    piVar3 = (s32 *)(temp_v0 + 0x54);
    temp_v1 = func_00285670(temp_v0 + 8, piVar3, 0, arg0);
    if (temp_v1 == 0) {
        if ((*piVar3 == 0) && (temp_v1 = func_0024DC08(), temp_v1 == 0)) {
            func_002858E8(piVar3, *(u32 *)(temp_v0 + 0x58));
        }
        temp_v1 = 0;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0024C9A0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024CA10);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024CA58);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024CB00);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024CB80);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024CCD8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024CD78);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024CDB0);

u32 func_0024CE20(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0024CE28);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024CF28);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024CF78);

void func_0024CFB0(s32 arg0) {
    func_0024DBC8();
    func_0024D9D8(*(u32 *)(arg0 + 0x60));
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0024CFD8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024D220);

u32 func_0024D260(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0024D268);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024D300);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024D350);

u32 func_0024D398(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    if (*(s32 *)(temp_v0 + 0xdc) == 0) {
        func_0024DED8();
        func_0024DEF8(0, 1);
        func_0024DEF8(1, 0);
    }
    else {
        func_00105B98(0, 0, 0, 0xf);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0024D400);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024D440);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024D500);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024D550);

u32 func_0024D588(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    *(u32 *)(temp_v0 + 0x90) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0024D5B0);

u32 func_0024D5F0(void) {
    return 0;
}

u32 func_0024D5F8(void) {
    return 0;
}

u32 func_0024D600(void) {
    return 0;
}

u32 func_0024D608(void) {
    return 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0024D610);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024D670);

void func_0024D6E0(void) {
    func_001068F0(0, 0, 0);
    func_00106D08(0);
    func_00106F40(0);
    func_0018F3B0();
    func_0018F438();
    func_0018F750();
    func_0018F4F0();
    func_0018F6E8();
}

void func_0024D738(void) {
    func_00226C38();
    func_0024D6E0();
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0024D758);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024D778);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024D7B8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024D828);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024D880);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024D8A8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024D8F8);

void func_0024D990(u32 arg0, u32 *arg1) {
    u32 temp_v0;

    temp_v0 = func_002EB028(arg0, arg1 + 1, 0);
    *arg1 = temp_v0;
}

void func_0024D9C0(u32 *arg0) {
    func_002D0918(*arg0);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0024D9D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024DA20);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024DA58);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024DAB8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024DAE8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024DB00);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024DB08);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024DB40);

u32 func_0024DB48(s32 arg0) {
    u32 temp_v0;

    temp_v0 = 0;
    if (-1 < D_003BC408) {
        func_0019AE58(D_003BC408, 0);
        if (arg0 != 0) {
            func_0019B4A0(D_003BC408);
        }
        func_0019BC98(D_003BC408, 0);
        func_0024DDC0(1);
        D_003BC40C = 0;
        temp_v0 = 1;
    }
    return temp_v0;
}

void func_0024DBB0(void) {
    func_0024DB48(1);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0024DBC8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024DC08);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024DC50);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024DC98);

void func_0024DD78(void) {
    func_0024DC98(1);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0024DD90);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024DDB8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024DDC0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024DE30);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024DE78);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024DED8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024DEE8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024DEF8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024DF20);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024DF48);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024DF78);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024DFC0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024E010);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024E100);

void func_0024E198(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    func_002C0DD8(arg0, arg1, 0, arg2, arg3, 0x30303040, 0x53);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0024E1C8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024E260);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024E310);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024E3C0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024E470);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024E5A0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024E728);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024E8D0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024EA50);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024EC08);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024EDC0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024EF68);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024F0D0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024F210);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024F338);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF658);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF668);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF678);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF688);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF6A0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF6B0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF6E0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF6F0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF700);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF710);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF720);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF730);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024F4F0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024F570);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024F5B0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024F608);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024F6F0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024F760);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024F7A8);

void func_0024F7D8(void) {
    func_002CB278(D_003BC4CC);
    D_003BC4CC = 0;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0024F800);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024F858);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024F8D8);

u32 func_0024FA18(void) {
    s32 temp_v0;

    temp_v0 = func_002CB3B8(D_003BC4CC, 0);
    return *(u32 *)(*(s32 *)(*(s32 *)(temp_v0 + 0xc) + 0x1c) + 0x70);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0024FA48);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024FA88);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024FAC8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024FB30);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF7A8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0024FBB8);

INCLUDE_ASM(const s32, "game/code_00242608", func_002501E0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00250758);

INCLUDE_ASM(const s32, "game/code_00242608", func_00250820);

INCLUDE_ASM(const s32, "game/code_00242608", func_002508D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00250978);

INCLUDE_ASM(const s32, "game/code_00242608", func_00250A48);

INCLUDE_ASM(const s32, "game/code_00242608", func_00250B60);

INCLUDE_ASM(const s32, "game/code_00242608", func_00250E88);

INCLUDE_ASM(const s32, "game/code_00242608", func_00250F60);

INCLUDE_ASM(const s32, "game/code_00242608", func_00251260);

INCLUDE_ASM(const s32, "game/code_00242608", func_002512F0);

INCLUDE_ASM(const s32, "game/code_00242608", func_002515C8);

INCLUDE_ASM(const s32, "game/code_00242608", func_002515F0);

INCLUDE_ASM(const s32, "game/code_00242608", func_002517C0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00251960);

INCLUDE_ASM(const s32, "game/code_00242608", func_002519E8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF810);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF830);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF840);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF850);

INCLUDE_ASM(const s32, "game/code_00242608", func_00251A38);

INCLUDE_ASM(const s32, "game/code_00242608", func_00251E38);

INCLUDE_ASM(const s32, "game/code_00242608", func_00252CE8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00252E38);

INCLUDE_ASM(const s32, "game/code_00242608", func_00252F88);

INCLUDE_ASM(const s32, "game/code_00242608", func_00253018);

INCLUDE_ASM(const s32, "game/code_00242608", func_002530D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00253208);

INCLUDE_ASM(const s32, "game/code_00242608", func_00253520);

INCLUDE_ASM(const s32, "game/code_00242608", func_00253558);

INCLUDE_ASM(const s32, "game/code_00242608", func_00253608);

INCLUDE_ASM(const s32, "game/code_00242608", func_00253640);

INCLUDE_ASM(const s32, "game/code_00242608", func_00253778);

INCLUDE_ASM(const s32, "game/code_00242608", func_00253830);

INCLUDE_ASM(const s32, "game/code_00242608", func_00253AD0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00253C78);

INCLUDE_ASM(const s32, "game/code_00242608", func_00253CC8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00253CF8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00253D40);

INCLUDE_ASM(const s32, "game/code_00242608", func_00253E58);

INCLUDE_ASM(const s32, "game/code_00242608", func_00254218);

INCLUDE_ASM(const s32, "game/code_00242608", func_00254288);

INCLUDE_ASM(const s32, "game/code_00242608", func_00254680);

INCLUDE_ASM(const s32, "game/code_00242608", func_002546D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00254758);

INCLUDE_ASM(const s32, "game/code_00242608", func_00254778);

INCLUDE_ASM(const s32, "game/code_00242608", func_00254810);

INCLUDE_ASM(const s32, "game/code_00242608", func_002549F0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00254B30);

INCLUDE_ASM(const s32, "game/code_00242608", func_00254C30);

INCLUDE_ASM(const s32, "game/code_00242608", func_00254C68);

INCLUDE_ASM(const s32, "game/code_00242608", func_00254E48);

INCLUDE_ASM(const s32, "game/code_00242608", func_00254EF0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00255010);

INCLUDE_ASM(const s32, "game/code_00242608", func_00255118);

INCLUDE_ASM(const s32, "game/code_00242608", func_002551B8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00255368);

INCLUDE_ASM(const s32, "game/code_00242608", func_00255508);

INCLUDE_ASM(const s32, "game/code_00242608", func_002555E8);

INCLUDE_ASM(const s32, "game/code_00242608", func_002556C8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00255798);

INCLUDE_ASM(const s32, "game/code_00242608", func_002557B8);

INCLUDE_ASM(const s32, "game/code_00242608", func_002557D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_002557F8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00255818);

INCLUDE_ASM(const s32, "game/code_00242608", func_00255838);

INCLUDE_ASM(const s32, "game/code_00242608", func_002559F8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00255A98);

INCLUDE_ASM(const s32, "game/code_00242608", func_00255B78);

INCLUDE_ASM(const s32, "game/code_00242608", func_00255D00);

INCLUDE_ASM(const s32, "game/code_00242608", func_00255E08);

INCLUDE_ASM(const s32, "game/code_00242608", func_00255EF8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00255F78);

INCLUDE_ASM(const s32, "game/code_00242608", func_00255FF8);

INCLUDE_ASM(const s32, "game/code_00242608", func_002560B8);

INCLUDE_ASM(const s32, "game/code_00242608", func_002561A0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00256290);

INCLUDE_ASM(const s32, "game/code_00242608", func_002562E8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00256400);

INCLUDE_ASM(const s32, "game/code_00242608", func_00256540);

INCLUDE_ASM(const s32, "game/code_00242608", func_00256B38);

INCLUDE_ASM(const s32, "game/code_00242608", func_00256B78);

u32 func_00256C00(s32 arg0) {
    u32 temp_v0;

    temp_v0 = *(u32 *)(arg0 + 0x10);
    func_002CFF98();
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00256C28);

INCLUDE_ASM(const s32, "game/code_00242608", func_00256C58);

INCLUDE_ASM(const s32, "game/code_00242608", func_00256D10);

INCLUDE_ASM(const s32, "game/code_00242608", func_00256D90);

INCLUDE_ASM(const s32, "game/code_00242608", func_00256E10);

void func_00256E88(void) {
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00256E90);

INCLUDE_ASM(const s32, "game/code_00242608", func_002570A8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00257150);

INCLUDE_ASM(const s32, "game/code_00242608", func_00257200);

INCLUDE_ASM(const s32, "game/code_00242608", func_00257270);

INCLUDE_ASM(const s32, "game/code_00242608", func_002573E8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00257670);

INCLUDE_ASM(const s32, "game/code_00242608", func_00257718);

INCLUDE_ASM(const s32, "game/code_00242608", func_002579B0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00257BD8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00257C10);

INCLUDE_ASM(const s32, "game/code_00242608", func_00257DF0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00257E78);

void func_00257EB0(s32 *arg0) {
    s32 temp_v0;

    temp_v0 = *arg0;
    *arg0 = temp_v0 + 1;
    if (0x3c < temp_v0 + 1) {
        *arg0 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00257ED0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00258258);

INCLUDE_ASM(const s32, "game/code_00242608", func_00258508);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF950);

INCLUDE_ASM(const s32, "game/code_00242608", func_00258620);

INCLUDE_ASM(const s32, "game/code_00242608", func_00258A70);

void func_00258AF0(u32 *arg0, u32 arg1) {
    arg0[1] = arg1;
    *arg0 = 0;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00258B00);

INCLUDE_ASM(const s32, "game/code_00242608", func_00258B90);

INCLUDE_ASM(const s32, "game/code_00242608", func_00258EB8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00258FD0);

INCLUDE_ASM(const s32, "game/code_00242608", func_002593E0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00259498);

INCLUDE_ASM(const s32, "game/code_00242608", func_00259890);

INCLUDE_ASM(const s32, "game/code_00242608", func_00259B40);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025A680);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025AA20);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025AB38);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025AC50);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025AD68);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025AE80);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025B0F0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF9B8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025B350);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025B7B0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025B7D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025B888);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025BA20);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025BC38);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025BCA0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025BD18);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025BDD0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025BF18);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025C030);

u8 func_0025C098(s32 arg0) {
    return *(s32 *)(arg0 + 0x6c) != 0;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0025C0A8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025C0D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025C1C8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025C278);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025C350);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025C418);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025C4C8);

void func_0025C568(s32 *arg0) {
    s32 temp_v0;

    temp_v0 = *arg0;
    *arg0 = temp_v0 + 1;
    if (0x3c < temp_v0 + 1) {
        *arg0 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0025C588);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025C7E0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025C830);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025C8D0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025CA50);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025CFA0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025D100);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025D2C0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025D2F8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025D628);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025D798);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025D7F8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025DA90);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025DAD0);

u32 func_0025DB58(s32 arg0) {
    u32 temp_v0;

    temp_v0 = *(u32 *)(arg0 + 0x10);
    func_002CFF98();
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0025DB80);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025DBB0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025DCC8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025DD80);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025DDF0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025DE60);

void func_0025DEC8(s32 arg0, u64 arg1, u64 arg2, u64 arg3,
                                    u64 arg4) {
    s32 temp_v0;
    u64 temp_v1;

    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0x70) + 0x14);
    if (*(s32 *)(temp_v0 + 0x20) != 0) {
        temp_v1 = func_00197C40(0x970, 0xb58, 1, *(u16 *)(*(s32 *)(temp_v0 + 0x1c) + 100), D_003BAA9C,
                                                    arg2);
        func_001954C8(temp_v1, arg3);
        func_001958A0(temp_v1, 1, arg4);
        func_00194920(temp_v1);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0025DF68);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025E108);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025E308);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025E420);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025E508);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025E5D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025E6B0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AF9F0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFA00);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFA18);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025E820);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025ECD0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025F138);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025F378);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025F408);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025F4E0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025F680);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025F7F0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025FB30);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025FC38);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025FD50);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025FE68);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025FEB8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0025FFC8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00260100);

INCLUDE_ASM(const s32, "game/code_00242608", func_00260208);

INCLUDE_ASM(const s32, "game/code_00242608", func_00260370);

void func_00260530(s32 arg0, u32 arg1) {
    u32 *puVar1;

    puVar1 = *(u32 **)(arg0 + 0x30);
    if (puVar1 != (u32 *)0x0) {
        *puVar1 = arg1;
        puVar1[1] = 1;
    }
}

void func_00260550(s32 arg0, u32 arg1) {
    u32 *puVar1;

    puVar1 = *(u32 **)(arg0 + 0x30);
    if (puVar1 != (u32 *)0x0) {
        *puVar1 = arg1;
        puVar1[1] = 2;
    }
}

void func_00260570(s32 arg0, u32 arg1) {
    u32 *puVar1;

    puVar1 = *(u32 **)(arg0 + 0x30);
    if (puVar1 != (u32 *)0x0) {
        *puVar1 = arg1;
        puVar1[1] = 1;
    }
}

void func_00260590(s32 arg0, u32 arg1) {
    u32 *puVar1;

    puVar1 = *(u32 **)(arg0 + 0x30);
    if (puVar1 != (u32 *)0x0) {
        *puVar1 = arg1;
        puVar1[1] = 2;
    }
}

void func_002605B0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0xac) = arg1;
    *(u32 *)(arg0 + 0xa8) = 0;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002605C0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00260670);

INCLUDE_ASM(const s32, "game/code_00242608", func_002609D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00260AB0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00261688);

INCLUDE_ASM(const s32, "game/code_00242608", func_00261760);

void func_00261F58(void) {
    func_00262818();
    D_003BC52A = 0;
    D_003BC52B = 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00261F80);

u32 func_00261F88(void) {
    D_003BC52A = 1;
    return 1;
}

void func_00261F98(void) {
    func_00262938();
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00261FB0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00261FB8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00261FD8);

void func_00262038(s32 arg0) {
    func_001198B8(*(u32 *)(arg0 + 0xc));
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00262050);

INCLUDE_ASM(const s32, "game/code_00242608", func_00262148);

void func_002622B0(u32 arg0, u32 arg1, u32 arg2) {
    func_00261FD8(arg1);
    func_00262038(arg1);
    func_00262148(arg0, arg2);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00262300);

void func_00262398(s32 arg0) {
    s32 temp_v0;

    temp_v0 = arg0 + 0x680;
    func_002BDD60(*(u32 *)(arg0 + 0x90));
    func_0027F6B8(temp_v0);
    func_0027FA20(temp_v0);
    func_00280488(temp_v0);
    func_00283038(*(u32 *)(arg0 + 0xd10));
    func_002832F8(*(u32 *)(arg0 + 0xd14));
    func_0027B010(arg0 + 0xd1c);
    func_00276320(arg0 + 0x4f8);
    func_00271648(arg0 + 0x4f8);
    func_00287548();
}

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFA88);

INCLUDE_ASM(const s32, "game/code_00242608", func_00262418);

INCLUDE_ASM(const s32, "game/code_00242608", func_002624C0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00262570);

void func_00262600(u32 arg0, u32 arg1, u32 arg2) {
    func_00262570(arg0, arg1, 2);
    func_00262570(arg0, arg2, 1);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00262640);

INCLUDE_ASM(const s32, "game/code_00242608", func_00262660);

INCLUDE_ASM(const s32, "game/code_00242608", func_00262790);

INCLUDE_ASM(const s32, "game/code_00242608", func_00262818);

INCLUDE_ASM(const s32, "game/code_00242608", func_002628C8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00262938);

INCLUDE_ASM(const s32, "game/code_00242608", func_00262970);

INCLUDE_ASM(const s32, "game/code_00242608", func_002629A8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00262A30);

INCLUDE_ASM(const s32, "game/code_00242608", func_00262A88);

void func_00262AC0(u32 arg0, s32 arg1) {
    func_00285A68(arg1 + 0x574);
    func_0027FF60(arg1 + 0x680);
    func_00280048(arg1 + 0x680);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00262AF8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00262BA8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00262C08);

INCLUDE_ASM(const s32, "game/code_00242608", func_00262CE8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00262EB8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00262F90);

INCLUDE_ASM(const s32, "game/code_00242608", func_00262FF0);

u32 func_00263050(void) {
    return 1;
}

u32 func_00263058(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00263060);

INCLUDE_ASM(const s32, "game/code_00242608", func_002630B0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00263100);

u32 func_00263138(void) {
    return 1;
}

u32 func_00263140(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00263148);

u32 func_00263220(u32 arg0) {
    u32 temp_v0;
    s32 temp_v1;

    temp_v1 = (s32)arg0;
    temp_v0 = func_00265E68(**(u32 **)(temp_v1 + 0x98), temp_v1 + 0x4c4);
    *(u32 *)(temp_v1 + 0x244) = temp_v0;
    func_00263148(arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00263260);

INCLUDE_ASM(const s32, "game/code_00242608", func_00263378);

u32 func_002633D8(void) {
    s64 temp_v0;

    temp_v0 = func_0021F600(0x911);
    if (temp_v0 == 0) {
        func_0021F580(0x911);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00263408);

INCLUDE_ASM(const s32, "game/code_00242608", func_00263570);

INCLUDE_ASM(const s32, "game/code_00242608", func_002635C0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00263608);

u32 func_00263638(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00263640);

INCLUDE_ASM(const s32, "game/code_00242608", func_00263728);

INCLUDE_ASM(const s32, "game/code_00242608", func_00263838);

void func_002639E0(s32 arg0) {
    func_0027B268(arg0 + 0xd1c, 0x20);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00263A00);

INCLUDE_ASM(const s32, "game/code_00242608", func_00263B78);

INCLUDE_ASM(const s32, "game/code_00242608", func_00263C98);

INCLUDE_ASM(const s32, "game/code_00242608", func_00263D10);

u32 func_00263D70(void) {
    s8 temp_v0;
    s32 temp_v1;
    u32 *puVar3;
    s8 *pcVar4;
    s32 temp_v2;
    s32 temp_v3;
    s32 temp_v4;

    temp_v1 = func_00101A70();
    temp_v4 = 0;
    temp_v2 = 4;
    temp_v3 = (*(s32 **)(temp_v1 + 0x98))[1] * 3;
    pcVar4 = (s8 *)(**(s32 **)(temp_v1 + 0x98) + 0x16);
    do {
        temp_v0 = *pcVar4;
        pcVar4 = pcVar4 + 1;
        temp_v2 = temp_v2 - 1;
        temp_v4 = temp_v4 + temp_v0;
    } while (-1 < temp_v2);
    *(u32 *)(temp_v1 + 0x3cc) = 0;
    temp_v2 = 4;
    puVar3 = (u32 *)(temp_v1 + 0x3e0);
    if (0x1ef - temp_v4 < temp_v3) {
        temp_v3 = 0x1ef - temp_v4;
    }
    *(s32 *)(temp_v1 + 0x3c8) = temp_v3;
    do {
        temp_v2 = temp_v2 - 1;
        *puVar3 = 0;
        puVar3 = puVar3 + -1;
    } while (-1 < temp_v2);
    if (*(s32 *)(temp_v1 + 0x1578) != 0) {
        func_002830F0(*(u32 *)(temp_v1 + 0xd10), 0);
    }
    return 1;
}

u32 func_00263E30(void) {
    return 1;
}

void func_00263E38(s32 arg0) {
    s32 temp_v0;
    u32 *puVar2;

    *(u32 *)(arg0 + 0x3cc) = 0;
    puVar2 = (u32 *)(arg0 + 0x3e0);
    temp_v0 = 4;
    do {
        temp_v0 = temp_v0 - 1;
        *puVar2 = 0;
        puVar2 = puVar2 + -1;
    } while (-1 < temp_v0);
}

void func_00263E70(u32 arg0, u32 arg1) {
    func_002CCE60(arg0, (s32)arg1 + 0x3d0);
    func_00262AC0(arg0, arg1);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00263EB0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00263EF8);

INCLUDE_ASM(const s32, "game/code_00242608", func_002641E0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00264238);

u32 func_00264280(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    func_002830F0(*(u32 *)(temp_v0 + 0xd10), 0xffffffffffffffff);
    func_0024DA58(0x16);
    func_0024DAE8(0);
    func_0024DAB8(0x1d);
    return 1;
}

u32 func_002642C8(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002642D0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00264498);

INCLUDE_ASM(const s32, "game/code_00242608", func_002644F0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00264538);

u32 func_00264608(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00264610);

INCLUDE_ASM(const s32, "game/code_00242608", func_002646A0);

INCLUDE_ASM(const s32, "game/code_00242608", func_002646F8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00264740);

u32 func_002647B0(void) {
    func_0024DBB0();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002647D0);

INCLUDE_ASM(const s32, "game/code_00242608", func_002648A8);

INCLUDE_ASM(const s32, "game/code_00242608", func_002649B0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00264A60);

INCLUDE_ASM(const s32, "game/code_00242608", func_00264AB8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFAB8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFAC8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFAD8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFAE8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00264B08);

INCLUDE_ASM(const s32, "game/code_00242608", func_00264D90);

INCLUDE_ASM(const s32, "game/code_00242608", func_00264E90);

INCLUDE_ASM(const s32, "game/code_00242608", func_00264EF0);

void func_00265078(void) {
}

void func_00265080(void) {
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00265088);

u32 func_002650B0(s32 arg0) {
    return *(u32 *)(arg0 + 0x1574);
}

void func_002650B8(s32 arg0) {
    *(u32 *)(arg0 + 0x1574) = 0;
}

void func_002650C0(void) {
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002650C8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00265220);

INCLUDE_ASM(const s32, "game/code_00242608", func_002652E0);

INCLUDE_ASM(const s32, "game/code_00242608", func_002653A0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00265478);

void func_002654E8(void) {
    func_00265088();
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00265500);

INCLUDE_ASM(const s32, "game/code_00242608", func_00265540);

u32 func_00265590(void) {
    return 1;
}

u32 func_00265598(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002655A0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00265610);

INCLUDE_ASM(const s32, "game/code_00242608", func_00265648);

INCLUDE_ASM(const s32, "game/code_00242608", func_002656C0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00265700);

INCLUDE_ASM(const s32, "game/code_00242608", func_002658B8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00265968);

INCLUDE_ASM(const s32, "game/code_00242608", func_002659C8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00265AB8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00265BE0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00265C28);

INCLUDE_ASM(const s32, "game/code_00242608", func_00265C90);

INCLUDE_ASM(const s32, "game/code_00242608", func_00265E68);

INCLUDE_ASM(const s32, "game/code_00242608", func_00265FD8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00266048);

void func_00266130(u32 arg0) {
    func_001953D8(arg0, 0xc, 0x10);
    func_001953A8(arg0, 0xfffffffffffffffc);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00266168);

INCLUDE_ASM(const s32, "game/code_00242608", func_002661A8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00266250);

INCLUDE_ASM(const s32, "game/code_00242608", func_002665E0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFB20);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFB30);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFB40);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFBA0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFBB0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFBC0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00266668);

INCLUDE_ASM(const s32, "game/code_00242608", func_00266908);

INCLUDE_ASM(const s32, "game/code_00242608", func_00266B10);

INCLUDE_ASM(const s32, "game/code_00242608", func_00266BC0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00266E28);

INCLUDE_ASM(const s32, "game/code_00242608", func_002673C8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00267850);

INCLUDE_ASM(const s32, "game/code_00242608", func_00267E20);

INCLUDE_ASM(const s32, "game/code_00242608", func_00267FF0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00268590);

INCLUDE_ASM(const s32, "game/code_00242608", func_002687C0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00268AB8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00268D40);

INCLUDE_ASM(const s32, "game/code_00242608", func_002692E0);

void func_002693E0(u32 arg0) {
    func_002E8F78(arg0, 0x7f, 0x3f);
}

void func_00269400(void) {
}

void func_00269408(void) {
}

void func_00269410(void) {
}

void func_00269418(void) {
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00269420);

INCLUDE_ASM(const s32, "game/code_00242608", func_00269450);

INCLUDE_ASM(const s32, "game/code_00242608", func_00269480);

u32 func_002694F8(void) {
    return 0x599;
}

u32 func_00269500(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    *(s32 *)(temp_v0 + 4) = *(s32 *)(temp_v0 + 4) + 1;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00269530);

INCLUDE_ASM(const s32, "game/code_00242608", func_00269558);

INCLUDE_ASM(const s32, "game/code_00242608", func_002695C8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00269628);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFBF0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFC30);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFC40);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFC50);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFC60);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFC70);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFC80);

INCLUDE_ASM(const s32, "game/code_00242608", func_00269658);

void func_002696F8(void) {
    func_002E97E8();
}

void func_00269710(void) {
    func_002E97E0();
}

void func_00269728(void) {
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00269730);

u32 func_00269758(void) {
    u64 temp_v0;

    temp_v0 = func_0010D428(0);
    func_002E8D10(temp_v0);
    return 1;
}

u32 func_00269780(void) {
    u64 temp_v0;

    temp_v0 = func_0010D428(0);
    func_002E8F78(temp_v0, 0x7f, 0x3f);
    return 1;
}

u32 func_002697B0(void) {
    func_002E8E50();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002697D0);

u32 func_00269820(void) {
    func_002E97E8();
    return 1;
}

u32 func_00269840(void) {
    u64 temp_v0;

    temp_v0 = func_002E97E0();
    func_0010D5F0(temp_v0);
    return 1;
}

u32 func_00269868(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00269870);

u32 func_002698C0(void) {
    func_0026A778();
    return 1;
}

u32 func_002698E0(void) {
    func_0026A808();
    func_0026A950();
    return 1;
}

u32 func_00269908(void) {
    func_0026A840();
    return 1;
}

u8 func_00269928(void) {
    s64 temp_v0;

    temp_v0 = func_0026A720();
    return temp_v0 == 0;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00269948);

INCLUDE_ASM(const s32, "game/code_00242608", func_002699A0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00269A68);

void func_00269B50(u32 arg0) {
    sceSdRemoteInit();
    func_002699A0(arg0);
    D_003BC5BC = 0;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00269B80);

INCLUDE_ASM(const s32, "game/code_00242608", func_00269C10);

INCLUDE_ASM(const s32, "game/code_00242608", func_00269CA0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00269D18);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026A1C8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026A1F8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026A248);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026A340);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026A390);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026A3E0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026A460);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026A490);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026A588);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026A5F0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026A6E0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026A720);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFCF0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026A778);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026A808);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026A840);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026A8A0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026A900);

void func_0026A950(void) {
    WaitSema(D_003BD8D0);
    func_0026A900();
    SignalSema(D_003BD8D0);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0026A980);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026AA28);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026ABA8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026AD28);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026AD98);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026ADE8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026AE10);

void func_0026AE50(void) {
    WaitSema(D_003BD8D0);
    func_0026ADE8();
    SignalSema(D_003BD8D0);
}

void func_0026AE80(void) {
    WaitSema(D_003BD8D0);
    func_0026AE10();
    SignalSema(D_003BD8D0);
}

void func_0026AEB0(void) {
    func_0026BFC8();
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0026AEC8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026AF30);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026AF78);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026B020);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026B050);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026B160);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026B1C0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFD80);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026B1F0);

u32 func_0026BCE8(void) {
    func_0026B050();
    return 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0026BD08);

u32 func_0026BD38(void) {
    func_0026B050(0);
    return 0;
}

void func_0026BD58(void) {
    func_0021FE38();
    func_00117730();
    func_001176A0();
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0026BD80);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026BE38);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026BEB0);

u32 func_0026BED0(void) {
    return **(u32 **)(*(s32 *)(D_003BC5D0 + 0x2c) + 0x1c);
}

void func_0026BEE8(s32 arg0) {
    func_0027BB08(*(u32 *)(D_003BC5D0 + 0x2c));
    if (0 < arg0) {
        do {
            arg0 = arg0 - 1;
            func_0027BE90(*(u32 *)(D_003BC5D0 + 0x2c));
        } while (arg0 != 0);
    }
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0026BF38);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026BFC8);

u32 func_0026C040(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0026C048);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026C098);

u8 func_0026C108(void) {
    return *(s32 *)(D_003BC5D0 + 8) != 0;
}

void func_0026C118(void) {
    if (*(s32 *)(D_003BC5D0 + 8) != 0) {
        func_002BDD60(*(s32 *)(D_003BC5D0 + 8));
        *(u32 *)(D_003BC5D0 + 8) = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0026C150);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026C188);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026C1C0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026C1F8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026C230);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026C290);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026C350);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026C4A8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026C4B8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026C7E0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026CA18);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026CAB0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026CAD0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026CB10);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026CD88);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026D108);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026D138);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026D150);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026D160);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026D270);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026D480);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026D510);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026D648);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFE40);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFE50);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFE80);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026D660);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026D808);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026DC50);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026DD10);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026DD30);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026DEA8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026DED0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026E160);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026E188);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026E240);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026E388);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026E4F0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026E5A0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026E608);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026E720);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026E798);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFEC8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFEE0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFEF8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFF18);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFF40);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFF58);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFF90);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFFC8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFFE0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003AFFF0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0008);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0018);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0028);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0038);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0050);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0068);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0078);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0088);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0098);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B00A8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B00C0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B00D0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B00E8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B00F8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0108);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0118);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0128);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0138);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0150);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0168);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0178);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0188);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0198);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B01B0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B01C0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B01D0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B01E8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B01F8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0208);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0218);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0230);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0240);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0250);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0260);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0270);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0288);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0298);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B02A8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B02B8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B02C8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B02D8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B02E8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B02F8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0310);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0320);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0338);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0350);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0360);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0370);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0388);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B03A0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B03B0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B03C0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B03D0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B03E0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B03F0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0408);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0420);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0438);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0450);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0460);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0470);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0480);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0490);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B04A8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B04C0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B04D0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B04E0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0500);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0518);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0538);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0548);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0558);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0568);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0578);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0588);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B05A0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B05B0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B05C0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B05D8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B05F0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0608);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0618);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0630);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0648);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0660);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0678);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0690);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B06A0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B06B0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B06C0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B06D8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B06F0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0700);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0710);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0728);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0738);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0748);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0758);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0768);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0778);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0788);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B07A0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B07B0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B07C0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B07D0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B07E0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B07F8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0808);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0818);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0828);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0840);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0850);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0860);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0870);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0880);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0898);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B08A8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B08B8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B08C8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B08D8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B08F0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0908);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0918);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0928);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0938);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0948);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0958);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0978);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0988);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B09A0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B09B8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B09C8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B09D8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B09E8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B09F8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0A08);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0A18);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0A28);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0A40);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0A50);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0A68);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0A78);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0A90);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0AA8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0AB8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0AD0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0AE0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0B00);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0B18);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0B38);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0B50);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0B78);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0B88);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0BA0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0BB0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0BC0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0BD0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0BF0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0C18);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0C30);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0C40);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0C58);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0C68);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0C80);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0C98);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0CA8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0CB8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0CC8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0CD8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0CF0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0D00);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0D20);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0D30);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0D48);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0D60);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0D70);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0D88);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0D98);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0DB0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0DC0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0DE0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0DF8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0E10);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0E20);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0E30);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0E48);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0E58);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0E70);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0E80);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0E98);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0EA8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0EC0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0ED0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0EE8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0F00);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0F18);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0F30);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0F40);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0F50);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0F60);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0F78);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0F88);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0FA0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0FB0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0FC8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0FE0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B0FF0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1008);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1018);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1028);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1050);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1060);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1078);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1088);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1098);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B10A8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B10B8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B10C8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B10E0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B10F0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1108);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1118);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026E8A0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026E8D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026EA70);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026EC90);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026F118);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026F230);

void func_0026F500(void) {
    func_00197348();
}

void func_0026F518(void) {
    func_00197378();
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0026F530);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026F5E8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1140);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026F918);

INCLUDE_ASM(const s32, "game/code_00242608", staffImageProc);

void func_0026FD88(void) {
    s64 temp_v0;

    D_003BA72C = 2;
    func_0026A808();
    func_0026A950();
    func_0026F518();
    do {
        temp_v0 = func_002D3EE8();
    } while (temp_v0 != 0);
    func_002D0A10(*D_003BC610);
    D_003BC610 = (u32 *)0x0;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0026FDD8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026FE10);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026FE98);

u32 func_0026FF18(void) {
    func_0026FE98();
    return 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0026FF38);

INCLUDE_ASM(const s32, "game/code_00242608", movieDraw);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1168);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1178);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1198);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B11B8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B11D8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B11F8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1218);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1238);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1258);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1278);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1298);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B12B8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B12D8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B12F8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1318);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1338);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1358);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1378);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1398);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B13B8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B13D8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B13F8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1418);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1438);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1458);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1478);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1498);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B14B8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B14D8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B14F8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1518);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1538);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1558);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1578);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1598);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B15B8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B15D8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B15F8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1618);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1638);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1658);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1678);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1698);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B16B8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B16D8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B16F8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1718);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1738);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1758);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1778);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1798);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B17B8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B17D8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B17F8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1818);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1838);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1850);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1868);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1888);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B18A0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B18B8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B18D0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B18F0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1908);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1920);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1938);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1958);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1970);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1990);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B19B0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B19C8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B19E8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1A08);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1A20);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1A38);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1A58);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026FFA0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0026FFF8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00270030);

INCLUDE_ASM(const s32, "game/code_00242608", func_00270068);

INCLUDE_ASM(const s32, "game/code_00242608", func_00270088);

INCLUDE_ASM(const s32, "game/code_00242608", func_00270098);

INCLUDE_ASM(const s32, "game/code_00242608", func_002700D0);

u32 func_00270110(void) {
    u64 temp_v0;

    temp_v0 = func_0010D428(0);
    func_0026FFF8(temp_v0);
    D_003BC630 = 0;
    return 1;
}

u32 func_00270140(void) {
    func_00270030();
    D_003BC630 = 0;
    func_00106690(0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00270170);

u8 func_00270218(void) {
    s64 temp_v0;

    temp_v0 = func_00270088();
    return temp_v0 == 2;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00270240);

INCLUDE_ASM(const s32, "game/code_00242608", func_002702A0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00270508);

INCLUDE_ASM(const s32, "game/code_00242608", func_00270558);

INCLUDE_ASM(const s32, "game/code_00242608", func_00270738);

INCLUDE_ASM(const s32, "game/code_00242608", movieViewer);

INCLUDE_ASM(const s32, "game/code_00242608", func_00270A30);

INCLUDE_ASM(const s32, "game/code_00242608", func_00270A80);

INCLUDE_ASM(const s32, "game/code_00242608", func_00270AC0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00270AD8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00270B10);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1AC8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00270BC8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00270FB0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00271020);

INCLUDE_ASM(const s32, "game/code_00242608", func_00271098);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1AF0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1B00);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1B10);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1B20);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1B30);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1B48);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1B60);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1B78);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1B88);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1BA0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1BB8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1BC8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1BE0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1BF8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1C10);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1C28);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1C38);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1C48);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1C60);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1C78);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1C90);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1CA8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1CB8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1CC8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1CD8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1CE8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1CF8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1D08);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1D28);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1D38);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1D48);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1D58);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1D68);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1D78);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1D88);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1D98);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1DA8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1DB8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1DC8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1DD8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1DE8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1DF8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1E08);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1E18);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1E28);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1E48);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1E58);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1E68);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1E78);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1E88);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1E98);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1EA8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1EB8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1EC8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1EE0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1F00);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1F18);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1F30);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1F48);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1F60);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1F70);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1F80);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1F90);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1FA0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1FB0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1FC0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1FD0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1FE0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B1FF0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B2000);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B2010);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B2020);

INCLUDE_ASM(const s32, "game/code_00242608", func_00271100);

INCLUDE_ASM(const s32, "game/code_00242608", func_00271180);

INCLUDE_ASM(const s32, "game/code_00242608", func_00271210);

INCLUDE_ASM(const s32, "game/code_00242608", func_002712A0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00271308);

INCLUDE_ASM(const s32, "game/code_00242608", func_00271368);

INCLUDE_ASM(const s32, "game/code_00242608", func_00271418);

void func_00271480(u32 arg0, u32 *arg1, u32 arg2, u32 arg3) {
    func_002802E0(arg0, arg3, arg1[3], 7, arg1[4], 0, *arg1, 0x11);
    func_0027FAA8(arg0, *arg1);
    func_0027FBE0(arg0, arg1 + 9);
    func_0027FC10(arg0, arg1 + 0x11);
    func_0027FC40(arg0, arg1 + 0x19);
    func_0027FF60(arg0);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00271500);

INCLUDE_ASM(const s32, "game/code_00242608", func_00271648);

INCLUDE_ASM(const s32, "game/code_00242608", func_002716E8);

INCLUDE_ASM(const s32, "game/code_00242608", func_002717D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00271948);

INCLUDE_ASM(const s32, "game/code_00242608", func_002719F0);

void func_00271B40(void) {
}

void func_00271B48(void) {
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00271B50);

INCLUDE_ASM(const s32, "game/code_00242608", func_00271D30);

INCLUDE_ASM(const s32, "game/code_00242608", func_00271DF8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00271E58);

INCLUDE_ASM(const s32, "game/code_00242608", func_00271F18);

u32 func_00271FC8(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    func_00283BF8(temp_v0 + 0x914, 0x53);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00271FF8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B20C0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B20D0);

INCLUDE_ASM(const s32, "game/code_00242608", func_002720B0);

INCLUDE_ASM(const s32, "game/code_00242608", func_002721E8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00272228);

u8 func_00272260(void) {
    s64 temp_v0;

    temp_v0 = func_00105C48();
    return temp_v0 == 0;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00272280);

INCLUDE_ASM(const s32, "game/code_00242608", func_00272350);

INCLUDE_ASM(const s32, "game/code_00242608", func_002723B0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00272518);

INCLUDE_ASM(const s32, "game/code_00242608", func_00272668);

INCLUDE_ASM(const s32, "game/code_00242608", func_00272688);

void func_00272778(u32 arg0) {
    func_00272688(0, arg0);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00272798);

INCLUDE_ASM(const s32, "game/code_00242608", func_002728F8);

INCLUDE_ASM(const s32, "game/code_00242608", func_002729C8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00272A00);

u32 func_00272A58(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    func_0027E790(*(u32 *)(temp_v0 + 0x138), *(u32 *)(temp_v0 + 0x6c), 0, 1);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00272A90);

INCLUDE_ASM(const s32, "game/code_00242608", func_00272B00);

INCLUDE_ASM(const s32, "game/code_00242608", func_00272B80);

u32 func_00272BB8(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00272BC0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00272D50);

void func_00273020(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x90c);
    func_0027C430(*(u32 *)(temp_v0 + 8));
    func_0027C430(*(u32 *)(temp_v0 + 0xc));
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00273050);

INCLUDE_ASM(const s32, "game/code_00242608", func_002730A0);

void func_00273200(s32 arg0) {
    func_0027C430(*(u32 *)(*(s32 *)(arg0 + 0x90c) + 0x10));
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00273220);

void func_00273390(u32 arg0) {
    func_00271308(2, arg0);
}

void func_002733B0(void) {
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002733B8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00273470);

INCLUDE_ASM(const s32, "game/code_00242608", func_002734C0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00273670);

INCLUDE_ASM(const s32, "game/code_00242608", func_00273718);

INCLUDE_ASM(const s32, "game/code_00242608", func_00273750);

void func_00273838(s32 arg0, s32 arg1) {
    s32 temp_v0;
    s64 temp_v1;

    temp_v0 = *(s32 *)(arg1 + 0x90c);
    temp_v1 = func_00273750();
    if (temp_v1 != 0) {
        *(u32 *)(*(s32 *)(*(s32 *)(*(s32 *)(temp_v0 + 8) + 0x14) + 0x1c) + 0x60) =
                  (u32)*(u8 *)(arg0 + D_003BAA00 + 0x12a0);
        *(s32 *)(temp_v0 + 0x28) = arg0;
    }
    func_00283BF0(arg1 + 0x914, 1);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002738A0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00273A30);

INCLUDE_ASM(const s32, "game/code_00242608", func_00273AB0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00273B98);

u32 func_00273C40(void) {
    return 1;
}

u32 func_00273C48(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00273C50);

INCLUDE_ASM(const s32, "game/code_00242608", func_00273D40);

INCLUDE_ASM(const s32, "game/code_00242608", func_00273DE8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00273E20);

INCLUDE_ASM(const s32, "game/code_00242608", func_00273F20);

INCLUDE_ASM(const s32, "game/code_00242608", func_00274008);

u32 func_00274040(void) {
    return 1;
}

u32 func_00274048(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00274050);

INCLUDE_ASM(const s32, "game/code_00242608", func_00274228);

INCLUDE_ASM(const s32, "game/code_00242608", func_00274310);

INCLUDE_ASM(const s32, "game/code_00242608", func_00274348);

INCLUDE_ASM(const s32, "game/code_00242608", func_00274430);

INCLUDE_ASM(const s32, "game/code_00242608", func_002744E0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00274610);

INCLUDE_ASM(const s32, "game/code_00242608", func_00274768);

INCLUDE_ASM(const s32, "game/code_00242608", func_00274978);

INCLUDE_ASM(const s32, "game/code_00242608", func_00274B30);

u32 func_00274B78(void) {
    return 1;
}

void func_00274B80(u32 arg0) {
    func_00271308(4, arg0);
}

void func_00274BA0(void) {
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00274BA8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00274BC0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00274D48);

void func_00274EE0(s32 arg0) {
    func_0027C430(*(u32 *)(*(s32 *)(arg0 + 0x90c) + 8));
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00274F00);

INCLUDE_ASM(const s32, "game/code_00242608", func_00275030);

INCLUDE_ASM(const s32, "game/code_00242608", func_00275328);

INCLUDE_ASM(const s32, "game/code_00242608", func_002754A0);

INCLUDE_ASM(const s32, "game/code_00242608", func_002754E0);

void func_002755A0(s32 arg0) {
    func_0027F0D8(arg0 + 0x15c);
    func_00285A68(arg0 + 0x7ec);
    func_0027FF60(arg0 + 0x15c);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002755E0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00275880);

INCLUDE_ASM(const s32, "game/code_00242608", func_002758D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00275920);

INCLUDE_ASM(const s32, "game/code_00242608", func_00275B40);

INCLUDE_ASM(const s32, "game/code_00242608", func_00275F48);

INCLUDE_ASM(const s32, "game/code_00242608", func_00276018);

INCLUDE_ASM(const s32, "game/code_00242608", func_002761C0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00276250);

u8 func_00276288(void) {
    s64 temp_v0;

    temp_v0 = func_002877A8();
    return temp_v0 != 1;
}

void func_002762B0(u32 arg0) {
    func_00271308(3, arg0);
}

void func_002762D0(void) {
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002762D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00276320);

INCLUDE_ASM(const s32, "game/code_00242608", func_00276368);

INCLUDE_ASM(const s32, "game/code_00242608", func_00276428);

void func_00276478(u32 arg0) {
    func_00276368(arg0, 1);
}

void func_00276490(void) {
    func_00276428();
}

void func_002764A8(u32 arg0) {
    func_00276368(arg0, 0);
}

void func_002764C0(void) {
    func_00276428();
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002764D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_002765E8);

INCLUDE_ASM(const s32, "game/code_00242608", func_002766E8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00276720);

INCLUDE_ASM(const s32, "game/code_00242608", func_00276898);

INCLUDE_ASM(const s32, "game/code_00242608", func_00276A18);

void func_00276B10(s32 arg0) {
    *(u32 *)
      (*(s32 *)(**(s32 **)(*(s32 *)(arg0 + 0x7d8) + 0x1c) * 0x134 + arg0 + 0x2b4) + 0x3c) = 0x100
    ;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00276B38);

INCLUDE_ASM(const s32, "game/code_00242608", func_00276C28);

INCLUDE_ASM(const s32, "game/code_00242608", func_00276DA0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00276E90);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B2100);

INCLUDE_ASM(const s32, "game/code_00242608", func_00276F70);

INCLUDE_ASM(const s32, "game/code_00242608", func_00277158);

INCLUDE_ASM(const s32, "game/code_00242608", func_00277220);

INCLUDE_ASM(const s32, "game/code_00242608", func_00277328);

INCLUDE_ASM(const s32, "game/code_00242608", func_00277390);

INCLUDE_ASM(const s32, "game/code_00242608", func_00277528);

INCLUDE_ASM(const s32, "game/code_00242608", func_002775D8);

u32 func_00277638(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00277640);

INCLUDE_ASM(const s32, "game/code_00242608", func_00277848);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B2208);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B2260);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B2270);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B2280);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B2290);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B22A0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B22B0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B22C0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B22D0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00277A50);

u32 func_00277C80(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    temp_v0 = *(s32 *)(temp_v0 + 0x90c);
    func_0027C430(*(u32 *)(temp_v0 + 0x24));
    *(u32 *)(temp_v0 + 0x24) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00277CB8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00277D38);

void func_00277DD0(u32 arg0) {
    func_00271308(1, arg0);
}

void func_00277DF0(void) {
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00277DF8);

INCLUDE_ASM(const s32, "game/code_00242608", func_002780D0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00278218);

void func_002782E0(void) {
    func_002CFF98();
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002782F8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00278330);

INCLUDE_ASM(const s32, "game/code_00242608", func_002786E8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00278760);

INCLUDE_ASM(const s32, "game/code_00242608", func_00278868);

INCLUDE_ASM(const s32, "game/code_00242608", func_002788D0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00278A90);

INCLUDE_ASM(const s32, "game/code_00242608", func_00278B90);

void func_00278BC8(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    *(u32 *)(*(s32 *)(temp_v0 + 0x90c) + 0x34) = 0xffffffff;
}

u32 func_00278BF0(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    return ~*(u32 *)(*(s32 *)(temp_v0 + 0x90c) + 0x34) >> 0x1f;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00278C20);

INCLUDE_ASM(const s32, "game/code_00242608", func_00278C90);

u32 func_00278D68(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    *(u32 *)(*(s32 *)(temp_v0 + 0x90c) + 0x30) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00278D90);

void func_00278E08(s32 arg0, s32 arg1) {
    *(u16 *)(arg1 * 2 + arg0 + 0x22) = 0;
    func_002CD0C0();
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00278E28);

INCLUDE_ASM(const s32, "game/code_00242608", func_00278F50);

INCLUDE_ASM(const s32, "game/code_00242608", func_00279130);

INCLUDE_ASM(const s32, "game/code_00242608", func_00279160);

INCLUDE_ASM(const s32, "game/code_00242608", func_00279328);

INCLUDE_ASM(const s32, "game/code_00242608", func_002793D0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00279568);

INCLUDE_ASM(const s32, "game/code_00242608", func_00279728);

INCLUDE_ASM(const s32, "game/code_00242608", func_00279760);

INCLUDE_ASM(const s32, "game/code_00242608", func_00279860);

INCLUDE_ASM(const s32, "game/code_00242608", func_002798F8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00279A30);

INCLUDE_ASM(const s32, "game/code_00242608", func_00279AF8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00279B30);

INCLUDE_ASM(const s32, "game/code_00242608", func_00279BA8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00279BF8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00279CC0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00279D68);

INCLUDE_ASM(const s32, "game/code_00242608", func_00279F88);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027A0A8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027A0E0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027A140);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027A300);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027A468);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027A4D0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027A540);

u32 func_0027A778(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    temp_v0 = *(s32 *)(temp_v0 + 0x90c);
    func_0027C430(*(u32 *)(temp_v0 + 0x24));
    *(u32 *)(temp_v0 + 0x24) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0027A7B0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027A810);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027A860);

void func_0027A9A8(s32 arg0) {
    func_002BF790(0x1c0, 0xa60, 0, 1, *(u32 *)(arg0 + 0x74), 0x1f, 0x53);
    func_002BF790(0x150, 0xa00, 0, 1, *(u32 *)(arg0 + 0x74), 0, 0x53);
    func_002BF790(0xbb0, 0xa00, 0, 1, *(u32 *)(arg0 + 0x74), 0, 0x53);
    func_002BF790(0x250, 0x9c0, 0, 1, *(u32 *)(arg0 + 0xe4), 0x18, 0x53);
    func_002BF790(0xce0, 0x9e0, 0, 1, *(u32 *)(arg0 + 100), 2, 0x53);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0027AA68);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027AB10);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027AC00);

void func_0027AC38(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;
    s32 temp_v2;

    temp_v1 = func_002BD908(2);
    *(u32 *)(arg0 + 0x18) = temp_v1;
    temp_v2 = func_002BD908(2);
    temp_v0 = *(s32 *)(temp_v2 + 8);
    *(s32 *)(arg0 + 0x1c) = temp_v2;
    func_002BDE18(temp_v0 + 0x28, *(u32 *)(arg0 + 0x14), 0, 0xc);
    func_002BDE18(*(s32 *)(*(s32 *)(arg0 + 0x1c) + 8) + 0x94, *(u32 *)(arg0 + 0x14), 1, 0xc)
    ;
    func_002BE128(*(u32 *)(arg0 + 0x10), 0, 0, *(u32 *)(*(s32 *)(arg0 + 0x1c) + 8));
    func_002BE128(*(u32 *)(arg0 + 0x10), 1, 0, *(s32 *)(*(s32 *)(arg0 + 0x1c) + 8) + 0x6c);
    func_002BE128(*(u32 *)(arg0 + 0x10), 2, 0, *(s32 *)(*(s32 *)(arg0 + 0x1c) + 8) + 0x6c);
    func_002BE128(*(u32 *)(arg0 + 0x10), 3, 0, *(u32 *)(*(s32 *)(arg0 + 0x1c) + 8));
    func_002BE128(*(u32 *)(arg0 + 0x10), 4, 0, *(u32 *)(*(s32 *)(arg0 + 0x1c) + 8));
    func_002BDE18(*(s32 *)(*(s32 *)(arg0 + 0x18) + 8) + 0x28, *(u32 *)(arg0 + 0x14), 2, 0xd)
    ;
    func_002BE0C0(*(u32 *)(arg0 + 4), 0, *(u32 *)(*(s32 *)(arg0 + 0x18) + 8));
    func_002BE258(*(u32 *)(arg0 + 8), 0, *(u32 *)(arg0 + 0x14), 3, 4);
    func_002BE258(*(u32 *)(arg0 + 0xc), 0, *(u32 *)(arg0 + 0x14), 4, 4);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0027AD80);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027AEA8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027AF28);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027B010);

void func_0027B088(s32 arg0, u32 arg1) {
    func_002C14E0(arg1);
    func_002BF790(0xffffffffffffff90, 0xa0, 0, 0x61, *(u32 *)(arg0 + 0x10), 0, arg1);
    func_002BF790(0xfffffffffffffb90, 0x808, 0, 0x61, *(u32 *)(arg0 + 0x10), 1, arg1);
    func_002BF790(0x1050, 0xfffffffffffffc18, 0, 0x61, *(u32 *)(arg0 + 0x10), 2, arg1);
    func_002BF790(0x10b0, 0x3c0, 0, 0x61, *(u32 *)(arg0 + 0x10), 3, arg1);
    func_002BF790(0x1300, 0xb70, 0, 0x61, *(u32 *)(arg0 + 0x10), 4, arg1);
    func_002C1548(0, arg1);
    func_002BF970(*(u32 *)(arg0 + 0x10), 0);
    func_002BF970(*(u32 *)(arg0 + 0x10), 1);
    func_002BF790(0, 0, 0, 0x60, *(u32 *)(arg0 + 4), 0, arg1);
    func_002BF970(*(u32 *)(arg0 + 4), 0);
    func_002C1588(arg1);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0027B1B0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027B268);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027B2F8);

u32 func_0027B368(u32 arg0) {
    s64 temp_v0;

    do {
        temp_v0 = func_0027B888(arg0);
    } while (temp_v0 != 0);
    func_002CFF98(arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0027B3A8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027B440);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027B4F0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027B540);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027B888);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027BA00);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027BA48);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027BA90);

void func_0027BB08(u32 arg0) {
    func_0027BA90(0, arg0);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0027BB28);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027BB48);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027BB80);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027BBF0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027BD48);

void func_0027BE90(u32 arg0) {
    func_0027BBF0(arg0, 0, 0);
}

void func_0027BEB0(u32 arg0) {
    func_0027BD48(arg0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0027BED0);

u32 func_0027BEF0(u32 *arg0) {
    return *arg0 & 2;
}

s32 func_0027BF00(s32 arg0) {
    return *(s32 *)(arg0 + 0x28) * *(s32 *)(arg0 + 0xc);
}

void func_0027BF10(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x10);
    if (temp_v0 != 0) {
        *(u32 *)(temp_v0 + 0x50) = 0;
        while (temp_v0 = *(s32 *)(temp_v0 + 0x58), temp_v0 != 0) {
            *(u32 *)(temp_v0 + 0x50) = 0;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0027BF48);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027BF90);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027C070);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027C0B0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027C140);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027C370);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027C3A0);

void func_0027C430(u32 arg0) {
    s32 temp_v0;

    func_0027B368(*(u32 *)((s32)arg0 + 0x14));
    temp_v0 = *(s32 *)((s32)arg0 + 0x84);
    if (temp_v0 != 0) {
        func_0027D1E8(temp_v0);
    }
    func_002CFF98(arg0);
}

void func_0027C470(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x3c) = arg1;
}

void func_0027C478(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x88) = arg1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0027C480);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027C558);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027C570);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027C590);

void func_0027C620(s32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4) {
    u32 temp_v0;

    temp_v0 = func_0027D148(arg1, arg2, arg3, arg4);
    *(u32 *)(arg0 + 0x84) = temp_v0;
}

void func_0027C658(s32 arg0) {
    *(u32 *)(arg0 + 4) = *(u32 *)(arg0 + 4) & 0xfffffffb;
}

void func_0027C670(s32 arg0) {
    func_0027B440(*(u32 *)(arg0 + 0x14));
}

void func_0027C688(s32 arg0) {
    func_0027B540(*(u32 *)(arg0 + 0x14));
}

void func_0027C6A0(s32 arg0) {
    func_0027B888(*(u32 *)(arg0 + 0x14));
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0027C6B8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027C708);

void func_0027C758(u32 arg0) {
    func_0027C6B8(arg0, 0);
}

void func_0027C770(u32 arg0) {
    func_0027C708(arg0, 0);
}

void func_0027C788(s32 arg0) {
    func_0027BED0(*(u32 *)(arg0 + 0x14));
}

void func_0027C7A0(s32 arg0) {
    func_0027BEF0(*(u32 *)(arg0 + 0x14));
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0027C7B8);

void func_0027CA78(void) {
    func_0027C7B8();
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0027CA90);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027CCD0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027CDD0);

void func_0027CEE8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0x14) + 0x20);
    if (0 < temp_v0) {
        do {
            temp_v0 = temp_v0 - 1;
        } while (temp_v0 != 0);
    }
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0027CF28);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027D148);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027D1E8);

void func_0027D248(s32 arg0, u32 arg1) {
    func_002BE378(*(u32 *)(arg0 + 0x10), 0, arg1, 0, 0x14, 0xc);
    func_002BE378(*(u32 *)(arg0 + 0x14), 0, arg1, 1, 10, 0xc);
    func_002BE378(*(u32 *)(arg0 + 0x18), 0, arg1, 2, 0, 0xc);
    func_002BE378(*(u32 *)(arg0 + 0x1c), 0, arg1, 2, 0, 0xc);
    func_002BE378(*(u32 *)(arg0 + 0x20), 0, arg1, 1, 10, 0xc);
    func_002BE378(*(u32 *)(arg0 + 0x24), 0, arg1, 0, 0x14, 0xc);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0027D318);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027D4A0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027D740);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027D7E0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027D850);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027DA80);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027DBD0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027DCE8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027DD58);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027DD78);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027DDA0);

void func_0027DE60(s32 arg0) {
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v2;

    temp_v2 = *(s32 *)(arg0 + 0x1c);
    temp_v1 = *(s32 *)(arg0 + 0x1c);
    while (temp_v0 = temp_v2, temp_v0 != 0) {
        temp_v1 = temp_v0;
        temp_v2 = *(s32 *)(temp_v0 + 0x5c);
    }
    *(s32 *)(arg0 + 0x10) = temp_v1;
}

void func_0027DE98(s32 arg0) {
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v2;

    temp_v2 = *(s32 *)(arg0 + 0x1c);
    temp_v1 = *(s32 *)(arg0 + 0x1c);
    while (temp_v0 = temp_v2, temp_v0 != 0) {
        temp_v1 = temp_v0;
        temp_v2 = *(s32 *)(temp_v0 + 0x58);
    }
    *(s32 *)(arg0 + 0x14) = temp_v1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0027DED0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027DF48);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027DFF8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027E020);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027E050);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027E078);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027E0A8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027E0D0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027E100);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027E228);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027E270);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027E2C0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027E2F0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027E3D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027E468);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027E4E0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027E570);

s32 func_0027E5C0(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v2;

    temp_v0 = func_002CFEB8(0x4c);
    memset(temp_v0, 0, 0x4c);
    *(u32 *)(temp_v0 + 8) = 0;
    *(u32 *)(temp_v0 + 0xc) = 0;
    temp_v2 = 1;
    func_002BFB98(temp_v0 + 0x10, arg0, arg1);
    func_002BFB98(temp_v0 + 0x18, arg2, arg3);
    temp_v1 = temp_v0;
    do {
        temp_v2 = temp_v2 - 1;
        func_002BFB98(temp_v1 + 0x20, 0, 0);
        func_002BFB98(temp_v1 + 0x30, 0, 0);
        temp_v1 = temp_v1 + 8;
    } while (-1 < temp_v2);
    func_0027E4E0(temp_v0);
    return temp_v0;
}

void func_0027E690(u32 arg0) {
    func_0027E570();
    func_002CFF98(arg0);
}

void func_0027E6B8(s32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    func_002BFB98(arg0 + 0x10);
    *(u32 *)(arg0 + 4) = arg3;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0027E6F0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027E790);

u8 func_0027E850(s32 arg0) {
    return *(s32 *)(arg0 + 0x20) != 0;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0027E860);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027E8D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027EAF0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027EC40);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027ECD8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027EF18);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027EFD0);

void func_0027F050(s32 arg0, u32 arg1, u32 arg2, u32 arg3, s32 arg4
                                    ) {
    u32 temp_v0;

    temp_v0 = func_002BDBC0(arg1, arg2, 1);
    *(u32 *)(arg0 + 0xe8) = temp_v0;
    temp_v0 = func_002BDBC0(arg1, arg3, 1);
    *(u32 *)(arg0 + 0xec) = temp_v0;
    if (-1 < arg4) {
        temp_v0 = func_002BDBC0(arg1, arg4, 1);
        *(u32 *)(arg0 + 0xf0) = temp_v0;
    }
}

void func_0027F0D8(s32 arg0) {
    u32 temp_v0;
    s32 *piVar2;
    u32 *puVar3;
    u32 *puVar4;
    u32 temp_v1;

    piVar2 = (s32 *)(arg0 + 0x168);
    puVar3 = (u32 *)(arg0 + 0x7c);
    puVar4 = (u32 *)(arg0 + 0x164);
    temp_v1 = 0;
    do {
        if (piVar2[-2] != 0) {
            func_002BDD60(piVar2[-2]);
        }
        if (piVar2[-1] != 0) {
            func_002BDD60(piVar2[-1]);
        }
        if (*piVar2 != 0) {
            func_002BDD60(*piVar2);
        }
        temp_v0 = *puVar3;
        temp_v1 = temp_v1 + 1;
        piVar2[-2] = 0;
        *puVar4 = 0;
        *puVar3 = temp_v0 & 0xffffff7f;
        puVar3 = puVar3 + 0x4d;
        *piVar2 = 0;
        piVar2 = piVar2 + 0x4d;
        puVar4 = puVar4 + 0x4d;
    } while (temp_v1 < 5);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0027F198);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027F1C8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027F230);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027F4B0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027F588);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027F638);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027F6B8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027F730);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027F838);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027F898);

void func_0027F9D8(s32 arg0, s32 arg1, u32 arg2) {
    u32 temp_v0;

    temp_v0 = func_0027F730(arg2);
    *(u32 *)(arg0 * 0x134 + arg1 + 0x15c) = temp_v0;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0027FA20);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027FA70);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027FAA8);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027FB90);

void func_0027FBE0(s32 arg0, u32 *arg1) {
    u32 temp_v0;
    u32 *puVar2;
    u32 temp_v1;

    puVar2 = (u32 *)(arg0 + 0x24);
    temp_v1 = 0;
    do {
        temp_v0 = *arg1;
        arg1 = arg1 + 1;
        temp_v1 = temp_v1 + 1;
        *puVar2 = temp_v0;
        puVar2 = puVar2 + 1;
    } while (temp_v1 < 8);
}

void func_0027FC10(s32 arg0, u32 *arg1) {
    u32 temp_v0;
    u32 *puVar2;
    u32 temp_v1;

    puVar2 = (u32 *)(arg0 + 0x44);
    temp_v1 = 0;
    do {
        temp_v0 = *arg1;
        arg1 = arg1 + 1;
        temp_v1 = temp_v1 + 1;
        *puVar2 = temp_v0;
        puVar2 = puVar2 + 1;
    } while (temp_v1 < 8);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_0027FC40);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027FCA0);

INCLUDE_ASM(const s32, "game/code_00242608", func_0027FF60);

INCLUDE_ASM(const s32, "game/code_00242608", func_00280048);

INCLUDE_ASM(const s32, "game/code_00242608", func_00280170);

void func_00280228(s32 arg0) {
    func_0027B368(*(u32 *)(arg0 + 0x67c));
    func_0027B368(*(u32 *)(arg0 + 0x680));
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00280258);

INCLUDE_ASM(const s32, "game/code_00242608", func_00280290);

INCLUDE_ASM(const s32, "game/code_00242608", func_002802E0);

INCLUDE_ASM(const s32, "game/code_00242608", func_002803A0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00280488);

INCLUDE_ASM(const s32, "game/code_00242608", func_002804F0);

INCLUDE_ASM(const s32, "game/code_00242608", func_002805B0);

INCLUDE_ASM(const s32, "game/code_00242608", func_002806E8);

INCLUDE_ASM(const s32, "game/code_00242608", func_002807E8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00280858);

void func_002808A8(s32 arg0) {
    u32 *puVar1;
    u32 temp_v0;

    puVar1 = (u32 *)(arg0 + 0x7c);
    temp_v0 = 0;
    do {
        temp_v0 = temp_v0 + 1;
        *puVar1 = *puVar1 & 0xfffffffe;
        puVar1 = puVar1 + 0x4d;
    } while (temp_v0 < 5);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002808E0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00280978);

INCLUDE_ASM(const s32, "game/code_00242608", func_00280A90);

INCLUDE_ASM(const s32, "game/code_00242608", func_00280BC0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00280D98);

INCLUDE_ASM(const s32, "game/code_00242608", func_00280E08);

INCLUDE_ASM(const s32, "game/code_00242608", func_00281108);

INCLUDE_ASM(const s32, "game/code_00242608", func_002811D0);

INCLUDE_ASM(const s32, "game/code_00242608", func_002812E8);

INCLUDE_ASM(const s32, "game/code_00242608", func_002814D0);

INCLUDE_ASM(const s32, "game/code_00242608", func_002815F0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00281688);

INCLUDE_ASM(const s32, "game/code_00242608", func_00281780);

void func_00281898(u32 arg0) {
    memset(arg0, 0, 0x20);
}

void func_002818B8(s32 arg0, s32 arg1) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = 1;
    temp_v0 = arg1 * 0x134 + arg0 + 0x16c;
    do {
        temp_v1 = temp_v1 - 1;
        func_00281898(temp_v0);
        temp_v0 = temp_v0 + 0x20;
    } while (-1 < temp_v1);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00281908);

INCLUDE_ASM(const s32, "game/code_00242608", func_002819F8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00281AE8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00281BE0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B22F0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B2310);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B2320);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B2330);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B2348);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B2358);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B2368);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B2380);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B23A0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B23B0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00281D40);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B23D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00282360);

INCLUDE_ASM(const s32, "game/code_00242608", func_00282850);

INCLUDE_ASM(const s32, "game/code_00242608", func_002828D0);

void func_002829C0(s32 arg0) {
    if (*(s32 *)(arg0 + 4) < 0x100) {
        *(s32 *)(arg0 + 4) = *(s32 *)(arg0 + 4) + 8;
    }
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002829E0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00282B08);

INCLUDE_ASM(const s32, "game/code_00242608", func_00282BE8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00282C10);

void func_00282C70(u32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 0x60);
    if (temp_v0 != 0) {
        func_0027D7E0(temp_v0);
    }
    func_002CFF98(arg0);
}

void func_00282CA8(s32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4) {
    *(u32 *)(arg0 + 0x18) = arg1;
    *(u32 *)(arg0 + 0x1c) = arg2;
    func_002BFB98(arg0 + 0x20, arg3, arg4);
}

void func_00282CD0(s32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4) {
    *(u32 *)(arg0 + 0x28) = arg1;
    *(u32 *)(arg0 + 0x2c) = arg2;
    func_002BFB98(arg0 + 0x30, arg3, arg4);
}

void func_00282CF8(s32 arg0) {
    u32 temp_v0;

    temp_v0 = func_0027D4A0(2);
    *(u32 *)(arg0 + 0x60) = temp_v0;
}

void func_00282D28(s32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4) {
    *(u32 *)(arg0 + 0x40) = arg1;
    *(u32 *)(arg0 + 0x44) = arg2;
    func_002BFB98(arg0 + 0x38, arg3, arg4);
}

void func_00282D50(s32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4, u32 arg5) {
    *(u32 *)(arg0 + 0x50) = arg1;
    *(u32 *)(arg0 + 0x54) = arg2;
    func_002BFB98(arg0 + 0x48, arg3, arg4);
    *(u32 *)(arg0 + 0x58) = arg5;
}

void func_00282D98(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00282DA0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00282F98);

INCLUDE_ASM(const s32, "game/code_00242608", func_00283038);

INCLUDE_ASM(const s32, "game/code_00242608", func_00283090);

void func_002830F0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x20) = arg1;
}

void func_002830F8(s32 arg0) {
    *(u32 *)(arg0 + 0x20) = 0xffffffff;
}

u32 func_00283108(s32 arg0) {
    return *(u32 *)(arg0 + 0x20);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00283110);

INCLUDE_ASM(const s32, "game/code_00242608", func_00283238);

INCLUDE_ASM(const s32, "game/code_00242608", func_00283280);

void func_002832F8(void) {
    func_002CFF98();
}

void func_00283310(u32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u16 arg4, s32 arg5, u32 arg6, u32 arg7) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = 0x11;
    if (arg5 != 0) {
        temp_v1 = 0x12;
    }
    temp_v0 = func_002860B8(arg4);
    func_002BF4E0(arg0, arg1, arg2, arg3, 1, arg6, temp_v0 * 2 + temp_v1, arg7);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002833B0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00283788);

void func_00283820(void) {
    func_002CFF98();
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00283838);

INCLUDE_ASM(const s32, "game/code_00242608", func_00283BE0);

void func_00283BF0(u32 *arg0, u32 arg1) {
    *arg0 = arg1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00283BF8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00283CA8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00283CE8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00283D10);

INCLUDE_ASM(const s32, "game/code_00242608", func_00283E60);

INCLUDE_ASM(const s32, "game/code_00242608", func_00283EE0);

void func_00284080(s32 arg0) {
    u32 temp_v0;

    temp_v0 = func_002BD258(3);
    *(u32 *)(arg0 + 0x40) = temp_v0;
    temp_v0 = func_002BD258(3);
    *(u32 *)(arg0 + 0x44) = temp_v0;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002840B8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00284108);

INCLUDE_ASM(const s32, "game/code_00242608", func_00284258);

INCLUDE_ASM(const s32, "game/code_00242608", func_00284340);

INCLUDE_ASM(const s32, "game/code_00242608", func_002843A0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00284418);

INCLUDE_ASM(const s32, "game/code_00242608", func_002845F8);

void func_00284880(void) {
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00284888);

INCLUDE_ASM(const s32, "game/code_00242608", func_002848E0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00284A90);

void func_00284BF8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00284C00);

void func_00284C10(s32 arg0, s32 arg1) {
    if (*(s32 *)(arg0 + 0x20) != arg1) {
        *(u32 *)(arg0 + 0x8c) = 0x100;
    }
    *(s32 *)(arg0 + 0x20) = arg1;
}

void func_00284C28(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x1c) = arg1;
}

void func_00284C30(void) {
    func_002CFF98();
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00284C48);

INCLUDE_ASM(const s32, "game/code_00242608", func_00284EB8);

INCLUDE_ASM(const s32, "game/code_00242608", func_002850C8);

INCLUDE_ASM(const s32, "game/code_00242608", func_002850D8);

void func_00285160(void) {
    func_002CFF98();
}

void func_00285178(s32 arg0, u32 arg1, u32 arg2, u32 arg3,
                                    u32 arg4) {
    func_002BFB98((u32 *)(arg0 + 0x18));
    func_002BF9E0(*(u32 *)(arg0 + 0x18), *(u32 *)(arg0 + 0x1c), 0, 0, 0, 0);
    func_002BFB98(arg0 + 0x20, arg1, arg3);
    func_002BFB98(arg0 + 0x28, arg1, arg4);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00285208);

INCLUDE_ASM(const s32, "game/code_00242608", func_00285440);

void func_00285490(u32 arg0) {
    memset(arg0, 0, 0x4c);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002854B0);

void func_00285600(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)arg0;
    while (temp_v0 != 0) {
        func_002854B0(1, 0, arg0, arg1);
        temp_v0 = *(s32 *)arg0;
    }
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00285658);

INCLUDE_ASM(const s32, "game/code_00242608", func_00285670);

u8 func_002858D8(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 0x44) == arg1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002858E8);

INCLUDE_ASM(const s32, "game/code_00242608", func_002858F8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00285910);

void func_00285928(s32 arg0, u32 arg1) {
    if (*(s32 *)(arg0 + 0x44) != 0) {
        func_002858F8(arg1, *(s32 *)(arg0 + 0x44));
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00285960);

INCLUDE_ASM(const s32, "game/code_00242608", func_00285A68);

INCLUDE_ASM(const s32, "game/code_00242608", func_00285B20);

INCLUDE_ASM(const s32, "game/code_00242608", func_00285E00);

INCLUDE_ASM(const s32, "game/code_00242608", func_00285F00);

void func_00286050(u32 arg0) {
    func_00285F00(arg0, 0);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00286068);

INCLUDE_ASM(const s32, "game/code_00242608", func_002860B8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00286138);

INCLUDE_ASM(const s32, "game/code_00242608", func_00286170);

INCLUDE_ASM(const s32, "game/code_00242608", func_002861B8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00286288);

u8 func_002862C0(u32 arg0) {
    return *(u8 *)((arg0 & 0xffff) * 0x38 + D_003BAA50 + 3);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002862E0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00286368);

INCLUDE_ASM(const s32, "game/code_00242608", func_00286440);

INCLUDE_ASM(const s32, "game/code_00242608", func_002864D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00286540);

INCLUDE_ASM(const s32, "game/code_00242608", func_002865B8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00286648);

void func_002866B0(u16 arg0) {
    func_00118E38(arg0);
}

u32 func_002866C8(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002866D0);

u8 func_002868C0(u32 arg0) {
    return *(s8 *)((arg0 & 0xffff) * 2 + D_003BAA4C) == '\x01';
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002868E0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00286990);

INCLUDE_ASM(const s32, "game/code_00242608", func_002869E8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00286A00);

INCLUDE_ASM(const s32, "game/code_00242608", func_00286A18);

u32 func_00286AC0(s32 arg0, u16 arg1) {
    *(u16 *)(arg0 + 0x52) = arg1;
    return 1;
}

u16 func_00286AD0(s32 arg0) {
    return *(u16 *)(arg0 + 0x52);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00286AD8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B2420);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B2430);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B2450);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B2468);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B2480);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B24A8);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B24D0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B24E8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00286B48);

INCLUDE_ASM(const s32, "game/code_00242608", func_00286D20);

INCLUDE_ASM(const s32, "game/code_00242608", func_00286E50);

INCLUDE_ASM(const s32, "game/code_00242608", func_00286EA0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00286EF8);

u8 func_00286F48(void) {
    return D_003BC7B4 != 0;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00286F58);

INCLUDE_ASM(const s32, "game/code_00242608", func_00286F80);

INCLUDE_ASM(const s32, "game/code_00242608", func_00286F90);

INCLUDE_ASM(const s32, "game/code_00242608", func_00286FA0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00287040);

INCLUDE_ASM(const s32, "game/code_00242608", func_002870D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00287138);

INCLUDE_ASM(const s32, "game/code_00242608", func_00287198);

INCLUDE_ASM(const s32, "game/code_00242608", func_002871C0);

INCLUDE_ASM(const s32, "game/code_00242608", func_002871E8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00287220);

INCLUDE_ASM(const s32, "game/code_00242608", func_00287258);

INCLUDE_ASM(const s32, "game/code_00242608", func_00287420);

INCLUDE_ASM(const s32, "game/code_00242608", func_00287450);

INCLUDE_ASM(const s32, "game/code_00242608", func_002874E8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00287548);

INCLUDE_ASM(const s32, "game/code_00242608", func_00287580);

INCLUDE_ASM(const s32, "game/code_00242608", func_002875A8);

u32 func_002875E8(u32 *arg0) {
    return *arg0 & 1;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002875F8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00287678);

INCLUDE_ASM(const s32, "game/code_00242608", func_00287698);

void func_00287788(u16 arg0, u32 arg1) {
    func_00287698(arg0, 0xffffffffffffffff, arg1);
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002877A8);

INCLUDE_ASM(const s32, "game/code_00242608", func_002878D8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00287998);

INCLUDE_ASM(const s32, "game/code_00242608", func_00287A18);

INCLUDE_ASM(const s32, "game/code_00242608", func_00287A50);

INCLUDE_ASM(const s32, "game/code_00242608", func_00287B88);

INCLUDE_ASM(const s32, "game/code_00242608", func_00287C20);

INCLUDE_ASM(const s32, "game/code_00242608", func_00287D98);

INCLUDE_ASM(const s32, "game/code_00242608", func_00287E60);

INCLUDE_ASM(const s32, "game/code_00242608", func_00287EC8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00287FA8);

INCLUDE_ASM(const s32, "game/code_00242608", func_00287FD0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00287FE0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00288008);

INCLUDE_ASM(const s32, "game/code_00242608", func_00288148);

INCLUDE_ASM(const s32, "game/code_00242608", func_00288190);

INCLUDE_ASM(const s32, "game/code_00242608", func_002881D0);

INCLUDE_ASM(const s32, "game/code_00242608", func_002881E0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B2520);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B2530);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B2540);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B2550);

INCLUDE_ASM(const s32, "game/code_00242608", func_002881F0);

u32 func_00288458(void) {
    func_002881F0();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00242608", func_00288478);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B25A0);

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B25B0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00288500);

void func_002886A0(void) {
    func_0021FE38();
}

INCLUDE_ASM(const s32, "game/code_00242608", func_002886B8);

INCLUDE_ASM(const s32, "game/code_00242608", func_002886E0);

INCLUDE_ASM(const s32, "game/code_00242608", func_00288728);

void func_00288788(void) {
    func_00288728();
}

INCLUDE_RODATA(const s32, "game/code_00242608", D_003B2608);

