#include "sdk_ps2.h"
#include "snd_packed_track_load_status.h"
#include "common.h"
#include "evt_viewer.h"
#include "evt_picture.h"
#include "evt_sky_alpha_animation_status.h"
#include "dds3_task_create.h"

/* Declared before first use (were implicit int). */

extern dds3TaskUpdateResult_t FrameVarProcess(dds3ProcessID_t *task);
struct SoundMixer;

/* Native viewer child node; frame rows traverse this same linked record. */

/* World-slot node borrowed by a viewer group. Its data is slot-specific;
 * model groups use EvtModelSlot, while all named nodes share the list links. */

/* The native 0x84-byte viewer entry owns its child list and borrows info.
 * EvtRuntime.frameGroup selects one of these entries, not a separate list. */

/* The command API stores words; timed-prompt drawing consumes their text pointers. */

/* The file header emits this whole word; type-8 spans use its low halfword. */

/* Native 0x2490-byte viewer runtime shared by dialogs, task polls and file writers. */

typedef struct {
    s16 enabled;
    s8 fields[8]; /* Per-row signed lookup values at +0x2. */
} EvtTblEntry; /* 0xA bytes */

/* Frame-variable task update: polls the parent task's user value each frame. */
dds3TaskUpdateResult_t FrameVarProcess(dds3ProcessID_t *task) {
    u8 unused[16]; /* retail frame 0x20: unused local storage */

    dds3GetProcessData(task->parent);
    return DDS3_TASK_UPDATE_RETAIN_CALLBACK;
}

dds3ProcessID_t *evtInitFrameVar(void) {
    return dds3StartProcess("FrameVar", 0x2AF9, 1, 1, FrameVarProcess, 0, 0);
}
