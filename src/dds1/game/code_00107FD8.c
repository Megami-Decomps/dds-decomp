#include "common.h"

extern s32 D_003BA960;

extern s32 func_002C2ED0(void);

extern s32 func_0024A0A0(void);

extern s32 func_00244AF8(void);

extern u32 D_003BA958;

extern s32 func_002913B8(void);

extern s32 func_00262938(void);

extern s32 func_00272228(void);

extern u32 D_003BA730;

extern s32 func_00125FD0(void);

extern s32 func_0028F600(void);

extern s32 func_0028F5F8(void);

extern u64 func_00197748(s32, s32, u64, u64, u64, u64);

extern u32 D_003BA8E8;

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00107FD8);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_001080D8);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00108218);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_001082D8);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00108360);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_001083F8);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_001089F8);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00108A48);

void func_00108A80(u32 arg0) {
    D_003BA8E8 = arg0;
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00108A88);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00108BA0);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00108CB8);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00108DC0);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00108E60);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00108F00);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00108FA0);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00109108);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_001093B8);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_001093F8);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00109640);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00109810);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00109A40);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00109AF0);

void func_00109BC0(s32 arg0, s32 arg1, u64 arg2, u64 arg3) {
    u64 temp_v0;

    temp_v0 = func_00197748(arg0 << 4, arg1 << 3, 0, arg2, arg3, 0);
    func_00195868(temp_v0);
    func_00194920(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00109C10);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00109CC0);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00109D20);

void func_0010A158(void) {
    func_0010A170();
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010A170);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010A1B0);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010A1E8);

u32 func_0010A210(void) {
    return 0;
}

void func_0010A218(void) {
    func_0028F0E0(3);
}

u32 func_0010A230(void) {
    s64 temp_v0;
    u32 temp_v1;

    func_0028F458();
    func_0028F460();
    temp_v0 = func_0028F5F8();
    temp_v1 = 0xffffffff;
    if (temp_v0 != 0) {
        temp_v1 = 0;
    }
    return temp_v1;
}

u8 func_0010A268(void) {
    s64 temp_v0;

    temp_v0 = func_0028F600();
    return temp_v0 == 0;
}

void func_0010A288(u32 arg0, s32 arg1) {
    if (arg1 == 0) {
        func_0026BCE8(0);
        return;
    }
    func_0026BCE8(*(u32 *)arg1);
}

u32 func_0010A2B8(void) {
    func_0026BD08();
    return 0;
}

u32 func_0010A2D8(void) {
    return 0;
}

void func_0010A2E0(void) {
    func_0028F0E0(1);
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010A2F8);

u8 func_0010A338(void) {
    s64 temp_v0;

    temp_v0 = func_0028F600();
    return temp_v0 == 0;
}

void func_0010A358(void) {
    func_0026FF18();
}

u32 func_0010A370(void) {
    func_0026FF38();
    return 0;
}

u32 func_0010A390(void) {
    return 0;
}

void func_0010A398(void) {
    func_0028F0E0(2);
}

u32 func_0010A3B0(void) {
    s64 temp_v0;
    u32 temp_v1;

    func_0028F458();
    func_0028F460();
    temp_v0 = func_0028F5F8();
    temp_v1 = 0xffffffff;
    if (temp_v0 != 0) {
        temp_v1 = 0;
    }
    return temp_v1;
}

u8 func_0010A3E8(void) {
    s64 temp_v0;

    temp_v0 = func_0028F600();
    return temp_v0 == 0;
}

void func_0010A408(u32 arg0, u32 arg1) {
    func_001060C8(0);
    func_00125E08(arg1, arg0);
}

u32 func_0010A448(void) {
    s64 temp_v0;
    u32 temp_v1;

    func_00123D98();
    temp_v0 = func_00125FD0();
    temp_v1 = 0xffffffff;
    if (temp_v0 != 0) {
        temp_v1 = 0;
    }
    return temp_v1;
}

u32 func_0010A478(void) {
    return 0;
}

void func_0010A480(void) {
    func_001060C8(0);
}

u32 func_0010A498(void) {
    return 0;
}

u32 func_0010A4A0(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010A4A8);

u32 func_0010A510(void) {
    D_003BA730 = 1;
    func_00235498();
    func_001260A8();
    return 0;
}

u32 func_0010A540(void) {
    return 0;
}

void func_0010A548(void) {
    func_002720B0();
}

u32 func_0010A560(void) {
    func_002721E8();
    return 0;
}

u8 func_0010A580(void) {
    s64 temp_v0;

    temp_v0 = func_00272228();
    return temp_v0 == 0;
}

void func_0010A5A0(void) {
}

u32 func_0010A5A8(void) {
    return 0;
}

u32 func_0010A5B0(void) {
    return 1;
}

void func_0010A5B8(void) {
}

u32 func_0010A5C0(void) {
    return 0;
}

u32 func_0010A5C8(void) {
    return 1;
}

void func_0010A5D0(void) {
    func_00262818();
}

u32 func_0010A5E8(void) {
    func_001260A8();
    func_002628C8();
    return 0;
}

u8 func_0010A610(void) {
    s64 temp_v0;

    temp_v0 = func_00262938();
    return temp_v0 == 0;
}

void func_0010A630(void) {
    func_002912C8(1);
}

u32 func_0010A648(void) {
    func_00291378();
    return 0;
}

u8 func_0010A668(void) {
    s64 temp_v0;

    temp_v0 = func_002913B8();
    return temp_v0 == 0;
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010A688);

void func_0010A6E0(void) {
    func_001A1068();
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010A6F8);

void func_0010A748(void) {
    func_002886E0();
}

u32 func_0010A760(void) {
    func_002886B8();
    return 0;
}

u32 func_0010A780(void) {
    return 0;
}

void func_0010A788(void) {
    D_003BA730 = 0;
    D_003BA958 = 0;
    func_0022AED0();
    func_002354D8();
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010A7A8);

u32 func_0010A7E0(void) {
    return 0;
}

void func_0010A7E8(void) {
    D_003BA730 = 0;
    func_0021FE38();
    func_00232D60();
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010A808);

u32 func_0010A838(void) {
    return 0;
}

void func_0010A840(void) {
}

u32 func_0010A848(void) {
    return 0;
}

u32 func_0010A850(void) {
    return 0;
}

void func_0010A858(void) {
    func_002720B0();
}

u32 func_0010A870(void) {
    func_002721E8();
    return 0;
}

u8 func_0010A890(void) {
    s64 temp_v0;

    temp_v0 = func_00272228();
    return temp_v0 == 0;
}

void func_0010A8B0(u32 arg0, u32 arg1) {
    func_002354D8();
    func_002449F0(arg1);
}

u32 func_0010A8D8(void) {
    func_00235498();
    func_00244AB8();
    func_00126068();
    return 0;
}

u8 func_0010A908(void) {
    s64 temp_v0;

    temp_v0 = func_00244AF8();
    return temp_v0 == 0;
}

void func_0010A928(u32 arg0, s32 arg1) {
    func_002354D8();
    if (arg1 != 0) {
        func_00249FA8(*(u32 *)arg1, ((u32 *)arg1)[1]);
        return;
    }
    func_00249FA8(0, 1);
}

u32 func_0010A970(void) {
    func_00235498();
    func_0024A058();
    return 0;
}

u8 func_0010A998(void) {
    s64 temp_v0;

    temp_v0 = func_0024A0A0();
    return temp_v0 == 0;
}

void func_0010A9B8(void) {
    func_00262818();
}

u32 func_0010A9D0(void) {
    func_002628C8();
    return 0;
}

u8 func_0010A9F0(void) {
    s64 temp_v0;

    temp_v0 = func_00262938();
    return temp_v0 == 0;
}

void func_0010AA10(void) {
}

u32 func_0010AA18(void) {
    return 0;
}

u32 func_0010AA20(void) {
    return 0;
}

void func_0010AA28(void) {
}

u32 func_0010AA30(void) {
    return 0;
}

u32 func_0010AA38(void) {
    return 0;
}

void func_0010AA40(void) {
    func_001983A8();
}

u32 func_0010AA58(void) {
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00107FD8", D_0039E1F0);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010AA60);

void func_0010AA88(void) {
    func_00270A30();
}

u32 func_0010AAA0(void) {
    func_00270A80();
    return 0;
}

u32 func_0010AAC0(void) {
    return 0;
}

void func_0010AAC8(void) {
    func_002C16E0();
}

u32 func_0010AAE0(void) {
    func_002C16E8();
    return 0;
}

u32 func_0010AB00(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010AB08);

u32 func_0010AB48(void) {
    func_002C2EA0();
    func_00126110();
    return 0;
}

u8 func_0010AB70(void) {
    s64 temp_v0;

    temp_v0 = func_002C2ED0();
    return temp_v0 == 0;
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010AB90);

void func_0010ABE0(void) {
    if (D_003BA960 != 0) {
        func_002E3D98(D_003BA960);
        D_003BA960 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010AC10);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010AC98);

void func_0010AEE8(void) {
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010AEF0);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010AF68);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010B1B0);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010B428);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010B558);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010B590);

void func_0010B6A8(void) {
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010B6B0);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010B728);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010B768);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010B7C0);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010BA18);
