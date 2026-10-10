#include "common.h"
#include "dat_state.h"
#include "sdf_texture_offset_list.h"
#include "itf_mes_window.h"
#include "sdf_packet_list.h"
#include "sdf_resource.h"
#include "evt_viewer.h"
#include "evt_viewer_rows.h"

#include "evt_world.h"
#include "evt_picture.h"
#include "eff_object.h"
#include "eff_event.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "mdl.h"
#include "sdf.h"
#include "sdf_sif_command.h"
#include "kwln.h"
#include "evt_task.h"
#include "evt_unit.h"
#include "evt_event_pack.h"
#include "evt_polygon_movie.h"
#include "fld.h"
#include "kwln_task_lifecycle.h"
#include "sdf_texture_file.h"

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

extern void fldSubmitTexturedOverlaySprite(void);



typedef struct {
    s16 columnCount;
    s8 columns[8]; /* The halfword counts signed property selectors. */
} EvtTblEntry; /* 0xA bytes */

extern EvtTblEntry D_003C9730[];


extern s32 effEventAdvanceSolidRectangleSetup(void);



/* Native viewer child node; frame rows traverse this same linked record. */


/* World-slot node borrowed by a viewer group. Its data is slot-specific;
 * model groups use EvtModelSlot, while all named nodes share the list links. */

/* The native 0x84-byte viewer entry owns its child list and borrows info.
 * EvtRuntime.frameGroup selects one of these entries, not a separate list. */


/* The command API stores words; timed-prompt drawing consumes their text pointers. */


/* The file header emits this whole word; type-8 spans use its low halfword. */

/* Native 0x24BC-byte viewer runtime shared by dialogs, task polls and file writers. */


extern void fldApplyCameraColorKeyWords(EvtRuntime *runtime, const EvtBlendKey *source);
extern s32 effUpdateCh72Params(void);
extern s32 effEventAdvanceBlurTemplateSetup(void);
extern s32 effEventAdvanceScatterBlurSetup(void);
extern s32 effEventAdvanceScaleBlurSetup(void);
extern s32 effEventAdvanceScreenDrawSetup(void);
extern s32 func_002570F8(EvtRuntime *runtime);
extern s32 evtEditCameraColorKeyFrame(EvtRuntime *runtime);
extern s32 func_002582D0(EvtRuntime *runtime);
extern void *D_004364B0;
extern void *D_0043653C;
extern void *D_00436520;


typedef struct EvtPad {
    u8 pad00[6];
    s8 frameEnd; /* +6: jump from group properties to the last key. */
    s8 frameStart; /* +7: jump from group properties to the first key. */
    u8 pad08[0x18];
    s8 syncKey; /* 0x20 */
    s8 confirm; /* 0x21 */
    u8 pad22;
    s8 cancel;  /* 0x23 */
    s8 decOne;  /* 0x24 */
    s8 incOne;  /* 0x25 */
    s8 decTen;  /* 0x26 */
    s8 incTen;  /* 0x27 */
    u8 decHun;  /* 0x28 */
    s8 unk29; /* 0x29: coarse decrease in the property editor */
    u8 incHun;  /* 0x2A */
    s8 unk2B; /* 0x2B: coarse increase in the property editor */
    s8 apply;   /* 0x2C */
    s8 unk2D;
    u8 pad2E[0x12]; /* Complete two-bank, two-port, 16-input backing. */
} EvtPad;

extern EvtPad D_0037F510;
extern SdfPoolNode kwlnPositionedTextSurface;
extern char D_004233F0[]; /* " RR  = ENTER" */
extern char D_00423400[]; /* " RD  = CANCEL" */
extern char D_00423428[]; /* " L,R = VALUE-+" */
extern char D_004374A0[]; /* "     %d" */
extern char D_004374F0[]; /* " EVENT" */
extern char D_004374F8[]; /* "%3d" */
extern char D_00437500[]; /* "   CUT" */
extern char D_00437508[]; /* "%03d" */
extern void kwlnDrawSpriteCell(void *list, s32 x, s32 y, s32 w, s32 h);
extern s32 kwlnStepTwoListCursors(s32, s32, s32, s32, s32, s32 *, s32 *, s32 *, s32 *);
extern s32 func_002521C8();
extern s32 evtIsMenuTableEntryEnabled(s32 *);

extern void sndEnsureMidiBankResident(s32 sound);

extern s32 sndFindPackedTrackLoadStatus(s32 sound);

extern void sndStartTrackDefault(s32 track);

extern void func_0035B6E0(const char *fmt, ...);

extern void sndStartTrackExtended(s32 track);

extern void func_00342580(u32 sound);

extern void sndSetSequenceVolumePan(s32 sequence, s32 volume, s32 pan);

extern void func_00341C78(u32 sound);

extern s32 (*D_003C9928[])(s32, s32, EvtRuntime *);

extern KwlnTask *kwlnTaskCreate(const char *, u32, s32, s32, TaskUpdate, TaskDestroy, u32);


/* Create a task with an initialized event payload. */
KwlnTask *evtCreateTask(s32 taskId, const char *path) {
    EvtPictureWork *taskData = evtAllocateContext();
    evtSetConvertedContextValue(taskData, path);
    return kwlnTaskCreate(evtPictureTaskName, taskId, 1, 1, evtUpdatePictureWhenFlagged, evtPictureReleaseTaskTextureAndState, (u32)taskData);
}


KwlnTask *evtCreateTaskWithValue(s32 taskId, SdfTex *texture) {
    EvtPictureWork *taskData = evtAllocateContext();
    taskData->texture = texture;
    return kwlnTaskCreate(evtPictureTaskName, taskId, 1, 1, evtUpdatePictureWhenFlagged, evtPictureReleaseTaskTextureAndState, (u32)taskData);
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

s32 evtUpdateSkyTask(KwlnTask *task) {
    evtAdvanceSkyTransition();
    func_00134A18();
    if (evtSkyOverlayEnabled != 0) {
        fldSelectDisplayBuffer(0x53);
        fldSubmitTexturedOverlaySprite();
    }
    return 0;
}

void evtResetSkyTaskFlags(KwlnTask *task) {
    evtSkyTransitionActive = 0;
    evtSkyOverlayEnabled = 0;
}

extern char evtSkyTaskName[];

void evtDestroySkyTask(void) {
    KwlnTask *task = kwlnTaskGetTaskByName(evtSkyTaskName);
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
    kwlnTaskCreate(evtSkyTaskName, 0x2B0E, 1, 1, evtUpdateSkyTask, evtResetSkyTaskFlags, 0);
}

/* Frame-variable task update: polls the parent task's user value each frame. */
s32 evtUpdateFrameVariableTask(KwlnTask *task) {
    u8 unused[16]; /* retail frame 0x20: unused local storage */

    kwlnTaskGetUserValue(task->parent);
    return 0;
}

extern char D_00423380[]; /* "FrameVar" */

void evtCreateFrameVariableTask(void) {
    kwlnTaskCreate(D_00423380, 0x2AF9, 1, 1, evtUpdateFrameVariableTask, 0, 0);
}

typedef s32 (*EvtMenuHeaderFn)();
typedef void (*EvtMenuRowFn)();
extern u32 D_00437488;
extern char D_00437490[];
extern char D_00437498[];

/* Draw a framed debug menu: the optional header returns how many rows it used, the row callback fills the rest,
 * and blinking scroll markers appear above/below when entries precede `first` or follow the last drawn one. */
void evtDrawMenuFrame(SdfListHead *list, s32 x, s32 y, s32 width, s32 rows, s32 first, s32 total, u8 *data,
                   EvtMenuHeaderFn header, EvtMenuRowFn row) {
    s32 i = 0;
    s32 textX;
    s32 textY;
    s32 index;

    kwlnDrawSpriteCell((void *)list, x, y, width, rows);
    textX = (x << 4) + 0x7000;
    textY = (y << 3) + 0x7900;
    if (header != NULL) {
        s32 headerRows = header(list, textX, textY, data);

        textY += headerRows * 96;
        i = headerRows;
    }
    index = first;
    for (; i < rows; i++) {
        if (row != NULL) {
            row(list, textX, textY, index, data);
            textY += 96;
            index++;
        }
    }
    if (D_00437488 & 0x10) {
        textY = ((y - 10) << 3) + 0x7900;
        textX = ((x + 5 * width + 6) << 4) + 0x7000;
        if (first > 0) {
            sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(textX, textY, 0xFEFFFF, 6, D_00437490));
        }
        textY = ((y + 12 * rows - 2) << 3) + 0x7900;
        if (index < total) {
            sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(textX, textY, 0xFEFFFF, 6, D_00437498));
        }
    }
    D_00437488++;
}

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423380);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423390);

s32 evtAppendValueChangeDebugLabel(SdfListHead *list, s32 x, s32 y) {
    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, "VALUE CHANGE."));
    return 2;
}

extern char D_004233B0[]; /* "     %.1f" */
extern char D_004233C0[]; /* " L,R = VALUE-+0.1" */
extern char D_004233D8[]; /* " U,D = VALUE-+1.0" */
extern char D_004233F0[]; /* " RR  = ENTER" */
extern char D_00423400[]; /* " RD  = CANCEL" */

INCLUDE_RODATA(const s32, "game/code_00250010", D_004233B0);

INCLUDE_RODATA(const s32, "game/code_00250010", D_004233C0);

INCLUDE_RODATA(const s32, "game/code_00250010", D_004233D8);

INCLUDE_RODATA(const s32, "game/code_00250010", D_004233F0);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423400);

void func_00250558(SdfListHead *list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
    switch (index) {
    case 0:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 4, D_004233B0,
                                                                 ctx->floatValue));
        return;
    case 2:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_004233C0));
        return;
    case 3:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_004233D8));
        return;
    case 4:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_004233F0));
        return;
    case 5:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_00423400));
        break;
    }
}

/* Edit the bounded float only in mode 8. Confirm precedes cancel; coarse steps
 * replace fine steps before the value is clamped to the runtime limits. */
s32 evtViewerFloatValueUpdate(s32 x, s32 y, EvtRuntime *ctx) {
    SdfListHead *list;
    f32 step;

    list = sdfCreateResetPacketList();
    evtDrawMenuFrame(list, x, y, 0x16, 9, 0, 1, ctx, evtAppendValueChangeDebugLabel, func_00250558);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, list);
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

s32 evtDrawValueChangeNoticeRow(SdfListHead *list, s32 x, s32 y) {
    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, "VALUE CHANGE."));
    return 2;
}

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423428);

void evtDrawValueChangeInstructionRow(SdfListHead *list, s32 x, s32 y, u32 index, EvtRuntime *ctx) {
    switch (index) {
    case 0:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 4, D_004374A0, ctx->value));
        return;
    case 2:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_00423428));
        return;
    case 3:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, " U,D = VALUE-+10"));
        return;
    case 4:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_004233F0));
        return;
    case 5:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_00423400));
        break;
    }
}

extern s32 evtDrawValueChangeNoticeRow(SdfListHead *list, s32 x, s32 y);
extern void evtDrawValueChangeInstructionRow(SdfListHead *list, s32 x, s32 y, u32 index, EvtRuntime *ctx);

s32 evtUpdateValueChangeDialog(s32 x, s32 y, EvtRuntime *ctx) {
    SdfListHead *list;
    s32 step;

    list = sdfCreateResetPacketList();
    evtDrawMenuFrame(list, x, y, 0x16, 9, 0, 1, ctx, evtDrawValueChangeNoticeRow, evtDrawValueChangeInstructionRow);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, list);
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

s32 mnuDrawFrameChangeLabel(SdfListHead *target, s32 x, s32 y) {
    sdfAppendPacket(target, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, "FRAME CHANGE."));
    return 2;
}

void evtViewerDrawFrameChangeRow(SdfListHead *list, s32 x, s32 y, u32 index, EvtRuntime *ctx) {
    switch (index) {
    case 0:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 4, D_004374A0, ctx->value));
        return;
    case 2:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, " L,R = FRMAE-+"));
        return;
    case 3:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, " U,D = FRAME-+10"));
        return;
    case 4:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, "L1,R1= FRAME-+100"));
        return;
    case 5:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_004233F0));
        return;
    case 6:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_00423400));
        return;
    case 7:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, " RL  = NOW FRAME"));
        return;
    case 8:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, " ST  = CAMERA FOCUS"));
        break;
    case 9:
        break;
    }
}

extern void evtApplyViewerTimelineFrame(s32 time, EvtRuntime *viewer);
extern void evtViewerDrawFrameChangeRow(SdfListHead *list, s32 x, s32 y, u32 index, EvtRuntime *ctx);

s32 evtViewerFrameChangeUpdate(s32 x, s32 y, EvtRuntime *ctx) {
    SdfListHead *list;
    s32 step;

    list = sdfCreateResetPacketList();
    evtDrawMenuFrame(list, x, y, 0x16, 0xB, 0, 1, ctx, mnuDrawFrameChangeLabel, evtViewerDrawFrameChangeRow);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, list);
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
            evtViewerDispatchFlagMode(ctx);
            evtApplyViewerTimelineFrame(ctx->curFrame, ctx);
        }
    }
    return 0;
}

extern const char *D_004374B8[];
extern const char *D_00423688[];
extern char D_004374C8[];
extern char D_004374D0[];
/* The camp provider owns the scene type (EvtRuntime); the viewer passes its runtime. */
extern u32 mnuCampGetPrimaryOption(void *scene);
extern u32 mnuCampGetSecondaryOption(void *scene);

/* Draw a project-menu command, including its current toggle/skip option. */
void evtDrawProjectCommandRow(SdfListHead *list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
    char labels[11][30] = {
        "SAVE PROJ    >>",
        "LOAD PROJ    >>",
        "LOAD NEW PROJ>>",
        "SET START FRAME",
        "SET END   FRAME",
        "SET TOTAL FRAME",
        "CAPTURE        ",
        "BATCH CAPTURE  ",
        "MOVE ALLFRAME ",
        "BISTA",
        "SKIP"
    };
    const char *primaryOptions[2];
    const char *secondaryOptions[3];
    s32 style;

    memcpy(primaryOptions, D_004374B8, sizeof(primaryOptions));
    memcpy(secondaryOptions, D_00423688, sizeof(secondaryOptions));
    if (index < 11) {
        if (ctx->inputA == index) {
            if (ctx->actionMode == 1) {
                style = 4;
            } else {
                style = 5;
            }
        } else {
            style = 0;
        }
        if (index == 9) {
            sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style,
                D_004374C8, labels[index], primaryOptions[mnuCampGetPrimaryOption(ctx)]));
        } else if (index == 10) {
            sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style,
                D_004374C8, labels[index], secondaryOptions[mnuCampGetSecondaryOption(ctx)]));
        } else {
            sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style,
                D_004374D0, labels[index]));
        }
    }
}

s32 mnuDrawInfoWindowA(s32 x, s32 y, EvtRuntime *ctx) {
    SdfListHead *list;

    list = sdfCreateResetPacketList();
    evtDrawMenuFrame(list, x, y, 0xF, 0xB, 0, 0xB, ctx, NULL, evtDrawProjectCommandRow);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, list);
    if (ctx->actionMode != 1) {
        return 0;
    }
    return kwlnStepTwoListCursors(0, 1, 0xB, 1, 0xB, 0, 0, 0, &ctx->inputA);
}

extern char D_004374D0[];

/* Draw the runtime title when present; return two rows used, or zero for no title. */
s32 evtDrawStringEntry(SdfListHead *list, s32 x, s32 y, EvtRuntime *ctx) {
    if (ctx->title == NULL) {
        return 0;
    }
    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_004374D0, ctx->title));
    return 2;
}

extern char D_004374D8[]; /* " %s" */

/* Draw an in-range item name, distinguishing selected active and inactive rows. */
void evtDrawSelectableTextRow(SdfListHead *list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
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
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_004374D8, ctx->itemNames[index]));
    }
}

extern s32 strlen(const char *s);

/* Draw a title-sized selection dialog; only action mode 2 advances its cursor. */
s32 evtUpdateTextSelectionDialog(s32 x, s32 y, EvtRuntime *ctx) {
    SdfListHead *list;
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
    evtDrawMenuFrame(list, x, y, width, ctx->itemCount + 3, 0, ctx->itemCount, ctx, evtDrawStringEntry, evtDrawSelectableTextRow);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, list);
    if (ctx->actionMode != 2) {
        return 0;
    }
    return kwlnStepTwoListCursors(0, 1, ctx->itemCount, 1, ctx->itemCount, 0, 0, 0, &ctx->cursor);
}

extern char D_004374E0[];
extern char D_004374D0[];

s32 evtDrawInputValueRow(SdfListHead *list, s32 x, s32 y, u8 *ctx) {
    char text[16];
    func_0035C860(text, D_004374E0, (s32)ctx + 0x22D4, (s32)ctx + 0x22E0);
    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_004374D0, text));
    return 2;
}

extern s8 D_003C9688[][12];
extern char D_004374E8[]; /* "%c" */

/* Draw eleven characters from a twelve-byte keyboard row and highlight the
 * runtime character selection, using mode 3 for the active color. */
void evtDrawKeyboardRow(SdfListHead *list, s32 x, s32 y, s32 row, EvtRuntime *ctx) {
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
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x + (i + 1) * 0xC0, y, 0xFEFFFF, color, D_004374E8, D_003C9688[row][i]));
    }
}

/* Draw the keyboard and edit the eight-character name when input is active. */
s32 func_002515C8(s32 x, s32 y, EvtRuntime *ctx) {
    SdfListHead *list;
    s32 input;
    s32 length;
    s32 key;

    list = sdfCreateResetPacketList();
    evtDrawMenuFrame(list, x, y, 14, 6, 0, 4, (u8 *)ctx,
                     evtDrawInputValueRow, evtDrawKeyboardRow);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface,
                                    list);
    if (ctx->actionMode != 3) {
        return 0;
    }
    input = kwlnStepTwoListCursors(0, 11, 4, 11, 4, NULL, NULL,
                                  &ctx->charCol, &ctx->charRow);
    if (input < 0) {
        return -1;
    }
    if (input == 1) {
        key = D_003C9688[ctx->charRow][ctx->charCol];
        if (ctx->charRow == 3 && ctx->charCol >= 4) {
            switch (key) {
            case 'B':
            case 'S':
                for (length = 0; ctx->nameStorage[length] != 0; length++) {
                }
                if (length > 0) {
                    ctx->nameStorage[length - 1] = 0;
                }
                break;
            case 'K':
            case 'O':
                if (ctx->nameStorage[0] != 0) {
                    return 1;
                }
                break;
            }
        } else {
            for (length = 0; ctx->nameStorage[length] != 0; length++) {
            }
            if (length < 8) {
                ctx->nameStorage[length] = key;
                ctx->nameStorage[length + 1] = 0;
            }
        }
    }
    return 0;
}


extern u16 D_004372B0;
extern u16 D_004372B2;

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423668);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423678);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423688);

s32 evtDrawEventFileNameRow(SdfListHead *list, s32 x, s32 y) {
    char text[32];
    func_0035C860(text, "[E%3d_%03d.PM1+2+3]", D_004372B0, D_004372B2);
    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_004374D0, text));
    return 2;
}

void evtDrawEventCutSelectRow(SdfListHead *list, s32 x, s32 y, u32 index, EvtRuntime *ctx) {
    s32 color = 0;

    if (ctx->charRow == index) {
        color = 4;
    }
    switch (index) {
    case 0:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0xE, D_004374F0));
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x + 0x600, y, 0xFEFFFF, color, D_004374F8, D_004372B0));
        return;
    case 1:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0xE, D_00437500));
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x + 0x600, y, 0xFEFFFF, color, D_00437508, D_004372B2));
        return;
    case 3:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, " L,R = NO-+"));
        return;
    case 4:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, " U,D = SELECT"));
        return;
    case 5:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_004233F0));
        return;
    case 6:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_00423400));
        return;
    case 7:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, " RL  = SET 600"));
        break;
    }
}

s32 evtUpdateEventCutSelectDialog(s32 x, s32 y, EvtRuntime *ctx) {
    SdfListHead *list;
    u32 num;

    list = sdfCreateResetPacketList();
    evtDrawMenuFrame(list, x, y, 0x12, 0xB, 0, 8, ctx, evtDrawEventFileNameRow, evtDrawEventCutSelectRow);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, list);
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

void evtDrawSelectedEntryLabel(SdfListHead *list, EvtRuntimeGroup *selected, s32 x,
                               s32 unused, EvtRuntime *runtime) {
    x += 0x6C0;
    kwlnDrawSpriteCell((void *)list, (x - 0x7000) >> 4, 0x3C, 0x1C, 3);
    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, 0x7AE0, 0xFEFFFF, 0xE, D_00437510));
    if (selected->entryHeader >= 0) {
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(
            x + 0x3C0, 0x7AE0, 0xFEFFFF, 0, D_004374D0,
            runtime->entryName[selected->entryHeader]));
    }
}

/* Draw one event-group row, highlighting the entry under the cursor. */
void func_00251ED0(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
    char names[33][30] = {
        "STAGE",
        "UNIT",
        "CAMERA",
        "D3P",
        "MESSAGE",
        "SE",
        "FADE",
        "QUAKE",
        "BLUR",
        "LIGHT",
        "SLIGHT",
        "SFOG",
        "FF",
        "BLUR2",
        "MBLUR",
        "DBLUR",
        "FILTER",
        "MFILTER",
        "BED",
        "BGM",
        "MG1",
        "MG2",
        "FBOKASI",
        "RBLUR",
        "TMX",
        "RAIN",
        "EPL",
        "HBLUR",
        "PADACT",
        "MOVIE",
        "TIMEI",
        "RENDERTEX",
        "BISTA"
    };
    EvtRuntimeGroup *group;
    s32 style;
    s32 i;

    if (ctx->entryCount == 0) {
        return;
    }
    if (index < ctx->entryCount) {
        group = ctx->groups;
        for (i = 0; i < index; i++) {
            group = group->next;
        }
        if (ctx->entryFirst + ctx->entryCursor == index) {
            if (ctx->actionMode == 4) {
                ctx->frameColumn = 0;
                ctx->frameFirst = 0;
                ctx->frameCursor = 0;
                ctx->frameGroup = group;
                style = 4;
            } else {
                style = 5;
            }
            evtDrawSelectedEntryLabel(list, group, x, y, ctx);
        } else {
            style = 0;
        }
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004374D0, names[group->type]));
    }
}

extern void func_00251ED0(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx);

s32 evtUpdateEntrySelectionDialog(s32 x, s32 y, EvtRuntime *ctx) {
    SdfListHead *list;
    s32 count;
    s32 shown;

    list = sdfCreateResetPacketList();
    evtDrawMenuFrame(list, x, y, 8, 0x1D, ctx->entryFirst, ctx->entryCount, ctx, NULL, func_00251ED0);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, list);
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

extern const char *D_003C94C8[];
extern s8 D_003C9520[];
extern char D_00437518[];

/* Header row of the frame list: one text cell per column of the selected group's table row. */
s32 func_002521C8(SdfListHead *list, s32 x, s32 y, u8 *data) {
    EvtRuntime *ctx = (EvtRuntime *)data;
    s32 type = ctx->frameGroup->type;
    s32 i;
    s32 kind;

    for (i = 0; i < D_003C9538[type].columns; i++) {
        kind = D_003C9538[type].columnTypes[i];
        if (kind == 0x15) {
            ctx->frameTextFormat = D_00437518;
            ctx->frameTextWidth = 0;
            sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0xE, D_004374D0, D_00437518));
            x += ctx->frameTextWidth * 0xC0;
        } else {
            sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0xE, D_004374D0, D_003C94C8[kind]));
            x += D_003C9520[D_003C9538[type].columnTypes[i]] * 0xC0;
        }
    }
    return 1;
}

extern char D_004375A8[]; /* "%4d" */
extern char D_004375B0[]; /* " ---" */
extern char D_004375B8[]; /* "..." */
extern char D_004375C0[]; /* "  x" */
extern char D_004375C8[]; /* "ON " */
extern char D_004375D0[]; /* "OFF" */
extern char D_004375D8[]; /* "NORMAL" */
extern char D_004375E0[]; /* "ADD" */
extern char D_004375E8[]; /* "SUB" */
extern char D_004375F0[]; /* " -- " */
extern char D_004375F8[]; /* "1STOP" */
extern char D_00437600[]; /* "DOWN" */
extern char D_00437608[]; /* "UP" */
extern char D_00437610[]; /* " IN " */
extern char D_00437618[]; /* " OUT " */
extern char D_00437638[]; /* "%.1f" */
extern char D_00437648[]; /* "%d" */
extern char D_00437650[]; /* "%-.11s" */
extern char D_00437658[]; /* "N" */
extern char D_00437660[]; /* "B%d" */
extern char D_00437668[]; /* " X" */
extern char D_00437670[]; /* " -" */
extern char D_00437678[]; /* "LEV %d" */
extern char D_00437680[]; /* "DEF " */
extern char D_00437688[]; /* "%3d " */
extern char D_00437690[]; /* " x" */
extern char D_00437698[]; /* "CUR" */
extern char D_004376A0[]; /* "DEF" */
extern s8 D_003C9520[];
extern char *D_003C96B8[];
extern char *D_003C96D8[];
extern char *D_003C96F8[];
extern char *D_003C9718[];
extern char *D_003C9720[];
extern char *D_003C9728[];

extern char D_00437578[]; /* "HIGH" */
extern char D_00437620[]; /* "LOW " */
extern char D_00437628[]; /* "OUT" */
extern char D_00437630[]; /* "IN" */
extern char D_00437640[]; /* "X" */

/* Format each enabled column of a timeline key. Some columns reuse the
 * preceding scalar selection; the selected group kind is fixed for this row. */
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

void func_00252378(SdfListHead *list, s32 x, s32 y, s32 color,
                   EvtRuntimeChild *node, EvtRuntime *ctx) {
    char name[32];
    s32 type;
    s32 column;
    s32 value;
    s32 style;
    s32 unavailable;
    s32 bank;

    type = ctx->frameGroup->type;
    value = 0;
    for (column = 0; column < D_003C9538[type].columns; column++) {
        style = color;
        if (ctx->frameColumn != column) {
            style = 0;
        }
        switch (D_003C9538[type].columnTypes[column]) {
        case 0:
            sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004375A8, node->frame + ctx->frameGroup->metadata.value));
            break;
        case 9:
            switch (type) {
            case 18: value = node->p08.sh[0]; break;
            case 3: value = node->p08.sh[1]; break;
            case 20: value = node->p08.sh[1]; break;
            case 21: value = node->p08.sh[1]; break;
            case 26: value = node->p08.sh[1]; break;
            }
            sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004375A8, value));
            break;
        case 15:
            switch (type) {
            case 3:
            case 20:
            case 21:
            case 26: value = node->duration; break;
            }
            sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004375A8, value));
            break;
        case 1:
            if (type != 10) {
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004375A8, node->duration));
            } else if (ctx->frameGroup->metadata.extra1 != 3) {
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004375B0));
            } else {
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004375A8, node->duration));
            }
            break;
        case 13:
            sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004375B8));
            break;
        case 14:
            sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, "%.1f %.2f", node->p08.f, node->p0C.f));
            break;
        case 2:
            if (type == 7) {
                value = node->p08.sh[0];
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004375A8, value));
            }
            break;
        case 3:
            switch (type) {
            case 8: value = node->p08.sh[0]; break;
            case 12: value = node->p08.sh[1]; break;
            case 31: value = node->p08.sh[1]; break;
            }
            if (type == 31 && node->p08.sh[0] == 0) {
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004375C0));
            } else {
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004375A8, value));
            }
            break;
        case 4:
            sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004375A8, node->p08.sh[1]));
            break;
        case 5:
            switch (type) {
            case 10: value = node->serializedValue; break;
            case 11: value = node->p08.sh[0]; break;
            case 13: value = node->p08.sh[0]; break;
            case 14: value = node->p08.sh[0]; break;
            case 15: value = node->p08.sh[0]; break;
            case 16: value = node->p08.sh[0]; break;
            case 17: value = node->p08.sh[0]; break;
            case 23: value = node->p08.sh[0]; break;
            case 27: value = node->p08.sh[0]; break;
            case 24: value = node->p08.sb[0]; break;
            case 8: value = node->p0C.sh[0]; break;
            case 25: value = node->p0C.sh[0]; break;
            }
            if (value) {
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004375C8));
            } else {
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004375D0));
            }
            break;
        case 12:
            switch (type) {
            case 12:
                switch (node->p08.sh[0]) {
                case 0: sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004375D8)); break;
                case 1: sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004375E0)); break;
                case 2: sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004375E8)); break;
                }
                break;
            case 6:
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004374D0, D_003C96B8[node->p08.sh[0]]));
                break;
            case 19:
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004374D0, D_003C96D8[node->p08.sh[0]]));
                break;
            case 1:
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004374D0, D_003C96F8[node->p08.sb[0]]));
                break;
            case 28:
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004374D0, D_003C9718[node->p08.sh[0]]));
                break;
            case 10:
                if (node->p14.sh[0] == 0) {
                    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004375F0));
                } else {
                    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004375F8));
                }
                break;
            case 30:
                if (node->p08.sh[1] == 0) {
                    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_00437600));
                } else {
                    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_00437608));
                }
                break;
            case 31:
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004374D0, D_003C9728[node->p08.sh[0]]));
                break;
            case 32:
                if (node->p08.sh[0] == 0) {
                    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_00437610));
                } else {
                    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_00437618));
                }
                break;
            case 24:
                if (node->p1C.sb[0] == 0) {
                    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_00437578));
                } else {
                    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_00437620));
                }
                break;
            case 3:
            case 20:
            case 21:
            case 26:
                if (node->p10.sb[0] == 0) {
                    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_00437628));
                } else {
                    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_00437630));
                }
                break;
            case 29:
                if (node->p0C.sb[0] == 0) {
                    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004375D8));
                } else {
                    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, "END STOP"));
                }
                break;
            }
            break;
        case 11:
            switch (type) {
            case 9: value = node->p08.sh[1]; break;
            case 22: value = node->p08.sh[0]; break;
            case 25: value = node->p08.sh[0]; break;
            }
            if (value) {
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004375C8));
            } else {
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004375D0));
            }
            break;
        case 16:
            switch (type) {
            case 3: value = node->p0C.sb[0]; break;
            case 20:
            case 21: value = node->p08.sb[1]; break;
            case 26: value = node->p0C.sb[0]; break;
            }
            if (value) {
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004375C8));
            } else {
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004375D0));
            }
            break;
        case 6:
            sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_00437638, node->p08.f * 57.29577637f));
            break;
        case 8:
            unavailable = 0;
            switch (type) {
            case 3: value = node->p0C.sb[1]; break;
            case 26: value = node->p0C.sb[1]; break;
            case 12: value = node->p0C.i; break;
            case 18: value = node->p0C.sb[0]; break;
            case 19: value = node->p08.sh[1]; break;
            case 28: value = node->p08.sh[1]; break;
            case 4: value = node->p08.sh[0]; break;
            case 5: value = node->p08.sh[0]; break;
            case 29: value = node->p08.sh[0]; break;
            case 30: value = node->p08.sh[0]; break;
            case 1:
                if (node->p08.sb[0] == 5) {
                    value = node->p18.sh[0];
                } else {
                    unavailable = 1;
                }
                break;
            case 2: value = node->p0C.sh[1]; break;
            }
            if (unavailable) {
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_00437640));
            } else {
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_00437648, value));
            }
            break;
        case 21:
            value = 0;
            sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_00437648, value));
            break;
        case 7:
            if (type == 5) {
                value = node->p08.sh[1];
            }
            sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004375A8, value));
            break;
        case 10:
            switch (type) {
            case 3: value = node->p0C.sh[1]; break;
            case 26: value = node->p0C.sh[1]; break;
            case 9: value = node->p08.sh[0]; break;
            case 18: value = node->p08.sh[1]; break;
            }
            if (value < 0) {
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, "-----------"));
            } else {
                memcpy(name, ctx->entryName[value], sizeof(name));
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_00437650, name));
            }
            break;
        case 17:
            value = node->p08.sh[0];
            bank = (value >> 12) & 15;
            if (bank == 0) {
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_00437658));
            } else {
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_00437660, bank - 1));
            }
            sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x + 0x240, y, 0xFEFFFF, style, D_00437648, value & 0xFFF));
            break;
        case 18:
            unavailable = 0;
            switch (type) {
            case 4:
                value = node->p08.sh[1];
                if (node->p08.h[0] >> 12) unavailable = 1;
                break;
            case 1:
                if (node->p08.sb[0] >= 6) {
                    unavailable = 1;
                } else if (node->p08.sb[0] < 0) {
                    unavailable = 1;
                } else {
                    value = node->p10.sh[0];
                }
                break;
            case 12: value = node->p14.sh[0]; break;
            case 16: value = node->p14.sh[0]; break;
            case 17: value = node->p14.sh[0]; break;
            case 5: value = node->p08.sh[1]; break;
            case 2:
            case 19: value = node->p0C.sh[0]; break;
            }
            if (unavailable) {
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_00437668));
            } else if (((value >> 12) & 15) == 0) {
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_00437670));
            } else {
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, "B%d = %d", ((value >> 12) & 15) - 1, value & 0xFFF));
            }
            break;
        case 19:
            switch (node->p08.sb[0]) {
            case 5:
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, "%d-%d %s H%d", node->p0C.sb[0], node->p0C.sb[1], D_003C9720[node->p0C.sb[2]], node->p0C.sb[3]));
                break;
            case 6:
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_00437678, node->p0C.sb[0]));
                break;
            case 7:
                value = node->p0C.sh[0];
                if (value < 0) {
                    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, "-----------"));
                } else {
                    memcpy(name, ctx->entryName[value], sizeof(name));
                    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_00437650, name));
                }
                break;
            case 3:
                if (node->p0C.sb[0] == 0 || node->p0C.sb[0] == 2) {
                    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_00437680));
                } else {
                    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_00437688, node->p0C.b[1]));
                }
                if (node->p0C.b[0] < 2) {
                    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, "    DEF "));
                } else {
                    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, "    %.1f", node->p14.f));
                }
                break;
            case 4:
                if (node->p0C.sb[0] == 0 || node->p0C.sb[0] == 2) {
                    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_00437680));
                } else {
                    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_00437688, node->p0C.b[1]));
                }
                break;
            default:
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_00437690));
                break;
            }
            break;
        case 20:
            value = 0;
            switch (type) {
            case 14:
            case 15:
            case 17:
            case 23: value = node->p10.sh[0]; break;
            }
            if (value == 0) {
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_00437698));
            } else if (value == 1) {
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004376A0));
            } else {
                value -= 2;
                memcpy(name, ctx->entryName[value], sizeof(name));
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_004374D0, name));
            }
            break;
        }
        x += D_003C9520[D_003C9538[type].columnTypes[column]] * 0xC0;
    }
}

extern void func_00252378(SdfListHead *list, s32 x, s32 y, s32 color, EvtRuntimeChild *node, EvtRuntime *ctx);

/* Draw an indexed child of the selected group, or its trailing new-frame row. */
s32 evtDrawFrameListRow(SdfListHead *list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
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
            sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color ? color : 8, "----- NEW FRAME -----"));
        }
    }
}

s32 evtUpdateFrameListDialog(s32 x, s32 y, EvtRuntime *ctx) {
    SdfListHead *list;
    s32 shown = 20;
    s32 count;
    s32 result;
    s16 columns;
    EvtRuntimeGroup *group;
    EvtRuntimeChild *node;

    if (ctx->entryCount == 0) {
        return 0;
    }
    group = ctx->frameGroup;
    if (D_003C9538[group->type].columns == 0) {
        return -1;
    }
    list = sdfCreateResetPacketList();
    evtDrawMenuFrame(list, x, y, 39, 21, ctx->frameCursor,
                 group->childCount + 1, ctx, func_002521C8, evtDrawFrameListRow);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, list);
    if (ctx->actionMode != 5) {
        return 0;
    }
    columns = D_003C9538[group->type].columns;
    if (group->childCount + 1 < shown) {
        shown = group->childCount + 1;
    }
    if (evtIsMenuTableEntryEnabled(&group->type) == 1) {
        if (D_0037F510.decTen < 0) {
            if (ctx->frameCursor + ctx->frameFirst == 0) {
                return -4;
            }
        } else if (D_0037F510.incTen < 0) {
            count = 0;
            for (node = group->children; node != NULL; node = node->next) {
                count++;
            }
            if (ctx->frameCursor + ctx->frameFirst == count) {
                return -4;
            }
        }
    }
    result = kwlnStepTwoListCursors(0, columns, group->childCount + 1,
                                  columns, shown, NULL, &ctx->frameCursor,
                                  &ctx->frameColumn, &ctx->frameFirst);
    if (D_0037F510.syncKey < 0) {
        result = -2;
    }
    if (D_0037F510.unk2D < 0) {
        result = -3;
    }
    return result;
}


extern EffWorldNode *dds3GetWorldObject();
extern char D_004374D8[]; /* " %s" */

void evtViewerDrawWorldNodeRow(SdfListHead *list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
    s32 color;
    s32 count;
    s32 i;
    EffWorldNode *node;

    color = 4;
    if (ctx->groupFirst + ctx->groupCursor != index) {
        color = 0;
    }
    if (index == 0) {
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, " -----------------------"));
        return;
    }
    count = 0;
    for (i = 0; i < 0x12; i++) {
        if (i != EVT_WORLD_SLOT_MOVIE) {
            for (node = ((EvtWorldTable *)dds3GetWorldObject()->data)->slots[i].head; node != NULL; node = node->next) {
                if (((char *)node->value) != NULL) {
                    count++;
                    if (count == index) {
                        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_004374D8, ((char *)node->value)));
                        return;
                    }
                }
            }
        }
    }
}

s32 evtUpdateWorldNodeListDialog(s32 x, s32 y, EvtRuntime *ctx) {
    s32 i;
    s32 count = 0;
    SdfListHead *list;
    s32 shown = 0x17;
    EffWorldNode *node;

    list = sdfCreateResetPacketList();
    for (i = 0; i < 0x12; i++) {
        if (i != EVT_WORLD_SLOT_MOVIE) {
            for (node = ((EvtWorldTable *)dds3GetWorldObject()->data)->slots[i].head;
                 node != NULL; node = node->next) {
                if (node->value != NULL) {
                    count++;
                }
            }
        }
    }
    count++;
    evtDrawMenuFrame(list, x, y, 0x1A, 0x17, ctx->groupFirst, count, ctx,
                     NULL, evtViewerDrawWorldNodeRow);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface,
                                    list);
    if (ctx->actionMode != 10) {
        return 0;
    }
    if (count < shown) {
        shown = count;
    }
    return kwlnStepTwoListCursors(0, 1, count, 1, shown, NULL,
                                 &ctx->groupFirst, NULL, &ctx->groupCursor);
}

extern char D_004376A8[]; /* "P%d:" */
extern char D_004376B0[]; /* "   %s" */
extern EvtRuntimeChild *evtEventViewerGetPendingNode(EvtRuntime *runtime);

/* The pending-node's signed slot indices begin at +0xC (also used in DDS1). */
void evtViewerDrawPendingNodeRow(SdfListHead *list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
    EffWorldNode *node;
    s32 color;
    s32 slot;

    node = NULL;
    color = 4;
    if (ctx->inputB != index) {
        color = 0;
    }
    slot = evtEventViewerGetPendingNode(ctx)->parameterBytes[4 + index];
    if (slot >= 0) {
        node = dds3FindObjectChainNodeByName(dds3GetWorldObject(), ctx->entryName[slot]);
    }
    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0xE, D_004376A8, index));
    if (node != NULL) {
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_004376B0, ((char *)node->value)));
    } else {
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, "   -----------------------"));
    }
}

extern void evtViewerDrawPendingNodeRow(SdfListHead *list, s32 x, s32 y, s32 index, EvtRuntime *ctx);

/* Draw the pending-node selector for group types 20/21; other types return -1. */
s32 mnuDrawInfoWindowB(s32 x, s32 y, EvtRuntime *ctx) {
    SdfListHead *list;
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
    evtDrawMenuFrame(list, x, y, 0x1C, rows, 0, rows, ctx, NULL, evtViewerDrawPendingNodeRow);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, list);
    if (ctx->actionMode != 0xC) {
        return 0;
    }
    return kwlnStepTwoListCursors(0, 1, rows, 1, rows, 0, 0, 0, &ctx->inputB);
}

extern char D_00423EC0[]; /* "MESSAGE MENU (MESMAX %3d)" */

/* The message-menu work references a window whose entry handle lives at +0x104. */


/* Draw the message count from the runtime window context
 * and its entry handle, returning two rows used. */
s32 mnuDrawMessageMenuLabel(SdfListHead *list, s32 x, s32 y, EvtRuntime *ctx) {
    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_00423EC0, itfMesGetEntryCount(ctx->windowContext->handle)));
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

void evtDrawMessageDataRow(SdfListHead *list, s32 x, s32 y, s32 kind, EvtRuntime *ctx) {
    char *names[11] = {D_004376B8, D_00423EE0, D_00423EF0, D_00423F00,
        D_00423F10, D_00423F20, D_00423F30, D_00423F40, D_00423F50, D_00423F60, D_00423F70};

    switch (kind) {
    case 0: {
        s32 color = 4;
        if (ctx->messageField != 0) {
            color = 0;
        }
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_004374A0, ctx->value & 0xFFF));
        if (ctx->windowContext->handle == -1) {
            sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x + 0x600, y, 0xFEFFFF, color, "NONE MESDATA!!"));
        } else if (itfMesGetWindowEntryItems(ctx->windowContext->handle, ctx->value & 0xFFF) == 0) {
            sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x + 0x600, y, 0xFEFFFF, color, "(NORMAL)"));
        } else {
            sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x + 0x600, y, 0xFEFFFF, color, "(BRANCH)"));
        }
        break;
    }
    case 1: {
        s32 color = 4;
        if (ctx->messageField != 1) {
            color = 0;
        }
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_004376C0, names[(ctx->value >> 12) & 0xF]));
        break;
    }
    case 3:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_00423428));
        break;
    case 4:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, " U,D = SELECT "));
        break;
    case 5:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_004233F0));
        break;
    case 6:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_00423400));
        break;
    }
}

s32 evtUpdateMessageValueDialog(s32 x, s32 y, EvtRuntime *ctx) {
    SdfListHead *list;
    s32 packed;
    s32 number;
    s32 branch;
    s32 field;
    s32 delta;
    s32 handle;

    list = sdfCreateResetPacketList();
    evtDrawMenuFrame(list, x, y, 0x18, 0xA, 0, 1, ctx, mnuDrawMessageMenuLabel, evtDrawMessageDataRow);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, list);
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
        handle = ctx->windowContext->handle;
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



s32 mnuDrawCutFlagLabel(SdfListHead *target, s32 x, s32 y) {
    sdfAppendPacket(target, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, "CUTFLAG MENU"));
    return 2;
}


/* Draw the packed four-bit option or twelve-bit number and highlight
 * the selected comparison field; later rows provide input instructions. */
void evtDrawComparisonValueRow(SdfListHead *list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
    char *labels[11] = {D_004376C8, D_00423EE0, D_00423EF0, D_00423F00,
        D_00423F10, D_00423F20, D_00423F30, D_00423F40, D_00423F50, D_00423F60, D_00423F70};
    s32 flag;

    switch (index) {
    case 0:
        flag = ctx->compareField != 0 ? 0 : 4;
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, flag, D_004376C0, labels[(ctx->value >> 12) & 0xF]));
        return;
    case 1:
        flag = ctx->compareField != 1 ? 0 : 4;
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, flag, D_004374A0, ctx->value & 0xFFF));
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x + 0x600, y, 0xFEFFFF, flag, "  (CMP VALUE)"));
        return;
    case 3:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_00423428));
        return;
    case 4:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, " U,D = SELECT "));
        return;
    case 5:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_004233F0));
        return;
    case 6:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_00423400));
        break;
    }
}

s32 evtUpdateComparisonValueDialog(s32 x, s32 y, EvtRuntime *ctx) {
    SdfListHead *list;
    s32 packed;
    s32 number;
    s32 branch;
    s32 field;
    s32 delta;

    list = sdfCreateResetPacketList();
    evtDrawMenuFrame(list, x, y, 0x18, 0xA, 0, 1, ctx, mnuDrawCutFlagLabel, evtDrawComparisonValueRow);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, list);
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
    return D_003C9730[*index].columnCount != 0;
}

/* Return the table value selected by this group's type and editor column. */
s32 mnuGetSelectedTableValue(EvtRuntime *runtime) {
    return D_003C9730[runtime->frameGroup->type].columns[runtime->tableColumn];
}

extern char *D_003C9880[];
extern char *D_003C9890[];
extern char D_004376F8[];
extern char D_00437700[];
extern char D_00437708[];
extern s8 D_004376D0[3];

void evtDrawGroupPropertyTable(SdfListHead *list, s32 x, s32 y, s32 hidden, EvtRuntime *runtime) {
    s32 offset = 0;
    EvtRuntimeGroup *group = runtime->frameGroup;
    s32 i;
    s32 field;
    s32 style;

    if (hidden != 0) {
        return;
    }
    for (i = 0; i < D_003C9730[group->type].columnCount; i++) {
        field = D_003C9730[group->type].columns[i];
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x + offset * 192, y,
            0xFEFFFF, 14, D_003C9880[field]));
        style = i == runtime->tableColumn && runtime->actionMode == 15 ? 4 : 0;
        switch (field) {
        case 0:
            sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x + (offset * 12 << 4),
                y + 0x80, 0xFEFFFF, style, D_004376F8, runtime->frameGroup->metadata.value));
            break;
        case 1:
            if (runtime->frameGroup->metadata.extra1 < 3) {
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x + (offset * 12 << 4),
                    y + 0x80, 0xFEFFFF, style, D_004374D0,
                    D_003C9890[runtime->frameGroup->metadata.extra1]));
            } else {
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x + (offset * 12 << 4),
                    y + 0x80, 0xFEFFFF, style, D_004374D0,
                    runtime->entryName[runtime->frameGroup->metadata.extra2]));
            }
            break;
        case 2:
            if (runtime->frameGroup->metadata.extra1 == 0) {
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x + (offset * 12 << 4),
                    y + 0x80, 0xFEFFFF, style, D_00437700));
            } else {
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x + (offset * 12 << 4),
                    y + 0x80, 0xFEFFFF, style, D_00437708));
            }
            break;
        }
        offset += D_004376D0[field];
    }
}

extern char *D_003C98A0[];

INCLUDE_RODATA(const s32, "game/code_00250010", D_00424080);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00424090);

s32 evtUpdateGroupPropertyDialog(s32 x, s32 y, EvtRuntime *runtime) {
    SdfListHead *packets;
    EvtRuntimeGroup *group;
    EvtRuntimeChild *child;
    s32 column;
    s32 field;
    s32 count;

    packets = sdfCreateResetPacketList();
    group = runtime->frameGroup;
    if (group == NULL) {
        return 0;
    }
    if (D_003C9730[group->type].columnCount == 0) {
        return 0;
    }
    evtDrawMenuFrame((u32)packets, x, y, 28, 3, 0, 1, (u8 *)runtime, NULL, evtDrawGroupPropertyTable);
    kwlnPositionedTextSurface.append((SdfListHead *)&kwlnPositionedTextSurface, packets);
    if (runtime->actionMode != 15) {
        return 0;
    }
    column = runtime->tableColumn;
    if (column >= D_003C9730[group->type].columnCount && D_003C9730[group->type].columnCount > 0) {
        column = runtime->tableColumn = D_003C9730[group->type].columnCount - 1;
    }
    field = D_003C9730[group->type].columns[column];
    if (D_0037F510.incOne < 0) {
        if (column < D_003C9730[group->type].columnCount - 1) {
            runtime->tableColumn = column + 1;
        } else {
            runtime->tableColumn = 0;
        }
    } else if (D_0037F510.decOne < 0) {
        if (column > 0) {
            runtime->tableColumn = column - 1;
        } else {
            runtime->tableColumn = D_003C9730[group->type].columnCount == 0 ? 0 : D_003C9730[group->type].columnCount - 1;
        }
    } else if (D_0037F510.confirm < 0) {
        switch (field) {
        case 0:
            runtime->value = runtime->frameGroup->metadata.value;
            runtime->valueMin = 0;
            runtime->valueMax = runtime->headerThird - 1;
            evtApplyViewerTimelineFrame(runtime->curFrame, runtime);
            evtViewerPushCommandHistory(7, 180, 120, runtime);
            break;
        case 1:
            runtime->groupFirst = 0;
            runtime->groupCursor = 0;
            evtApplyViewerTimelineFrame(runtime->curFrame, runtime);
            evtViewerPushCommandHistory(17, 180, 120, runtime);
            break;
        case 2:
            runtime->cursor = runtime->frameGroup->metadata.extra1;
            runtime->itemCount = field;
            runtime->title = "PATH APPLY MODE";
            runtime->itemNames = D_003C98A0;
            evtViewerPushCommandHistory(2, 216, 120, runtime);
            break;
        }
        return 1;
    } else if (D_0037F510.frameEnd < 0) {
        if (column == 0) {
            runtime->frameColumn = 0;
        } else {
            runtime->frameColumn = 1;
        }
        count = 0;
        for (child = group->children; child != NULL; child = child->next) {
            count++;
        }
        if (count - 20 >= 0) {
            runtime->frameCursor = count - 19;
            runtime->frameFirst = 19;
        } else {
            runtime->frameFirst = count;
            runtime->frameCursor = 0;
        }
        return -1;
    } else if (D_0037F510.frameStart < 0) {
        if (column == 0) {
            runtime->frameColumn = 0;
        } else {
            runtime->frameColumn = 1;
        }
        runtime->frameCursor = 0;
        runtime->frameFirst = 0;
        return -1;
    }
    return D_0037F510.cancel >= 0 ? 0 : -1;
}

s32 mnuDrawMotionChangeLabel(SdfListHead *target, s32 x, s32 y) {
    sdfAppendPacket(target, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, "MOTION CHANGE MENU"));
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

extern Motion *mdlFindNodeById(MdlCtx *model, s32 index);

extern char D_00437710[];
extern char D_00437438[];

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

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376D0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376D8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376E0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376E8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376F0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376F8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437700);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437708);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437710);

void func_00254CE0(SdfListHead *list, s32 x, s32 y, s32 row, EvtRuntime *ctx) {
    s32 i;
    s32 count = 0;
    s32 style = 0;
    MdlCtx *model = (MdlCtx *)((EffectObjectData *)ctx->frameGroup->info->data)->modelHolder->resourceHandle;
    EvtMotionValue packed;
    char *loopNames[] = {D_00437710, D_00437438};

    if (ctx->fieldIndex == row) {
        style = 4;
    }
    packed.word = ctx->value;
    for (i = 0; i < 4; i++) {
        if (mdlFindNodeById(model, i) != NULL) {
            count++;
        }
    }
    switch (row) {
    case 0:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style,
            "GROUP   %d  (MAX %d)", packed.bits.group, count));
        break;
    case 1:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style,
            "MOTNO   %d  (MAX %d)", packed.bits.motion, mdlGetNodeRefHalf(model, packed.bits.group)));
        break;
    case 2:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style,
            "LOOP    %s", loopNames[packed.bits.loop]));
        break;
    case 3:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style,
            "HOKAN   %d", packed.bits.hokan));
        break;
    case 6:
        if (mdlFindNodeById(model, packed.bits.group)->motionTable->entries[packed.bits.motion] != NULL) {
            sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style,
                "MAXFRAME (%d)", mdlFindNodeById(model, packed.bits.group)->motionTable->entries[packed.bits.motion]->packedHeader));
        } else {
            sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, "(DUMMY)"));
        }
        break;
    }
}


extern void func_00254CE0();

/* Motion editor row: ctx->value packs group (byte 0), motion number (byte 1), loop flag (byte 2) and interpolation (byte 3). */
s32 evtUpdateMotionChangeRow(s32 x, s32 y, EvtRuntime *ctx) {
    SdfListHead *list;
    MdlCtx *model;
    s32 count;
    EvtMotionValue packed;

    list = sdfCreateResetPacketList();
    model = (MdlCtx *)((EffectObjectData *)ctx->frameGroup->info->data)->modelHolder->resourceHandle;
    evtDrawMenuFrame(list, x, y, 0x14, 0xA, 0, 1, ctx, mnuDrawMotionChangeLabel, func_00254CE0);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, list);
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
    count = mdlGetNodeRefHalf((MdlCtx *)model, packed.bits.group);
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
        if (packed.bits.motion >= mdlGetNodeRefHalf((MdlCtx *)model, packed.bits.group)) {
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
    if (D_0037F510.confirm < 0 && mdlFindNodeById(model, packed.bits.group)->motionTable->entries[packed.bits.motion] != NULL) {
        return 1;
    }
    return D_0037F510.cancel >= 0 ? 0 : -1;
}

extern char D_00424090[]; /* "UNIT ALL" */
extern char D_004376E8[]; /* "ALL" */
extern char D_004376F0[]; /* "DISABLE" */

void evtDrawGroupListRow(SdfListHead *list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
    EvtRuntimeGroup *group;
    s32 color;
    s32 n;

    color = 4;
    if (ctx->groupFirst + ctx->groupCursor != index) {
        color = 0;
    }
    if (index == 0) {
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_004376F0));
    } else if (index == 1) {
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_004376E8));
        return;
    } else if (index == 2) {
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_00424090));
        return;
    }
    n = 3;
    for (group = ctx->groups; group != NULL; group = group->next) {
        if (group->type == 1) {
            if (n == index) {
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_004374D0, ((char *)group->info->value)));
                return;
            }
            n++;
        }
    }
}

s32 evtUpdateGroupListDialog(s32 x, s32 y, EvtRuntime *ctx) {
    EvtRuntimeGroup *group;
    s32 count = 0;
    SdfListHead *packets;
    s32 shown = 15;

    for (group = ctx->groups; group != NULL; group = group->next) {
        if (group->type == 1) {
            count++;
        }
    }
    count += 3;
    packets = sdfCreateResetPacketList();
    evtDrawMenuFrame(packets, x, y, 20, 15, ctx->groupFirst, count, (u8 *)ctx, NULL, evtDrawGroupListRow);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, packets);
    if (ctx->actionMode != 17) {
        return 0;
    }
    if (count < shown) {
        shown = count;
    }
    return kwlnStepTwoListCursors(0, 1, count, 1, shown, 0, &ctx->groupFirst, 0, &ctx->groupCursor);
}

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

typedef struct EvtFloatPanelRow {
    u16 x;
    u16 y;
    s32 parameter;
    const char *format;
} EvtFloatPanelRow;

/* This coordinate table is supplied by the ordinary data segment. */
extern const u16 D_003C98C0[];
extern const EvtFloatPanelRow D_003C98C8[4];
extern char D_00437728[];
extern char D_00437750[];
extern char D_00437758[];
extern char D_00437760[];
extern char D_004375D0[];
extern u8 D_00438A34;
extern f32 D_00438A48;
extern f32 D_00438A4C;
extern void func_0023E320(s32 vectorSlot, f32 x, f32 y);

/* Draw the XY/overlay editor; cancel restores the captured values and flag. */
s32 func_002560B0(s32 x, s32 y, EvtRuntime *runtime) {
    SdfListHead *list;
    EvtRuntimeGroup *group;
    const EvtFloatPanelRow *row;
    s32 i;
    s32 status = 0;

    group = runtime->frameGroup;
    list = sdfCreateResetPacketList();
    evtDrawMenuFrame(list, x + runtime->horizontalOffset + 0x60, y,
                     0xF, 6, 0, 1, (u8 *)runtime, NULL, func_002560A8);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface,
                                     list);

    sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(
        0x8500 + (runtime->horizontalOffset << 4),
        D_003C98C0[runtime->floatSelection], 0xFF0080, 0, D_00437728));
    row = D_003C98C8;
    i = 0;
    do {
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(
            0x85C0 + (runtime->horizontalOffset << 4), row->y, 0xFF0080,
            row->parameter, row->format));
        ++row;
        ++i;
    } while (i != 4);

    {
        s32 valueX = 0x8B00 + (runtime->horizontalOffset << 4);
        s32 style = runtime->floatEditMode == 1 && runtime->floatSelection == 0 ? 6 : 0;

        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(
            valueX, 0x7A80, 0xFF0080, style, D_00437750,
            runtime->floatEditX));
    }
    {
        s32 valueX = 0x8B00 + (runtime->horizontalOffset << 4);
        s32 style = runtime->floatEditMode == 1 && runtime->floatSelection == 1 ? 6 : 0;

        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(
            valueX, 0x7AE0, 0xFF0080, style, D_00437758,
            runtime->floatEditY));
    }
    {
        s32 valueX = 0x8B00 + (runtime->horizontalOffset << 4);
        s32 style = runtime->floatEditMode == 1 && runtime->floatSelection == 2 ? 6 : 0;
        const char *toggleText;
        if (D_00438A34 == 0) {
            toggleText = D_00437760;
        } else {
            toggleText = D_004375D0;
        }

        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(
            valueX, 0x7BA0, 0xFF0080, style, toggleText));
    }

    if (runtime->actionMode != 0x13) {
        return status;
    }
    {
        if (D_0037F510.apply < 0) {
            if (runtime->floatEditMode == 0) {
                return 1;
            }
        } else if (D_0037F510.confirm < 0) {
            if (runtime->floatEditMode == 0) {
                runtime->floatEditMode = 1;
            } else if (runtime->floatEditMode == 1) {
                runtime->floatEditMode = 0;
            }
        } else if (D_0037F510.cancel < 0) {
            if (runtime->floatEditMode == 1) {
                runtime->floatEditMode = 0;
            } else {
                status = -1;
            }
        } else if (D_0037F510.incTen & 2) {
            if (runtime->floatEditMode == 0) {
                if (runtime->floatSelection < 2) {
                    runtime->floatSelection++;
                } else {
                    runtime->floatSelection = 0;
                }
            } else if (runtime->floatEditMode == 1) {
                switch (runtime->floatSelection) {
                case 0:
                    runtime->floatEditX += 0.1f;
                    break;
                case 1:
                    runtime->floatEditY += 0.1f;
                    break;
                }
            }
        } else if (D_0037F510.decTen & 2) {
            if (runtime->floatEditMode == 0) {
                if (runtime->floatSelection > 0) {
                    runtime->floatSelection--;
                } else {
                    runtime->floatSelection = 2;
                }
            } else if (runtime->floatEditMode == 1) {
                switch (runtime->floatSelection) {
                case 0:
                    runtime->floatEditX -= 0.1f;
                    break;
                case 1:
                    runtime->floatEditY -= 0.1f;
                    break;
                }
            }
        } else if (D_0037F510.incOne & 2) {
            if (runtime->floatEditMode == 1) {
                if (runtime->floatSelection == 0) {
                    runtime->floatEditX += 0.01f;
                } else if (runtime->floatSelection == 1) {
                    runtime->floatEditY += 0.01f;
                } else if (runtime->floatSelection == 2) {
                    D_00438A34 ^= 1;
                }
            }
        } else if (D_0037F510.decOne & 2) {
            if (runtime->floatEditMode == 1) {
                if (runtime->floatSelection == 0) {
                    runtime->floatEditX -= 0.01f;
                } else if (runtime->floatSelection == 1) {
                    runtime->floatEditY -= 0.01f;
                } else if (runtime->floatSelection == 2) {
                    D_00438A34 ^= 1;
                }
            }
        } else if (D_0037F510.unk2B != 0) {
            runtime->horizontalOffset += 8;
        } else if (D_0037F510.unk29 != 0) {
            runtime->horizontalOffset -= 8;
        }

        switch (group->metadata.extra1) {
        case 0:
            break;
        case 1: {
            f32 currentX = runtime->floatEditX;
            f32 currentY = runtime->floatEditY;

            D_00438A48 = currentX;
            D_00438A4C = currentY;
            func_0023E320(group->setterId, currentX, currentY);
            break;
        }
        case 2:
            func_0023E320(group->setterId, runtime->floatEditX,
                          runtime->floatEditY);
            break;
        case 3:
            func_0023E320(group->setterId, runtime->floatEditX,
                          runtime->floatEditY);
            break;
        }

        if (status == -1) {
            switch (group->metadata.extra1) {
            case 0:
                break;
            case 1: {
                f32 savedX = runtime->savedFloatEditX;
                f32 savedY = runtime->savedFloatEditY;

                D_00438A48 = savedX;
                D_00438A4C = savedY;
                func_0023E320(group->setterId, savedX, savedY);
                break;
            }
            case 2:
                func_0023E320(group->setterId, runtime->savedFloatEditX,
                              runtime->savedFloatEditY);
                break;
            case 3:
                func_0023E320(group->setterId, runtime->savedFloatEditX,
                              runtime->savedFloatEditY);
                break;
            }
            D_00438A34 = runtime->savedOverlayFlag;
        }
    }
    return status;
}

/* Draw command text for row 0 or 1; null texts and other rows emit nothing. */
void evtDrawOptionalPromptText(SdfListHead *list, s32 x, s32 y, s32 kind, EvtRuntime *ctx) {
    switch (kind) {
    case 0:
        if (ctx->commandSecond.text != NULL) {
            sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_004374D0, ctx->commandSecond.text));
            return;
        }
        break;
    case 1:
        if (ctx->commandThird.text != NULL) {
            sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_004374D0, ctx->commandThird.text));
        }
        break;
    }
}

extern void evtDrawOptionalPromptText(SdfListHead *list, s32 x, s32 y, s32 kind, EvtRuntime *ctx);

/* In mode 0x14, count down before checking input: zero expires, negative waits
 * indefinitely, and confirm takes precedence over cancel. */
s32 mnuDrawTimedPrompt(s32 x, s32 y, EvtRuntime *ctx) {
    SdfListHead *list;

    list = sdfCreateResetPacketList();
    evtDrawMenuFrame(list, x, y, 0x19, 2, 0, 1, ctx, NULL, evtDrawOptionalPromptText);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, list);
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

void evtViewerDrawGroupRow(SdfListHead *list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
    EvtRuntimeGroup *group;
    s32 color;
    s32 n;

    color = 4;
    if (ctx->groupFirst + ctx->groupCursor != index) {
        color = 0;
    }
    if (index == 0) {
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_00437768));
        return;
    }
    if (index == 1) {
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, "DEFFAULT"));
        return;
    }
    n = 2;
    for (group = ctx->groups; group != NULL; group = group->next) {
        if (group->type == 0x18) {
            if (n == index) {
                sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_004374D0, ctx->entryName[group->entryHeader]));
                return;
            }
            n++;
        }
    }
}

s32 evtViewerDrawGroupWindow(s32 x, s32 y, EvtRuntime *ctx) {
    EvtRuntimeGroup *group;
    s32 count = 0;
    SdfListHead *packets;
    s32 shown = 15;

    for (group = ctx->groups; group != NULL; group = group->next) {
        if (group->type == 0x18) {
            count++;
        }
    }
    count += 2;
    packets = sdfCreateResetPacketList();
    evtDrawMenuFrame(packets, x, y, 20, 15, ctx->groupFirst, count, (u8 *)ctx, NULL, evtViewerDrawGroupRow);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, packets);
    if (ctx->actionMode != 21) {
        return 0;
    }
    if (count < shown) {
        shown = count;
    }
    return kwlnStepTwoListCursors(0, 1, count, 1, shown, 0, &ctx->groupFirst, 0, &ctx->groupCursor);
}

extern char *D_003C98F8[];

/* Draw one row of the shadow-configuration menu. */
INCLUDE_RODATA(const s32, "game/code_00250010", D_004241C0);

void func_00256AE0(SdfListHead *list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
    EvtRuntimeChild *node;
    s32 color;

    color = ctx->groupCursor + 2 == index ? 4 : 0;
    node = evtEventViewerGetPendingNode(ctx);
    switch (index) {
    case 0:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, "SHADOW CONFIG"));
        return;
    case 2:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, "MODE   :%s", D_003C98F8[ctx->shadowMode]));
        return;
    case 3:
        sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, "ALPHA  :%3d", ctx->shadowAlpha));
        return;
    case 4:
        if (node->parameterBytes[0] == 3) {
            sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, "Y ZAHYO:%.1f", (double)ctx->shadowY));
        }
        break;
    }
}

/* Shadow-configuration menu: draw it, move the row cursor, and step the
 * selected row's mode, alpha or Y offset; confirm returns 1, cancel -1. */
s32 evtViewerEditShadowProperties(s32 x, s32 y, EvtRuntime *ctx) {
    EvtRuntimeChild *node = evtEventViewerGetPendingNode(ctx);
    SdfListHead *list = sdfCreateResetPacketList();
    s32 lastRow = 0;
    s32 lastMode = 0;

    evtDrawMenuFrame(list, x, y, 0x16, 6, 0, 0, (u8 *)ctx, 0, func_00256AE0);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, list);
    if (ctx->actionMode != 0x16) {
        return 0;
    }
    switch (node->parameterBytes[0]) {
    case 3:
        lastRow = 2;
        lastMode = 3;
        break;
    case 4:
        lastRow = 1;
        lastMode = 1;
        break;
    }
    if (D_0037F510.incTen & 2) {
        if (ctx->groupCursor < lastRow) {
            ctx->groupCursor++;
        } else {
            ctx->groupCursor = 0;
        }
    } else if (D_0037F510.decTen & 2) {
        if (ctx->groupCursor == 0) {
            ctx->groupCursor = lastRow;
        } else {
            ctx->groupCursor--;
        }
    } else if (D_0037F510.incOne & 2) {
        switch (ctx->groupCursor) {
        case 0:
            if (ctx->shadowMode < lastMode) {
                ctx->shadowMode++;
            } else {
                ctx->shadowMode = 0;
            }
            break;
        case 1:
            ctx->shadowAlpha++;
            break;
        case 2:
            ctx->shadowY += 0.1f;
            break;
        }
    } else if (D_0037F510.unk2B & 2) {
        switch (ctx->groupCursor) {
        case 0:
            if (ctx->shadowMode < lastMode) {
                ctx->shadowMode++;
            } else {
                ctx->shadowMode = 0;
            }
            break;
        case 1:
            ctx->shadowAlpha += 10;
            break;
        case 2:
            ctx->shadowY += 1.0f;
            break;
        }
    } else if (D_0037F510.decOne & 2) {
        switch (ctx->groupCursor) {
        case 0:
            if (ctx->shadowMode == 0) {
                ctx->shadowMode = lastMode;
            } else {
                ctx->shadowMode--;
            }
            break;
        case 1:
            ctx->shadowAlpha--;
            break;
        case 2:
            ctx->shadowY -= 0.1f;
            break;
        }
    } else if (D_0037F510.unk29 & 2) {
        switch (ctx->groupCursor) {
        case 0:
            if (ctx->shadowMode == 0) {
                ctx->shadowMode = lastMode;
            } else {
                ctx->shadowMode--;
            }
            break;
        case 1:
            ctx->shadowAlpha -= 10;
            break;
        case 2:
            ctx->shadowY -= 1.0f;
            break;
        }
    } else {
        if (D_0037F510.confirm < 0) {
            return 1;
        }
        if (D_0037F510.cancel < 0) {
            return -1;
        }
    }
    return 0;
}

extern s32 effEventAdvanceResourceTemplateSetup();


typedef struct EvtSelectionCache {
    u8 pad00[0x24];
    s32 selectedEntry; /* 0x24 */
} EvtSelectionCache;

extern EvtSelectionCache *D_00436518;

extern s32 evtActiveEntryFlags;

/* Cache nonzero entry selections, publishing first-tick flags only at timer zero.
 * A zero setup status clears runtime control and returns -1. */
s32 evtSynchronizeSelectedEntry(KwlnTask *task) {
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
s32 evtPollRuntimeControlReady(KwlnTask *task) {
    EvtRuntime *runtime = (EvtRuntime *)kwlnTaskGetUserValue(task);
    if (effEventAdvanceSolidRectangleSetup() == 0) {
        runtime->controlState = 0;
        return -1;
    }
    return 0;
}




typedef char EvtViewKey_size_must_be_0x38[(sizeof(EvtRuntimeChild) == 0x38) ? 1 : -1];

extern void fldDrawPackedRgbEditor(void *packetList, s32 x, s32 y,
                                   s32 selected, u32 color,
                                   s32 showNormalized);
extern char D_00437780[];
extern char D_00437788[];
extern char D_00437790[];
extern char D_00437798[];
extern char D_004377A0[];
extern char D_004377A8[];
extern char D_00424210[];
extern char D_00424220[];
extern char D_00424230[];
extern char D_00424240[];

/* Edit the pending key's position, color, blend mode and scale. */
INCLUDE_RODATA(const s32, "game/code_00250010", D_00424210);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00424220);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00424230);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00424240);

s32 func_002570F8(EvtRuntime *runtime) {
    EvtRuntimeChild *key;
    u8 *channel;
    SdfListHead *packetList;
    s32 selectedChannel;
    s32 step;
    s32 next;
    s32 color;
    s32 style;

    packetList = sdfCreateResetPacketList();
    kwlnDrawSpriteCell((void *)packetList, 0x78, 0x138, 0xC, 9);
    key = evtEventViewerGetPendingNode(runtime);

    style = runtime->editField == 0 ? 6 : 0;
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
        0x7780, 0x82C0, 0xFEFFFF, 0, D_00437780));
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
        0x7780, 0x82C0, 0xFEFFFF, style, D_004376F8, key->p0C.sh[0]));

    style = runtime->editField == 1 ? 6 : 0;
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
        0x7780, 0x8320, 0xFEFFFF, 0, D_00437788));
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
        0x7780, 0x8320, 0xFEFFFF, style, D_004376F8, key->p0C.sh[1]));

    selectedChannel = runtime->editField - 2;
    if ((u32)selectedChannel >= 3) {
        selectedChannel = -1;
    }
    color = key->p10.b[0] | (key->p10.b[1] << 8) | (key->p10.b[2] << 16);
    fldDrawPackedRgbEditor(packetList, 0x7780, 0x8380,
                           selectedChannel, color, 0);

    style = runtime->editField == 5 ? 6 : 0;
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
        0x7780, 0x84A0, 0xFEFFFF, 0, D_00437790));
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
        0x7780, 0x84A0, 0xFEFFFF, style, D_004376F8, key->p10.b[3]));

    style = runtime->editField == 6 ? 6 : 0;
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
        0x7780, 0x8500, 0xFEFFFF, 0, D_00437798));
    switch ((s8)key->p08.b[1]) {
    case 0:
        sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
            0x7780, 0x8500, 0xFEFFFF, style, D_00424210));
        break;
    case 1:
        sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
            0x7780, 0x8500, 0xFEFFFF, style, D_00424220));
        break;
    case 2:
        sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
            0x7780, 0x8500, 0xFEFFFF, style, D_00424230));
        break;
    }

    style = runtime->editField == 7 ? 6 : 0;
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
        0x7780, 0x8560, 0xFEFFFF, 0, D_004377A0));
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
        0x7780, 0x8560, 0xFEFFFF, style, D_00424240,
        key->p14.f));

    style = runtime->editField == 8 ? 6 : 0;
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
        0x7780, 0x85C0, 0xFEFFFF, 0, D_004377A8));
    sdfAppendPacket(packetList, (u32)sdfCreateFormattedSifCommand(
        0x7780, 0x85C0, 0xFEFFFF, style, D_00424240,
        key->p18.f));
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface,
                                     packetList);

    step = 0;
    if ((D_0037F510.incTen & 2) != 0) {
        if (runtime->editField >= 8) {
            runtime->editField = 0;
        } else {
            runtime->editField++;
        }
    } else if ((D_0037F510.decTen & 2) != 0) {
        if (runtime->editField <= 0) {
            runtime->editField = 8;
        } else {
            runtime->editField--;
        }
    } else if ((D_0037F510.decOne & 2) != 0) {
        step = -1;
    } else if ((D_0037F510.incOne & 2) != 0) {
        step = 1;
    } else if ((D_0037F510.unk29 & 2) != 0) {
        step = -10;
    } else if ((D_0037F510.unk2B & 2) != 0) {
        step = 10;
    }

    if (step != 0) {
        switch (runtime->editField) {
        case 0:
            key->p0C.sh[0] = (s16)(key->p0C.sh[0] + step);
            if (key->p0C.sh[0] < -500) {
                key->p0C.sh[0] = -500;
            }
            if (key->p0C.sh[0] >= 1001) {
                key->p0C.sh[0] = 1000;
            }
            break;
        case 1:
            key->p0C.sh[1] = (s16)(key->p0C.sh[1] + step);
            if (key->p0C.sh[1] < -500) {
                key->p0C.sh[1] = -500;
            }
            if (key->p0C.sh[1] >= 1001) {
                key->p0C.sh[1] = 1000;
            }
            break;
        case 2:
            channel = &key->p10.b[0];
        editColor:
            next = *channel + step;
            if (next >= 255) {
                *channel = 255;
            } else if (next <= 0) {
                *channel = 0;
            } else {
                *channel = next;
            }
            break;
        case 3:
            channel = &key->p10.b[1];
            goto editColor;
        case 4:
            channel = &key->p10.b[2];
            goto editColor;
        case 5:
            channel = &key->p10.b[3];
            next = *channel + step;
            if (next >= 255) {
                *channel = 255;
            } else if (next <= 0) {
                *channel = 0;
            } else {
                *channel = next;
            }
            break;
        case 6:
            key->p08.sb[1] += step;
            if (key->p08.sb[1] <= step && step < 0) {
                key->p08.sb[1] = 2;
            }
            if (key->p08.sb[1] >= step + 2 && step > 0) {
                key->p08.sb[1] = 0;
            }
            if (key->p08.sb[1] < 0) {
                key->p08.sb[1] = 0;
            }
            if (key->p08.sb[1] >= 3) {
                key->p08.sb[1] = 2;
            }
            break;
        case 7:
            key->p14.f += ((f32)step / 100.0f);
            if (key->p14.f <= ((f32)step / 100.0f) + (-10.0f) && ((f32)step / 100.0f) < 0.0f) {
                key->p14.f = 10.0f;
            }
            if (((f32)step / 100.0f) + 10.0f <= key->p14.f && ((f32)step / 100.0f) > 0.0f) {
                key->p14.f = -10.0f;
            }
            if (key->p14.f < -10.0f) {
                key->p14.f = -10.0f;
            }
            if (10.0f < key->p14.f) {
                key->p14.f = 10.0f;
            }
            break;
        case 8:
            key->p18.f += ((f32)step / 100.0f);
            if (key->p18.f <= ((f32)step / 100.0f) + (-10.0f) && ((f32)step / 100.0f) < 0.0f) {
                key->p18.f = 10.0f;
            }
            if (((f32)step / 100.0f) + 10.0f <= key->p18.f && ((f32)step / 100.0f) > 0.0f) {
                key->p18.f = -10.0f;
            }
            if (key->p18.f < -10.0f) {
                key->p18.f = -10.0f;
            }
            if (10.0f < key->p18.f) {
                key->p18.f = 10.0f;
            }
            break;
        }
    }


    if (D_0037F510.cancel < 0) {
        runtime->frameGroup->unk28 = 0;
        return 0;
    }
    return 1;
}





/* Type-0x19 node payload: the producer allocates and the writer emits 0x40 bytes. */


typedef char EvtCameraColorPayload_size_must_be_0x40[
    (sizeof(EvtCameraColorPayload) == 0x40) ? 1 : -1];

/* Edit the camera color channels and the three speed/alpha parameter banks. */
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

s32 evtEditCameraColorKeyFrame(EvtRuntime *runtime) {
    EvtCameraColorPayload *key;
    s32 packetList;
    s32 selectedChannel;
    s32 color;
    s32 style;
    s32 bank;
    s32 panelX;
    s32 textX;
    s32 textY;
    s32 step;

    packetList = (s32)sdfCreateResetPacketList();
    panelX = 0x138;
    kwlnDrawSpriteCell((void *)packetList, panelX, 0x30, 0xD, 0x10);
    key = evtEventViewerGetPendingNode(runtime)->payload;

    style = runtime->colorSelection == 0 ? 6 : 0;
    sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
        0x7000 + (panelX << 4), 0x7A80, 0xFEFFFF, 0, "SPEED:"));
    sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
        0x7000 + (panelX << 4), 0x7A80, 0xFEFFFF, style, "       %d", key->parameters.x));

    if (runtime->colorSelection > 0 && runtime->editField < 4) {
        selectedChannel = runtime->colorSelection - 1;
    } else {
        selectedChannel = -1;
    }
    color = key->parameters.w[0] | (key->parameters.w[1] << 8) | (key->parameters.w[2] << 16);
    fldDrawPackedRgbEditor((void *)packetList, 0x7000 + (panelX << 4), 0x7AE0,
                           selectedChannel, color, 0);

    style = runtime->colorSelection == 4 ? 6 : 0;
    sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
        0x7000 + (panelX << 4), 0x7C00, 0xFEFFFF, 0, D_00437790));
    sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
        0x7000 + (panelX << 4), 0x7C00, 0xFEFFFF, style, D_004376F8, key->parameters.w[3]));

    style = runtime->colorSelection == 5 ? 6 : 0;
    sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
        0x7000 + (panelX << 4), 0x7C60, 0xFEFFFF, 0, D_00437798));
    switch ((s32)key->parameters.flagWord) {
    case 0:
        sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
            0x7000 + (panelX << 4), 0x7C60, 0xFEFFFF, style, D_00424210));
        break;
    case 1:
        sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
            0x7000 + (panelX << 4), 0x7C60, 0xFEFFFF, style, D_00424220));
        break;
    case 2:
        sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
            0x7000 + (panelX << 4), 0x7C60, 0xFEFFFF, style, D_00424230));
        break;
    }

    bank = 0;
    textX = 0x7000 + (panelX << 4);
    for (; bank < 3; bank++) {
        style = runtime->colorSelection == bank * 2 + 6 ? 6 : 0;
        textY = 0x7900 + ((bank * 36 + 144) << 3);
        sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
            textX, textY, 0xFEFFFF, 0,
            "S%d SPEED:", bank));
        sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
            textX, textY, 0xFEFFFF, style,
            "          %d", key->parameters.y[bank]));

        style = runtime->colorSelection == bank * 2 + 7 ? 6 : 0;
        textY = 0x7900 + ((bank * 36 + 156) << 3);
        sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
            textX, textY, 0xFEFFFF, 0,
            "S0 ALPHA:"));
        sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
            textX, textY, 0xFEFFFF, style,
            "          %d", key->parameters.z[bank]));
    }

    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface,
                                     (SdfListHead *)packetList);
    fldApplyCameraColorKeyWords(runtime, &key->parameters);

    step = 0;
    if ((D_0037F510.incTen & 2) != 0) {
        if (runtime->colorSelection >= 11) {
            runtime->colorSelection = 0;
        } else {
            runtime->colorSelection++;
        }
    } else if ((D_0037F510.decTen & 2) != 0) {
        if (runtime->colorSelection <= 0) {
            runtime->colorSelection = 11;
        } else {
            runtime->colorSelection--;
        }
    } else if ((D_0037F510.decOne & 2) != 0) {
        step = -1;
    } else if ((D_0037F510.incOne & 2) != 0) {
        step = 1;
    } else if ((D_0037F510.unk29 & 2) != 0) {
        step = -10;
    } else if ((D_0037F510.unk2B & 2) != 0) {
        step = 10;
    }

    if (step != 0) {
        switch (runtime->colorSelection) {
        case 0:
            key->parameters.x += step;
            if (key->parameters.x < 0) {
                key->parameters.x = 0;
            }
            if (key->parameters.x >= 1001) {
                key->parameters.x = 1000;
            }
            break;
        case 1:
            key->parameters.w[0] += step;
            if (key->parameters.w[0] < 0) {
                key->parameters.w[0] = 0;
            }
            if (key->parameters.w[0] >= 256) {
                key->parameters.w[0] = 255;
            }
            break;
        case 2:
            key->parameters.w[1] += step;
            if (key->parameters.w[1] <= step && step < 0) {
                key->parameters.w[1] = 255;
            }
            if (key->parameters.w[1] >= step + 255 && step > 0) {
                key->parameters.w[1] = 0;
            }
            if (key->parameters.w[1] < 0) {
                key->parameters.w[1] = 0;
            }
            if (key->parameters.w[1] >= 256) {
                key->parameters.w[1] = 255;
            }
            break;
        case 3:
            key->parameters.w[2] += step;
            if (key->parameters.w[2] <= step && step < 0) {
                key->parameters.w[2] = 255;
            }
            if (key->parameters.w[2] >= step + 255 && step > 0) {
                key->parameters.w[2] = 0;
            }
            if (key->parameters.w[2] < 0) {
                key->parameters.w[2] = 0;
            }
            if (key->parameters.w[2] >= 256) {
                key->parameters.w[2] = 255;
            }
            break;
        case 4:
            key->parameters.w[3] += step;
            if (key->parameters.w[3] <= step && step < 0) {
                key->parameters.w[3] = 255;
            }
            if (key->parameters.w[3] >= step + 255 && step > 0) {
                key->parameters.w[3] = 0;
            }
            if (key->parameters.w[3] < 0) {
                key->parameters.w[3] = 0;
            }
            if (key->parameters.w[3] >= 256) {
                key->parameters.w[3] = 255;
            }
            break;
        case 5:
            key->parameters.flagWord += step;
            if ((s32)key->parameters.flagWord <= step && step < 0) {
                key->parameters.flagWord = 2;
            }
            if ((s32)key->parameters.flagWord >= step + 2 && step > 0) {
                key->parameters.flagWord = 0;
            }
            if ((s32)key->parameters.flagWord < 0) {
                key->parameters.flagWord = 0;
            }
            if ((s32)key->parameters.flagWord >= 3) {
                key->parameters.flagWord = 2;
            }
            break;
        case 6:
            key->parameters.y[0] += step;
            if (key->parameters.y[0] <= step && step < 0) {
                key->parameters.y[0] = 1000;
            }
            if (key->parameters.y[0] >= step + 1000 && step > 0) {
                key->parameters.y[0] = 0;
            }
            if (key->parameters.y[0] < 0) {
                key->parameters.y[0] = 0;
            }
            if (key->parameters.y[0] >= 1001) {
                key->parameters.y[0] = 1000;
            }
            break;
        case 7:
            key->parameters.z[0] += step;
            if (key->parameters.z[0] <= step && step < 0) {
                key->parameters.z[0] = 255;
            }
            if (key->parameters.z[0] >= step + 255 && step > 0) {
                key->parameters.z[0] = 0;
            }
            if (key->parameters.z[0] < 0) {
                key->parameters.z[0] = 0;
            }
            if (key->parameters.z[0] >= 256) {
                key->parameters.z[0] = 255;
            }
            break;
        case 8:
            key->parameters.y[1] += step;
            if (key->parameters.y[1] <= step && step < 0) {
                key->parameters.y[1] = 1000;
            }
            if (key->parameters.y[1] >= step + 1000 && step > 0) {
                key->parameters.y[1] = 0;
            }
            if (key->parameters.y[1] < 0) {
                key->parameters.y[1] = 0;
            }
            if (key->parameters.y[1] >= 1001) {
                key->parameters.y[1] = 1000;
            }
            break;
        case 9:
            key->parameters.z[1] += step;
            if (key->parameters.z[1] <= step && step < 0) {
                key->parameters.z[1] = 255;
            }
            if (key->parameters.z[1] >= step + 255 && step > 0) {
                key->parameters.z[1] = 0;
            }
            if (key->parameters.z[1] < 0) {
                key->parameters.z[1] = 0;
            }
            if (key->parameters.z[1] >= 256) {
                key->parameters.z[1] = 255;
            }
            break;
        case 10:
            key->parameters.y[2] += step;
            if (key->parameters.y[2] <= step && step < 0) {
                key->parameters.y[2] = 1000;
            }
            if (key->parameters.y[2] >= step + 1000 && step > 0) {
                key->parameters.y[2] = 0;
            }
            if (key->parameters.y[2] < 0) {
                key->parameters.y[2] = 0;
            }
            if (key->parameters.y[2] >= 1001) {
                key->parameters.y[2] = 1000;
            }
            break;
        case 11:
            key->parameters.z[2] += step;
            if (key->parameters.z[2] <= step && step < 0) {
                key->parameters.z[2] = 255;
            }
            if (key->parameters.z[2] >= step + 255 && step > 0) {
                key->parameters.z[2] = 0;
            }
            if (key->parameters.z[2] < 0) {
                key->parameters.z[2] = 0;
            }
            if (key->parameters.z[2] >= 256) {
                key->parameters.z[2] = 255;
            }
            break;
        }
    }

    if (D_0037F510.cancel < 0) {
        runtime->colorEditorActive = 0;
        return 0;
    }
    return 1;
}


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
        status = effEventAdvanceScreenDrawSetup();
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
        status = evtEditCameraColorKeyFrame(runtime);
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

s32 evtDispatchActionByIndex(s32 index, s32 x, s32 y, EvtRuntime *runtime) {
    s32 mode = runtime->actionMode;
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
                child->duration = index++;
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

extern s32 evtEventViewerCountEntriesById(s32 id, EvtRuntime *runtime);
extern s32 evtEventViewerCountEntries(EvtRuntime *runtime);
extern s32 evtEventViewerSumNodeCounts(s32 mode, EvtRuntime *runtime);

/* Fixed prefix shared by PMD directory streams. */
typedef struct EvtPmdFilePrefix {
    u8 reserved00;
    u8 reserved01;
    u16 reserved02;
    s32 byteLength;
    char tag[4];
    s32 reserved0C;
    s32 count;
    s32 kind;
    s32 reserved18;
    s32 reserved1C;
} EvtPmdFilePrefix;
typedef char EvtPmdFilePrefix_size_check[sizeof(EvtPmdFilePrefix) == 0x20 ? 1 : -1];

/* Build the directory after assigning each payload its serialized index. */
void evtViewerWriteTrackDirectory(s32 output, s32 mode, EvtRuntime *runtime) {
    EvtPmdFilePrefix header;
    PmdEntry entry;
    s32 directoryCount = 0;
    s32 payloadOffset = 0;
    s32 id0Count;
    s32 id1Count;
    s32 nodeCount;
    s32 nodeBytes;
    s32 nameCount;
    s32 nameBytes;
    s32 id2Count;
    s32 id2Bytes;
    s32 typeACount;
    s32 typeABytes;
    s32 typeBCount;
    s32 typeBBytes;
    s32 typeDCount;
    s32 typeDBytes;
    s32 typeECount;
    s32 typeEBytes;
    s32 typeFCount;
    s32 typeFBytes;
    s32 type16Count;
    s32 type16Bytes;
    s32 type17Count;
    s32 type17Bytes;
    s32 type23Count;
    s32 type23Bytes;
    s32 type27Count;
    s32 type27Bytes;
    s32 groupCount;
    s32 groupBytes;
    s32 type25Count;
    s32 type25Bytes;
    s32 byteLength;
    s32 i;

    if (mode == 1) {
        return;
    }
    id0Count = evtEventViewerCountEntriesById(0, runtime);
    id1Count = evtEventViewerCountEntriesById(1, runtime);
    nodeCount = evtEventViewerSumNodeCounts(mode, runtime);
    nodeBytes = nodeCount * 0x2C;
    nameCount = runtime->entryTotal;
    nameBytes = nameCount * 0x20;
    id2Count = evtEventViewerCountEntriesById(2, runtime);
    id2Bytes = id2Count * 0x10;
    typeACount = evtAssignRuntimeChildSequenceAndCount(runtime);
    typeABytes = typeACount * 0x30;
    typeBCount = evtIndexGroupTypeElevenChildren(runtime);
    typeBBytes = typeBCount * 0x20;
    typeDCount = evtIndexGroupTypeThirteenChildren(runtime);
    typeDBytes = typeDCount * 0x28;
    typeECount = evtIndexGroupTypeFourteenChildren(runtime);
    typeEBytes = typeECount * 0x2C;
    typeFCount = evtIndexGroupTypeFifteenChildren(runtime);
    typeFBytes = typeFCount * 0x2C;
    type16Count = evtIndexGroupTypeSixteenChildren(runtime);
    type16Bytes = type16Count * 0x18;
    type17Count = evtIndexGroupTypeSeventeenChildren(runtime);
    type17Bytes = type17Count * 0x24;
    type23Count = evtIndexGroupTypeTwentyThreeChildren(runtime);
    type23Bytes = type23Count * 0x2C;
    type27Count = evtIndexGroupTypeTwentySevenChildren(runtime);
    type27Bytes = type27Count * 0x28;
    groupCount = evtEventViewerCountEntries(runtime);
    groupBytes = groupCount * 8;
    type25Count = evtIndexGroupTypeTwentyFiveChildren(runtime);
    type25Bytes = type25Count * 0x40;

    byteLength = 0x20;
    switch (mode) {
    case 1:
        break;
    case 2:
        directoryCount = 15;
        payloadOffset = 0x110;
        byteLength = nameBytes + 0x120;
        byteLength += nodeBytes;
        byteLength += id2Bytes;
        byteLength += typeABytes;
        byteLength += typeBBytes;
        byteLength += typeDBytes;
        byteLength += typeEBytes;
        byteLength += typeFBytes;
        byteLength += type16Bytes;
        byteLength += type17Bytes;
        byteLength += type23Bytes;
        byteLength += groupBytes;
        byteLength += type27Bytes;
        byteLength += type25Bytes;
        break;
    case 3:
        directoryCount = 1;
        payloadOffset = 0x30;
        byteLength = nodeBytes + payloadOffset;
        break;
    }
    header.reserved00 = 0;
    header.reserved01 = 0;
    header.reserved02 = 0;
    header.byteLength = byteLength;
    header.tag[0] = 'P';
    header.tag[1] = 'M';
    header.tag[2] = 'D';
    header.tag[3] = mode + '0';
    header.reserved0C = 0;
    header.count = directoryCount;
    header.kind = 9;
    header.reserved18 = 0;
    header.reserved1C = 0;
    func_002588A0(output, (s32)&header, sizeof(header));

    for (i = 0; i < 26; i++) {
        entry.type = i;
        entry.unk_04 = 0;
        entry.value = 0;
        entry.offset = 0;
        switch (i) {
        case 0:
            if (mode == 2) {
                entry.unk_04 = 0x10;
                entry.value = 1;
            }
            break;
        case 2:
            if (mode == 1) {
                entry.unk_04 = 0x10;
                entry.value = id0Count;
            }
            break;
        case 3:
            if (mode == 1) {
                entry.unk_04 = 0x20;
                entry.value = id1Count;
            }
            break;
        case 4:
            if (mode == 2 || mode == 3) {
                entry.unk_04 = 0x2C;
                entry.value = nodeCount;
            }
            break;
        case 1:
            if (mode != 3) {
                entry.unk_04 = 0x20;
                entry.value = nameCount;
            }
            break;
        case 5:
            if (mode == 2) {
                entry.unk_04 = 0x10;
                entry.value = id2Count;
            }
            break;
        case 13:
            if (mode == 2) {
                entry.unk_04 = 0x30;
                entry.value = typeACount;
            }
            break;
        case 14:
            if (mode == 2) {
                entry.unk_04 = 0x20;
                entry.value = typeBCount;
            }
            break;
        case 15:
            if (mode == 2) {
                entry.unk_04 = 0x28;
                entry.value = typeDCount;
            }
            break;
        case 16:
            if (mode == 2) {
                entry.unk_04 = 0x2C;
                entry.value = typeECount;
            }
            break;
        case 17:
            if (mode == 2) {
                entry.unk_04 = 0x2C;
                entry.value = typeFCount;
            }
            break;
        case 20:
            if (mode == 2) {
                entry.unk_04 = 0x2C;
                entry.value = type23Count;
            }
            break;
        case 24:
            if (mode == 2) {
                entry.unk_04 = 0x28;
                entry.value = type27Count;
            }
            break;
        case 18:
            if (mode == 2) {
                entry.unk_04 = 0x18;
                entry.value = type16Count;
            }
            break;
        case 19:
            if (mode == 2) {
                entry.unk_04 = 0x24;
                entry.value = type17Count;
            }
            break;
        case 21:
            if (mode == 2) {
                entry.unk_04 = 8;
                entry.value = groupCount;
            }
            break;
        case 25:
            if (mode == 2) {
                entry.unk_04 = 0x40;
                entry.value = type25Count;
            }
            break;
        }
        if (entry.unk_04 != 0 || entry.value != 0) {
            entry.offset = payloadOffset;
            payloadOffset += (s32)entry.unk_04 * (s32)entry.value;
            func_002588A0(output, (s32)&entry, sizeof(entry));
        }
    }
}


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
void evtViewerWriteChildRecords(s32 output, s32 mode, EvtRuntime *runtime) {
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

            value = (u16)child->serializedValue;
            record.groupType = group->type;
            record.start = child->frame;
            record.span = child->duration;
            record.value = value;
            for (i = 0; i < 8; i++) {
                record.body[i] = child->body.words[i];
            }
            if (group->type == 8) {
                if (child->body.words[0] == 0) {
                    record.span = 0;
                } else {
                    if (child->next != NULL) {
                        record.span = child->next->frame - child->frame;
                    } else {
                        record.span = runtime->frameRange.f.end - child->frame;
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
            header[0] = group->entryHeader;
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

void evtWriteGroupMetadata(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    EvtGroupMetadata record;
    for (group = runtime->groups; group != 0; group = group->next) {
        record.type = *(u8 *)&group->type;
        record.flag = group->metadataFlag;
        record.entry = (u16)group->entryHeader;
        record.value = group->metadata.value;
        record.extra1 = group->metadata.extra1;
        record.extra2 = group->metadata.extra2;
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

extern u8 sdfPfsDebugMode;
extern char *sdfDevGetPathBuffer(void);
extern s32 func_0035C860(char *buffer, const char *format, ...);
extern s32 func_00369B70(const char *path, s32 flags, ...);
extern s32 func_00369DF8(s32 descriptor);
extern s32 func_0036BCD0(const char *device, s32 flags);
extern void evtViewerWriteTrackDirectory(s32 output, s32 format, EvtRuntime *runtime);
extern char D_004377C0[];
extern char D_004377C8[];

/* Save the runtime to its paired PM2/PM3 files. Mode zero uses the
 * viewer name; other modes use the selected event and cut identifiers. */
s32 evtViewerSaveTrackFiles(s32 mode, EvtRuntime *runtime) {
    char pm2Path[64];
    char pm3Path[64];
    s32 pm2;
    s32 pm3;
    s32 section;
    EvtRuntimeGroup *group;

    for (group = runtime->groups; group != NULL; group = group->next) {
    }

    if (mode == 0) {
        if (sdfPfsDebugMode) {
            func_0035C860(pm2Path, "pfs0:/event/pmvtool/%s.PM2", (char *)&runtime->nameStorage[0x14]);
            func_0035C860(pm3Path, "pfs0:/event/pmvtool/%s.PM3", (char *)&runtime->nameStorage[0x14]);
        } else {
            func_0035C860(pm2Path, "%sevent/pmvtool/%s.PM2", sdfDevGetPathBuffer(), (char *)&runtime->nameStorage[0x14]);
            func_0035C860(pm3Path, "%sevent/pmvtool/%s.PM3", sdfDevGetPathBuffer(), (char *)&runtime->nameStorage[0x14]);
        }
    } else if (sdfPfsDebugMode) {
        func_0035C860(pm2Path, "pfs0:/event/e%03d/e%03d/e%03d_%03d/E%03d_%03d.PM2",
            ((u32)D_004372B0 / 10U) * 10, D_004372B0, D_004372B0,
            D_004372B2, D_004372B0, D_004372B2);
        func_0035C860(pm3Path, "pfs0:/event/e%03d/e%03d/e%03d_%03d/E%03d_%03d.PM3",
            ((u32)D_004372B0 / 10U) * 10, D_004372B0, D_004372B0,
            D_004372B2, D_004372B0, D_004372B2);
    } else {
        func_0035C860(pm2Path, "%sevent/e%03d/e%03d/e%03d_%03d/E%03d_%03d.PM2",
            sdfDevGetPathBuffer(), ((u32)D_004372B0 / 10U) * 10,
            D_004372B0, D_004372B0, D_004372B2, D_004372B0, D_004372B2);
        func_0035C860(pm3Path, "%sevent/e%03d/e%03d/e%03d_%03d/E%03d_%03d.PM3",
            sdfDevGetPathBuffer(), ((u32)D_004372B0 / 10U) * 10,
            D_004372B0, D_004372B0, D_004372B2, D_004372B0, D_004372B2);
    }

    if (sdfPfsDebugMode) {
        func_0035B6E0("hdd -> %s\n", pm2Path);
        pm2 = func_00369B70(pm2Path, 0x602, 0666);
        pm3 = func_00369B70(pm3Path, 0x602, 0666);
    } else {
        func_0035B6E0("pc -> %s\n", pm2Path);
        pm2 = func_00369B70(pm2Path, 0x602);
        pm3 = func_00369B70(pm3Path, 0x602);
    }
    if (pm2 < 0 || pm3 < 0) {
        func_0035B6E0(D_004377C0);
        return 0;
    }

    evtViewerWriteTrackDirectory(pm2, 2, runtime);
    for (section = 0; section < 26; section++) {
        switch (section) {
        case 0: evtWriteRuntimeHeaderValues(pm2, runtime); break;
        case 1: evtWriteFixedSizeEntries(pm2, runtime); break;
        case 4: evtViewerWriteChildRecords(pm2, 2, runtime); break;
        case 5: evtWriteGroupHeader(pm2, runtime); break;
        case 13: evtCopyRuntimeChildPayloadsToBuffer(pm2, runtime); break;
        case 14: evtEmitGroupTypeElevenPayloads(pm2, runtime); break;
        case 15: evtEmitGroupTypeThirteenPayloads(pm2, runtime); break;
        case 16: evtEmitGroupTypeFourteenPayloads(pm2, runtime); break;
        case 17: evtEmitGroupTypeFifteenPayloads(pm2, runtime); break;
        case 18: evtEmitGroupTypeSixteenPayloads(pm2, runtime); break;
        case 19: evtEmitGroupTypeSeventeenPayloads(pm2, runtime); break;
        case 20: evtEmitGroupTypeTwentyThreePayloads(pm2, runtime); break;
        case 24: evtEmitGroupTypeTwentySevenPayloads(pm2, runtime); break;
        case 21:
            evtWriteGroupMetadata(pm2, runtime);
            func_0035B6E0("save object table\n");
            break;
        case 25:
            evtEmitGroupTypeTwentyFivePayloads(pm2, runtime);
            func_0035B6E0("save rain data\n");
            break;
        }
    }
    func_00369DF8(pm2);
    func_0036BCD0(D_004377C8, 0);
    evtViewerWriteTrackDirectory(pm3, 3, runtime);
    for (section = 0; section < 26; section++) {
        if (section == 4) {
            evtViewerWriteChildRecords(pm3, 3, runtime);
        }
    }
    func_00369DF8(pm3);
    func_0035B6E0("save pm3 file\n");
    func_0036BCD0(D_004377C8, 0);
    return 1;
}


u16 evtGetRowValue(PolyMovieWork *work, s32 row) {
    if (work->sub->kind == 4) {
        return ((EvtCompactRow *)work->subEntry4Kind4Data)[row].value;
    }
    return ((EvtExtendedRow *)work->subEntry4OtherData)[row].value;
}

s16 evtGetRowVariant(PolyMovieWork *work, s32 row) {
    if (work->sub->kind == 4) {
        return ((EvtCompactRow *)work->subEntry4Kind4Data)[row].variant;
    }
    return ((EvtExtendedRow *)work->subEntry4OtherData)[row].variant;
}

u16 evtGetRowParameter(PolyMovieWork *work, s32 row) {
    if (work->sub->kind == 4) {
        return ((EvtCompactRow *)work->subEntry4Kind4Data)[row].parameter;
    }
    return ((EvtExtendedRow *)work->subEntry4OtherData)[row].parameter;
}

u16 evtGetRowFlags(PolyMovieWork *work, s32 row) {
    if (work->sub->kind == 4) {
        return ((EvtCompactRow *)work->subEntry4Kind4Data)[row].flags;
    }
    return ((EvtExtendedRow *)work->subEntry4OtherData)[row].flags;
}

u8 *evtGetRowPayloadAddress(PolyMovieWork *work, s32 row) {
    if (work->sub->kind == 4) {
        return ((EvtCompactRow *)work->subEntry4Kind4Data)[row].payload;
    }
    return ((EvtExtendedRow *)work->subEntry4OtherData)[row].payload;
}

extern s32 strcmp(const char *a, const char *b);

void evtResolveLinkGroupIndex(PolyMovieWork *src, EvtRuntime *runtime, EvtGroupMetadata *link) {
    s32 i;
    EvtRuntimeGroup *group;

    if (link->extra1 != 3) {
        link->extra2 = 0;
    } else {
        for (i = 0; i < (s32)src->unk_F8; i++) {
            for (group = runtime->groups; group != NULL; group = group->next) {
                if (strcmp(runtime->entryName[group->entryHeader], (char *)src->subEntry1Data + link->extra2 * 32) == 0) {
                    link->extra2 = group->entryHeader;
                    return;
                }
            }
        }
        func_0035B6E0("not found linkslight obj index %s\n", (char *)src->subEntry1Data + link->extra2 * 32);
        link->extra1 = 0;
        link->extra2 = 0;
    }
}

/* Independent reconstruction of the PMD resource-to-viewer initializer.
 * Phase boundaries were previously mapped by Kestrel; no prior complete C
 * implementation was available for this reconstruction. */
/* PMD section payloads are not directory PmdEntry records. */
typedef struct EvtPmdWorldResource {
    s32 nameIndex;
    s32 unknown04;
    s32 area;
    s32 room;
} EvtPmdWorldResource;

typedef struct EvtPmdAuxResource {
    s32 nameIndex;
    s32 dataOffset;
    s32 kind;
    s32 unknown0C;
} EvtPmdAuxResource;

typedef struct EvtPmdModelResource {
    s32 nameIndex;
    s32 unknown04;
    s32 resourceGroup;
    s32 resourceId;
    s32 dataOffset;
    s32 dataSize;
    s32 loadOrder;
    s32 unknown1C;
} EvtPmdModelResource;

typedef struct EvtPmdRuntimeHeader {
    s32 firstFrame;
    s32 frameRange;
    s32 frameCount;
    s32 metadata;
} EvtPmdRuntimeHeader;

extern void *dds3GetWorldSecondaryObject(void);
extern s32 evtCreateWorldObjectFromResource(s32, s32, s32, s32, const SdfTextureOffsetListHeader *, s32);
extern void mdlLoadViewerPackage(s32, s32, s32, s32, s32);
extern s32 mdlSpawnCameraSlotViewerObject(s32, s32);
extern EvtUnit *evtGetWorldUnitNestedValue(s32);
extern s32 evtStageRelinkOwnedNodeResource(void *, void *);
extern EvtRuntimeGroup *evtEventViewerCreateEntry(s32, EvtRuntime *);
extern EvtRuntimeChild *evtCreateViewerTimelineKey(EvtRuntimeGroup *, s32, EvtRuntime *);
extern s32 evtEventViewerAddName(const char *, EvtRuntime *);
extern void dds3SetCameraFieldOfView(EffWorldNode *, f32);
extern s32 mnuCampFindMatchingEntryIndex(PolyMovieWork *, EvtRuntime *, s32);
extern s32 evtPreloadBgm(s32);
extern s32 evtIsBgmLoaded(s32);
extern s32 fldTitleIsActive(void);
extern void fldStartTitle(s32, s32, s32);
extern void kwlnFadeBackgroundStartIn(s32);
extern void kwlnFadeBackgroundStartOut(s32);

s32 func_0025A280(PolyMovieWork *work, EvtRuntime *runtime) {
    char motionName[0x40];
    EvtWorldTable *world;
    s32 needsBgm;
    s32 row;
    s32 index;
    s32 resourceGroup;
    s32 resourceId;
    s32 objectId;
    s32 nameIndex;
    s32 linkIndex;
    s32 count;
    s32 title;
    s32 groupType;
    EffWorldNode *node;
    EffWorldNode *owner;
    EffWorldNode *worldObject;
    ObjectTransform *transform;
    EvtRuntimeGroup *group;
    EvtRuntimeChild *key;
    EvtPmdRuntimeHeader *header;
    EvtCameraColorPayload *color;
    void *payload;

    func_0035B6E0("start SetGameData\n");
    needsBgm = 0;
    if (work->mainEntry2Data != NULL) {
        resourceGroup = ((EvtPmdWorldResource *)work->mainEntry2Data)->area;
        resourceId = ((EvtPmdWorldResource *)work->mainEntry2Data)->room;
        evtCreateWorldObjectFromResource(resourceGroup, resourceId,
            (s32)work->mainEntry10Data, (s32)work->mainEntry11Data,
            (SdfTextureOffsetListHeader *)work->mainEntry12Data, 0);
        group = evtEventViewerCreateEntry(0, runtime);
        group->entryHeader = evtEventViewerAddName((char *)work->mainEntry1Data +
            ((EvtPmdWorldResource *)work->mainEntry2Data)->nameIndex * 32, runtime);
        group->resourceIds.resourceGroup = resourceGroup;
        group->resourceIds.resourceId = resourceId;
    }
    world = ((EffWorldNode *)dds3GetWorldSecondaryObject())->data;
    for (node = world->slots[4].head; node != NULL; node = node->next) {
        if (node->value != 0) {
            group = evtEventViewerCreateEntry(2, runtime);
            group->info = node;
            group->entryHeader = evtEventViewerAddName((char *)node->value, runtime);
        }
    }
    for (index = 2; index >= 0; index--) {
        for (row = 0; row < (s32)work->unk_38; row++) {
            if (((EvtPmdModelResource *)work->mainEntry3Data)[row].loadOrder != index) {
                continue;
            }
            resourceGroup = ((EvtPmdModelResource *)work->mainEntry3Data)[row].resourceGroup;
            resourceId = ((EvtPmdModelResource *)work->mainEntry3Data)[row].resourceId;
            mdlLoadViewerPackage(resourceGroup, resourceId, 0x103,
                (s32)work->data + ((EvtPmdModelResource *)work->mainEntry3Data)[row].dataOffset,
                ((EvtPmdModelResource *)work->mainEntry3Data)[row].dataSize);
            objectId = mdlSpawnCameraSlotViewerObject(resourceGroup, resourceId);
            group = evtEventViewerCreateEntry(1, runtime);
            group->info = dds3FindWorldObjectNodeByKey(dds3GetWorldSecondaryObject(), objectId, 5);
            group->entryHeader = evtEventViewerAddName((char *)work->mainEntry1Data +
                ((EvtPmdModelResource *)work->mainEntry3Data)[row].nameIndex * 32, runtime);
            group->info->value = runtime->entryName[group->entryHeader];
            group->resourceIds.resourceGroup = resourceGroup;
            group->resourceIds.resourceId = resourceId;
            if (evtGetWorldUnitNestedValue(objectId)->owner->first != NULL) {
                mdlAddEntryPlainEx(evtGetWorldUnitNestedValue(objectId)->owner, 0, 0, 0.0f, 0.0f);
            }
            transform = group->info->inner;
            PCP_COPY_VECTOR(group->savedPosition, transform->position);
            PCP_COPY_VECTOR(group->savedRotation, transform->rotation);
        }
    }
    for (row = 0; row < (s32)work->unk_44; row++) {
        switch (((EvtPmdAuxResource *)work->mainEntry7Data)[row].kind) {
        case 0: group = evtEventViewerCreateEntry(3, runtime); break;
        case 1: group = evtEventViewerCreateEntry(0x12, runtime); break;
        case 2: group = evtEventViewerCreateEntry(0x14, runtime); break;
        case 3: group = evtEventViewerCreateEntry(0x15, runtime); break;
        case 4: group = evtEventViewerCreateEntry(0x1A, runtime); break;
        default: continue;
        }
        group->entryHeader = evtEventViewerAddName((char *)work->mainEntry1Data +
            ((EvtPmdAuxResource *)work->mainEntry7Data)[row].nameIndex * 32, runtime);
        group->resourceData = (u8 *)work->data + ((EvtPmdAuxResource *)work->mainEntry7Data)[row].dataOffset;
    }
    for (row = 0; row < (s32)work->unk_54; row++) {
        group = evtEventViewerCreateEntry(0x18, runtime);
        group->entryHeader = evtEventViewerAddName((char *)work->mainEntry1Data +
            ((EvtPmdAuxResource *)work->mainEntry22Data)[row].nameIndex * 32, runtime);
        group->texture = sdfTexAcquireResourceTexture((SdfTextureFileHeader *)
            ((u8 *)work->data + ((EvtPmdAuxResource *)work->mainEntry22Data)[row].dataOffset));
    }
    for (node = world->slots[9].head; node != NULL; node = node->next) {
        if (node->value != 0) {
            group = evtEventViewerCreateEntry(9, runtime);
            group->info = node;
            group->entryHeader = evtEventViewerAddName((char *)node->value, runtime);
        }
    }
    runtime->activeEntryIndex = NULL;
    for (node = world->slots[16].head; node != NULL; node = node->next) {
        if (node->value != 0) {
            for (owner = world->slots[4].head; owner != NULL; owner = owner->next) {
                if (owner->value != 0) {
                    func_0035C860(motionName, "%s_MOTION", (char *)owner->value);
                    if (strcmp((char *)node->value, motionName) == 0) {
                        evtStageRelinkOwnedNodeResource(node, owner);
                    }
                }
            }
            for (owner = world->slots[9].head; owner != NULL; owner = owner->next) {
                if (owner->value != 0) {
                    func_0035C860(motionName, "%s_MOTION", (char *)owner->value);
                    if (strcmp((char *)node->value, motionName) == 0) {
                        evtStageRelinkOwnedNodeResource(node, owner);
                    }
                }
            }
            for (owner = world->slots[7].head; owner != NULL; owner = owner->next) {
                if (owner->value != 0) {
                    func_0035C860(motionName, "%s_MOTION", (char *)owner->value);
                    if (strcmp((char *)node->value, motionName) == 0) {
                        evtStageRelinkOwnedNodeResource(node, owner);
                    }
                }
            }
        }
    }
    func_0035B6E0("end pm1 load\n");
    header = (EvtPmdRuntimeHeader *)work->subEntry0Data;
    if (header != NULL) {
        runtime->headerThird = header->frameCount;
        runtime->headerFirst = header->firstFrame;
        runtime->frameRange.word = header->frameRange;
        runtime->headerMetadata = header->metadata;
        runtime->curFrame = runtime->headerFirst < 0 ? 0 : runtime->headerFirst;
        if (runtime->curFrame > runtime->headerThird - 1) {
            runtime->curFrame = runtime->headerThird - 1;
        }
    }
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 1) {
            for (group = runtime->groups; group != NULL; group = group->next) {
                if (strcmp(runtime->entryName[group->entryHeader],
                    (char *)work->subEntry1Data + evtGetRowVariant(work, row) * 32) == 0) {
                    key = evtCreateViewerTimelineKey(group, evtGetRowParameter(work, row), runtime);
                    key->p08.b[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].b[0];
                    key->p0C.i = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[1].i;
                    key->serializedValue = evtEventViewerAddName((char *)work->subEntry1Data +
                        evtGetRowVariant(work, row) * 32, runtime);
                    if (key->p08.sb[0] == 7) {
                        nameIndex = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[1].sh[0];
                        func_0035B6E0("oldtable index=%d string=%s\n", nameIndex,
                            (char *)work->subEntry1Data + ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[1].sh[0] * 32);
                        if (((EvtViewParam *)evtGetRowPayloadAddress(work, row))[1].sh[0] != -1) {
                            worldObject = dds3GetWorldObject();
                            if (dds3FindObjectChainNodeByName(worldObject, work->subEntry1Data +
                                ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[1].sh[0] * 32) != NULL) {
                                key->p0C.h[0] = evtEventViewerAddName((char *)work->subEntry1Data +
                                    ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[1].sh[0] * 32, runtime);
                            } else {
                                key->p0C.h[0] = -1;
                                func_0035B6E0("name not found \n");
                            }
                        }
                    }
                    key->p10.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[2].h[0];
                    key->p14.f = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[3].f;
                    key->p18.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[4].h[0];
                }
            }
        }
    }
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 2) {
            for (group = runtime->groups; group != NULL; group = group->next) {
                if (strcmp(runtime->entryName[group->entryHeader],
                    (char *)work->subEntry1Data + evtGetRowVariant(work, row) * 32) == 0) {
                    key = evtCreateViewerTimelineKey(group, evtGetRowParameter(work, row), runtime);
                    key->p08.f = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].f;
                    key->p0C.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[1].h[0];
                    key->p0C.h[1] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[1].h[1];
                    key->serializedValue = evtEventViewerAddName((char *)work->subEntry1Data +
                        evtGetRowVariant(work, row) * 32, runtime);
                    dds3SetCameraFieldOfView(group->info, key->p08.f);
                }
            }
        }
    }
    for (row = 0; row < (s32)work->unk_A0; row++) {
        switch (evtGetRowValue(work, row)) {
        case 3: case 0x12: case 0x14: case 0x15: case 0x1A:
            for (group = runtime->groups; group != NULL; group = group->next) {
                if (strcmp(runtime->entryName[group->entryHeader],
                    (char *)work->subEntry1Data + evtGetRowVariant(work, row) * 32) == 0) {
                    key = evtCreateViewerTimelineKey(group, evtGetRowParameter(work, row), runtime);
                    switch (evtGetRowValue(work, row)) {
                    case 3: case 0x1A:
                        key->p08.h[1] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[1];
                        key->p0C.b[1] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[1].b[1];
                        if (((EvtViewParam *)evtGetRowPayloadAddress(work, row))[1].sh[1] < 0) {
                            key->p0C.h[1] = -1;
                        } else {
                            key->p0C.h[1] = evtEventViewerAddName((char *)work->subEntry1Data +
                                ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[1].sh[1] * 32, runtime);
                        }
                        key->p0C.b[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[1].b[0];
                        key->p0C.b[1] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[2].b[0];
                        if (work->sub->kind < 4) {
                            key->duration = 0;
                        } else {
                            key->duration = evtGetRowFlags(work, row);
                        }
                        key->p10.b[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[2].b[0];
                        if (work->sub->kind < 9) {
                            key->duration = 0;
                        }
                        break;
                    case 0x14: case 0x15:
                        key->p08.h[1] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[1];
                        key->duration = evtGetRowFlags(work, row);
                        for (index = 0; index < 4; index++) {
                            if (((EvtViewParam *)evtGetRowPayloadAddress(work, row))[1].sb[index] < 0) {
                                key->p0C.sb[index] = -1;
                            } else {
                                key->p0C.sb[index] = evtEventViewerAddName((char *)work->subEntry1Data +
                                    ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[1].sb[index] * 32, runtime);
                            }
                        }
                        key->p08.b[1] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].b[1];
                        key->p08.b[2] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[2].b[0];
                        key->p10.b[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[2].b[0];
                        if (work->sub->kind < 9) {
                            key->duration = 0;
                        }
                        break;
                    case 0x12:
                        key->p08.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[0];
                        key->p0C.b[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[1].b[0];
                        if (((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].sh[1] < 0) {
                            key->p08.h[1] = -1;
                        } else {
                            key->p08.h[1] = evtEventViewerAddName((char *)work->subEntry1Data +
                                ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].sh[1] * 32, runtime);
                        }
                        break;
                    }
                }
            }
            break;
        }
    }
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 9) {
            for (group = runtime->groups; group != NULL; group = group->next) {
                if (strcmp(runtime->entryName[group->entryHeader],
                    (char *)work->subEntry1Data + evtGetRowVariant(work, row) * 32) == 0) {
                    key = evtCreateViewerTimelineKey(group, evtGetRowParameter(work, row), runtime);
                    key->duration = evtGetRowFlags(work, row);
                    key->p08.h[1] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[1];
                    if (((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].sh[0] < 0) {
                        key->p08.h[0] = -1;
                    } else {
                        key->p08.h[0] = evtEventViewerAddName((char *)work->subEntry1Data +
                            ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].sh[0] * 32, runtime);
                    }
                }
            }
        }
    }
    for (index = 0; index < 10; index++) {
        group = evtEventViewerCreateEntry(0xA, runtime);
        group->setterId = index;
        for (row = 0; row < (s32)work->unk_A0; row++) {
            if (evtGetRowValue(work, row) == 0xA &&
                ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[2].sh[0] == index) {
                key = evtCreateViewerTimelineKey(group, evtGetRowParameter(work, row), runtime);
                key->p08.f = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].f;
                key->p0C.f = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[1].f;
                key->p10.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[2].h[0];
                if (work->sub->kind < 7) {
                    key->duration = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[2].h[1];
                } else {
                    key->duration = evtGetRowFlags(work, row);
                }
                key->serializedValue = evtGetRowVariant(work, row);
                if (work->sub->kind < 7) {
                    payload = key->payload;
                    memcpy(payload, work->subEntry13Data + evtGetRowFlags(work, row) * 0x30, 0x30);
                } else {
                    payload = key->payload;
                    memcpy(payload, work->subEntry13Data +
                        ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[2].sh[1] * 0x30, 0x30);
                }
                key->p14.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[3].h[0];
            }
        }
    }
    group = evtEventViewerCreateEntry(0xB, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0xB) {
            key = evtCreateViewerTimelineKey(group, evtGetRowParameter(work, row), runtime);
            key->p08.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[0];
            payload = key->payload;
            memcpy(payload, work->subEntry14Data + evtGetRowFlags(work, row) * 0x20, 0x20);
        }
    }
    group = evtEventViewerCreateEntry(0xC, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0xC) {
            key = evtCreateViewerTimelineKey(group, evtGetRowParameter(work, row), runtime);
            key->duration = evtGetRowFlags(work, row);
            key->p08.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[0];
            key->p08.h[1] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[1];
            key->p0C.i = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[1].i;
            if (work->sub->kind == 1) {
                key->p08.h[1] = 0;
            }
            if (work->sub->kind == 2) {
                key->p08.sh[1] -= 128;
                if (key->p08.sh[1] < -255) {
                    key->p08.sh[1] = -255;
                }
                if (key->p08.sh[1] > 255) {
                    key->p08.sh[1] = 255;
                }
            }
            key->p14.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[3].h[0];
        }
    }
    group = evtEventViewerCreateEntry(0x16, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0x16) {
            key = evtCreateViewerTimelineKey(group, evtGetRowParameter(work, row), runtime);
            key->p08.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[0];
        }
    }
    group = evtEventViewerCreateEntry(0xD, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0xD) {
            key = evtCreateViewerTimelineKey(group, evtGetRowParameter(work, row), runtime);
            key->p08.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[0];
            payload = key->payload;
            memcpy(payload, work->subEntry15Data +
                ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].sh[1] * 0x28, 0x28);
        }
    }
    group = evtEventViewerCreateEntry(0xE, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0xE) {
            key = evtCreateViewerTimelineKey(group, evtGetRowParameter(work, row), runtime);
            key->p08.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[0];
            payload = key->payload;
            memcpy(payload, work->subEntry16Data +
                ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].sh[1] * 0x2C, 0x2C);
            key->p10.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[2].h[0];
            if (((EvtViewParam *)evtGetRowPayloadAddress(work, row))[2].sh[0] != 0 &&
                ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[2].sh[0] != 1) {
                linkIndex = mnuCampFindMatchingEntryIndex(work, runtime,
                    ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[2].sh[0] - 2);
                if (linkIndex != -1) {
                    key->p10.h[0] = linkIndex + 2;
                } else {
                    key->p10.h[0] = 0;
                }
            }
        }
    }
    group = evtEventViewerCreateEntry(0xF, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0xF) {
            key = evtCreateViewerTimelineKey(group, evtGetRowParameter(work, row), runtime);
            key->p08.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[0];
            payload = key->payload;
            memcpy(payload, work->subEntry17Data +
                ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].sh[1] * 0x2C, 0x2C);
            key->p10.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[2].h[0];
            if (((EvtViewParam *)evtGetRowPayloadAddress(work, row))[2].sh[0] != 0 &&
                ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[2].sh[0] != 1) {
                linkIndex = mnuCampFindMatchingEntryIndex(work, runtime,
                    ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[2].sh[0] - 2);
                if (linkIndex != -1) {
                    key->p10.h[0] = linkIndex + 2;
                } else {
                    key->p10.h[0] = 0;
                }
            }
        }
    }
    group = evtEventViewerCreateEntry(0x17, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0x17) {
            key = evtCreateViewerTimelineKey(group, evtGetRowParameter(work, row), runtime);
            key->p08.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[0];
            payload = key->payload;
            memcpy(payload, work->subEntry20Data +
                ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].sh[1] * 0x2C, 0x2C);
            key->p10.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[2].h[0];
            if (((EvtViewParam *)evtGetRowPayloadAddress(work, row))[2].sh[0] != 0 &&
                ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[2].sh[0] != 1) {
                linkIndex = mnuCampFindMatchingEntryIndex(work, runtime,
                    ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[2].sh[0] - 2);
                if (linkIndex != -1) {
                    key->p10.h[0] = linkIndex + 2;
                } else {
                    key->p10.h[0] = 0;
                }
            }
        }
    }
    group = evtEventViewerCreateEntry(0x1B, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0x1B) {
            key = evtCreateViewerTimelineKey(group, evtGetRowParameter(work, row), runtime);
            key->p08.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[0];
            payload = key->payload;
            memcpy(payload, work->subEntry24Data +
                ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].sh[1] * 0x28, 0x28);
        }
    }
    group = evtEventViewerCreateEntry(0x10, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0x10) {
            key = evtCreateViewerTimelineKey(group, evtGetRowParameter(work, row), runtime);
            key->p08.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[0];
            payload = key->payload;
            memcpy(payload, work->subEntry18Data +
                ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].sh[1] * 0x18, 0x18);
            key->p14.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[3].h[0];
        }
    }
    group = evtEventViewerCreateEntry(0x11, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0x11) {
            key = evtCreateViewerTimelineKey(group, evtGetRowParameter(work, row), runtime);
            key->p08.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[0];
            payload = key->payload;
            memcpy(payload, work->subEntry19Data +
                ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].sh[1] * 0x24, 0x24);
            key->p10.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[2].h[0];
            if (((EvtViewParam *)evtGetRowPayloadAddress(work, row))[2].sh[0] != 0 &&
                ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[2].sh[0] != 1) {
                linkIndex = mnuCampFindMatchingEntryIndex(work, runtime,
                    ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[2].sh[0] - 2);
                if (linkIndex != -1) {
                    key->p10.h[0] = linkIndex + 2;
                } else {
                    key->p10.h[0] = 0;
                }
            }
            key->p14.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[3].h[0];
        }
    }
    if (work->handle != -1 && itfMesGetEntryCount(work->handle) != 0) {
        itfMesSetWindowPageAndRefresh(work->handle, 3, 0);
        group = evtEventViewerCreateEntry(4, runtime);
        for (row = 0; row < (s32)work->unk_A0; row++) {
            if (evtGetRowValue(work, row) == 4) {
                key = evtCreateViewerTimelineKey(group, evtGetRowParameter(work, row), runtime);
                key->duration = evtGetRowFlags(work, row);
                key->p08.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[0];
                key->p08.h[1] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[1];
            }
        }
        itfMesSetWindowHighFlags(work->handle, 0xD00000);
    }
    group = evtEventViewerCreateEntry(5, runtime);
    if (work->sub->kind < 6) {
        count = work->unk_A0;
    } else {
        count = work->unk_9C;
    }
    for (row = 0; row < count; row++) {
        if (work->sub->kind < 6) {
            if (evtGetRowValue(work, row) == 5) {
                needsBgm = 1;
                key = evtCreateViewerTimelineKey(group, evtGetRowParameter(work, row), runtime);
                key->p08.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[0];
                key->p08.h[1] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[1];
            }
        } else if (((EvtExtendedRow *)work->secondEntry4Data)[row].value == 5) {
            needsBgm = 1;
            key = evtCreateViewerTimelineKey(group, ((EvtExtendedRow *)work->secondEntry4Data)[row].parameter, runtime);
            key->p08.h[0] = ((EvtViewParam *)((EvtExtendedRow *)work->secondEntry4Data)[row].payload)[0].h[0];
            key->p08.h[1] = ((EvtViewParam *)((EvtExtendedRow *)work->secondEntry4Data)[row].payload)[0].h[1];
        }
    }
    group = evtEventViewerCreateEntry(0x13, runtime);
    if (work->sub->kind < 6) {
        count = work->unk_A0;
    } else {
        count = work->unk_9C;
    }
    for (row = 0; row < count; row++) {
        if (work->sub->kind < 6) {
            if (evtGetRowValue(work, row) == 0x13) {
                needsBgm = 1;
                key = evtCreateViewerTimelineKey(group, evtGetRowParameter(work, row), runtime);
                key->p08.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[0];
                key->p08.h[1] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[1];
                key->p0C.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[1].h[0];
            }
        } else if (((EvtExtendedRow *)work->secondEntry4Data)[row].value == 0x13) {
            needsBgm = 1;
            key = evtCreateViewerTimelineKey(group, ((EvtExtendedRow *)work->secondEntry4Data)[row].parameter, runtime);
            key->p08.h[0] = ((EvtViewParam *)((EvtExtendedRow *)work->secondEntry4Data)[row].payload)[0].h[0];
            key->p08.h[1] = ((EvtViewParam *)((EvtExtendedRow *)work->secondEntry4Data)[row].payload)[0].h[1];
            key->p0C.h[0] = ((EvtViewParam *)((EvtExtendedRow *)work->secondEntry4Data)[row].payload)[1].h[0];
        }
    }
    group = evtEventViewerCreateEntry(6, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 6) {
            key = evtCreateViewerTimelineKey(group, evtGetRowParameter(work, row), runtime);
            key->duration = evtGetRowFlags(work, row);
            key->p08.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[0];
            key->p0C.b[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[1].b[0];
            key->p0C.b[1] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[1].b[1];
            key->p0C.b[2] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[1].b[2];
            key->p0C.b[3] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[1].b[3];
        }
    }
    group = evtEventViewerCreateEntry(7, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 7) {
            key = evtCreateViewerTimelineKey(group, evtGetRowParameter(work, row), runtime);
            key->duration = evtGetRowFlags(work, row);
            key->p08.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[0];
        }
    }
    group = evtEventViewerCreateEntry(8, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 8) {
            key = evtCreateViewerTimelineKey(group, evtGetRowParameter(work, row), runtime);
            key->p08.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[0];
            key->p08.h[1] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[1];
            key->p0C.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[1].h[0];
        }
    }
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0x18) {
            for (group = runtime->groups; group != NULL; group = group->next) {
                if (strcmp(runtime->entryName[group->entryHeader],
                    (char *)work->subEntry1Data + evtGetRowVariant(work, row) * 32) == 0) {
                    key = evtCreateViewerTimelineKey(group, evtGetRowParameter(work, row), runtime);
                    key->p08.b[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].b[0];
                    key->p08.b[1] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].b[1];
                    key->p0C.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[1].h[0];
                    key->p0C.h[1] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[1].h[1];
                    key->p10.b[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[2].b[0];
                    key->p10.b[1] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[2].b[1];
                    key->p10.b[2] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[2].b[2];
                    key->p10.b[3] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[2].b[3];
                    key->p14.f = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[3].f;
                    key->p18.f = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[4].f;
                    key->p1C.b[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[5].b[0];
                }
            }
        }
    }
    group = evtEventViewerCreateEntry(0x19, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0x19) {
            key = evtCreateViewerTimelineKey(group, evtGetRowParameter(work, row), runtime);
            key->p08.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[0];
            key->p08.h[1] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[1];
            key->p0C.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[1].h[0];
            if (work->unk_100 != 0 && work->subEntry25Data != NULL) {
                memcpy(key->payload, work->subEntry25Data + key->p08.sh[1] * 0x40, 0x40);
            }
            if (work->sub->kind < 8 && key->p08.sh[0] == 0) {
                s32 component;
                color = key->payload;
                color->parameters.w[3] = 0;
                for (component = 2; component >= 0; component--) {
                    color->parameters.z[component] = 0;
                }
                func_0035B6E0("modify rain param\n");
            }
        }
    }
    group = evtEventViewerCreateEntry(0x1C, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0x1C) {
            key = evtCreateViewerTimelineKey(group, evtGetRowParameter(work, row), runtime);
            key->duration = evtGetRowFlags(work, row);
            key->p08.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[0];
            key->p08.h[1] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[1];
        }
    }
    group = evtEventViewerCreateEntry(0x1D, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0x1D) {
            key = evtCreateViewerTimelineKey(group, evtGetRowParameter(work, row), runtime);
            key->duration = evtGetRowFlags(work, row);
            key->p08.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[0];
            key->p0C.b[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[1].b[0];
        }
    }
    group = evtEventViewerCreateEntry(0x1E, runtime);
    title = -1;
    row = 0;
    if ((s32)work->unk_A0 > 0) {
        do {
            if (evtGetRowValue(work, row) == 0x1E) {
                key = evtCreateViewerTimelineKey(group, evtGetRowParameter(work, row), runtime);
                key->p08.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[0];
                key->p08.h[1] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[1];
                title = key->p08.sh[0];
                break;
            }
            row++;
        } while (row < (s32)work->unk_A0);
    }
    group = evtEventViewerCreateEntry(0x20, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0x20) {
            key = evtCreateViewerTimelineKey(group, evtGetRowParameter(work, row), runtime);
            key->duration = evtGetRowFlags(work, row);
            key->p08.h[0] = ((EvtViewParam *)evtGetRowPayloadAddress(work, row))[0].h[0];
        }
    }
    if (work->subEntry21Data != NULL) {
        for (row = 0; row < (s32)work->unk_F8; row++) {
            if (((EvtGroupMetadata *)work->subEntry21Data)[row].entry != -1) {
                for (group = runtime->groups; group != NULL; group = group->next) {
                    if (strcmp(runtime->entryName[group->entryHeader], (char *)work->subEntry1Data +
                        ((EvtGroupMetadata *)work->subEntry21Data)[row].entry * 32) == 0) {
                        group->metadata.type = ((EvtGroupMetadata *)work->subEntry21Data)[row].type;
                        group->metadata.entry = ((EvtGroupMetadata *)work->subEntry21Data)[row].entry;
                        if (group->type == 2) {
                            group->metadata.value = 0;
                        } else {
                            group->metadata.value = ((EvtGroupMetadata *)work->subEntry21Data)[row].value;
                        }
                        group->metadata.extra1 = ((EvtGroupMetadata *)work->subEntry21Data)[row].extra1;
                        group->metadata.extra2 = ((EvtGroupMetadata *)work->subEntry21Data)[row].extra2;
                    }
                }
            } else {
                for (group = runtime->groups; group != NULL; group = group->next) {
                    groupType = group->type;
                    if (groupType == ((EvtGroupMetadata *)work->subEntry21Data)[row].type &&
                        group->setterId == ((EvtGroupMetadata *)work->subEntry21Data)[row].flag) {
                        group->metadata.type = groupType;
                        group->metadata.flag = ((EvtGroupMetadata *)work->subEntry21Data)[row].flag;
                        group->metadata.entry = ((EvtGroupMetadata *)work->subEntry21Data)[row].entry;
                        group->metadata.value = ((EvtGroupMetadata *)work->subEntry21Data)[row].value;
                        group->metadata.extra1 = ((EvtGroupMetadata *)work->subEntry21Data)[row].extra1;
                        group->metadata.extra2 = ((EvtGroupMetadata *)work->subEntry21Data)[row].extra2;
                        evtResolveLinkGroupIndex(work, runtime, &group->metadata);
                        break;
                    }
                }
            }
        }
    }
    for (row = 0; row < 10; row++) {
        datGameState->script.ints[200 + row] = -1;
    }
    if (needsBgm == 1) {
        evtPreloadBgm(((EvtPmdWorldResource *)work->mainEntry2Data)->area);
        while (evtIsBgmLoaded(((EvtPmdWorldResource *)work->mainEntry2Data)->area) == 0) {
        }
        func_0035B6E0("smg file load :eventViewer\n");
    }
    if (title != -1) {
        if (fldTitleIsActive() == 0) {
            fldStartTitle(title, -1, 30);
            func_0035B6E0("timei file load=%d\n", title);
        } else {
            func_0035B6E0("warning!! timei process doing!!\n");
        }
    }
    if (mnuCampGetPrimaryOption(runtime) == 0) {
        kwlnFadeBackgroundStartIn(0);
    } else {
        kwlnFadeBackgroundStartOut(0);
    }
    evtViewerDispatchFlagMode(runtime);
    evtApplyViewerTimelineFrame(runtime->curFrame, runtime);
    evtViewerDispatchFlagMode(runtime);
    func_0035B6E0("SetGameData PM2 Version = %d Stageno = %d\n",
        work->sub->kind, ((EvtPmdWorldResource *)work->mainEntry2Data)->area);
    return 1;
}


extern void mnuReleaseCampSceneRegisteredIds(EvtRuntime *runtime);
extern void mnuStopMovieDrawTask(void);
extern s32 sdfCheckPendingWorkWithInterrupts(void);
extern void kwlnTextureReleaseHeldReference(void);
extern u32 kwlnDrawControlFlags;
extern void evtFormatPolygonMoviePaths(u16 a, u16 b, char *path0, char *path1, char *path2);
extern s32 sdfPathExists(char *path);
extern void evtDestroySecondaryWorldNode(void);

s32 evtReloadEventViewer(s32 mode, EvtRuntime *runtime) {
    char path0[0x80];
    char path1[0x80];
    char path2[0x80];
    PolyMovieWork *work;

    mnuReleaseCampSceneRegisteredIds(runtime);
    if (runtime->timedActive == 1) {
        mnuStopMovieDrawTask();
        runtime->timedActive = 0;
    }
    while (sdfCheckPendingWorkWithInterrupts() != 0) {
    }
    if (runtime->pendingResource != 0) {
        sdfQueueGeneralAllocationRelease((struct SdfMemBlock *)runtime->pendingResource);
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
    work = func_0024FB48(D_004372B0, D_004372B2, mode);
    if (work != NULL) {
        runtime->windowContext = work;
        func_0025A280(work, runtime);
        return 1;
    }
    return 0;
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

KwlnTask *evtFindTaskById(u32 taskId) {
    u8 taskName[32];

    evtFormatTaskName(taskId, taskName);
    return kwlnTaskGetTaskByName((const char *)taskName);
}

/* Return this event task's loaded state, or -1 when it does not exist. */
s32 evtGetEventPackLoadedState(u32 taskId) {
    KwlnTask *task = evtFindTaskById(taskId);
    if (task == 0) {
        return -1;
    }
    return ((EvtPackLoadState *)kwlnTaskGetUserValue(task))->loaded;
}

EvtPackLoadState *evtGetEventPackLoadState(u32 taskId) {
    KwlnTask *task = evtFindTaskById(taskId);
    if (task != 0) {
        return (EvtPackLoadState *)kwlnTaskGetUserValue(task);
    }
    return NULL;
}


void *evtFindTaskResourceEntryByKey(u32 id, s32 key) {
    KwlnTask *task;
    EvtPackLoadState *data;
    s32 i;

    task = evtFindTaskById(id);
    if (task == 0) {
        return 0;
    }
    data = (EvtPackLoadState *)kwlnTaskGetUserValue(task);
    if (data->loaded != 2) {
        return 0;
    }
    for (i = 0; i < data->header->entryCount; i++) {
        if (data->entries[i].secondaryResourceId == key) {
            return data->data + data->entries[i].dataOffset;
        }
    }
    return 0;
}

extern void effSetCh72Id(SdfTex *);
extern void sdfTexReleaseReferenceViaHandler(SdfTex *);

void evtRefreshTaskEffectTexture(s32 taskId, s32 key) {
    EvtPackLoadState *data = evtGetEventPackLoadState(taskId);
    void *address = evtFindTaskResourceEntryByKey(taskId, key);
    SdfTex *texture;
    if (address != 0) {
        if (data->effect72 != 0) {
            sdfTexReleaseReferenceViaHandler(data->effect72);
            data->effect72 = 0;
        }
        texture = sdfTexAcquireResourceTexture((SdfTextureFileHeader *)(address));
        effSetCh72Id(texture);
        data->effect72 = texture;
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
    found = evtFindTaskResourceEntryByKey(taskId, key);
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

INCLUDE_SDATA(const s32, "game/code_00250010", D_004377B8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004377C0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004377C8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004377D0);

