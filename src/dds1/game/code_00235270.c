#include "common.h"
#include "evt_world.h"

extern void func_0023D5B0(s32 output, void *data, s32 size);

extern void *kwlnTaskGetUserValue();
extern s32 effEventAdvanceSolidRectangleSetup(void);
extern s32 sndFindPackedTrackLoadStatus(s32 sequence);
extern void sndEnsureMidiBankResident(s32 sequence);
extern s32 evtUpdateFrameVariableTask(s32 *task);
extern s32 evtAllocateContext(void);
extern void evtSetConvertedContextValue(s32 context, s32 value);
extern void evtUpdatePictureWhenFlagged(void);
extern void evtPictureReleaseTaskTextureAndState(void);
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
extern void fldSetSkyDrawState(s32 value);
extern void func_00132B80(s32 value);
extern s16 func_00132B90(void);
extern void fldSetFadeTarget(s32 area, s32 value, s32 duration);
extern void func_00132010(void);
extern void fldSelectDisplayBuffer(s32 id);
extern void func_0012AEB0(void);
extern void sdfAppendPacket(s32 list, s32 packet);
extern void sndStartTrackDefault(s32 track);
extern void sndStartTrackExtended(s32 track);
extern void func_002E9758(s32 sequence);
extern void func_003003F0(char *fmt, ...);
extern void func_002E96D8(u32 sequence);
extern void func_002E8DD0(u32 sequence);
extern void sndSetSequenceVolumePan(s32 sequence, s32 volume, s32 pan);
extern s32 sdfCreateFormattedSifCommand();
extern u32 itfMesGetEntryCount(s32 window);
extern void evtViewerDispatchFlagMode();
extern void func_0022E5A0();
extern u8 evtPictureTaskName[];
extern u8 evtSkyTaskName[];
extern u8 D_003BC360[];
extern char D_003BC058[]; /* "     %d" */
extern char D_003BC0A8[]; /* " EVENT" */
extern char D_003BC0B0[]; /* "%3d" */
extern char D_003BC0B8[]; /* "   CUT" */
extern char D_003BC0C0[]; /* "%03d" */
extern char D_003BC098[];
extern u16 evtSkyTransitionFrame;
extern u16 evtSkyTransitionDuration;
extern s16 evtSkyTransitionStartValue;
extern s16 evtSkyTransitionTargetValue;


typedef struct EvtRuntimeChild {
    u8 pad00[2];
    u16 unk02;
    u8 pad04[6];
    u16 groupTypeIndex; /* Reassigned consecutively across children of a group type. */
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
    u8 pad04[0xC];
    struct EvtModelOwner *owner; /* 0x10 */
    u8 pad14[0x3C];
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
    u8 pad22BC[0x14];
    s32 charRow; /* 0x22D0 */
    u8 pad22D4[0x20];
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
    u8 pad2328[0x68];
    s32 messageField;
    s32 compareField;
    s32 fieldIndex; /* 0x2398: selected column of the motion editor row */
    u8 pad239C[0x2C];
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
    s16 enabled;
    s8 tableValue; /* D_00368952 aliases this byte for indexed menu lookups. */
    u8 pad3[7];
} EvtTblEntry; /* 0xA bytes */
extern EvtTblEntry D_00368950[];
extern s8 D_00368952[];

extern u16 evtSkyTransitionActive;
extern u16 D_003BBE78;
extern u16 D_003BBE7A;

extern u32 evtSkyOverlayEnabled;

extern void kwlnDrawSpriteCell(s32 list, s32 x, s32 y, s32 w, s32 h);

extern char D_003BC0C8[]; /* "NAME:" */

/* Create a task with an initialized event payload. */
void evtCreateTask(s32 taskId, s32 value) {
    s32 data;

    data = evtAllocateContext();
    evtSetConvertedContextValue(data, value);
    kwlnTaskCreate(evtPictureTaskName, taskId, 1, 1, evtUpdatePictureWhenFlagged, evtPictureReleaseTaskTextureAndState, (void *)data);
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
    kwlnTaskCreate(evtPictureTaskName, taskId, 1, 1, evtUpdatePictureWhenFlagged, evtPictureReleaseTaskTextureAndState, data);
}

void evtSetSkyOverlayEnabled(u32 enabled) {
    evtSkyOverlayEnabled = enabled;
}

/* Either set the sky parameter immediately or interpolate from its current value. */
void evtBeginSkyParameterTransition(s32 duration, s32 target) {
    s16 current;

    current = func_00132B90();
    if (current != target) {
        if (duration == 0) {
            func_00132B80(target);
            evtSkyTransitionActive = 0;
        } else {
            evtSkyTransitionDuration = duration;
            evtSkyTransitionStartValue = current;
            evtSkyTransitionTargetValue = target;
            evtSkyTransitionActive = 1;
            evtSkyTransitionFrame = 0;
        }
    }
}

u16 evtIsSkyTransitionActive(void) {
    return evtSkyTransitionActive;
}

void evtAdvanceSkyTransition(void) {
    if (evtSkyTransitionActive != 0) {
        evtSkyTransitionFrame += 1;
        func_00132B80(evtSkyTransitionStartValue + (s32)((f32)(evtSkyTransitionTargetValue - evtSkyTransitionStartValue) * ((f32)evtSkyTransitionFrame / (f32)evtSkyTransitionDuration)));
        if (evtSkyTransitionFrame >= evtSkyTransitionDuration) {
            evtSkyTransitionActive = 0;
        }
    }
}

s32 evtUpdateSkyTask(void) {
    evtAdvanceSkyTransition();
    func_00132010();
    if (evtSkyOverlayEnabled != 0) {
        fldSelectDisplayBuffer(0x53);
        func_0012AEB0();
    }
    return 0;
}

void evtResetSkyTaskFlags(void) {
    evtSkyTransitionActive = 0;
    evtSkyOverlayEnabled = 0;
}

void evtDestroySkyTask(void) {
    s32 task;

    task = kwlnTaskGetTaskByName(evtSkyTaskName);
    if (task != 0) {
        kwlnTaskDestroyWithHierarchy(task, 1);
    }
}

void evtCreateSkyTask(void) {
    fldSetSwayMode(0);
    fldSetSkyDrawState(0x80);
    func_00132B80(0);
    fldSetFadeTarget(0, 1, 0);
    kwlnTaskCreate(evtSkyTaskName, 0x2B0E, 1, 1, evtUpdateSkyTask, evtResetSkyTaskFlags, 0);
}

INCLUDE_ASM(const s32, "game/code_00235270", evtUpdateFrameVariableTask);

void evtCreateFrameVariableTask(void) {
    kwlnTaskCreate("FrameVar", 0x2AF9, 1, 1, evtUpdateFrameVariableTask, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_00235270", func_00235598);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003ADDE0);

s32 evtAppendValueChangeDebugLabel(s32 list, s32 x, s32 y) {
    sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, "VALUE CHANGE."));
    return 2;
}

INCLUDE_ASM(const s32, "game/code_00235270", func_002357B8);

typedef struct MenuGfxCallback {
    u8 unknown[0x10];
    void (*invoke)(void *, void *);
} MenuGfxCallback;
extern MenuGfxCallback kwlnPositionedTextSurface;
extern u32 sdfCreateResetPacketList(void);
extern void func_00235598(u32, s32, s32, s32, s32, s32, s32, u8 *, void *, void *);
extern s8 D_00324510[];
extern void func_002357B8();

s32 evtViewerFloatValueUpdate(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;
    f32 step;

    list = sdfCreateResetPacketList();
    func_00235598(list, x, y, 0x16, 9, 0, 1, (u8 *)ctx, evtAppendValueChangeDebugLabel, func_002357B8);
    kwlnPositionedTextSurface.invoke(&kwlnPositionedTextSurface, (void *)list);
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

s32 evtDrawValueChangeNoticeRow(s32 list, s32 x, s32 y) {
    sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, "VALUE CHANGE."));
    return 2;
}

extern char D_003ADE40[];
extern char D_003ADE50[];
extern char D_003ADE78[];

INCLUDE_RODATA(const s32, "game/code_00235270", D_003ADE40);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003ADE50);

INCLUDE_RODATA(const s32, "game/code_00235270", jtbl_003ADE60);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003ADE78);

void evtDrawValueChangeInstructionRow(s32 list, s32 x, s32 y, u32 index, EvtRuntime *ctx) {
    switch (index) {
    case 0:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 4, D_003BC058, ctx->value));
        return;
    case 2:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003ADE78));
        return;
    case 3:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, " U,D = VALUE-+10"));
        return;
    case 4:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003ADE40));
        return;
    case 5:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003ADE50));
        break;
    }
}

s32 evtUpdateValueChangeDialog(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;
    s32 step;

    list = sdfCreateResetPacketList();
    func_00235598(list, x, y, 0x16, 9, 0, 1, (u8 *)ctx, evtDrawValueChangeNoticeRow, evtDrawValueChangeInstructionRow);
    kwlnPositionedTextSurface.invoke(&kwlnPositionedTextSurface, (void *)list);
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
    sdfAppendPacket(target, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, "FRAME CHANGE."));
    return 2;
}

void evtViewerDrawFrameChangeRow(s32 list, s32 x, s32 y, u32 index, EvtRuntime *ctx) {
    switch (index) {
    case 0:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 4, D_003BC058, ctx->value));
        return;
    case 2:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, " L,R = FRMAE-+"));
        return;
    case 3:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, " U,D = FRAME-+10"));
        return;
    case 4:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, "L1,R1= FRAME-+100"));
        return;
    case 5:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003ADE40));
        return;
    case 6:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003ADE50));
        return;
    case 7:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, " RL  = NOW FRAME"));
        return;
    case 8:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, " ST  = CAMERA FOCUS"));
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
    kwlnPositionedTextSurface.invoke(&kwlnPositionedTextSurface, (void *)list);
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
extern s32 kwlnStepTwoListCursors(s32, s32, s32, s32, s32, s32, s32, s32, u8 *);

s32 mnuDrawInfoWindowA(s32 x, s32 y, u8 *work) {
    u32 packets = sdfCreateResetPacketList();
    func_00235598(packets, x, y, 0xF, 0xB, 0, 0xB, work, 0, func_00236180);
    kwlnPositionedTextSurface.invoke(&kwlnPositionedTextSurface, (void *)packets);
    if (((EvtRuntime *)work)->actionMode != 1) {
        return 0;
    }
    return kwlnStepTwoListCursors(0, 1, 0xB, 1, 0xB, 0, 0, 0, work + 0x22A8);
}

extern char D_003BC088[];
extern char D_003BC090[];
extern char D_003BC0A0[];
extern u8 D_003688B8[];

typedef struct EvtDrawWork {
    u8 pad00[0x2280];
    s32 mode;
    u8 pad2284[0x38];
    s32 cursor;
    s32 itemCount;
    char *title;
    s32 *itemNames;
    s32 charCol;
    s32 charRow;
    u8 pad22D4[0x114];
    char *text0;
    char *text1;
} EvtDrawWork;

s32 evtDrawStringEntry(s32 output, s32 x, s32 y, EvtDrawWork *work) {
    if (work->title == 0) {
        return 0;
    }
    sdfAppendPacket(output, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003BC088, work->title));
    return 2;
}

void evtDrawSelectableTextRow(s32 list, s32 x, s32 y, s32 index, EvtDrawWork *work) {
    s32 color;

    if (index < work->itemCount) {
        if (work->cursor == index) {
            color = work->mode == 2 ? 4 : 5;
        } else {
            color = 0;
        }
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_003BC090, work->itemNames[index]));
    }
}

s32 evtUpdateTextSelectionDialog(s32 x, s32 y, EvtDrawWork *work) {
    u32 packets = sdfCreateResetPacketList();
    s32 width = 10;

    if (work->title != 0) {
        width = strlen(work->title);
        if (width < 6) {
            width = 6;
        }
    }
    func_00235598(packets, x, y, width, work->itemCount + 3, 0, work->itemCount, (u8 *)work, evtDrawStringEntry, evtDrawSelectableTextRow);
    kwlnPositionedTextSurface.invoke(&kwlnPositionedTextSurface, (void *)packets);
    if (work->mode != 2) {
        return 0;
    }
    return kwlnStepTwoListCursors(0, 1, work->itemCount, 1, work->itemCount, 0, 0, 0, (u8 *)work + 0x22BC);
}

s32 evtDrawInputValueRow(s32 list, s32 x, s32 y, u8 *ctx) {
    char text[16];
    func_003014F0(text, D_003BC098, (s32)ctx + 0x22D4, (s32)ctx + 0x22E0);
    sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003BC088, text));
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
        if (work->charRow == row && work->charCol == i) {
            color = work->mode == 3 ? 4 : 5;
        }
        ch = *table++;
        drawX = x;
        x += 0xC0;
        i++;
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(drawX, y, 0xFEFFFF, color, D_003BC0A0, ch));
    } while (i < 0xB);
}

INCLUDE_ASM(const s32, "game/code_00235270", func_00236828);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE0B8);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE0C8);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE0D8);

s32 evtDrawEventFileNameRow(s32 list, s32 x, s32 y) {
    char text[32];
    func_003014F0(text, "[E%3d_%03d.PM1+2+3]", D_003BBE78, D_003BBE7A);
    sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003BC088, text));
    return 2;
}

void evtDrawEventCutSelectRow(s32 list, s32 x, s32 y, u32 index, EvtRuntime *ctx) {
    s32 color = 0;

    if (ctx->charRow == index) {
        color = 4;
    }
    switch (index) {
    case 0:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0xE, D_003BC0A8));
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x + 0x600, y, 0xFEFFFF, color, D_003BC0B0, D_003BBE78));
        return;
    case 1:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0xE, D_003BC0B8));
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x + 0x600, y, 0xFEFFFF, color, D_003BC0C0, D_003BBE7A));
        return;
    case 3:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, " L,R = NO-+"));
        return;
    case 4:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, " U,D = SELECT"));
        return;
    case 5:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_003ADE40));
        return;
    case 6:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_003ADE50));
        return;
    case 7:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, " RL  = SET 600"));
        break;
    }
}

s32 evtUpdateEventCutSelectDialog(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;
    u32 num;

    list = sdfCreateResetPacketList();
    func_00235598(list, x, y, 0x12, 0xB, 0, 8, (u8 *)ctx, evtDrawEventFileNameRow, evtDrawEventCutSelectRow);
    kwlnPositionedTextSurface.invoke(&kwlnPositionedTextSurface, (void *)list);
    if (ctx->actionMode != 9) {
        return 0;
    }
    if (D_00324510[0x26] < 0) {
        ctx->charRow ^= 1;
    }
    if (D_00324510[0x27] < 0) {
        ctx->charRow ^= 1;
    }
    if (D_00324510[0x24] & 2) {
        switch (ctx->charRow) {
        case 0:
            if (D_003BBE78 >= 0x1F5) {
                D_003BBE78--;
            }
            break;
        case 1:
            if (D_003BBE7A >= 2) {
                D_003BBE7A--;
            }
            break;
        }
    }
    if (D_00324510[0x25] & 2) {
        switch (ctx->charRow) {
        case 0:
            if (D_003BBE78 < 0x3E7) {
                D_003BBE78++;
            }
            break;
        case 1:
            if (D_003BBE7A < 0x3E7) {
                D_003BBE7A++;
            }
            break;
        }
    }
    if (D_00324510[0x28] & 2) {
        switch (ctx->charRow) {
        case 0:
            if (D_003BBE78 >= 0x1F5) {
                num = (u16)(D_003BBE78 - 500);
                if (num < 0x1F4) {
                    D_003BBE78 = 0x1F4;
                } else {
                    D_003BBE78 -= 500;
                }
            }
            break;
        case 1:
            if (D_003BBE7A >= 2) {
                num = (u16)(D_003BBE7A - 10);
                if (num == 0) {
                    D_003BBE7A = 1;
                } else {
                    D_003BBE7A -= 10;
                }
            }
            break;
        }
    }
    if (D_00324510[0x2A] & 2) {
        switch (ctx->charRow) {
        case 0:
            D_003BBE78 += 100;
            break;
        case 1:
            D_003BBE7A += 10;
            break;
        }
    }
    if (D_00324510[0x21] < 0) {
        return 1;
    }
    if (D_00324510[0x23] < 0) {
        return -1;
    }
    if (D_00324510[0x20] < 0) {
        D_003BBE78 = 0x258;
    }
    return 0;
}

void evtDrawSelectedEntryLabel(s32 list, s32 *sel, s32 x, s32 unused, u8 *base) {
    x += 0x6C0;
    kwlnDrawSpriteCell(list, (x - 0x7000) >> 4, 0x3C, 0x1C, 3);
    sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, 0x7AE0, 0xFEFFFF, 0xE, D_003BC0C8));
    if (sel[2] >= 0) {
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x + 0x3C0, 0x7AE0, 0xFEFFFF, 0, D_003BC088, base + sel[2] * 32 + 0x24));
    }
}

INCLUDE_ASM(const s32, "game/code_00235270", func_00237130);

s32 evtUpdateEntrySelectionDialog(s32 x, s32 y, u8 *work) {
    u32 packets = sdfCreateResetPacketList();
    s32 count;
    s32 shown;

    func_00235598(packets, x, y, 8, 0x1D, ((EvtRuntime *)work)->entryFirst, ((EvtRuntime *)work)->entryCount, work, 0, func_00237130);
    kwlnPositionedTextSurface.invoke(&kwlnPositionedTextSurface, (void *)packets);
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
    return kwlnStepTwoListCursors(0, 1, count, 1, shown, 0, (s32)(work + 0x22F8), 0, work + 0x22F4);
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
            sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color ? color : 8, "----- NEW FRAME -----"));
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
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, " -----------------------"));
        return;
    }
    count = 0;
    for (i = 0; i < 0x12; i++) {
        if (i != EVT_WORLD_SLOT_MOVIE) {
            for (node = dds3GetWorldObject()->table->slots[i].head; node != NULL; node = node->next) {
                if (node->name != NULL) {
                    count++;
                    if (count == index) {
                        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_003BC090, node->name));
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
extern EvtWorldNode *dds3FindObjectChainNodeByName(EvtWorldObject *world, char *name);

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
        node = dds3FindObjectChainNodeByName(dds3GetWorldObject(), ctx->entryName[slot]);
    }
    sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0xE, D_003BC240, index));
    if (node != NULL) {
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_003BC248, node->name));
    } else {
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, "   -----------------------"));
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
    kwlnPositionedTextSurface.invoke(&kwlnPositionedTextSurface, (void *)packets);
    if (((EvtRuntime *)work)->actionMode != 0xC) {
        return 0;
    }
    return kwlnStepTwoListCursors(0, 1, rows, 1, rows, 0, 0, 0, work + 0x22B8);
}

typedef struct EvtMessageWindow {
    u8 pad00[0x104];
    s32 entryHandle;
} EvtMessageWindow;

typedef struct EvtMessageMenuWork {
    u8 pad00[8];
    EvtMessageWindow *window;
} EvtMessageMenuWork;

s32 mnuDrawMessageMenuLabel(s32 list, s32 x, s32 y, EvtMessageMenuWork *work) {
    s32 count = itfMesGetEntryCount(work->window->entryHandle);
    sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, "MESSAGE MENU (MESMAX %3d)", count));
    return 2;
}


extern char D_003BC250[];
extern char D_003BC258[];
extern char D_003BC260[];
extern char D_003AE8D0[];
extern char D_003AE8E0[];
extern char D_003AE8F0[];
extern char D_003AE900[];
extern char D_003AE910[];
extern char D_003AE920[];
extern char D_003AE930[];
extern char D_003AE940[];
extern char D_003AE950[];
extern char D_003AE960[];
extern s32 itfMesGetWindowEntryItems(s32, s32);

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

void func_00238ED0(s32 list, s32 x, s32 y, u32 kind, EvtRuntime *ctx) {
    char *names[11] = {D_003BC250, D_003AE8D0, D_003AE8E0, D_003AE8F0,
        D_003AE900, D_003AE910, D_003AE920, D_003AE930, D_003AE940, D_003AE950, D_003AE960};

    switch (kind) {
    case 0: {
        s32 color = 4;
        if (ctx->messageField != 0) {
            color = 0;
        }
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_003BC058, ctx->value & 0xFFF));
        if (((EvtMessageWindow *)ctx->windowContext)->entryHandle == -1) {
            sdfAppendPacket(list, sdfCreateFormattedSifCommand(x + 0x600, y, 0xFEFFFF, color, "NONE MESDATA!!"));
        } else if (itfMesGetWindowEntryItems(((EvtMessageWindow *)ctx->windowContext)->entryHandle, ctx->value & 0xFFF) == 0) {
            sdfAppendPacket(list, sdfCreateFormattedSifCommand(x + 0x600, y, 0xFEFFFF, color, "(NORMAL)"));
        } else {
            sdfAppendPacket(list, sdfCreateFormattedSifCommand(x + 0x600, y, 0xFEFFFF, color, "(BRANCH)"));
        }
        break;
    }
    case 1: {
        s32 color = 4;
        if (ctx->messageField != 1) {
            color = 0;
        }
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_003BC258, names[(ctx->value >> 12) & 0xF]));
        break;
    }
    case 3:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003ADE78));
        break;
    case 4:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, " U,D = SELECT "));
        break;
    case 5:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003ADE40));
        break;
    case 6:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003ADE50));
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_00235270", func_00239148);



s32 mnuDrawCutFlagLabel(s32 target, s32 x, s32 y) {
    sdfAppendPacket(target, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, "CUTFLAG MENU"));
    return 2;
}


void func_002393A0(s32 list, s32 x, s32 y, u32 index, EvtRuntime *ctx) {
    char *labels[11] = {D_003BC260, D_003AE8D0, D_003AE8E0, D_003AE8F0,
        D_003AE900, D_003AE910, D_003AE920, D_003AE930, D_003AE940, D_003AE950, D_003AE960};
    s32 flag;

    switch (index) {
    case 0:
        flag = ctx->compareField != 0 ? 0 : 4;
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, flag, D_003BC258, labels[(ctx->value >> 12) & 0xF]));
        return;
    case 1:
        flag = ctx->compareField != 1 ? 0 : 4;
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, flag, D_003BC058, ctx->value & 0xFFF));
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x + 0x600, y, 0xFEFFFF, flag, "  (CMP VALUE)"));
        return;
    case 3:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003ADE78));
        return;
    case 4:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, " U,D = SELECT "));
        return;
    case 5:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003ADE40));
        return;
    case 6:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003ADE50));
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_00235270", func_002395A8);

s32 evtIsMenuTableEntryEnabled(s32 *index) {
    return D_00368950[*index].enabled != 0;
}

s32 mnuGetSelectedTableValue(EvtRuntime *runtime) {
    return D_00368952[runtime->tableColumn + runtime->frameList->kind * 10];
}

INCLUDE_ASM(const s32, "game/code_00235270", func_002397C8);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AEA70);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AEA80);

INCLUDE_ASM(const s32, "game/code_00235270", func_00239A90);

s32 mnuDrawMotionChangeLabel(s32 target, s32 x, s32 y) {
    sdfAppendPacket(target, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, "MOTION CHANGE MENU"));
    return 2;
}

typedef struct EvtMotionBits {
    s32 group : 8;
    s32 motion : 8;
    s32 loop : 8;
    s32 hokan : 8;
} EvtMotionBits;

typedef union EvtMotionValue {
    s32 word;
    EvtMotionBits bits;
} EvtMotionValue;

typedef struct EvtModelRef {
    u8 pad00[0xC];
    s32 handle; /* 0xC */
} EvtModelRef;

typedef struct EvtModelSlot {
    u8 pad00[0xC];
    EvtModelRef *ref; /* 0xC */
} EvtModelSlot;

struct EvtModelOwner {
    u8 pad00[0x18];
    EvtModelSlot *slot; /* 0x18 */
};

typedef struct EvtMotionData {
    u8 pad00[4];
    s32 **frames; /* 0x4 */
} EvtMotionData;

typedef struct EvtMotionNode {
    u8 pad00[8];
    EvtMotionData *data; /* 0x8 */
} EvtMotionNode;

extern EvtMotionNode *mdlFindNodeById(s32 model, s32 index);
extern s32 mdlGetNodeRefHalf(s32 model, s32 index);

INCLUDE_ASM(const s32, "game/code_00235270", func_00239E30);

extern void func_00239E30();

/* Motion editor row: ctx->value packs group (byte 0), motion number (byte 1), loop flag (byte 2) and interpolation (byte 3). */
s32 evtUpdateMotionChangeRow(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;
    s32 model;
    s32 count;
    EvtMotionValue packed;

    list = sdfCreateResetPacketList();
    model = ctx->frameList->owner->slot->ref->handle;
    func_00235598(list, x, y, 0x14, 0xA, 0, 1, (u8 *)ctx, mnuDrawMotionChangeLabel, func_00239E30);
    kwlnPositionedTextSurface.invoke(&kwlnPositionedTextSurface, (void *)list);
    if (ctx->actionMode != 0x10) {
        return 0;
    }
    if (D_00324510[0x26] & 2) {
        if (ctx->fieldIndex == 0) {
            ctx->fieldIndex = 3;
        } else {
            ctx->fieldIndex = ctx->fieldIndex - 1;
        }
    } else if (D_00324510[0x27] & 2) {
        if (ctx->fieldIndex == 3) {
            ctx->fieldIndex = 0;
        } else {
            ctx->fieldIndex = ctx->fieldIndex + 1;
        }
    }
    packed.word = ctx->value;
    count = mdlGetNodeRefHalf(model, packed.bits.group);
    switch (ctx->fieldIndex) {
    case 0:
        if (D_00324510[0x25] & 2) {
            do {
                if (packed.bits.group < 3) {
                    packed.bits.group = packed.bits.group + 1;
                } else {
                    packed.bits.group = 0;
                }
            } while (mdlFindNodeById(model, packed.bits.group) == NULL);
        } else if (D_00324510[0x24] & 2) {
            do {
                if (packed.bits.group > 0) {
                    packed.bits.group = packed.bits.group - 1;
                } else {
                    packed.bits.group = 3;
                }
            } while (mdlFindNodeById(model, packed.bits.group) == NULL);
        }
        if (packed.bits.motion >= mdlGetNodeRefHalf(model, packed.bits.group)) {
            packed.bits.motion = 0;
        }
        break;
    case 1:
        if (D_00324510[0x25] & 2) {
            if (packed.bits.motion >= count - 1) {
                packed.bits.motion = 0;
            } else {
                packed.bits.motion = packed.bits.motion + 1;
            }
        } else if (D_00324510[0x24] & 2) {
            if (packed.bits.motion > 0) {
                packed.bits.motion = packed.bits.motion - 1;
            } else {
                packed.bits.motion = count - 1;
            }
        }
        break;
    case 2:
        if ((D_00324510[0x25] & 2) || (D_00324510[0x24] & 2)) {
            packed.bits.loop = packed.bits.loop == 0;
        }
        break;
    case 3:
        if (D_00324510[0x25] & 2) {
            if (packed.bits.hokan < 0x64) {
                packed.bits.hokan = packed.bits.hokan + 1;
            } else {
                packed.bits.hokan = 0;
            }
        }
        if (D_00324510[0x24] & 2) {
            if (packed.bits.hokan > 0) {
                packed.bits.hokan = packed.bits.hokan - 1;
            } else {
                packed.bits.hokan = 0x64;
            }
        }
        break;
    }
    ctx->value = packed.word;
    if (D_00324510[0x21] < 0 && mdlFindNodeById(model, packed.bits.group)->data->frames[packed.bits.motion] != NULL) {
        return 1;
    }
    return D_00324510[0x23] >= 0 ? 0 : -1;
}

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
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_003BC288));
    } else if (index == 1) {
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_003BC280));
        return;
    } else if (index == 2) {
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_003AEA80));
        return;
    }
    n = 3;
    for (group = ctx->groups; group != NULL; group = group->next) {
        if (group->type == 1) {
            if (n == index) {
                sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_003BC088, group->info->name));
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

void evtDrawOptionalPromptText(s32 list, s32 x, s32 y, s32 kind, EvtDrawWork *work) {
    switch (kind) {
    case 0:
        if (work->text0 != 0) {
            sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003BC088, work->text0));
        }
        return;
    case 1:
        if (work->text1 != 0) {
            sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003BC088, work->text1));
        }
        break;
    }
}

extern s8 D_00324510[];

s32 mnuDrawTimedPrompt(s32 x, s32 y, u8 *work) {
    u32 packets = sdfCreateResetPacketList();
    s32 count;
    func_00235598(packets, x, y, 0x19, 2, 0, 1, work, 0, evtDrawOptionalPromptText);
    kwlnPositionedTextSurface.invoke(&kwlnPositionedTextSurface, (void *)packets);
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
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_003BC300));
        return;
    }
    if (index == 1) {
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, "DEFFAULT"));
        return;
    }
    n = 2;
    for (group = ctx->groups; group != NULL; group = group->next) {
        if (group->type == 0x18) {
            if (n == index) {
                sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_003BC088, ctx->entryName[group->entryHeader.word]));
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
extern s32 evtActiveEntryFlags;

s32 evtSynchronizeSelectedEntry(s32 task) {
    EvtRuntime *runtime = kwlnTaskGetUserValue();
    if (func_0018FEB0() == 0) {
        runtime->controlState = 0;
        return -1;
    }
    if (kwlnTaskGetTimer(task) == 0) {
        s32 selected = runtime->selectedEntry;
        evtActiveEntryFlags = selected;
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

s32 evtPollRuntimeControlReady(void) {
    EvtRuntime *runtime;

    runtime = kwlnTaskGetUserValue();
    if (effEventAdvanceSolidRectangleSetup() == 0) {
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

s32 evtAssignRuntimeChildSequenceAndCount(EvtRuntime *runtime) {
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

s32 evtIndexGroupTypeElevenChildren(EvtRuntime *runtime) {
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

s32 evtIndexGroupTypeThirteenChildren(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xD) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->groupTypeIndex = index++;
            }
        }
    }
    return index;
}

s32 evtIndexGroupTypeFourteenChildren(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xE) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->groupTypeIndex = index++;
            }
        }
    }
    return index;
}

s32 evtIndexGroupTypeFifteenChildren(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xF) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->groupTypeIndex = index++;
            }
        }
    }
    return index;
}

s32 evtIndexGroupTypeTwentyThreeChildren(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x17) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->groupTypeIndex = index++;
            }
        }
    }
    return index;
}

s32 evtIndexGroupTypeTwentySevenChildren(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x1B) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->groupTypeIndex = index++;
            }
        }
    }
    return index;
}

s32 evtIndexGroupTypeSixteenChildren(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x10) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->groupTypeIndex = index++;
            }
        }
    }
    return index;
}

s32 evtIndexGroupTypeSeventeenChildren(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x11) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->groupTypeIndex = index++;
            }
        }
    }
    return index;
}

s32 evtIndexGroupTypeTwentyFiveChildren(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x19) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->groupTypeIndex = index++;
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

void evtCopyRuntimeChildPayloadsToBuffer(s32 output, EvtRuntime *runtime) {
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

void evtEmitGroupTypeElevenPayloads(s32 output, EvtRuntime *runtime) {
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

void evtEmitGroupTypeThirteenPayloads(s32 output, EvtRuntime *runtime) {
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

void evtEmitGroupTypeFourteenPayloads(s32 output, EvtRuntime *runtime) {
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

void evtEmitGroupTypeFifteenPayloads(s32 output, EvtRuntime *runtime) {
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

void evtEmitGroupTypeTwentyThreePayloads(s32 output, EvtRuntime *runtime) {
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

void evtEmitGroupTypeTwentySevenPayloads(s32 output, EvtRuntime *runtime) {
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

void evtEmitGroupTypeSixteenPayloads(s32 output, EvtRuntime *runtime) {
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

void evtEmitGroupTypeSeventeenPayloads(s32 output, EvtRuntime *runtime) {
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

void evtEmitGroupTypeTwentyFivePayloads(s32 output, EvtRuntime *runtime) {
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

u16 evtGetRowValue(s32 table, s32 row) {
    if (((EvtRowDescriptor *)((EvtRowTable *)table)->descriptor)->format == 4) {
        return ((EvtCompactRow *)(((EvtRowTable *)table)->compactRows + row * 0x10))->value;
    }
    return ((EvtExtendedRow *)(((EvtRowTable *)table)->extendedRows + row * 0x2c))->value;
}

s16 evtGetRowVariant(s32 table, s32 row) {
    if (((EvtRowDescriptor *)((EvtRowTable *)table)->descriptor)->format == 4) {
        return ((EvtCompactRow *)(((EvtRowTable *)table)->compactRows + row * 0x10))->variant;
    }
    return ((EvtExtendedRow *)(((EvtRowTable *)table)->extendedRows + row * 0x2c))->variant;
}

u16 evtGetRowParameter(s32 table, s32 row) {
    if (((EvtRowDescriptor *)((EvtRowTable *)table)->descriptor)->format == 4) {
        return ((EvtCompactRow *)(((EvtRowTable *)table)->compactRows + row * 0x10))->parameter;
    }
    return ((EvtExtendedRow *)(((EvtRowTable *)table)->extendedRows + row * 0x2c))->parameter;
}

u16 evtGetRowFlags(s32 table, s32 row) {
    if (((EvtRowDescriptor *)((EvtRowTable *)table)->descriptor)->format == 4) {
        return ((EvtCompactRow *)(((EvtRowTable *)table)->compactRows + row * 0x10))->flags;
    }
    return ((EvtExtendedRow *)(((EvtRowTable *)table)->extendedRows + row * 0x2c))->flags;
}

s32 evtGetRowPayloadAddress(s32 table, s32 row) {
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

extern void mnuReleaseCampSceneRegisteredIds(EvtRuntime *runtime);
extern void mnuStopMovieDrawTask(void);
extern s32 sdfCheckPendingWorkWithInterrupts(void);
extern void sdfQueueNonzeroResourceId(s32 resource);
extern void kwlnTextureReleaseHeldReference(void);
extern u32 kwlnDrawControlFlags;
extern void evtFormatPolygonMoviePaths(u16 a, u16 b, char *path0, char *path1, char *path2);
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

    mnuReleaseCampSceneRegisteredIds(runtime);
    if (runtime->timedActive == 1) {
        mnuStopMovieDrawTask();
        runtime->timedActive = 0;
    }
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    if (runtime->pendingResource != 0) {
        sdfQueueNonzeroResourceId(runtime->pendingResource);
        runtime->pendingResource = 0;
        runtime->pendingWork = 0;
    }
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    kwlnTextureReleaseHeldReference();
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    kwlnDrawControlFlags |= 0x2000000;
    evtFormatPolygonMoviePaths(D_003BBE78, D_003BBE7A, path0, path1, path2);
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
    sndEnsureMidiBankResident(sound);
    return sound;
}

s32 evtIsBgmLoaded(s32 id) {
    if ((u32)(id - 0x258) >= 0x100) {
        return 1;
    }
    return sndFindPackedTrackLoadStatus(evtEncodeBgmSoundCode(id, 0)) == 1;
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

s32 evtQueueValidatedBgmSoundCode(s32 id, s32 fade) {
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

s32 evtStartBgmBySoundIdAndFade(s32 id, s32 fade) {
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
        return ((EvtTaskData *)kwlnTaskGetUserValue(task))->value;
    } else {
        return -1;
    }
}

EvtTaskData *evtGetTaskData(u32 taskId) {
    s32 task;

    task = evtFindTaskById(taskId);
    if (task != 0) {
        return kwlnTaskGetUserValue(task);
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

s32 evtFindTaskResourceEntryByKey(u32 taskId, s32 key) {
    s32 i;
    EvtResourceTask *data;
    s32 task;

    task = evtFindTaskById(taskId);
    if (task != 0) {
        data = kwlnTaskGetUserValue(task);
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
extern s32 sdfTexAcquireResourceTexture(s32 resource);
extern void effSetCh72Id(s32 id);

void evtRefreshTaskData(s32 taskId, s32 key) {
    EvtTaskData *data = evtGetTaskData(taskId);
    s32 resource = evtFindTaskResourceEntryByKey(taskId, key);
    if (resource != 0) {
        s32 old = data->effectHandle;
        if (old != 0) {
            sdfTexReleaseReferenceViaHandler(old);
            data->effectHandle = 0;
        }
        resource = sdfTexAcquireResourceTexture(resource);
        effSetCh72Id(resource);
        data->effectHandle = resource;
    }
}

/* Party/enemy model table entry (0x270 bytes); only the scale-source field is known here. */
typedef struct Entry270 {
    u8 pad00[0x18];
    f32 modelScale;
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
    found = (void *)evtFindTaskResourceEntryByKey(taskId, key);
    if (found != 0) {
        obj = func_00115298(found, vecA, vecB);
        if (obj != 0) {
            effObjSetFlags(obj, 1);
            if (index >= 0) {
                inner = ((EvtEffectObject *)obj)->inner;
                func_00190308(inner->scaledObject, D_003BAA20[index].modelScale / D_003BAA10->modelScale);
            }
            return obj;
        }
        return obj;
    }
    return found;
}

INCLUDE_SDATA(const s32, "game/code_00235270", evtPictureTaskName);

INCLUDE_SDATA(const s32, "game/code_00235270", evtSkyTransitionActive);

INCLUDE_SDATA(const s32, "game/code_00235270", evtSkyOverlayEnabled);

INCLUDE_SDATA(const s32, "game/code_00235270", evtSkyTaskName);

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

