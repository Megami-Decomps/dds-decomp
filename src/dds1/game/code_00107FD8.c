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

extern u32 D_003BA904;

typedef struct KwlnTask KwlnTask;

extern s32 kwlnTaskCreate(const char *name, s32 arg1, s32 arg2, s32 arg3, s32 update, s32 destroy, s32 data);
extern s32 kwlnTaskDestroyWithHierarchyByName(const char *name, s32 arg1);
extern KwlnTask *kwlnTaskGetTaskByName(const char *name);
extern KwlnTask *func_00101938(u32 prio);
extern u32 func_00104260(void);
extern void func_00122D60(void);
extern void func_00102A18(void);
extern void func_001A11F0(s32 arg0, s32 arg1, s32 arg2);
extern void func_00232E00(void);
extern void func_0022AF18(void);
extern void func_00235498(void);
extern void *func_002DA730(void);
extern char D_00325748[];
extern s8 D_0032453B;
extern s8 D_003BA948;
extern void *D_003BD760;
extern u8 D_00324590[];
extern char D_003BA950[];
extern void *D_003BD6B0;
extern f32 D_003BD358;
extern f32 D_003BD35C;
extern u16 D_003BD706;
extern f32 D_003BD708;
extern f32 D_003BD70C;
extern f32 D_003BD710;
extern f32 D_003BD714;
extern u16 D_003BD704;

extern char D_0039E1F0[];
extern char D_0039E200[];
extern s32 D_003BBDA8;
extern void func_00220110(s32 arg0);
extern void func_002354D8(void);

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} DrawVec4;

extern DrawVec4 D_00324790;
extern DrawVec4 D_003C2C20;
extern DrawVec4 D_003C2C30;
extern u16 D_003BD6FA;
extern u16 D_003BD6F8;
extern void func_00109D20(void);

extern void func_0010A170(void);
extern s32 D_0032E3C0[];
extern void func_002C2E38(s32 arg0);
extern s32 func_002E3E18(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern s32 func_002E41B0(s32 arg0, s32 arg1);
extern void func_002E41A0(s32 arg0, s32 arg1);
extern void *D_003BA994;
extern void *D_003BA998;
extern u32 D_003BA990;

typedef struct {
    u8 pad00[0xE8];
    void *unkE8;
    void *unkEC;
} B728Work;

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00107FD8);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_001080D8);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00108218);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_001082D8);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00108360);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_001083F8);

void func_001089F8(s32 arg0, f32 farg0, f32 farg1) {
    if (arg0 == 0) {
        D_003BA904 &= ~0x4000;
        D_003BD358 = farg0;
        D_003BD35C = farg1;
    }
    else {
        f32 b0 = D_003BD358;
        f32 b1 = D_003BD35C;
        D_003BA904 |= 0x4000;
        D_003BD706 = arg0;
        D_003BD708 = b0;
        D_003BD70C = farg0;
        D_003BD710 = b1;
        D_003BD714 = farg1;
        D_003BD704 = 0;
    }
}

void func_00108A48(void) {
    if (D_003BD6B0 == NULL) {
        D_003BD6B0 = func_002DA730();
        *(f32 *)((u8 *)D_003BD6B0 + 0x1C) = 1.0f;
    }
}

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

s32 func_0010A1B0(void) {
    return kwlnTaskCreate(D_0039E1F0, 0x2AF9, 0, 0, (s32)func_00104260, 0, 0);
}

u32 func_0010A1E8(void) {
    kwlnTaskDestroyWithHierarchyByName(D_0039E1F0, 1);
    return 1;
}

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

s32 func_0010A2F8(void) {
    func_0028F458();
    func_0028F460();
    if (func_0028F5F8() != 0) {
        func_00122D60();
        return 0;
    }
    return -1;
}

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

void func_0010A4A8(s32 arg0, s32 *arg1) {
    s32 v0 = 0;

    D_003BA730 = 0;
    switch (arg0) {
    case 0:
        v0 = arg1[0];
        break;
    case 1:
        v0 = D_003BBDA8;
        break;
    }
    if (v0 <= 0) {
        return;
    }
    func_002354D8();
    func_00220110(v0);
}

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

void func_0010A688(s32 arg0, s32 *arg1) {
    if (arg0 == 0) {
        if (arg1 == NULL) {
            func_001A11F0(1, 0, 0);
        }
        else {
            func_001A11F0(0, arg1[0], arg1[1]);
        }
    }
    else {
        func_00102A18();
        func_001260A8();
    }
}

void func_0010A6E0(void) {
    func_001A1068();
}

u32 func_0010A6F8(void) {
    if (kwlnTaskGetTaskByName(D_003BA950) != NULL) {
        return 0;
    }
    if (func_00101938(0x3FA) == NULL) {
        func_001260A8();
        return 1;
    }
    return 0;
}

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

u32 func_0010A808(void) {
    func_00232E00();
    D_003BA730 = 1;
    func_001060C8(0);
    return 0;
}

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

u32 func_0010AA60(void) {
    return kwlnTaskGetTaskByName(D_0039E200) == NULL;
}

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

void func_0010AB08(s32 arg0, s32 *arg1) {
    if (arg1 == NULL) {
        func_002C2E38(0);
    }
    else {
        func_002C2E38(arg1[0]);
    }
    D_0032E3C0[0] = 0x3E7;
}

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

void func_0010B728(B728Work *arg0) {
    B728Work *t = D_003BA998;

    if (t == NULL) {
        D_003BA994 = arg0;
        D_003BA998 = arg0;
        arg0->unkE8 = NULL;
        arg0->unkEC = NULL;
    }
    else {
        arg0->unkE8 = t;
        t->unkEC = arg0;
        arg0->unkEC = NULL;
        D_003BA998 = arg0;
    }
    D_003BA990++;
}

void func_0010B768(B728Work *arg0) {
    if ((B728Work *)D_003BA994 == arg0) {
        D_003BA994 = arg0->unkEC;
    }
    else {
        ((B728Work *)arg0->unkE8)->unkEC = arg0->unkEC;
    }
    if ((B728Work *)D_003BA998 == arg0) {
        D_003BA998 = arg0->unkE8;
    }
    else {
        ((B728Work *)arg0->unkEC)->unkE8 = arg0->unkE8;
    }
    arg0->unkE8 = NULL;
    arg0->unkEC = NULL;
    D_003BA990--;
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010B7C0);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010BA18);
