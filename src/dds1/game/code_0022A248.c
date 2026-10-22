#include "sdk_ps2.h"
#include "mnu_camp_display.h"
#include "common.h"
#include "mdl_flag_api.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "evt_viewer.h"
#include "sdf.h"
#include "sdf_sif_command.h"
#include "evt_solar.h"
#include "eff.h"
#include "eff_blur.h"
#include "eff_event_draw.h"
#include "eff_event_work_api.h"
#include "kwln_task_lifecycle.h"
#include "btl.h"

#define SOLAR_FADE_DRAW_ENABLED 1
#define SOLAR_FADE_IN 2
#define SOLAR_FADE_OUT 4
#define SOLAR_FADE_IN_STATE (SOLAR_FADE_DRAW_ENABLED | SOLAR_FADE_IN)
#define SOLAR_FADE_OUT_STATE (SOLAR_FADE_DRAW_ENABLED | SOLAR_FADE_OUT)

extern s8 evtSolarOverlayFadeFlags;

extern s32 evtSolarOverlayFadeCounter;

extern s32 evtSolarOverlayFadeDuration;

INCLUDE_ASM(const s32, "game/code_0022A248", evtDrawFadingSolarOverlayFrame);

/* Seed both noise-state geometries and cache the current solar phase. */
void evtInitializeVisualData(SolarOverlayWork *overlay) {
    s8 solarPhase = evtGetAgeOfMoon16();
    SolarNoiseState *noise = &overlay->state.firstNoise;
    overlay->state.solarPhase = solarPhase;
    noise->centerX = 0x39;
    noise->centerY = 0x33;
    noise->radiusX = 0x1D;
    noise->radiusY = 0x23;
    noise->spawnInterval = 5;
    noise->unk0E = 10;
    noise = &overlay->state.secondNoise;
    noise->centerX = 0x37;
    noise->centerY = 0x32;
    noise->radiusX = 15;
    noise->radiusY = 15;
    noise->spawnInterval = 10;
    noise->unk0E = 0;
}

/* Restart the transition timer when the solar phase changes; count it down afterwards. */
void evtUpdateSolarPhaseTransition(SolarOverlayWork *overlay) {
    SolarOverlayState *state = &overlay->state;
    u8 solarPhase;
    u32 flags;
    s32 remaining;

    solarPhase = evtGetAgeOfMoon16();
    if (state->solarPhase != solarPhase) {
        flags = state->flags;
        state->transitionTimer = 20;
        state->solarPhase = solarPhase;
        state->flags = flags | 1;
        evtSetSolarPointActiveCount(overlay, evtGetMirroredSolarPhase());
    }
    remaining = state->transitionTimer;
    if (remaining != 0) {
        remaining--;
        state->transitionTimer = remaining;
        if (remaining == 0) {
            state->flags &= ~1;
        }
    }
}

/* Advance the fade and draw with scaled alpha; renderContext is forwarded unchanged. */
void evtAdvanceSolarOverlayFadeAndDraw(s32 x, s32 y, s32 z, s32 alpha, SolarOverlayWork *overlay, s32 renderContext) {
    s32 mirroredPhase;
    /* Preserve signed-byte narrowing before forwarding the mirrored phase. */
    mirroredPhase = (s8)evtGetMirroredSolarPhase();
    evtUpdateSolarPhaseTransition(overlay);
    if ((evtSolarOverlayFadeFlags & SOLAR_FADE_DRAW_ENABLED) != 0) {
        if ((evtSolarOverlayFadeFlags & SOLAR_FADE_IN) != 0) {
            if (evtSolarOverlayFadeCounter < evtSolarOverlayFadeDuration) {
                evtSolarOverlayFadeCounter++;
            } else {
                evtSolarOverlayFadeFlags = 0;
            }
        } else if ((evtSolarOverlayFadeFlags & SOLAR_FADE_OUT) != 0) {
            if (evtSolarOverlayFadeCounter > 0) {
                evtSolarOverlayFadeCounter--;
            } else {
                /* Clears only bit 0 while testing bit 2: the original quirk, kept verbatim. */
                evtSolarOverlayFadeFlags &= ~SOLAR_FADE_DRAW_ENABLED;
            }
        }
        if ((evtSolarOverlayFadeFlags & SOLAR_FADE_DRAW_ENABLED) != 0) {
            f32 fadeRatio = (f32)evtSolarOverlayFadeCounter / (f32)evtSolarOverlayFadeDuration;
            s32 fadedAlpha = (s32)((f32)alpha * fadeRatio);
            evtDrawFadingSolarOverlayFrame(x, y, z, fadedAlpha, mirroredPhase, overlay, renderContext);
            return;
        }
    }
    if ((evtSolarOverlayFadeFlags & SOLAR_FADE_OUT) == 0) {
        evtDrawFadingSolarOverlayFrame(x, y, z, alpha, mirroredPhase, overlay, renderContext);
    }
}

/* The whole flags byte is tested: a latched fade-out flag still reports state. */
s32 evtHasSolarOverlayTransitionState(void) {
    return evtSolarOverlayFadeFlags != 0;
}

/* Zero cancels the transition; otherwise start from zero for the supplied duration. */
void evtBeginSolarOverlayFadeIn(s32 fadeDuration) {
    if (fadeDuration == 0) {
        evtSolarOverlayFadeFlags = 0;
        evtSolarOverlayFadeCounter = 0;
        evtSolarOverlayFadeDuration = 0;
        return;
    }
    evtSolarOverlayFadeDuration = (s32)fadeDuration;
    evtSolarOverlayFadeFlags = SOLAR_FADE_IN_STATE;
    evtSolarOverlayFadeCounter = 0;
}

/* Zero requests an immediate hidden state, retaining the fade-out direction flag. */
void evtBeginSolarOverlayFadeOut(s32 fadeDuration) {
    if (fadeDuration == 0) {
        evtSolarOverlayFadeFlags = SOLAR_FADE_OUT_STATE;
        evtSolarOverlayFadeDuration = 1;
        evtSolarOverlayFadeCounter = 0;
    } else {
        evtSolarOverlayFadeCounter = fadeDuration;
        evtSolarOverlayFadeFlags = SOLAR_FADE_OUT_STATE;
        evtSolarOverlayFadeDuration = fadeDuration;
    }
}
