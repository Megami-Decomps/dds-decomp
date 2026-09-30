#include "common.h"

extern void func_0023D5B0(s32 output, void *data, s32 size);

extern void *func_00101A70();
extern s32 func_0018FDA8(void);
extern s32 func_002E92C0(s32 sequence);
extern void func_002E9340(s32 sequence);
extern s32 evtUpdateFrameVariableTask(s32 *task);
extern s32 evtAllocateContext(void);
extern void evtSetConvertedContextValue(s32 context, s32 value);
extern void func_002351E0(void);
extern void func_00235228(void);
extern s32 evtFindTaskById();
extern s32 func_003014F0(char *, char *, ...);
extern void evtFormatTaskName(s32 taskId, void *buffer);
extern void kwlnTaskCreate(void *name, s32 taskId, s32, s32, void *update, void *destroy, void *data);
extern void *memset(void *, s32, u32);
extern void effObjSetFlags(void *object, s32 flags);
extern void *func_00115298(void *obj, void *vecA, void *vecB);
extern void func_00190308(void *target, f32 scale);
extern s32 kwlnTaskGetTaskByName(void *name);
extern void kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);
extern void fldSetSwayMode(s32 mode);
extern void func_00132B70(s32 value);
extern void func_00132B80(s32 value);
extern s16 func_00132B90(void);
extern void fldSetFadeTarget(s32 area, s32 value, s32 duration);
extern void func_00132010(void);
extern void func_00129720(s32 id);
extern void func_0012AEB0(void);
extern void sdfAppendPacket(s32 list, s32 packet);
extern void sndStartTrackDefault(s32 track);
extern void sndStartTrackExtended(s32 track);
extern void func_002E9758(s32 sequence);
extern void func_003003F0(char *fmt, ...);
extern void func_002E96D8(u32 sequence);
extern void func_002E8DD0(u32 sequence);
extern void sndSetSequenceVolumePan(s32 sequence, s32 volume, s32 pan);
extern s32 func_002E4960();
extern u32 itfMesGetEntryCount(s32 window);
extern void evtViewerDispatchFlagMode();
extern void func_0022E5A0();
extern u8 D_003BBF80[];
extern u8 D_003BBF90[];
extern u8 D_003BC360[];
extern char D_003BC058[]; /* "     %d" */
extern char D_003BC098[];
extern u16 D_003BD898;
extern u16 D_003BD89A;
extern s16 D_003BD89C;
extern s16 D_003BD89E;


typedef struct EvtRuntimeChild {
    u8 pad00[2];
    u16 unk02;
    u8 pad04[6];
    u16 unk0A;
    u8 pad0C[6];
    u16 unk12;
    u8 pad14[0x18];
    void *payload; /* 0x2C: serialized child data */
    struct EvtRuntimeChild *next; /* 0x30 */
} EvtRuntimeChild;

typedef struct EvtGroupInfo {
    u8 pad00[8];
    char *name; /* 0x08 */
} EvtGroupInfo;

typedef struct EvtRuntimeGroup {
    s32 type;
    u8 metadataFlag;  /* 0x04: included in serialized group metadata */
    u8 pad05[3];
    union {
        s32 word;      /* 0x08: full value for group type 2 */
        u16 shortValue; /* 0x08: truncated value in metadata */
    } entryHeader;
    u8 pad0C[4];
    EvtGroupInfo *info; /* 0x10 */
    u8 pad14[8];
    u16 metadataValue; /* 0x1C */
    u8 metadataByte1;  /* 0x1E */
    u8 metadataByte2;  /* 0x1F */
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
    u8 pad00[4];
    u32 flags; /* 0x04 */
    s32 windowContext; /* 0x08 */
    u8 pad0C[0xC];
    s32 curFrame; /* 0x18 */
    u8 pad1C[4];
    s32 entryTotal; /* 0x20 */
    char entryName[256][32]; /* 0x24 */
    u8 pad2024[0xC];
    s32 entryCount; /* 0x2030 */
    EvtRuntimeGroup *groups; /* 0x2034 */
    u8 pad2038[0x248];
    s32 actionMode; /* 0x2280 */
    u8 pad2284[8];
    s32 controlState; /* 0x228C */
    u8 pad2290[0x1C];
    s32 groupFirst; /* 0x22AC */
    u8 pad22B0[4];
    s32 groupCursor; /* 0x22B4 */
    s32 inputB; /* 0x22B8 */
    u8 pad22BC[0x38];
    s32 entryCursor; /* 0x22F4 */
    s32 entryFirst;  /* 0x22F8 */
    u8 pad22FC[0x4];
    s32 frameFirst; /* 0x2300 */
    s32 frameCursor; /* 0x2304 */
    EvtFrameList *frameList; /* 0x2308 */
    u8 pad230C[0x4];
    s32 value; /* 0x2310 */
    s32 valueMin; /* 0x2314 */
    s32 valueMax; /* 0x2318 */
    f32 floatValue; /* 0x231C */
    f32 floatMin; /* 0x2320 */
    f32 floatMax; /* 0x2324 */
    u8 pad2328[0xA0];
    s32 tableColumn; /* 0x23C8 */
    u8 pad23CC[0x14];
    s32 selectedEntry; /* 0x23E0 */
    s32 commandFirst; /* 0x23E4 */
    s32 commandSecond; /* 0x23E8 */
    s32 commandThird; /* 0x23EC */
    u8 pad23F0[0x24];
    s32 timedActive; /* 0x2414 */
    u8 pad2418[0x10];
    s32 pendingWork; /* 0x2428 */
    s32 pendingResource; /* 0x242C */
    u8 pad2430[0xC];
    s32 headerMetadata; /* 0x243C: fourth serialized header word */
} EvtRuntime;

typedef struct {
    s16 unk0;
    s8 unk2;
    u8 pad3[7];
} EvtTblEntry; /* 0xA bytes */
extern EvtTblEntry D_00368950[];
extern s8 D_00368952[];

extern u16 D_003BBF88;
extern u16 D_003BBE78;
extern u16 D_003BBE7A;

extern u32 D_003BBF8C;

extern void kwlnDrawSpriteCell(s32 list, s32 x, s32 y, s32 w, s32 h);

extern char D_003BC0C8[]; /* "NAME:" */

/* Create a task with an initialized event payload. */
void evtCreateTask(s32 taskId, s32 value) {
    s32 data;

    data = evtAllocateContext();
    evtSetConvertedContextValue(data, value);
    kwlnTaskCreate(D_003BBF80, taskId, 1, 1, func_002351E0, func_00235228, (void *)data);
}

typedef struct EvtTaskData {
    u32 pad00;
    s32 value; /* 0x04 */
    u8 pad08[0x30];
    s32 effectHandle; /* 0x38: released before an updated effect is installed */
} EvtTaskData;

void evtCreateTaskWithValue(s32 taskId, s32 value) {
    EvtTaskData *data;

    data = (EvtTaskData *)evtAllocateContext();
    data->value = value;
    kwlnTaskCreate(D_003BBF80, taskId, 1, 1, func_002351E0, func_00235228, data);
}

void evtSetSkyOverlayEnabled(u32 enabled) {
    D_003BBF8C = enabled;
}

/* Either set the sky parameter immediately or interpolate from its current value. */
void evtBeginSkyParameterTransition(s32 duration, s32 target) {
    s16 current;

    current = func_00132B90();
    if (current != target) {
        if (duration == 0) {
            func_00132B80(target);
            D_003BBF88 = 0;
        } else {
            D_003BD89A = duration;
            D_003BD89C = current;
            D_003BD89E = target;
            D_003BBF88 = 1;
            D_003BD898 = 0;
        }
    }
}

u16 evtIsSkyTransitionActive(void) {
    return D_003BBF88;
}

void evtAdvanceSkyTransition(void) {
    if (D_003BBF88 != 0) {
        D_003BD898 += 1;
        func_00132B80(D_003BD89C + (s32)((f32)(D_003BD89E - D_003BD89C) * ((f32)D_003BD898 / (f32)D_003BD89A)));
        if (D_003BD898 >= D_003BD89A) {
            D_003BBF88 = 0;
        }
    }
}

s32 evtUpdateSkyTask(void) {
    evtAdvanceSkyTransition();
    func_00132010();
    if (D_003BBF8C != 0) {
        func_00129720(0x53);
        func_0012AEB0();
    }
    return 0;
}

void evtResetSkyTaskFlags(void) {
    D_003BBF88 = 0;
    D_003BBF8C = 0;
}

void evtDestroySkyTask(void) {
    s32 task;

    task = kwlnTaskGetTaskByName(D_003BBF90);
    if (task != 0) {
        kwlnTaskDestroyWithHierarchy(task, 1);
    }
}

void evtCreateSkyTask(void) {
    fldSetSwayMode(0);
    func_00132B70(0x80);
    func_00132B80(0);
    fldSetFadeTarget(0, 1, 0);
    kwlnTaskCreate(D_003BBF90, 0x2B0E, 1, 1, evtUpdateSkyTask, evtResetSkyTaskFlags, 0);
}

INCLUDE_ASM(const s32, "game/code_00235270", evtUpdateFrameVariableTask);

void evtCreateFrameVariableTask(void) {
    kwlnTaskCreate("FrameVar", 0x2AF9, 1, 1, evtUpdateFrameVariableTask, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_00235270", func_00235598);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003ADDE0);

s32 func_00235768(s32 list, s32 x, s32 y) {
    sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, 0, "VALUE CHANGE."));
    return 2;
}

INCLUDE_ASM(const s32, "game/code_00235270", func_002357B8);

typedef struct MenuGfxCallback {
    u8 unknown[0x10];
    void (*invoke)(void *, void *);
} MenuGfxCallback;
extern MenuGfxCallback D_00325748;
extern u32 sdfCreateResetPacketList(void);
extern void func_00235598(u32, s32, s32, s32, s32, s32, s32, u8 *, void *, void *);
extern s8 D_00324510[];
extern void func_002357B8();

s32 evtViewerFloatValueUpdate(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;
    f32 step;

    list = sdfCreateResetPacketList();
    func_00235598(list, x, y, 0x16, 9, 0, 1, (u8 *)ctx, func_00235768, func_002357B8);
    D_00325748.invoke(&D_00325748, (void *)list);
    if (ctx->actionMode != 8) {
        return 0;
    }
    if (D_00324510[0x21] < 0) {
        return 1;
    }
    if (D_00324510[0x23] < 0) {
        return -1;
    }
    step = 0.0f;
    if (D_00324510[0x24] & 2) {
        step = -0.1f;
    } else if (D_00324510[0x25] & 2) {
        step = 0.1f;
    }
    if (D_00324510[0x26] & 2) {
        step = -1.0f;
    } else if (D_00324510[0x27] & 2) {
        step = 1.0f;
    }
    ctx->floatValue += step;
    if (ctx->floatValue < ctx->floatMin) {
        ctx->floatValue = ctx->floatMin;
    }
    if (ctx->floatValue >= ctx->floatMax) {
        ctx->floatValue = ctx->floatMax;
    }
    return 0;
}

s32 func_00235AE0(s32 list, s32 x, s32 y) {
    sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, 0, "VALUE CHANGE."));
    return 2;
}

extern char D_003ADE40[];
extern char D_003ADE50[];
extern char D_003ADE78[];

INCLUDE_RODATA(const s32, "game/code_00235270", D_003ADE40);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003ADE50);

INCLUDE_RODATA(const s32, "game/code_00235270", jtbl_003ADE60);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003ADE78);

void func_00235B30(s32 list, s32 x, s32 y, u32 index, EvtRuntime *ctx) {
    switch (index) {
    case 0:
        sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, 4, D_003BC058, ctx->value));
        return;
    case 2:
        sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, 0, D_003ADE78));
        return;
    case 3:
        sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, 0, " U,D = VALUE-+10"));
        return;
    case 4:
        sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, 0, D_003ADE40));
        return;
    case 5:
        sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, 0, D_003ADE50));
        break;
    }
}

s32 func_00235C68(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;
    s32 step;

    list = sdfCreateResetPacketList();
    func_00235598(list, x, y, 0x16, 9, 0, 1, (u8 *)ctx, func_00235AE0, func_00235B30);
    D_00325748.invoke(&D_00325748, (void *)list);
    if (ctx->actionMode != 7) {
        return 0;
    }
    if (D_00324510[0x21] < 0) {
        return 1;
    }
    if (D_00324510[0x23] < 0) {
        return -1;
    }
    if (D_00324510[0x24] & 2) {
        step = -1;
    } else if (D_00324510[0x25] & 2) {
        step = 1;
    } else {
        step = 0;
    }
    if (D_00324510[0x26] & 2) {
        step = -10;
    } else if (D_00324510[0x27] & 2) {
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
    sdfAppendPacket(target, func_002E4960(x, y, 0xFEFFFF, 0, "FRAME CHANGE."));
    return 2;
}

void evtViewerDrawFrameChangeRow(s32 list, s32 x, s32 y, u32 index, EvtRuntime *ctx) {
    switch (index) {
    case 0:
        sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, 4, D_003BC058, ctx->value));
        return;
    case 2:
        sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, 0, " L,R = FRMAE-+"));
        return;
    case 3:
        sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, 0, " U,D = FRAME-+10"));
        return;
    case 4:
        sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, 0, "L1,R1= FRAME-+100"));
        return;
    case 5:
        sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, 0, D_003ADE40));
        return;
    case 6:
        sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, 0, D_003ADE50));
        return;
    case 7:
        sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, 0, " RL  = NOW FRAME"));
        return;
    case 8:
        sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, 0, " ST  = CAMERA FOCUS"));
        break;
    case 9:
        break;
    }
}

s32 evtViewerFrameChangeUpdate(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;
    s32 step;

    list = sdfCreateResetPacketList();
    func_00235598(list, x, y, 0x16, 0xB, 0, 1, (u8 *)ctx, mnuDrawFrameChangeLabel, evtViewerDrawFrameChangeRow);
    D_00325748.invoke(&D_00325748, (void *)list);
    if (ctx->actionMode != 6) {
        return 0;
    }
    if (D_00324510[0x21] < 0) {
        return 1;
    }
    if (D_00324510[0x23] < 0) {
        return -1;
    }
    if (D_00324510[0x24] & 2) {
        step = -1;
    } else if (D_00324510[0x25] & 2) {
        step = 1;
    } else {
        step = 0;
    }
    if (D_00324510[0x26] & 2) {
        step = -10;
    } else if (D_00324510[0x27] & 2) {
        step = 10;
    }
    if (D_00324510[0x28] & 2) {
        step = -100;
    } else if (D_00324510[0x2A] & 2) {
        step = 100;
    }
    if (D_00324510[0x20] < 0) {
        step = ctx->curFrame - ctx->value;
    }
    ctx->value += step;
    if (ctx->value < ctx->valueMin) {
        ctx->value = ctx->valueMin;
    }
    if (ctx->value >= ctx->valueMax) {
        ctx->value = ctx->valueMax;
    }
    if (D_00324510[0x2C] != 0) {
        if (ctx->curFrame != ctx->value) {
            ctx->curFrame = ctx->value;
            evtViewerDispatchFlagMode(ctx);
            func_0022E5A0(ctx->curFrame, ctx);
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00235270", func_00236180);

extern void func_00236180();
extern void func_00237130();
extern s32 func_001037C0(s32, s32, s32, s32, s32, s32, s32, s32, u8 *);

s32 mnuDrawInfoWindowA(s32 x, s32 y, u8 *work) {
    u32 packets = sdfCreateResetPacketList();
    func_00235598(packets, x, y, 0xF, 0xB, 0, 0xB, work, 0, func_00236180);
    D_00325748.invoke(&D_00325748, (void *)packets);
    if (((EvtRuntime *)work)->actionMode != 1) {
        return 0;
    }
    return func_001037C0(0, 1, 0xB, 1, 0xB, 0, 0, 0, work + 0x22A8);
}

extern char D_003BC088[];
extern char D_003BC090[];
extern char D_003BC0A0[];
extern u8 D_003688B8[];

typedef struct EvtDrawWork {
    u8 pad00[0x2280];
    s32 unk2280;
    u8 pad2284[0x38];
    s32 unk22BC;
    s32 unk22C0;
    char *label;
    s32 *unk22C8;
    s32 unk22CC;
    s32 unk22D0;
    u8 pad22D4[0x114];
    char *name0;
    char *name1;
} EvtDrawWork;

s32 evtDrawStringEntry(s32 output, s32 x, s32 y, EvtDrawWork *work) {
    if (work->label == 0) {
        return 0;
    }
    sdfAppendPacket(output, func_002E4960(x, y, 0xFEFFFF, 0, D_003BC088, work->label));
    return 2;
}

void func_00236510(s32 list, s32 x, s32 y, s32 index, EvtDrawWork *work) {
    s32 color;

    if (index < work->unk22C0) {
        if (work->unk22BC == index) {
            color = work->unk2280 == 2 ? 4 : 5;
        } else {
            color = 0;
        }
        sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, color, D_003BC090, work->unk22C8[index]));
    }
}

s32 func_002365A0(s32 x, s32 y, EvtDrawWork *work) {
    u32 packets = sdfCreateResetPacketList();
    s32 width = 10;

    if (work->label != 0) {
        width = strlen(work->label);
        if (width < 6) {
            width = 6;
        }
    }
    func_00235598(packets, x, y, width, work->unk22C0 + 3, 0, work->unk22C0, (u8 *)work, evtDrawStringEntry, func_00236510);
    D_00325748.invoke(&D_00325748, (void *)packets);
    if (work->unk2280 != 2) {
        return 0;
    }
    return func_001037C0(0, 1, work->unk22C0, 1, work->unk22C0, 0, 0, 0, (u8 *)work + 0x22BC);
}

s32 evtDrawInputValueRow(s32 list, s32 x, s32 y, u8 *ctx) {
    char text[16];
    func_003014F0(text, D_003BC098, (s32)ctx + 0x22D4, (s32)ctx + 0x22E0);
    sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, 0, D_003BC088, text));
    return 2;
}

void evtDrawKeyboardRow(s32 list, s32 xPosition, s32 y, s32 row, EvtDrawWork *work) {
    s32 x = xPosition + 0xC0;
    u8 *table = &D_003688B8[row * 0xC];
    s32 i = 0;
    s32 color;
    s8 ch;
    s32 drawX;

    do {
        color = 0;
        if (work->unk22D0 == row && work->unk22CC == i) {
            color = work->unk2280 == 3 ? 4 : 5;
        }
        ch = *table++;
        drawX = x;
        x += 0xC0;
        i++;
        sdfAppendPacket(list, func_002E4960(drawX, y, 0xFEFFFF, color, D_003BC0A0, ch));
    } while (i < 0xB);
}

INCLUDE_ASM(const s32, "game/code_00235270", func_00236828);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE0B8);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE0C8);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE0D8);

s32 evtDrawEventFileNameRow(s32 list, s32 x, s32 y) {
    char text[32];
    func_003014F0(text, "[E%3d_%03d.PM1+2+3]", D_003BBE78, D_003BBE7A);
    sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, 0, D_003BC088, text));
    return 2;
}

INCLUDE_ASM(const s32, "game/code_00235270", func_00236AC8);

INCLUDE_ASM(const s32, "game/code_00235270", func_00236D80);

void func_00237048(s32 list, s32 *sel, s32 x, s32 unused, u8 *base) {
    x += 0x6C0;
    kwlnDrawSpriteCell(list, (x - 0x7000) >> 4, 0x3C, 0x1C, 3);
    sdfAppendPacket(list, func_002E4960(x, 0x7AE0, 0xFEFFFF, 0xE, D_003BC0C8));
    if (sel[2] >= 0) {
        sdfAppendPacket(list, func_002E4960(x + 0x3C0, 0x7AE0, 0xFEFFFF, 0, D_003BC088, base + sel[2] * 32 + 0x24));
    }
}

INCLUDE_ASM(const s32, "game/code_00235270", func_00237130);

s32 func_00237348(s32 x, s32 y, u8 *work) {
    u32 packets = sdfCreateResetPacketList();
    s32 count;
    s32 shown;

    func_00235598(packets, x, y, 8, 0x1D, ((EvtRuntime *)work)->entryFirst, ((EvtRuntime *)work)->entryCount, work, 0, func_00237130);
    D_00325748.invoke(&D_00325748, (void *)packets);
    if (((EvtRuntime *)work)->actionMode != 4) {
        return 0;
    }
    count = ((EvtRuntime *)work)->entryCount;
    if (count == 0) {
        return 0;
    }
    shown = 0x1D;
    if (count < 0x1D) {
        shown = count;
    }
    return func_001037C0(0, 1, count, 1, shown, 0, (s32)(work + 0x22F8), 0, work + 0x22F4);
}

INCLUDE_ASM(const s32, "game/code_00235270", func_00237428);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE530);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE540);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE550);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE560);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE570);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE580);

INCLUDE_ASM(const s32, "game/code_00235270", func_002375D8);

extern void func_002375D8(s32 list, s32 x, s32 y, s32 color, EvtFrameNode *node, EvtRuntime *ctx);

s32 evtDrawFrameListRow(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
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
            if (ctx->actionMode == 5) {
                color = 4;
            } else {
                color = 5;
            }
        } else {
            color = 0;
        }
        if (node != NULL) {
            func_002375D8(list, x, y, color, node, ctx);
        } else {
            sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, color ? color : 8, "----- NEW FRAME -----"));
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00235270", func_002386E0);

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

void evtViewerDrawWorldNodeRow(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
    s32 color;
    s32 count;
    s32 i;
    EvtWorldNode *node;

    color = 4;
    if (ctx->groupFirst + ctx->groupCursor != index) {
        color = 0;
    }
    if (index == 0) {
        sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, color, " -----------------------"));
        return;
    }
    count = 0;
    for (i = 0; i < 0x12; i++) {
        if (i != 3) {
            for (node = dds3GetWorldObject()->table->slots[i].head; node != NULL; node = node->next) {
                if (node->name != NULL) {
                    count++;
                    if (count == index) {
                        sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, color, D_003BC090, node->name));
                        return;
                    }
                }
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00235270", func_00238A88);

extern char D_003BC240[]; /* "P%d:" */
extern char D_003BC248[]; /* "   %s" */
extern s32 evtEventViewerGetPendingNode();
extern EvtWorldNode *func_00110F80(EvtWorldObject *world, char *name);

void evtViewerDrawPendingNodeRow(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
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
        node = func_00110F80(dds3GetWorldObject(), ctx->entryName[slot]);
    }
    sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, 0xE, D_003BC240, index));
    if (node != NULL) {
        sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, color, D_003BC248, node->name));
    } else {
        sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, color, "   -----------------------"));
    }
}

extern void evtViewerDrawPendingNodeRow();

s32 mnuDrawInfoWindowB(s32 x, s32 y, u8 *work) {
    u32 packets = sdfCreateResetPacketList();
    s32 rows;
    switch (((EvtRuntime *)work)->frameList->kind) {
    case 20:
        rows = 2;
        break;
    case 21:
        rows = 4;
        break;
    default:
        return -1;
    }
    func_00235598(packets, x, y, 0x1C, rows, 0, rows, work, 0, evtViewerDrawPendingNodeRow);
    D_00325748.invoke(&D_00325748, (void *)packets);
    if (((EvtRuntime *)work)->actionMode != 0xC) {
        return 0;
    }
    return func_001037C0(0, 1, rows, 1, rows, 0, 0, 0, work + 0x22B8);
}

s32 mnuDrawMessageMenuLabel(s32 list, s32 x, s32 y, u8 *work) {
    s32 count = itfMesGetEntryCount(*(s32 *)(*(s32 *)(work + 8) + 0x104));
    sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, 0, "MESSAGE MENU (MESMAX %3d)", count));
    return 2;
}

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE8D0);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE8E0);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE8F0);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE900);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE910);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE920);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE930);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE940);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE950);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE960);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE970);

INCLUDE_ASM(const s32, "game/code_00235270", func_00238ED0);

INCLUDE_ASM(const s32, "game/code_00235270", func_00239148);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE9D0);

INCLUDE_RODATA(const s32, "game/code_00235270", jtbl_003AE9E0);

s32 mnuDrawCutFlagLabel(s32 target, s32 x, s32 y) {
    sdfAppendPacket(target, func_002E4960(x, y, 0xFEFFFF, 0, "CUTFLAG MENU"));
    return 2;
}

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AEA10);

INCLUDE_ASM(const s32, "game/code_00235270", func_002393A0);

INCLUDE_ASM(const s32, "game/code_00235270", func_002395A8);

s32 func_00239770(s32 *index) {
    return D_00368950[*index].unk0 != 0;
}

s32 mnuGetSelectedTableValue(EvtRuntime *runtime) {
    return D_00368952[runtime->tableColumn + runtime->frameList->kind * 10];
}

INCLUDE_ASM(const s32, "game/code_00235270", func_002397C8);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AEA70);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AEA80);

INCLUDE_ASM(const s32, "game/code_00235270", func_00239A90);

s32 mnuDrawMotionChangeLabel(s32 target, s32 x, s32 y) {
    sdfAppendPacket(target, func_002E4960(x, y, 0xFEFFFF, 0, "MOTION CHANGE MENU"));
    return 2;
}

INCLUDE_ASM(const s32, "game/code_00235270", func_00239E30);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023A0D0);

extern char D_003BC288[]; /* "DISABLE" */
extern char D_003BC280[]; /* "ALL" */
extern char D_003AEA80[]; /* "UNIT ALL" */

void evtDrawGroupListRow(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
    EvtRuntimeGroup *group;
    s32 color;
    s32 n;

    color = 4;
    if (ctx->groupFirst + ctx->groupCursor != index) {
        color = 0;
    }
    if (index == 0) {
        sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, color, D_003BC288));
    } else if (index == 1) {
        sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, color, D_003BC280));
        return;
    } else if (index == 2) {
        sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, color, D_003AEA80));
        return;
    }
    n = 3;
    for (group = ctx->groups; group != NULL; group = group->next) {
        if (group->type == 1) {
            if (n == index) {
                sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, color, D_003BC088, group->info->name));
                return;
            }
            n++;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00235270", func_0023A688);

void func_0023A798(void) {
}

INCLUDE_ASM(const s32, "game/code_00235270", func_0023A7A0);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023A968);

void func_0023B1F8(void) {
}

INCLUDE_ASM(const s32, "game/code_00235270", func_0023B200);

void func_0023B848(s32 list, s32 x, s32 y, s32 kind, EvtDrawWork *work) {
    switch (kind) {
    case 0:
        if (work->name0 != 0) {
            sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, 0, D_003BC088, work->name0));
        }
        return;
    case 1:
        if (work->name1 != 0) {
            sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, 0, D_003BC088, work->name1));
        }
        break;
    }
}

extern s8 D_00324510[];

s32 mnuDrawTimedPrompt(s32 x, s32 y, u8 *work) {
    u32 packets = sdfCreateResetPacketList();
    s32 count;
    func_00235598(packets, x, y, 0x19, 2, 0, 1, work, 0, func_0023B848);
    D_00325748.invoke(&D_00325748, (void *)packets);
    if (((EvtRuntime *)work)->actionMode != 0x14) {
        return 0;
    }
    count = ((EvtRuntime *)work)->commandFirst;
    if (count > 0) {
        ((EvtRuntime *)work)->commandFirst = count - 1;
    } else if (count == 0) {
        return -1;
    }
    if (D_00324510[0x21] < 0) {
        return 1;
    }
    return D_00324510[0x23] >= 0 ? 0 : -1;
}

void evtSetRuntimeCommandValues(EvtRuntime *runtime, s32 first, s32 second, s32 third) {
    runtime->commandFirst = first;
    runtime->commandSecond = second;
    runtime->commandThird = third;
}

extern char D_003BC300[]; /* "CURRENT" */

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AEB90);

void evtViewerDrawGroupRow(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
    EvtRuntimeGroup *group;
    s32 color;
    s32 n;

    color = 4;
    if (ctx->groupFirst + ctx->groupCursor != index) {
        color = 0;
    }
    if (index == 0) {
        sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, color, D_003BC300));
        return;
    }
    if (index == 1) {
        sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, color, "DEFFAULT"));
        return;
    }
    n = 2;
    for (group = ctx->groups; group != NULL; group = group->next) {
        if (group->type == 0x18) {
            if (n == index) {
                sdfAppendPacket(list, func_002E4960(x, y, 0xFEFFFF, color, D_003BC088, ctx->entryName[group->entryHeader.word]));
                return;
            }
            n++;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00235270", func_0023BB20);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AEBB0);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023BC30);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023BE40);

extern s32 func_0018FEB0();
extern s32 kwlnTaskGetTimer(s32 task);
typedef struct EvtSelectionCache {
    u8 pad00[0x24];
    s32 selectedEntry; /* 0x24 */
} EvtSelectionCache;

extern EvtSelectionCache *D_003BB128;
extern s32 D_003BD8A0;

s32 evtSynchronizeSelectedEntry(s32 task) {
    EvtRuntime *runtime = func_00101A70();
    if (func_0018FEB0() == 0) {
        runtime->controlState = 0;
        return -1;
    }
    if (kwlnTaskGetTimer(task) == 0) {
        s32 selected = runtime->selectedEntry;
        D_003BD8A0 = selected;
        if (selected != 0) {
            D_003BB128->selectedEntry = selected;
        }
    }
    if (D_003BB128->selectedEntry != runtime->selectedEntry) {
        s32 selected = runtime->selectedEntry;
        if (selected != 0) {
            D_003BB128->selectedEntry = selected;
        }
    }
    return 0;
}

s32 func_0023C208(void) {
    EvtRuntime *runtime;

    runtime = func_00101A70();
    if (func_0018FDA8() == 0) {
        runtime->controlState = 0;
        return -1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AEC00);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AEC10);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AEC20);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023C248);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023CA60);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023D420);

extern s32 (*D_00368B48[])(s32, s32, void *);

s32 evtDispatchActionByIndex(s32 index, s32 x, s32 y, EvtRuntime *runtime) {
    s32 mode = runtime->actionMode;
    if (mode == 11 && index != mode) {
        return 0;
    }
    return D_00368B48[index](x, y, runtime);
}

void func_0023D5B0(s32 output, void *data, s32 size) {
    func_0030F190(output, data, size);
}

s32 func_0023D5C8(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xA) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->unk12 = index++;
            }
        }
    }
    return index;
}

s32 func_0023D630(EvtRuntime *runtime) {
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

s32 func_0023D698(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xD) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->unk0A = index++;
            }
        }
    }
    return index;
}

s32 func_0023D700(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xE) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->unk0A = index++;
            }
        }
    }
    return index;
}

s32 func_0023D768(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xF) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->unk0A = index++;
            }
        }
    }
    return index;
}

s32 func_0023D7D0(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x17) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->unk0A = index++;
            }
        }
    }
    return index;
}

s32 func_0023D838(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x1B) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->unk0A = index++;
            }
        }
    }
    return index;
}

s32 func_0023D8A0(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x10) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->unk0A = index++;
            }
        }
    }
    return index;
}

s32 func_0023D908(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x11) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->unk0A = index++;
            }
        }
    }
    return index;
}

s32 func_0023D970(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x19) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->unk0A = index++;
            }
        }
    }
    return index;
}

INCLUDE_ASM(const s32, "game/code_00235270", func_0023D9D8);

void evtWriteRuntimeHeaderValues(s32 output, s32 *state) {
    s32 first = state[4];
    s32 second = state[5];
    s32 third = state[3];
    s32 metadata = ((EvtRuntime *)state)->headerMetadata;
    s32 buf[4];

    buf[0] = first;
    buf[1] = second;
    buf[2] = third;
    buf[3] = metadata;
    func_0023D5B0(output, buf, 0x10);
}

INCLUDE_ASM(const s32, "game/code_00235270", func_0023DFA8);

typedef struct EvtFixedEntryTable {
    u8 pad00[0x20];
    s32 count;                 /* 0x20 */
    u8 firstEntry[0x20];      /* 0x24: 0x20-byte records follow */
} EvtFixedEntryTable;

void evtWriteFixedSizeEntries(s32 output, EvtFixedEntryTable *table) {
    void *entry;
    s32 index;

    index = 0;
    if (0 < table->count) {
        entry = table->firstEntry;
        do {
            func_0023D5B0(output, entry, 0x20);
            index = index + 1;
            entry = (void *)((s32)entry + 0x20);
        } while (index < table->count);
    }
}

void evtWriteGroupHeader(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 2) {
            s32 header[4];
            header[0] = group->entryHeader.word;
            header[1] = 0;
            header[2] = 0;
            header[3] = 0;
            func_0023D5B0(output, header, 0x10);
        }
    }
}

void func_0023E228(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xA) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                func_0023D5B0(output, child->payload, 0x30);
            }
        }
    }
}

void func_0023E2B0(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xB) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                func_0023D5B0(output, child->payload, 0x20);
            }
        }
    }
}

void func_0023E338(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xD) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                func_0023D5B0(output, child->payload, 0x28);
            }
        }
    }
}

void func_0023E3C0(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xE) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                func_0023D5B0(output, child->payload, 0x2C);
            }
        }
    }
}

void func_0023E448(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xF) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                func_0023D5B0(output, child->payload, 0x2C);
            }
        }
    }
}

void func_0023E4D0(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x17) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                func_0023D5B0(output, child->payload, 0x2C);
            }
        }
    }
}

void func_0023E558(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x1B) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                func_0023D5B0(output, child->payload, 0x28);
            }
        }
    }
}

void func_0023E5E0(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x10) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                func_0023D5B0(output, child->payload, 0x18);
            }
        }
    }
}

void func_0023E668(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x11) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                func_0023D5B0(output, child->payload, 0x24);
            }
        }
    }
}

void evtWriteGroupMetadata(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        u8 header[8];
        header[0] = *(u8 *)group;
        header[1] = group->metadataFlag;
        *(u16 *)&header[2] = group->entryHeader.shortValue;
        *(u16 *)&header[4] = group->metadataValue;
        header[6] = group->metadataByte1;
        header[7] = group->metadataByte2;
        func_0023D5B0(output, header, sizeof(header));
    }
}

void func_0023E770(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x19) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                func_0023D5B0(output, child->payload, 0x40);
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00235270", func_0023E7F8);

/* A row table switches between 0x10-byte and 0x2C-byte entry formats. */
typedef struct EvtRowTable {
    u8 pad00[0x74];
    s32 descriptor;   /* 0x74: format at descriptor + 0x14 */
    u8 pad78[0x10];
    s32 compactRows;  /* 0x88 */
    s32 extendedRows; /* 0x8C */
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

u16 func_0023ED08(s32 table, s32 row) {
    if (((EvtRowDescriptor *)((EvtRowTable *)table)->descriptor)->format == 4) {
        return ((EvtCompactRow *)(((EvtRowTable *)table)->compactRows + row * 0x10))->value;
    }
    return ((EvtExtendedRow *)(((EvtRowTable *)table)->extendedRows + row * 0x2c))->value;
}

s16 func_0023ED58(s32 table, s32 row) {
    if (((EvtRowDescriptor *)((EvtRowTable *)table)->descriptor)->format == 4) {
        return ((EvtCompactRow *)(((EvtRowTable *)table)->compactRows + row * 0x10))->variant;
    }
    return ((EvtExtendedRow *)(((EvtRowTable *)table)->extendedRows + row * 0x2c))->variant;
}

u16 func_0023EDA8(s32 table, s32 row) {
    if (((EvtRowDescriptor *)((EvtRowTable *)table)->descriptor)->format == 4) {
        return ((EvtCompactRow *)(((EvtRowTable *)table)->compactRows + row * 0x10))->parameter;
    }
    return ((EvtExtendedRow *)(((EvtRowTable *)table)->extendedRows + row * 0x2c))->parameter;
}

u16 func_0023EDF8(s32 table, s32 row) {
    if (((EvtRowDescriptor *)((EvtRowTable *)table)->descriptor)->format == 4) {
        return ((EvtCompactRow *)(((EvtRowTable *)table)->compactRows + row * 0x10))->flags;
    }
    return ((EvtExtendedRow *)(((EvtRowTable *)table)->extendedRows + row * 0x2c))->flags;
}

s32 func_0023EE48(s32 table, s32 row) {
    if (((EvtRowDescriptor *)((EvtRowTable *)table)->descriptor)->format == 4) {
        return ((EvtRowTable *)table)->compactRows + row * 0x10 + 8;
    }
    return ((EvtRowTable *)table)->extendedRows + row * 0x2c + 0xc;
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

void evtResolveLinkGroupIndex(EvtLinkSource *src, EvtRuntime *runtime, EvtLink *link) {
    s32 i;
    EvtRuntimeGroup *group;

    if (link->type != 3) {
        link->index = 0;
    } else {
        for (i = 0; i < src->count; i++) {
            for (group = runtime->groups; group != NULL; group = group->next) {
                if (strcmp(runtime->entryName[group->entryHeader.word], src->names + link->index * 32) == 0) {
                    link->index = group->entryHeader.word;
                    return;
                }
            }
        }
        func_003003F0("not found linkslight obj index %s\n", src->names + link->index * 32);
        link->type = 0;
        link->index = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00235270", func_0023EF90);

extern void func_002441E8(EvtRuntime *runtime);
extern void func_00270030(void);
extern s32 sdfCheckPendingWorkWithInterrupts(void);
extern void func_002D0A10(s32 resource);
extern void kwlnTextureReleaseHeldReference(void);
extern u32 D_003BA904;
extern void func_00234CA8(u16 a, u16 b, char *path0, char *path1, char *path2);
extern s32 sdfPathExists(char *path);
extern void evtEventViewerShutdown(EvtRuntime *runtime);
extern void evtDestroySecondaryWorldNode(void);
extern void evtEventViewerReleaseGroups(EvtRuntime *runtime);
extern void evtEventViewerReset(EvtRuntime *runtime);
extern s32 func_00234DA8(u16 a, u16 b, s32 mode);
extern void func_0023EF90(s32 handle, EvtRuntime *runtime);

s32 evtReloadEventViewer(s32 mode, EvtRuntime *runtime) {
    char path0[0x80];
    char path1[0x80];
    char path2[0x80];
    s32 handle;

    func_002441E8(runtime);
    if (runtime->timedActive == 1) {
        func_00270030();
        runtime->timedActive = 0;
    }
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    if (runtime->pendingResource != 0) {
        func_002D0A10(runtime->pendingResource);
        runtime->pendingResource = 0;
        runtime->pendingWork = 0;
    }
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    kwlnTextureReleaseHeldReference();
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    D_003BA904 |= 0x2000000;
    func_00234CA8(D_003BBE78, D_003BBE7A, path0, path1, path2);
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
    handle = func_00234DA8(D_003BBE78, D_003BBE7A, mode);
    if (handle != 0) {
        runtime->windowContext = handle;
        func_0023EF90(handle, runtime);
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
    func_002E9340(sound);
    return sound;
}

s32 evtIsBgmLoaded(s32 id) {
    if ((u32)(id - 0x258) >= 0x100) {
        return 1;
    }
    return func_002E92C0(evtEncodeBgmSoundCode(id, 0)) == 1;
}

s32 evtPlayBgm(s32 id, s32 fade) {
    s32 sound;
    if ((u32)(id - 0x258) >= 0x100) {
        return 0;
    }
    sound = evtEncodeBgmSoundCode(id, fade);
    func_003003F0("Event BGM play :%08X\n", sound);
    sndStartTrackDefault(sound);
    return sound;
}

void evtTransitionBgm(s32 id, s32 fade) {
    s32 sound;
    if ((u32)(id - 0x258) >= 0x100) {
        return;
    }
    sound = -1;
    if (fade >= 0) {
        sound = evtEncodeBgmSoundCode(id, fade);
    }
    func_003003F0("Event BGM trans :%08X\n", sound);
    func_002E9758(sound);
}

s32 evtFadeInBgm(s32 id, s32 fade) {
    s32 sound;
    if ((u32)(id - 0x258) >= 0x100) {
        return 0;
    }
    sound = evtEncodeBgmSoundCode(id, fade);
    func_003003F0("Event BGM fade in play :%08X\n", sound);
    sndStartTrackExtended(sound);
    return sound;
}

s32 func_00241A50(s32 id, s32 fade) {
    s32 sound;
    if ((u32)(id - 0x258) >= 0x100) {
        return 0;
    }
    sound = evtEncodeBgmSoundCode(id, fade);
    func_002E96D8(sound);
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

s32 func_00241AE8(s32 id, s32 fade) {
    s32 sound;
    if ((u32)(id - 0x258) >= 0x100) {
        return 0;
    }
    sound = evtEncodeBgmSoundCode(id, fade);
    func_002E8DD0(sound);
    return sound;
}

void evtFormatTaskName(s32 taskId, void *buffer) {
    func_003014F0(buffer, D_003BC360, taskId);
}

s32 evtFindTaskById(u32 taskId) {
    u8 taskName[32];

    evtFormatTaskName(taskId, taskName);
    return kwlnTaskGetTaskByName(taskName);
}

s32 evtGetTaskValueWord(u32 taskId) {
    s32 task;

    task = evtFindTaskById(taskId);
    if (task != 0) {
        return ((EvtTaskData *)func_00101A70(task))->value;
    } else {
        return -1;
    }
}

EvtTaskData *evtGetTaskData(u32 taskId) {
    s32 task;

    task = evtFindTaskById(taskId);
    if (task != 0) {
        return func_00101A70(task);
    }
    return (EvtTaskData *)task;
}

/* Task data of a resource-list task: entries[] are 0x20 bytes, keyed at +0x10. */
typedef struct EvtResourceEntry {
    u8 pad00[0xC];
    s32 offset;
    s32 key;
    u8 pad14[0xC];
} EvtResourceEntry;

typedef struct EvtResourceHeader {
    u8 pad00[0x10];
    s32 count;
} EvtResourceHeader;

typedef struct EvtResourceTask {
    u32 pad00;
    s32 kind;
    u8 pad08[8];
    s32 base;
    EvtResourceHeader *header;
    EvtResourceEntry *entries;
} EvtResourceTask;

s32 func_00241BF0(u32 taskId, s32 key) {
    s32 i;
    EvtResourceTask *data;
    s32 task;

    task = evtFindTaskById(taskId);
    if (task != 0) {
        data = func_00101A70(task);
        if (data->kind != 2) {
            return 0;
        }
        for (i = 0; i < data->header->count; i++) {
            if (data->entries[i].key == key) {
                return data->base + data->entries[i].offset;
            }
        }
        return 0;
    }
    return task;
}

extern void sdfTexReleaseReferenceViaHandler(s32 handle);
extern s32 func_002D3288(s32 resource);
extern void effSetCh72Id(s32 id);

void evtRefreshTaskData(s32 taskId, s32 key) {
    EvtTaskData *data = evtGetTaskData(taskId);
    s32 resource = func_00241BF0(taskId, key);
    if (resource != 0) {
        s32 old = data->effectHandle;
        if (old != 0) {
            sdfTexReleaseReferenceViaHandler(old);
            data->effectHandle = 0;
        }
        resource = func_002D3288(resource);
        effSetCh72Id(resource);
        data->effectHandle = resource;
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

extern Entry270 *D_003BAA10;
extern Entry270 *D_003BAA20;

void *evtSpawnResourceObject(s32 taskId, s32 key, s32 index) {
    u8 vecA[16];
    u8 vecB[16];
    void *found;
    void *obj;
    EvtEffectInner *inner;

    memset(vecA, 0, 0x10);
    memset(vecB, 0, 0x10);
    found = (void *)func_00241BF0(taskId, key);
    if (found != 0) {
        obj = func_00115298(found, vecA, vecB);
        if (obj != 0) {
            effObjSetFlags(obj, 1);
            if (index >= 0) {
                inner = ((EvtEffectObject *)obj)->inner;
                func_00190308(inner->scaledObject, D_003BAA20[index].unk18 / D_003BAA10->unk18);
            }
            return obj;
        }
        return obj;
    }
    return found;
}

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBF80);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBF88);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBF8C);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBF90);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBFA0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBFA8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBFB0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBFB8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBFC0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBFC8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBFD0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBFD8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBFE0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBFE8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBFF0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBFF8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC000);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC008);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC010);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC018);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC020);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC028);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC030);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC038);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC040);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC048);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC050);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC058);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC060);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC068);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC070);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC078);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC080);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC088);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC090);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC098);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC0A0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC0A8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC0B0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC0B8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC0C0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC0C8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC0D0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC0D8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC0E0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC0E8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC0F0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC0F8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC100);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC108);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC110);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC118);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC120);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC128);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC130);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC138);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC140);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC148);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC150);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC158);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC160);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC168);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC170);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC178);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC180);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC188);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC190);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC198);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC1A0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC1A8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC1B0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC1B8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC1C0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC1C8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC1D0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC1D8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC1E0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC1E8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC1F0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC1F8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC200);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC208);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC210);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC218);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC220);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC228);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC230);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC238);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC240);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC248);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC250);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC258);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC260);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC270);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC278);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC280);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC288);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC290);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC298);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC2A0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC2A8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC2B0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC2B8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC2C0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC2C8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC2D0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC2D8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC2E0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC2E8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC2F0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC2F8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC300);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC308);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC310);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC318);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC320);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC328);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC330);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC338);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC340);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC348);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC350);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC358);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC360);

