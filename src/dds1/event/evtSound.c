#include "sdk_ps2.h"
#include "snd_packed_track_load_status.h"
#include "common.h"
#include "snd_track_playback_api.h"
#include "snd_sequence_volume_pan.h"
#include "evt_viewer.h"
#include "evt_event_bgm_readiness.h"
#include "evt_sound_variation.h"
#include "evt_sky_alpha_animation_status.h"
#include "mdl_motion_controller_lookup_api.h"
#include "evt_task.h"
#include "btl.h"
#include "sdf_debug_api.h"

/* Declared before first use (were implicit int). */

extern SndPackedTrackLoadStatus p2sdLoadCheckMidi(s32 sequence);
extern void p2sdLoadSMGFile(s32 sequence);
struct SoundMixer;
extern void p2sdPlayBGMFade(s32 track);
extern void p2sdSetBgmTrack(s32 sequence);
extern void p2sdStopBGM(u32 sequence);
extern void p2sdStopSe(u32 sequence);
extern u8 D_003BC360[];

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

void evtTransEventBgm(s32 eventId, EvtSoundVariation variation) {
    EvtPackedSoundTrackId packedTrackId;
    if ((u32)(eventId - 0x258) >= 0x100) {
        return;
    }
    packedTrackId = -1;
    if (variation >= 0) {
        packedTrackId = evtGetSoundIndex(eventId, variation);
    }
    printf("Event BGM trans :%08X\n", packedTrackId);
    p2sdSetBgmTrack(packedTrackId);
}

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

void evtMakeEventPackName(EvtPackEventId eventId, char *buffer) {
    sprintf(buffer, D_003BC360, eventId);
}

INCLUDE_SDATA(const s32, "event/evtSound", D_003BC360);

