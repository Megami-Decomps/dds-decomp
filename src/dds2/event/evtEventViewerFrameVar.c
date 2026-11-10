#include "sdk_ps2.h"
#include "snd_packed_track_load_status.h"
#include "common.h"
#include "evt_viewer.h"
#include "evt_pad.h"
#include "evt_picture.h"
#include "evt_sky_alpha_animation_status.h"
#include "dds3_task_create.h"

/* Declared before first use (were implicit int). */
/* Original-style implicit int, kept: matched code depends on it: sceWrite. */

typedef struct {
    s16 columnCount;
    s8 columns[8]; /* The halfword counts signed property selectors. */
} EvtTblEntry; /* 0xA bytes */

/* Native viewer child node; frame rows traverse this same linked record. */

/* World-slot node borrowed by a viewer group. Its data is slot-specific;
 * model groups use EvtModelSlot, while all named nodes share the list links. */

/* The native 0x84-byte viewer entry owns its child list and borrows info.
 * EvtRuntime.frameGroup selects one of these entries, not a separate list. */

/* The command API stores words; timed-prompt drawing consumes their text pointers. */

/* The file header emits this whole word; type-8 spans use its low halfword. */

/* Native 0x24BC-byte viewer runtime shared by dialogs, task polls and file writers. */

extern char D_004233F0[]; /* " RR  = ENTER" */
extern char D_00423400[]; /* " RD  = CANCEL" */
extern char D_00423428[]; /* " L,R = VALUE-+" */
extern s32 dds3SelectMenu(s32 padSet, s32 secondaryCount, s32 primaryCount, s32 secondaryVisible,
                                  s32 primaryVisible, s32 *secondaryScroll, s32 *primaryScroll,
                                  s32 *secondaryCursor, s32 *primaryCursor);

/* Frame-variable task update: polls the parent task's user value each frame. */
dds3TaskUpdateResult_t evtUpdateFrameVariableTask(dds3ProcessID_t *task) {
    u8 unused[16]; /* retail frame 0x20: unused local storage */

    dds3GetProcessData(task->parent);
    return DDS3_TASK_UPDATE_RETAIN_CALLBACK;
}

extern char D_00423380[]; /* "FrameVar" */

dds3ProcessID_t *evtInitFrameVar(void) {
    return dds3StartProcess(D_00423380, 0x2AF9, 1, 1, evtUpdateFrameVariableTask, 0, 0);
}

INCLUDE_RODATA(const s32, "event/evtEventViewerFrameVar", D_00423380);

