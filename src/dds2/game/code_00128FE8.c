#include "common.h"

extern u32 D_00436014;

extern u32 D_00436018;

extern u32 D_00435FC0;

extern u32 D_00436060;

extern s32 D_00436068;

extern s32 func_003283E0(u32);

extern u32 D_00436088;

extern u64 func_0010FFA8(void);

extern s64 func_00110C18(u64);

extern s64 func_00125F38(void);

extern u32 D_004360AC;

extern u32 D_004360C4;

extern u32 D_004360C8;

extern u32 D_004360CC;

extern u32 D_004360D0;

extern u32 D_00435F0C;

extern s32 D_004360F8;

extern u32 D_004360FC;

extern s32 D_00436100;

extern s32 D_00438ECC;

extern u32 D_0043610C;

extern u32 D_00436110;

extern u32 D_00436114;

extern u32 D_00436108;

extern u32 D_00436178;

extern u32 D_00436180;

extern u32 D_0043618C;

extern u32 D_00436190;

extern s32 D_004361F8;

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00128FE8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001295E0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00129660);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00129940);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00129B40);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00129CC0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00129D40);

s32 func_00129D60(s32 arg0) {
    return arg0 + 0xc;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00129D68);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00129E50);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00129E78);

void func_00129EA0(u32 arg0, u32 arg1) {
    D_00436014 = arg0;
    D_00436018 = arg1;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00129EB0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00129F58);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012A078);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012A0E8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012A140);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012A150);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012A190);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012A1F8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012A270);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012A2E8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012A360);

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_004130D8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012A3D8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012A4E0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012A510);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012A698);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012A6F0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012AC90);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012ADA0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012B068);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012B0D0);

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_00413198);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012B2B8);

void func_0012B4A0(u32 arg0) {
    D_00435FC0 = arg0;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012B4A8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012B4F8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012B518);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012B690);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012B7F8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012B9B8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012BA90);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012BB68);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012BBD0);

void func_0012BC38(u32 arg0) {
    D_00436060 = arg0;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012BC40);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012BD00);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012BE18);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012BF38);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012C098);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012C1D8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012C360);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012C4F0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012C650);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012C888);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012CB08);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012CDC0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012CF50);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012D070);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012D110);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012D1B0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012D260);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012D3E0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012D5C0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012D7E0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012D9D0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012DAA0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012DB68);

void func_0012DC70(void) {
    if (D_00436068 == 0) {
        D_00436068 = func_003283E0(0x70000);
    }
}

void func_0012DC98(void) {
    if (D_00436068 != 0) {
        func_00328470(D_00436068);
        D_00436068 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012DCC8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012DD48);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012DDC0);

void func_0012DE10(s32 arg0) {
    *(s32 *)(arg0 + 0x20) = *(s32 *)(arg0 + 0x20) + 0x60;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012DE20);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012DE70);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012E008);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012E0F8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012E1F8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012E2F8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012E3F8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012E4F0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012E5F8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012E720);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012E958);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012EB78);

void func_0012EBB8(u32 arg0) {
    D_00436088 = arg0;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", encProc);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012EBF8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012EC80);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012ECF0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012ED08);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012ED48);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012EDB0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012F078);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012F400);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012F770);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012F908);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012FA58);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001300A0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001302A0);

s64 func_001309B8(void) {
    u64 temp_v0;
    s64 temp_v1;
    s64 temp_v2;

    temp_v0 = func_0010FFA8();
    temp_v1 = func_00110C18(temp_v0);
    temp_v2 = func_00125F38();
    if (temp_v2 == temp_v1) {
        temp_v1 = 0;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001309F8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00130A40);

void func_00130C20(void) {
    D_004360AC = 0;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00130C28);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00130DE0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00130F38);

void func_00130F80(void) {
    D_004360C8 = 0;
    D_004360C4 = 0xffffffff;
    D_004360CC = 0;
    D_004360D0 = 0;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00130F98);

void func_00130FF0(u32 arg0, u32 arg1) {
    D_004360C4 = arg0;
    D_004360C8 = arg1;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00131000);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00131478);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00131A90);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00131B50);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001321F8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001322D8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00132408);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00132540);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133840);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133950);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133988);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001339C0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133A00);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133A28);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133A50);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133A78);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133AA8);

void func_00133AE8(void) {
    func_00111B60(D_00435F0C, 0x400);
    func_00111B60(D_00435F0C, 0x200);
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133B10);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133C18);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133C90);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133CF8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133D40);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133DB8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133DC8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133DD8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133DF8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133E38);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133EE0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133F08);

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_00413280);

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_00413290);

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_004132A0);

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_004132E0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001343E8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00134620);

void func_00134790(void) {
    if (D_00436100 != 0) {
        func_0032BBB0(D_00436100);
        D_00436100 = 0;
    }
    if (D_00438ECC != 0) {
        func_003298C0(D_00438ECC);
        D_00438ECC = 0;
    }
    if (D_004360F8 != 0) {
        func_002DEBB0(D_004360F8);
        D_004360F8 = 0;
    }
    D_004360FC = 0;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001347F0);

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_00413350);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00134910);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00134A18);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001354F0);

void func_00135568(u32 arg0) {
    D_0043610C = arg0;
    D_00436110 = 0;
    D_00436114 = 0;
}

void func_00135578(u32 arg0) {
    D_00436108 = arg0;
}

u32 func_00135580(void) {
    return D_00436108;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00135588);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00135598);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001355A8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001355D8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00135840);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00135A68);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00135D18);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00135D80);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00136098);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001360B8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00136368);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00136388);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001363D8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00136718);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00136850);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00136A70);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00136C90);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00136E60);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00136EF8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00137818);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00137888);

void func_001378A0(void) {
    D_004360FC = 0;
    if (D_00436100 != 0) {
        func_0032BBB0(D_00436100);
        D_00436100 = 0;
    }
    if (D_004360F8 != 0) {
        func_002DEBB0(D_004360F8);
        D_004360F8 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001378E8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001379C0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00137B60);

void func_00137BC8(void) {
    func_00329910(D_00436190);
    func_003298C0(D_00436190);
    D_00436180 = 0;
    func_00329910(D_0043618C);
    func_003298C0(D_0043618C);
    D_00436178 = 0;
}

float func_00137C08(float *arg0, float *arg1) {
    return *arg0 * *arg1 + arg0[1] * arg1[1] + arg0[2] * arg1[2];
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00137C38);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00137C68);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00137E00);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00137E48);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00137E90);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00137F10);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00139400);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00139628);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00139950);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00139B98);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00139EC0);

void func_0013AA78(void) {
}

void func_0013AA80(void) {
}

void func_0013AA88(void) {
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013AA90);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013AC40);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013B0D0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013B4F8);

void func_0013B810(void) {
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013B818);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013B970);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013BA98);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013BAB8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013D308);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013D598);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013D7F8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013DA10);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013DB08);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013DBB0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013DC70);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013DCC8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013DD18);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013DDC0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013E958);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013E9B8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013EA18);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013EA78);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013EAD0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013EB30);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013ED20);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013EEA0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013F108);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013F168);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013F1B8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013F1E8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013F3E0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013F5C8);

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_004133A0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013F790);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013FA98);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013FFF8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001400F8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00140180);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00140238);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001402B8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001404E0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001406E8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00140750);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00140780);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00140830);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00140A58);

u8 func_00140B80(void) {
    return D_004361F8 == 8;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00140B90);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00140BC8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001411F8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00141840);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00141898);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00141B20);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00141CF0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00141F58);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001420F0);

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_004134C0);

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_004134D0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001421C0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001422E8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00142478);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00142618);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00142670);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00142990);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00142A10);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00142AB8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00142B70);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00143768);

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_004135D0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00143910);

void func_00143C90(void) {
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00143C98);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00143D90);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00143F78);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00144028);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00144178);
