#include "common.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "evt_picture.h"
#include "itf_panel_draw.h"
#include "sdk_ps2.h"
#include "snd_packed_track_load_status.h"
#include "evt_viewer.h"
#include "evt_pad.h"
#include "evt_sky_alpha_animation_status.h"
#include "mdl_motion_controller_lookup_api.h"
#include "dds3_task_create.h"
#include "evt_task.h"
#include "btl.h"

extern u8 evtPictureQuadBounds[];
extern u8 evtPictureQuadTexcoords[];
extern u8 evtPictureQuadColor[];
SYMBOL_AT(sdfGrOt_t, D_003805A8, sdfGrOtTable, 0xA60);
#include "sdf_packet_alloc.h"
extern void itfMiscDrawAlphaPrim(sdfDrawTag_t *packet, s32 index, s32 flag);

void evtSubmitPictureDrawPacket(const struct sdfTexHandle_t *borrowedTexture) {
    sdfDrawTag_t *packet;

    packet = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdfDrawTagInit(packet);
    itfMiscDrawAlphaPrim(packet, 0, 0);
    itfMiscDrawSpritePrim((DrawVertex *)evtPictureQuadBounds, (DrawColorRec *)evtPictureQuadTexcoords, (DrawColorRec *)evtPictureQuadColor, 0xFFF, borrowedTexture, 0, packet);
    D_003805A8.regFunc(&D_003805A8, packet);
}

/* This recurring task keeps its callback installed while drawing or idle. */
dds3TaskUpdateResult_t evtUpdatePictureWhenFlagged(dds3ProcessID_t *task) {
    EvtPictureWork *picture;

    picture = (EvtPictureWork *)dds3GetProcessData(task);
    if (picture->drawEnabled & EVT_PICTURE_FLAG_DRAW_ENABLED) {
        if (picture->texture != 0) {
            evtSubmitPictureDrawPacket(picture->texture);
        }
        return DDS3_TASK_UPDATE_RETAIN_CALLBACK;
    }
    return DDS3_TASK_UPDATE_RETAIN_CALLBACK;
}

/* Drop the picture's texture, then free the task data. */
void evtPictureShutdown(dds3ProcessID_t *task) {
    EvtPictureWork *picture;

    picture = (EvtPictureWork *)dds3GetProcessData(task);
    if (picture->texture != 0) {
        sdfTexFreeDelayed(picture->texture);
        picture->texture = 0;
    }
    sdfChipFreeDirect(picture);
}

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

/* Create a task with an initialized event payload. */
dds3ProcessID_t *evtStartPictureProcess2(s32 taskId, const char *path) {
    EvtPictureWork *taskData = evtAllocateContext();
    LoadTexture(taskData, path);
    return dds3StartProcess(evtPictureTaskName, taskId, 1, 1, evtUpdatePictureWhenFlagged, evtPictureShutdown, (u32)taskData);
}

dds3ProcessID_t *evtStartPictureProcess(s32 taskId, sdfTexHandle_t *texture) {
    EvtPictureWork *taskData = evtAllocateContext();
    taskData->texture = texture;
    return dds3StartProcess(evtPictureTaskName, taskId, 1, 1, evtUpdatePictureWhenFlagged, evtPictureShutdown, (u32)taskData);
}

INCLUDE_SDATA(const s32, "event/evtPicture", evtPictureTaskName);

