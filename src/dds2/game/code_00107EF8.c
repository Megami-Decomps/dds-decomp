#include "common.h"

extern s32 func_00316ED0(void);

extern u32 D_00435D28;

extern u64 func_0019F448(s32, s32, u64, u64, u64, u64);

extern u32 D_00435CB8;

extern s32 func_002CE920(void);

extern s32 func_002CE928(void);

extern s32 func_00128580(void);

extern u32 D_00435BB0;

extern s32 mnuAcknowledgeCampState(void);

extern s32 func_00299868(void);

extern s32 func_002D13F0(void);

extern s32 func_00260848(void);

extern s32 fldPollSceneState(void);

extern s32 fldLmapTaskExists(void);

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
    void *previous;
    void *next;
} B728Work;

extern u8 D_00380748[];

extern s8 D_0037F53B[];

extern void func_0010B650(void *arg0);

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} DrawVec4;

extern DrawVec4 D_0037F790;

extern DrawVec4 D_0043E3A0;

extern DrawVec4 D_0043E3B0;

extern u16 D_00438DFA;

extern u16 D_00438DF8;

typedef struct EvtDrawSurface {
    u8 unk_00[0x10];
    void (*submit)(struct EvtDrawSurface *, void *);
    u8 unk_14[0xC];
} EvtDrawSurface;

extern EvtDrawSurface D_0037FB48[];

extern void *sdfAllocPacketAligned(s32);

extern void sdfResetPacketList(void *);

extern void sdfAppendPacket(void *, void *);

extern u8 *func_0033A290(void *, s32);

extern s32 func_00100400(void);

extern u8 D_00381ED0[];

extern void func_0032DB30(const void *, void *, s32);

extern void func_0032CF98(void *, void *);

extern void func_0032DB78(const void *, void *, s32);

extern f32 D_0037F5B0[];

extern u32 D_0037F5D0[];

extern void *func_00348158(const void *, const void *, s32, s32);

extern s8 D_00435D18;

extern void *D_00438E60;

extern void func_0010A2B0(void);

typedef struct EvtSelState {
    u8 unk_00[4];
    s32 limit;
    s32 unk_08;
    s32 unk_0C;
    s16 unk_10;
    s16 unk_12;
    s16 unk_14;
    u8 unk_16[2];
    s32 count;
} EvtSelState;

extern void func_00109E60(void);

extern u8 D_0037F590[];

extern s32 kwlnTaskCreate(const char *name, s32 arg1, s32 arg2, s32 arg3, s32 update, s32 destroy, s32 data);

extern u32 func_00104150(void);

extern s32 D_004371E8;

extern void func_0023AC80(s32 arg0);

extern void evtCreateSkyTask(void);

extern void evtStopTestTasks(void);

extern void evtDestroySkyTask(void);

extern void func_00105FE8();

extern s32 sdfDevConsNodeCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

extern s32 func_0033D060(s32 arg0, s32 arg1);

extern void func_0033D050(s32 arg0, s32 arg1);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00107EF8);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00107FF8);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00108138);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_001081F8);

void evtUnk8360SetVec(s32 mode, f32 x, f32 y, f32 z, f32 w) {
    if (mode == 0) {
        D_00435CD4 &= ~0x400;
        D_0037F790.x = x;
        D_0037F790.y = y;
        D_0037F790.z = z;
        D_0037F790.w = w;
    } else {
        f32 b0 = D_0037F790.x;
        f32 b1 = D_0037F790.y;
        f32 b2 = D_0037F790.z;
        f32 b3 = D_0037F790.w;
        D_00435CD4 |= 0x400;
        D_00438DFA = mode;
        D_0043E3A0.x = b0;
        D_0043E3A0.y = b1;
        D_0043E3A0.z = b2;
        D_0043E3A0.w = b3;
        D_0043E3B0.x = x;
        D_0043E3B0.y = y;
        D_0043E3B0.z = z;
        D_0043E3B0.w = w;
        D_00438DF8 = 0;
    }
}

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

void func_00108CE0(s32 value) {
    void *list = sdfAllocPacketAligned(0x20);
    void *packet;
    u8 *command;
    sdfResetPacketList(list);
    packet = sdfAllocPacketAligned(0x30);
    command = func_0033A290(packet, 0x30);
    *(u64 *)(command + 0x20) = ((u64)value << 32) | 0x64;
    *(u64 *)(command + 0x28) = 0x42;
    sdfAppendPacket(list, packet);
    {
        u8 *surface = (u8 *)D_0037FB48 + (D_00435CB8 << 5);
        (*(void (**)(u8 *, void *))(surface + 0x10))(surface, list);
    }
}

void func_00108D80(void) {
    void *list = sdfAllocPacketAligned(0x20);
    void *texture;
    sdfResetPacketList(list);
    texture = sdfAllocPacketAligned(0x40);
    func_0032DB30(D_00381ED0 + func_00100400() * 0x1F40, texture, 0);
    func_0032CF98(list, texture);
    {
        u8 *surface = (u8 *)D_0037FB48 + (D_00435CB8 << 5);
        (*(void (**)(u8 *, void *))(surface + 0x10))(surface, list);
    }
}

void func_00108E20(void) {
    void *list = sdfAllocPacketAligned(0x20);
    void *texture;
    sdfResetPacketList(list);
    texture = sdfAllocPacketAligned(0x40);
    func_0032DB78(D_00381ED0 + func_00100400() * 0x1F40, texture, 0);
    func_0032CF98(list, texture);
    {
        u8 *surface = (u8 *)D_0037FB48 + (D_00435CB8 << 5);
        (*(void (**)(u8 *, void *))(surface + 0x10))(surface, list);
    }
}

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00108EC0);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00109028);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00109248);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_001094F8);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00109538);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00109780);

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00109950);

void func_00109B80(u32 first, u32 second, f32 x, f32 y, f32 z, f32 u, f32 v, f32 w) {
    void *list;
    D_0037F5B0[0] = x;
    D_0037F5B0[1] = y;
    D_0037F5B0[2] = z;
    D_0037F5B0[4] = u;
    D_0037F5B0[5] = v;
    D_0037F5B0[6] = w;
    D_0037F5D0[1] = second;
    D_0037F5D0[0] = first;
    list = sdfAllocPacketAligned(0x20);
    sdfResetPacketList(list);
    sdfAppendPacket(list, func_00348158(D_0037F5B0, D_0037F5D0, 2, 0x80));
    {
        u8 *surface = (u8 *)D_0037FB48 + (D_00435CB8 << 5);
        (*(void (**)(u8 *, void *))(surface + 0x10))(surface, list);
    }
}

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00109C30);

void func_00109D00(s32 arg0, s32 arg1, u64 arg2, u64 arg3) {
    u64 temp_v0;

    temp_v0 = func_0019F448(arg0 << 4, arg1 << 3, 0, arg2, arg3, 0);
    func_0019D518(temp_v0);
    func_0019C5B0(temp_v0);
}

s32 func_00109D50(s32 limit, s16 frames, s32 arg2, s32 arg3) {
    EvtSelState *node;
    if (frames == 0) {
        return 0;
    }
    if (D_00435D18 != 0) {
        func_0010A2B0();
    }
    node = func_00328E18(0x28);
    D_00438E60 = node;
    if (limit == 0) {
        node->limit = -1;
    } else {
        node->limit = limit;
    }
    ((EvtSelState *)D_00438E60)->unk_08 = arg2;
    ((EvtSelState *)D_00438E60)->unk_0C = arg3;
    ((EvtSelState *)D_00438E60)->unk_10 = frames;
    ((EvtSelState *)D_00438E60)->unk_12 = frames;
    ((EvtSelState *)D_00438E60)->unk_14 = frames;
    D_00435D18 = 1;
    return 1;
}

u32 evtUnk9CC0Check(void) {
    EvtSelState *sel;
    if (D_00435D18 == 0) {
        return 0;
    }
    sel = D_00438E60;
    if (sel->limit > sel->count || sel->limit == -1) {
        func_00109E60();
    } else {
        func_0010A2B0();
        return 1;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00107EF8", func_00109E60);

void func_0010A298(void) {
    func_0010A2B0();
}

void func_0010A2B0(void) {
    if (D_00435D18 != 0) {
        func_00328E48(D_00438E60);
        D_00435D18 = 0;
        __asm__ volatile(".set noreorder\n\tvmove.xyzw $vf10, $vf0\n\tvmulx.w $vf10, $vf10, $vf0x\n\t.set reorder");
        __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(D_0037F590) : "memory");
    }
}

s32 evtStartSelCreate(void) {
    return kwlnTaskCreate(D_00411370, 0x2AF9, 0, 0, (s32)func_00104150, 0, 0);
}

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

void evtUnkA4A8Dispatch(s32 arg0, s32 *arg1) {
    s32 v0 = 0;

    D_00435BB0 = 0;
    switch (arg0) {
    case 0:
        v0 = arg1[0];
        break;
    case 1:
        v0 = D_004371E8;
        break;
    }
    if (v0 <= 0) {
        return;
    }
    evtCreateSkyTask();
    func_0023AC80(v0);
}

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
    mnuDestroyCampTasks();
    return 0;
}

u8 func_0010A6C0(void) {
    s64 temp_v0;

    temp_v0 = mnuAcknowledgeCampState();
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
    configTasksCreate(1);
}

u32 func_0010A788(void) {
    mnuConfigTasksDestroy();
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
    btlCreateStageTestTask();
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
    evtStartTestTask();
    evtCreateSkyTask();
}

u32 evtUnkA7A8Init(void) {
    D_00435BB0 = 1;
    func_00105FE8(0);
    evtStopTestTasks();
    evtDestroySkyTask();
    return 1;
}

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
    mnuDestroyCampTasks();
    return 0;
}

u8 func_0010A9D0(void) {
    s64 temp_v0;

    temp_v0 = mnuAcknowledgeCampState();
    return temp_v0 == 0;
}

void func_0010A9F0(u32 arg0, u32 arg1) {
    evtCreateSkyTask();
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
    evtCreateSkyTask();
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

    temp_v0 = fldPollSceneState();
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
    mnuCreateMovieViewerTask();
}

u32 func_0010ABE0(void) {
    mnuDestroyMovieViewerTask();
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
    fldStopLmapTask();
    func_001286C8();
    return 0;
}

u8 func_0010ACB0(void) {
    s64 temp_v0;

    temp_v0 = fldLmapTaskExists();
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

u32 evtUnkAB90Ensure(void) {
    if (D_00435D30 == 0) {
        D_00435D30 = sdfDevConsNodeCreate(0x7100, 0x7A60, 0x28, 0x14);
        func_0033D060(D_00435D30, 2);
        func_0033D050(D_00435D30, 7);
    }
    return 0;
}

void func_0010AE08(void) {
    if (D_00435D30 != 0) {
        sdfDevConsNodeDestroy(D_00435D30);
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

void evtUnkB728Link(B728Work *node) {
    B728Work *tail = D_00435D68;

    if (tail == NULL) {
        D_00435D64 = node;
        D_00435D68 = node;
        node->previous = NULL;
        node->next = NULL;
    }
    else {
        node->previous = tail;
        tail->next = node;
        node->next = NULL;
        D_00435D68 = node;
    }
    D_00435D60++;
}

void evtUnkB768Unlink(B728Work *node) {
    if ((B728Work *)D_00435D64 == node) {
        D_00435D64 = node->next;
    }
    else {
        ((B728Work *)node->previous)->next = node->next;
    }
    if ((B728Work *)D_00435D68 == node) {
        D_00435D68 = node->previous;
    }
    else {
        ((B728Work *)node->next)->previous = node->previous;
    }
    node->previous = NULL;
    node->next = NULL;
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
