#include "common.h"
#include "scr_process_handle_api.h"
#include "sdf_chip.h"
#include "dat_state.h"
#include "dat_world_field_flags.h"
#include "dds3_task_create.h"
#include "evt_picture.h"
#include "evt_event_pack.h"
#include "evt_solar.h"
#include "dds3_task_lifecycle.h"
#include "mdl_flag_api.h"

extern f32 gMoonCnt;
extern char D_00437208[];
extern s32 fmProcessChk(void);
extern u32 dds3NullDrawProc(void);
extern void evtEnableMoonCalc(void);
extern void evtDisableMoonCalc(void);
extern void evtSetAgeOfMoon(s32 phase);

/* Fold phases 9-15 back toward zero for the symmetric solar animation. */
EvtMirroredSolarPhase evtGetAgeOfMoon(void) {
    EvtSolarCyclePhase phase;

    phase = DDS3_GBWK->world.phase;
    if (phase >= 9) {
        phase = 8 - (phase & 7);
    }
    return phase;
}

EvtSolarCyclePhase evtGetAgeOfMoon16(void) {
    return DDS3_GBWK->world.phase;
}

void evtSetAgeOfMoon(s32 phase) {
    DDS3_GBWK->world.phase = phase & 0xf;
    DDS3_GBWK->world.phaseTimer = 0;
}

void evtEnableMoonCalc(void) {
    DDS3_GBWK->world.flags = DDS3_GBWK->world.flags | SOLAR_PHASE_ADVANCE_FLAG;
}

void evtDisableMoonCalc(void) {
    DDS3_GBWK->world.flags = DDS3_GBWK->world.flags & ~SOLAR_PHASE_ADVANCE_FLAG;
}

void evtEnableMoonDraw(void) {
    DDS3_GBWK->world.flags = DDS3_GBWK->world.flags | SOLAR_ALPHA_ENABLED_FLAG;
    gMoonCnt = 1.0f;
    evtBeginSolarOverlayFadeIn(0);
}

void evtDisableMoonDraw(void) {
    DDS3_GBWK->world.flags = DDS3_GBWK->world.flags & ~SOLAR_ALPHA_ENABLED_FLAG;
    gMoonCnt = 0.0f;
}

void evtEnableMoonDraw2(void) {
    DDS3_GBWK->world.flags = DDS3_GBWK->world.flags | SOLAR_ALPHA_ENABLED_FLAG;
}

void evtDisableMoonDraw2(void) {
    DDS3_GBWK->world.flags = DDS3_GBWK->world.flags & ~SOLAR_ALPHA_ENABLED_FLAG;
}

dds3TaskUpdateResult_t evtUpdateSolarOverlayFade(dds3ProcessID_t *task) {
    DatWorldState *state;
    SolarOverlayWork *overlay;
    f32 alpha;
    f32 f;

    f = 0.0f;
    if ((fmProcessChk() != 0) || (dds3NullDrawProc() != 0)) {
        return DDS3_TASK_UPDATE_RETAIN_CALLBACK;
    }
    overlay = (SolarOverlayWork *)dds3GetProcessData(task);
    state = &DDS3_GBWK->world;
    alpha = gMoonCnt;
    if ((state->flags & SOLAR_ALPHA_ENABLED_FLAG) != 0) {
        if (alpha < 1.0f) {
            alpha += 0.1f;
            if (alpha > 1.0f) {
                alpha = 1.0f;
            }
            gMoonCnt = alpha;
        }
    } else {
        if (alpha > f) {
            alpha -= 0.1f;
            if (alpha < f) {
                alpha = f;
            }
            gMoonCnt = alpha;
        }
    }
    /* Two jal sites in retail: the DDS1 twin issues extra draw calls in the first arm, removed here. The flags are re-read from the global; the cached state copy does not match. */
    if ((DDS3_GBWK->world.flags & SOLAR_ALPHA_ENABLED_FLAG) != 0) {
        evtAdvanceSolarOverlayFadeAndDraw(0, 0, 1, (s32)(alpha * 128.0f), overlay, 0x53);
    } else if (alpha > 0.0f) {
        evtAdvanceSolarOverlayFadeAndDraw(0, 0, 1, (s32)(alpha * 128.0f), overlay, 0x53);
    }
    return DDS3_TASK_UPDATE_RETAIN_CALLBACK;
}

extern s32 D_004371FC;
extern void fldEsutoMaClearMsg(void);
extern void fldRiberaMaClearMsg(void);
extern void fldRifutoMaClearMsg(void);
extern void fldLightMaClearMsg(void);

/* Advance the enabled solar clock and expire its associated field controls. */
void evtAdvanceSolarClockAndExpireControls(f32 travelDistance) {
    if (DDS3_GBWK->world.flags & SOLAR_PHASE_ADVANCE_FLAG) {
        DDS3_GBWK->world.phaseTimer += travelDistance;
        if (DDS3_GBWK->world.phaseTimer > 4500.0f) {
            DDS3_GBWK->world.phaseTimer = 0;
            evtSetAgeOfMoon((DDS3_GBWK->world.phase + 1) & 0xF);
            D_004371FC = 30;
            if (evtGetAgeOfMoon() == 0) {
                fldEsutoMaClearMsg();
                fldRiberaMaClearMsg();
            }
            if (DDS3_GBWK->world.fieldFlags & DAT_WORLD_FIELD_TRANSITION_WORK) {
                DDS3_GBWK->world.fieldTransitionCycleCount++;
                if (DDS3_GBWK->world.fieldTransitionCycleCount >= 6U) {
                    fldRifutoMaClearMsg();
                    DDS3_GBWK->world.fieldTransitionCycleCount = 0;
                }
            }
            if (DDS3_GBWK->world.fieldFlags & DAT_WORLD_FIELD_LIGHT_COMMAND_PENDING) {
                DDS3_GBWK->world.overlayFlag++;
                if (DDS3_GBWK->world.overlayFlag >= 6) {
                    fldLightMaClearMsg();
                    DDS3_GBWK->world.overlayFlag = 0;
                }
            }
            if (evtBitCheck(0x802)) {
                DDS3_GBWK->world.timedEventSolarCycleCount++;
                if (DDS3_GBWK->world.timedEventSolarCycleCount >= 81) {
                    evtBitOff(0x802);
                    DDS3_GBWK->world.timedEventSolarCycleCount = 0;
                }
            }
            DDS3_GBWK->world.specialEncounterAttemptCount = 0;
        }
    }
}

dds3TaskUpdateResult_t evtCreateSolarOverlayWork(dds3ProcessID_t *task) {
    SolarOverlayWork *overlay;

    overlay = sdfChipAllocDirect(0x104);
    evtInitializeVisualData(overlay);
    evtLoadSolarNoiseSprite(&overlay->noiseSprite);
    dds3SetProcessData(task, (u32)overlay);
    return (dds3TaskUpdateResult_t)evtUpdateSolarOverlayFade;
}

void evtFreeSolarOverlayWork(dds3ProcessID_t *task) {
    SolarOverlayWork *overlay;

    overlay = (SolarOverlayWork *)dds3GetProcessData(task);
    evtReleaseSolarNoiseSprite(&overlay->noiseSprite);
    sdfChipFreeDirect(overlay);
    moonprocess_id = 0;
}

/* One scheduler task owns the overlay; a second start leaves its state intact. */
void evtEnsureSolarOverlayTaskAndResetPhase(void) {
    if (moonprocess_id != 0) {
        return;
    }
    moonprocess_id = dds3StartProcess(D_00437208, 0x2B0B, 1, 1, evtCreateSolarOverlayWork, evtFreeSolarOverlayWork, 0);
    evtEnableMoonCalc();
    evtDisableMoonDraw();
    evtSetAgeOfMoon(0);
    DDS3_GBWK->world.timedEventSolarCycleCount = 0;
    DDS3_GBWK->world.overlayFlag = 0;
    DDS3_GBWK->world.fieldTransitionCycleCount = 0;
}

void evtStopSolarOverlay(void) {
    if (moonprocess_id != 0) {
        dds3KillProcess(moonprocess_id, 1);
    }
}

INCLUDE_SDATA(const s32, "event/evtMoon", gMoonCnt);

INCLUDE_SDATA(const s32, "event/evtMoon", D_004371FC);

INCLUDE_SDATA(const s32, "event/evtMoon", moonprocess_id);

INCLUDE_SDATA(const s32, "event/evtMoon", D_00437208);

