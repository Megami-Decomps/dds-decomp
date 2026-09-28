#include "common.h"

extern s32 D_00435F14;

extern s32 D_00435F18;

extern s32 D_00435F10;

extern s32 D_00435E04;

extern u32 func_0011F218(void);

extern u64 func_0032CE80(u64);

extern u64 func_0019F460(s32, s32, u64, u64, u64, u64);

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

extern u32 D_00389770[];

extern u32 D_00435FCC;

extern u32 func_002C8168(u32 arg0);

extern u8 D_00435BB4;

extern u32 D_003897E8[];

extern char D_00444950[];

extern void func_0012A3D8(char *arg0);

extern s32 strcmp(const char *a, const char *b);

extern u32 D_00435FD8;

extern u32 D_00435FC4;

extern u8 D_00436020[];

extern void func_003298C0(u32 arg0);

extern void func_002C7CE8(u32 arg0);

extern u8 D_003846F0[];

extern u8 D_0037F610[];

extern u8 D_0037F660[];

extern void func_00336C10(void *src);

extern u32 D_00389904[];

extern u32 D_00389910[];

extern u32 D_003899C0[];

extern void func_0012D9D0(u32 value);

extern void func_001295E0(u32, u32);

extern void func_00126040(void);

extern void func_00128FE8(u32, u32, s32);

extern u32 D_00436064;

extern u32 D_0043607C;

extern u32 D_00436080;

extern u32 D_00438EC8;

extern u8 D_0038A700[];

extern void *func_003335E0(void);

extern u32 func_0032C138(void *);

extern u32 D_004360B0;

extern u8 D_00444980[];

extern u8 D_00444970[];

extern void func_00113110(s64 arg0, void *arg1, void *arg2);

extern f32 func_00340898(f32 arg0, f32 arg1);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00128FE8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001295E0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00129660);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00129940);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00129B40);

void func_00129CC0(u32 *args) {
    s32 state;
    func_001295E0(args[4], args[3]);
    state = D_00389770[4];
    if (state != 1 && state < 200) func_00126040();
    func_00128FE8(args[1], args[0], 0);
    D_00389770[1] = ((u32 *)args[2])[1];
}

void func_00129D40(u32 *arg0) {
    func_00128FE8(arg0[1], *arg0, 1);
}

s32 func_00129D60(s32 arg0) {
    return arg0 + 0xc;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00129D68);

void func_00129E50(u32 arg0, s32 arg1) {
    func_00344120(arg0, arg0, (s32)arg0 + *(s32 *)(arg1 + 4), *(u32 *)(arg1 + 8));
}

void func_00129E78(u32 arg0, s32 arg1) {
    func_00129D68(arg0, arg0, (s32)arg0 + *(s32 *)(arg1 + 4), *(u32 *)(arg1 + 8));
}

void func_00129EA0(u32 arg0, u32 arg1) {
    D_00436014 = arg0;
    D_00436018 = arg1;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00129EB0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00129F58);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012A078);

u32 func_0012A0E8(void) {
    u32 temp_v1 = D_00389770[0x1E];

    if (temp_v1 != 0) {
        if (temp_v1 == 1) {
            if (func_002C8168(D_00435FCC) != 0) {
                D_00389770[0x1E] = 0;
                D_00435BB4 = 0;
            }
        }
    }
    return 0;
}

u32 func_0012A140(void) {
    return D_003897E8[0];
}

u8 func_0012A150(void) {
    if (D_00435FCC != 0) {
        if (func_002C8168(D_00435FCC) != 0) {
            return 1;
        }
    }
    return D_003897E8[0] != 0;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012A190);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012A1F8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012A270);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012A2E8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012A360);

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_004130D8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012A3D8);

u8 func_0012A4E0(void) {
    char buf[32];

    func_0012A3D8(buf);
    return strcmp(D_00444950, buf) != 0;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012A510);

void func_0012A698(void) {
    u32 temp_v0 = D_00435FD8;

    if (temp_v0 != 0) {
        func_003298C0(temp_v0);
        D_00435FD8 = 0;
    }
    temp_v0 = D_00435FC4;
    if (temp_v0 != 0) {
        func_002C7CE8(temp_v0);
        D_00435FC4 = 0;
    }
    D_00444950[0] = D_00436020[0];
}

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

void func_0012B4A8(void) {
    if (D_00436064 == 0) {
        void *object;
        D_00436064 = 1;
        object = func_003335E0();
        D_0043607C = (u32)object;
        *(f32 *)((u8 *)object + 0x1C) = 1.0f;
        D_00438EC8 = (u32)func_003335E0();
        D_00436080 = func_0032C138(D_0038A700);
    }
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012B4F8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012B518);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012B690);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012B7F8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012B9B8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012BA90);

void func_0012BB68(void) {
    u8 *matrix;
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf28, 0(%0)\n"
        "lqc2 vf29, 0x10(%0)\n"
        "lqc2 vf30, 0x20(%0)\n"
        "lqc2 vf31, 0x30(%0)\n"
        ".set reorder"
        : : "r"(D_003846F0) : "memory");
    matrix = D_0037F610;
    func_00336C10(matrix);
    __asm__ volatile (
        ".set noreorder\n"
        "vmove.xyzw vf24, vf28\n"
        "vmove.xyzw vf25, vf29\n"
        "vmove.xyzw vf26, vf30\n"
        "vmove.xyzw vf27, vf31\n"
        ".set reorder"
        : : : "memory");
    matrix += 0x40;
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf11, 0(%0)\n"
        ".set reorder"
        : : "r"(matrix) : "memory");
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf12, 0(%0)\n"
        ".set reorder"
        : : "r"(D_0037F660) : "memory");
}

void func_0012BBD0(f32 *dstX, f32 *dstY, f32 x, f32 y, f32 z) {
    f32 vec[4] = { x, y, z, 1.0f };
    f32 result[4];
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf10, 0(%0)\n"
        "vmulax.xyzw ACC, vf28, vf10x\n"
        "vmadday.xyzw ACC, vf29, vf10y\n"
        "vmaddaz.xyzw ACC, vf30, vf10z\n"
        "vmaddw.xyzw vf10, vf31, vf0w\n"
        "vdiv Q, vf0w, vf10w\n"
        "vmove.w vf10, vf0\n"
        "vwaitq\n"
        "vmulq.xyzw vf10, vf10, Q\n"
        "vmul.xyzw vf10, vf10, vf11\n"
        "vadd.xyzw vf10, vf10, vf12\n"
        ".set reorder"
        : : "r"(vec) : "memory");
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(result) : "memory");
    *dstX = result[0];
    *dstY = result[1];
}

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

void func_0012DDC0(s32 arg0, s32 arg1, u64 arg2, u64 arg3) {
    u64 temp_v0;

    temp_v0 = func_0019F460(arg0 << 4, arg1 << 4, 0, arg2, arg3, 0);
    func_0019D518(temp_v0);
    func_0019C5B0(temp_v0);
}

void func_0012DE10(s32 arg0) {
    *(s32 *)(arg0 + 0x20) = *(s32 *)(arg0 + 0x20) + 0x60;
}

void func_0012DE20(s32 arg0) {
    u64 temp_v0;
    u32 temp_v1;

    temp_v1 = func_0011F218();
    *(u32 *)(arg0 + 0x28) = temp_v1;
    temp_v0 = func_0032CE80(0x40);
    func_0032E4B8(temp_v0);
    func_0032CEE8(*(u32 *)(arg0 + 0x28), temp_v0);
}

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

void func_0012EBF8(u32 arg0, s32 arg1) {
    if ((arg1 < 0x400) && ((*(u16 *)((s32)arg1 * 0x28 + D_00435E04 + 0x20) & 0x8000) != 0))
    {
        func_00105FE8(0);
        func_0012EC80(3);
        return;
    }
    func_00341E20(0xf, 0x7f, 0x3f);
    func_0012EC80(arg0);
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012EC80);

void func_0012ECF0(void) {
    func_0022E390();
}

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

void func_001309F8(u32 value) {
    D_004360AC = value;
    func_00113110(func_00110C18(func_0010FFA8()), D_00444980, D_00444970);
    D_004360B0 = 0;
}

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

void func_00133988(void) {
    func_00111B30(D_00435F0C, 1);
    if (D_00435F10 != 0) {
        func_00111B30(D_00435F10, 1);
    }
    func_00133C90();
}

void func_001339C0(void) {
    func_00111B60(D_00435F0C, 1);
    if (D_00435F10 != 0) {
        func_00111B60(D_00435F10, 1);
        return;
    }
}

void func_00133A00(void) {
    func_00112B58(D_00435F0C, 0);
    func_00111B60(D_00435F0C, 0x800);
}

void func_00133A28(void) {
    func_00112B58(D_00435F0C, 0x80);
    func_00111B30(D_00435F0C, 0x800);
}

void func_00133A50(void) {
    u8 temp_v0;

    temp_v0 = D_00435F10 != 0;
    *(u32 *)(*(s32 *)(D_00435F14 + 0x18) + 0x1c) = 0;
    if (temp_v0) {
        *(u32 *)(*(s32 *)(D_00435F18 + 0x18) + 0x1c) = 0;
    }
}

void func_00133A78(void) {
    u8 temp_v0;

    temp_v0 = D_00435F10 != 0;
    *(u32 *)(*(s32 *)(D_00435F14 + 0x18) + 0x1c) = 0x80808080;
    if (temp_v0) {
        *(u32 *)(*(s32 *)(D_00435F18 + 0x18) + 0x1c) = 0x80808080;
    }
}

void func_00133AA8(void) {
    if (D_00435F10 != 0) {
        func_00111B30(D_00435F0C, 0x400);
        return;
    }
    func_00111B30(D_00435F0C, 0x200);
}

void func_00133AE8(void) {
    func_00111B60(D_00435F0C, 0x400);
    func_00111B60(D_00435F0C, 0x200);
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133B10);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133C18);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133C90);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133CF8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133D40);

void func_00133DB8(void) {
    D_00389904[0] = 0;
}

void func_00133DC8(void) {
    D_00389910[0] = 0;
}

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

void func_00135588(u32 arg0) {
    D_003899C0[0] = arg0;
}

u32 func_00135598(void) {
    return D_003899C0[0];
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001355A8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001355D8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00135840);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00135A68);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00135D18);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00135D80);

void func_00136098(void) {
    func_00111B60(D_00435F0C, 0x100);
}

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FA0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FA4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FA8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FAC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FB0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FB4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FB8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FBC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FC0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FC4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FC8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FCC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FD0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FD8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FDC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FE0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FE4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FE8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FEC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FF0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FF4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FF8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FFC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436000);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436004);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436008);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_0043600C);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436010);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436014);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436018);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436020);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436028);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436030);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436038);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436040);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436048);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436050);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436060);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436064);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436068);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436070);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436078);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_0043607C);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436080);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436088);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_0043608C);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436090);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436098);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360A0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360A4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360A8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360AC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360B0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360B4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360B8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360BC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360C0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360C4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360C8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360CC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360D0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360D4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360D8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360DC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360E0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360E4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360E8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360EC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360F0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360F4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360F8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360FC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436100);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436104);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436108);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_0043610C);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436110);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436114);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436118);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_0043611C);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436120);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436124);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436128);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_0043612C);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436130);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436134);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436138);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_0043613C);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436140);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436144);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436148);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_0043614C);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436150);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436154);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436158);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_0043615C);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436160);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436164);

