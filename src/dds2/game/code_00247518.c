#include "common.h"

extern s32 func_0024A010(void);

INCLUDE_ASM(const s32, "game/code_00247518", func_00247518);

INCLUDE_ASM(const s32, "game/code_00247518", func_002475C8);

INCLUDE_ASM(const s32, "game/code_00247518", func_002476B8);

INCLUDE_ASM(const s32, "game/code_00247518", func_002477F0);

INCLUDE_ASM(const s32, "game/code_00247518", func_00247858);

INCLUDE_ASM(const s32, "game/code_00247518", func_00247DE0);

INCLUDE_ASM(const s32, "game/code_00247518", func_00247EE0);

INCLUDE_ASM(const s32, "game/code_00247518", func_00248000);

INCLUDE_ASM(const s32, "game/code_00247518", func_00248B80);

INCLUDE_ASM(const s32, "game/code_00247518", func_00248D70);

INCLUDE_ASM(const s32, "game/code_00247518", func_00249088);

INCLUDE_ASM(const s32, "game/code_00247518", func_00249518);

INCLUDE_ASM(const s32, "game/code_00247518", func_00249598);

INCLUDE_ASM(const s32, "game/code_00247518", func_002496B0);

INCLUDE_ASM(const s32, "game/code_00247518", func_00249A98);

INCLUDE_ASM(const s32, "game/code_00247518", func_00249B40);

INCLUDE_ASM(const s32, "game/code_00247518", func_00249C40);

INCLUDE_ASM(const s32, "game/code_00247518", func_00249DC8);

void func_00249EB0(s32 arg0) {
    s64 temp_v0;

    temp_v0 = func_0024A010();
    if (temp_v0 != 0) {
        *(s32 *)(arg0 + 0x23c0) = *(s32 *)(arg0 + 0x23c0) + 1;
    }
}

INCLUDE_ASM(const s32, "game/code_00247518", func_00249EE8);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024A010);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024A020);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024A158);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024A380);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024A400);

void func_0024A5F8(void) {
}

void func_0024A600(s32 arg0) {
    s32 temp_v0;

    if ((*(s32 *)(arg0 + 0x18) < *(s32 *)(arg0 + 0x14) - 3) && (0 < *(s32 *)(arg0 + 0x23f0)))
    {
        func_0019D518(*(u32 *)(arg0 + 0x2410));
        temp_v0 = *(s32 *)(arg0 + 0x23f0) + 1;
        *(s32 *)(arg0 + 0x23f0) = temp_v0;
        if (0x1d < temp_v0) {
            *(u32 *)(arg0 + 0x23f0) = 0;
        }
    }
}

void func_0024A668(void) {
}

INCLUDE_ASM(const s32, "game/code_00247518", func_0024A670);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024A738);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024A9F0);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024AA38);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024AAB8);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024AB38);

u16 func_0024ABA0(s32 arg0) {
    u16 temp_v0;
    s32 temp_v1;

    temp_v1 = *(s32 *)(arg0 + 0x227c) - 1;
    if (*(s32 *)(arg0 + 0x227c) == 0) {
        *(u32 *)(arg0 + 0x2280) = 0;
        return 0;
    }
    *(s32 *)(arg0 + 0x227c) = temp_v1;
    temp_v0 = *(u16 *)(temp_v1 * 8 + arg0 + 0x223c);
    *(u32 *)(arg0 + 0x2280) = (u32)temp_v0;
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_00247518", func_0024ABD0);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024ACC0);

void func_0024AD30(s32 arg0) {
    *(u8 *)(arg0 + 0x23c5) = 1;
}

void func_0024AD40(s32 arg0) {
    *(u8 *)(arg0 + 0x23c5) = 0;
}

INCLUDE_ASM(const s32, "game/code_00247518", func_0024AD48);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024B040);

u32 func_0024B078(void) {
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00247518", D_004227A0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004227B0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004227C0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004227D0);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024B080);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024B268);

u32 func_0024B678(u32 arg0, u32 arg1, u32 arg2) {
    func_0024AB38(5, 0x90, 0x48, arg2);
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00247518", D_004229A0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004229B0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004229C0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004229D0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004229E0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004229F0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A00);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A10);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A20);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A30);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A40);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A50);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A60);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A70);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A80);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422A90);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422AA0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422AB0);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00422AC0);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024B6A8);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024C540);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024C650);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024C8C0);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024C928);

u32 func_0024C9D0(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00247518", func_0024C9D8);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024CA28);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024CB20);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024CBA0);

u32 func_0024CC08(u32 arg0, u32 arg1, u32 arg2) {
    func_0024ABA0(arg2);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00247518", func_0024CC28);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024CD00);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024CD58);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024CE18);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024D0F8);

INCLUDE_RODATA(const s32, "game/code_00247518", D_00423050);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024D148);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024D408);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024D430);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024D6B0);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024D710);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024D760);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024D788);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024D878);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024D908);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024D918);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024DAA0);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024DAC0);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024DAE0);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024DAF8);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024DB98);

INCLUDE_ASM(const s32, "game/code_00247518", func_0024DBB8);

INCLUDE_RODATA(const s32, "game/code_00247518", D_004230D0);
