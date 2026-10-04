#include "common.h"
#include "evt_world.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"

extern u32 evtSkyOverlayEnabled;

extern u16 evtSkyTransitionActive;

extern void func_00135588(s32 value);

extern s16 func_00135598(void);

extern u16 evtSkyTransitionFrame;

extern u16 evtSkyTransitionDuration;

extern s16 evtSkyTransitionStartValue;

extern s16 evtSkyTransitionTargetValue;

extern void func_00134A18(void);

extern void fldSelectDisplayBuffer(s32 id);

extern void func_0012D3E0(void);

extern void sdfAppendPacket(s32 list, s32 packet);

extern s32 sdfCreateFormattedSifCommand();

typedef struct {
    s16 enabled;
    s8 tableValue; /* D_003C9732 aliases this byte for indexed menu lookups. */
    u8 pad3[7];
} EvtTblEntry; /* 0xA bytes */

extern EvtTblEntry D_003C9730[];

extern s8 D_003C9732[];

extern void *kwlnTaskGetUserValue();

extern s32 effEventAdvanceSolidRectangleSetup(void);

/* Native viewer child node; frame rows traverse this same linked record. */
typedef struct EvtRuntimeChild {
    u16 unk00;
    u16 groupTypeBIndex; /* Consecutive index among children of type 0xB groups. */
    u16 unk04;
    u8 pad06[2];
    union {
        s32 words[8];
        struct {
            u8 pad00[2];
            u16 groupTypeIndex; /* Reassigned consecutively across children of a group type. */
            u8 pad04[6];
            u16 groupTypeAIndex; /* Consecutive index among children of type 0xA groups. */
        } f;
    } body; /* 0x08 */
    u8 pad28[4];
    void *payload; /* 0x2C: serialized child data */
    struct EvtRuntimeChild *next; /* 0x30 */
    struct EvtRuntimeChild *prev; /* 0x34 */
} EvtRuntimeChild;

/* World-slot node borrowed by a viewer group. Its data is slot-specific;
 * model groups use EvtModelSlot, while all named nodes share the list links. */
typedef struct EvtWorldNode {
    u8 pad00[8];
    char *name; /* 0x08 */
    u8 pad0C[0xC];
    void *data; /* 0x18 */
    u8 pad1C[4];
    struct EvtWorldNode *next; /* 0x20 */
} EvtWorldNode;

/* The native 0x84-byte viewer entry owns its child list and borrows info.
 * EvtRuntime.frameGroup selects one of these entries, not a separate list. */
typedef struct EvtRuntimeGroup {
    s32 type;
    u8 value04;
    u8 pad05[3];
    s32 value08;
    u8 pad0C[4];
    EvtWorldNode *info; /* 0x10 */
    u8 pad14[8];
    u16 value1C;
    u8 value1E;
    u8 value1F;
    u8 pad20[0x30];
    s32 childCount; /* 0x50 */
    EvtRuntimeChild *children; /* 0x54 */
    EvtRuntimeChild *lastChild; /* 0x58 */
    u8 pad5C[0x20];
    struct EvtRuntimeGroup *next; /* 0x7C */
    struct EvtRuntimeGroup *prev; /* 0x80 */
} EvtRuntimeGroup;

/* The command API stores words; timed-prompt drawing consumes their text pointers. */
typedef union EvtCommandArgument {
    s32 word;
    char *text;
} EvtCommandArgument;

/* The file header emits this whole word; type-8 spans use its low halfword. */
typedef union EvtFrameRange {
    s32 word;
    struct {
        u16 end;
        u16 unk02;
    } f;
} EvtFrameRange;
/* Native viewer runtime: dialogs, task polls and file writers consume this same
 * record. The recovered extent includes the trailing serialized metadata word. */
typedef struct EvtRuntime {
    u8 pad0000[4];
    u32 flags; /* 0x04 */
    struct EvtMessageWindow *windowContext; /* 0x08: message window context */
    s32 headerThird; /* 0x0C: third emitted header word */
    s32 headerFirst; /* 0x10: first emitted header word */
    EvtFrameRange frameRange; /* 0x14: second header word and terminal span value */
    s32 curFrame; /* 0x18 */
    u8 pad001C[0x4];
    s32 entryTotal; /* 0x20 */
    char entryName[256][32]; /* 0x24 */
    u8 pad2024[0xC];
    s32 entryCount; /* 0x2030 */
    EvtRuntimeGroup *groups; /* 0x2034 */
    u8 pad2038[0x248];
    s32 actionMode; /* 0x2280 */
    u8 pad2284[0x8];
    s32 controlState; /* 0x228C */
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
    EvtRuntimeGroup *frameGroup; /* 0x2308: selected entry from groups */
    u8 pad230C[0x4];
    s32 value; /* 0x2310: modes D/E pack a 12-bit number and 4-bit option */
    s32 valueMin; /* 0x2314 */
    s32 valueMax; /* 0x2318 */
    f32 floatValue; /* 0x231C */
    f32 floatMin; /* 0x2320 */
    f32 floatMax; /* 0x2324 */
    u8 pad2328[0x68];
    s32 messageField;
    s32 compareField; /* 0x2394 */
    s32 fieldIndex; /* 0x2398: selected column of the motion editor row */
    u8 pad239C[0x2C];
    s32 tableColumn; /* 0x23C8: index within selected table row */
    u8 pad23CC[0x14];
    s32 selectedEntry; /* 0x23E0 */
    s32 commandFirst; /* 0x23E4 */
    EvtCommandArgument commandSecond; /* 0x23E8 */
    EvtCommandArgument commandThird; /* 0x23EC */
    u8 pad23F0[0x24];
    s32 timedActive; /* 0x2414 */
    u8 pad2418[0x8];
    u8 shadowMode; /* 0x2420 */
    u8 shadowAlpha; /* 0x2421 */
    u8 pad2422[0x2];
    f32 shadowY; /* 0x2424 */
    s32 pendingWork; /* 0x2428 */
    s32 pendingResource; /* 0x242C */
    u8 pad2430[0xC];
    s32 headerMetadata; /* 0x243C: fourth emitted header word */
} EvtRuntime;
extern s32 effUpdateCh72Params(void);
extern s32 effEventAdvanceBlurTemplateSetup(void);
extern s32 effEventAdvanceScatterBlurSetup(void);
extern s32 effEventAdvanceScaleBlurSetup(void);
extern s32 func_001978B8(void);
extern s32 func_002570F8(EvtRuntime *runtime);
extern s32 func_00257910(EvtRuntime *runtime);
extern s32 func_002582D0(EvtRuntime *runtime);
extern void *D_004364B0;
extern void *D_0043653C;
extern void *D_00436520;


typedef struct GsSurface {
    u8 pad00[0x10];
    void (*submit)(struct GsSurface *, s32);
} GsSurface;

/* Header callbacks have three-argument labels and context-aware variants. */
typedef s32 (*EvtMenuHeaderFn)();
typedef void (*EvtMenuRowFn)(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx);

typedef struct EvtPad {
    u8 pad00[0x20];
    s8 syncKey; /* 0x20 */
    s8 confirm; /* 0x21 */
    u8 pad22;
    s8 cancel;  /* 0x23 */
    s8 decOne;  /* 0x24 */
    s8 incOne;  /* 0x25 */
    s8 decTen;  /* 0x26 */
    s8 incTen;  /* 0x27 */
    u8 decHun;  /* 0x28 */
    u8 pad29;
    u8 incHun;  /* 0x2A */
    u8 pad2B;
    s8 apply;   /* 0x2C */
} EvtPad;

extern EvtPad D_0037F510;
extern GsSurface kwlnPositionedTextSurface;
extern char D_004233F0[]; /* " RR  = ENTER" */
extern char D_00423400[]; /* " RD  = CANCEL" */
extern char D_00423428[]; /* " L,R = VALUE-+" */
extern char D_004374A0[]; /* "     %d" */
extern char D_004374F0[]; /* " EVENT" */
extern char D_004374F8[]; /* "%3d" */
extern char D_00437500[]; /* "   CUT" */
extern char D_00437508[]; /* "%03d" */
extern s32 sdfCreateResetPacketList(void);
extern void func_00250338(s32 list, s32 x, s32 y, s32 col, s32 rows, s32 first, s32 total, EvtRuntime *ctx,
                          EvtMenuHeaderFn header, EvtMenuRowFn row);
extern void kwlnDrawSpriteCell(s32 list, s32 x, s32 y, s32 w, s32 h);
extern s32 kwlnStepTwoListCursors(s32, s32, s32, s32, s32, s32, s32 *, s32, s32 *);

extern void sndEnsureMidiBankResident(s32 sound);

extern s32 sndFindPackedTrackLoadStatus(s32 sound);

extern void sndStartTrackDefault(s32 track);

extern void func_0035B6E0(const char *fmt, ...);

extern void sndStartTrackExtended(s32 track);

extern void func_00342580(u32 sound);

extern void sndSetSequenceVolumePan(s32 sequence, s32 volume, s32 pan);

extern void func_00341C78(u32 sound);

extern s32 (*D_003C9928[])(s32, s32, void *);

extern char evtPictureTaskName[];
extern void evtUpdatePictureWhenFlagged();
extern void evtPictureReleaseTaskTextureAndState();
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
    kwlnTaskCreate(evtPictureTaskName, taskId, 1, 1, (s32)evtUpdatePictureWhenFlagged, (s32)evtPictureReleaseTaskTextureAndState, taskData);
}

extern s32 kwlnTaskCreate(char *name, s32 taskId, s32 arg2, s32 arg3, s32 update, s32 destroy, s32 data);
extern char evtPictureTaskName[];
extern void evtUpdatePictureWhenFlagged();
extern void evtPictureReleaseTaskTextureAndState();
extern u8 *evtAllocateContext();

void evtCreateTaskWithValue(s32 taskId, s32 value) {
    EvtTaskData *taskData = (EvtTaskData *)evtAllocateContext();
    taskData->value = value;
    kwlnTaskCreate(evtPictureTaskName, taskId, 1, 1, (s32)evtUpdatePictureWhenFlagged, (s32)evtPictureReleaseTaskTextureAndState, (s32)taskData);
}

void evtSetSkyOverlayEnabled(u32 enabled) {
    evtSkyOverlayEnabled = enabled;
}

/* Either set the sky alpha immediately or interpolate from its current value. */
void evtBeginSkyParameterTransition(s32 duration, s32 target) {
    s16 current;

    current = func_00135598();
    if (current != target) {
        if (duration == 0) {
            func_00135588(target);
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
        func_00135588(evtSkyTransitionStartValue + (s32)((f32)(evtSkyTransitionTargetValue - evtSkyTransitionStartValue) * ((f32)evtSkyTransitionFrame / (f32)evtSkyTransitionDuration)));
        if (evtSkyTransitionFrame >= evtSkyTransitionDuration) {
            evtSkyTransitionActive = 0;
        }
    }
}

s32 evtUpdateSkyTask(void) {
    evtAdvanceSkyTransition();
    func_00134A18();
    if (evtSkyOverlayEnabled != 0) {
        fldSelectDisplayBuffer(0x53);
        func_0012D3E0();
    }
    return 0;
}

void evtResetSkyTaskFlags(void) {
    evtSkyTransitionActive = 0;
    evtSkyOverlayEnabled = 0;
}

extern char evtSkyTaskName[];

void evtDestroySkyTask(void) {
    s32 task = kwlnTaskGetTaskByName(evtSkyTaskName);
    if (task != 0) {
        kwlnTaskDestroyWithHierarchy(task, 1);
    }
}

extern void fldSetSwayMode();
extern void fldSetSkyDrawState();
extern void func_00135588();
extern void fldSetFadeTarget();

void evtCreateSkyTask(void) {
    fldSetSwayMode(0);
    fldSetSkyDrawState(0x80);
    func_00135588(0);
    fldSetFadeTarget(0, 1, 0);
    kwlnTaskCreate(evtSkyTaskName, 0x2B0E, 1, 1, (s32)evtUpdateSkyTask, (s32)evtResetSkyTaskFlags, 0);
}

INCLUDE_ASM(const s32, "game/code_00250010", evtUpdateFrameVariableTask);

extern s32 kwlnTaskCreate(char *name, s32 taskId, s32 arg2, s32 arg3, s32 update, s32 destroy, s32 data);
extern s32 evtUpdateFrameVariableTask();
extern char D_00423380[]; /* "FrameVar" */

void evtCreateFrameVariableTask(void) {
    kwlnTaskCreate(D_00423380, 0x2AF9, 1, 1, (s32)evtUpdateFrameVariableTask, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00250338);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423380);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423390);

s32 evtAppendValueChangeDebugLabel(s32 list, s32 x, s32 y) {
    sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, "VALUE CHANGE."));
    return 2;
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00250558);

extern s32 func_00250558(s32, s32, s32, s32, EvtRuntime *);
/* Edit the bounded float only in mode 8. Confirm precedes cancel; coarse steps
 * replace fine steps before the value is clamped to the runtime limits. */
s32 evtViewerFloatValueUpdate(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;
    f32 step;

    list = sdfCreateResetPacketList();
    func_00250338(list, x, y, 0x16, 9, 0, 1, ctx, evtAppendValueChangeDebugLabel, func_00250558);
    kwlnPositionedTextSurface.submit(&kwlnPositionedTextSurface, list);
    if (ctx->actionMode != 8) {
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

INCLUDE_RODATA(const s32, "game/code_00250010", D_004233F0);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423400);

INCLUDE_RODATA(const s32, "game/code_00250010", jtbl_00423410);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423428);

void evtDrawValueChangeInstructionRow(s32 list, s32 x, s32 y, u32 index, EvtRuntime *ctx) {
    switch (index) {
    case 0:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 4, D_004374A0, ctx->value));
        return;
    case 2:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_00423428));
        return;
    case 3:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, " U,D = VALUE-+10"));
        return;
    case 4:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_004233F0));
        return;
    case 5:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_00423400));
        break;
    }
}

extern s32 evtDrawValueChangeNoticeRow(s32 list, s32 x, s32 y);
extern void evtDrawValueChangeInstructionRow(s32 list, s32 x, s32 y, u32 index, EvtRuntime *ctx);

s32 evtUpdateValueChangeDialog(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;
    s32 step;

    list = sdfCreateResetPacketList();
    func_00250338(list, x, y, 0x16, 9, 0, 1, ctx, evtDrawValueChangeNoticeRow, evtDrawValueChangeInstructionRow);
    kwlnPositionedTextSurface.submit(&kwlnPositionedTextSurface, list);
    if (ctx->actionMode != 7) {
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
    sdfAppendPacket(target, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, "FRAME CHANGE."));
    return 2;
}

void evtViewerDrawFrameChangeRow(s32 list, s32 x, s32 y, u32 index, EvtRuntime *ctx) {
    switch (index) {
    case 0:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 4, D_004374A0, ctx->value));
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
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_004233F0));
        return;
    case 6:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_00423400));
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

extern void evtViewerDispatchFlagMode();
extern void func_00249088();
extern void evtViewerDrawFrameChangeRow(s32 list, s32 x, s32 y, u32 index, EvtRuntime *ctx);

s32 evtViewerFrameChangeUpdate(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;
    s32 step;

    list = sdfCreateResetPacketList();
    func_00250338(list, x, y, 0x16, 0xB, 0, 1, ctx, mnuDrawFrameChangeLabel, evtViewerDrawFrameChangeRow);
    kwlnPositionedTextSurface.submit(&kwlnPositionedTextSurface, list);
    if (ctx->actionMode != 6) {
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
    kwlnPositionedTextSurface.submit(&kwlnPositionedTextSurface, list);
    if (ctx->actionMode != 1) {
        return 0;
    }
    return kwlnStepTwoListCursors(0, 1, 0xB, 1, 0xB, 0, 0, 0, &ctx->inputA);
}

extern char D_004374D0[];

/* Draw the runtime title when present; return two rows used, or zero for no title. */
s32 evtDrawStringEntry(s32 list, s32 x, s32 y, EvtRuntime *ctx) {
    if (ctx->title == NULL) {
        return 0;
    }
    sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_004374D0, ctx->title));
    return 2;
}

extern char D_004374D8[]; /* " %s" */

/* Draw an in-range item name, distinguishing selected active and inactive rows. */
void evtDrawSelectableTextRow(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
    if (index < ctx->itemCount) {
        s32 color;

        if (ctx->cursor == index) {
            if (ctx->actionMode == 2) {
                color = 4;
            } else {
                color = 5;
            }
        } else {
            color = 0;
        }
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_004374D8, ctx->itemNames[index]));
    }
}

extern s32 strlen(const char *s);

/* Draw a title-sized selection dialog; only action mode 2 advances its cursor. */
s32 evtUpdateTextSelectionDialog(s32 x, s32 y, EvtRuntime *ctx) {
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
    func_00250338(list, x, y, width, ctx->itemCount + 3, 0, ctx->itemCount, ctx, evtDrawStringEntry, evtDrawSelectableTextRow);
    kwlnPositionedTextSurface.submit(&kwlnPositionedTextSurface, list);
    if (ctx->actionMode != 2) {
        return 0;
    }
    return kwlnStepTwoListCursors(0, 1, ctx->itemCount, 1, ctx->itemCount, 0, 0, 0, &ctx->cursor);
}

extern char D_004374E0[];
extern char D_004374D0[];

s32 evtDrawInputValueRow(s32 list, s32 x, s32 y, u8 *ctx) {
    char text[16];
    func_0035C860(text, D_004374E0, (s32)ctx + 0x22D4, (s32)ctx + 0x22E0);
    sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_004374D0, text));
    return 2;
}

extern s8 D_003C9688[];
extern char D_004374E8[]; /* "%c" */

/* Draw eleven characters from a twelve-byte keyboard row and highlight the
 * runtime character selection, using mode 3 for the active color. */
void evtDrawKeyboardRow(s32 list, s32 x, s32 y, s32 row, EvtRuntime *ctx) {
    s32 i;
    s32 color;

    for (i = 0; i < 11; i++) {
        color = 0;
        if (ctx->charRow == row && ctx->charCol == i) {
            if (ctx->actionMode == 3) {
                color = 4;
            } else {
                color = 5;
            }
        }
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x + (i + 1) * 0xC0, y, 0xFEFFFF, color, D_004374E8, D_003C9688[row * 12 + i]));
    }
}

INCLUDE_ASM(const s32, "game/code_00250010", func_002515C8);

extern u16 D_004372B0;
extern u16 D_004372B2;

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423668);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423678);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423688);

s32 evtDrawEventFileNameRow(s32 list, s32 x, s32 y) {
    char text[32];
    func_0035C860(text, "[E%3d_%03d.PM1+2+3]", D_004372B0, D_004372B2);
    sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_004374D0, text));
    return 2;
}

void evtDrawEventCutSelectRow(s32 list, s32 x, s32 y, u32 index, EvtRuntime *ctx) {
    s32 color = 0;

    if (ctx->charRow == index) {
        color = 4;
    }
    switch (index) {
    case 0:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0xE, D_004374F0));
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x + 0x600, y, 0xFEFFFF, color, D_004374F8, D_004372B0));
        return;
    case 1:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0xE, D_00437500));
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x + 0x600, y, 0xFEFFFF, color, D_00437508, D_004372B2));
        return;
    case 3:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, " L,R = NO-+"));
        return;
    case 4:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, " U,D = SELECT"));
        return;
    case 5:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_004233F0));
        return;
    case 6:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_00423400));
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
    func_00250338(list, x, y, 0x12, 0xB, 0, 8, ctx, evtDrawEventFileNameRow, evtDrawEventCutSelectRow);
    kwlnPositionedTextSurface.submit(&kwlnPositionedTextSurface, list);
    if (ctx->actionMode != 9) {
        return 0;
    }
    if (D_0037F510.decTen < 0) {
        ctx->charRow ^= 1;
    }
    if (D_0037F510.incTen < 0) {
        ctx->charRow ^= 1;
    }
    if (D_0037F510.decOne & 2) {
        switch (ctx->charRow) {
        case 0:
            if (D_004372B0 >= 0x1F5) {
                D_004372B0--;
            }
            break;
        case 1:
            if (D_004372B2 >= 2) {
                D_004372B2--;
            }
            break;
        }
    }
    if (D_0037F510.incOne & 2) {
        switch (ctx->charRow) {
        case 0:
            if (D_004372B0 < 0x3E7) {
                D_004372B0++;
            }
            break;
        case 1:
            if (D_004372B2 < 0x3E7) {
                D_004372B2++;
            }
            break;
        }
    }
    if (D_0037F510.decHun & 2) {
        switch (ctx->charRow) {
        case 0:
            if (D_004372B0 >= 0x1F5) {
                num = (u16)(D_004372B0 - 500);
                if (num < 0x1F4) {
                    D_004372B0 = 0x1F4;
                } else {
                    D_004372B0 -= 500;
                }
            }
            break;
        case 1:
            if (D_004372B2 >= 2) {
                num = (u16)(D_004372B2 - 10);
                if (num == 0) {
                    D_004372B2 = 1;
                } else {
                    D_004372B2 -= 10;
                }
            }
            break;
        }
    }
    if (D_0037F510.incHun & 2) {
        switch (ctx->charRow) {
        case 0:
            D_004372B0 += 100;
            break;
        case 1:
            D_004372B2 += 10;
            break;
        }
    }
    if (D_0037F510.confirm < 0) {
        return 1;
    }
    if (D_0037F510.cancel < 0) {
        return -1;
    }
    if (D_0037F510.syncKey < 0) {
        D_004372B0 = 0x258;
    }
    return 0;
}

extern char D_00437510[]; /* "NAME:" */

void evtDrawSelectedEntryLabel(s32 list, s32 *sel, s32 x, s32 unused, u8 *base) {
    x += 0x6C0;
    kwlnDrawSpriteCell(list, (x - 0x7000) >> 4, 0x3C, 0x1C, 3);
    sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, 0x7AE0, 0xFEFFFF, 0xE, D_00437510));
    if (sel[2] >= 0) {
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x + 0x3C0, 0x7AE0, 0xFEFFFF, 0, D_004374D0, base + sel[2] * 32 + 0x24));
    }
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00251ED0);

extern void func_00251ED0(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx);

s32 evtUpdateEntrySelectionDialog(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;
    s32 count;
    s32 shown;

    list = sdfCreateResetPacketList();
    func_00250338(list, x, y, 8, 0x1D, ctx->entryFirst, ctx->entryCount, ctx, NULL, func_00251ED0);
    kwlnPositionedTextSurface.submit(&kwlnPositionedTextSurface, list);
    if (ctx->actionMode != 4) {
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
    return kwlnStepTwoListCursors(0, 1, count, 1, shown, 0, &ctx->entryFirst, 0, &ctx->entryCursor);
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

extern void func_00252378(s32 list, s32 x, s32 y, s32 color, EvtRuntimeChild *node, EvtRuntime *ctx);

/* Draw an indexed child of the selected group, or its trailing new-frame row. */
s32 evtDrawFrameListRow(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
    EvtRuntimeChild *node;
    s32 color;
    s32 i;

    if (index < ctx->frameGroup->childCount + 1) {
        node = NULL;
        if (index != ctx->frameGroup->childCount) {
            node = ctx->frameGroup->children;
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
            func_00252378(list, x, y, color, node, ctx);
        } else {
            sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color ? color : 8, "----- NEW FRAME -----"));
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00253590);


extern EvtWorldObject *dds3GetWorldObject();
extern char D_004374D8[]; /* " %s" */

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
                        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_004374D8, node->name));
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
extern EvtWorldNode *dds3FindObjectChainNodeByName(EvtWorldObject *world, char *name);

/* The pending-node's signed slot indices begin at +0xC (also used in DDS1). */
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
    sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0xE, D_004376A8, index));
    if (node != NULL) {
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_004376B0, node->name));
    } else {
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, "   -----------------------"));
    }
}

extern void evtViewerDrawPendingNodeRow(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx);

/* Draw the pending-node selector for group types 20/21; other types return -1. */
s32 mnuDrawInfoWindowB(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;
    s32 rows;

    list = sdfCreateResetPacketList();
    switch (ctx->frameGroup->type) {
    case 0x14:
        rows = 2;
        break;
    case 0x15:
        rows = 4;
        break;
    default:
        return -1;
    }
    func_00250338(list, x, y, 0x1C, rows, 0, rows, ctx, NULL, evtViewerDrawPendingNodeRow);
    kwlnPositionedTextSurface.submit(&kwlnPositionedTextSurface, list);
    if (ctx->actionMode != 0xC) {
        return 0;
    }
    return kwlnStepTwoListCursors(0, 1, rows, 1, rows, 0, 0, 0, &ctx->inputB);
}

extern char D_00423EC0[]; /* "MESSAGE MENU (MESMAX %3d)" */

/* The message-menu work references a window whose entry handle lives at +0x104. */
typedef struct EvtMessageWindow {
    u8 pad00[0x104];
    s32 entryHandle;
} EvtMessageWindow;


/* Draw the message count from the runtime window context
 * and its entry handle, returning two rows used. */
s32 mnuDrawMessageMenuLabel(s32 list, s32 x, s32 y, EvtRuntime *ctx) {
    sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_00423EC0, itfMesGetEntryCount(ctx->windowContext->entryHandle)));
    return 2;
}


extern char D_004376B8[];
extern char D_004376C0[];
extern char D_004376C8[];
extern char D_004374A0[];
extern char D_00423EE0[];
extern char D_00423EF0[];
extern char D_00423F00[];
extern char D_00423F10[];
extern char D_00423F20[];
extern char D_00423F30[];
extern char D_00423F40[];
extern char D_00423F50[];
extern char D_00423F60[];
extern char D_00423F70[];
extern char D_00423428[];
extern char D_004233F0[];
extern char D_00423400[];
extern s32 itfMesGetWindowEntryItems(s32, s32);

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

void evtDrawMessageDataRow(s32 list, s32 x, s32 y, s32 kind, EvtRuntime *ctx) {
    char *names[11] = {D_004376B8, D_00423EE0, D_00423EF0, D_00423F00,
        D_00423F10, D_00423F20, D_00423F30, D_00423F40, D_00423F50, D_00423F60, D_00423F70};

    switch (kind) {
    case 0: {
        s32 color = 4;
        if (ctx->messageField != 0) {
            color = 0;
        }
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_004374A0, ctx->value & 0xFFF));
        if (ctx->windowContext->entryHandle == -1) {
            sdfAppendPacket(list, sdfCreateFormattedSifCommand(x + 0x600, y, 0xFEFFFF, color, "NONE MESDATA!!"));
        } else if (itfMesGetWindowEntryItems(ctx->windowContext->entryHandle, ctx->value & 0xFFF) == 0) {
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
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_004376C0, names[(ctx->value >> 12) & 0xF]));
        break;
    }
    case 3:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_00423428));
        break;
    case 4:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, " U,D = SELECT "));
        break;
    case 5:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_004233F0));
        break;
    case 6:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_00423400));
        break;
    }
}

s32 evtUpdateMessageValueDialog(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;
    s32 packed;
    s32 number;
    s32 branch;
    s32 field;
    s32 delta;
    s32 handle;

    list = sdfCreateResetPacketList();
    func_00250338(list, x, y, 0x18, 0xA, 0, 1, ctx, mnuDrawMessageMenuLabel, evtDrawMessageDataRow);
    kwlnPositionedTextSurface.submit(&kwlnPositionedTextSurface, list);
    if (ctx->actionMode != 0xD) {
        return 0;
    }
    packed = ctx->value;
    number = packed & 0xFFF;
    branch = (packed >> 12) & 0xF;
    field = ctx->messageField;
    if (field != 0) {
        if (field == 1) {
            if (D_0037F510.decOne & 2) {
                branch = branch == 0 ? 0xA : branch - 1;
            } else if (D_0037F510.incOne & 2) {
                branch = branch >= 0xA ? 0 : branch + 1;
            }
            ctx->value &= 0xFFF;
            ctx->value |= branch << 12;
        }
    } else {
        delta = 0;
        if (D_0037F510.decOne & 2) {
            delta = -1;
        } else if (D_0037F510.incOne & 2) {
            delta = 1;
        }
        number += delta;
        if (number < ctx->valueMin) {
            number = ctx->valueMax;
        }
        if (number > ctx->valueMax) {
            number = 0;
        }
        ctx->value = number | (branch << 12);
    }
    if ((D_0037F510.decTen & 2) || (D_0037F510.incTen & 2)) {
        ctx->messageField = !field;
    }
    if (D_0037F510.confirm < 0) {
        handle = ctx->windowContext->entryHandle;
        if (handle != -1) {
            if (branch == 0) {
                if (itfMesGetWindowEntryItems(handle, number) == 0) {
                    return 1;
                }
            } else if (itfMesGetWindowEntryItems(handle, number) == 1) {
                return 1;
            }
        }
    }
    return D_0037F510.cancel >= 0 ? 0 : -1;
}



s32 mnuDrawCutFlagLabel(s32 target, s32 x, s32 y) {
    sdfAppendPacket(target, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, "CUTFLAG MENU"));
    return 2;
}


/* Draw the packed four-bit option or twelve-bit number and highlight
 * the selected comparison field; later rows provide input instructions. */
void evtDrawComparisonValueRow(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
    char *labels[11] = {D_004376C8, D_00423EE0, D_00423EF0, D_00423F00,
        D_00423F10, D_00423F20, D_00423F30, D_00423F40, D_00423F50, D_00423F60, D_00423F70};
    s32 flag;

    switch (index) {
    case 0:
        flag = ctx->compareField != 0 ? 0 : 4;
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, flag, D_004376C0, labels[(ctx->value >> 12) & 0xF]));
        return;
    case 1:
        flag = ctx->compareField != 1 ? 0 : 4;
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, flag, D_004374A0, ctx->value & 0xFFF));
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x + 0x600, y, 0xFEFFFF, flag, "  (CMP VALUE)"));
        return;
    case 3:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_00423428));
        return;
    case 4:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, " U,D = SELECT "));
        return;
    case 5:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_004233F0));
        return;
    case 6:
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_00423400));
        break;
    }
}

s32 evtUpdateComparisonValueDialog(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;
    s32 packed;
    s32 number;
    s32 branch;
    s32 field;
    s32 delta;

    list = sdfCreateResetPacketList();
    func_00250338(list, x, y, 0x18, 0xA, 0, 1, ctx, mnuDrawCutFlagLabel, evtDrawComparisonValueRow);
    kwlnPositionedTextSurface.submit(&kwlnPositionedTextSurface, list);
    if (ctx->actionMode != 0xE) {
        return 0;
    }
    packed = ctx->value;
    number = packed & 0xFFF;
    branch = (packed >> 12) & 0xF;
    field = ctx->compareField;
    switch (field) {
    case 0:
        if (D_0037F510.decOne & 2) {
            branch = branch == 0 ? 0xA : branch - 1;
        } else if (D_0037F510.incOne & 2) {
            branch = branch >= 0xA ? 0 : branch + 1;
        }
        ctx->value &= 0xFFF;
        ctx->value |= branch << 12;
        break;
    case 1:
        delta = 0;
        if (D_0037F510.decOne & 2) {
            delta = -1;
        } else if (D_0037F510.incOne & 2) {
            delta = 1;
        }
        number += delta;
        if (number < ctx->valueMin) {
            number = ctx->valueMax;
        }
        if (number > ctx->valueMax) {
            number = 0;
        }
        ctx->value = number | (branch << 12);
        break;
    }
    if ((D_0037F510.decTen & 2) || (D_0037F510.incTen & 2)) {
        ctx->compareField = !field;
    }
    if (D_0037F510.confirm < 0) {
        return 1;
    }
    return D_0037F510.cancel >= 0 ? 0 : -1;
}

s32 evtIsMenuTableEntryEnabled(s32 *index) {
    return D_003C9730[*index].enabled != 0;
}

/* Return the table value selected by this group's type and editor column. */
s32 mnuGetSelectedTableValue(EvtRuntime *runtime) {
    return D_003C9732[runtime->tableColumn + runtime->frameGroup->type * 10];
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00254678);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00424080);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00424090);

INCLUDE_ASM(const s32, "game/code_00250010", func_00254940);

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

INCLUDE_ASM(const s32, "game/code_00250010", func_00254CE0);

extern void func_00254CE0();

/* Motion editor row: ctx->value packs group (byte 0), motion number (byte 1), loop flag (byte 2) and interpolation (byte 3). */
s32 evtUpdateMotionChangeRow(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;
    s32 model;
    s32 count;
    EvtMotionValue packed;

    list = sdfCreateResetPacketList();
    model = ((EvtModelSlot *)ctx->frameGroup->info->data)->ref->handle;
    func_00250338(list, x, y, 0x14, 0xA, 0, 1, ctx, mnuDrawMotionChangeLabel, func_00254CE0);
    kwlnPositionedTextSurface.submit(&kwlnPositionedTextSurface, list);
    if (ctx->actionMode != 0x10) {
        return 0;
    }
    if (D_0037F510.decTen & 2) {
        if (ctx->fieldIndex == 0) {
            ctx->fieldIndex = 3;
        } else {
            ctx->fieldIndex = ctx->fieldIndex - 1;
        }
    } else if (D_0037F510.incTen & 2) {
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
        if (D_0037F510.incOne & 2) {
            do {
                if (packed.bits.group < 3) {
                    packed.bits.group = packed.bits.group + 1;
                } else {
                    packed.bits.group = 0;
                }
            } while (mdlFindNodeById(model, packed.bits.group) == NULL);
        } else if (D_0037F510.decOne & 2) {
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
        if (D_0037F510.incOne & 2) {
            if (packed.bits.motion >= count - 1) {
                packed.bits.motion = 0;
            } else {
                packed.bits.motion = packed.bits.motion + 1;
            }
        } else if (D_0037F510.decOne & 2) {
            if (packed.bits.motion > 0) {
                packed.bits.motion = packed.bits.motion - 1;
            } else {
                packed.bits.motion = count - 1;
            }
        }
        break;
    case 2:
        if ((D_0037F510.incOne & 2) || (D_0037F510.decOne & 2)) {
            packed.bits.loop = packed.bits.loop == 0;
        }
        break;
    case 3:
        if (D_0037F510.incOne & 2) {
            if (packed.bits.hokan < 0x64) {
                packed.bits.hokan = packed.bits.hokan + 1;
            } else {
                packed.bits.hokan = 0;
            }
        }
        if (D_0037F510.decOne & 2) {
            if (packed.bits.hokan > 0) {
                packed.bits.hokan = packed.bits.hokan - 1;
            } else {
                packed.bits.hokan = 0x64;
            }
        }
        break;
    }
    ctx->value = packed.word;
    if (D_0037F510.confirm < 0 && mdlFindNodeById(model, packed.bits.group)->data->frames[packed.bits.motion] != NULL) {
        return 1;
    }
    return D_0037F510.cancel >= 0 ? 0 : -1;
}

extern char D_00424090[]; /* "UNIT ALL" */
extern char D_004376E8[]; /* "ALL" */
extern char D_004376F0[]; /* "DISABLE" */

void evtDrawGroupListRow(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
    EvtRuntimeGroup *group;
    s32 color;
    s32 n;

    color = 4;
    if (ctx->groupFirst + ctx->groupCursor != index) {
        color = 0;
    }
    if (index == 0) {
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_004376F0));
    } else if (index == 1) {
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_004376E8));
        return;
    } else if (index == 2) {
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_00424090));
        return;
    }
    n = 3;
    for (group = ctx->groups; group != NULL; group = group->next) {
        if (group->type == 1) {
            if (n == index) {
                sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_004374D0, group->info->name));
                return;
            }
            n++;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00255538);

void func_00255648(void) {
}

extern f32 sdfViewMatrix[];
extern void effMiscAxisAngleToQuaternionVU(f32 angle);
extern void effMiscQuaternionToMatrixVU(void);

/* vu0 routine: rotate an event-viewer position about camera axes selected by the pad. */
void func_00255650(f32 *position)
{
    if (D_0037F510.incOne != 0) {
        EE_MMI_LOAD_MATRIX_COLUMN(vf10, sdfViewMatrix + 1);
        effMiscAxisAngleToQuaternionVU(2.0f * 3.14159265f / 180.0f);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, position);
        VU0_TRANSFORM_POINT(vf10, vf10);
        VU0_SET_W_ONE(vf10);
        VU0_STORE_VF(vf10, position);
    } else if (D_0037F510.decOne != 0) {
        EE_MMI_LOAD_MATRIX_COLUMN(vf10, sdfViewMatrix + 1);
        effMiscAxisAngleToQuaternionVU(-2.0f * 3.14159265f / 180.0f);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, position);
        VU0_TRANSFORM_POINT(vf10, vf10);
        VU0_SET_W_ONE(vf10);
        VU0_STORE_VF(vf10, position);
    }
    if (D_0037F510.incTen != 0) {
        EE_MMI_LOAD_MATRIX_COLUMN(vf10, sdfViewMatrix);
        effMiscAxisAngleToQuaternionVU(-2.0f * 3.14159265f / 180.0f);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, position);
        VU0_TRANSFORM_POINT(vf10, vf10);
        VU0_SET_W_ONE(vf10);
        VU0_STORE_VF(vf10, position);
    } else if (D_0037F510.decTen != 0) {
        EE_MMI_LOAD_MATRIX_COLUMN(vf10, sdfViewMatrix);
        effMiscAxisAngleToQuaternionVU(2.0f * 3.14159265f / 180.0f);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, position);
        VU0_TRANSFORM_POINT(vf10, vf10);
        VU0_SET_W_ONE(vf10);
        VU0_STORE_VF(vf10, position);
    }
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00255818);

void func_002560A8(void) {
}

INCLUDE_ASM(const s32, "game/code_00250010", func_002560B0);

/* Draw command text for row 0 or 1; null texts and other rows emit nothing. */
void evtDrawOptionalPromptText(s32 list, s32 x, s32 y, s32 kind, EvtRuntime *ctx) {
    switch (kind) {
    case 0:
        if (ctx->commandSecond.text != NULL) {
            sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_004374D0, ctx->commandSecond.text));
            return;
        }
        break;
    case 1:
        if (ctx->commandThird.text != NULL) {
            sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_004374D0, ctx->commandThird.text));
        }
        break;
    }
}

extern void evtDrawOptionalPromptText(s32 list, s32 x, s32 y, s32 kind, EvtRuntime *ctx);

/* In mode 0x14, count down before checking input: zero expires, negative waits
 * indefinitely, and confirm takes precedence over cancel. */
s32 mnuDrawTimedPrompt(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;

    list = sdfCreateResetPacketList();
    func_00250338(list, x, y, 0x19, 2, 0, 1, ctx, NULL, evtDrawOptionalPromptText);
    kwlnPositionedTextSurface.submit(&kwlnPositionedTextSurface, list);
    if (ctx->actionMode != 0x14) {
        return 0;
    }
    if (ctx->commandFirst > 0) {
        ctx->commandFirst--;
    } else if (ctx->commandFirst == 0) {
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

/* Store three command arguments. Timed prompts consume the first as a countdown
 * and the other two as text; the DDS1 API passes those addresses as words. */
void evtSetRuntimeCommandValues(EvtRuntime *runtime, s32 frames, char *firstText, char *secondText) {
    runtime->commandFirst = frames;
    runtime->commandSecond.text = firstText;
    runtime->commandThird.text = secondText;
}

extern char D_00437768[]; /* "CURRENT" */

INCLUDE_RODATA(const s32, "game/code_00250010", D_004241A0);

void evtViewerDrawGroupRow(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
    EvtRuntimeGroup *group;
    s32 color;
    s32 n;

    color = 4;
    if (ctx->groupFirst + ctx->groupCursor != index) {
        color = 0;
    }
    if (index == 0) {
        sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_00437768));
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
                sdfAppendPacket(list, sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_004374D0, ctx->entryName[group->value08]));
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

extern s32 effEventAdvanceResourceTemplateSetup();

extern u32 kwlnTaskGetTimer(s32 task);

typedef struct EvtSelectionCache {
    u8 pad00[0x24];
    s32 selectedEntry; /* 0x24 */
} EvtSelectionCache;

extern EvtSelectionCache *D_00436518;

extern s32 evtActiveEntryFlags;

/* Cache nonzero entry selections, publishing first-tick flags only at timer zero.
 * A zero setup status clears runtime control and returns -1. */
s32 evtSynchronizeSelectedEntry(s32 task) {
    EvtRuntime *runtime = (EvtRuntime *)kwlnTaskGetUserValue(task);

    if (effEventAdvanceResourceTemplateSetup() == 0) {
        runtime->controlState = 0;
        return -1;
    }
    if (kwlnTaskGetTimer(task) == 0) {
        s32 selected = runtime->selectedEntry;

        evtActiveEntryFlags = selected;
        if (selected != 0) {
            D_00436518->selectedEntry = selected;
        }
    }
    if (D_00436518->selectedEntry != runtime->selectedEntry) {
        s32 selected = runtime->selectedEntry;

        if (selected != 0) {
            D_00436518->selectedEntry = selected;
        }
    }
    return 0;
}

/* Poll solid-rectangle setup; zero status clears runtime control and returns -1. */
s32 evtPollRuntimeControlReady(void) {
    void *runtime;

    runtime = kwlnTaskGetUserValue();
    if (effEventAdvanceSolidRectangleSetup() == 0) {
        ((EvtRuntime *)runtime)->controlState = 0;
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

s32 func_00258700(s32 arg0, s32 arg1, EvtRuntime *runtime) {
    s32 status = 1;

    switch (runtime->frameGroup->type) {
    case 0xD:
        status = effUpdateCh72Params();
        break;
    case 0xE:
        status = effEventAdvanceBlurTemplateSetup();
        if (runtime->selectedEntry != 0) {
            *(s32 *)((u8 *)D_004364B0 + 0x2C) = runtime->selectedEntry;
        }
        break;
    case 0xF:
        status = effEventAdvanceScatterBlurSetup();
        if (runtime->selectedEntry != 0) {
            *(s32 *)((u8 *)D_0043653C + 0x2C) = runtime->selectedEntry;
        }
        break;
    case 0x17:
        status = effEventAdvanceScaleBlurSetup();
        if (runtime->selectedEntry != 0) {
            *(s32 *)((u8 *)D_00436520 + 0x2C) = runtime->selectedEntry;
        }
        break;
    case 0x1B:
        status = func_001978B8();
        break;
    case 0x10:
    case 0x11:
        if (runtime->controlState == 0) {
            return 1;
        }
        break;
    case 0x18:
        status = func_002570F8(runtime);
        break;
    case 0x19:
        status = func_00257910(runtime);
        break;
    case 0x6:
        status = func_002582D0(runtime);
        break;
    case 0x7:
    case 0x8:
    case 0x9:
    case 0xA:
    case 0xB:
    case 0xC:
    default:
        if (D_0037F510.apply < 0) {
            status = 0;
        }
        break;
    }

    if (status != 0) {
        return 0;
    }
    if (runtime->controlState != 0) {
        kwlnTaskDestroyWithHierarchy(runtime->controlState, 1);
        runtime->controlState = 0;
    }
    return 1;
}

s32 evtDispatchActionByIndex(s32 index, s32 x, s32 y, void *runtime) {
    s32 mode = ((EvtRuntime *)runtime)->actionMode;
    if (mode == 11 && index != mode) {
        return 0;
    }
    return D_003C9928[index](x, y, runtime);
}

void func_002588A0(s32 output, s32 data, s32 size) {
    func_0036A420();
}

s32 evtAssignRuntimeChildSequenceAndCount(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xA) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->body.f.groupTypeAIndex = index++;
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
                child->groupTypeBIndex = index++;
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
                child->body.f.groupTypeIndex = index++;
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
                child->body.f.groupTypeIndex = index++;
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
                child->body.f.groupTypeIndex = index++;
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
                child->body.f.groupTypeIndex = index++;
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
                child->body.f.groupTypeIndex = index++;
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
                child->body.f.groupTypeIndex = index++;
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
                child->body.f.groupTypeIndex = index++;
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
                child->body.f.groupTypeIndex = index++;
            }
        }
    }
    return index;
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00258CC8);


/* Emit first, range, third and metadata words in file order. The range word
 * is written whole, not narrowed to the terminal halfword used by child spans. */
void evtWriteRuntimeHeaderValues(s32 output, EvtRuntime *state) {
    s32 buffer[4];
    buffer[0] = state->headerFirst;
    buffer[1] = state->frameRange.word;
    buffer[2] = state->headerThird;
    buffer[3] = state->headerMetadata;
    func_002588A0(output, buffer, 0x10);
}

typedef struct EvtSerializedChild {
    u16 groupType;
    u16 start;
    u16 span;
    u16 value;
    u8 pad08[4];
    s32 body[8];
} EvtSerializedChild;

/* Filter groups by mode and serialize each child body. Type-8 spans use the
 * next start or terminal range halfword, unless their body marker disables them. */
void func_00259298(s32 output, s32 mode, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;

    for (group = runtime->groups; group != NULL; group = group->next) {
        EvtRuntimeChild *child = group->children;

        if (mode == 2) {
            if (group->type == 5 || group->type == 0x13) {
                continue;
            }
        } else if (mode == 3) {
            if (group->type != 5 && group->type != 0x13) {
                continue;
            }
        }

        for (; child != NULL; child = child->next) {
            EvtSerializedChild record;
            s32 i;
            u16 value;

            value = child->unk04;
            record.groupType = group->type;
            record.start = child->unk00;
            record.span = child->groupTypeBIndex;
            record.value = value;
            for (i = 0; i < 8; i++) {
                record.body[i] = child->body.words[i];
            }
            if (group->type == 8) {
                if (child->body.words[0] == 0) {
                    record.span = 0;
                } else {
                    if (child->next != NULL) {
                        record.span = child->next->unk00 - child->unk00;
                    } else {
                        record.span = runtime->frameRange.f.end - child->unk00;
                    }
                }
            }
            func_002588A0(output, (s32)&record, sizeof(record));
        }
    }
}

/* Emit entryTotal consecutive thirty-two-byte names; nonpositive totals emit nothing. */
void evtWriteFixedSizeEntries(s32 output, EvtRuntime *table) {
    s32 i;
    u8 *entry;
    i = 0;
    if (table->entryTotal > 0) {
        entry = (u8 *)table->entryName;
        do {
            func_002588A0(output, entry, 0x20);
            entry += 0x20;
            i++;
        } while (i < table->entryTotal);
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

void evtCopyRuntimeChildPayloadsToBuffer(s32 output, EvtRuntime *runtime) {
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

void evtEmitGroupTypeElevenPayloads(s32 output, EvtRuntime *runtime) {
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

void evtEmitGroupTypeThirteenPayloads(s32 output, EvtRuntime *runtime) {
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

void evtEmitGroupTypeFourteenPayloads(s32 output, EvtRuntime *runtime) {
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

void evtEmitGroupTypeFifteenPayloads(s32 output, EvtRuntime *runtime) {
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

void evtEmitGroupTypeTwentyThreePayloads(s32 output, EvtRuntime *runtime) {
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

void evtEmitGroupTypeTwentySevenPayloads(s32 output, EvtRuntime *runtime) {
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

void evtEmitGroupTypeSixteenPayloads(s32 output, EvtRuntime *runtime) {
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

void evtEmitGroupTypeSeventeenPayloads(s32 output, EvtRuntime *runtime) {
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

void evtEmitGroupTypeTwentyFivePayloads(s32 output, EvtRuntime *runtime) {
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

u16 evtGetRowValue(s32 group, s32 index) {
    if (((EvtRowDescriptor *)((EvtRowTable *)group)->descriptor)->format == 4) {
        return ((EvtCompactRow *)(((EvtRowTable *)group)->compactRows + index * 0x10))->value;
    }
    return ((EvtExtendedRow *)(((EvtRowTable *)group)->extendedRows + index * 0x2c))->value;
}

s16 evtGetRowVariant(s32 group, s32 index) {
    if (((EvtRowDescriptor *)((EvtRowTable *)group)->descriptor)->format == 4) {
        return ((EvtCompactRow *)(((EvtRowTable *)group)->compactRows + index * 0x10))->variant;
    }
    return ((EvtExtendedRow *)(((EvtRowTable *)group)->extendedRows + index * 0x2c))->variant;
}

u16 evtGetRowParameter(s32 group, s32 index) {
    if (((EvtRowDescriptor *)((EvtRowTable *)group)->descriptor)->format == 4) {
        return ((EvtCompactRow *)(((EvtRowTable *)group)->compactRows + index * 0x10))->parameter;
    }
    return ((EvtExtendedRow *)(((EvtRowTable *)group)->extendedRows + index * 0x2c))->parameter;
}

u16 evtGetRowFlags(s32 group, s32 index) {
    if (((EvtRowDescriptor *)((EvtRowTable *)group)->descriptor)->format == 4) {
        return ((EvtCompactRow *)(((EvtRowTable *)group)->compactRows + index * 0x10))->flags;
    }
    return ((EvtExtendedRow *)(((EvtRowTable *)group)->extendedRows + index * 0x2c))->flags;
}

s32 evtGetRowPayloadAddress(s32 group, s32 index) {
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

void evtResolveLinkGroupIndex(EvtLinkSource *src, EvtRuntime *runtime, EvtLink *link) {
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
extern s32 func_0024FB48(u16 a, u16 b, s32 mode);
extern void func_0025A280(s32 handle, EvtRuntime *runtime);

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
    evtFormatPolygonMoviePaths(D_004372B0, D_004372B2, path0, path1, path2);
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
        runtime->windowContext = (EvtMessageWindow *)handle;
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

s32 evtQueueValidatedBgmSoundCode(s32 id, s32 fade) {
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

s32 evtStartBgmBySoundIdAndFade(s32 id, s32 fade) {
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
    return kwlnTaskGetTaskByName(taskName);
}

s32 evtGetTaskValueWord(u32 taskId) {
    s32 task = evtFindTaskById(taskId);
    if (task == 0) {
        return -1;
    }
    return ((EvtTaskData *)kwlnTaskGetUserValue(task))->value;
}

void *evtGetTaskData(u32 taskId) {
    s32 task = evtFindTaskById(taskId);
    if (task != 0) {
        return kwlnTaskGetUserValue(task);
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
    s32 pad00;
    s32 type;             /* 0x04 */
    u8 pad08[8];
    s32 base;             /* 0x10 */
    EvtResHeader *header; /* 0x14 */
    EvtResEntry *entries; /* 0x18 */
} EvtResTask;

s32 evtFindTaskResourceEntryByKey(u32 id, s32 key) {
    s32 task;
    EvtResTask *data;
    s32 i;

    task = evtFindTaskById(id);
    if (task == 0) {
        return 0;
    }
    data = kwlnTaskGetUserValue(task);
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
    s32 resource = evtFindTaskResourceEntryByKey(taskId, key);
    s32 handle;
    if (resource != 0) {
        if (data->effectHandle != 0) {
            sdfTexReleaseReferenceViaHandler(data->effectHandle);
            data->effectHandle = 0;
        }
        handle = sdfTexAcquireResourceTexture(resource);
        effSetCh72Id(handle);
        data->effectHandle = handle;
    }
}

/* Party/enemy model table entry (0x270 bytes); only the scale-source field is known here. */
typedef struct EvtModelScaleEntry {
    u8 pad00[0x18];
    f32 modelScale;
    u8 pad1C[0x254];
} EvtModelScaleEntry;

typedef struct EvtEffectInner {
    u8 pad00[0x2C];
    void *scaledObject; /* 0x2C: object passed to the scale setter */
} EvtEffectInner;

typedef struct EvtEffectObject {
    u8 pad00[0x18];
    EvtEffectInner *inner;
} EvtEffectObject;

extern EvtModelScaleEntry *D_00435DE0;
extern EvtModelScaleEntry *D_00435DF0;
extern void *memset(void *, s32, u32);
extern void effObjSetFlags(void *object, s32 flags);
extern void *func_00115500(void *obj, void *vecA, void *vecB);
extern void effEventSetScale(void *target, f32 scale);

/* Spawn from a keyed task resource with two zeroed constructor vectors.
 * A nonnegative index applies the selected model-table scale relative to the base. */
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
        obj = func_00115500(found, vecA, vecB);
        if (obj != 0) {
            effObjSetFlags(obj, 1);
            if (index >= 0) {
                inner = ((EvtEffectObject *)obj)->inner;
                effEventSetScale(inner->scaledObject, D_00435DF0[index].modelScale / D_00435DE0->modelScale);
            }
            return obj;
        }
        return obj;
    }
    return found;
}

INCLUDE_SDATA(const s32, "game/code_00250010", evtPictureTaskName);

INCLUDE_SDATA(const s32, "game/code_00250010", evtSkyTransitionActive);

INCLUDE_SDATA(const s32, "game/code_00250010", evtSkyOverlayEnabled);

INCLUDE_SDATA(const s32, "game/code_00250010", evtSkyTaskName);

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

