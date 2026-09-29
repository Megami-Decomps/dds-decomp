#include "common.h"

#include "kwln.h"

extern s32 D_003BA960;

extern s32 fldLmapTaskExists(void);

extern s32 fldPollSceneState(void);

extern s32 mnuPollTaskState(void);

extern u32 D_003BA958;

extern s32 func_002913B8(void);

extern s32 func_00262938(void);

extern s32 mnuAcknowledgeCampState(void);

extern u32 D_003BA730;

extern s32 func_00125FD0(void);

extern s32 func_0028F600(void);

extern s32 func_0028F5F8(void);

extern u64 func_00197748(s32, s32, u64, u64, u64, u64);

extern u32 D_003BA8E8;

extern u32 D_003BA904;

extern s32 kwlnTaskDestroyWithHierarchyByName(const char *name, s32 arg1);

extern KwlnTask *kwlnTaskGetTaskByName(const char *name);

extern KwlnTask *kwlnTaskFindByPriority(u32 prio);

extern u32 func_00104260(void);

extern void fldStartSequenceRecord(void);

extern void func_00102A18(void);

extern void func_001A11F0(s32 arg0, s32 arg1, s32 arg2);

extern void evtEventViewerDestroyTask(void);

extern void evtStopTestTasks(void);

extern void evtDestroySkyTask(void);

extern void *func_002DA730(void);

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

extern void evtCreateSkyTask(void);

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

extern void evtSelStateDestroy(void);

extern s32 D_0032E3C0[];

extern void func_002C2E38(s32 arg0);

extern s32 sdfDevConsNodeCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

extern s32 func_002E41B0(s32 arg0, s32 arg1);

extern void sdfDevConsSetTextAttribute(s32 arg0, s32 arg1);

extern void *D_003BA994;

extern void *D_003BA998;

extern u32 D_003BA990;

typedef struct B728Work {
    u8 pad00[0xE8];
    struct B728Work *previous;
    struct B728Work *next;
} B728Work;

extern u8 D_00325748[];

extern s8 D_0032453B[];

extern void func_0010B428(void *arg0);

extern void kwlnTaskDestroyWithHierarchy(void *, s32);

extern void func_0010AC98(void);

extern void *D_003BD764;

extern void func_00109108();

typedef struct KwlnResourceNode {
    s32 unk0;
    struct KwlnResourceNode *next;
    s32 *ready;
} KwlnResourceNode;

extern s32 D_003BD3C8;

extern void *kwlnTaskCreate(const char *, s32, s32, s32, void (*)(void), void (*)(void), void *);

extern void func_0010B590(void);

extern void *D_003BD768;

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00107FD8);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_001080D8);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00108218);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_001082D8);

void evtUnk8360SetVec(s32 mode, f32 x, f32 y, f32 z, f32 w) {
    if (mode == 0) {
        D_003BA904 &= ~0x400;
        D_00324790.x = x;
        D_00324790.y = y;
        D_00324790.z = z;
        D_00324790.w = w;
    } else {
        f32 b0 = D_00324790.x;
        f32 b1 = D_00324790.y;
        f32 b2 = D_00324790.z;
        f32 b3 = D_00324790.w;
        D_003BA904 |= 0x400;
        D_003BD6FA = mode;
        D_003C2C20.x = b0;
        D_003C2C20.y = b1;
        D_003C2C20.z = b2;
        D_003C2C20.w = b3;
        D_003C2C30.x = x;
        D_003C2C30.y = y;
        D_003C2C30.z = z;
        D_003C2C30.w = w;
        D_003BD6F8 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_001083F8);

void evtUnk89F8SetState(s32 arg0, f32 farg0, f32 farg1) {
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

void evtUnk8A48Ensure(void) {
    if (D_003BD6B0 == NULL) {
        D_003BD6B0 = func_002DA730();
        *(f32 *)((u8 *)D_003BD6B0 + 0x1C) = 1.0f;
    }
}

void func_00108A80(u32 arg0) {
    D_003BA8E8 = arg0;
}

typedef struct EvtDrawSurface {
    u8 unk_00[0x10];
    void (*submit)(struct EvtDrawSurface *, void *);
    u8 unk_14[0xC];
} EvtDrawSurface;

extern EvtDrawSurface D_00324B48[];

extern void *sdfAllocPacketAligned(s32);

extern void sdfInitPacketList(void *);

extern void sdfAppendPacket(void *, void *);

extern u8 *func_002E13E0(void *, s32);

void func_00108A88(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    void *list = sdfAllocPacketAligned(0x20);
    void *packet;
    u8 *command;
    sdfInitPacketList(list);
    packet = sdfAllocPacketAligned(0x30);
    command = func_002E13E0(packet, 0x30);
    *(u64 *)(command + 0x20) = (arg7 << 17) | 0x10000 | (arg5 << 15) | (arg4 << 14) | (arg3 << 12) | (arg2 << 4) | (arg1 << 1) | arg0;
    *(u64 *)(command + 0x28) = 0x47;
    sdfAppendPacket(list, packet);
    {
        u8 *surface = (u8 *)D_00324B48 + (D_003BA8E8 << 5);
        (*(void (**)(u8 *, void *))(surface + 0x10))(surface, list);
    }
}

void func_00108BA0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    void *list = sdfAllocPacketAligned(0x20);
    void *packet;
    u8 *command;
    sdfInitPacketList(list);
    packet = sdfAllocPacketAligned(0x30);
    command = func_002E13E0(packet, 0x30);
    *(u64 *)(command + 0x20) = (arg7 << 17) | 0x10000 | (arg5 << 15) | (arg4 << 14) | (arg3 << 12) | (arg2 << 4) | (arg1 << 1) | arg0;
    *(u64 *)(command + 0x28) = 0x48;
    sdfAppendPacket(list, packet);
    {
        u8 *surface = (u8 *)D_00324B48 + (D_003BA8E8 << 5);
        (*(void (**)(u8 *, void *))(surface + 0x10))(surface, list);
    }
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00108CB8);

void evtSubmitTexturePacket(s32 value) {
    void *list = sdfAllocPacketAligned(0x20);
    void *packet;
    u8 *command;
    sdfInitPacketList(list);
    packet = sdfAllocPacketAligned(0x30);
    command = func_002E13E0(packet, 0x30);
    *(u64 *)(command + 0x20) = ((u64)value << 32) | 0x64;
    *(u64 *)(command + 0x28) = 0x42;
    sdfAppendPacket(list, packet);
    {
        u8 *surface = (u8 *)D_00324B48 + (D_003BA8E8 << 5);
        (*(void (**)(u8 *, void *))(surface + 0x10))(surface, list);
    }
}

extern s32 func_00100518(void);

extern u8 D_00326ED0[];

extern void func_002D4C80(const void *, void *, s32);

extern void func_002D40E8(void *, void *);

void func_00108E60(void) {
    void *list = sdfAllocPacketAligned(0x20);
    void *texture;
    sdfInitPacketList(list);
    texture = sdfAllocPacketAligned(0x40);
    func_002D4C80(D_00326ED0 + func_00100518() * 0x1F40, texture, 0);
    func_002D40E8(list, texture);
    {
        u8 *surface = (u8 *)D_00324B48 + (D_003BA8E8 << 5);
        (*(void (**)(u8 *, void *))(surface + 0x10))(surface, list);
    }
}

extern void func_002D4CC8(const void *, void *, s32);

void func_00108F00(void) {
    void *list = sdfAllocPacketAligned(0x20);
    void *texture;
    sdfInitPacketList(list);
    texture = sdfAllocPacketAligned(0x40);
    func_002D4CC8(D_00326ED0 + func_00100518() * 0x1F40, texture, 0);
    func_002D40E8(list, texture);
    {
        u8 *surface = (u8 *)D_00324B48 + (D_003BA8E8 << 5);
        (*(void (**)(u8 *, void *))(surface + 0x10))(surface, list);
    }
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00108FA0);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00109108);

void func_001093B8(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7) {
    func_00109108(a0, a1, a2, a3, 0xFFFFFF, a4, a5, a6, a7);
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_001093F8);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00109640);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00109810);

extern f32 D_003245B0[];

extern u32 D_003245D0[];

extern void *func_002EF2B0(const void *, const void *, s32, s32);

void evtSubmitViewParamPacket(u32 first, u32 second, f32 x, f32 y, f32 z, f32 u, f32 v, f32 w) {
    void *list;
    D_003245B0[0] = x;
    D_003245B0[1] = y;
    D_003245B0[2] = z;
    D_003245B0[4] = u;
    D_003245B0[5] = v;
    D_003245B0[6] = w;
    D_003245D0[1] = second;
    D_003245D0[0] = first;
    list = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    sdfAppendPacket(list, func_002EF2B0(D_003245B0, D_003245D0, 2, 0x80));
    {
        u8 *surface = (u8 *)D_00324B48 + (D_003BA8E8 << 5);
        (*(void (**)(u8 *, void *))(surface + 0x10))(surface, list);
    }
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00109AF0);

void func_00109BC0(s32 arg0, s32 arg1, u64 arg2, u64 arg3) {
    u64 temp_v0;

    temp_v0 = func_00197748(arg0 << 4, arg1 << 3, 0, arg2, arg3, 0);
    func_00195868(temp_v0);
    func_00194920(temp_v0);
}

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

s32 evtSelStateCreate(s32 limit, s16 frames, s32 arg2, s32 arg3) {
    EvtSelState *node;
    if (frames == 0) {
        return 0;
    }
    if (D_003BA948 != 0) {
        evtSelStateDestroy();
    }
    node = func_002CFF68(0x28);
    D_003BD760 = node;
    if (limit == 0) {
        node->limit = -1;
    } else {
        node->limit = limit;
    }
    ((EvtSelState *)D_003BD760)->unk_08 = arg2;
    ((EvtSelState *)D_003BD760)->unk_0C = arg3;
    ((EvtSelState *)D_003BD760)->unk_10 = frames;
    ((EvtSelState *)D_003BD760)->unk_12 = frames;
    ((EvtSelState *)D_003BD760)->unk_14 = frames;
    D_003BA948 = 1;
    return 1;
}

u32 evtUnk9CC0Check(void) {
    EvtSelState *sel;
    if (D_003BA948 == 0) {
        return 0;
    }
    sel = D_003BD760;
    if (sel->limit > sel->count || sel->limit == -1) {
        func_00109D20();
    } else {
        evtSelStateDestroy();
        return 1;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_00109D20);

void func_0010A158(void) {
    evtSelStateDestroy();
}

void evtSelStateDestroy(void) {
    if (D_003BA948 != 0) {
        func_002CFF98(D_003BD760);
        D_003BA948 = 0;
        __asm__ volatile(".set noreorder\n\tvmove.xyzw $vf10, $vf0\n\tvmulx.w $vf10, $vf10, $vf0x\n\t.set reorder");
        __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(D_00324590) : "memory");
    }
}

s32 evtStartSelCreate(void) {
    return kwlnTaskCreate(D_0039E1F0, 0x2AF9, 0, 0, (s32)func_00104260, 0, 0);
}

u32 evtStartSelDestroy(void) {
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

s32 evtUnkA2F8Check(void) {
    func_0028F458();
    func_0028F460();
    if (func_0028F5F8() != 0) {
        fldStartSequenceRecord();
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
    mnuStartStaffMovieRequest();
}

u32 func_0010A370(void) {
    mnuStopStaffTasks();
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

    fldCleanupFieldScene();
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

void evtUnkA4A8Dispatch(s32 arg0, s32 *arg1) {
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
    evtCreateSkyTask();
    func_00220110(v0);
}

u32 func_0010A510(void) {
    D_003BA730 = 1;
    evtDestroySkyTask();
    fldDispatchDeferredFieldCommand();
    return 0;
}

u32 func_0010A540(void) {
    return 0;
}

void func_0010A548(void) {
    func_002720B0();
}

u32 func_0010A560(void) {
    mnuDestroyCampTasks();
    return 0;
}

u8 func_0010A580(void) {
    s64 temp_v0;

    temp_v0 = mnuAcknowledgeCampState();
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
    mnuStaffCreateTasks();
}

u32 func_0010A5E8(void) {
    fldDispatchDeferredFieldCommand();
    mnuStaffDestroyTasks();
    return 0;
}

u8 func_0010A610(void) {
    s64 temp_v0;

    temp_v0 = func_00262938();
    return temp_v0 == 0;
}

void func_0010A630(void) {
    configTasksCreate(1);
}

u32 func_0010A648(void) {
    mnuConfigTasksDestroy();
    return 0;
}

u8 func_0010A668(void) {
    s64 temp_v0;

    temp_v0 = func_002913B8();
    return temp_v0 == 0;
}

void evtUnkA688Dispatch(s32 arg0, s32 *arg1) {
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
        fldDispatchDeferredFieldCommand();
    }
}

void func_0010A6E0(void) {
    func_001A1068();
}

u32 evtUnkA6F8Ensure(void) {
    if (kwlnTaskGetTaskByName(D_003BA950) != NULL) {
        return 0;
    }
    if (kwlnTaskFindByPriority(0x3FA) == NULL) {
        fldDispatchDeferredFieldCommand();
        return 1;
    }
    return 0;
}

void func_0010A748(void) {
    btlCreateStageTestTask();
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
    evtStartTestTask();
    evtCreateSkyTask();
}

extern void func_001060C8();

u32 evtUnkA7A8Init(void) {
    D_003BA730 = 1;
    func_001060C8(0);
    evtStopTestTasks();
    evtDestroySkyTask();
    return 1;
}

u32 func_0010A7E0(void) {
    return 0;
}

void func_0010A7E8(void) {
    D_003BA730 = 0;
    evtDestroySecondaryWorldNode();
    func_00232D60();
}

u32 evtUnkA808Init(void) {
    evtEventViewerDestroyTask();
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
    mnuDestroyCampTasks();
    return 0;
}

u8 func_0010A890(void) {
    s64 temp_v0;

    temp_v0 = mnuAcknowledgeCampState();
    return temp_v0 == 0;
}

void func_0010A8B0(u32 arg0, u32 arg1) {
    evtCreateSkyTask();
    func_002449F0(arg1);
}

u32 func_0010A8D8(void) {
    evtDestroySkyTask();
    func_00244AB8();
    func_00126068();
    return 0;
}

u8 func_0010A908(void) {
    s64 temp_v0;

    temp_v0 = mnuPollTaskState();
    return temp_v0 == 0;
}

void func_0010A928(u32 arg0, s32 arg1) {
    evtCreateSkyTask();
    if (arg1 != 0) {
        func_00249FA8(*(u32 *)arg1, ((u32 *)arg1)[1]);
        return;
    }
    func_00249FA8(0, 1);
}

u32 func_0010A970(void) {
    evtDestroySkyTask();
    fldStopSceneTasks();
    return 0;
}

u8 func_0010A998(void) {
    s64 temp_v0;

    temp_v0 = fldPollSceneState();
    return temp_v0 == 0;
}

void func_0010A9B8(void) {
    mnuStaffCreateTasks();
}

u32 func_0010A9D0(void) {
    mnuStaffDestroyTasks();
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

u32 evtTestFontCheck(void) {
    return kwlnTaskGetTaskByName(D_0039E200) == NULL;
}

void func_0010AA88(void) {
    mnuCreateMovieViewerTask();
}

u32 func_0010AAA0(void) {
    mnuDestroyMovieViewerTask();
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

void evtUnkAB08Call(s32 arg0, s32 *arg1) {
    if (arg1 == NULL) {
        func_002C2E38(0);
    }
    else {
        func_002C2E38(arg1[0]);
    }
    D_0032E3C0[0] = 0x3E7;
}

u32 func_0010AB48(void) {
    fldStopLmapTask();
    func_00126110();
    return 0;
}

u8 func_0010AB70(void) {
    s64 temp_v0;

    temp_v0 = fldLmapTaskExists();
    return temp_v0 == 0;
}

u32 evtUnkAB90Ensure(void) {
    if (D_003BA960 == 0) {
        D_003BA960 = sdfDevConsNodeCreate(0x7100, 0x7A60, 0x28, 0x14);
        func_002E41B0(D_003BA960, 2);
        sdfDevConsSetTextAttribute(D_003BA960, 7);
    }
    return 0;
}

void func_0010ABE0(void) {
    if (D_003BA960 != 0) {
        sdfDevConsNodeDestroy(D_003BA960);
        D_003BA960 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010AC10);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010AC98);

void func_0010AEE8(void) {
}

INCLUDE_RODATA(const s32, "game/code_00107FD8", D_0039E1F0);

INCLUDE_RODATA(const s32, "game/code_00107FD8", D_0039E200);

void func_0010AEF0(s8 mode) {
    if (mode == 1) {
        D_003BD764 = kwlnTaskCreate("DebugTimeGrph", 0x2710, 1, 1, func_0010AC98, func_0010AEE8, NULL);
    } else if (mode == 0) {
        kwlnTaskDestroyWithHierarchy(D_003BD764, 0);
    }
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010AF68);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010B1B0);

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010B428);

s32 func_0010B558(void) {
    if (D_0032453B[0] != 0) {
        return 0;
    }
    func_0010B428(D_00325748);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010B590);

void func_0010B6A8(void) {
}

INCLUDE_ASM(const s32, "game/code_00107FD8", func_0010B6B0);

void evtLinkWorkNode(B728Work *node) {
    B728Work *tail = D_003BA998;

    if (tail == NULL) {
        D_003BA994 = node;
        D_003BA998 = node;
        node->previous = NULL;
        node->next = NULL;
    }
    else {
        node->previous = tail;
        tail->next = node;
        node->next = NULL;
        D_003BA998 = node;
    }
    D_003BA990++;
}

void evtUnlinkWorkNode(B728Work *node) {
    if ((B728Work *)D_003BA994 == node) {
        D_003BA994 = node->next;
    }
    else {
        node->previous->next = node->next;
    }
    if ((B728Work *)D_003BA998 == node) {
        D_003BA998 = node->previous;
    }
    else {
        node->next->previous = node->previous;
    }
    node->previous = NULL;
    node->next = NULL;
    D_003BA990--;
}

INCLUDE_ASM(const s32, "game/code_00107FD8", bfContextCreate);

INCLUDE_ASM(const s32, "game/code_00107FD8", bfParseFLW0);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA948);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA950);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA958);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA960);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA968);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA970);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA978);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA97C);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA980);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA988);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA990);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA994);

INCLUDE_SDATA(const s32, "game/code_00107FD8", D_003BA998);
