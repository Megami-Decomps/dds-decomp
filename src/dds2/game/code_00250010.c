#include "common.h"

extern u32 D_004373CC;

extern u16 D_004373C8;

extern void func_00135588(s32 arg0);

extern s16 func_00135598(void);

extern u16 D_00438FB0;

extern u16 D_00438FB2;

extern s16 D_00438FB4;

extern s16 D_00438FB6;

extern void func_00134A18(void);

extern void func_0012BC38(s32 arg0);

extern void func_0012D3E0(void);

extern void sdfAppendPacket(s32 list, s32 packet);

extern s32 func_0033D810();

typedef struct {
    s16 unk0;
    s8 unk2;
    u8 pad3[7];
} EvtTblEntry; /* 0xA bytes */

extern EvtTblEntry D_003C9730[];

extern s8 D_003C9732[];

extern void *func_00101958();

extern s32 func_001979E0(void);

typedef struct EvtRuntimeChild {
    u16 unk00;
    u16 unk02;
    u16 unk04;
    u8 pad06[2];
    union {
        s32 words[8];
        struct {
            u8 pad00[2];
            u16 unk0A;
            u8 pad04[6];
            u16 unk12;
        } f;
    } body; /* 0x08 */
    u8 pad28[4];
    void *payload; /* 0x2C: serialized child data */
    struct EvtRuntimeChild *next; /* 0x30 */
} EvtRuntimeChild;

typedef struct EvtGroupInfo {
    u8 pad00[8];
    char *name; /* 0x08 */
} EvtGroupInfo;

typedef struct EvtRuntimeGroup {
    s32 type;
    u8 value04;
    u8 pad05[3];
    s32 value08;
    u8 pad0C[4];
    struct EvtGroupInfo *info; /* 0x10 */
    u8 pad14[8];
    u16 value1C;
    u8 value1E;
    u8 value1F;
    u8 pad20[0x34];
    EvtRuntimeChild *children;
    u8 pad58[0x24];
    struct EvtRuntimeGroup *next;
} EvtRuntimeGroup;

typedef struct EvtFrameNode {
    u8 pad00[0x30];
    struct EvtFrameNode *next; /* 0x30 */
} EvtFrameNode;

typedef struct EvtFrameList {
    s32 kind;           /* 0x00 */
    u8 pad04[0x4C];
    s32 count;          /* 0x50 */
    EvtFrameNode *head; /* 0x54 */
} EvtFrameList;

typedef struct EvtRuntime {
    u8 pad0000[4];
    u32 flags; /* 0x04 */
    s32 windowContext; /* 0x08 */
    u8 pad000C[0xC];
    s32 curFrame; /* 0x18 */
    u8 pad001C[0x4];
    s32 entryTotal; /* 0x20 */
    char entryName[256][32]; /* 0x24 */
    u8 pad2024[0xC];
    s32 entryCount; /* 0x2030 */
    EvtRuntimeGroup *groups; /* 0x2034 */
    u8 pad2038[0x248];
    s32 mode; /* 0x2280 */
    u8 pad2284[0x8];
    s32 busy; /* 0x228C */
    u8 pad2290[0x18];
    s32 inputA; /* 0x22A8 */
    s32 groupFirst; /* 0x22AC */
    u8 pad22B0[0x4];
    s32 groupCursor; /* 0x22B4 */
    s32 inputB; /* 0x22B8 */
    s32 cursor; /* 0x22BC */
    s32 itemCount; /* 0x22C0 */
    char *title; /* 0x22C4 */
    char **itemNames; /* 0x22C8 */
    s32 charCol; /* 0x22CC */
    s32 charRow; /* 0x22D0 */
    u8 pad22D4[0x20];
    s32 entryCursor; /* 0x22F4 */
    s32 entryFirst; /* 0x22F8 */
    u8 pad22FC[0x4];
    s32 frameFirst; /* 0x2300 */
    s32 frameCursor; /* 0x2304 */
    EvtFrameList *frameList; /* 0x2308 */
    u8 pad230C[0x4];
    s32 value; /* 0x2310 */
    s32 valueMin; /* 0x2314 */
    s32 valueMax; /* 0x2318 */
    f32 fvalue; /* 0x231C */
    f32 fvalueMin; /* 0x2320 */
    f32 fvalueMax; /* 0x2324 */
    u8 pad2328[0x6C];
    s32 cutSel; /* 0x2394 */
    u8 pad2398[0x30];
    s32 tableColumn; /* 0x23C8: index within selected table row */
    u8 pad23CC[0x14];
    s32 selected; /* 0x23E0 */
    s32 frames; /* 0x23E4 */
    char *text0; /* 0x23E8 */
    char *text1; /* 0x23EC */
    u8 pad23F0[0x24];
    s32 timedActive; /* 0x2414 */
    u8 pad2418[0x8];
    u8 shadowMode; /* 0x2420 */
    u8 shadowAlpha; /* 0x2421 */
    u8 pad2422[0x2];
    f32 shadowY; /* 0x2424 */
    s32 pendingWork; /* 0x2428 */
    s32 pendingResource; /* 0x242C */
} EvtRuntime;

typedef struct GsSurface {
    u8 pad00[0x10];
    void (*submit)(struct GsSurface *, s32);
} GsSurface;

typedef s32 (*EvtMenuHeaderFn)(s32 list, s32 x, s32 y, EvtRuntime *ctx);
typedef void (*EvtMenuRowFn)(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx);

typedef struct EvtPad {
    u8 pad00[0x20];
    s8 syncKey; /* 0x20 */
    s8 confirm; /* 0x21 */
    u8 pad22;
    s8 cancel;  /* 0x23 */
    u8 decOne;  /* 0x24 */
    u8 incOne;  /* 0x25 */
    u8 decTen;  /* 0x26 */
    u8 incTen;  /* 0x27 */
    u8 decHun;  /* 0x28 */
    u8 pad29;
    u8 incHun;  /* 0x2A */
    u8 pad2B;
    s8 apply;   /* 0x2C */
} EvtPad;

extern EvtPad D_0037F510;
extern GsSurface D_00380748;
extern char D_004233F0[]; /* " RR  = ENTER" */
extern char D_00423400[]; /* " RD  = CANCEL" */
extern char D_00423428[]; /* " L,R = VALUE-+" */
extern char D_004374A0[]; /* "     %d" */
extern s32 sdfCreateResetPacketList(void);
extern void func_00250338(s32 list, s32 x, s32 y, s32 col, s32 rows, s32 first, s32 total, EvtRuntime *ctx,
                          EvtMenuHeaderFn header, EvtMenuRowFn row);
extern void func_00103790(s32 list, s32 x, s32 y, s32 w, s32 h);
extern s32 func_001036B0(s32, s32, s32, s32, s32, s32, s32 *, s32, s32 *);

extern void func_003421E8(s32 arg0);

extern s32 func_00342168(s32 arg0);

extern void sndStartTrackDefault(s32 arg0);

extern void func_0035B6E0(char *fmt, ...);

extern void sndStartTrackExtended(s32 arg0);

extern void func_00342580(u32 arg0);

extern void sndSetSequenceVolumePan(s32 arg0, s32 arg1, s32 arg2);

extern void func_00341C78(u32 arg0);

extern s32 (*D_003C9928[])(s32, s32, void *);

extern char D_004373C0[];
extern void func_0024FF80();
extern void func_0024FFC8();
extern u8 *evtAllocateContext();
extern void evtSetConvertedContextValue();

typedef struct EvtTaskData {
    u32 pad00;
    s32 value; /* 0x04 */
    u8 pad08[0x30];
    s32 effectHandle; /* 0x38: released before an updated effect is installed */
} EvtTaskData;

/* Create a task with an initialized event payload. */
void evtCreateTask(s32 taskId, s32 value) {
    s32 taskData = (s32)evtAllocateContext();
    evtSetConvertedContextValue(taskData, value);
    kwlnTaskCreate(D_004373C0, taskId, 1, 1, (s32)func_0024FF80, (s32)func_0024FFC8, taskData);
}

extern s32 kwlnTaskCreate(char *name, s32 arg1, s32 arg2, s32 arg3, s32 update, s32 destroy, s32 data);
extern char D_004373C0[];
extern void func_0024FF80();
extern void func_0024FFC8();
extern u8 *evtAllocateContext();

void evtCreateTaskWithValue(s32 taskId, s32 value) {
    EvtTaskData *taskData = (EvtTaskData *)evtAllocateContext();
    taskData->value = value;
    kwlnTaskCreate(D_004373C0, taskId, 1, 1, (s32)func_0024FF80, (s32)func_0024FFC8, (s32)taskData);
}

void evtSetSkyOverlayEnabled(u32 enabled) {
    D_004373CC = enabled;
}

/* Either set the sky alpha immediately or interpolate from its current value. */
void evtBeginSkyParameterTransition(s32 duration, s32 target) {
    s16 current;

    current = func_00135598();
    if (current != target) {
        if (duration == 0) {
            func_00135588(target);
            D_004373C8 = 0;
        } else {
            D_00438FB2 = duration;
            D_00438FB4 = current;
            D_00438FB6 = target;
            D_004373C8 = 1;
            D_00438FB0 = 0;
        }
    }
}

u16 evtIsSkyTransitionActive(void) {
    return D_004373C8;
}

void evtAdvanceSkyTransition(void) {
    if (D_004373C8 != 0) {
        D_00438FB0 += 1;
        func_00135588(D_00438FB4 + (s32)((f32)(D_00438FB6 - D_00438FB4) * ((f32)D_00438FB0 / (f32)D_00438FB2)));
        if (D_00438FB0 >= D_00438FB2) {
            D_004373C8 = 0;
        }
    }
}

s32 evtUpdateSkyTask(void) {
    evtAdvanceSkyTransition();
    func_00134A18();
    if (D_004373CC != 0) {
        func_0012BC38(0x53);
        func_0012D3E0();
    }
    return 0;
}

void evtResetSkyTaskFlags(void) {
    D_004373C8 = 0;
    D_004373CC = 0;
}

extern char D_004373D0[];

void evtDestroySkyTask(void) {
    s32 task = func_00101740(D_004373D0);
    if (task != 0) {
        kwlnTaskDestroyWithHierarchy(task, 1);
    }
}

extern void fldSetSwayMode();
extern void func_00135578();
extern void func_00135588();
extern void fldSetFadeTarget();

void evtCreateSkyTask(void) {
    fldSetSwayMode(0);
    func_00135578(0x80);
    func_00135588(0);
    fldSetFadeTarget(0, 1, 0);
    kwlnTaskCreate(D_004373D0, 0x2B0E, 1, 1, (s32)evtUpdateSkyTask, (s32)evtResetSkyTaskFlags, 0);
}

/* Parked: build/parked/dds2/game/code_00250010/func_002502E0.c (needs a 4-word
   stack local; an unused buffer matches but is a codegen lever, not source). */
INCLUDE_ASM(const s32, "game/code_00250010", func_002502E0);

extern s32 kwlnTaskCreate(char *name, s32 arg1, s32 arg2, s32 arg3, s32 update, s32 destroy, s32 data);
extern s32 func_002502E0();
extern char D_00423380[]; /* "FrameVar" */

void evtCreateFrameVariableTask(void) {
    kwlnTaskCreate(D_00423380, 0x2AF9, 1, 1, (s32)func_002502E0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00250338);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423380);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423390);

s32 func_00250508(s32 list, s32 x, s32 y) {
    sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, 0, "VALUE CHANGE."));
    return 2;
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00250558);

extern s32 func_00250558(s32, s32, s32, s32, EvtRuntime *);
s32 func_00250718(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;
    f32 step;

    list = sdfCreateResetPacketList();
    func_00250338(list, x, y, 0x16, 9, 0, 1, ctx, func_00250508, func_00250558);
    D_00380748.submit(&D_00380748, list);
    if (ctx->mode != 8) {
        return 0;
    }
    if (D_0037F510.confirm < 0) {
        return 1;
    }
    if (D_0037F510.cancel < 0) {
        return -1;
    }
    step = 0.0f;
    if (D_0037F510.decOne & 2) {
        step = -0.1f;
    } else if (D_0037F510.incOne & 2) {
        step = 0.1f;
    }
    if (D_0037F510.decTen & 2) {
        step = -1.0f;
    } else if (D_0037F510.incTen & 2) {
        step = 1.0f;
    }
    ctx->fvalue += step;
    if (ctx->fvalue < ctx->fvalueMin) {
        ctx->fvalue = ctx->fvalueMin;
    }
    if (ctx->fvalue >= ctx->fvalueMax) {
        ctx->fvalue = ctx->fvalueMax;
    }
    return 0;
}

s32 func_00250880(s32 list, s32 x, s32 y) {
    sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, 0, "VALUE CHANGE."));
    return 2;
}

INCLUDE_RODATA(const s32, "game/code_00250010", D_004233F0);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423400);

INCLUDE_RODATA(const s32, "game/code_00250010", jtbl_00423410);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423428);

void func_002508D0(s32 list, s32 x, s32 y, u32 index, EvtRuntime *ctx) {
    switch (index) {
    case 0:
        sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, 4, D_004374A0, ctx->value));
        return;
    case 2:
        sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, 0, D_00423428));
        return;
    case 3:
        sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, 0, " U,D = VALUE-+10"));
        return;
    case 4:
        sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, 0, D_004233F0));
        return;
    case 5:
        sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, 0, D_00423400));
        break;
    }
}

extern s32 func_00250880(s32 list, s32 x, s32 y);
extern void func_002508D0(s32 list, s32 x, s32 y, u32 index, EvtRuntime *ctx);

s32 func_00250A08(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;
    s32 step;

    list = sdfCreateResetPacketList();
    func_00250338(list, x, y, 0x16, 9, 0, 1, ctx, func_00250880, func_002508D0);
    D_00380748.submit(&D_00380748, list);
    if (ctx->mode != 7) {
        return 0;
    }
    if (D_0037F510.confirm < 0) {
        return 1;
    }
    if (D_0037F510.cancel < 0) {
        return -1;
    }
    if (D_0037F510.decOne & 2) {
        step = -1;
    } else if (D_0037F510.incOne & 2) {
        step = 1;
    } else {
        step = 0;
    }
    if (D_0037F510.decTen & 2) {
        step = -10;
    } else if (D_0037F510.incTen & 2) {
        step = 10;
    }
    ctx->value += step;
    if (ctx->value < ctx->valueMin) {
        ctx->value = ctx->valueMin;
    }
    if (ctx->value >= ctx->valueMax) {
        ctx->value = ctx->valueMax;
    }
    return 0;
}

s32 mnuDrawFrameChangeLabel(s32 target, s32 x, s32 y) {
    sdfAppendPacket(target, func_0033D810(x, y, 0xFEFFFF, 0, "FRAME CHANGE."));
    return 2;
}

void func_00250BA0(s32 list, s32 x, s32 y, u32 index, EvtRuntime *ctx) {
    switch (index) {
    case 0:
        sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, 4, D_004374A0, ctx->value));
        return;
    case 2:
        sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, 0, " L,R = FRMAE-+"));
        return;
    case 3:
        sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, 0, " U,D = FRAME-+10"));
        return;
    case 4:
        sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, 0, "L1,R1= FRAME-+100"));
        return;
    case 5:
        sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, 0, D_004233F0));
        return;
    case 6:
        sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, 0, D_00423400));
        return;
    case 7:
        sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, 0, " RL  = NOW FRAME"));
        return;
    case 8:
        sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, 0, " ST  = CAMERA FOCUS"));
        break;
    case 9:
        break;
    }
}

extern void evtViewerDispatchFlagMode();
extern void func_00249088();
extern void func_00250BA0(s32 list, s32 x, s32 y, u32 index, EvtRuntime *ctx);

s32 func_00250D68(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;
    s32 step;

    list = sdfCreateResetPacketList();
    func_00250338(list, x, y, 0x16, 0xB, 0, 1, ctx, mnuDrawFrameChangeLabel, func_00250BA0);
    D_00380748.submit(&D_00380748, list);
    if (ctx->mode != 6) {
        return 0;
    }
    if (D_0037F510.confirm < 0) {
        return 1;
    }
    if (D_0037F510.cancel < 0) {
        return -1;
    }
    if (D_0037F510.decOne & 2) {
        step = -1;
    } else if (D_0037F510.incOne & 2) {
        step = 1;
    } else {
        step = 0;
    }
    if (D_0037F510.decTen & 2) {
        step = -10;
    } else if (D_0037F510.incTen & 2) {
        step = 10;
    }
    if (D_0037F510.decHun & 2) {
        step = -100;
    } else if (D_0037F510.incHun & 2) {
        step = 100;
    }
    if (D_0037F510.syncKey < 0) {
        step = ctx->curFrame - ctx->value;
    }
    ctx->value += step;
    if (ctx->value < ctx->valueMin) {
        ctx->value = ctx->valueMin;
    }
    if (ctx->value >= ctx->valueMax) {
        ctx->value = ctx->valueMax;
    }
    if (D_0037F510.apply != 0) {
        if (ctx->curFrame != ctx->value) {
            ctx->curFrame = ctx->value;
            evtViewerDispatchFlagMode(ctx, step, &D_0037F510);
            func_00249088(ctx->curFrame, ctx);
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00250F20);

extern void func_00250F20(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx);

s32 mnuDrawInfoWindowA(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;

    list = sdfCreateResetPacketList();
    func_00250338(list, x, y, 0xF, 0xB, 0, 0xB, ctx, NULL, func_00250F20);
    D_00380748.submit(&D_00380748, list);
    if (ctx->mode != 1) {
        return 0;
    }
    return func_001036B0(0, 1, 0xB, 1, 0xB, 0, 0, 0, &ctx->inputA);
}

extern char D_004374D0[];

s32 evtDrawStringEntry(s32 list, s32 x, s32 y, EvtRuntime *ctx) {
    if (ctx->title == NULL) {
        return 0;
    }
    sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, 0, D_004374D0, ctx->title));
    return 2;
}

extern char D_004374D8[]; /* " %s" */

void func_002512B0(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
    if (index < ctx->itemCount) {
        s32 color;

        if (ctx->cursor == index) {
            if (ctx->mode == 2) {
                color = 4;
            } else {
                color = 5;
            }
        } else {
            color = 0;
        }
        sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, color, D_004374D8, ctx->itemNames[index]));
    }
}

extern s32 strlen(const char *s);

s32 func_00251340(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;
    s32 width;
    s32 len;

    list = sdfCreateResetPacketList();
    width = 10;
    if (ctx->title != NULL) {
        len = strlen(ctx->title);
        width = len;
        if (len < 6) {
            width = 6;
        }
    }
    func_00250338(list, x, y, width, ctx->itemCount + 3, 0, ctx->itemCount, ctx, evtDrawStringEntry, func_002512B0);
    D_00380748.submit(&D_00380748, list);
    if (ctx->mode != 2) {
        return 0;
    }
    return func_001036B0(0, 1, ctx->itemCount, 1, ctx->itemCount, 0, 0, 0, &ctx->cursor);
}

extern char D_004374E0[];
extern char D_004374D0[];

s32 func_00251440(s32 list, s32 x, s32 y, u8 *ctx) {
    char text[16];
    func_0035C860(text, D_004374E0, (s32)ctx + 0x22D4, (s32)ctx + 0x22E0);
    sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, 0, D_004374D0, text));
    return 2;
}

extern s8 D_003C9688[];
extern char D_004374E8[]; /* "%c" */

void func_002514C8(s32 list, s32 x, s32 y, s32 row, EvtRuntime *ctx) {
    s32 i;
    s32 color;

    for (i = 0; i < 11; i++) {
        color = 0;
        if (ctx->charRow == row && ctx->charCol == i) {
            if (ctx->mode == 3) {
                color = 4;
            } else {
                color = 5;
            }
        }
        sdfAppendPacket(list, func_0033D810(x + (i + 1) * 0xC0, y, 0xFEFFFF, color, D_004374E8, D_003C9688[row * 12 + i]));
    }
}

INCLUDE_ASM(const s32, "game/code_00250010", func_002515C8);

extern u16 D_004372B0;
extern u16 D_004372B2;

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423668);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423678);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423688);

s32 func_002517E0(s32 list, s32 x, s32 y) {
    char text[32];
    func_0035C860(text, "[E%3d_%03d.PM1+2+3]", D_004372B0, D_004372B2);
    sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, 0, D_004374D0, text));
    return 2;
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00251868);

INCLUDE_ASM(const s32, "game/code_00250010", func_00251B20);

extern char D_00437510[]; /* "NAME:" */

void func_00251DE8(s32 list, s32 *sel, s32 x, s32 unused, u8 *base) {
    x += 0x6C0;
    func_00103790(list, (x - 0x7000) >> 4, 0x3C, 0x1C, 3);
    sdfAppendPacket(list, func_0033D810(x, 0x7AE0, 0xFEFFFF, 0xE, D_00437510));
    if (sel[2] >= 0) {
        sdfAppendPacket(list, func_0033D810(x + 0x3C0, 0x7AE0, 0xFEFFFF, 0, D_004374D0, base + sel[2] * 32 + 0x24));
    }
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00251ED0);

extern void func_00251ED0(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx);

s32 func_002520E8(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;
    s32 count;
    s32 shown;

    list = sdfCreateResetPacketList();
    func_00250338(list, x, y, 8, 0x1D, ctx->entryFirst, ctx->entryCount, ctx, NULL, func_00251ED0);
    D_00380748.submit(&D_00380748, list);
    if (ctx->mode != 4) {
        return 0;
    }
    count = ctx->entryCount;
    if (count == 0) {
        return 0;
    }
    shown = 0x1D;
    if (count < 0x1D) {
        shown = count;
    }
    return func_001036B0(0, 1, count, 1, shown, 0, &ctx->entryFirst, 0, &ctx->entryCursor);
}

INCLUDE_ASM(const s32, "game/code_00250010", func_002521C8);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423AE0);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423AF0);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423B00);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423B10);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423B20);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423B30);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423B40);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423B50);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423B60);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423B70);

INCLUDE_ASM(const s32, "game/code_00250010", func_00252378);

extern void func_00252378(s32 list, s32 x, s32 y, s32 color, EvtFrameNode *node, EvtRuntime *ctx);

s32 func_002534B0(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
    EvtFrameNode *node;
    s32 color;
    s32 i;

    if (index < ctx->frameList->count + 1) {
        node = NULL;
        if (index != ctx->frameList->count) {
            node = ctx->frameList->head;
            for (i = 0; i < index; i++) {
                node = node->next;
            }
        }
        if (ctx->frameCursor + ctx->frameFirst == index) {
            if (ctx->mode == 5) {
                color = 4;
            } else {
                color = 5;
            }
        } else {
            color = 0;
        }
        if (node != NULL) {
            func_00252378(list, x, y, color, node, ctx);
        } else {
            sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, color ? color : 8, "----- NEW FRAME -----"));
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00253590);

typedef struct EvtWorldNode {
    u8 pad00[8];
    char *name; /* 0x08 */
    u8 pad0C[0x14];
    struct EvtWorldNode *next; /* 0x20 */
} EvtWorldNode;

typedef struct EvtWorldSlot {
    u8 pad00[4];
    EvtWorldNode *head; /* 0x04 */
    u8 pad08[4];
} EvtWorldSlot; /* 0xC bytes */

typedef struct EvtWorldTable {
    u8 pad00[8];
    EvtWorldSlot *slots; /* 0x08 */
} EvtWorldTable;

typedef struct EvtWorldObject {
    u8 pad00[0x18];
    EvtWorldTable *table; /* 0x18 */
} EvtWorldObject;

extern EvtWorldObject *dds3GetWorldObject();
extern char D_004374D8[]; /* " %s" */

void func_002537A8(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
    s32 color;
    s32 count;
    s32 i;
    EvtWorldNode *node;

    color = 4;
    if (ctx->groupFirst + ctx->groupCursor != index) {
        color = 0;
    }
    if (index == 0) {
        sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, color, " -----------------------"));
        return;
    }
    count = 0;
    for (i = 0; i < 0x12; i++) {
        if (i != 3) {
            for (node = dds3GetWorldObject()->table->slots[i].head; node != NULL; node = node->next) {
                if (node->name != NULL) {
                    count++;
                    if (count == index) {
                        sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, color, D_004374D8, node->name));
                        return;
                    }
                }
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00253938);

extern char D_004376A8[]; /* "P%d:" */
extern char D_004376B0[]; /* "   %s" */
extern s32 evtEventViewerGetPendingNode();
extern EvtWorldNode *func_001111A8(EvtWorldObject *world, char *name);

/* The pending-node's signed slot indices begin at +0xC (also used in DDS1). */
void func_00253A98(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
    EvtWorldNode *node;
    s32 color;
    s32 slot;

    node = NULL;
    color = 4;
    if (ctx->inputB != index) {
        color = 0;
    }
    slot = *(s8 *)(index + evtEventViewerGetPendingNode(ctx) + 0xC);
    if (slot >= 0) {
        node = func_001111A8(dds3GetWorldObject(), ctx->entryName[slot]);
    }
    sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, 0xE, D_004376A8, index));
    if (node != NULL) {
        sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, color, D_004376B0, node->name));
    } else {
        sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, color, "   -----------------------"));
    }
}

extern void func_00253A98(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx);

s32 mnuDrawInfoWindowB(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;
    s32 rows;

    list = sdfCreateResetPacketList();
    switch (ctx->frameList->kind) {
    case 0x14:
        rows = 2;
        break;
    case 0x15:
        rows = 4;
        break;
    default:
        return -1;
    }
    func_00250338(list, x, y, 0x1C, rows, 0, rows, ctx, NULL, func_00253A98);
    D_00380748.submit(&D_00380748, list);
    if (ctx->mode != 0xC) {
        return 0;
    }
    return func_001036B0(0, 1, rows, 1, rows, 0, 0, 0, &ctx->inputB);
}

extern char D_00423EC0[]; /* "MESSAGE MENU (MESMAX %3d)" */

/* The message-menu work references a window whose entry handle lives at +0x104. */
typedef struct EvtMessageWindow {
    u8 pad00[0x104];
    s32 entryHandle;
} EvtMessageWindow;

typedef struct EvtMessageMenuWork {
    u8 pad00[8];
    EvtMessageWindow *window;
} EvtMessageMenuWork;

s32 mnuDrawMessageMenuLabel(s32 list, s32 x, s32 y, EvtMessageMenuWork *ctx) {
    sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, 0, D_00423EC0, itfMesGetEntryCount(ctx->window->entryHandle)));
    return 2;
}

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423EC0);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423EE0);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423EF0);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423F00);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423F10);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423F20);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423F30);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423F40);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423F50);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423F60);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423F70);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423F80);

INCLUDE_ASM(const s32, "game/code_00250010", func_00253D80);

INCLUDE_ASM(const s32, "game/code_00250010", func_00253FF8);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423FE0);

INCLUDE_RODATA(const s32, "game/code_00250010", jtbl_00423FF0);

s32 mnuDrawCutFlagLabel(s32 target, s32 x, s32 y) {
    sdfAppendPacket(target, func_0033D810(x, y, 0xFEFFFF, 0, "CUTFLAG MENU"));
    return 2;
}

INCLUDE_RODATA(const s32, "game/code_00250010", D_00424020);

INCLUDE_ASM(const s32, "game/code_00250010", func_00254250);

INCLUDE_ASM(const s32, "game/code_00250010", func_00254458);

s32 func_00254620(s32 *index) {
    return D_003C9730[*index].unk0 != 0;
}

s32 mnuGetSelectedTableValue(EvtRuntime *runtime) {
    return D_003C9732[runtime->tableColumn + runtime->frameList->kind * 10];
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00254678);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00424080);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00424090);

INCLUDE_ASM(const s32, "game/code_00250010", func_00254940);

s32 mnuDrawMotionChangeLabel(s32 target, s32 x, s32 y) {
    sdfAppendPacket(target, func_0033D810(x, y, 0xFEFFFF, 0, "MOTION CHANGE MENU"));
    return 2;
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00254CE0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00254F80);

extern char D_00424090[]; /* "UNIT ALL" */
extern char D_004376E8[]; /* "ALL" */
extern char D_004376F0[]; /* "DISABLE" */

void func_00255360(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
    EvtRuntimeGroup *group;
    s32 color;
    s32 n;

    color = 4;
    if (ctx->groupFirst + ctx->groupCursor != index) {
        color = 0;
    }
    if (index == 0) {
        sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, color, D_004376F0));
    } else if (index == 1) {
        sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, color, D_004376E8));
        return;
    } else if (index == 2) {
        sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, color, D_00424090));
        return;
    }
    n = 3;
    for (group = ctx->groups; group != NULL; group = group->next) {
        if (group->type == 1) {
            if (n == index) {
                sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, color, D_004374D0, group->info->name));
                return;
            }
            n++;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00255538);

void func_00255648(void) {
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00255650);

INCLUDE_ASM(const s32, "game/code_00250010", func_00255818);

void func_002560A8(void) {
}

INCLUDE_ASM(const s32, "game/code_00250010", func_002560B0);

void func_002566F8(s32 list, s32 x, s32 y, s32 kind, EvtRuntime *ctx) {
    switch (kind) {
    case 0:
        if (ctx->text0 != NULL) {
            sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, 0, D_004374D0, ctx->text0));
            return;
        }
        break;
    case 1:
        if (ctx->text1 != NULL) {
            sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, 0, D_004374D0, ctx->text1));
        }
        break;
    }
}

extern void func_002566F8(s32 list, s32 x, s32 y, s32 kind, EvtRuntime *ctx);

s32 mnuDrawTimedPrompt(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;

    list = sdfCreateResetPacketList();
    func_00250338(list, x, y, 0x19, 2, 0, 1, ctx, NULL, func_002566F8);
    D_00380748.submit(&D_00380748, list);
    if (ctx->mode != 0x14) {
        return 0;
    }
    if (ctx->frames > 0) {
        ctx->frames--;
    } else if (ctx->frames == 0) {
        return -1;
    }
    if (D_0037F510.confirm < 0) {
        return 1;
    }
    if (D_0037F510.cancel >= 0) {
        return 0;
    }
    return -1;
}

void evtSetRuntimeCommandValues(EvtRuntime *runtime, s32 frames, char *firstText, char *secondText) {
    runtime->frames = frames;
    runtime->text0 = firstText;
    runtime->text1 = secondText;
}

extern char D_00437768[]; /* "CURRENT" */

INCLUDE_RODATA(const s32, "game/code_00250010", D_004241A0);

void func_00256898(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
    EvtRuntimeGroup *group;
    s32 color;
    s32 n;

    color = 4;
    if (ctx->groupFirst + ctx->groupCursor != index) {
        color = 0;
    }
    if (index == 0) {
        sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, color, D_00437768));
        return;
    }
    if (index == 1) {
        sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, color, "DEFFAULT"));
        return;
    }
    n = 2;
    for (group = ctx->groups; group != NULL; group = group->next) {
        if (group->type == 0x18) {
            if (n == index) {
                sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, color, D_004374D0, ctx->entryName[group->value08]));
                return;
            }
            n++;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00250010", func_002569D0);

INCLUDE_RODATA(const s32, "game/code_00250010", D_004241C0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00256AE0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00256CF0);

extern s32 func_00197AE8();

extern s32 kwlnTaskGetTimer(s32 task);

typedef struct EvtSelectionCache {
    u8 pad00[0x24];
    s32 selected; /* 0x24 */
} EvtSelectionCache;

extern EvtSelectionCache *D_00436518;

extern s32 D_00438FB8;

s32 evtSynchronizeSelectedEntry(s32 task) {
    EvtRuntime *runtime = (EvtRuntime *)func_00101958(task);

    if (func_00197AE8() == 0) {
        runtime->busy = 0;
        return -1;
    }
    if (kwlnTaskGetTimer(task) == 0) {
        s32 selected = runtime->selected;

        D_00438FB8 = selected;
        if (selected != 0) {
            D_00436518->selected = selected;
        }
    }
    if (D_00436518->selected != runtime->selected) {
        s32 selected = runtime->selected;

        if (selected != 0) {
            D_00436518->selected = selected;
        }
    }
    return 0;
}

s32 func_002570B8(void) {
    void *runtime;

    runtime = func_00101958();
    if (func_001979E0() == 0) {
        ((EvtRuntime *)runtime)->busy = 0;
        return -1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00250010", D_00424210);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00424220);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00424230);

INCLUDE_ASM(const s32, "game/code_00250010", func_002570F8);

INCLUDE_ASM(const s32, "game/code_00250010", func_00257910);

INCLUDE_ASM(const s32, "game/code_00250010", func_002582D0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00258700);

s32 evtDispatchActionByIndex(s32 index, s32 x, s32 y, void *runtime) {
    s32 mode = ((EvtRuntime *)runtime)->mode;
    if (mode == 11 && index != mode) {
        return 0;
    }
    return D_003C9928[index](x, y, runtime);
}

void func_002588A0(s32 output, s32 data, s32 size) {
    func_0036A420();
}

s32 func_002588B8(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xA) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.unk12 = index++;
            }
        }
    }
    return index;
}

s32 func_00258920(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xB) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->unk02 = index++;
            }
        }
    }
    return index;
}

s32 func_00258988(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xD) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.unk0A = index++;
            }
        }
    }
    return index;
}

s32 func_002589F0(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xE) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.unk0A = index++;
            }
        }
    }
    return index;
}

s32 func_00258A58(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xF) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.unk0A = index++;
            }
        }
    }
    return index;
}

s32 func_00258AC0(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x17) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.unk0A = index++;
            }
        }
    }
    return index;
}

s32 func_00258B28(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x1B) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.unk0A = index++;
            }
        }
    }
    return index;
}

s32 func_00258B90(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x10) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.unk0A = index++;
            }
        }
    }
    return index;
}

s32 func_00258BF8(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x11) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.unk0A = index++;
            }
        }
    }
    return index;
}

s32 func_00258C60(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x19) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.unk0A = index++;
            }
        }
    }
    return index;
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00258CC8);

/* Header fields precede 0x20-byte records; trailing metadata is at +0x243C. */
typedef struct EvtSerializedState {
    u8 pad00[0xC];
    s32 third; /* 0x0C */
    s32 first; /* 0x10 */
    s32 second; /* 0x14 */
    u8 pad18[8];
    s32 count; /* 0x20 */
    u8 records[0x20]; /* 0x24: first serialized record */
    u8 pad44[0x23F8];
    s32 metadata; /* 0x243C */
} EvtSerializedState;

void evtWriteRuntimeHeaderValues(s32 output, EvtSerializedState *state) {
    s32 buffer[4];
    buffer[0] = state->first;
    buffer[1] = state->second;
    buffer[2] = state->third;
    buffer[3] = state->metadata;
    func_002588A0(output, buffer, 0x10);
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00259298);

void evtWriteFixedSizeEntries(s32 output, EvtSerializedState *table) {
    s32 i;
    u8 *entry;
    i = 0;
    if (table->count > 0) {
        entry = table->records;
        do {
            func_002588A0(output, entry, 0x20);
            entry += 0x20;
            i++;
        } while (i < table->count);
    }
}

void evtWriteGroupHeader(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    s32 header[4];
    for (group = runtime->groups; group != 0; group = group->next) {
        if (group->type == 2) {
            header[0] = group->value08;
            header[1] = 0;
            header[2] = 0;
            header[3] = 0;
            func_002588A0(output, header, 0x10);
        }
    }
}

void func_00259518(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    EvtRuntimeChild *child;
    for (group = runtime->groups; group != 0; group = group->next) {
        if (group->type == 0xA) {
            for (child = group->children; child != 0; child = child->next) {
                func_002588A0(output, (s32)child->payload, 0x30);
            }
        }
    }
}

void func_002595A0(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    EvtRuntimeChild *child;
    for (group = runtime->groups; group != 0; group = group->next) {
        if (group->type == 0xB) {
            for (child = group->children; child != 0; child = child->next) {
                func_002588A0(output, (s32)child->payload, 0x20);
            }
        }
    }
}

void func_00259628(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    EvtRuntimeChild *child;
    for (group = runtime->groups; group != 0; group = group->next) {
        if (group->type == 0xD) {
            for (child = group->children; child != 0; child = child->next) {
                func_002588A0(output, (s32)child->payload, 0x28);
            }
        }
    }
}

void func_002596B0(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    EvtRuntimeChild *child;
    for (group = runtime->groups; group != 0; group = group->next) {
        if (group->type == 0xE) {
            for (child = group->children; child != 0; child = child->next) {
                func_002588A0(output, (s32)child->payload, 0x2C);
            }
        }
    }
}

void func_00259738(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    EvtRuntimeChild *child;
    for (group = runtime->groups; group != 0; group = group->next) {
        if (group->type == 0xF) {
            for (child = group->children; child != 0; child = child->next) {
                func_002588A0(output, (s32)child->payload, 0x2C);
            }
        }
    }
}

void func_002597C0(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    EvtRuntimeChild *child;
    for (group = runtime->groups; group != 0; group = group->next) {
        if (group->type == 0x17) {
            for (child = group->children; child != 0; child = child->next) {
                func_002588A0(output, (s32)child->payload, 0x2C);
            }
        }
    }
}

void func_00259848(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    EvtRuntimeChild *child;
    for (group = runtime->groups; group != 0; group = group->next) {
        if (group->type == 0x1B) {
            for (child = group->children; child != 0; child = child->next) {
                func_002588A0(output, (s32)child->payload, 0x28);
            }
        }
    }
}

void func_002598D0(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    EvtRuntimeChild *child;
    for (group = runtime->groups; group != 0; group = group->next) {
        if (group->type == 0x10) {
            for (child = group->children; child != 0; child = child->next) {
                func_002588A0(output, (s32)child->payload, 0x18);
            }
        }
    }
}

void func_00259958(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    EvtRuntimeChild *child;
    for (group = runtime->groups; group != 0; group = group->next) {
        if (group->type == 0x11) {
            for (child = group->children; child != 0; child = child->next) {
                func_002588A0(output, (s32)child->payload, 0x24);
            }
        }
    }
}

/* On-disk group header: two bytes followed by two little-endian halfwords. */
typedef struct EvtGroupMetadata {
    u8 type;
    u8 flag;
    u16 entry;
    u16 value;
    u8 extra1;
    u8 extra2;
} EvtGroupMetadata;

void evtWriteGroupMetadata(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    EvtGroupMetadata record;
    for (group = runtime->groups; group != 0; group = group->next) {
        record.type = *(u8 *)&group->type;
        record.flag = group->value04;
        record.entry = *(u16 *)&group->value08;
        record.value = group->value1C;
        record.extra1 = group->value1E;
        record.extra2 = group->value1F;
        func_002588A0(output, &record, 8);
    }
}

void func_00259A60(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    EvtRuntimeChild *child;
    for (group = runtime->groups; group != 0; group = group->next) {
        if (group->type == 0x19) {
            for (child = group->children; child != 0; child = child->next) {
                func_002588A0(output, (s32)child->payload, 0x40);
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00259AE8);

/* The same table has compact 0x10-byte rows or extended 0x2C-byte rows. */
typedef struct EvtRowTable {
    u8 pad00[0x74];
    s32 descriptor;    /* 0x74: row format lives at descriptor + 0x14 */
    u8 pad78[0x10];
    s32 compactRows;   /* 0x88 */
    s32 extendedRows;  /* 0x8C */
} EvtRowTable;

typedef struct EvtRowDescriptor {
    u8 pad00[0x14];
    s32 format;
} EvtRowDescriptor;

typedef struct EvtCompactRow {
    u16 value;
    u16 parameter;
    u16 flags;
    s16 variant;
    u8 pad08[8];
} EvtCompactRow;

typedef struct EvtExtendedRow {
    u16 value;
    u16 parameter;
    u16 flags;
    s16 variant;
    u8 pad08[0x24];
} EvtExtendedRow;

u16 func_00259FF8(s32 group, s32 index) {
    if (((EvtRowDescriptor *)((EvtRowTable *)group)->descriptor)->format == 4) {
        return ((EvtCompactRow *)(((EvtRowTable *)group)->compactRows + index * 0x10))->value;
    }
    return ((EvtExtendedRow *)(((EvtRowTable *)group)->extendedRows + index * 0x2c))->value;
}

s16 func_0025A048(s32 group, s32 index) {
    if (((EvtRowDescriptor *)((EvtRowTable *)group)->descriptor)->format == 4) {
        return ((EvtCompactRow *)(((EvtRowTable *)group)->compactRows + index * 0x10))->variant;
    }
    return ((EvtExtendedRow *)(((EvtRowTable *)group)->extendedRows + index * 0x2c))->variant;
}

u16 func_0025A098(s32 group, s32 index) {
    if (((EvtRowDescriptor *)((EvtRowTable *)group)->descriptor)->format == 4) {
        return ((EvtCompactRow *)(((EvtRowTable *)group)->compactRows + index * 0x10))->parameter;
    }
    return ((EvtExtendedRow *)(((EvtRowTable *)group)->extendedRows + index * 0x2c))->parameter;
}

u16 func_0025A0E8(s32 group, s32 index) {
    if (((EvtRowDescriptor *)((EvtRowTable *)group)->descriptor)->format == 4) {
        return ((EvtCompactRow *)(((EvtRowTable *)group)->compactRows + index * 0x10))->flags;
    }
    return ((EvtExtendedRow *)(((EvtRowTable *)group)->extendedRows + index * 0x2c))->flags;
}

s32 func_0025A138(s32 group, s32 index) {
    if (((EvtRowDescriptor *)((EvtRowTable *)group)->descriptor)->format == 4) {
        return ((EvtRowTable *)group)->compactRows + index * 0x10 + 8;
    }
    return ((EvtRowTable *)group)->extendedRows + index * 0x2c + 0xc;
}

typedef struct EvtLinkSource {
    u8 pad00[0x7C];
    char *names; /* 0x7C: 0x20-byte entries */
    u8 pad80[0x78];
    s32 count;   /* 0xF8 */
} EvtLinkSource;

typedef struct EvtLink {
    u8 pad00[6];
    s8 type;  /* 0x06 */
    s8 index; /* 0x07 */
} EvtLink;

extern s32 strcmp(const char *a, const char *b);

void func_0025A188(EvtLinkSource *src, EvtRuntime *runtime, EvtLink *link) {
    s32 i;
    EvtRuntimeGroup *group;

    if (link->type != 3) {
        link->index = 0;
    } else {
        for (i = 0; i < src->count; i++) {
            for (group = runtime->groups; group != NULL; group = group->next) {
                if (strcmp(runtime->entryName[group->value08], src->names + link->index * 32) == 0) {
                    link->index = group->value08;
                    return;
                }
            }
        }
        func_0035B6E0("not found linkslight obj index %s\n", src->names + link->index * 32);
        link->type = 0;
        link->index = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00250010", func_0025A280);

extern void func_0025F5D0(EvtRuntime *runtime);
extern void func_002A7FD0(void);
extern s32 sdfCheckPendingWorkWithInterrupts(void);
extern void func_003298C0(s32 resource);
extern void kwlnTextureReleaseHeldReference(void);
extern u32 D_00435CD4;
extern void func_0024FA48(u16 a, u16 b, char *path0, char *path1, char *path2);
extern s32 sdfPathExists(char *path);
extern void evtEventViewerShutdown(EvtRuntime *runtime);
extern void evtDestroySecondaryWorldNode(void);
extern void evtEventViewerReleaseGroups(EvtRuntime *runtime);
extern void evtEventViewerReset(EvtRuntime *runtime);
extern s32 func_0024FB48(u16 a, u16 b, s32 mode);
extern void func_0025A280(s32 handle, EvtRuntime *runtime);

s32 func_0025CAF8(s32 mode, EvtRuntime *runtime) {
    char path0[0x80];
    char path1[0x80];
    char path2[0x80];
    s32 handle;

    func_0025F5D0(runtime);
    if (runtime->timedActive == 1) {
        func_002A7FD0();
        runtime->timedActive = 0;
    }
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    if (runtime->pendingResource != 0) {
        func_003298C0(runtime->pendingResource);
        runtime->pendingResource = 0;
        runtime->pendingWork = 0;
    }
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    kwlnTextureReleaseHeldReference();
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    D_00435CD4 |= 0x2000000;
    func_0024FA48(D_004372B0, D_004372B2, path0, path1, path2);
    if (mode == 1) {
        if (sdfPathExists(path0) != 1) {
            return 0;
        }
    } else {
        if (sdfPathExists(path0) != 1 || sdfPathExists(path1) != 1) {
            return 0;
        }
    }
    evtEventViewerShutdown(runtime);
    evtDestroySecondaryWorldNode();
    evtEventViewerReleaseGroups(runtime);
    evtEventViewerReset(runtime);
    runtime->flags |= 1;
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    handle = func_0024FB48(D_004372B0, D_004372B2, mode);
    if (handle != 0) {
        runtime->windowContext = handle;
        func_0025A280(handle, runtime);
        return 1;
    }
    return handle;
}

s32 evtEncodeBgmSoundCode(s32 eventId, s32 variation) {
    s32 sequenceId;

    sequenceId = 0xC7;
    if (eventId != 0x31F) {
        sequenceId = eventId - 0x259;
        if (eventId >= 0x320) {
            sequenceId = (eventId < 0x384) ? (eventId - 0x258) : (eventId - 0x29E);
        }
    }
    return ((sequenceId + 0x100) << 0x10) + variation;
}

s32 evtPreloadBgm(s32 id) {
    s32 sound;
    if ((u32)(id - 0x258) >= 0x100) {
        return 0;
    }
    sound = evtEncodeBgmSoundCode(id, 0);
    func_003421E8(sound);
    return sound;
}

s32 evtIsBgmLoaded(s32 id) {
    if ((u32)(id - 0x258) >= 0x100) {
        return 1;
    }
    return func_00342168(evtEncodeBgmSoundCode(id, 0)) == 1;
}

s32 evtPlayBgm(s32 id, s32 fade) {
    s32 sound;
    if ((u32)(id - 0x258) >= 0x100) {
        return 0;
    }
    sound = evtEncodeBgmSoundCode(id, fade);
    func_0035B6E0("Event BGM play :%08X\n", sound);
    sndStartTrackDefault(sound);
    return sound;
}

extern char D_004247F8[]; /* "Event BGM trans :%08X\n" */
extern s32 evtEncodeBgmSoundCode();
extern void func_00342600();

void evtTransitionBgm(s32 id, s32 fade) {
    s32 sound;
    if ((u32)(id - 0x258) < 0x100U) {
        sound = -1;
        if (fade >= 0) {
            sound = evtEncodeBgmSoundCode(id, fade);
        }
        func_0035B6E0(D_004247F8, sound);
        func_00342600(sound);
    }
}

INCLUDE_RODATA(const s32, "game/code_00250010", D_004247F8);

s32 evtFadeInBgm(s32 id, s32 fade) {
    s32 sound;
    if ((u32)(id - 0x258) >= 0x100) {
        return 0;
    }
    sound = evtEncodeBgmSoundCode(id, fade);
    func_0035B6E0("Event BGM fade in play :%08X\n", sound);
    sndStartTrackExtended(sound);
    return sound;
}

s32 func_0025CE68(s32 id, s32 fade) {
    s32 sound;
    if ((u32)(id - 0x258) >= 0x100) {
        return 0;
    }
    sound = evtEncodeBgmSoundCode(id, fade);
    func_00342580(sound);
    return sound;
}

s32 evtSetBgmVolumePan(s32 id, s32 fade) {
    s32 sound;
    if ((u32)(id - 0x258) >= 0x100) {
        return 0;
    }
    sound = evtEncodeBgmSoundCode(id, fade);
    sndSetSequenceVolumePan(sound, 0x7F, 0x3F);
    return sound;
}

s32 func_0025CF00(s32 id, s32 fade) {
    s32 sound;
    if ((u32)(id - 0x258) >= 0x100) {
        return 0;
    }
    sound = evtEncodeBgmSoundCode(id, fade);
    func_00341C78(sound);
    return sound;
}

extern char D_004377D0[];

void evtFormatTaskName(s32 id, char *buffer) {
    func_0035C860(buffer, D_004377D0, id);
}

s32 evtFindTaskById(u32 taskId) {
    u8 taskName[32];

    evtFormatTaskName(taskId, taskName);
    return func_00101740(taskName);
}

s32 evtGetTaskValueWord(u32 taskId) {
    s32 task = evtFindTaskById(taskId);
    if (task == 0) {
        return -1;
    }
    return ((EvtTaskData *)func_00101958(task))->value;
}

void *evtGetTaskData(u32 taskId) {
    s32 task = evtFindTaskById(taskId);
    if (task != 0) {
        return func_00101958(task);
    }
    return (void *)task;
}

typedef struct EvtResEntry {
    u8 pad00[0xC];
    s32 offset; /* 0x0C */
    s32 key;    /* 0x10 */
    u8 pad14[0xC];
} EvtResEntry; /* 0x20 bytes */

typedef struct EvtResHeader {
    u8 pad00[0x10];
    s32 count; /* 0x10 */
} EvtResHeader;

typedef struct EvtResTask {
    s32 unk00;
    s32 type;             /* 0x04 */
    u8 pad08[8];
    s32 base;             /* 0x10 */
    EvtResHeader *header; /* 0x14 */
    EvtResEntry *entries; /* 0x18 */
} EvtResTask;

s32 func_0025D008(u32 id, s32 key) {
    s32 task;
    EvtResTask *data;
    s32 i;

    task = evtFindTaskById(id);
    if (task == 0) {
        return 0;
    }
    data = func_00101958(task);
    if (data->type != 2) {
        return 0;
    }
    for (i = 0; i < data->header->count; i++) {
        if (data->entries[i].key == key) {
            return data->base + data->entries[i].offset;
        }
    }
    return 0;
}

extern void effSetCh72Id();

void evtRefreshTaskData(s32 taskId, s32 key) {
    EvtTaskData *data = evtGetTaskData(taskId);
    s32 resource = func_0025D008(taskId, key);
    s32 handle;
    if (resource != 0) {
        if (data->effectHandle != 0) {
            sdfTexReleaseReferenceViaHandler(data->effectHandle);
            data->effectHandle = 0;
        }
        handle = func_0032C138(resource);
        effSetCh72Id(handle);
        data->effectHandle = handle;
    }
}

/* Party/enemy model table entry (0x270 bytes); only the scale-source field is known here. */
typedef struct Entry270 {
    u8 pad00[0x18];
    f32 unk18;
    u8 pad1C[0x254];
} Entry270;

typedef struct EvtEffectInner {
    u8 pad00[0x2C];
    void *scaledObject; /* 0x2C: object passed to the scale setter */
} EvtEffectInner;

typedef struct EvtEffectObject {
    u8 pad00[0x18];
    EvtEffectInner *inner;
} EvtEffectObject;

extern Entry270 *D_00435DE0;
extern Entry270 *D_00435DF0;
extern void *memset(void *, s32, u32);
extern void effObjSetFlags(void *object, s32 flags);
extern void *func_00115500(void *obj, void *vecA, void *vecB);
extern void func_00197F40(void *target, f32 scale);

void *func_0025D140(s32 taskId, s32 key, s32 index) {
    u8 vecA[16];
    u8 vecB[16];
    void *found;
    void *obj;
    EvtEffectInner *inner;

    memset(vecA, 0, 0x10);
    memset(vecB, 0, 0x10);
    found = (void *)func_0025D008(taskId, key);
    if (found != 0) {
        obj = func_00115500(found, vecA, vecB);
        if (obj != 0) {
            effObjSetFlags(obj, 1);
            if (index >= 0) {
                inner = ((EvtEffectObject *)obj)->inner;
                func_00197F40(inner->scaledObject, D_00435DF0[index].unk18 / D_00435DE0->unk18);
            }
            return obj;
        }
        return obj;
    }
    return found;
}

INCLUDE_SDATA(const s32, "game/code_00250010", D_004373C0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004373C8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004373CC);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004373D0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004373E0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004373E8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004373F0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004373F8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437400);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437408);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437410);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437418);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437420);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437428);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437430);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437438);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437440);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437448);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437450);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437458);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437460);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437468);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437470);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437478);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437480);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437488);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437490);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437498);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374A0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374A8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374B0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374B8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374C0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374C8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374D0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374D8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374E0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374E8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374F0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374F8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437500);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437508);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437510);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437518);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437520);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437528);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437530);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437538);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437540);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437548);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437550);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437558);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437560);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437568);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437570);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437578);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437580);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437588);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437590);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437598);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375A0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375A8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375B0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375B8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375C0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375C8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375D0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375D8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375E0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375E8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375F0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375F8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437600);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437608);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437610);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437618);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437620);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437628);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437630);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437638);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437640);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437648);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437650);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437658);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437660);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437668);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437670);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437678);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437680);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437688);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437690);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437698);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376A0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376A8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376B0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376B8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376C0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376C8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376D8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376E0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376E8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376F0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376F8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437700);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437708);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437710);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437718);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437720);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437728);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437730);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437738);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437740);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437748);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437750);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437758);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437760);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437768);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437770);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437778);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437780);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437788);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437790);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437798);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004377A0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004377A8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004377B0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004377B8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004377C0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004377C8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004377D0);

