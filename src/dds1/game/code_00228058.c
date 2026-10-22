#include "sdf_debug_api.h"
#include "common.h"
#include "scr_integer_return_value_api.h"
#include "kwln_task_add_child.h"
#include "sdf_chip.h"
#include "dat_state.h"
#include "scr.h"
#include "scr_command_timer_api.h"
#include "scr_string_parameter_api.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "evt_world.h"
#include "evt_picture.h"
#include "evt_event_pack.h"
#include "evt_solar.h"
#include "kwln_task_lifecycle.h"
#include "mdl_flag_api.h"

extern char D_003ACAE0[];

extern s32 fileMenuTaskExists(void);
extern u32 dds3NullDrawProc(void);
extern void rsetOtOfs(u32 row);
extern void rsetAlpha(s32 arg);
extern void rsetTest(s32 bit0, s32 bit1, s32 bit4, s32 bit12,
                               s32 bit14, s32 bit15, s32 unused, s32 bit17);
extern u32 dds3DrawFlag;
s32 evtLoadEventBgm(s32 id);
s32 evtCheckEventBgmLoaded(s32 id);


extern char D_003BBDC8[];



extern f32 gMoonCnt; /* solar overlay alpha, interpolated toward 0 or 1 */


extern void fldSetFadeTarget(s32 area, s32 targetMode, s32 duration);
extern void evtSetSkyAlpha(s32 duration, s32 target);

s32 scrGetIntPara(s32 idx);
extern void *evtSearchBasic(s32 objectKind, s32 worldObjectKey);
extern s32 evtCheckPathStat(void *object);

s32 evtCommand_LIGHT_PATH_WAIT(void) {
    s32 status = evtCheckPathStat(
        evtSearchBasic(9, scrGetIntPara(0)));

    switch (status) {
    case 0:
        return 1;
    case 1:
        return 0;
    case -1:
        return 1;
    default:
        return 0;
    }
}

extern char D_003ACA78[];
void evtPrintDeveloperConsoleMessage(const char *fmt, ...);

INCLUDE_RODATA(const s32, "game/code_00228058", D_003ACA78);

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
        printf(D_003ACA78);
        return 1;
    }
    id = scrGetIntPara(0);
    entry = evtGetResorceData(id, scrGetIntPara(1));
    if (entry == 0) {
        evtPrintDeveloperConsoleMessage("TEX_BE not fount RID = %d\n", scrGetIntPara(1));
        return 1;
    }
    task = evtStartPictureProcess(0x2AFE, entry);
    dds3LinkProcess(work->task, task);
    scrSetIntReturnValue((s32)task);
    return 1;
}

u32 evtCommand_TEX_LOAD(void) {
    scrProcessWork_s *work;
    dds3ProcessID_t *task;

    work = scrGetScriptHandle();
    if (work == NULL) {
        return 1;
    }
    if (work->task == NULL) {
        printf(D_003ACAE0);
        return 1;
    }
    task = evtStartPictureProcess2(0x2afe, scrGetStrPara(0));
    dds3LinkProcess(work->task, task);
    scrSetIntReturnValue((s32)task);
    return 1;
}

u32 evtCommand_TEX_ON(void) {
    u32 task;
    s32 registered;

    task = scrGetIntPara(0);
    registered = dds3ExistProcess((dds3ProcessID_t *)task);
    if (registered != 0) {
        evtPictureDrawOn((dds3ProcessID_t *)task);
    }
    return 1;
}

u32 evtCommand_TEX_OFF(void) {
    u32 task;
    s32 registered;

    task = scrGetIntPara(0);
    registered = dds3ExistProcess((dds3ProcessID_t *)task);
    if (registered != 0) {
        evtPictureDrawOff((dds3ProcessID_t *)task);
    }
    return 1;
}

extern void evtKillPictureProcess(u32 taskHandle);

u32 evtCommand_TEX_CLS(void) {
    u32 task;
    s32 registered;

    task = scrGetIntPara(0);
    registered = dds3ExistProcess((dds3ProcessID_t *)task);
    if (registered != 0) {
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

INCLUDE_RODATA(const s32, "game/code_00228058", D_003ACAE0);

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
        if (evtCheckEventBgmLoaded(eventId) != 0) {
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
    s32 fade;

    eventId = scrGetIntPara(0);
    fade = scrGetIntPara(1);
    evtStopEventBgm(eventId, fade);
    return 1;
}

extern void btlAdvanceTitleStateWithAudioCleanupTask(void);
extern void p2sdBgmVolDown(void);

u32 evtCommand_BGM_VOL_DOWN(void) {
    p2sdBgmVolDown();
    btlAdvanceTitleStateWithAudioCleanupTask();
    return 1;
}

extern void p2sdBgmVolUp(void);

u32 evtCommand_BGM_VOL_UP(void) {
    p2sdBgmVolUp();
    btlAdvanceTitleStateWithAudioCleanupTask();
    return 1;
}

extern s32 evtPlaySe(s32 eventId, s32 fade);

u32 evtOpcodeSetBgmVolumePan(void) {
    s32 id;
    s32 fade;

    id = scrGetIntPara(0);
    fade = scrGetIntPara(1);
    evtPlaySe(id, fade);
    return 1;
}

extern s32 evtStartBgmBySoundIdAndFade(s32 eventId, s32 fade);

u32 evtCommandStartBgmWithFade(void) {
    s32 eventId;
    s32 fade;

    eventId = scrGetIntPara(0);
    fade = scrGetIntPara(1);
    evtStartBgmBySoundIdAndFade(eventId, fade);
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


#define SOLAR_PHASE_ADVANCE_FLAG 1
#define SOLAR_ALPHA_ENABLED_FLAG 2

/* Fold phases 9-15 back toward zero for the symmetric solar animation. */
s32 evtGetMirroredSolarPhase(void) {
    s32 phase;

    phase = DDS3_GBWK->world.phase;
    if (phase >= 9) {
        phase = 8 - (phase & 7);
    }
    return phase;
}

s32 evtGetAgeOfMoon16(void) {
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
    if ((state->flags & 2) != 0) {
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
    if ((DDS3_GBWK->world.flags & 2) != 0) {
        evtAdvanceSolarOverlayFadeAndDraw(0, 0, 1, (s32)(alpha * 128.0f), overlay, 0x53);
        rsetOtOfs(0x53);
        rsetAlpha(0);
        rsetTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    } else if (alpha > 0.0f) {
        evtAdvanceSolarOverlayFadeAndDraw(0, 0, 1, (s32)(alpha * 128.0f), overlay, 0x53);
    }
    return 0;
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
            if (evtGetMirroredSolarPhase() == 0) {
                fldEsutoMaClearMsg();
                fldRiberaMaClearMsg();
                fldRifutoMaClearMsg();
                fldLightMaClearMsg();
            }
            if (evtBitCheck(0x819)) {
                DDS3_GBWK->world.secondaryPhase++;
                if (DDS3_GBWK->world.secondaryPhase >= 81) {
                    evtBitOff(0x819);
                    DDS3_GBWK->world.secondaryPhase = 0;
                }
            }
        }
    }
}

s32 evtCreateSolarOverlayWork(dds3ProcessID_t *owner) {
    SolarOverlayWork *overlay;

    overlay = sdfChipAllocDirect(sizeof(SolarOverlayWork));
    evtInitializeVisualData(overlay);
    evtLoadSolarNoiseSprite(&overlay->noiseSprite);
    dds3SetProcessData(owner, (u32)overlay);
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
void evtStartSolarOverlay(void) {
    if (moonprocess_id != 0) {
        return;
    }
    moonprocess_id = dds3StartProcess(D_003BBDC8, 0x2b0b, 1, 1, evtCreateSolarOverlayWork, evtFreeSolarOverlayWork, 0);
    evtEnableMoonCalc();
    evtDisableMoonDraw();
    evtSetAgeOfMoon(0);
    DDS3_GBWK->world.secondaryPhase = 0;
}

void evtStopSolarOverlay(void) {
    if (moonprocess_id != 0) {
        dds3KillProcess(moonprocess_id, 1);
    }
}

INCLUDE_SDATA(const s32, "game/code_00228058", gMoonCnt);

INCLUDE_SDATA(const s32, "game/code_00228058", D_003BBDBC);

INCLUDE_SDATA(const s32, "game/code_00228058", moonprocess_id);

INCLUDE_SDATA(const s32, "game/code_00228058", D_003BBDC8);

