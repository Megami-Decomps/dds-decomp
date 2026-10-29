#include "sdk_ps2.h"
#include "mnu_camp_display.h"
#include "common.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "sdf_chip.h"
#include "evt_viewer.h"
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

extern s32 evtSolarOverlayFadeCounter;

extern s32 evtSolarOverlayFadeDuration;

extern s8 evtSolarOverlayFadeFlags;

#ifdef NON_MATCHING
#include "common.h"
#include "sdf_trig.h"
#include "evt_solar.h"

extern void evtPrepareSolarOverlayTestState(s32 renderHandle);
extern void evtDrawSolarNoiseLayerAtConfiguredScale(s32 x, s32 y, s32 z, s32 alpha, s32 layer, s32 mode,
                                                    s32 contextHandle, s32 color);
extern void evtDrawSolarNoiseLayerAtCenteredScale(s32 x, s32 y, s32 z, s32 alpha, s32 layer,
                                                   s32 mode, s32 contextHandle, s32 color, f32 scale);
extern void evtDrawSolarOverlayMarker(s32 unused0, s32 unused1, s32 alpha, s32 intensity, s32 object);
extern void evtDrawSolarOverlayShapeWithScaledAlpha(s32 unused0, s32 unused1, s32 alpha, s32 alphaScale,
                                                    s32 entryIndex, s32 object);
extern void evtDrawPartialSolarOverlay(s32 x, s32 y, s32 unused2, s32 alpha, s32 entryCount,
                                       s32 contextHandle, s32 renderHandle);
extern void evtDrawIndexedSolarOverlayLayers(s32 x, s32 y, s32 unused2, s32 alpha, s32 entryCount,
                                             s32 contextHandle, s32 renderHandle);
extern void evtDrawSolarLayerPair(s32 x, s32 y, s32 z, s32 width, s32 layer, s32 context, s32 color);
extern void evtDrawShortSolarNoiseLayers(s32 x, s32 y, s32 z, s32 width, SolarNoiseState *state,
                                         s32 context, s32 color);
extern void evtDrawLongSolarNoiseLayers(s32 x, s32 y, s32 z, s32 width, SolarNoiseState *state,
                                        s32 context, s32 color);
extern void evtAdvanceShortSolarNoiseLayers(SolarNoiseState *state);
extern void evtAdvanceLongSolarNoiseLayers(SolarNoiseState *state);
extern void evtDrawSolarPointTransitionShapes(s32 unused0, s32 unused1, s32 alpha, s32 alphaScale, s32 phase,
                                              SolarOverlayWork *overlay, s32 object);

#define SOLAR_PULSE_FRAMES 120.0f     /* the pulse's period, in frames */
#define SOLAR_TRANSITION_FRAMES 20.0f /* evtUpdateSolarPhaseTransition's timer */

/* Draw one frame of the solar (moon phase) overlay: advance its pulse frame
 * (0..120); draw noise layer 13, during a phase transition a centred, scaled
 * flash of layer 14, then layer 14 pulsing (its swing smaller as the phase
 * grows), the short noise layers, layers 11 and 12 and the marker. During a
 * transition, fade the new age's shape in and draw the point shapes and
 * indexed layers for the new age, else for the mirrored phase; from phase 2
 * on, the long noise layers; last the layer pair, cross-faded from the
 * previous age's over the transition's first half. Upstream: INCLUDE_ASM in
 * game/code_0022A248 (DDS1) and game/code_00244F00 (DDS2), whose routines are
 * the same; written from the retail routine. */
void evtDrawFadingSolarOverlayFrame(s32 x, s32 y, s32 z, s32 alpha, s32 mirroredPhase,
                                    SolarOverlayWork *overlay, s32 renderContext) {
    SolarOverlayState *state;
    s32 pulseFrame;
    s32 timer;
    s32 transitionAlpha;
    u8 phase;
    f32 phaseProgress;
    f32 pulse;
    f32 progress;

    phaseProgress = ((f32)mirroredPhase + 1.0f) / 9.0f;
    pulseFrame = overlay->state.pulseFrame + 1;
    overlay->state.pulseFrame = pulseFrame;
    state = &overlay->state;
    if ((f32)pulseFrame > SOLAR_PULSE_FRAMES) {
        overlay->state.pulseFrame = 0;
    }
    pulse = sdfSin((f32)overlay->state.pulseFrame / SOLAR_PULSE_FRAMES * 3.14159265f);

    evtPrepareSolarOverlayTestState(renderContext);
    evtDrawSolarNoiseLayerAtConfiguredScale(x, y, 0, alpha, 13, 0, (s32)overlay, renderContext);

    timer = state->transitionTimer;
    if (timer != 0) {
        f32 swell;

        swell = sdfSin((f32)timer / SOLAR_TRANSITION_FRAMES * 3.14159265f) * 0.4f;
        evtDrawSolarNoiseLayerAtCenteredScale(x + 37, y + 40, z, (s32)((f32)alpha * swell), 14, 0,
                                              (s32)overlay, renderContext,
                                              (1.0f - swell) * 0.1f + 1.0f);
    }

    evtDrawSolarNoiseLayerAtConfiguredScale(
        x, y, z, (s32)((f32)alpha * (pulse * 0.6f * (1.0f - phaseProgress) + 0.2f)), 14, 0,
        (s32)overlay, renderContext);
    evtAdvanceShortSolarNoiseLayers(&state->firstNoise);
    evtDrawShortSolarNoiseLayers(x, y, z, alpha, &state->firstNoise, (s32)overlay, renderContext);
    evtDrawSolarNoiseLayerAtConfiguredScale(x, y, z, alpha, 11, 0, (s32)overlay, renderContext);
    evtDrawSolarNoiseLayerAtConfiguredScale(x, y, z, alpha, 12, 0, (s32)overlay, renderContext);
    evtDrawSolarOverlayMarker(x, y, z, alpha, renderContext);

    timer = state->transitionTimer;
    if (timer != 0) {
        f32 wave;

        progress = (f32)timer / SOLAR_TRANSITION_FRAMES;
        wave = sdfSin(progress * 3.14159265f) * 0.5f;
        phase = (u8)evtGetAgeOfMoon16();
        /* Age 0 and the waning ages 9..15 (mirrored to 7..1) also draw the
         * phase's shape, fading in over the transition's second half. */
        if (phase == 0 || phase >= 9) {
            f32 shapeProgress;

            if (phase != 0) {
                phase = (u8)(8 - (phase & 7));
            }
            shapeProgress = progress - 0.5f;
            shapeProgress += shapeProgress;
            if (shapeProgress < 0.0f) {
                shapeProgress = 0.0f;
            }
            transitionAlpha = (s32)((f32)alpha * shapeProgress);
            evtDrawSolarOverlayShapeWithScaledAlpha(x, y, z, transitionAlpha, phase + 1, renderContext);
            evtDrawPartialSolarOverlay(x, y, z, transitionAlpha, phase + 1, (s32)overlay, renderContext);
        }
        transitionAlpha = (s32)((f32)alpha * wave);
        evtDrawSolarPointTransitionShapes(x, y, z, transitionAlpha, phase, overlay, renderContext);
        evtDrawIndexedSolarOverlayLayers(x, y, z, transitionAlpha, phase, (s32)overlay, renderContext);
        evtDrawSolarPointTransitionShapes(x, y, z, alpha, phase, overlay, renderContext);
        evtDrawIndexedSolarOverlayLayers(x, y, z,
                                         (s32)((f32)alpha * ((f32)mirroredPhase * 0.3f * 0.125f + 0.3f)),
                                         phase, (s32)overlay, renderContext);
    } else {
        evtDrawSolarPointTransitionShapes(x, y, z, alpha, mirroredPhase, overlay, renderContext);
        evtDrawIndexedSolarOverlayLayers(x, y, z,
                                         (s32)((f32)alpha * ((f32)mirroredPhase * 0.2f * 0.125f + 0.3f)),
                                         mirroredPhase, (s32)overlay, renderContext);
    }

    if (mirroredPhase >= 2) {
        evtAdvanceLongSolarNoiseLayers(&overlay->state.secondNoise);
        evtDrawLongSolarNoiseLayers(x, y, z, alpha, &overlay->state.secondNoise, (s32)overlay,
                                    renderContext);
    }

    timer = state->transitionTimer;
    if ((f32)timer > 10.0f) {
        progress = ((f32)timer - 10.0f) / 10.0f;
        phase = (u8)evtGetAgeOfMoon16();
        if (phase == 0) {
            phase = 1;
        } else {
            phase--;
        }
        if (phase >= 9) {
            phase = (u8)(8 - (phase & 7));
        }
        evtDrawSolarLayerPair(x, y, z, (s32)((f32)alpha * progress), phase, (s32)overlay, renderContext);
        evtDrawSolarLayerPair(x, y, z, (s32)((f32)alpha * (1.0f - progress)), mirroredPhase,
                              (s32)overlay, renderContext);
    } else {
        evtDrawSolarLayerPair(x, y, z, alpha, mirroredPhase, (s32)overlay, renderContext);
    }
}
#else
INCLUDE_ASM(const s32, "game/code_00244F00", evtDrawFadingSolarOverlayFrame);
#endif

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
