#include "common.h"
#include "scr_process_handle_api.h"
#include "sdf_chip.h"
#include "dat_state.h"
#include "dds3_task_create.h"
#include "evt_picture.h"
#include "evt_event_pack.h"
#include "evt_solar.h"
#include "dds3_task_lifecycle.h"
#include "mdl_flag_api.h"

extern s32 fmProcessChk(void);
extern u32 dds3NullDrawProc(void);
extern void rsetOtOfs(u32 row);
extern void rsetAlpha(s32 arg);
extern void rsetTest(s32 bit0, s32 bit1, s32 bit4, s32 bit12,
                               s32 bit14, s32 bit15, s32 unused, s32 bit17);
extern char D_003BBDC8[];
extern f32 gMoonCnt; /* solar overlay alpha, interpolated toward 0 or 1 */
void evtEnableMoonCalc(void);
void evtDisableMoonCalc(void);
void evtSetAgeOfMoon(s32 phase);

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
    DDS3_GBWK->world.phaseTimer = 0.0f;
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
    if ((DDS3_GBWK->world.flags & SOLAR_ALPHA_ENABLED_FLAG) != 0) {
        evtAdvanceSolarOverlayFadeAndDraw(0, 0, 1, (s32)(alpha * 128.0f), overlay, 0x53);
        rsetOtOfs(0x53);
        rsetAlpha(0);
        rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    } else if (alpha > 0.0f) {
        evtAdvanceSolarOverlayFadeAndDraw(0, 0, 1, (s32)(alpha * 128.0f), overlay, 0x53);
    }
    return DDS3_TASK_UPDATE_RETAIN_CALLBACK;
}

extern s32 D_003BBDBC;
extern void fldEsutoMaClearMsg(void);
extern void fldRiberaMaClearMsg(void);
extern void fldRifutoMaClearMsg(void);
extern void fldLightMaClearMsg(void);

void evtAdvanceSolarClockAndExpireControls(f32 travelDistance) {
    if ((u8)(DDS3_GBWK->world.flags & SOLAR_PHASE_ADVANCE_FLAG)) {
        f32 timer = DDS3_GBWK->world.phaseTimer + travelDistance;

        DDS3_GBWK->world.phaseTimer = timer;
        if (timer > 4500.0f) {
            DDS3_GBWK->world.phaseTimer = 0;
            evtSetAgeOfMoon((DDS3_GBWK->world.phase + 1) & 0xF);
            D_003BBDBC = 30;
            if (evtGetAgeOfMoon() == 0) {
                fldEsutoMaClearMsg();
                fldRiberaMaClearMsg();
                fldRifutoMaClearMsg();
                fldLightMaClearMsg();
            }
            if (evtBitCheck(0x819)) {
                DDS3_GBWK->world.timedEventSolarCycleCount++;
                if (DDS3_GBWK->world.timedEventSolarCycleCount >= 81) {
                    evtBitOff(0x819);
                    DDS3_GBWK->world.timedEventSolarCycleCount = 0;
                }
            }
        }
    }
}

dds3TaskUpdateResult_t evtCreateSolarOverlayWork(dds3ProcessID_t *owner) {
    SolarOverlayWork *overlay;

    overlay = sdfChipAllocDirect(sizeof(SolarOverlayWork));
    evtInitializeVisualData(overlay);
    evtLoadSolarNoiseSprite(&overlay->noiseSprite);
    dds3SetProcessData(owner, (u32)overlay);
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
void evtStartSolarOverlay(void) {
    if (moonprocess_id != 0) {
        return;
    }
    moonprocess_id = dds3StartProcess(D_003BBDC8, 0x2b0b, 1, 1, evtCreateSolarOverlayWork, evtFreeSolarOverlayWork, 0);
    evtEnableMoonCalc();
    evtDisableMoonDraw();
    evtSetAgeOfMoon(0);
    DDS3_GBWK->world.timedEventSolarCycleCount = 0;
}

void evtStopSolarOverlay(void) {
    if (moonprocess_id != 0) {
        dds3KillProcess(moonprocess_id, 1);
    }
}

INCLUDE_SDATA(const s32, "event/evtMoon", gMoonCnt);

INCLUDE_SDATA(const s32, "event/evtMoon", D_003BBDBC);

INCLUDE_SDATA(const s32, "event/evtMoon", moonprocess_id);

INCLUDE_SDATA(const s32, "event/evtMoon", D_003BBDC8);

