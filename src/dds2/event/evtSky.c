#include "sdk_ps2.h"
#include "snd_packed_track_load_status.h"
#include "common.h"
#include "evt_viewer.h"
#include "evt_pad.h"
#include "evt_sky_alpha_animation_status.h"
#include "mdl_motion_controller_lookup_api.h"
#include "dds3_task_create.h"
#include "evt_task.h"
#include "dds3_task_lifecycle.h"
#include "btl.h"

/* Declared before first use (were implicit int). */
/* Original-style implicit int, kept: matched code depends on it: sceWrite. */

extern u32 fldbokasi;

extern EvtSkyAlphaAnimationStatus skyanime;

extern void fldSkySetAlphaOfs(u32 alpha);

extern s16 fldSkyGetAlphaOfs(void);

extern u16 skyanow;

extern u16 skyamax;

extern s16 skyatarget;

extern s16 skyabase_par;

extern void fldSkyDrawFilter(void);

extern void rsetOtOfs(s32 id);

extern void fldDrawBokasi(void);

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

void evtSetBokasi(u32 enabled) {
    fldbokasi = enabled;
}

/* Either set the sky alpha immediately or interpolate from its current value. */
void evtSetSkyAlpha(s32 duration, s32 target) {
    s16 current;

    current = fldSkyGetAlphaOfs();
    if (current != target) {
        if (duration == 0) {
            fldSkySetAlphaOfs(target);
            skyanime = EVT_SKY_ALPHA_ANIMATION_INACTIVE;
        } else {
            skyamax = duration;
            skyatarget = current;
            skyabase_par = target;
            skyanime = EVT_SKY_ALPHA_ANIMATION_ACTIVE;
            skyanow = 0;
        }
    }
}

EvtSkyAlphaAnimationStatus evtCheckSkyAlphaAnime(void) {
    return skyanime;
}

void evtUpdateSkyAlpha(void) {
    if (skyanime != EVT_SKY_ALPHA_ANIMATION_INACTIVE) {
        skyanow += 1;
        fldSkySetAlphaOfs(skyatarget + (s32)((f32)(skyabase_par - skyatarget) * ((f32)skyanow / (f32)skyamax)));
        if (skyanow >= skyamax) {
            skyanime = EVT_SKY_ALPHA_ANIMATION_INACTIVE;
        }
    }
}

s32 evtSkyProcess1(dds3ProcessID_t *task) {
    evtUpdateSkyAlpha();
    fldSkyDrawFilter();
    if (fldbokasi != 0) {
        rsetOtOfs(0x53);
        fldDrawBokasi();
    }
    return 0;
}

void evtSkyShutdown(dds3ProcessID_t *task) {
    skyanime = EVT_SKY_ALPHA_ANIMATION_INACTIVE;
    fldbokasi = 0;
}

extern char evtSkyTaskName[];

void evtExitSky(void) {
    dds3ProcessID_t *task = dds3SearchProcessName(evtSkyTaskName);
    if (task != 0) {
        dds3KillProcess(task, 1);
    }
}

extern void fldSetFilterAnime(u32 mode);
extern void fldSkySetAlpha(u32 drawState);
extern void fldSkySetAlphaOfs(u32 alpha);
extern void fldSetFadeTarget(s32 area, s32 targetMode, s32 duration);

dds3ProcessID_t *evtInitSky(void) {
    fldSetFilterAnime(0);
    fldSkySetAlpha(0x80);
    fldSkySetAlphaOfs(0);
    fldSetFadeTarget(0, 1, 0);
    return dds3StartProcess(evtSkyTaskName, 0x2B0E, 1, 1, evtSkyProcess1, evtSkyShutdown, 0);
}

INCLUDE_SDATA(const s32, "event/evtSky", skyanime);

INCLUDE_SDATA(const s32, "event/evtSky", fldbokasi);

INCLUDE_SDATA(const s32, "event/evtSky", evtSkyTaskName);

