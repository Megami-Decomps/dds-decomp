#include "sdk_ps2.h"
#include "snd_packed_track_load_status.h"
#include "common.h"
#include "snd_track_playback_api.h"
#include "snd_sequence_volume_pan.h"
#include "evt_viewer.h"
#include "evt_pad.h"
#include "evt_event_bgm_readiness.h"
#include "evt_sound_variation.h"
#include "evt_sky_alpha_animation_status.h"
#include "mdl_motion_controller_lookup_api.h"
#include "evt_task.h"
#include "btl.h"
#include "sdf_debug_api.h"

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

extern void p2sdLoadSMGFile(s32 sound);

extern SndPackedTrackLoadStatus p2sdLoadCheckMidi(s32 sound);

extern void p2sdPlayBGMFade(s32 track);

extern void p2sdStopBGM(u32 sound);
extern void p2sdStopSe(u32 sound);

EvtPackedSoundTrackId evtGetSoundIndex(s32 eventId, EvtSoundVariation variation) {
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

EvtPackedSoundTrackId evtLoadEventBgm(s32 eventId) {
    EvtPackedSoundTrackId packedTrackId;
    if ((u32)(eventId - 0x258) >= 0x100) {
        return 0;
    }
    packedTrackId = evtGetSoundIndex(eventId, 0);
    p2sdLoadSMGFile(packedTrackId);
    return packedTrackId;
}

EvtEventBgmReadiness evtCheckEventBgmLoaded(s32 eventId) {
    if ((u32)(eventId - 0x258) >= 0x100) {
        return EVT_EVENT_BGM_READY;
    }
    return p2sdLoadCheckMidi(evtGetSoundIndex(eventId, 0)) == SND_PACKED_TRACK_LOAD_RESIDENT
        ? EVT_EVENT_BGM_READY
        : EVT_EVENT_BGM_NOT_READY;
}

EvtPackedSoundTrackId evtPlayEventBgm(s32 eventId, EvtSoundVariation variation) {
    EvtPackedSoundTrackId packedTrackId;
    if ((u32)(eventId - 0x258) >= 0x100) {
        return 0;
    }
    packedTrackId = evtGetSoundIndex(eventId, variation);
    printf("Event BGM play :%08X\n", packedTrackId);
    p2sdPlayBGM(packedTrackId);
    return packedTrackId;
}

extern char D_004247F8[]; /* "Event BGM trans :%08X\n" */
extern EvtPackedSoundTrackId evtGetSoundIndex(s32 eventId, EvtSoundVariation variation);
extern void p2sdSetBgmTrack();

void evtTransEventBgm(s32 eventId, EvtSoundVariation variation) {
    EvtPackedSoundTrackId packedTrackId;
    if ((u32)(eventId - 0x258) < 0x100U) {
        packedTrackId = -1;
        if (variation >= 0) {
            packedTrackId = evtGetSoundIndex(eventId, variation);
        }
        printf(D_004247F8, packedTrackId);
        p2sdSetBgmTrack(packedTrackId);
    }
}

INCLUDE_RODATA(const s32, "event/evtSound", D_004247F8);

EvtPackedSoundTrackId evtFadeInEventBgm(s32 eventId, EvtSoundVariation variation) {
    EvtPackedSoundTrackId packedTrackId;
    if ((u32)(eventId - 0x258) >= 0x100) {
        return 0;
    }
    packedTrackId = evtGetSoundIndex(eventId, variation);
    printf("Event BGM fade in play :%08X\n", packedTrackId);
    p2sdPlayBGMFade(packedTrackId);
    return packedTrackId;
}

EvtPackedSoundTrackId evtStopEventBgm(s32 eventId, EvtSoundVariation variation) {
    EvtPackedSoundTrackId packedTrackId;
    if ((u32)(eventId - 0x258) >= 0x100) {
        return 0;
    }
    packedTrackId = evtGetSoundIndex(eventId, variation);
    p2sdStopBGM(packedTrackId);
    return packedTrackId;
}

EvtPackedSoundTrackId evtPlaySe(s32 eventId, EvtSoundVariation variation) {
    EvtPackedSoundTrackId packedTrackId;
    if ((u32)(eventId - 0x258) >= 0x100) {
        return 0;
    }
    packedTrackId = evtGetSoundIndex(eventId, variation);
    p2sdPlaySE(packedTrackId, 0x7F, 0x3F);
    return packedTrackId;
}

EvtPackedSoundTrackId evtStartBgmBySoundIdAndFade(s32 eventId, EvtSoundVariation variation) {
    EvtPackedSoundTrackId packedTrackId;
    if ((u32)(eventId - 0x258) >= 0x100) {
        return 0;
    }
    packedTrackId = evtGetSoundIndex(eventId, variation);
    p2sdStopSe(packedTrackId);
    return packedTrackId;
}

extern char D_004377D0[];

void evtMakeEventPackName(EvtPackEventId eventId, char *buffer) {
    sprintf(buffer, D_004377D0, eventId);
}

INCLUDE_SDATA(const s32, "event/evtSound", D_004377D0);

