#include "common.h"
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

extern s32 evtIsMenuTableEntryEnabled(s32 *);
extern s32 func_00237428();

extern void func_0023D5B0(s32 output, void *data, s32 size);

extern s32 effEventAdvanceSolidRectangleSetup(void);
extern s32 sndFindPackedTrackLoadStatus(s32 sequence);
extern void sndEnsureMidiBankResident(s32 sequence);
extern s32 evtUpdateFrameVariableTask(KwlnTask *task);
extern s32 func_003014F0(char *, char *, ...);
extern void evtFormatTaskName(s32 taskId, void *buffer);
extern KwlnTask *kwlnTaskCreate(const char *, u32, s32, s32, TaskUpdate, TaskDestroy, u32);
extern void *memset(void *, s32, u32);
extern void effObjSetFlags(void *object, s32 flags);
extern void *func_00115298(void *obj, void *vecA, void *vecB);
extern void fldSetSwayMode(s32 mode);
extern void fldSetSkyDrawState(s32 value);
extern void func_00132B80(s32 value);
extern s16 func_00132B90(void);
extern void fldSetFadeTarget(s32 area, s32 value, s32 duration);
extern void func_00132010(void);
extern void fldSelectDisplayBuffer(s32 id);
extern void func_0012AEB0(void);
extern void sndStartTrackDefault(s32 track);
extern void sndStartTrackExtended(s32 track);
extern void func_002E9758(s32 sequence);
extern void func_003003F0(char *fmt, ...);
extern void func_002E96D8(u32 sequence);
extern void func_002E8DD0(u32 sequence);
extern void sndSetSequenceVolumePan(s32 sequence, s32 volume, s32 pan);
extern void evtViewerDispatchFlagMode();
extern void func_0022E5A0();
extern char evtSkyTaskName[];
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




/* Native viewer child node; frame rows traverse this same linked record. */


/* World-slot node borrowed by a viewer group. Its data is slot-specific;
 * model groups use EvtModelSlot, while all named nodes share the list links. */

/* The native 0x84-byte viewer entry owns its child list and borrows info.
 * EvtRuntime.frameGroup selects one of these entries, not a separate list. */


/* The command API stores words; timed-prompt drawing consumes their text pointers. */


/* The file header emits this whole word; type-8 spans use its low halfword. */

/* Native 0x2490-byte viewer runtime shared by dialogs, task polls and file writers. */


extern void fldApplyCameraColorKeyWords(EvtRuntime *runtime, const EvtBlendKey *source);

typedef struct {
    s16 enabled;
    s8 fields[8]; /* Per-row signed lookup values at +0x2. */
} EvtTblEntry; /* 0xA bytes */
extern EvtTblEntry D_00368950[];

extern u16 evtSkyTransitionActive;
extern u16 D_003BBE78;
extern u16 D_003BBE7A;

extern u32 evtSkyOverlayEnabled;

extern void kwlnDrawSpriteCell(u32 list, s32 x, s32 y, s32 w, s32 h);

extern char D_003BC0C8[]; /* "NAME:" */

/* Create a task with an initialized event payload. */
KwlnTask *evtCreateTask(s32 taskId, const char *path) {
    EvtPictureWork *data;

    data = evtAllocateContext();
    evtSetConvertedContextValue(data, path);
    return kwlnTaskCreate(evtPictureTaskName, taskId, 1, 1, evtUpdatePictureWhenFlagged, evtPictureReleaseTaskTextureAndState, (u32)data);
}


KwlnTask *evtCreateTaskWithValue(s32 taskId, SdfTex *texture) {
    EvtPictureWork *data;

    data = evtAllocateContext();
    data->texture = texture;
    return kwlnTaskCreate(evtPictureTaskName, taskId, 1, 1, evtUpdatePictureWhenFlagged, evtPictureReleaseTaskTextureAndState, (u32)data);
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

s32 evtUpdateSkyTask(KwlnTask *task) {
    evtAdvanceSkyTransition();
    func_00132010();
    if (evtSkyOverlayEnabled != 0) {
        fldSelectDisplayBuffer(0x53);
        func_0012AEB0();
    }
    return 0;
}

void evtResetSkyTaskFlags(KwlnTask *task) {
    evtSkyTransitionActive = 0;
    evtSkyOverlayEnabled = 0;
}

void evtDestroySkyTask(void) {
    KwlnTask *task;

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

/* Frame-variable task update: polls the parent task's user value each frame. */
s32 evtUpdateFrameVariableTask(KwlnTask *task) {
    u8 unused[16]; /* retail frame 0x20: unused local storage */

    kwlnTaskGetUserValue(task->parent);
    return 0;
}

void evtCreateFrameVariableTask(void) {
    kwlnTaskCreate("FrameVar", 0x2AF9, 1, 1, evtUpdateFrameVariableTask, 0, 0);
}

typedef s32 (*EvtMenuHeaderFn)();
typedef void (*EvtMenuRowFn)();
extern u32 D_003BC040;
extern char D_003BC048[];
extern char D_003BC050[];

/* Draw a framed debug menu: the optional header returns how many rows it used, the row callback fills the rest,
 * and blinking scroll markers appear above/below when entries precede `first` or follow the last drawn one. */
void evtDrawMenuFrame(u32 list, s32 x, s32 y, s32 width, s32 rows, s32 first, s32 total, u8 *data,
                   EvtMenuHeaderFn header, EvtMenuRowFn row) {
    s32 i = 0;
    s32 textX;
    s32 textY;
    s32 index;

    kwlnDrawSpriteCell((u32)list, x, y, width, rows);
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
    if (D_003BC040 & 0x10) {
        textY = ((y - 10) << 3) + 0x7900;
        textX = ((x + 5 * width + 6) << 4) + 0x7000;
        if (first > 0) {
            sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(textX, textY, 0xFEFFFF, 6, D_003BC048));
        }
        textY = ((y + 12 * rows - 2) << 3) + 0x7900;
        if (index < total) {
            sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(textX, textY, 0xFEFFFF, 6, D_003BC050));
        }
    }
    D_003BC040++;
}

INCLUDE_RODATA(const s32, "game/code_00235270", D_003ADDE0);

s32 evtAppendValueChangeDebugLabel(s32 list, s32 x, s32 y) {
    sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, "VALUE CHANGE."));
    return 2;
}

extern char D_003ADE00[]; /* "     %.1f" */
extern char D_003ADE10[]; /* " L,R = VALUE-+0.1" */
extern char D_003ADE28[]; /* " U,D = VALUE-+1.0" */
extern char D_003ADE40[];
extern char D_003ADE50[];

INCLUDE_RODATA(const s32, "game/code_00235270", D_003ADE00);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003ADE10);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003ADE28);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003ADE40);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003ADE50);

void func_002357B8(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
    switch (index) {
    case 0:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 4, D_003ADE00,
                                                                 ctx->floatValue));
        return;
    case 2:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003ADE10));
        return;
    case 3:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003ADE28));
        return;
    case 4:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003ADE40));
        return;
    case 5:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003ADE50));
        break;
    }
}

extern SdfPoolNode kwlnPositionedTextSurface;
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
    s8 unk29; /* 0x29: coarse decrease in the property editor */
    u8 incHun;  /* 0x2A */
    s8 unk2B; /* 0x2B: coarse increase in the property editor */
    s8 apply;   /* 0x2C */
    s8 unk2D;
    u8 pad2E[0x12]; /* Complete two-bank, two-port, 16-input backing. */
} EvtPad;

extern EvtPad D_00324510;

extern void func_002357B8();

/* Edit the bounded float only in mode 8. Confirm precedes cancel; coarse steps
 * replace fine steps before the value is clamped to the runtime limits. */
s32 evtViewerFloatValueUpdate(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;
    f32 step;

    list = (s32)sdfCreateResetPacketList();
    evtDrawMenuFrame(list, x, y, 0x16, 9, 0, 1, (u8 *)ctx, evtAppendValueChangeDebugLabel, func_002357B8);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, (SdfListHead *)list);
    if (ctx->actionMode != 8) {
        return 0;
    }
    if (D_00324510.confirm < 0) {
        return 1;
    }
    if (D_00324510.cancel < 0) {
        return -1;
    }
    step = 0.0f;
    if (D_00324510.decOne & 2) {
        step = -0.1f;
    } else if (D_00324510.incOne & 2) {
        step = 0.1f;
    }
    if (D_00324510.decTen & 2) {
        step = -1.0f;
    } else if (D_00324510.incTen & 2) {
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
    sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, "VALUE CHANGE."));
    return 2;
}

extern char D_003ADE40[];
extern char D_003ADE50[];
extern char D_003ADE78[];

INCLUDE_RODATA(const s32, "game/code_00235270", D_003ADE78);

void evtDrawValueChangeInstructionRow(s32 list, s32 x, s32 y, u32 index, EvtRuntime *ctx) {
    switch (index) {
    case 0:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 4, D_003BC058, ctx->value));
        return;
    case 2:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003ADE78));
        return;
    case 3:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, " U,D = VALUE-+10"));
        return;
    case 4:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003ADE40));
        return;
    case 5:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003ADE50));
        break;
    }
}

s32 evtUpdateValueChangeDialog(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;
    s32 step;

    list = (s32)sdfCreateResetPacketList();
    evtDrawMenuFrame(list, x, y, 0x16, 9, 0, 1, (u8 *)ctx, evtDrawValueChangeNoticeRow, evtDrawValueChangeInstructionRow);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, (SdfListHead *)list);
    if (ctx->actionMode != 7) {
        return 0;
    }
    if (D_00324510.confirm < 0) {
        return 1;
    }
    if (D_00324510.cancel < 0) {
        return -1;
    }
    if (D_00324510.decOne & 2) {
        step = -1;
    } else if (D_00324510.incOne & 2) {
        step = 1;
    } else {
        step = 0;
    }
    if (D_00324510.decTen & 2) {
        step = -10;
    } else if (D_00324510.incTen & 2) {
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
    sdfAppendPacket((SdfListHead *)target, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, "FRAME CHANGE."));
    return 2;
}

void evtViewerDrawFrameChangeRow(s32 list, s32 x, s32 y, u32 index, EvtRuntime *ctx) {
    switch (index) {
    case 0:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 4, D_003BC058, ctx->value));
        return;
    case 2:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, " L,R = FRMAE-+"));
        return;
    case 3:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, " U,D = FRAME-+10"));
        return;
    case 4:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, "L1,R1= FRAME-+100"));
        return;
    case 5:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003ADE40));
        return;
    case 6:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003ADE50));
        return;
    case 7:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, " RL  = NOW FRAME"));
        return;
    case 8:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, " ST  = CAMERA FOCUS"));
        break;
    case 9:
        break;
    }
}

s32 evtViewerFrameChangeUpdate(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;
    s32 step;

    list = (s32)sdfCreateResetPacketList();
    evtDrawMenuFrame(list, x, y, 0x16, 0xB, 0, 1, (u8 *)ctx, mnuDrawFrameChangeLabel, evtViewerDrawFrameChangeRow);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, (SdfListHead *)list);
    if (ctx->actionMode != 6) {
        return 0;
    }
    if (D_00324510.confirm < 0) {
        return 1;
    }
    if (D_00324510.cancel < 0) {
        return -1;
    }
    if (D_00324510.decOne & 2) {
        step = -1;
    } else if (D_00324510.incOne & 2) {
        step = 1;
    } else {
        step = 0;
    }
    if (D_00324510.decTen & 2) {
        step = -10;
    } else if (D_00324510.incTen & 2) {
        step = 10;
    }
    if (D_00324510.decHun & 2) {
        step = -100;
    } else if (D_00324510.incHun & 2) {
        step = 100;
    }
    if (D_00324510.syncKey < 0) {
        step = ctx->curFrame - ctx->value;
    }
    ctx->value += step;
    if (ctx->value < ctx->valueMin) {
        ctx->value = ctx->valueMin;
    }
    if (ctx->value >= ctx->valueMax) {
        ctx->value = ctx->valueMax;
    }
    if (D_00324510.apply != 0) {
        if (ctx->curFrame != ctx->value) {
            ctx->curFrame = ctx->value;
            evtViewerDispatchFlagMode(ctx);
            func_0022E5A0(ctx->curFrame, ctx);
        }
    }
    return 0;
}

extern const char *D_003BC070[];
extern const char *D_003AE0D8[];
extern char D_003BC080[];
extern char D_003BC088[];
/* The camp provider owns the scene type (EvtRuntime); the viewer passes its runtime. */
extern u32 mnuCampGetPrimaryOption(void *scene);
extern u32 mnuCampGetSecondaryOption(void *scene);

/* Draw a project-menu command, including its current toggle/skip option. */
void evtDrawProjectCommandRow(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
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

    memcpy(primaryOptions, D_003BC070, sizeof(primaryOptions));
    memcpy(secondaryOptions, D_003AE0D8, sizeof(secondaryOptions));
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
            sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style,
                D_003BC080, labels[index], primaryOptions[mnuCampGetPrimaryOption(ctx)]));
        } else if (index == 10) {
            sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style,
                D_003BC080, labels[index], secondaryOptions[mnuCampGetSecondaryOption(ctx)]));
        } else {
            sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style,
                D_003BC088, labels[index]));
        }
    }
}

extern void func_00237130();
extern s32 kwlnStepTwoListCursors(s32, s32, s32, s32, s32, s32 *, s32 *, s32 *, s32 *);

s32 mnuDrawInfoWindowA(s32 x, s32 y, EvtRuntime *work) {
    u32 packets = (u32)sdfCreateResetPacketList();
    evtDrawMenuFrame(packets, x, y, 0xF, 0xB, 0, 0xB, (u8 *)work, 0, evtDrawProjectCommandRow);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, (SdfListHead *)packets);
    if (work->actionMode != 1) {
        return 0;
    }
    return kwlnStepTwoListCursors(0, 1, 0xB, 1, 0xB, 0, 0, 0, &work->inputA);
}

extern char D_003BC090[];
extern char D_003BC0A0[];
extern s8 D_003688B8[][12];


/* Draw the runtime title when present; return two rows used, or zero for no title. */
s32 evtDrawStringEntry(s32 output, s32 x, s32 y, EvtRuntime *work) {
    if (work->title == 0) {
        return 0;
    }
    sdfAppendPacket((SdfListHead *)output, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003BC088, work->title));
    return 2;
}

/* Draw an in-range item name, distinguishing selected active and inactive rows. */
void evtDrawSelectableTextRow(s32 list, s32 x, s32 y, s32 index, EvtRuntime *work) {
    s32 color;

    if (index < work->itemCount) {
        if (work->cursor == index) {
            color = work->actionMode == 2 ? 4 : 5;
        } else {
            color = 0;
        }
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_003BC090, work->itemNames[index]));
    }
}

/* Draw a title-sized selection dialog; only action mode 2 advances its cursor. */
s32 evtUpdateTextSelectionDialog(s32 x, s32 y, EvtRuntime *work) {
    u32 packets = (u32)sdfCreateResetPacketList();
    s32 width = 10;

    if (work->title != 0) {
        width = strlen(work->title);
        if (width < 6) {
            width = 6;
        }
    }
    evtDrawMenuFrame(packets, x, y, width, work->itemCount + 3, 0, work->itemCount, (u8 *)work, evtDrawStringEntry, evtDrawSelectableTextRow);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, (SdfListHead *)packets);
    if (work->actionMode != 2) {
        return 0;
    }
    return kwlnStepTwoListCursors(0, 1, work->itemCount, 1, work->itemCount, 0, 0, 0, &work->cursor);
}

s32 evtDrawInputValueRow(s32 list, s32 x, s32 y, u8 *ctx) {
    char text[16];
    func_003014F0(text, D_003BC098, (s32)ctx + 0x22D4, (s32)ctx + 0x22E0);
    sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003BC088, text));
    return 2;
}

/* Draw eleven characters from a twelve-byte keyboard row and highlight the
 * runtime character selection, using mode 3 for the active color. */
void evtDrawKeyboardRow(s32 list, s32 xPosition, s32 y, s32 row, EvtRuntime *work) {
    s32 x = xPosition + 0xC0;
    s8 *table = D_003688B8[row];
    s32 i = 0;
    s32 color;
    s8 ch;
    s32 drawX;

    do {
        color = 0;
        if (work->charRow == row && work->charCol == i) {
            color = work->actionMode == 3 ? 4 : 5;
        }
        ch = *table++;
        drawX = x;
        x += 0xC0;
        i++;
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(drawX, y, 0xFEFFFF, color, D_003BC0A0, ch));
    } while (i < 0xB);
}

/* Draw the keyboard and edit the eight-character name when input is active. */
s32 func_00236828(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;
    s32 input;
    s32 length;
    s32 key;

    list = (s32)sdfCreateResetPacketList();
    evtDrawMenuFrame(list, x, y, 14, 6, 0, 4, (u8 *)ctx,
                     evtDrawInputValueRow, evtDrawKeyboardRow);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface,
                                    (SdfListHead *)list);
    if (ctx->actionMode != 3) {
        return 0;
    }
    input = kwlnStepTwoListCursors(0, 11, 4, 11, 4, NULL, NULL,
                                  &ctx->charCol, &ctx->charRow);
    if (input < 0) {
        return -1;
    }
    if (input == 1) {
        key = D_003688B8[ctx->charRow][ctx->charCol];
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

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE0B8);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE0C8);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE0D8);

s32 evtDrawEventFileNameRow(s32 list, s32 x, s32 y) {
    char text[32];
    func_003014F0(text, "[E%3d_%03d.PM1+2+3]", D_003BBE78, D_003BBE7A);
    sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003BC088, text));
    return 2;
}

void evtDrawEventCutSelectRow(s32 list, s32 x, s32 y, u32 index, EvtRuntime *ctx) {
    s32 color = 0;

    if (ctx->charRow == index) {
        color = 4;
    }
    switch (index) {
    case 0:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0xE, D_003BC0A8));
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x + 0x600, y, 0xFEFFFF, color, D_003BC0B0, D_003BBE78));
        return;
    case 1:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0xE, D_003BC0B8));
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x + 0x600, y, 0xFEFFFF, color, D_003BC0C0, D_003BBE7A));
        return;
    case 3:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, " L,R = NO-+"));
        return;
    case 4:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, " U,D = SELECT"));
        return;
    case 5:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_003ADE40));
        return;
    case 6:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_003ADE50));
        return;
    case 7:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, " RL  = SET 600"));
        break;
    }
}

s32 evtUpdateEventCutSelectDialog(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;
    u32 num;

    list = (s32)sdfCreateResetPacketList();
    evtDrawMenuFrame(list, x, y, 0x12, 0xB, 0, 8, (u8 *)ctx, evtDrawEventFileNameRow, evtDrawEventCutSelectRow);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, (SdfListHead *)list);
    if (ctx->actionMode != 9) {
        return 0;
    }
    if (D_00324510.decTen < 0) {
        ctx->charRow ^= 1;
    }
    if (D_00324510.incTen < 0) {
        ctx->charRow ^= 1;
    }
    if (D_00324510.decOne & 2) {
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
    if (D_00324510.incOne & 2) {
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
    if (D_00324510.decHun & 2) {
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
    if (D_00324510.incHun & 2) {
        switch (ctx->charRow) {
        case 0:
            D_003BBE78 += 100;
            break;
        case 1:
            D_003BBE7A += 10;
            break;
        }
    }
    if (D_00324510.confirm < 0) {
        return 1;
    }
    if (D_00324510.cancel < 0) {
        return -1;
    }
    if (D_00324510.syncKey < 0) {
        D_003BBE78 = 0x258;
    }
    return 0;
}

void evtDrawSelectedEntryLabel(s32 list, EvtRuntimeGroup *selected, s32 x,
                               s32 unused, EvtRuntime *runtime) {
    x += 0x6C0;
    kwlnDrawSpriteCell((u32)list, (x - 0x7000) >> 4, 0x3C, 0x1C, 3);
    sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, 0x7AE0, 0xFEFFFF, 0xE, D_003BC0C8));
    if (selected->entryHeader >= 0) {
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(
            x + 0x3C0, 0x7AE0, 0xFEFFFF, 0, D_003BC088,
            runtime->entryName[selected->entryHeader]));
    }
}

void func_00237130(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
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
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC088, names[group->type]));
    }
}

s32 evtUpdateEntrySelectionDialog(s32 x, s32 y, EvtRuntime *work) {
    u32 packets = (u32)sdfCreateResetPacketList();
    s32 count;
    s32 shown;

    evtDrawMenuFrame(packets, x, y, 8, 0x1D, work->entryFirst, work->entryCount, (u8 *)work, 0, func_00237130);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, (SdfListHead *)packets);
    if (work->actionMode != 4) {
        return 0;
    }
    count = work->entryCount;
    if (count == 0) {
        return 0;
    }
    shown = 0x1D;
    if (count < 0x1D) {
        shown = count;
    }
    return kwlnStepTwoListCursors(0, 1, count, 1, shown, 0, &work->entryFirst, 0, &work->entryCursor);
}

extern const char *D_003686F8[];
extern s8 D_00368750[];
extern char D_003BC0D0[];

/* Header row of the frame list: one text cell per column of the selected group's table row. */
s32 func_00237428(SdfListHead *list, s32 x, s32 y, u8 *data) {
    EvtRuntime *ctx = (EvtRuntime *)data;
    s32 type = ctx->frameGroup->type;
    s32 i;
    s32 kind;

    for (i = 0; i < D_00368768[type].columns; i++) {
        kind = D_00368768[type].columnTypes[i];
        if (kind == 0x15) {
            ctx->frameTextFormat = D_003BC0D0;
            ctx->frameTextWidth = 0;
            sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0xE, D_003BC088, D_003BC0D0));
            x += ctx->frameTextWidth * 0xC0;
        } else {
            sdfAppendPacket(list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0xE, D_003BC088, D_003686F8[kind]));
            x += D_00368750[D_00368768[type].columnTypes[i]] * 0xC0;
        }
    }
    return 1;
}

extern char D_003BC160[]; /* "%4d" */
extern char D_003BC168[]; /* " ---" */
extern char D_003BC170[]; /* "..." */
extern char D_003BC178[]; /* "  x" */
extern char D_003BC180[]; /* "ON " */
extern char D_003BC188[]; /* "OFF" */
extern char D_003BC190[]; /* "NORMAL" */
extern char D_003BC198[]; /* "ADD" */
extern char D_003BC1A0[]; /* "SUB" */
extern char D_003BC1A8[]; /* " -- " */
extern char D_003BC1B0[]; /* "1STOP" */
extern char D_003BC1B8[]; /* "DOWN" */
extern char D_003BC1C0[]; /* "UP" */
extern char D_003BC1C8[]; /* " IN " */
extern char D_003BC1D0[]; /* " OUT " */
extern char D_003BC1D8[]; /* "%.1f" */
extern char D_003BC1E0[]; /* "%d" */
extern char D_003BC1E8[]; /* "%-.11s" */
extern char D_003BC1F0[]; /* "N" */
extern char D_003BC1F8[]; /* "B%d" */
extern char D_003BC200[]; /* " X" */
extern char D_003BC208[]; /* " -" */
extern char D_003BC210[]; /* "LEV %d" */
extern char D_003BC218[]; /* "DEF " */
extern char D_003BC220[]; /* "%3d " */
extern char D_003BC228[]; /* " x" */
extern char D_003BC230[]; /* "CUR" */
extern char D_003BC238[]; /* "DEF" */
extern s8 D_00368750[];
extern char *D_003688E8[];
extern char *D_003688F8[];
extern char *D_00368918[];
extern char *D_00368938[];
extern char *D_00368940[];
extern char *D_00368948[];

/* Format each enabled column of a timeline key. Some columns reuse the
 * preceding scalar selection; the selected group kind is fixed for this row. */
INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE530);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE540);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE550);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE560);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE570);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE580);

void func_002375D8(s32 list, s32 x, s32 y, s32 color,
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
    for (column = 0; column < D_00368768[type].columns; column++) {
        style = color;
        if (ctx->frameColumn != column) {
            style = 0;
        }
        switch (D_00368768[type].columnTypes[column]) {
        case 0:
            sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC160, node->frame + ctx->frameGroup->metadata.value));
            break;
        case 9:
            switch (type) {
            case 18: value = node->p08.sh[0]; break;
            case 3: value = node->p08.sh[1]; break;
            case 20: value = node->p08.sh[1]; break;
            case 21: value = node->p08.sh[1]; break;
            case 26: value = node->p08.sh[1]; break;
            }
            sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC160, value));
            break;
        case 15:
            switch (type) {
            case 3:
            case 20:
            case 21:
            case 26: value = node->duration; break;
            }
            sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC160, value));
            break;
        case 1:
            if (type != 10) {
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC160, node->duration));
            } else if (ctx->frameGroup->metadata.extra1 != 3) {
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC168));
            } else {
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC160, node->duration));
            }
            break;
        case 13:
            sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC170));
            break;
        case 14:
            sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, "%.1f %.2f", node->p08.f, node->p0C.f));
            break;
        case 2:
            if (type == 7) {
                value = node->p08.sh[0];
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC160, value));
            }
            break;
        case 3:
            switch (type) {
            case 8: value = node->p08.sh[0]; break;
            case 12: value = node->p08.sh[1]; break;
            case 31: value = node->p08.sh[1]; break;
            }
            if (type == 31 && node->p08.sh[0] == 0) {
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC178));
            } else {
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC160, value));
            }
            break;
        case 4:
            sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC160, node->p08.sh[1]));
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
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC180));
            } else {
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC188));
            }
            break;
        case 12:
            switch (type) {
            case 12:
                switch (node->p08.sh[0]) {
                case 0: sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC190)); break;
                case 1: sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC198)); break;
                case 2: sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC1A0)); break;
                }
                break;
            case 6:
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC088, D_003688E8[node->p08.sh[0]]));
                break;
            case 19:
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC088, D_003688F8[node->p08.sh[0]]));
                break;
            case 1:
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC088, D_00368918[node->p08.sb[0]]));
                break;
            case 28:
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC088, D_00368938[node->p08.sh[0]]));
                break;
            case 10:
                if (node->p14.sh[0] == 0) {
                    sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC1A8));
                } else {
                    sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC1B0));
                }
                break;
            case 30:
                if (node->p08.sh[1] == 0) {
                    sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC1B8));
                } else {
                    sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC1C0));
                }
                break;
            case 31:
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC088, D_00368948[node->p08.sh[0]]));
                break;
            case 32:
                if (node->p08.sh[0] == 0) {
                    sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC1C8));
                } else {
                    sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC1D0));
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
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC180));
            } else {
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC188));
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
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC180));
            } else {
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC188));
            }
            break;
        case 6:
            sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC1D8, node->p08.f * 57.29577637f));
            break;
        case 8:
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
            }
            sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC1E0, value));
            break;
        case 21:
            value = 0;
            sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC1E0, value));
            break;
        case 7:
            if (type == 5) {
                value = node->p08.sh[1];
            }
            sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC160, value));
            break;
        case 10:
            switch (type) {
            case 3: value = node->p0C.sh[1]; break;
            case 26: value = node->p0C.sh[1]; break;
            case 9: value = node->p08.sh[0]; break;
            case 18: value = node->p08.sh[1]; break;
            }
            if (value < 0) {
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, "-----------"));
            } else {
                memcpy(name, ctx->entryName[value], sizeof(name));
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC1E8, name));
            }
            break;
        case 17:
            value = node->p08.sh[0];
            bank = (value >> 12) & 15;
            if (bank == 0) {
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC1F0));
            } else {
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC1F8, bank - 1));
            }
            sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x + 0x240, y, 0xFEFFFF, style, D_003BC1E0, value & 0xFFF));
            break;
        case 18:
            unavailable = 0;
            switch (type) {
            case 4:
                value = node->p08.sh[1];
                if (node->p08.h[0] >> 12) unavailable = 1;
                break;
            case 2: value = node->p0C.sh[0]; break;
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
            }
            if (unavailable) {
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC200));
            } else if (((value >> 12) & 15) == 0) {
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC208));
            } else {
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, "B%d = %d", ((value >> 12) & 15) - 1, value & 0xFFF));
            }
            break;
        case 19:
            switch (node->p08.sb[0]) {
            case 5:
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, "%d-%d %s H%d", node->p0C.sb[0], node->p0C.sb[1], D_00368940[node->p0C.sb[2]], node->p0C.sb[3]));
                break;
            case 6:
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC210, node->p0C.sb[0]));
                break;
            case 7:
                value = node->p0C.sh[0];
                if (value < 0) {
                    sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, "-----------"));
                } else {
                    memcpy(name, ctx->entryName[value], sizeof(name));
                    sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC1E8, name));
                }
                break;
            case 3:
                if (node->p0C.sb[0] == 0 || node->p0C.sb[0] == 2) {
                    sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC218));
                } else {
                    sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC220, node->p0C.b[1]));
                }
                if (node->p0C.b[0] < 2) {
                    sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, "    DEF "));
                } else {
                    sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, "    %.1f", node->p14.f));
                }
                break;
            case 4:
                if (node->p0C.sb[0] == 0 || node->p0C.sb[0] == 2) {
                    sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC218));
                } else {
                    sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC220, node->p0C.b[1]));
                }
                break;
            default:
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC228));
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
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC230));
            } else if (value == 1) {
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC238));
            } else {
                value -= 2;
                memcpy(name, ctx->entryName[value], sizeof(name));
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, D_003BC088, name));
            }
            break;
        }
        x += D_00368750[D_00368768[type].columnTypes[column]] * 0xC0;
    }
}


extern void func_002375D8(s32 list, s32 x, s32 y, s32 color, EvtRuntimeChild *node, EvtRuntime *ctx);

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
            func_002375D8(list, x, y, color, node, ctx);
        } else {
            sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color ? color : 8, "----- NEW FRAME -----"));
        }
    }
}

s32 evtUpdateFrameListDialog(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;
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
    if (D_00368768[group->type].columns == 0) {
        return -1;
    }
    list = (s32)sdfCreateResetPacketList();
    evtDrawMenuFrame(list, x, y, 28, 21, ctx->frameCursor,
                 group->childCount + 1, (u8 *)ctx, func_00237428, evtDrawFrameListRow);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, (SdfListHead *)list);
    if (ctx->actionMode != 5) {
        return 0;
    }
    columns = D_00368768[group->type].columns;
    if (group->childCount + 1 < shown) {
        shown = group->childCount + 1;
    }
    if (evtIsMenuTableEntryEnabled(&group->type) == 1) {
        if (D_00324510.decTen < 0) {
            if (ctx->frameCursor + ctx->frameFirst == 0) {
                return -4;
            }
        } else if (D_00324510.incTen < 0) {
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
    if (D_00324510.syncKey < 0) {
        result = -2;
    }
    if (D_00324510.unk2D < 0) {
        result = -3;
    }
    return result;
}


extern EffWorldNode *dds3GetWorldObject(void);

void evtViewerDrawWorldNodeRow(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
    s32 color;
    s32 count;
    s32 i;
    EffWorldNode *node;

    color = 4;
    if (ctx->groupFirst + ctx->groupCursor != index) {
        color = 0;
    }
    if (index == 0) {
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, " -----------------------"));
        return;
    }
    count = 0;
    for (i = 0; i < 0x12; i++) {
        if (i != EVT_WORLD_SLOT_MOVIE) {
            for (node = ((EvtWorldTable *)dds3GetWorldObject()->data)->slots[i].head; node != NULL; node = node->next) {
                if (((char *)node->value) != NULL) {
                    count++;
                    if (count == index) {
                        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_003BC090, ((char *)node->value)));
                        return;
                    }
                }
            }
        }
    }
}

s32 func_00238A88(s32 x, s32 y, EvtRuntime *ctx) {
    s32 i;
    s32 count = 0;
    s32 list;
    s32 shown = 0x17;
    EffWorldNode *node;

    list = (s32)sdfCreateResetPacketList();
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
                                    (SdfListHead *)list);
    if (ctx->actionMode != 10) {
        return 0;
    }
    if (count < shown) {
        shown = count;
    }
    return kwlnStepTwoListCursors(0, 1, count, 1, shown, NULL,
                                 &ctx->groupFirst, NULL, &ctx->groupCursor);
}

extern char D_003BC240[]; /* "P%d:" */
extern char D_003BC248[]; /* "   %s" */
extern EvtRuntimeChild *evtEventViewerGetPendingNode(EvtRuntime *runtime);

void evtViewerDrawPendingNodeRow(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
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
    sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0xE, D_003BC240, index));
    if (node != NULL) {
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_003BC248, ((char *)node->value)));
    } else {
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, "   -----------------------"));
    }
}

extern void evtViewerDrawPendingNodeRow();

/* Draw the pending-node selector for group types 20/21; other types return -1. */
s32 mnuDrawInfoWindowB(s32 x, s32 y, EvtRuntime *work) {
    u32 packets = (u32)sdfCreateResetPacketList();
    s32 rows;
    switch (work->frameGroup->type) {
    case 20:
        rows = 2;
        break;
    case 21:
        rows = 4;
        break;
    default:
        return -1;
    }
    evtDrawMenuFrame(packets, x, y, 0x1C, rows, 0, rows, (u8 *)work, 0, evtViewerDrawPendingNodeRow);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, (SdfListHead *)packets);
    if (work->actionMode != 0xC) {
        return 0;
    }
    return kwlnStepTwoListCursors(0, 1, rows, 1, rows, 0, 0, 0, &work->inputB);
}



/* Draw the message count from the runtime window context
 * and its entry handle, returning two rows used. */
s32 mnuDrawMessageMenuLabel(s32 list, s32 x, s32 y, EvtRuntime *work) {
    s32 count = itfMesGetEntryCount(work->windowContext->handle);
    sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, "MESSAGE MENU (MESMAX %3d)", count));
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

void evtDrawMessageDataRow(s32 list, s32 x, s32 y, u32 kind, EvtRuntime *ctx) {
    char *names[11] = {D_003BC250, D_003AE8D0, D_003AE8E0, D_003AE8F0,
        D_003AE900, D_003AE910, D_003AE920, D_003AE930, D_003AE940, D_003AE950, D_003AE960};

    switch (kind) {
    case 0: {
        s32 color = 4;
        if (ctx->messageField != 0) {
            color = 0;
        }
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_003BC058, ctx->value & 0xFFF));
        if (ctx->windowContext->handle == -1) {
            sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x + 0x600, y, 0xFEFFFF, color, "NONE MESDATA!!"));
        } else if (itfMesGetWindowEntryItems(ctx->windowContext->handle, ctx->value & 0xFFF) == 0) {
            sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x + 0x600, y, 0xFEFFFF, color, "(NORMAL)"));
        } else {
            sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x + 0x600, y, 0xFEFFFF, color, "(BRANCH)"));
        }
        break;
    }
    case 1: {
        s32 color = 4;
        if (ctx->messageField != 1) {
            color = 0;
        }
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_003BC258, names[(ctx->value >> 12) & 0xF]));
        break;
    }
    case 3:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003ADE78));
        break;
    case 4:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, " U,D = SELECT "));
        break;
    case 5:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003ADE40));
        break;
    case 6:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003ADE50));
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

    list = (s32)sdfCreateResetPacketList();
    evtDrawMenuFrame(list, x, y, 0x18, 0xA, 0, 1, (u8 *)ctx, mnuDrawMessageMenuLabel, evtDrawMessageDataRow);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, (SdfListHead *)list);
    if (ctx->actionMode != 0xD) {
        return 0;
    }
    packed = ctx->value;
    number = packed & 0xFFF;
    branch = (packed >> 12) & 0xF;
    field = ctx->messageField;
    if (field != 0) {
        if (field == 1) {
            if (D_00324510.decOne & 2) {
                branch = branch == 0 ? 0xA : branch - 1;
            } else if (D_00324510.incOne & 2) {
                branch = branch >= 0xA ? 0 : branch + 1;
            }
            ctx->value &= 0xFFF;
            ctx->value |= branch << 12;
        }
    } else {
        delta = 0;
        if (D_00324510.decOne & 2) {
            delta = -1;
        } else if (D_00324510.incOne & 2) {
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
    if ((D_00324510.decTen & 2) || (D_00324510.incTen & 2)) {
        ctx->messageField = !field;
    }
    if (D_00324510.confirm < 0) {
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
    return D_00324510.cancel >= 0 ? 0 : -1;
}



s32 mnuDrawCutFlagLabel(s32 target, s32 x, s32 y) {
    sdfAppendPacket((SdfListHead *)target, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, "CUTFLAG MENU"));
    return 2;
}


/* Draw the packed four-bit option or twelve-bit number and highlight
 * the selected comparison field; later rows provide input instructions. */
void evtDrawComparisonValueRow(s32 list, s32 x, s32 y, u32 index, EvtRuntime *ctx) {
    char *labels[11] = {D_003BC260, D_003AE8D0, D_003AE8E0, D_003AE8F0,
        D_003AE900, D_003AE910, D_003AE920, D_003AE930, D_003AE940, D_003AE950, D_003AE960};
    s32 flag;

    switch (index) {
    case 0:
        flag = ctx->compareField != 0 ? 0 : 4;
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, flag, D_003BC258, labels[(ctx->value >> 12) & 0xF]));
        return;
    case 1:
        flag = ctx->compareField != 1 ? 0 : 4;
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, flag, D_003BC058, ctx->value & 0xFFF));
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x + 0x600, y, 0xFEFFFF, flag, "  (CMP VALUE)"));
        return;
    case 3:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003ADE78));
        return;
    case 4:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, " U,D = SELECT "));
        return;
    case 5:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003ADE40));
        return;
    case 6:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003ADE50));
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

    list = (s32)sdfCreateResetPacketList();
    evtDrawMenuFrame(list, x, y, 0x18, 0xA, 0, 1, (u8 *)ctx, mnuDrawCutFlagLabel, evtDrawComparisonValueRow);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, (SdfListHead *)list);
    if (ctx->actionMode != 0xE) {
        return 0;
    }
    packed = ctx->value;
    number = packed & 0xFFF;
    branch = (packed >> 12) & 0xF;
    field = ctx->compareField;
    switch (field) {
    case 0:
        if (D_00324510.decOne & 2) {
            branch = branch == 0 ? 0xA : branch - 1;
        } else if (D_00324510.incOne & 2) {
            branch = branch >= 0xA ? 0 : branch + 1;
        }
        ctx->value &= 0xFFF;
        ctx->value |= branch << 12;
        break;
    case 1:
        delta = 0;
        if (D_00324510.decOne & 2) {
            delta = -1;
        } else if (D_00324510.incOne & 2) {
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
    if ((D_00324510.decTen & 2) || (D_00324510.incTen & 2)) {
        ctx->compareField = !field;
    }
    if (D_00324510.confirm < 0) {
        return 1;
    }
    return D_00324510.cancel >= 0 ? 0 : -1;
}

s32 evtIsMenuTableEntryEnabled(s32 *index) {
    return D_00368950[*index].enabled != 0;
}

/* Return the table value selected by this group's type and editor column. */
s32 mnuGetSelectedTableValue(EvtRuntime *runtime) {
    return D_00368950[runtime->frameGroup->type].fields[runtime->tableColumn];
}

extern char *D_00368AA0[];
extern char *D_00368AB0[];
extern char D_003BC290[];
extern char D_003BC298[];
extern char D_003BC2A0[];
extern s8 D_003BC268[3];

void evtDrawGroupPropertyTable(s32 list, s32 x, s32 y, s32 hidden, EvtRuntime *runtime) {
    s32 offset = 0;
    EvtRuntimeGroup *group = runtime->frameGroup;
    s32 i;
    s32 field;
    s32 style;

    if (hidden != 0) {
        return;
    }
    for (i = 0; i < D_00368950[group->type].enabled; i++) {
        field = D_00368950[group->type].fields[i];
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x + offset * 192, y,
            0xFEFFFF, 14, D_00368AA0[field]));
        style = i == runtime->tableColumn && runtime->actionMode == 15 ? 4 : 0;
        switch (field) {
        case 0:
            sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x + (offset * 12 << 4),
                y + 0x80, 0xFEFFFF, style, D_003BC290, runtime->frameGroup->metadata.value));
            break;
        case 1:
            if (runtime->frameGroup->metadata.extra1 < 3) {
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x + (offset * 12 << 4),
                    y + 0x80, 0xFEFFFF, style, D_003BC088,
                    D_00368AB0[runtime->frameGroup->metadata.extra1]));
            } else {
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x + (offset * 12 << 4),
                    y + 0x80, 0xFEFFFF, style, D_003BC088,
                    runtime->entryName[runtime->frameGroup->metadata.extra2]));
            }
            break;
        case 2:
            if (runtime->frameGroup->metadata.extra1 == 0) {
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x + (offset * 12 << 4),
                    y + 0x80, 0xFEFFFF, style, D_003BC298));
            } else {
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x + (offset * 12 << 4),
                    y + 0x80, 0xFEFFFF, style, D_003BC2A0));
            }
            break;
        }
        offset += D_003BC268[field];
    }
}
INCLUDE_RODATA(const s32, "game/code_00235270", D_003AEA70);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AEA80);

INCLUDE_ASM(const s32, "game/code_00235270", func_00239A90);

s32 mnuDrawMotionChangeLabel(s32 target, s32 x, s32 y) {
    sdfAppendPacket((SdfListHead *)target, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, "MOTION CHANGE MENU"));
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

extern char D_003BC2A8[];
extern char D_003BBFF0[];

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

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC268);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC270);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC278);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC280);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC288);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC290);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC298);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC2A0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC2A8);

void func_00239E30(s32 list, s32 x, s32 y, s32 row, EvtRuntime *ctx) {
    s32 i;
    s32 count = 0;
    s32 style = 0;
    MdlCtx *model = (MdlCtx *)((EffectObjectData *)ctx->frameGroup->info->data)->modelHolder->resourceHandle;
    EvtMotionValue packed;
    char *loopNames[] = {D_003BC2A8, D_003BBFF0};

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
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style,
            "GROUP   %d  (MAX %d)", packed.bits.group, count));
        break;
    case 1:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style,
            "MOTNO   %d  (MAX %d)", packed.bits.motion, mdlGetNodeRefHalf(model, packed.bits.group)));
        break;
    case 2:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style,
            "LOOP    %s", loopNames[packed.bits.loop]));
        break;
    case 3:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style,
            "HOKAN   %d", packed.bits.hokan));
        break;
    case 6:
        if (mdlFindNodeById(model, packed.bits.group)->motionTable->entries[packed.bits.motion] != NULL) {
            sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style,
                "MAXFRAME (%d)", mdlFindNodeById(model, packed.bits.group)->motionTable->entries[packed.bits.motion]->packedHeader));
        } else {
            sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, style, "(DUMMY)"));
        }
        break;
    }
}




extern void func_00239E30();

/* Motion editor row: ctx->value packs group (byte 0), motion number (byte 1), loop flag (byte 2) and interpolation (byte 3). */
s32 evtUpdateMotionChangeRow(s32 x, s32 y, EvtRuntime *ctx) {
    s32 list;
    MdlCtx *model;
    s32 count;
    EvtMotionValue packed;

    list = (s32)sdfCreateResetPacketList();
    model = (MdlCtx *)((EffectObjectData *)ctx->frameGroup->info->data)->modelHolder->resourceHandle;
    evtDrawMenuFrame(list, x, y, 0x14, 0xA, 0, 1, (u8 *)ctx, mnuDrawMotionChangeLabel, func_00239E30);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, (SdfListHead *)list);
    if (ctx->actionMode != 0x10) {
        return 0;
    }
    if (D_00324510.decTen & 2) {
        if (ctx->fieldIndex == 0) {
            ctx->fieldIndex = 3;
        } else {
            ctx->fieldIndex = ctx->fieldIndex - 1;
        }
    } else if (D_00324510.incTen & 2) {
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
        if (D_00324510.incOne & 2) {
            do {
                if (packed.bits.group < 3) {
                    packed.bits.group = packed.bits.group + 1;
                } else {
                    packed.bits.group = 0;
                }
            } while (mdlFindNodeById(model, packed.bits.group) == NULL);
        } else if (D_00324510.decOne & 2) {
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
        if (D_00324510.incOne & 2) {
            if (packed.bits.motion >= count - 1) {
                packed.bits.motion = 0;
            } else {
                packed.bits.motion = packed.bits.motion + 1;
            }
        } else if (D_00324510.decOne & 2) {
            if (packed.bits.motion > 0) {
                packed.bits.motion = packed.bits.motion - 1;
            } else {
                packed.bits.motion = count - 1;
            }
        }
        break;
    case 2:
        if ((D_00324510.incOne & 2) || (D_00324510.decOne & 2)) {
            packed.bits.loop = packed.bits.loop == 0;
        }
        break;
    case 3:
        if (D_00324510.incOne & 2) {
            if (packed.bits.hokan < 0x64) {
                packed.bits.hokan = packed.bits.hokan + 1;
            } else {
                packed.bits.hokan = 0;
            }
        }
        if (D_00324510.decOne & 2) {
            if (packed.bits.hokan > 0) {
                packed.bits.hokan = packed.bits.hokan - 1;
            } else {
                packed.bits.hokan = 0x64;
            }
        }
        break;
    }
    ctx->value = packed.word;
    if (D_00324510.confirm < 0 && mdlFindNodeById(model, packed.bits.group)->motionTable->entries[packed.bits.motion] != NULL) {
        return 1;
    }
    return D_00324510.cancel >= 0 ? 0 : -1;
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
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_003BC288));
    } else if (index == 1) {
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_003BC280));
        return;
    } else if (index == 2) {
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_003AEA80));
        return;
    }
    n = 3;
    for (group = ctx->groups; group != NULL; group = group->next) {
        if (group->type == 1) {
            if (n == index) {
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_003BC088, ((char *)group->info->value)));
                return;
            }
            n++;
        }
    }
}

s32 func_0023A688(s32 x, s32 y, EvtRuntime *ctx) {
    EvtRuntimeGroup *group;
    s32 count = 0;
    u32 packets;
    s32 shown = 15;

    for (group = ctx->groups; group != NULL; group = group->next) {
        if (group->type == 1) {
            count++;
        }
    }
    count += 3;
    packets = (u32)sdfCreateResetPacketList();
    evtDrawMenuFrame(packets, x, y, 20, 15, ctx->groupFirst, count, (u8 *)ctx, NULL, evtDrawGroupListRow);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, (SdfListHead *)packets);
    if (ctx->actionMode != 17) {
        return 0;
    }
    if (count < shown) {
        shown = count;
    }
    return kwlnStepTwoListCursors(0, 1, count, 1, shown, 0, &ctx->groupFirst, 0, &ctx->groupCursor);
}

void func_0023A798(u32 list, s32 x, s32 y, s32 row, EvtRuntime *runtime) {
}

extern f32 sdfViewMatrix[];
extern void effMiscAxisAngleToQuaternionVU(f32 angle);
extern void effMiscQuaternionToMatrixVU(void);

/* vu0 routine: apply the selected camera-axis rotations to a position. */
void func_0023A7A0(f32 *position)
{
    if (D_00324510.incOne != 0) {
        EE_MMI_LOAD_MATRIX_COLUMN(vf10, sdfViewMatrix + 1);
        effMiscAxisAngleToQuaternionVU(2.0f * 3.14159265f / 180.0f);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, position);
        VU0_TRANSFORM_POINT(vf10, vf10);
        VU0_SET_W_ONE(vf10);
        VU0_STORE_VF(vf10, position);
    } else if (D_00324510.decOne != 0) {
        EE_MMI_LOAD_MATRIX_COLUMN(vf10, sdfViewMatrix + 1);
        effMiscAxisAngleToQuaternionVU(-2.0f * 3.14159265f / 180.0f);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, position);
        VU0_TRANSFORM_POINT(vf10, vf10);
        VU0_SET_W_ONE(vf10);
        VU0_STORE_VF(vf10, position);
    }
    if (D_00324510.incTen != 0) {
        EE_MMI_LOAD_MATRIX_COLUMN(vf10, sdfViewMatrix);
        effMiscAxisAngleToQuaternionVU(-2.0f * 3.14159265f / 180.0f);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, position);
        VU0_TRANSFORM_POINT(vf10, vf10);
        VU0_SET_W_ONE(vf10);
        VU0_STORE_VF(vf10, position);
    } else if (D_00324510.decTen != 0) {
        EE_MMI_LOAD_MATRIX_COLUMN(vf10, sdfViewMatrix);
        effMiscAxisAngleToQuaternionVU(2.0f * 3.14159265f / 180.0f);
        effMiscQuaternionToMatrixVU();
        VU0_LOAD_VF(vf10, position);
        VU0_TRANSFORM_POINT(vf10, vf10);
        VU0_SET_W_ONE(vf10);
        VU0_STORE_VF(vf10, position);
    }
}

INCLUDE_ASM(const s32, "game/code_00235270", func_0023A968);

void func_0023B1F8(void) {
}

typedef struct EvtFloatPanelRow {
    u16 x;
    u16 y;
    s32 parameter;
    const char *format;
} EvtFloatPanelRow;

/* This coordinate table is supplied by the ordinary data segment. */
extern const u16 D_00368AE0[];
extern const EvtFloatPanelRow D_00368AE8[4];
extern char D_003BC2C0[];
extern char D_003BC2E8[];
extern char D_003BC2F0[];
extern char D_003BC2F8[];
extern char D_003BC188[];
extern u8 D_003BD344;
extern f32 D_003BD358;
extern f32 D_003BD35C;
extern void evtSetSlotVector(s32 vectorSlot, f32 x, f32 y);

/* Draw the XY/overlay editor; cancel restores the captured values and flag. */
s32 func_0023B200(s32 x, s32 y, EvtRuntime *runtime) {
    s32 list;
    EvtRuntimeGroup *group;
    const EvtFloatPanelRow *row;
    s32 i;
    s32 status = 0;

    group = runtime->frameGroup;
    list = (s32)sdfCreateResetPacketList();
    evtDrawMenuFrame((u32)list, x + runtime->horizontalOffset + 0x60, y,
                     0xF, 6, 0, 1, (u8 *)runtime, NULL, func_0023B1F8);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface,
                                     (SdfListHead *)list);

    sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(
        0x8500 + (runtime->horizontalOffset << 4),
        D_00368AE0[runtime->floatSelection], 0xFF0080, 0, D_003BC2C0));
    row = D_00368AE8;
    i = 0;
    do {
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(
            0x85C0 + (runtime->horizontalOffset << 4), row->y, 0xFF0080,
            row->parameter, row->format));
        ++row;
        ++i;
    } while (i != 4);

    {
        s32 valueX = 0x8B00 + (runtime->horizontalOffset << 4);
        s32 style = runtime->floatEditMode == 1 && runtime->floatSelection == 0 ? 6 : 0;

        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(
            valueX, 0x7A80, 0xFF0080, style, D_003BC2E8,
            runtime->floatEditX));
    }
    {
        s32 valueX = 0x8B00 + (runtime->horizontalOffset << 4);
        s32 style = runtime->floatEditMode == 1 && runtime->floatSelection == 1 ? 6 : 0;

        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(
            valueX, 0x7AE0, 0xFF0080, style, D_003BC2F0,
            runtime->floatEditY));
    }
    {
        s32 valueX = 0x8B00 + (runtime->horizontalOffset << 4);
        s32 style = runtime->floatEditMode == 1 && runtime->floatSelection == 2 ? 6 : 0;
        const char *toggleText;
        if (D_003BD344 == 0) {
            toggleText = D_003BC2F8;
        } else {
            toggleText = D_003BC188;
        }

        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(
            valueX, 0x7BA0, 0xFF0080, style, toggleText));
    }

    if (runtime->actionMode != 0x13) {
        return status;
    }
    {
        if (D_00324510.apply < 0) {
            if (runtime->floatEditMode == 0) {
                return 1;
            }
        } else if (D_00324510.confirm < 0) {
            if (runtime->floatEditMode == 0) {
                runtime->floatEditMode = 1;
            } else if (runtime->floatEditMode == 1) {
                runtime->floatEditMode = 0;
            }
        } else if (D_00324510.cancel < 0) {
            if (runtime->floatEditMode == 1) {
                runtime->floatEditMode = 0;
            } else {
                status = -1;
            }
        } else if (D_00324510.incTen & 2) {
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
        } else if (D_00324510.decTen & 2) {
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
        } else if (D_00324510.incOne & 2) {
            if (runtime->floatEditMode == 1) {
                if (runtime->floatSelection == 0) {
                    runtime->floatEditX += 0.01f;
                } else if (runtime->floatSelection == 1) {
                    runtime->floatEditY += 0.01f;
                } else if (runtime->floatSelection == 2) {
                    D_003BD344 ^= 1;
                }
            }
        } else if (D_00324510.decOne & 2) {
            if (runtime->floatEditMode == 1) {
                if (runtime->floatSelection == 0) {
                    runtime->floatEditX -= 0.01f;
                } else if (runtime->floatSelection == 1) {
                    runtime->floatEditY -= 0.01f;
                } else if (runtime->floatSelection == 2) {
                    D_003BD344 ^= 1;
                }
            }
        } else if (D_00324510.unk2B != 0) {
            runtime->horizontalOffset += 8;
        } else if (D_00324510.unk29 != 0) {
            runtime->horizontalOffset -= 8;
        }

        switch (group->metadata.extra1) {
        case 0:
            break;
        case 1: {
            f32 currentX = runtime->floatEditX;
            f32 currentY = runtime->floatEditY;

            D_003BD358 = currentX;
            D_003BD35C = currentY;
            evtSetSlotVector(group->setterId, currentX, currentY);
            break;
        }
        case 2:
            evtSetSlotVector(group->setterId, runtime->floatEditX,
                          runtime->floatEditY);
            break;
        case 3:
            evtSetSlotVector(group->setterId, runtime->floatEditX,
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

                D_003BD358 = savedX;
                D_003BD35C = savedY;
                evtSetSlotVector(group->setterId, savedX, savedY);
                break;
            }
            case 2:
                evtSetSlotVector(group->setterId, runtime->savedFloatEditX,
                              runtime->savedFloatEditY);
                break;
            case 3:
                evtSetSlotVector(group->setterId, runtime->savedFloatEditX,
                              runtime->savedFloatEditY);
                break;
            }
            D_003BD344 = runtime->savedOverlayFlag;
        }
    }
    return status;
}

/* Draw command text for row 0 or 1; null texts and other rows emit nothing. */
void evtDrawOptionalPromptText(s32 list, s32 x, s32 y, s32 kind, EvtRuntime *work) {
    switch (kind) {
    case 0:
        if (work->commandSecond.text != 0) {
            sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003BC088, work->commandSecond.text));
        }
        return;
    case 1:
        if (work->commandThird.text != 0) {
            sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, 0, D_003BC088, work->commandThird.text));
        }
        break;
    }
}


/* In mode 0x14, count down before checking input: zero expires, negative waits
 * indefinitely, and confirm takes precedence over cancel. */
s32 mnuDrawTimedPrompt(s32 x, s32 y, EvtRuntime *work) {
    u32 packets = (u32)sdfCreateResetPacketList();
    s32 count;
    evtDrawMenuFrame(packets, x, y, 0x19, 2, 0, 1, (u8 *)work, 0, evtDrawOptionalPromptText);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, (SdfListHead *)packets);
    if (work->actionMode != 0x14) {
        return 0;
    }
    count = work->commandFirst;
    if (count > 0) {
        work->commandFirst = count - 1;
    } else if (count == 0) {
        return -1;
    }
    if (D_00324510.confirm < 0) {
        return 1;
    }
    return D_00324510.cancel >= 0 ? 0 : -1;
}

/* Store three command arguments. Timed prompts consume the first as a countdown
 * and the other two as text; the DDS1 API passes those addresses as words. */
void evtSetRuntimeCommandValues(EvtRuntime *runtime, s32 first, s32 second, s32 third) {
    runtime->commandFirst = first;
    runtime->commandSecond.word = second;
    runtime->commandThird.word = third;
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
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_003BC300));
        return;
    }
    if (index == 1) {
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, "DEFFAULT"));
        return;
    }
    n = 2;
    for (group = ctx->groups; group != NULL; group = group->next) {
        if (group->type == 0x18) {
            if (n == index) {
                sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, D_003BC088, ctx->entryName[group->entryHeader]));
                return;
            }
            n++;
        }
    }
}

s32 evtViewerDrawGroupWindow(s32 x, s32 y, EvtRuntime *ctx) {
    EvtRuntimeGroup *group;
    s32 count = 0;
    u32 packets;
    s32 shown = 15;

    for (group = ctx->groups; group != NULL; group = group->next) {
        if (group->type == 0x18) {
            count++;
        }
    }
    count += 2;
    packets = (u32)sdfCreateResetPacketList();
    evtDrawMenuFrame(packets, x, y, 20, 15, ctx->groupFirst, count, (u8 *)ctx, NULL, evtViewerDrawGroupRow);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, (SdfListHead *)packets);
    if (ctx->actionMode != 21) {
        return 0;
    }
    if (count < shown) {
        shown = count;
    }
    return kwlnStepTwoListCursors(0, 1, count, 1, shown, 0, &ctx->groupFirst, 0, &ctx->groupCursor);
}

extern char *D_00368B18[];

/* Draw one row of the shadow-configuration menu. */
INCLUDE_RODATA(const s32, "game/code_00235270", D_003AEBB0);

void func_0023BC30(s32 list, s32 x, s32 y, s32 index, EvtRuntime *ctx) {
    EvtRuntimeChild *node;
    s32 color;

    color = ctx->groupCursor + 2 == index ? 4 : 0;
    node = evtEventViewerGetPendingNode(ctx);
    switch (index) {
    case 0:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, "SHADOW CONFIG"));
        return;
    case 2:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, "MODE   :%s", D_00368B18[ctx->shadowMode]));
        return;
    case 3:
        sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, "ALPHA  :%3d", ctx->shadowAlpha));
        return;
    case 4:
        if (node->parameterBytes[0] == 3) {
            sdfAppendPacket((SdfListHead *)list, (u32)sdfCreateFormattedSifCommand(x, y, 0xFEFFFF, color, "Y ZAHYO:%.1f", (double)ctx->shadowY));
        }
        break;
    }
}

/* Shadow-configuration menu: draw it, move the row cursor, and step the
 * selected row's mode, alpha or Y offset; confirm returns 1, cancel -1. */
s32 func_0023BE40(s32 x, s32 y, EvtRuntime *ctx) {
    EvtRuntimeChild *node = evtEventViewerGetPendingNode(ctx);
    s32 list = (s32)sdfCreateResetPacketList();
    s32 lastRow = 0;
    s32 lastMode = 0;

    evtDrawMenuFrame(list, x, y, 0x16, 6, 0, 0, (u8 *)ctx, 0, func_0023BC30);
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface, (SdfListHead *)list);
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
    if (D_00324510.incTen & 2) {
        if (ctx->groupCursor < lastRow) {
            ctx->groupCursor++;
        } else {
            ctx->groupCursor = 0;
        }
    } else if (D_00324510.decTen & 2) {
        if (ctx->groupCursor == 0) {
            ctx->groupCursor = lastRow;
        } else {
            ctx->groupCursor--;
        }
    } else if (D_00324510.incOne & 2) {
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
    } else if (D_00324510.unk2B & 2) {
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
    } else if (D_00324510.decOne & 2) {
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
    } else if (D_00324510.unk29 & 2) {
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
        if (D_00324510.confirm < 0) {
            return 1;
        }
        if (D_00324510.cancel < 0) {
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

extern EvtSelectionCache *D_003BB128;
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

/* Poll solid-rectangle setup; zero status clears runtime control and returns -1. */
s32 evtPollRuntimeControlReady(KwlnTask *task) {
    EvtRuntime *runtime;

    runtime = (EvtRuntime *)kwlnTaskGetUserValue(task);
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
extern char D_003BC318[];
extern char D_003BC320[];
extern char D_003BC328[];
extern char D_003BC330[];
extern char D_003BC338[];
extern char D_003BC340[];
extern char D_003AEC00[];
extern char D_003AEC10[];
extern char D_003AEC20[];
extern char D_003AEC30[];

/* Edit the pending key's position, color, blend mode and scale. */
INCLUDE_RODATA(const s32, "game/code_00235270", D_003AEC00);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AEC10);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AEC20);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AEC30);

s32 func_0023C248(EvtRuntime *runtime) {
    EvtRuntimeChild *key;
    u8 *channel;
    s32 packetList;
    s32 selectedChannel;
    s32 step;
    s32 next;
    s32 color;
    s32 style;

    packetList = (s32)sdfCreateResetPacketList();
    kwlnDrawSpriteCell((u32)packetList, 0x78, 0x138, 0xC, 9);
    key = evtEventViewerGetPendingNode(runtime);

    style = runtime->editField == 0 ? 6 : 0;
    sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
        0x7780, 0x82C0, 0xFEFFFF, 0, D_003BC318));
    sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
        0x7780, 0x82C0, 0xFEFFFF, style, D_003BC290, key->p0C.sh[0]));

    style = runtime->editField == 1 ? 6 : 0;
    sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
        0x7780, 0x8320, 0xFEFFFF, 0, D_003BC320));
    sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
        0x7780, 0x8320, 0xFEFFFF, style, D_003BC290, key->p0C.sh[1]));

    selectedChannel = runtime->editField - 2;
    if ((u32)selectedChannel >= 3) {
        selectedChannel = -1;
    }
    color = key->p10.b[0] | (key->p10.b[1] << 8) | (key->p10.b[2] << 16);
    fldDrawPackedRgbEditor((void *)packetList, 0x7780, 0x8380,
                           selectedChannel, color, 0);

    style = runtime->editField == 5 ? 6 : 0;
    sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
        0x7780, 0x84A0, 0xFEFFFF, 0, D_003BC328));
    sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
        0x7780, 0x84A0, 0xFEFFFF, style, D_003BC290, key->p10.b[3]));

    style = runtime->editField == 6 ? 6 : 0;
    sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
        0x7780, 0x8500, 0xFEFFFF, 0, D_003BC330));
    switch ((s8)key->p08.b[1]) {
    case 0:
        sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
            0x7780, 0x8500, 0xFEFFFF, style, D_003AEC00));
        break;
    case 1:
        sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
            0x7780, 0x8500, 0xFEFFFF, style, D_003AEC10));
        break;
    case 2:
        sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
            0x7780, 0x8500, 0xFEFFFF, style, D_003AEC20));
        break;
    }

    style = runtime->editField == 7 ? 6 : 0;
    sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
        0x7780, 0x8560, 0xFEFFFF, 0, D_003BC338));
    sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
        0x7780, 0x8560, 0xFEFFFF, style, D_003AEC30,
        key->p14.f));

    style = runtime->editField == 8 ? 6 : 0;
    sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
        0x7780, 0x85C0, 0xFEFFFF, 0, D_003BC340));
    sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
        0x7780, 0x85C0, 0xFEFFFF, style, D_003AEC30,
        key->p18.f));
    kwlnPositionedTextSurface.append(&kwlnPositionedTextSurface,
                                     (SdfListHead *)packetList);

    step = 0;
    if ((D_00324510.incTen & 2) != 0) {
        if (runtime->editField >= 8) {
            runtime->editField = 0;
        } else {
            runtime->editField++;
        }
    } else if ((D_00324510.decTen & 2) != 0) {
        if (runtime->editField <= 0) {
            runtime->editField = 8;
        } else {
            runtime->editField--;
        }
    } else if ((D_00324510.decOne & 2) != 0) {
        step = -1;
    } else if ((D_00324510.incOne & 2) != 0) {
        step = 1;
    } else if ((D_00324510.unk29 & 2) != 0) {
        step = -10;
    } else if ((D_00324510.unk2B & 2) != 0) {
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


    if (D_00324510.cancel < 0) {
        runtime->frameGroup->unk28 = 0;
        return 0;
    }
    return 1;
}





/* Type-0x19 node payload: the producer allocates and the writer emits 0x40 bytes. */


typedef char EvtCameraColorPayload_size_must_be_0x40[
    (sizeof(EvtCameraColorPayload) == 0x40) ? 1 : -1];

/* Edit the camera color channels and the three speed/alpha parameter banks. */
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

s32 func_0023CA60(EvtRuntime *runtime) {
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
    kwlnDrawSpriteCell((u32)packetList, panelX, 0x30, 0xD, 0x10);
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
        0x7000 + (panelX << 4), 0x7C00, 0xFEFFFF, 0, D_003BC328));
    sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
        0x7000 + (panelX << 4), 0x7C00, 0xFEFFFF, style, D_003BC290, key->parameters.w[3]));

    style = runtime->colorSelection == 5 ? 6 : 0;
    sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
        0x7000 + (panelX << 4), 0x7C60, 0xFEFFFF, 0, D_003BC330));
    switch ((s32)key->parameters.flagWord) {
    case 0:
        sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
            0x7000 + (panelX << 4), 0x7C60, 0xFEFFFF, style, D_003AEC00));
        break;
    case 1:
        sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
            0x7000 + (panelX << 4), 0x7C60, 0xFEFFFF, style, D_003AEC10));
        break;
    case 2:
        sdfAppendPacket((SdfListHead *)packetList, (u32)sdfCreateFormattedSifCommand(
            0x7000 + (panelX << 4), 0x7C60, 0xFEFFFF, style, D_003AEC20));
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
    if ((D_00324510.incTen & 2) != 0) {
        if (runtime->colorSelection >= 11) {
            runtime->colorSelection = 0;
        } else {
            runtime->colorSelection++;
        }
    } else if ((D_00324510.decTen & 2) != 0) {
        if (runtime->colorSelection <= 0) {
            runtime->colorSelection = 11;
        } else {
            runtime->colorSelection--;
        }
    } else if ((D_00324510.decOne & 2) != 0) {
        step = -1;
    } else if ((D_00324510.incOne & 2) != 0) {
        step = 1;
    } else if ((D_00324510.unk29 & 2) != 0) {
        step = -10;
    } else if ((D_00324510.unk2B & 2) != 0) {
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

    if (D_00324510.cancel < 0) {
        runtime->colorEditorActive = 0;
        return 0;
    }
    return 1;
}


extern s32 effUpdateCh72Params(void);
extern s32 effEventAdvanceBlurTemplateSetup(void);
extern s32 effEventAdvanceScatterBlurSetup(void);
extern s32 effEventAdvanceScaleBlurSetup(void);
extern s32 effEventAdvanceScreenDrawSetup(void);
extern s32 func_0023C248(EvtRuntime *runtime);
extern s32 func_0023CA60(EvtRuntime *runtime);
extern void *D_003BB0C0;
extern void *D_003BB14C;
extern void *D_003BB130;

s32 evtPollEffectFrameControl(s32 arg0, s32 arg1, EvtRuntime *runtime) {
    s32 status = 1;

    switch (runtime->frameGroup->type) {
    case 0xD:
        status = effUpdateCh72Params();
        break;
    case 0xE:
        status = effEventAdvanceBlurTemplateSetup();
        if (runtime->selectedEntry != 0) {
            *(s32 *)((u8 *)D_003BB0C0 + 0x2C) = runtime->selectedEntry;
        }
        break;
    case 0xF:
        status = effEventAdvanceScatterBlurSetup();
        if (runtime->selectedEntry != 0) {
            *(s32 *)((u8 *)D_003BB14C + 0x2C) = runtime->selectedEntry;
        }
        break;
    case 0x17:
        status = effEventAdvanceScaleBlurSetup();
        if (runtime->selectedEntry != 0) {
            *(s32 *)((u8 *)D_003BB130 + 0x2C) = runtime->selectedEntry;
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
        status = func_0023C248(runtime);
        break;
    case 0x19:
        status = func_0023CA60(runtime);
        break;
    default:
        if (D_00324510.apply < 0) {
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

extern s32 (*D_00368B48[])(s32, s32, EvtRuntime *);

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
    func_0023D5B0(output, &header, sizeof(header));

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
            func_0023D5B0(output, &entry, sizeof(entry));
        }
    }
}

/* Emit first, range, third and metadata words in file order. The range word
 * is written whole, not narrowed to the terminal halfword used by child spans. */
void evtWriteRuntimeHeaderValues(s32 output, EvtRuntime *state) {
    s32 first = state->headerFirst;
    s32 second = state->frameRange.word;
    s32 third = state->headerThird;
    s32 metadata = state->headerMetadata;
    s32 buf[4];

    buf[0] = first;
    buf[1] = second;
    buf[2] = third;
    buf[3] = metadata;
    func_0023D5B0(output, buf, 0x10);
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
            func_0023D5B0(output, &record, sizeof(record));
        }
    }
}


/* Emit entryTotal consecutive thirty-two-byte names; nonpositive totals emit nothing. */
void evtWriteFixedSizeEntries(s32 output, EvtRuntime *table) {
    void *entry;
    s32 index;

    index = 0;
    if (0 < table->entryTotal) {
        entry = table->entryName;
        do {
            func_0023D5B0(output, entry, 0x20);
            index = index + 1;
            entry = (void *)((s32)entry + 0x20);
        } while (index < table->entryTotal);
    }
}

void evtWriteGroupHeader(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 2) {
            s32 header[4];
            header[0] = group->entryHeader;
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
        *(u16 *)&header[2] = (u16)group->entryHeader;
        *(u16 *)&header[4] = group->metadata.value;
        header[6] = group->metadata.extra1;
        header[7] = group->metadata.extra2;
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

extern u8 sdfPfsDebugMode;
extern s32 func_0030E8F0(const char *path, s32 flags, ...);
extern s32 func_0030EB78(s32 descriptor);
extern s32 func_00310A68(const char *path, s32 mode);
extern void *sdfDevGetPathBuffer(void);
extern char D_003BC350[];
extern char D_003BC358[];

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
            func_003014F0(pm2Path, "pfs0:/event/pmvtool/%s.PM2", (char *)&runtime->nameStorage[0x14]);
            func_003014F0(pm3Path, "pfs0:/event/pmvtool/%s.PM3", (char *)&runtime->nameStorage[0x14]);
        } else {
            func_003014F0(pm2Path, "%sevent/pmvtool/%s.PM2", sdfDevGetPathBuffer(), (char *)&runtime->nameStorage[0x14]);
            func_003014F0(pm3Path, "%sevent/pmvtool/%s.PM3", sdfDevGetPathBuffer(), (char *)&runtime->nameStorage[0x14]);
        }
    } else if (sdfPfsDebugMode) {
        func_003014F0(pm2Path, "pfs0:/event/e%03d/e%03d/e%03d_%03d/E%03d_%03d.PM2",
            ((u32)D_003BBE78 / 10U) * 10, D_003BBE78, D_003BBE78,
            D_003BBE7A, D_003BBE78, D_003BBE7A);
        func_003014F0(pm3Path, "pfs0:/event/e%03d/e%03d/e%03d_%03d/E%03d_%03d.PM3",
            ((u32)D_003BBE78 / 10U) * 10, D_003BBE78, D_003BBE78,
            D_003BBE7A, D_003BBE78, D_003BBE7A);
    } else {
        func_003014F0(pm2Path, "%sevent/e%03d/e%03d/e%03d_%03d/E%03d_%03d.PM2",
            sdfDevGetPathBuffer(), ((u32)D_003BBE78 / 10U) * 10,
            D_003BBE78, D_003BBE78, D_003BBE7A, D_003BBE78, D_003BBE7A);
        func_003014F0(pm3Path, "%sevent/e%03d/e%03d/e%03d_%03d/E%03d_%03d.PM3",
            sdfDevGetPathBuffer(), ((u32)D_003BBE78 / 10U) * 10,
            D_003BBE78, D_003BBE78, D_003BBE7A, D_003BBE78, D_003BBE7A);
    }

    if (sdfPfsDebugMode) {
        func_003003F0("hdd -> %s\n", pm2Path);
        pm2 = func_0030E8F0(pm2Path, 0x602, 0666);
        pm3 = func_0030E8F0(pm3Path, 0x602, 0666);
    } else {
        func_003003F0("pc -> %s\n", pm2Path);
        pm2 = func_0030E8F0(pm2Path, 0x602);
        pm3 = func_0030E8F0(pm3Path, 0x602);
    }
    if (pm2 < 0 || pm3 < 0) {
        func_003003F0(D_003BC350);
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
            func_003003F0("save object table\n");
            break;
        case 25:
            evtEmitGroupTypeTwentyFivePayloads(pm2, runtime);
            func_003003F0("save rain data\n");
            break;
        }
    }
    func_0030EB78(pm2);
    func_00310A68(D_003BC358, 0);
    evtViewerWriteTrackDirectory(pm3, 3, runtime);
    for (section = 0; section < 26; section++) {
        if (section == 4) {
            evtViewerWriteChildRecords(pm3, 3, runtime);
        }
    }
    func_0030EB78(pm3);
    func_003003F0("save pm3 file\n");
    func_00310A68(D_003BC358, 0);
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

u8 * evtGetRowPayloadAddress(PolyMovieWork *work, s32 row) {
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
        func_003003F0("not found linkslight obj index %s\n", (char *)src->subEntry1Data + link->extra2 * 32);
        link->extra1 = 0;
        link->extra2 = 0;
    }
}

#include "dds3obj.h"
#include "dat_state.h"
#include "sdf_texture_offset_list.h"

/* PM1 section 2 and section 3 use the same 0x20-byte package record. */
typedef struct EvtViewerPackageRecord {
    s32 nameIndex;
    s32 reserved04;
    s32 packageId;
    s32 partId;
    u32 offset;
    s32 length;
    s32 loadOrder;
    s32 reserved1C;
} EvtViewerPackageRecord;

/* PM1 auxiliary and texture section entries each occupy 0x10 bytes. */
typedef struct EvtViewerResourceRecord {
    s32 nameIndex;
    u32 offset;
    s32 kind;
    s32 reserved0C;
} EvtViewerResourceRecord;

/* The section-21 wire record is also the group's link subobject at +0x18. */
typedef char EvtViewerPackageRecord_size[(sizeof(EvtViewerPackageRecord) == 0x20) ? 1 : -1];
typedef char EvtViewerResourceRecord_size[(sizeof(EvtViewerResourceRecord) == 0x10) ? 1 : -1];


extern s32 evtCreateWorldObjectFromResource(s32 area, s32 room, s32 resource0, s32 resource1,
                                            const SdfTextureOffsetListHeader *textureOffsets, s32 flags);
extern EvtRuntimeGroup *evtEventViewerCreateEntry(s32 kind, EvtRuntime *runtime);
extern s32 evtEventViewerAddName(const char *name, EvtRuntime *runtime);
extern void mdlLoadViewerPackage(s32 packageId, s32 partId, s32 flags, void *data, s32 length);
extern s32 mdlSpawnCameraSlotViewerObject(s32 packageId, s32 partId);
extern EvtUnit *evtGetWorldUnitNestedValue(s32 key);
extern s32 evtStageRelinkOwnedNodeResource(void *target, void *owner);
extern EvtRuntimeChild *func_0022B7A0(EvtRuntimeGroup *group, s32 frame, EvtRuntime *runtime);
extern s32 evtPreloadBgm(s32 id);
extern s32 evtIsBgmLoaded(s32 id);
extern s32 fldTitleIsActive(void);
extern void fldStartTitle(s32 id, s32 mode, s32 frames);
extern void kwlnFadeBackgroundStartIn(s32 frames);
extern void kwlnFadeBackgroundStartOut(s32 frames);

typedef struct { u8 bytes[0x18]; } EvtRowPayload18;
typedef struct { u8 bytes[0x20]; } EvtRowPayload20;
typedef struct { u8 bytes[0x24]; } EvtRowPayload24;
typedef struct { u8 bytes[0x28]; } EvtRowPayload28;
typedef struct { u8 bytes[0x2C]; } EvtRowPayload2C;
typedef struct { u8 bytes[0x30]; } EvtRowPayload30;
typedef struct { u8 bytes[0x40]; } EvtRowPayload40;

extern EvtRuntimeChild *func_0022B7A0(EvtRuntimeGroup *, s32, EvtRuntime *);
extern EvtRuntimeGroup *evtEventViewerCreateEntry(s32, EvtRuntime *);
extern s32 evtEventViewerAddName(const char *, EvtRuntime *);
extern void dds3SetCameraFieldOfView(EffWorldNode *, f32);
/* The camp lookup receives the movie owner through its opaque API boundary. */
extern s32 mnuCampFindMatchingEntryIndex(void *, EvtRuntime *, s32);



s32 func_0023EF90(PolyMovieWork *work, EvtRuntime *runtime) {
    char motionName[64];
    EvtWorldTable *worldTable;
    s32 soundRowsPresent;
    s16 timeiId;
    s32 row;
    s32 secondaryIndex;
    s32 packageId;
    s32 partId;
    s32 objectKey;
    EffWorldNode *node;
    EffWorldNode *owner;
    EvtRuntimeGroup *group;
    EvtRuntimeChild *key;

    EffWorldNode *world;
    EvtCameraColorPayload *rain;
    u8 *data;
    void *payload;
    s32 *header;
    s32 rowCount;
    s32 oldIndex;
    s32 headerLast, headerFirst;

    func_003003F0("start SetGameData\n");
    soundRowsPresent = 0;
    if (work->mainEntry2Data != NULL) {
        packageId = ((EvtViewerPackageRecord *)work->mainEntry2Data)->packageId;
        partId = ((EvtViewerPackageRecord *)work->mainEntry2Data)->partId;
        evtCreateWorldObjectFromResource(packageId, partId, (s32)work->mainEntry10Data,
            (s32)work->mainEntry11Data, (SdfTextureOffsetListHeader *)work->mainEntry12Data, 0);
        group = evtEventViewerCreateEntry(0, runtime);
        group->entryHeader = evtEventViewerAddName(
            (char *)work->mainEntry1Data + ((EvtViewerPackageRecord *)work->mainEntry2Data)->nameIndex * 32, runtime);
        group->resourceIds.resourceGroup = packageId;
        group->resourceIds.resourceId = partId;
    }
    worldTable = ((EffWorldNode *)dds3GetWorldSecondaryObject())->data;
    for (node = worldTable->slots[4].head; node != NULL; node = node->next) {
        if (node->value != 0) {
            const char *objectName;
            group = evtEventViewerCreateEntry(2, runtime);
            objectName = (char *)node->value;
            group->info = node;
            group->entryHeader = evtEventViewerAddName(objectName, runtime);
        }
    }
    for (secondaryIndex = 2; secondaryIndex >= 0; secondaryIndex--) {
        for (row = 0; row < (s32)work->unk_38; row++) {
            if (((EvtViewerPackageRecord *)work->mainEntry3Data)[row].loadOrder == secondaryIndex) {
                packageId = ((EvtViewerPackageRecord *)work->mainEntry3Data)[row].packageId;
                partId = ((EvtViewerPackageRecord *)work->mainEntry3Data)[row].partId;
                mdlLoadViewerPackage(packageId, partId, 0x103,
                    (u8 *)work->data + ((EvtViewerPackageRecord *)work->mainEntry3Data)[row].offset,
                    ((EvtViewerPackageRecord *)work->mainEntry3Data)[row].length);
                objectKey = mdlSpawnCameraSlotViewerObject(packageId, partId);
                group = evtEventViewerCreateEntry(1, runtime);
                {
                    EffWorldNode *modelObject;
                    EvtViewerPackageRecord *packages;
                    char *names;
                    modelObject = dds3FindWorldObjectNodeByKey((void *)dds3GetWorldSecondaryObject(), objectKey, 5);
                    packages = (EvtViewerPackageRecord *)work->mainEntry3Data;
                    names = (char *)work->mainEntry1Data;
                    group->info = modelObject;
                    group->entryHeader = evtEventViewerAddName(names + packages[row].nameIndex * 32, runtime);
                }
                group->info->value = (u32)runtime->entryName[group->entryHeader];
                group->resourceIds.resourceGroup = packageId;
                group->resourceIds.resourceId = partId;
                if (evtGetWorldUnitNestedValue(objectKey)->owner->first != NULL) {
                    mdlAddEntryPlainEx(evtGetWorldUnitNestedValue(objectKey)->owner, 0, 0, 0.0f, 0.0f);
                }
                {
                    ObjectTransform *transform = group->info->inner;
                    PCP_COPY_VECTOR(group->savedPosition, transform->position);
                    PCP_COPY_VECTOR(group->savedRotation, transform->rotation);
                }
            }
        }
    }
    for (row = 0; row < (s32)work->unk_44; row++) {
        s32 entryNameIndex;
        EvtViewerResourceRecord *resources;
        u8 *resourceBase;
        switch (((EvtViewerResourceRecord *)work->mainEntry7Data)[row].kind) {
        case 0: group = evtEventViewerCreateEntry(3, runtime); break;
        case 1: group = evtEventViewerCreateEntry(0x12, runtime); break;
        case 2: group = evtEventViewerCreateEntry(0x14, runtime); break;
        case 3: group = evtEventViewerCreateEntry(0x15, runtime); break;
        case 4: group = evtEventViewerCreateEntry(0x1A, runtime); break;
        default: continue;
        }
        entryNameIndex = evtEventViewerAddName(
            (char *)work->mainEntry1Data + ((EvtViewerResourceRecord *)work->mainEntry7Data)[row].nameIndex * 32, runtime);
        resources = (EvtViewerResourceRecord *)work->mainEntry7Data;
        group->entryHeader = entryNameIndex;
        resourceBase = (u8 *)work->data;
        group->resourceData = resourceBase + resources[row].offset;
    }
    row = 0;
    if ((s32)work->unk_54 > 0) {
        s32 textureCount;
        do {
            s32 entryNameIndex;
            EvtViewerResourceRecord *resources;
            u8 *resourceBase;
            SdfTex *texture;
            group = evtEventViewerCreateEntry(0x18, runtime);
            entryNameIndex = evtEventViewerAddName(
                (char *)work->mainEntry1Data + ((EvtViewerResourceRecord *)work->mainEntry22Data)[row].nameIndex * 32, runtime);
            resources = (EvtViewerResourceRecord *)work->mainEntry22Data;
            group->entryHeader = entryNameIndex;
            resourceBase = (u8 *)work->data;
            texture = sdfTexAcquireResourceTexture(
                (SdfTextureFileHeader *)(resourceBase + resources[row].offset));
            textureCount = (s32)work->unk_54;
            group->texture = texture;
            row++;
        } while (row < textureCount);
    }
    for (node = worldTable->slots[9].head; node != NULL; node = node->next) {
        if (node->value != 0) {
            const char *objectName;
            group = evtEventViewerCreateEntry(9, runtime);
            objectName = (char *)node->value;
            group->info = node;
            group->entryHeader = evtEventViewerAddName(objectName, runtime);
        }
    }
    runtime->activeEntryIndex = 0;
    for (node = worldTable->slots[16].head; node != NULL; node = node->next) {
        if (node->value != 0) {
            for (owner = worldTable->slots[4].head; owner != NULL; owner = owner->next) {
                if (owner->value != 0) {
                    func_003014F0(motionName, "%s_MOTION", (char *)owner->value);
                    if (strcmp((char *)node->value, motionName) == 0) {
                        evtStageRelinkOwnedNodeResource(node, owner);
                    }
                }
            }
            for (owner = worldTable->slots[9].head; owner != NULL; owner = owner->next) {
                if (owner->value != 0) {
                    func_003014F0(motionName, "%s_MOTION", (char *)owner->value);
                    if (strcmp((char *)node->value, motionName) == 0) {
                        evtStageRelinkOwnedNodeResource(node, owner);
                    }
                }
            }
            for (owner = worldTable->slots[7].head; owner != NULL; owner = owner->next) {
                if (owner->value != 0) {
                    func_003014F0(motionName, "%s_MOTION", (char *)owner->value);
                    if (strcmp((char *)node->value, motionName) == 0) {
                        evtStageRelinkOwnedNodeResource(node, owner);
                    }
                }
            }
        }
    }
    func_003003F0("end pm1 load\n");

    /* Access each serialized row through its version-specific accessor. */
    if (work->subEntry0Data != NULL) {
        s32 metadata;
        header = (s32 *)work->subEntry0Data;
        runtime->headerThird = header[2];
        headerLast = runtime->headerThird - 1;
        headerFirst = header[0];
        runtime->headerFirst = headerFirst;
        if (headerFirst < 0) {
            headerFirst = 0;
        }
        runtime->frameRange.word = header[1];
        metadata = header[3];
        runtime->curFrame = headerFirst;
        runtime->headerMetadata = metadata;
        if (headerLast < headerFirst) {
            runtime->curFrame = headerLast;
        }
    }

    /* 0023F500..0023F6F0: kind 0x1. */
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0x1) {
            for (group = runtime->groups; group != NULL; group = group->next) {
                const char *rowName;
                s32 nameIndex;
                nameIndex = evtGetRowVariant(work, row);
                rowName = (char *)work->subEntry1Data + nameIndex * 32;
                if (strcmp(runtime->entryName[group->entryHeader], rowName) == 0) {
                    key = func_0022B7A0(group, evtGetRowParameter(work, row), runtime);
                    key->p08.b[0] = *(u8 *)(evtGetRowPayloadAddress(work, row) + 0x0);
                    key->p0C.i = *(s32 *)(evtGetRowPayloadAddress(work, row) + 0x4);
                    nameIndex = evtGetRowVariant(work, row);
                    key->serializedValue = evtEventViewerAddName(
                        (char *)work->subEntry1Data + nameIndex * 32, runtime);
                    if (key->p08.sb[0] == 7) {
                        const char *oldName;
                        oldIndex = *(s16 *)(evtGetRowPayloadAddress(work, row) + 0x4);
                        nameIndex = *(s16 *)(evtGetRowPayloadAddress(work, row) + 0x4);
                        oldName = (char *)work->subEntry1Data + nameIndex * 32;
                        func_003003F0("oldtable index=%d string=%s\n", oldIndex, oldName);
                        if (*(s16 *)(evtGetRowPayloadAddress(work, row) + 0x4) != -1) {
                            s32 objectNameIndex;
                            world = dds3GetWorldObject();
                            objectNameIndex = *(s16 *)(evtGetRowPayloadAddress(work, row) + 0x4);
                            if (dds3FindObjectChainNodeByName(world,
                                    work->subEntry1Data + objectNameIndex * 32) != NULL) {
                                s32 linkedNameIndex = *(s16 *)(evtGetRowPayloadAddress(work, row) + 0x4);
                                key->p0C.h[0] = evtEventViewerAddName(
                                    (char *)work->subEntry1Data + linkedNameIndex * 32, runtime);
                            } else {
                                key->p0C.h[0] = -1;
                                func_003003F0("name not found \n");
                            }
                        }
                    }
                    key->p10.h[0] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x8);
                    key->p14.f = *(f32 *)(evtGetRowPayloadAddress(work, row) + 0xC);
                }
            }
        }
    }

    /* 0023F6F4..0023F7EC: kind 0x2. */
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0x2) {
            for (group = runtime->groups; group != NULL; group = group->next) {
                const char *rowName;
                s32 nameIndex;
                nameIndex = evtGetRowVariant(work, row);
                rowName = (char *)work->subEntry1Data + nameIndex * 32;
                if (strcmp(runtime->entryName[group->entryHeader], rowName) == 0) {
                    key = func_0022B7A0(group, evtGetRowParameter(work, row), runtime);
                    key->p08.f = *(f32 *)(evtGetRowPayloadAddress(work, row) + 0x0);
                    key->p0C.h[0] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x4);
                    {
                        s32 serializedNameIndex;
                        EffWorldNode *camera;
                        nameIndex = evtGetRowVariant(work, row);
                        serializedNameIndex = evtEventViewerAddName(
                            (char *)work->subEntry1Data + nameIndex * 32, runtime);
                        camera = group->info;
                        key->serializedValue = serializedNameIndex;
                        dds3SetCameraFieldOfView(camera, key->p08.f);
                    }
                }
            }
        }
    }

    /* 0023F7F0..0023FAC8: native two jump tables both cover 3..0x1A.
     * Do not replace the second accessor call with the outer switch value. */
    for (row = 0; row < (s32)work->unk_A0; row++) {
        switch (evtGetRowValue(work, row)) {
        case 3: case 0x12: case 0x14: case 0x15: case 0x1A:
            for (group = runtime->groups; group != NULL; group = group->next) {
                const char *rowName;
                s32 nameIndex;
                nameIndex = evtGetRowVariant(work, row);
                rowName = (char *)work->subEntry1Data + nameIndex * 32;
                if (strcmp(runtime->entryName[group->entryHeader], rowName) == 0) {
                    key = func_0022B7A0(group, evtGetRowParameter(work, row), runtime);
                    switch (evtGetRowValue(work, row)) {
                    case 3: case 0x1A:
                        key->p08.h[1] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x2);
                        key->p0C.b[1] = *(u8 *)(evtGetRowPayloadAddress(work, row) + 0x5);
                        if (*(s16 *)(evtGetRowPayloadAddress(work, row) + 0x6) < 0) {
                            key->p0C.h[1] = -1;
                        } else {
                            nameIndex = *(s16 *)(evtGetRowPayloadAddress(work, row) + 0x6);
                            key->p0C.h[1] = evtEventViewerAddName(
                                (char *)work->subEntry1Data + nameIndex * 32, runtime);
                        }
                        key->p0C.b[0] = *(u8 *)(evtGetRowPayloadAddress(work, row) + 0x4);
                        if (work->sub->kind < 4) {
                            key->duration = 0;
                        } else {
                            key->duration = evtGetRowFlags(work, row);
                        }
                        if (work->sub->kind < 9) {
                            key->duration = 0;
                        }
                        break;
                    case 0x14: case 0x15:
                        key->p08.h[1] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x2);
                        key->duration = evtGetRowFlags(work, row);
                        for (secondaryIndex = 0; secondaryIndex < 4; secondaryIndex++) {
                            if (*(s8 *)(evtGetRowPayloadAddress(work, row) + 4 + secondaryIndex) < 0) {
                                key->p0C.b[secondaryIndex] = -1;
                            } else {
                                nameIndex = *(s8 *)(evtGetRowPayloadAddress(work, row) + 4 + secondaryIndex);
                                key->p0C.b[secondaryIndex] = evtEventViewerAddName(
                                    (char *)work->subEntry1Data + nameIndex * 32, runtime);
                            }
                        }
                        key->p08.b[1] = *(u8 *)(evtGetRowPayloadAddress(work, row) + 0x1);
                        if (work->sub->kind < 9) {
                            key->duration = 0;
                        }
                        break;
                    case 0x12:
                        key->p08.h[0] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x0);
                        key->p0C.b[0] = *(u8 *)(evtGetRowPayloadAddress(work, row) + 0x4);
                        if (*(s16 *)(evtGetRowPayloadAddress(work, row) + 0x2) < 0) {
                            key->p08.h[1] = -1;
                        } else {
                            nameIndex = *(s16 *)(evtGetRowPayloadAddress(work, row) + 0x2);
                            key->p08.h[1] = evtEventViewerAddName(
                                (char *)work->subEntry1Data + nameIndex * 32, runtime);
                        }
                        break;
                    }
                }
            }
            break;
        }
    }

    /* 0023FACC..0023FBD8: kind 0x9. */
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0x9) {
            for (group = runtime->groups; group != NULL; group = group->next) {
                const char *rowName;
                s32 nameIndex;
                nameIndex = evtGetRowVariant(work, row);
                rowName = (char *)work->subEntry1Data + nameIndex * 32;
                if (strcmp(runtime->entryName[group->entryHeader], rowName) == 0) {
                    key = func_0022B7A0(group, evtGetRowParameter(work, row), runtime);
                    key->duration = evtGetRowFlags(work, row);
                    key->p08.h[1] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x2);
                    if (*(s16 *)(evtGetRowPayloadAddress(work, row) + 0x0) < 0) {
                        key->p08.h[0] = -1;
                    } else {
                        nameIndex = *(s16 *)(evtGetRowPayloadAddress(work, row) + 0x0);
                        key->p08.h[0] = evtEventViewerAddName(
                            (char *)work->subEntry1Data + nameIndex * 32, runtime);
                    }
                }
            }
        }
    }

    /* 0023FBDC..0023FE28: seven separate groups are always constructed.
     * Version <7 swaps ordinal/flags sources relative to later versions. */
    for (secondaryIndex = 0; secondaryIndex < 7; secondaryIndex++) {
        group = evtEventViewerCreateEntry(0xA, runtime);
        group->setterId = secondaryIndex;
        for (row = 0; row < (s32)work->unk_A0; row++) {
            if (evtGetRowValue(work, row) == 0xA &&
                *(s16 *)(evtGetRowPayloadAddress(work, row) + 0x8) == secondaryIndex) {
                key = func_0022B7A0(group, evtGetRowParameter(work, row), runtime);
                key->p08.f = *(f32 *)(evtGetRowPayloadAddress(work, row) + 0x0);
                key->p0C.f = *(f32 *)(evtGetRowPayloadAddress(work, row) + 0x4);
                key->p10.h[0] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x8);
                if (work->sub->kind < 7) {
                    key->duration = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0xA);
                } else {
                    key->duration = evtGetRowFlags(work, row);
                }
                {
                    s32 serializedValue = evtGetRowVariant(work, row);
                    PmdHeader *format = work->sub;
                    key->serializedValue = serializedValue;
                    if (format->kind < 7) {
                        s32 ordinal;
                        payload = key->payload;
                        ordinal = evtGetRowFlags(work, row);
                        *(EvtRowPayload30 *)payload =
                            *(EvtRowPayload30 *)(work->subEntry13Data + ordinal * 0x30);
                    } else {
                        s32 ordinal;
                        payload = key->payload;
                        ordinal = *(s16 *)(evtGetRowPayloadAddress(work, row) + 0xA);
                        *(EvtRowPayload30 *)payload =
                            *(EvtRowPayload30 *)(work->subEntry13Data + ordinal * 0x30);
                    }
                }
                key->p14.h[0] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0xC);
            }
        }
    }

    /* 0023FE2C..0023FF00: kind 0xB. */
    group = evtEventViewerCreateEntry(0xB, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0xB) {
            s32 ordinal;
            key = func_0022B7A0(group, evtGetRowParameter(work, row), runtime);
            data = (u8 *)evtGetRowPayloadAddress(work, row);
            payload = key->payload;
            key->p08.h[0] = *(u16 *)data;
            ordinal = evtGetRowFlags(work, row);
            *(EvtRowPayload20 *)payload =
                *(EvtRowPayload20 *)(work->subEntry14Data + ordinal * 0x20);
        }
    }

    /* 0023FF04..00240030: kind 0xC. */
    group = evtEventViewerCreateEntry(0xC, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0xC) {
            key = func_0022B7A0(group, evtGetRowParameter(work, row), runtime);
            key->duration = evtGetRowFlags(work, row);
            key->p08.h[0] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x0);
            key->p08.h[1] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x2);
            key->p0C.i = *(s32 *)(evtGetRowPayloadAddress(work, row) + 0x4);
            if (work->sub->kind == 1) {
                key->p08.h[1] = 0;
            }
            if (work->sub->kind == 2) {
                key->p08.sh[1] -= 128;
                if (key->p08.sh[1] < -255) {
                    key->p08.h[1] = -255;
                }
                if (key->p08.sh[1] > 255) {
                    key->p08.h[1] = 255;
                }
            }
            key->p14.h[0] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0xC);
        }
    }

    /* 00240034..002400AC: kind 0x16. */
    group = evtEventViewerCreateEntry(0x16, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0x16) {
            key = func_0022B7A0(group, evtGetRowParameter(work, row), runtime);
            key->p08.h[0] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x0);
        }
    }

    /* 002400B0..002401A4: kind 0xD. */
    group = evtEventViewerCreateEntry(0xD, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0xD) {
            s32 ordinal;
            key = func_0022B7A0(group, evtGetRowParameter(work, row), runtime);
            data = (u8 *)evtGetRowPayloadAddress(work, row);
            payload = key->payload;
            key->p08.h[0] = *(u16 *)data;
            ordinal = *(s16 *)(evtGetRowPayloadAddress(work, row) + 0x2);
            *(EvtRowPayload28 *)payload =
                *(EvtRowPayload28 *)(work->subEntry15Data + ordinal * 0x28);
        }
    }

    /* 002401A8..0024032C: kind 0xE. */
    group = evtEventViewerCreateEntry(0xE, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0xE) {
            s32 ordinal;
            key = func_0022B7A0(group, evtGetRowParameter(work, row), runtime);
            data = (u8 *)evtGetRowPayloadAddress(work, row);
            payload = key->payload;
            key->p08.h[0] = *(u16 *)data;
            ordinal = *(s16 *)(evtGetRowPayloadAddress(work, row) + 0x2);
            *(EvtRowPayload2C *)payload =
                *(EvtRowPayload2C *)(work->subEntry16Data + ordinal * 0x2C);
            key->p10.h[0] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x8);
            if (*(s16 *)(evtGetRowPayloadAddress(work, row) + 0x8) != 0 && *(s16 *)(evtGetRowPayloadAddress(work, row) + 0x8) != 1) {
                s32 matchedNameIndex = mnuCampFindMatchingEntryIndex(work, runtime, *(s16 *)(evtGetRowPayloadAddress(work, row) + 0x8) - 2);
                if (matchedNameIndex != -1) {
                    key->p10.h[0] = matchedNameIndex + 2;
                } else {
                    key->p10.h[0] = 0;
                }
            }
        }
    }

    /* 00240330..002404B4: kind 0xF. */
    group = evtEventViewerCreateEntry(0xF, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0xF) {
            s32 ordinal;
            key = func_0022B7A0(group, evtGetRowParameter(work, row), runtime);
            data = (u8 *)evtGetRowPayloadAddress(work, row);
            payload = key->payload;
            key->p08.h[0] = *(u16 *)data;
            ordinal = *(s16 *)(evtGetRowPayloadAddress(work, row) + 0x2);
            *(EvtRowPayload2C *)payload =
                *(EvtRowPayload2C *)(work->subEntry17Data + ordinal * 0x2C);
            key->p10.h[0] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x8);
            if (*(s16 *)(evtGetRowPayloadAddress(work, row) + 0x8) != 0 && *(s16 *)(evtGetRowPayloadAddress(work, row) + 0x8) != 1) {
                s32 matchedNameIndex = mnuCampFindMatchingEntryIndex(work, runtime, *(s16 *)(evtGetRowPayloadAddress(work, row) + 0x8) - 2);
                if (matchedNameIndex != -1) {
                    key->p10.h[0] = matchedNameIndex + 2;
                } else {
                    key->p10.h[0] = 0;
                }
            }
        }
    }

    /* 002404B8..0024063C: kind 0x17. */
    group = evtEventViewerCreateEntry(0x17, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0x17) {
            s32 ordinal;
            key = func_0022B7A0(group, evtGetRowParameter(work, row), runtime);
            data = (u8 *)evtGetRowPayloadAddress(work, row);
            payload = key->payload;
            key->p08.h[0] = *(u16 *)data;
            ordinal = *(s16 *)(evtGetRowPayloadAddress(work, row) + 0x2);
            *(EvtRowPayload2C *)payload =
                *(EvtRowPayload2C *)(work->subEntry20Data + ordinal * 0x2C);
            key->p10.h[0] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x8);
            if (*(s16 *)(evtGetRowPayloadAddress(work, row) + 0x8) != 0 && *(s16 *)(evtGetRowPayloadAddress(work, row) + 0x8) != 1) {
                s32 matchedNameIndex = mnuCampFindMatchingEntryIndex(work, runtime, *(s16 *)(evtGetRowPayloadAddress(work, row) + 0x8) - 2);
                if (matchedNameIndex != -1) {
                    key->p10.h[0] = matchedNameIndex + 2;
                } else {
                    key->p10.h[0] = 0;
                }
            }
        }
    }

    /* 00240640..00240734: kind 0x1B. */
    group = evtEventViewerCreateEntry(0x1B, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0x1B) {
            s32 ordinal;
            key = func_0022B7A0(group, evtGetRowParameter(work, row), runtime);
            data = (u8 *)evtGetRowPayloadAddress(work, row);
            payload = key->payload;
            key->p08.h[0] = *(u16 *)data;
            ordinal = *(s16 *)(evtGetRowPayloadAddress(work, row) + 0x2);
            *(EvtRowPayload28 *)payload =
                *(EvtRowPayload28 *)(work->subEntry24Data + ordinal * 0x28);
        }
    }

    /* 00240738..00240820: kind 0x10. */
    group = evtEventViewerCreateEntry(0x10, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0x10) {
            s32 ordinal;
            key = func_0022B7A0(group, evtGetRowParameter(work, row), runtime);
            data = (u8 *)evtGetRowPayloadAddress(work, row);
            payload = key->payload;
            key->p08.h[0] = *(u16 *)data;
            ordinal = *(s16 *)(evtGetRowPayloadAddress(work, row) + 0x2);
            *(EvtRowPayload18 *)payload =
                *(EvtRowPayload18 *)(work->subEntry18Data + ordinal * 0x18);
            key->p14.h[0] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0xC);
        }
    }

    /* 00240824..002409A8: kind 0x11. */
    group = evtEventViewerCreateEntry(0x11, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0x11) {
            s32 ordinal;
            key = func_0022B7A0(group, evtGetRowParameter(work, row), runtime);
            data = (u8 *)evtGetRowPayloadAddress(work, row);
            payload = key->payload;
            key->p08.h[0] = *(u16 *)data;
            ordinal = *(s16 *)(evtGetRowPayloadAddress(work, row) + 0x2);
            *(EvtRowPayload24 *)payload =
                *(EvtRowPayload24 *)(work->subEntry19Data + ordinal * 0x24);
            key->p10.h[0] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x8);
            if (*(s16 *)(evtGetRowPayloadAddress(work, row) + 0x8) != 0 && *(s16 *)(evtGetRowPayloadAddress(work, row) + 0x8) != 1) {
                s32 matchedNameIndex = mnuCampFindMatchingEntryIndex(work, runtime, *(s16 *)(evtGetRowPayloadAddress(work, row) + 0x8) - 2);
                if (matchedNameIndex != -1) {
                    key->p10.h[0] = matchedNameIndex + 2;
                } else {
                    key->p10.h[0] = 0;
                }
            }
            key->p14.h[0] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0xC);
        }
    }

    /* 002409AC..00240A84: message track exists only for a live populated window. */
    if (work->handle != -1 && itfMesGetEntryCount(work->handle) != 0) {
        itfMesSetWindowPageAndRefresh(work->handle, 3, 0);
        group = evtEventViewerCreateEntry(4, runtime);
        for (row = 0; row < (s32)work->unk_A0; row++) {
            if (evtGetRowValue(work, row) == 4) {
                key = func_0022B7A0(group, evtGetRowParameter(work, row), runtime);
                key->duration = evtGetRowFlags(work, row);
                key->p08.h[0] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x0);
                key->p08.h[1] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x2);
            }
        }
        itfMesSetWindowHighFlags(work->handle, 0xD00000);
    }

    /* Primary rows precede version 6; later versions use the extended table. */
    group = evtEventViewerCreateEntry(5, runtime);
    if (work->sub->kind < 6) {
        rowCount = work->unk_A0;
    } else {
        rowCount = work->unk_9C;
    }
    for (row = 0; row < rowCount; row++) {
        if (work->sub->kind < 6) {
            if (evtGetRowValue(work, row) == 5) {
                soundRowsPresent = 1;
                key = func_0022B7A0(group, evtGetRowParameter(work, row), runtime);
                key->p08.h[0] = *(u16 *)evtGetRowPayloadAddress(work, row);
            }
        } else if (((EvtExtendedRow *)work->secondEntry4Data)[row].value == 5) {
            soundRowsPresent = 1;
            key = func_0022B7A0(group, ((EvtExtendedRow *)work->secondEntry4Data)[row].parameter, runtime);
            key->p08.h[0] = *(u16 *)((EvtExtendedRow *)work->secondEntry4Data)[row].payload;
        }
    }
    group = evtEventViewerCreateEntry(0x13, runtime);
    if (work->sub->kind < 6) {
        rowCount = work->unk_A0;
    } else {
        rowCount = work->unk_9C;
    }
    for (row = 0; row < rowCount; row++) {
        if (work->sub->kind < 6) {
            if (evtGetRowValue(work, row) == 0x13) {
                soundRowsPresent = 1;
                key = func_0022B7A0(group, evtGetRowParameter(work, row), runtime);
                key->p08.h[0] = *(u16 *)evtGetRowPayloadAddress(work, row);
                key->p08.h[1] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 2);
            }
        } else if (((EvtExtendedRow *)work->secondEntry4Data)[row].value == 0x13) {
            soundRowsPresent = 1;
            key = func_0022B7A0(group, ((EvtExtendedRow *)work->secondEntry4Data)[row].parameter, runtime);
            key->p08.h[0] = *(u16 *)((EvtExtendedRow *)work->secondEntry4Data)[row].payload;
            key->p08.h[1] = *(u16 *)(((EvtExtendedRow *)work->secondEntry4Data)[row].payload + 2);
        }
    }

    /* 00240D0C..00240D94: kind 0x6. */
    group = evtEventViewerCreateEntry(0x6, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0x6) {
            key = func_0022B7A0(group, evtGetRowParameter(work, row), runtime);
            key->duration = evtGetRowFlags(work, row);
            key->p08.h[0] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x0);
        }
    }

    /* 00240D98..00240E24: kind 0x7. */
    group = evtEventViewerCreateEntry(0x7, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0x7) {
            key = func_0022B7A0(group, evtGetRowParameter(work, row), runtime);
            key->duration = evtGetRowFlags(work, row);
            key->p08.h[0] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x0);
        }
    }

    /* 00240E28..00240ECC: kind 0x8. */
    group = evtEventViewerCreateEntry(0x8, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0x8) {
            key = func_0022B7A0(group, evtGetRowParameter(work, row), runtime);
            key->p08.h[0] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x0);
            key->p08.h[1] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x2);
            key->p0C.h[0] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x4);
        }
    }

    /* 00240ED0..0024103C: kind 0x18. */
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0x18) {
            for (group = runtime->groups; group != NULL; group = group->next) {
                const char *rowName;
                s32 nameIndex;
                nameIndex = evtGetRowVariant(work, row);
                rowName = (char *)work->subEntry1Data + nameIndex * 32;
                if (strcmp(runtime->entryName[group->entryHeader], rowName) == 0) {
                    key = func_0022B7A0(group, evtGetRowParameter(work, row), runtime);
                    key->p08.b[0] = *(u8 *)(evtGetRowPayloadAddress(work, row) + 0x0);
                    key->p08.b[1] = *(u8 *)(evtGetRowPayloadAddress(work, row) + 0x1);
                    key->p0C.h[0] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x4);
                    key->p0C.h[1] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x6);
                    key->p10.b[0] = *(u8 *)(evtGetRowPayloadAddress(work, row) + 0x8);
                    key->p10.b[1] = *(u8 *)(evtGetRowPayloadAddress(work, row) + 0x9);
                    key->p10.b[2] = *(u8 *)(evtGetRowPayloadAddress(work, row) + 0xA);
                    key->p10.b[3] = *(u8 *)(evtGetRowPayloadAddress(work, row) + 0xB);
                    key->p14.f = *(f32 *)(evtGetRowPayloadAddress(work, row) + 0xC);
                    key->p18.f = *(f32 *)(evtGetRowPayloadAddress(work, row) + 0x10);
                }
            }
        }
    }

    /* 00241040..002411E4: kind 0x19. */
    group = evtEventViewerCreateEntry(0x19, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0x19) {
            key = func_0022B7A0(group, evtGetRowParameter(work, row), runtime);
            key->p08.h[0] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x0);
            key->p08.h[1] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x2);
            key->p0C.h[0] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x4);
            if (work->unk_100 != 0 && work->subEntry25Data != NULL) {
                *(EvtRowPayload40 *)key->payload =
                    *(EvtRowPayload40 *)(work->subEntry25Data + key->p08.sh[1] * 0x40);
            }
            if (work->sub->kind < 8 && key->p08.sh[0] == 0) {
                s32 component;
                rain = (EvtCameraColorPayload *)key->payload;
                rain->parameters.w[3] = 0;
                for (component = 2; component >= 0; component--) {
                    rain->parameters.z[component] = 0;
                }
                func_003003F0("modify rain param\n");
            }
        }
    }

    /* 002411E8..00241288: kind 0x1C. */
    group = evtEventViewerCreateEntry(0x1C, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0x1C) {
            key = func_0022B7A0(group, evtGetRowParameter(work, row), runtime);
            key->duration = evtGetRowFlags(work, row);
            key->p08.h[0] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x0);
            key->p08.h[1] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x2);
        }
    }

    /* 0024128C..00241314: kind 0x1D. */
    group = evtEventViewerCreateEntry(0x1D, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0x1D) {
            key = func_0022B7A0(group, evtGetRowParameter(work, row), runtime);
            key->duration = evtGetRowFlags(work, row);
            key->p08.h[0] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x0);
        }
    }

    /* 00241318..00241360 plus cold .L00240BC8..00240C10.
     * Exactly the first matching title row is restored, then scanning stops. */
    group = evtEventViewerCreateEntry(0x1E, runtime);
    timeiId = -1;
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0x1E) {
            key = func_0022B7A0(group, evtGetRowParameter(work, row), runtime);
            key->p08.h[0] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x0);
            data = (u8 *)evtGetRowPayloadAddress(work, row);
            timeiId = key->p08.sh[0];
            key->p08.h[1] = *(u16 *)(data + 2);
            break;
        }
    }

    /* 00241364..002413EC: kind 0x20. */
    group = evtEventViewerCreateEntry(0x20, runtime);
    for (row = 0; row < (s32)work->unk_A0; row++) {
        if (evtGetRowValue(work, row) == 0x20) {
            key = func_0022B7A0(group, evtGetRowParameter(work, row), runtime);
            key->duration = evtGetRowFlags(work, row);
            key->p08.h[0] = *(u16 *)(evtGetRowPayloadAddress(work, row) + 0x0);
        }
    }

    /* Continue with native .L002413F0: work->subEntry21Data link metadata. */
    if (work->subEntry21Data != NULL) {
        for (row = 0; row < (s32)work->unk_F8; row++) {
            if (((EvtGroupMetadata *)work->subEntry21Data)[row].entry != -1) {
                for (group = runtime->groups; group != NULL; group = group->next) {
                    if (strcmp(runtime->entryName[group->entryHeader],
                        (char *)work->subEntry1Data + ((EvtGroupMetadata *)work->subEntry21Data)[row].entry * 32) == 0) {
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
                    s32 groupType = group->type;
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
    {
        s32 *ids = &datGameState->script.ints[200];
        for (row = 0; row < 10; row++) {
            *ids++ = -1;
        }
    }
    if (soundRowsPresent == 1) {
        evtPreloadBgm(((EvtViewerPackageRecord *)work->mainEntry2Data)->packageId);
        while (evtIsBgmLoaded(((EvtViewerPackageRecord *)work->mainEntry2Data)->packageId) == 0) {
        }
        func_003003F0("smg file load :eventViewer\n");
    }
    if (timeiId != -1) {
        if (!fldTitleIsActive()) {
            fldStartTitle(timeiId, -1, 30);
            func_003003F0("timei file load=%d\n", timeiId);
        } else {
            func_003003F0("warning!! timei process doing!!\n");
        }
    }
    if (mnuCampGetPrimaryOption(runtime) == 0) {
        kwlnFadeBackgroundStartIn(0);
    } else {
        kwlnFadeBackgroundStartOut(0);
    }
    evtViewerDispatchFlagMode(runtime);
    func_0022E5A0(runtime->curFrame, runtime);
    evtViewerDispatchFlagMode(runtime);
    func_003003F0("SetGameData PM2 Version = %d Stageno = %d\n",
        work->sub->kind, ((EvtViewerPackageRecord *)work->mainEntry2Data)->packageId);
    return 1;
}


extern void mnuReleaseCampSceneRegisteredIds(EvtRuntime *runtime);
extern void mnuStopMovieDrawTask(void);
extern s32 sdfCheckPendingWorkWithInterrupts(void);
extern void kwlnTextureReleaseHeldReference(void);
extern u32 kwlnDrawControlFlags;
extern void evtFormatPolygonMoviePaths(u16 a, u16 b, char *path0, char *path1, char *path2);
extern s32 sdfPathExists(char *path);
extern void evtEventViewerShutdown(EvtRuntime *runtime);
extern void evtDestroySecondaryWorldNode(void);
extern void evtEventViewerReleaseGroups(EvtRuntime *runtime);
extern void evtEventViewerReset(EvtRuntime *runtime);

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
    work = func_00234DA8(D_003BBE78, D_003BBE7A, mode);
    if (work != NULL) {
        runtime->windowContext = work;
        func_0023EF90(work, runtime);
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

KwlnTask *evtFindTaskById(u32 taskId) {
    u8 taskName[32];

    evtFormatTaskName(taskId, taskName);
    return kwlnTaskGetTaskByName((const char *)taskName);
}

/* Return this event task's loaded state, or -1 when it does not exist. */
s32 evtGetEventPackLoadedState(u32 taskId) {
    KwlnTask *task;

    task = evtFindTaskById(taskId);
    if (task != 0) {
        EvtPackLoadState *state = (EvtPackLoadState *)kwlnTaskGetUserValue(task);
        return state->loaded;
    }
    return -1;
}

EvtPackLoadState *evtGetEventPackLoadState(u32 taskId) {
    KwlnTask *task;

    task = evtFindTaskById(taskId);
    if (task != 0) {
        return (EvtPackLoadState *)kwlnTaskGetUserValue(task);
    }
    return NULL;
}


void *evtFindTaskResourceEntryByKey(u32 taskId, s32 key) {
    s32 i;
    EvtPackLoadState *data;
    KwlnTask *task;

    task = evtFindTaskById(taskId);
    if (task != 0) {
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
    return NULL;
}

extern void sdfTexReleaseReferenceViaHandler(SdfTex *);
extern void effSetCh72Id(u32);

void evtRefreshTaskEffectTexture(s32 taskId, s32 key) {
    EvtPackLoadState *data = evtGetEventPackLoadState(taskId);
    void *address = evtFindTaskResourceEntryByKey(taskId, key);
    SdfTex *texture;
    if (address != 0) {
        SdfTex *old = data->effect72;
        if (old != 0) {
            sdfTexReleaseReferenceViaHandler(old);
            data->effect72 = 0;
        }
        texture = sdfTexAcquireResourceTexture((SdfTextureFileHeader *)(address));
        effSetCh72Id((u32)texture);
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

extern EvtModelScaleEntry *D_003BAA10;
extern EvtModelScaleEntry *D_003BAA20;

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
        obj = func_00115298(found, vecA, vecB);
        if (obj != 0) {
            effObjSetFlags(obj, 1);
            if (index >= 0) {
                inner = ((EvtEffectObject *)obj)->inner;
                effEventSetScale(inner->scaledObject, D_003BAA20[index].modelScale / D_003BAA10->modelScale);
            }
            return obj;
        }
        return obj;
    }
    return found;
}

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC350);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC358);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC360);

