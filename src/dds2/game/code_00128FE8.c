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

extern u32 D_00436178;

extern u32 D_00436180;

extern u32 D_0043618C;

extern u32 D_00436190;

extern s32 D_004361F8;

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

extern f32 func_00340898(f32 arg0, f32 arg1);

extern u32 D_003899C0[];

extern u32 D_0038BBD8[];

extern u32 D_0038BB50[];

extern u32 D_0038BB60[];

extern void *memset(void *s, s32 c, u32 n);

extern void *func_003292A8(s32 size);

extern void *func_003298F8(void *p);

extern u32 D_0038BD50[];

extern s32 func_0010C100(u32 arg0);

extern s32 func_00110FB0(u64 arg0, u32 arg1);

extern s32 func_001018B0(u32 arg0);

extern void kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);

extern s32 D_004361A4;

extern u32 *D_0038BC50[];

extern void func_0012D9D0(u32 value);

extern s32 D_004361CC;

extern s16 D_00444C68[];

extern u32 D_004361F4;

extern s16 D_003932B2[];

typedef struct {
    s16 unk0;
    s16 unk2;
} FldIndexPair;

extern FldIndexPair D_0038A3B8[];

extern FldIndexPair D_0038A480[];

extern s32 D_004361E4;

extern s32 D_004361EC;

extern s32 D_004361F0;

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00128FE8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001295E0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00129660);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00129940);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00129B40);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00129CC0);

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

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012B4A8);

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

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001360B8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00136368);

void func_00136388(void) {
    u32 *temp_v0 = D_0038BBD8;

    memset(temp_v0, 0, 0x14);
    temp_v0[0] = (u32)D_0038BB50;
    temp_v0[1] = (u32)D_0038BB60;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001363D8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00136718);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00136850);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00136A70);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00136C90);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00136E60);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00136EF8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00137818);

void func_00137888(void) {
    func_00136EF8();
}

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

void func_00137B60(void) {
    u8 *temp_v0 = func_003292A8(0x72000);

    D_00436190 = (u32)temp_v0;
    temp_v0 = func_003298F8(temp_v0);
    D_00436180 = (u32)temp_v0;
    memset(temp_v0, 0, 0x72000);
    temp_v0 = func_003292A8(0x4A00);
    D_0043618C = (u32)temp_v0;
    temp_v0 = func_003298F8(temp_v0);
    D_00436178 = (u32)temp_v0;
    memset(temp_v0, 0, 0x4A00);
}

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

s32 func_0013DC70(u32 flag, u32 slot) {
    D_0038BD50[slot] = 0;
    if (func_0010C100(flag) != 0) {
        func_00110FB0(func_0010FFA8(), flag);
        return 0;
    }
    return -1;
}

u32 func_0013DCC8(u32 arg0) {
    u32 *temp_v0 = &D_0038BD50[arg0];

    if (func_001018B0(*temp_v0) != 0) {
        kwlnTaskDestroyWithHierarchy(*temp_v0, 0);
    }
    *temp_v0 = 0;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013DD18);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013DDC0);

void func_0013E958(void) {
    s32 count = D_004361A4;
    s32 i = 0;
    if (count > 0) {
        u32 **entry = D_0038BC50;
        do {
            func_0012D9D0((*entry)[4]);
            i++;
            entry++;
        } while (i < D_004361A4);
    }
}

void func_0013E9B8(void) {
    s32 count = D_004361A4;
    s32 i = 0;
    if (count > 0) {
        u32 *entry = D_0038BD50;
        do {
            if (func_001018B0(*entry) == 0) {
                *entry = 0;
            }
            i++;
            entry++;
        } while (i < D_004361A4);
    }
}

s32 func_0013EA18(u32 task) {
    s32 i;
    for (i = 0; i < D_004361A4; i++) {
        if (D_0038BD50[i] == task) {
            return D_0038BC50[i][0];
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013EA78);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013EAD0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013EB30);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013ED20);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013EEA0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013F108);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0013F168);

s32 func_0013F1B8(void) {
    s32 temp_v0 = D_004361CC;

    if (temp_v0 < 0) {
        return -1;
    }
    return D_00444C68[temp_v0 * 160];
}

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

s32 func_00140750(void) {
    return D_003932B2[D_004361F4 * 54];
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00140780);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00140830);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00140A58);

u8 func_00140B80(void) {
    return D_004361F8 == 8;
}

void func_00140B90(s8 *arg0) {
    if (arg0[0x53] != 0) {
        D_00389770[0x22] = arg0[0x53] - 1;
    }
    D_00389770[0x16] = arg0[0x45];
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00140BC8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001411F8);

void func_00141840(s32 mode, s32 index) {
    switch (mode) {
    case 0:
        D_004361EC = 3;
        D_004361E4 = D_0038A3B8[index].unk0;
        D_004361F0 = index;
        break;
    case 1:
        D_004361F0 = index;
        D_004361E4 = D_0038A480[index].unk0;
        break;
    }
}

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
