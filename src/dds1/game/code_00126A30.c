#include "common.h"

extern s32 D_003BAE68;

extern u32 D_003BADE8;
extern u32 D_003BADF0;
extern u32 D_003BADFC;
extern u32 D_003BAE00;

extern u32 D_003BAD78;

extern u32 D_003BAD7C;
extern u32 D_003BAD80;
extern u32 D_003BAD84;

extern s32 D_003BAD68;
extern u32 D_003BAD6C;
extern s32 D_003BAD70;
extern s32 D_003BD7C4;

extern s32 D_003BAB38;

extern u32 D_003BAB34;

extern u32 D_003BAD34;
extern u32 D_003BAD38;
extern u32 D_003BAD3C;
extern u32 D_003BAD40;

extern u32 D_003BAD1C;

extern u64 func_0010FD80(void);
extern s64 func_001109F0(u64);
extern s64 func_00123DE0(void);

extern s32 D_003BAA34;

extern u32 D_003BACF8;

extern u32 func_0011D3B0(void);
extern u64 func_002D3FD0(u64);

extern u64 func_00197760(s32, s32, u64, u64, u64, u64);

extern s32 D_003BACD8;
extern s32 func_002CF530(u32);

extern u32 D_003BACD0;

extern u32 D_003BAC30;

extern u32 D_003BAC84;
extern u32 D_003BAC88;
extern u32 D_003BAD98;
extern u32 D_003BAD9C;
extern u32 D_003BADA0;
extern u32 D_003BADA4;
extern u32 D_003BADC8;
extern u32 D_003BADD8;
extern u32 D_003BAE28;
extern s32 D_003BAE3C;
extern u32 D_003BAE64;
extern u32 D_0032E428[];
extern u32 D_0032E3B0[];
extern u32 D_0032E538[];
extern u32 D_0032E544[];
extern u32 D_0032E570[];
extern u32 D_0032E59C[];
extern u32 D_0032E5A8[];
extern u32 D_00324B48[];
extern u8 D_003296F0[];
extern u8 D_00324610[];
extern u8 D_00324660[];
extern void func_002DDD60(void *src);
extern char D_003C9200[];
extern u32 D_003C92E0[];
extern s16 D_00337D12[];
extern s16 D_003C9518[];
extern void func_00127E20(char *arg0);
extern void func_00132FD0(u32 arg0, s32 arg1);
extern s32 strcmp(const char *a, const char *b);
extern u32 D_003BAD20;
extern char D_003BAD08[];
extern u8 D_003C9230[];
extern u8 D_003C9220[];
extern void encProc(void);
extern void func_0012C7C0(void);
extern void func_00213A38(void);
extern void func_00112EE8(s32 arg0, void *arg1, void *arg2);
extern f32 func_002E79F0(f32 arg0, f32 arg1);
extern s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
extern u32 D_003BAC3C;
extern s32 D_003BAD00;
extern u32 D_003BACFC;
extern u32 D_00330738[];
extern u32 D_003306B0[];
extern u32 D_003306C0[];
extern u32 D_003308B0[];
extern u32 func_00288BE8(u32 arg0);
extern void *memset(void *s, s32 c, u32 n);
extern void *func_002CFEB8(s32 size);
extern void func_00101A68(u32 arg0, void *arg1);
extern void func_00140F70(void);
extern s32 func_001019C8(u32 arg0);
extern s32 func_00213B50(void);
extern void kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);
extern s32 D_003BADF4;
extern u8 D_003BA734;
extern s32 func_0010BED8(u32 arg0);
extern s32 func_00110D88(u64 arg0, u32 arg1);
extern u32 D_003BAC48;
extern u32 D_003BAC34;
extern u8 D_003BAC90[];
extern void func_002D0A10(u32 arg0);
extern void func_00288788(u32 arg0);
extern void func_00218320(s32 arg0);
extern void func_00218368(s32 arg0);
extern s32 D_003BAE30;
extern s32 D_003BAE1C;
extern s32 D_003BAE14;
extern s16 D_003C9510[];
extern void *func_002D03F8(s32 size);
extern void *func_002D0A48(void *p);
extern u32 D_003BAE4C;
extern s32 D_003BAE50;
extern s16 D_00337C60[];
extern u32 *D_003307B0[];
extern void func_0012B4A0(u32 value);

typedef struct {
    s16 unk0;
    s16 unk2;
} FldIndexPair;
extern FldIndexPair D_0032EF18[];
extern FldIndexPair D_0032EFE0[];
extern s32 D_003BAE54;
extern s32 D_003BAE5C;
extern s32 D_003BAE60;

INCLUDE_ASM(const s32, "game/code_00126A30", func_00126A30);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00127028);

INCLUDE_ASM(const s32, "game/code_00126A30", func_001270A8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00127388);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00127588);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00127708);

void func_00127788(u32 *arg0) {
    func_00126A30(arg0[1], *arg0, 1);
}

s32 func_001277A8(s32 arg0) {
    return arg0 + 0xc;
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_001277B0);

void func_00127898(u32 arg0, s32 arg1) {
    func_002EB278(arg0, arg0, (s32)arg0 + *(s32 *)(arg1 + 4), *(u32 *)(arg1 + 8));
}

void func_001278C0(u32 arg0, s32 arg1) {
    func_001277B0(arg0, arg0, (s32)arg0 + *(s32 *)(arg1 + 4), *(u32 *)(arg1 + 8));
}

void func_001278E8(u32 arg0, u32 arg1) {
    D_003BAC84 = arg0;
    D_003BAC88 = arg1;
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_001278F8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_001279A0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00127AC0);

u32 func_00127B30(void) {
    u32 temp_v1 = D_0032E3B0[0x1E];

    if (temp_v1 != 0) {
        if (temp_v1 == 1) {
            if (func_00288BE8(D_003BAC3C) != 0) {
                D_0032E3B0[0x1E] = 0;
                D_003BA734 = 0;
            }
        }
    }
    return 0;
}

u32 func_00127B88(void) {
    return D_0032E428[0];
}

u8 func_00127B98(void) {
    if (D_003BAC3C != 0) {
        if (func_00288BE8(D_003BAC3C) != 0) {
            return 1;
        }
    }
    return D_0032E428[0] != 0;
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00127BD8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00127C40);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00127CB8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00127D30);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00127DA8);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_0039FE38);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00127E20);

u8 func_00127FD0(void) {
    char buf[32];

    func_00127E20(buf);
    return strcmp(D_003C9200, buf) != 0;
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00128000);

void func_00128188(void) {
    u32 temp_v0 = D_003BAC48;

    if (temp_v0 != 0) {
        func_002D0A10(temp_v0);
        D_003BAC48 = 0;
    }
    temp_v0 = D_003BAC34;
    if (temp_v0 != 0) {
        func_00288788(temp_v0);
        D_003BAC34 = 0;
    }
    D_003C9200[0] = D_003BAC90[0];
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_001281E0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00128780);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00128890);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00128B50);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00128BB8);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_0039FF88);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00128DA0);

void func_00128F88(u32 arg0) {
    D_003BAC30 = arg0;
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00128F90);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00128FE0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00129000);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00129178);

INCLUDE_ASM(const s32, "game/code_00126A30", func_001292E0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_001294A0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00129578);

void func_00129650(void) {
    u8 *matrix;
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf28, 0(%0)\n"
        "lqc2 vf29, 0x10(%0)\n"
        "lqc2 vf30, 0x20(%0)\n"
        "lqc2 vf31, 0x30(%0)\n"
        ".set reorder"
        : : "r"(D_003296F0) : "memory");
    matrix = D_00324610;
    func_002DDD60(matrix);
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
        : : "r"(D_00324660) : "memory");
}

void func_001296B8(f32 *dstX, f32 *dstY, f32 x, f32 y, f32 z) {
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

void func_00129720(u32 arg0) {
    D_003BACD0 = arg0;
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00129728);

INCLUDE_ASM(const s32, "game/code_00126A30", func_001297E8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00129900);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00129A08);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00129B68);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00129CA8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00129E30);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00129FC0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012A120);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012A358);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012A5D8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012A890);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012AA20);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012AB40);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012ABE0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012AC80);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012AD30);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012AEB0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012B090);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012B2B0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012B4A0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012B570);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012B638);

void func_0012B740(void) {
    if (D_003BACD8 == 0) {
        D_003BACD8 = func_002CF530(0x70000);
    }
}

void func_0012B768(void) {
    if (D_003BACD8 != 0) {
        func_002CF5C0(D_003BACD8);
        D_003BACD8 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012B798);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012B818);

void func_0012B890(s32 arg0, s32 arg1, u64 arg2, u64 arg3) {
    u64 temp_v0;

    temp_v0 = func_00197760(arg0 << 4, arg1 << 4, 0, arg2, arg3, 0);
    func_00195868(temp_v0);
    func_00194920(temp_v0);
}

void func_0012B8E0(s32 arg0) {
    *(s32 *)(arg0 + 0x20) = *(s32 *)(arg0 + 0x20) + 0x60;
}

void func_0012B8F0(s32 arg0) {
    u64 temp_v0;
    u32 temp_v1;

    temp_v1 = func_0011D3B0();
    *(u32 *)(arg0 + 0x28) = temp_v1;
    temp_v0 = func_002D3FD0(0x40);
    func_002D5608(temp_v0);
    func_002D4038(*(u32 *)(arg0 + 0x28), temp_v0);
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012B940);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012BAD8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012BBC8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012BCC8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012BDC8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012BEC8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012BFC0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012C0C8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012C1F0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012C428);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012C648);

void func_0012C688(u32 arg0) {
    D_003BACF8 = arg0;
}

INCLUDE_ASM(const s32, "game/code_00126A30", encProc);

void func_0012C6C8(u32 arg0, s32 arg1) {
    if ((arg1 < 0x400) && ((*(u16 *)((s32)arg1 * 0x28 + D_003BAA34 + 0x20) & 0x8000) != 0))
    {
        func_001060C8(0);
        func_0012C750(3);
        return;
    }
    func_002E8F78(0xf, 0x7f, 0x3f);
    func_0012C750(arg0);
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012C750);

void func_0012C7C0(void) {
    func_00213A90();
}

void func_0012C7D8(void) {
    kwlnTaskCreate((s32)D_003BAD08, 0x2B0F, 0, 1, (s32)encProc, (s32)func_0012C7C0, 0);
    func_00213A38();
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012C818);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012C880);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012CB48);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012CED0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012D240);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012D3D8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012D528);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012DB70);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012DD70);

s64 func_0012E488(void) {
    u64 temp_v0;
    s64 temp_v1;
    s64 temp_v2;

    temp_v0 = func_0010FD80();
    temp_v1 = func_001109F0(temp_v0);
    temp_v2 = func_00123DE0();
    if (temp_v2 == temp_v1) {
        temp_v1 = 0;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012E4C8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012E510);

void func_0012E6F0(void) {
    D_003BAD1C = 0;
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012E6F8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012E878);

void func_0012E9D0(void) {
    D_003BAD38 = 0;
    D_003BAD34 = 0xffffffff;
    D_003BAD3C = 0;
    D_003BAD40 = 0;
}

void func_0012E9E8(u32 arg0) {
    if (arg0 == 0) {
        D_003BAD40 = 0;
        if (D_003BAB38 != 0) {
            func_00218368(D_003BAB38);
        }
    } else {
        D_003BAD40 = arg0;
        func_00218320(D_003BAB38);
    }
}

void func_0012EA40(u32 arg0, u32 arg1) {
    D_003BAD34 = arg0;
    D_003BAD38 = arg1;
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012EA50);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012EEA0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012F4B8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012F578);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012FC20);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012FD00);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012FE30);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012FF48);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00131098);

u8 func_001311A0(void) {
    if (D_0032E3B0[0x5F] == 0) {
        if (D_0032E3B0[0x61] == 0) {
            if (D_0032E3B0[0x60] == 0) {
                return 0;
            }
        }
    }
    return 1;
}

void func_001311D8(void) {
    func_00111908(D_003BAB34, 1);
    func_00131458();
}

void func_001311F8(void) {
    func_00111938(D_003BAB34, 1);
}

void func_00131218(void) {
    func_00112930(D_003BAB34, 0);
    func_00111938(D_003BAB34, 0x800);
}

void func_00131240(void) {
    func_00112930(D_003BAB34, 0x80);
    func_00111908(D_003BAB34, 0x800);
}

void func_00131268(void) {
    *(u32 *)(*(s32 *)(D_003BAB38 + 0x18) + 0x1c) = 0;
}

void func_00131278(void) {
    *(u32 *)(*(s32 *)(D_003BAB38 + 0x18) + 0x1c) = 0x80808080;
}

void func_00131290(void) {
    func_00111908(D_003BAB34, 0x200);
}

void func_001312B0(void) {
    func_00111938(D_003BAB34, 0x400);
    func_00111938(D_003BAB34, 0x200);
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_001312D8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_001313E0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00131458);

INCLUDE_ASM(const s32, "game/code_00126A30", func_001314C0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00131508);

void func_00131580(void) {
    D_0032E538[0] = 0;
}

void func_00131590(void) {
    D_0032E544[0] = 0;
}

void func_001315A0(f32 arg0, f32 arg1) {
    u8 *temp_v0 = (u8 *)D_0032E3B0;

    *(f32 *)(temp_v0 + 0x18C) = arg0;
    *(f32 *)(temp_v0 + 0x190) = arg1;
    *(u32 *)(temp_v0 + 0x188) = 1;
}

void func_001315C0(f32 arg0, f32 arg1, f32 arg2) {
    u8 *temp_v0;
    f32 temp_f0;

    temp_f0 = func_002E79F0(arg0, arg2);
    temp_v0 = (u8 *)D_0032E3B0;
    temp_f0 *= 180.0f / 3.14f;
    *(u32 *)(temp_v0 + 0x194) = 1;
    *(f32 *)(temp_v0 + 0x198) = -temp_f0;
}

void func_00131600(void) {
    u8 *temp_v0 = (u8 *)D_0032E3B0;

    if (*(u32 *)(temp_v0 + 0x188) != 0) {
        f32 temp_f12 = *(f32 *)(temp_v0 + 0x140) - *(f32 *)(temp_v0 + 0x18C);
        f32 temp_f13 = *(f32 *)(temp_v0 + 0x148) - *(f32 *)(temp_v0 + 0x190);
        f32 temp_f0;

        *(u32 *)(temp_v0 + 0x188) = 2;
        temp_f0 = func_002E79F0(temp_f12, temp_f13);
        temp_f0 *= 180.0f / 3.14f;
        *(f32 *)(temp_v0 + 0x168) = -temp_f0;
    }
}

void func_00131660(void) {
    u8 *temp_v0 = (u8 *)D_0032E3B0;

    if (*(u32 *)(temp_v0 + 0x194) != 0) {
        *(u32 *)(temp_v0 + 0x194) = 2;
        *(f32 *)(temp_v0 + 0x168) = *(f32 *)(temp_v0 + 0x198);
    }
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00131688);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0048);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0058);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0068);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A00A8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00131A88);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00131CC0);

void func_00131D88(void) {
    if (D_003BAD70 != 0) {
        func_002D2D00(D_003BAD70);
        D_003BAD70 = 0;
    }
    if (D_003BD7C4 != 0) {
        func_002D0A10(D_003BD7C4);
        D_003BD7C4 = 0;
    }
    if (D_003BAD68 != 0) {
        func_0029CE80(D_003BAD68);
        D_003BAD68 = 0;
    }
    D_003BAD6C = 0;
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00131DE8);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0100);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00131F08);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00132010);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00132AE8);

void func_00132B60(u32 arg0) {
    D_003BAD7C = arg0;
    D_003BAD80 = 0;
    D_003BAD84 = 0;
}

void func_00132B70(u32 arg0) {
    D_003BAD78 = arg0;
}

u32 func_00132B78(void) {
    return D_003BAD78;
}

void func_00132B80(u32 arg0) {
    D_0032E5A8[0] = arg0;
}

u32 func_00132B90(void) {
    return D_0032E5A8[0];
}

void func_00132BA0(u32 arg0) {
    u32 temp_v0 = D_0032E59C[0];

    D_003BAD9C = arg0;
    D_003BADA0 = 0;
    D_003BADA4 = temp_v0;
    func_00132FD0(temp_v0, 1);
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00132BD0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00132E38);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00132FD0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00133280);

INCLUDE_ASM(const s32, "game/code_00126A30", func_001332E8);

void func_00133600(s32 arg0) {
    if (arg0 == 0) {
        func_00111938(D_003BAB34, 0x100);
        return;
    }
    func_00111908(D_003BAB34, 0x100);
    func_00113AA8(D_003BAB34);
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00133640);

INCLUDE_ASM(const s32, "game/code_00126A30", func_001338F0);

void func_00133910(void) {
    u32 *temp_v0 = D_00330738;

    memset(temp_v0, 0, 0x14);
    temp_v0[0] = (u32)D_003306B0;
    temp_v0[1] = (u32)D_003306C0;
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00133960);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00133CA0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00133EC0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_001340E0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_001342B0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00134348);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00134C68);

void func_00134CD8(void) {
    func_00134348();
}

void func_00134CF0(void) {
    D_003BAD6C = 0;
    if (D_003BAD70 != 0) {
        func_002D2D00(D_003BAD70);
        D_003BAD70 = 0;
    }
    if (D_003BAD68 != 0) {
        func_0029CE80(D_003BAD68);
        D_003BAD68 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00134D38);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00134E10);

void func_00134FB0(void) {
    u8 *temp_v0 = func_002D03F8(0x72000);

    D_003BAE00 = (u32)temp_v0;
    temp_v0 = func_002D0A48(temp_v0);
    D_003BADF0 = (u32)temp_v0;
    memset(temp_v0, 0, 0x72000);
    temp_v0 = func_002D03F8(0x4A00);
    D_003BADFC = (u32)temp_v0;
    temp_v0 = func_002D0A48(temp_v0);
    D_003BADE8 = (u32)temp_v0;
    memset(temp_v0, 0, 0x4A00);
}

void func_00135018(void) {
    func_002D0A60(D_003BAE00);
    func_002D0A10(D_003BAE00);
    D_003BADF0 = 0;
    func_002D0A60(D_003BADFC);
    func_002D0A10(D_003BADFC);
    D_003BADE8 = 0;
}

float func_00135058(float *arg0, float *arg1) {
    return *arg0 * *arg1 + arg0[1] * arg1[1] + arg0[2] * arg1[2];
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00135088);

INCLUDE_ASM(const s32, "game/code_00126A30", func_001350B8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00135250);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00135298);

INCLUDE_ASM(const s32, "game/code_00126A30", func_001352E0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00135360);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00136850);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00136A78);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00136DA0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00136FE8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_001372D0);

void func_00137E90(void) {
}

void func_00137E98(void) {
}

void func_00137EA0(void) {
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00137EA8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00138058);

INCLUDE_ASM(const s32, "game/code_00126A30", func_001384E8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00138910);

void func_00138C28(void) {
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00138C30);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00138D88);
INCLUDE_ASM(const s32, "game/code_00126A30", func_00138EB0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00138ED0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013A720);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013A9B0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013AC10);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013AE28);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013AF20);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013AFC8);

s32 func_0013B088(u32 flag, u32 slot) {
    D_003308B0[slot] = 0;
    if (func_0010BED8(flag) != 0) {
        func_00110D88(func_0010FD80(), flag);
        return 0;
    }
    return -1;
}

u32 func_0013B0E0(u32 arg0) {
    u32 *temp_v0 = &D_003308B0[arg0];

    if (func_001019C8(*temp_v0) != 0) {
        kwlnTaskDestroyWithHierarchy(*temp_v0, 0);
    }
    *temp_v0 = 0;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013B130);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013B1D8);

void func_0013BD70(void) {
    s32 count = D_003BAE14;
    s32 i = 0;
    if (count > 0) {
        u32 **entry = D_003307B0;
        do {
            func_0012B4A0((*entry)[4]);
            i++;
            entry++;
        } while (i < D_003BAE14);
    }
}

void func_0013BDD0(void) {
    s32 count = D_003BAE14;
    s32 i = 0;
    if (count > 0) {
        u32 *entry = D_003308B0;
        do {
            if (func_001019C8(*entry) == 0) {
                *entry = 0;
            }
            i++;
            entry++;
        } while (i < D_003BAE14);
    }
}

s32 func_0013BE30(u32 task) {
    s32 i;
    for (i = 0; i < D_003BAE14; i++) {
        if (D_003308B0[i] == task) {
            return D_003307B0[i][0];
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013BE90);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013BEE8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013BF48);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013C138);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013C2B8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013C520);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013C580);

s32 func_0013C5D0(void) {
    s32 temp_v0 = D_003BAE3C;

    if (temp_v0 < 0) {
        return -1;
    }
    return D_003C9518[temp_v0 * 160];
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013C600);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013C7F8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013C9E0);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0150);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013CBA8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013CEB0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013D410);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013D510);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013D598);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013D650);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0200);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013D6D0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013D8D8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013DAC0);

s32 func_0013DB28(void) {
    return D_00337D12[D_003BAE64 * 54];
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013DB58);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013DC08);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013DDF0);

u8 func_0013DF18(void) {
    return D_003BAE68 == 8;
}

void func_0013DF28(s8 *arg0) {
    if (arg0[0x53] != 0) {
        D_0032E3B0[0x22] = arg0[0x53] - 1;
    }
    D_0032E3B0[0x16] = arg0[0x45];
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013DF60);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013E5A8);

void func_0013EC10(s32 mode, s32 index) {
    switch (mode) {
    case 0:
        D_003BAE5C = 3;
        D_003BAE54 = D_0032EF18[index].unk0;
        D_003BAE60 = index;
        break;
    case 1:
        D_003BAE60 = index;
        D_003BAE54 = D_0032EFE0[index].unk0;
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013EC68);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013EF10);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013F100);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013F340);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013F4D8);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0270);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0280);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013F5A8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013F6B8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013F848);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013F9E8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013FA40);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013FCE0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013FD60);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013FE08);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013FEC0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00140AB8);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0380);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00140BE8);

void func_00140F68(void) {
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00140F70);

void * func_00141098(u32 arg0) {
    u16 *temp_v0 = func_002CFEB8(8);

    temp_v0[1] = 1;
    temp_v0[0] = 0;
    temp_v0[2] = 0;
    temp_v0[3] = 0;
    func_00101A68(arg0, temp_v0);
    return (void *)func_00140F70;
}



INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC10);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC14);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC18);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC1C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC20);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC24);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC28);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC2C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC30);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC34);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC38);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC3C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC40);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC48);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC4C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC50);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC54);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC58);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC5C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC60);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC64);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC68);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC6C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC70);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC74);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC78);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC7C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC80);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC84);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC88);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC90);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC98);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACA0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACA8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACB0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACB8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACC0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACD0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACD4);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACD8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACE0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACE8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACEC);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACF0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACF8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACFC);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD00);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD08);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD10);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD14);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD18);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD1C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD20);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD24);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD28);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD2C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD30);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD34);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD38);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD3C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD40);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD44);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD48);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD4C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD50);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD54);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD58);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD5C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD60);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD64);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD68);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD6C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD70);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD74);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD78);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD7C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD80);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD84);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD88);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD8C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD90);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD94);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD98);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD9C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADA0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADA4);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADA8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADAC);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADB0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADB4);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADB8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADBC);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADC0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADC4);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADC8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADCC);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADD0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADD4);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADD8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADDC);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADE0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADE4);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADE8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADEC);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADF0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADF4);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADF8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADFC);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE00);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE04);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE08);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE0C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE10);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE14);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE18);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE1C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE20);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE24);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE28);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE2C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE30);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE34);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE38);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE3C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE40);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE44);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE48);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE4C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE50);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE54);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE58);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE5C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE60);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE64);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE68);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE6C);


INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE70);

