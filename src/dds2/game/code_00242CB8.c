#include "common.h"
#include "scr_integer_parameter_api.h"
#include "scr_integer_return_value_api.h"
#include "scr_process_handle_api.h"
#include "kwln_task_add_child.h"
#include "sdf_chip.h"
#include "dat_state.h"
#include "dat_world_field_flags.h"
#include "scr.h"
#include "scr_command_timer_api.h"
#include "scr_string_parameter_api.h"
#include "kwln.h"
#include "kwln_process_registration.h"
#include "kwln_task_create.h"
#include "evt_world.h"
#include "evt_picture.h"
#include "evt_event_pack.h"
#include "evt_event_bgm_readiness.h"
#include "evt_solar.h"
#include "kwln_task_lifecycle.h"
#include "mdl_flag_api.h"

extern void fldSetFadeTarget(s32 area, s32 targetMode, s32 duration);
extern void evtSetSkyAlpha(s32 duration, s32 target);

extern f32 gMoonCnt;

extern u32 dds3DrawFlag;




extern char D_00437208[];

extern char D_00422050[];





extern s32 fileMenuTaskExists(void);
extern u32 dds3NullDrawProc(void);
#include "sdf_debug_api.h"
void evtPrintDeveloperConsoleMessage(const char *fmt, ...);
extern char D_00422030[];
extern char D_00421FE8[];
s32 evtLoadEventBgm(s32 id);
EvtEventBgmReadiness evtCheckEventBgmLoaded(s32 id);

extern dds3Basic_t *evtSearchBasic(s32 worldKind, s32 worldObjectKey);
extern EvtPathStatus evtCheckPathStat(dds3Basic_t *object);

s32 evtCommand_LIGHT_PATH_WAIT(void) {
    EvtPathStatus status = evtCheckPathStat(
        evtSearchBasic(EFF_WORLD_KIND_LIGHT, scrGetIntPara(0)));

    switch (status) {
    case EVT_PATH_STATUS_AT_ENDPOINT:
        return 1;
    case EVT_PATH_STATUS_IN_PROGRESS:
        return 0;
    case EVT_PATH_STATUS_INVALID:
        return 1;
    default:
        return 0;
    }
}

u32 evtCommand_TEX_BE(void) {
    scrProcessWork_s *work;
    s32 id;
    struct sdfTexHandle_t *entry;
    dds3ProcessID_t *task;

    work = scrGetScriptHandle();
    if (work == NULL) {
        return 1;
    }
    if (work->task == NULL) {
        printf(D_00421FE8);
        return 1;
    }
    id = scrGetIntPara(0);
    entry = evtGetEventPackResourceAddress(id, scrGetIntPara(1));
    if (entry == 0) {
        evtPrintDeveloperConsoleMessage(D_00422030, scrGetIntPara(1));
        return 1;
    }
    task = evtStartPictureProcess(0x2AFE, entry);
    dds3LinkProcess(work->task, task);
    scrSetIntReturnValue(scrProcessHandleToVmInteger(task));
    return 1;
}

u32 evtCommand_TEX_LOAD(void) {
    scrProcessWork_s *work;
    dds3ProcessID_t *childTask;

    work = scrGetScriptHandle();
    if (work == NULL) {
        return 1;
    }
    if (work->task == NULL) {
        printf(D_00422050);
        return 1;
    }
    childTask = evtStartPictureProcess2(0x2afe, scrGetStrPara(0));
    dds3LinkProcess(work->task, childTask);
    scrSetIntReturnValue(scrProcessHandleToVmInteger(childTask));
    return 1;
}

u32 evtCommand_TEX_ON(void) {
    dds3ProcessID_t *task;
    KwlnProcessRegistrationResult registered;

    task = scrVmIntegerToProcessHandle(scrGetIntPara(0));
    registered = dds3ExistProcess(task);
    if (registered != KWLN_PROCESS_NOT_REGISTERED) {
        evtPictureDrawOn(task);
    }
    return 1;
}

u32 evtCommand_TEX_OFF(void) {
    dds3ProcessID_t *task;
    KwlnProcessRegistrationResult registered;

    task = scrVmIntegerToProcessHandle(scrGetIntPara(0));
    registered = dds3ExistProcess(task);
    if (registered != KWLN_PROCESS_NOT_REGISTERED) {
        evtPictureDrawOff(task);
    }
    return 1;
}

extern void evtKillPictureProcess(dds3ProcessID_t *task);

u32 evtCommand_TEX_CLS(void) {
    dds3ProcessID_t *task;
    KwlnProcessRegistrationResult registered;

    task = scrVmIntegerToProcessHandle(scrGetIntPara(0));
    registered = dds3ExistProcess(task);
    if (registered != KWLN_PROCESS_NOT_REGISTERED) {
        evtKillPictureProcess(task);
    }
    return 1;
}

extern void fldSkySetAlpha(u32 drawState);
extern void fldSetFilterAnime(u32 mode);

u32 evtCommand_SET_SKY(void) {
    s32 area;
    s32 target;
    s32 duration;

    fldSetFilterAnime(0);
    fldSkySetAlpha(0x80);
    area = scrGetIntPara(0);
    target = scrGetIntPara(1);
    duration = scrGetIntPara(2);
    fldSetFadeTarget(area, target, duration);
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00242CB8", D_00421FE8);

INCLUDE_RODATA(const s32, "game/code_00242CB8", D_00422030);

INCLUDE_RODATA(const s32, "game/code_00242CB8", D_00422050);

u32 evtCommand_SET_SKY_A(void) {
    s32 alpha;

    alpha = scrGetIntPara(1);
    if (alpha < -255) {
        evtPrintDeveloperConsoleMessage("warning : SET_SKY_A alpha < -255\n");
        alpha = -255;
    }
    if (alpha > 255) {
        evtPrintDeveloperConsoleMessage("warning : SET_SKY_A alpha > 255\n");
        alpha = 255;
    }
    evtSetSkyAlpha(scrGetIntPara(0), alpha);
    return 1;
}

extern void evtSetBokasi(u32 enabled);

u32 evtCommand_BOKASI_ON(void) {
    evtSetBokasi(1);
    return 1;
}

u32 evtCommand_BOKASI_OFF(void) {
    evtSetBokasi(0);
    return 1;
}

u32 evtCommand_LOAD_SOUND_E(void) {
    s32 eventId;

    /* One parameter call per arm; retail has two call sites and sharing one changes the code. */
    if (scrGetCommandTimer() == 0) {
        evtLoadEventBgm(scrGetIntPara(0));
    } else {
        eventId = scrGetIntPara(0);
        if (evtCheckEventBgmLoaded(eventId) != EVT_EVENT_BGM_NOT_READY) {
            return 1;
        }
    }
    return 0;
}

extern s32 evtPlayEventBgm(s32 eventId, s32 fade);

u32 evtCommand_BGM_PLAY_E(void) {
    s32 eventId;
    s32 fade;

    eventId = scrGetIntPara(0);
    fade = scrGetIntPara(1);
    evtPlayEventBgm(eventId, fade);
    return 1;
}

extern void evtTransEventBgm(s32 eventId, s32 fade);

u32 evtCommand_BGM_TRANS_E(void) {
    s32 eventId;
    s32 fade;

    eventId = scrGetIntPara(0);
    fade = scrGetIntPara(1);
    evtTransEventBgm(eventId, fade);
    return 1;
}

extern s32 evtStopEventBgm(s32 eventId, s32 fade);

u32 evtCommand_FADEOUT_E(void) {
    s32 eventId;
    s32 second;

    eventId = scrGetIntPara(0);
    second = scrGetIntPara(1);
    evtStopEventBgm(eventId, second);
    return 1;
}

extern void p2sdBgmVolDown(void);

u32 evtCommand_BGM_VOL_DOWN(void) {
    p2sdBgmVolDown();
    return 1;
}

extern void p2sdBgmVolUp(void);

u32 evtCommand_BGM_VOL_UP(void) {
    p2sdBgmVolUp();
    return 1;
}

extern s32 evtPlaySe(s32 eventId, s32 fade);

u32 evtOpcodeSetBgmVolumePan(void) {
    s32 volume;
    s32 pan;

    volume = scrGetIntPara(0);
    pan = scrGetIntPara(1);
    evtPlaySe(volume, pan);
    return 1;
}

extern s32 evtStartBgmBySoundIdAndFade(s32 eventId, s32 fade);

u32 evtCommandStartBgmWithFade(void) {
    s32 eventId;
    s32 second;

    eventId = scrGetIntPara(0);
    second = scrGetIntPara(1);
    evtStartBgmBySoundIdAndFade(eventId, second);
    return 1;
}

extern void evtRefreshTaskEffectTexture(s32 taskId, s32 key);

u32 evtOpcodeRefreshTaskData(void) {
    s32 task;
    s32 value;

    task = scrGetIntPara(0);
    value = scrGetIntPara(1);
    evtRefreshTaskEffectTexture(task, value);
    return 1;
}

extern void Eve_DistortionBlurSetDefTex(void);

u32 evtOpcodeInitializeEffectSoundChannel(void) {
    Eve_DistortionBlurSetDefTex();
    return 1;
}

u32 evtCommandSetDrawFlagWhileWaiting(void) {
    if (scrGetIntPara(0) <= 0) {
        dds3DrawFlag |= 0x2000000;
        return 1;
    }
    if ((s32)scrGetCommandTimer() < scrGetIntPara(0)) {
        dds3DrawFlag |= 0x2000000;
        return 0;
    }
    return 1;
}



void evtClearSolarOverlayControl(void) {
    DDS3_GBWK->world.overlayFlag = 0;
}

/* Reset the field-transition expiry counter at world offset 0xA4C. */
void evtResetFieldTransitionCounter(void) {
    DDS3_GBWK->world.fieldTransitionCycleCount = 0;
}

/* Fold phases 9-15 back toward zero for the symmetric solar animation. */
EvtMirroredSolarPhase evtGetMirroredSolarPhase(void) {
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

void evtSetSolarOverlayFullyVisible(void) {
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

s32 evtUpdateSolarOverlayFade(dds3ProcessID_t *task) {
    DatWorldState *state;
    SolarOverlayWork *overlay;
    f32 alpha;
    f32 f;

    f = 0.0f;
    if ((fileMenuTaskExists() != 0) || (dds3NullDrawProc() != 0)) {
        return 0;
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
    return 0;
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
            if (evtGetMirroredSolarPhase() == 0) {
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



s32 evtCreateSolarOverlayWork(dds3ProcessID_t *task) {
    SolarOverlayWork *overlay;

    overlay = sdfChipAllocDirect(0x104);
    evtInitializeVisualData(overlay);
    evtLoadSolarNoiseSprite(&overlay->noiseSprite);
    dds3SetProcessData(task, (u32)overlay);
    return (s32)evtUpdateSolarOverlayFade;
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

INCLUDE_SDATA(const s32, "game/code_00242CB8", gMoonCnt);

INCLUDE_SDATA(const s32, "game/code_00242CB8", D_004371FC);

INCLUDE_SDATA(const s32, "game/code_00242CB8", moonprocess_id);

INCLUDE_SDATA(const s32, "game/code_00242CB8", D_00437208);
