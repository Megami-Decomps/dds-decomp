#include "common.h"

extern s32 func_00316ED0(void);

extern u32 D_00435D28;

extern u64 func_0019F448(s32, s32, u64, u64, u64, u64);

extern u32 D_00435CB8;

extern s32 func_002CE920(void);

extern s32 func_002CE928(void);

extern s32 func_00128580(void);

extern u32 D_00435BB0;

extern s32 acknowledgeCampState(void);

extern s32 func_00299868(void);

extern s32 func_002D13F0(void);

extern s32 func_00260848(void);

extern s32 pollSceneState(void);

extern s32 func_0030AA40(void);

extern s32 D_00435D30;

extern u32 D_00435CD4;

extern f32 D_00438A48;

extern f32 D_00438A4C;

extern u16 D_00438E06;

extern f32 D_00438E08;

extern f32 D_00438E0C;

extern f32 D_00438E10;

extern f32 D_00438E14;

extern u16 D_00438E04;

extern void *func_003335E0(void);

extern void *D_00438DB0;

extern s32 kwlnTaskDestroyWithHierarchyByName(const char *name, s32 arg1);

extern char D_00411370[];

extern void func_00124D70(void);

typedef struct KwlnTask KwlnTask;

extern KwlnTask *func_00101740(const char *name);

extern KwlnTask *func_00101820(u32 prio);

extern char D_00435D20[];

extern void evtEventViewerDestroyTask(void);

extern char D_00411380[];

extern s32 D_00389780[];

extern void func_0030A970(s32 arg0);

extern void *D_00435D64;

extern void *D_00435D68;

extern u32 D_00435D60;

typedef struct {
    u8 pad00[0xE8];
    void *unkE8;
    void *unkEC;
} B728Work;

extern u8 D_00380748[];

extern s8 D_0037F53B[];

extern void func_0010B650(void *arg0);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00107EF8);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00107FF8);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00108138);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_001081F8);

INCLUDE_ASM(const s32, "game/code_00107EF8", evtUnk8360SetVec);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00108318);

void evtUnk89F8SetState(s32 arg0, f32 farg0, f32 farg1) {
    if (arg0 == 0) {
        D_00435CD4 &= ~0x4000;
        D_00438A48 = farg0;
        D_00438A4C = farg1;
    }
    else {
        f32 b0 = D_00438A48;
        f32 b1 = D_00438A4C;
        D_00435CD4 |= 0x4000;
        D_00438E06 = arg0;
        D_00438E08 = b0;
        D_00438E0C = farg0;
        D_00438E10 = b1;
        D_00438E14 = farg1;
        D_00438E04 = 0;
    }
}

void evtUnk8A48Ensure(void) {
    if (D_00438DB0 == NULL) {
        D_00438DB0 = func_003335E0();
        *(f32 *)((u8 *)D_00438DB0 + 0x1C) = 1.0f;
    }
}

void func_001089A0(u32 arg0) {
    D_00435CB8 = arg0;
}

INCLUDE_ASM(const s32, "game/code_00107EF8", func_001089A8);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00108AC0);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00108BD8);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00108CE0);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00108D80);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00108E20);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00108EC0);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00109028);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00109248);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_001094F8);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00109538);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00109780);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00109950);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00109B80);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00109C30);

void func_00109D00(s32 arg0, s32 arg1, u64 arg2, u64 arg3) {
    u64 temp_v0;

    temp_v0 = func_0019F448(arg0 << 4, arg1 << 3, 0, arg2, arg3, 0);
    func_0019D518(temp_v0);
    func_0019C5B0(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00109D50);

INCLUDE_ASM(const s32, "game/code_00107EF8", evtUnk9CC0Check);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00109E60);

void func_0010A298(void) {
    func_0010A2B0();
}

INCLUDE_ASM(const s32, "game/code_00107EF8", func_0010A2B0);

INCLUDE_ASM(const s32, "game/code_00107EF8", evtStartSelCreate);

u32 evtStartSelDestroy(void) {
    kwlnTaskDestroyWithHierarchyByName(D_00411370, 1);
    return 1;
}

u32 func_0010A350(void) {
    return 0;
}

void func_0010A358(void) {
    func_002CE208(3);
}

u32 func_0010A370(void) {
    s64 temp_v0;
    u32 temp_v1;

    func_002CE750();
    func_002CE758();
    temp_v0 = func_002CE920();
    temp_v1 = 0xffffffff;
    if (temp_v0 != 0) {
        temp_v1 = 0;
    }
    return temp_v1;
}

u8 func_0010A3A8(void) {
    s64 temp_v0;

    temp_v0 = func_002CE928();
    return temp_v0 == 0;
}

void func_0010A3C8(u32 arg0, s32 arg1) {
    if (arg1 == 0) {
        func_002A3A50(0);
        return;
    }
    func_002A3A50(*(u32 *)arg1);
}

u32 func_0010A3F8(void) {
    func_002A3A70();
    return 0;
}

u32 func_0010A418(void) {
    return 0;
}

void func_0010A420(void) {
    func_002CE208(1);
}

s32 evtUnkA2F8Check(void) {
    func_002CE750();
    func_002CE758();
    if (func_002CE920() != 0) {
        func_00124D70();
        return 0;
    }
    return -1;
}

u8 func_0010A478(void) {
    s64 temp_v0;

    temp_v0 = func_002CE928();
    return temp_v0 == 0;
}

void func_0010A498(void) {
    func_002A7A10();
}

u32 func_0010A4B0(void) {
    func_002A7A30();
    return 0;
}

u32 func_0010A4D0(void) {
    return 0;
}

void func_0010A4D8(void) {
    func_002CE208(2);
}

u32 func_0010A4F0(void) {
    s64 temp_v0;
    u32 temp_v1;

    func_002CE750();
    func_002CE758();
    temp_v0 = func_002CE920();
    temp_v1 = 0xffffffff;
    if (temp_v0 != 0) {
        temp_v1 = 0;
    }
    return temp_v1;
}

u8 func_0010A528(void) {
    s64 temp_v0;

    temp_v0 = func_002CE928();
    return temp_v0 == 0;
}

void func_0010A548(u32 arg0, u32 arg1) {
    func_00105FE8(0);
    func_001283B8(arg1, arg0);
}

u32 func_0010A588(void) {
    s64 temp_v0;
    u32 temp_v1;

    func_00125EE8();
    temp_v0 = func_00128580();
    temp_v1 = 0xffffffff;
    if (temp_v0 != 0) {
        temp_v1 = 0;
    }
    return temp_v1;
}

u32 func_0010A5B8(void) {
    return 0;
}

void func_0010A5C0(void) {
    func_00105FE8(0);
}

u32 func_0010A5D8(void) {
    return 0;
}

u32 func_0010A5E0(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00107EF8", evtUnkA4A8Dispatch);

u32 func_0010A650(void) {
    D_00435BB0 = 1;
    evtDestroySkyTask();
    func_00128658();
    return 0;
}

u32 func_0010A680(void) {
    return 0;
}

void func_0010A688(void) {
    func_002AA360();
}

u32 func_0010A6A0(void) {
    destroyCampTasks();
    return 0;
}

u8 func_0010A6C0(void) {
    s64 temp_v0;

    temp_v0 = acknowledgeCampState();
    return temp_v0 == 0;
}

void func_0010A6E0(void) {
}

u32 func_0010A6E8(void) {
    return 0;
}

u32 func_0010A6F0(void) {
    return 1;
}

void func_0010A6F8(void) {
}

u32 func_0010A700(void) {
    return 0;
}

u32 func_0010A708(void) {
    return 1;
}

void func_0010A710(void) {
    func_00299748();
}

u32 func_0010A728(void) {
    func_00128658();
    func_002997F8();
    return 0;
}

u8 func_0010A750(void) {
    s64 temp_v0;

    temp_v0 = func_00299868();
    return temp_v0 == 0;
}

void func_0010A770(void) {
    func_002D1300(1);
}

u32 func_0010A788(void) {
    configTasksDestroy();
    return 0;
}

u8 func_0010A7A8(void) {
    s64 temp_v0;

    temp_v0 = func_002D13F0();
    return temp_v0 == 0;
}

INCLUDE_ASM(const s32, "game/code_00107EF8", evtUnkA688Dispatch);

void func_0010A820(void) {
    func_001A9D70();
}

u32 evtUnkA6F8Ensure(void) {
    if (func_00101740(D_00435D20) != NULL) {
        return 0;
    }
    if (func_00101820(0x3FA) == NULL) {
        func_00128658();
        return 1;
    }
    return 0;
}

void func_0010A888(void) {
    createBattleStageTestTask();
}

u32 func_0010A8A0(void) {
    func_002C7C18();
    return 0;
}

u32 func_0010A8C0(void) {
    return 0;
}

void func_0010A8C8(void) {
    D_00435BB0 = 0;
    D_00435D28 = 0;
    startEventTestTask();
    func_00250278();
}

INCLUDE_ASM(const s32, "game/code_00107EF8", evtUnkA7A8Init);

u32 func_0010A920(void) {
    return 0;
}

void func_0010A928(void) {
    D_00435BB0 = 0;
    func_0023A9A8();
    func_0024DAF8();
}

u32 evtUnkA808Init(void) {
    evtEventViewerDestroyTask();
    D_00435BB0 = 1;
    func_00105FE8(0);
    return 0;
}

u32 func_0010A978(void) {
    return 0;
}

void func_0010A980(void) {
}

u32 func_0010A988(void) {
    return 0;
}

u32 func_0010A990(void) {
    return 0;
}

void func_0010A998(void) {
    func_002AA360();
}

u32 func_0010A9B0(void) {
    destroyCampTasks();
    return 0;
}

u8 func_0010A9D0(void) {
    s64 temp_v0;

    temp_v0 = acknowledgeCampState();
    return temp_v0 == 0;
}

void func_0010A9F0(u32 arg0, u32 arg1) {
    func_00250278();
    func_00260708(arg1);
}

u32 func_0010AA18(void) {
    evtDestroySkyTask();
    func_00260808();
    func_00128618();
    return 0;
}

u8 func_0010AA48(void) {
    s64 temp_v0;

    temp_v0 = func_00260848();
    return temp_v0 == 0;
}

void func_0010AA68(u32 arg0, s32 arg1) {
    func_00250278();
    if (arg1 != 0) {
        func_00268380(*(u32 *)arg1, ((u32 *)arg1)[1]);
        return;
    }
    func_00268380(0, 1);
}

u32 func_0010AAB0(void) {
    evtDestroySkyTask();
    func_00268470();
    return 0;
}

u8 func_0010AAD8(void) {
    s64 temp_v0;

    temp_v0 = pollSceneState();
    return temp_v0 == 0;
}

void func_0010AAF8(void) {
    func_00299748();
}

u32 func_0010AB10(void) {
    func_002997F8();
    return 0;
}

u8 func_0010AB30(void) {
    s64 temp_v0;

    temp_v0 = func_00299868();
    return temp_v0 == 0;
}

void func_0010AB50(void) {
}

u32 func_0010AB58(void) {
    return 0;
}

u32 func_0010AB60(void) {
    return 0;
}

void func_0010AB68(void) {
}

u32 func_0010AB70(void) {
    return 0;
}

u32 func_0010AB78(void) {
    return 0;
}

void func_0010AB80(void) {
    func_001A03D8();
}

u32 func_0010AB98(void) {
    return 0;
}

u32 evtTestFontCheck(void) {
    return func_00101740(D_00411380) == NULL;
}

void func_0010ABC8(void) {
    createMovieViewerTask();
}

u32 func_0010ABE0(void) {
    destroyMovieViewerTask();
    return 0;
}

u32 func_0010AC00(void) {
    return 0;
}

void func_0010AC08(void) {
    func_003091E8();
}

u32 func_0010AC20(void) {
    func_003091F0();
    return 0;
}

u32 func_0010AC40(void) {
    return 0;
}

void evtUnkAB08Call(s32 arg0, s32 *arg1) {
    if (arg1 == NULL) {
        func_0030A970(0);
    }
    else {
        func_0030A970(arg1[0]);
    }
    D_00389780[0] = 0x3E7;
}

u32 func_0010AC88(void) {
    func_0030AA10();
    func_001286C8();
    return 0;
}

u8 func_0010ACB0(void) {
    s64 temp_v0;

    temp_v0 = func_0030AA40();
    return temp_v0 == 0;
}

void func_0010ACD0(void) {
    func_00316E78();
}

u32 func_0010ACE8(void) {
    func_00316EF8();
    return 0;
}

u8 func_0010AD08(void) {
    s64 temp_v0;

    temp_v0 = func_00316ED0();
    return temp_v0 == 0;
}

void func_0010AD28(u32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = 0;
    if (arg1 != 0) {
        temp_v0 = *(u32 *)arg1;
    }
    func_002D0A20(temp_v0);
    func_002CE208(4);
}

INCLUDE_ASM(const s32, "game/code_00107EF8", func_0010AD58);

u8 func_0010AD98(void) {
    s64 temp_v0;

    temp_v0 = func_002CE928();
    return temp_v0 == 0;
}

INCLUDE_ASM(const s32, "game/code_00107EF8", evtUnkAB90Ensure);

void func_0010AE08(void) {
    if (D_00435D30 != 0) {
        devConsNodeDestroy(D_00435D30);
        D_00435D30 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00107EF8", func_0010AE38);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_0010AEC0);

void func_0010B110(void) {
}

INCLUDE_RODATA(const s32, "game/code_00107EF8", D_00411370);

INCLUDE_RODATA(const s32, "game/code_00107EF8", D_00411380);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_0010B118);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_0010B190);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_0010B3D8);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_0010B650);

s32 func_0010B780(void) {
    if (D_0037F53B[0] != 0) {
        return 0;
    }
    func_0010B650(D_00380748);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00107EF8", func_0010B7B8);

void func_0010B8D0(void) {
}

INCLUDE_ASM(const s32, "game/code_00107EF8", func_0010B8D8);

void evtUnkB728Link(B728Work *arg0) {
    B728Work *t = D_00435D68;

    if (t == NULL) {
        D_00435D64 = arg0;
        D_00435D68 = arg0;
        arg0->unkE8 = NULL;
        arg0->unkEC = NULL;
    }
    else {
        arg0->unkE8 = t;
        t->unkEC = arg0;
        arg0->unkEC = NULL;
        D_00435D68 = arg0;
    }
    D_00435D60++;
}

void evtUnkB768Unlink(B728Work *arg0) {
    if ((B728Work *)D_00435D64 == arg0) {
        D_00435D64 = arg0->unkEC;
    }
    else {
        ((B728Work *)arg0->unkE8)->unkEC = arg0->unkEC;
    }
    if ((B728Work *)D_00435D68 == arg0) {
        D_00435D68 = arg0->unkE8;
    }
    else {
        ((B728Work *)arg0->unkEC)->unkE8 = arg0->unkE8;
    }
    arg0->unkE8 = NULL;
    arg0->unkEC = NULL;
    D_00435D60--;
}

INCLUDE_ASM(const s32, "game/code_00107EF8", func_0010B9E8);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_0010BC40);

INCLUDE_SDATA(const s32, "game/code_00107EF8", D_00435D18);

INCLUDE_SDATA(const s32, "game/code_00107EF8", D_00435D20);

INCLUDE_SDATA(const s32, "game/code_00107EF8", D_00435D28);

INCLUDE_SDATA(const s32, "game/code_00107EF8", D_00435D30);

INCLUDE_SDATA(const s32, "game/code_00107EF8", D_00435D38);

INCLUDE_SDATA(const s32, "game/code_00107EF8", D_00435D40);

INCLUDE_SDATA(const s32, "game/code_00107EF8", D_00435D48);

INCLUDE_SDATA(const s32, "game/code_00107EF8", D_00435D4C);

INCLUDE_SDATA(const s32, "game/code_00107EF8", D_00435D50);

INCLUDE_SDATA(const s32, "game/code_00107EF8", D_00435D58);

INCLUDE_SDATA(const s32, "game/code_00107EF8", D_00435D60);

INCLUDE_SDATA(const s32, "game/code_00107EF8", D_00435D64);

INCLUDE_SDATA(const s32, "game/code_00107EF8", D_00435D68);

