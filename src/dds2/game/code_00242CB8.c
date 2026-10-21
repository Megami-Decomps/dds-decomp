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

extern void fldSetFadeTarget(s32 area, s32 targetMode, s32 duration);
extern void evtBeginSkyParameterTransition(s32 duration, s32 target);

extern s32 scrReadIntParameter(s32 idx);



extern f32 evtSolarOverlayAlpha;

extern u32 kwlnDrawControlFlags;




extern char D_00437208[];

extern char D_00422050[];





extern s32 fileMenuTaskExists(void);
extern u32 fldIsUpdateInhibited(void);
#include "sdf_debug_api.h"
void evtPrintDeveloperConsoleMessage(const char *fmt, ...);
extern char D_00422030[];
extern char D_00421FE8[];
s32 evtPreloadBgm(s32 id);
s32 evtIsBgmLoaded(s32 id);

extern void *evtFindWorldObjectByIdAndKind(s32 objectKind, s32 worldObjectKey);
extern s32 evtCheckWorldObjectResourceScale(void *object);

s32 evtCommandWaitLightResourceScale(void) {
    s32 status = evtCheckWorldObjectResourceScale(
        evtFindWorldObjectByIdAndKind(9, scrReadIntParameter(0)));

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

u32 evtCreateTextureEntryChildTask(void) {
    ScrData *work;
    s32 id;
    struct SdfTex *entry;
    KwlnTask *task;

    work = scrGetCurrentContext();
    if (work == NULL) {
        return 1;
    }
    if (work->task == NULL) {
        sdfDebugPrintf(D_00421FE8);
        return 1;
    }
    id = scrReadIntParameter(0);
    entry = evtFindTaskResourceEntryByKey(id, scrReadIntParameter(1));
    if (entry == 0) {
        evtPrintDeveloperConsoleMessage(D_00422030, scrReadIntParameter(1));
        return 1;
    }
    task = evtCreateTaskWithValue(0x2AFE, entry);
    dds3LinkProcess(work->task, task);
    scrSetIntegerReturnValue((s32)task);
    return 1;
}

u32 evtOpcodeCreateWorldChildTask(void) {
    ScrData *work;
    KwlnTask *childTask;

    work = scrGetCurrentContext();
    if (work == NULL) {
        return 1;
    }
    if (work->task == NULL) {
        sdfDebugPrintf(D_00422050);
        return 1;
    }
    childTask = evtCreateTask(0x2afe, scrReadStringParameter(0));
    dds3LinkProcess(work->task, childTask);
    scrSetIntegerReturnValue((s32)childTask);
    return 1;
}

u32 evtOpcodeSetTaskContextFlag(void) {
    u32 task;
    s32 registered;

    task = scrReadIntParameter(0);
    registered = dds3ExistProcess((KwlnTask *)task);
    if (registered != 0) {
        evtSetContextFlag((KwlnTask *)task);
    }
    return 1;
}

u32 evtOpcodeClearTaskContextFlag(void) {
    u32 task;
    s32 registered;

    task = scrReadIntParameter(0);
    registered = dds3ExistProcess((KwlnTask *)task);
    if (registered != 0) {
        evtClearContextFlag((KwlnTask *)task);
    }
    return 1;
}

extern void evtDestroyTaskHierarchy(u32 taskHandle);

u32 evtOpcodeDestroyTask(void) {
    u32 task;
    s32 registered;

    task = scrReadIntParameter(0);
    registered = dds3ExistProcess((KwlnTask *)task);
    if (registered != 0) {
        evtDestroyTaskHierarchy(task);
    }
    return 1;
}

extern void fldSetSkyDrawState(u32 drawState);
extern void fldSetSwayMode(u32 mode);

u32 evtOpcodeSetFadeTarget(void) {
    s32 area;
    s32 target;
    s32 duration;

    fldSetSwayMode(0);
    fldSetSkyDrawState(0x80);
    area = scrReadIntParameter(0);
    target = scrReadIntParameter(1);
    duration = scrReadIntParameter(2);
    fldSetFadeTarget(area, target, duration);
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00242CB8", D_00421FE8);

INCLUDE_RODATA(const s32, "game/code_00242CB8", D_00422030);

INCLUDE_RODATA(const s32, "game/code_00242CB8", D_00422050);

u32 evtSetSkyAlpha(void) {
    s32 alpha;

    alpha = scrReadIntParameter(1);
    if (alpha < -255) {
        evtPrintDeveloperConsoleMessage("warning : SET_SKY_A alpha < -255\n");
        alpha = -255;
    }
    if (alpha > 255) {
        evtPrintDeveloperConsoleMessage("warning : SET_SKY_A alpha > 255\n");
        alpha = 255;
    }
    evtBeginSkyParameterTransition(scrReadIntParameter(0), alpha);
    return 1;
}

extern void evtSetSkyOverlayEnabled(u32 enabled);

u32 evtShowSky(void) {
    evtSetSkyOverlayEnabled(1);
    return 1;
}

u32 evtHideSky(void) {
    evtSetSkyOverlayEnabled(0);
    return 1;
}

u32 evtCommandAwaitBgmPreload(void) {
    s32 eventId;

    /* One parameter call per arm; retail has two call sites and sharing one changes the code. */
    if (scrGetCommandTimer() == 0) {
        evtPreloadBgm(scrReadIntParameter(0));
    } else {
        eventId = scrReadIntParameter(0);
        if (evtIsBgmLoaded(eventId) != 0) {
            return 1;
        }
    }
    return 0;
}

extern s32 evtPlayBgm(s32 eventId, s32 fade);

u32 evtOpcodePlayBgm(void) {
    s32 eventId;
    s32 fade;

    eventId = scrReadIntParameter(0);
    fade = scrReadIntParameter(1);
    evtPlayBgm(eventId, fade);
    return 1;
}

extern void evtTransitionBgm(s32 eventId, s32 fade);

u32 evtOpcodeTransitionBgm(void) {
    s32 eventId;
    s32 fade;

    eventId = scrReadIntParameter(0);
    fade = scrReadIntParameter(1);
    evtTransitionBgm(eventId, fade);
    return 1;
}

extern s32 evtQueueValidatedBgmSoundCode(s32 eventId, s32 fade);

u32 evtCommandQueueBgmWithFade(void) {
    s32 eventId;
    s32 second;

    eventId = scrReadIntParameter(0);
    second = scrReadIntParameter(1);
    evtQueueValidatedBgmSoundCode(eventId, second);
    return 1;
}

extern void sndStartReducedBgm(void);

u32 evtOpcodeStartReducedBgm(void) {
    sndStartReducedBgm();
    return 1;
}

extern void sndStopReducedBgm(void);

u32 evtOpcodeStopReducedBgm(void) {
    sndStopReducedBgm();
    return 1;
}

extern s32 evtSetBgmVolumePan(s32 eventId, s32 fade);

u32 evtOpcodeSetBgmVolumePan(void) {
    s32 volume;
    s32 pan;

    volume = scrReadIntParameter(0);
    pan = scrReadIntParameter(1);
    evtSetBgmVolumePan(volume, pan);
    return 1;
}

extern s32 evtStartBgmBySoundIdAndFade(s32 eventId, s32 fade);

u32 evtCommandStartBgmWithFade(void) {
    s32 eventId;
    s32 second;

    eventId = scrReadIntParameter(0);
    second = scrReadIntParameter(1);
    evtStartBgmBySoundIdAndFade(eventId, second);
    return 1;
}

extern void evtRefreshTaskEffectTexture(s32 taskId, s32 key);

u32 evtOpcodeRefreshTaskData(void) {
    s32 task;
    s32 value;

    task = scrReadIntParameter(0);
    value = scrReadIntParameter(1);
    evtRefreshTaskEffectTexture(task, value);
    return 1;
}

extern void effInitCh72Id(void);

u32 evtOpcodeInitializeEffectSoundChannel(void) {
    effInitCh72Id();
    return 1;
}

u32 evtCommandSetDrawFlagWhileWaiting(void) {
    if (scrReadIntParameter(0) <= 0) {
        kwlnDrawControlFlags |= 0x2000000;
        return 1;
    }
    if ((s32)scrGetCommandTimer() < scrReadIntParameter(0)) {
        kwlnDrawControlFlags |= 0x2000000;
        return 0;
    }
    return 1;
}



void evtClearSolarOverlayControl(void) {
    datGameState->world.overlayFlag = 0;
}

/* Reset the field-transition expiry counter at world offset 0xA4C. */
void evtResetFieldTransitionCounter(void) {
    datGameState->world.fieldTransitionCycleCount = 0;
}

/* Fold phases 9-15 back toward zero for the symmetric solar animation. */
s32 evtGetMirroredSolarPhase(void) {
    s32 phase;

    phase = datGameState->world.phase;
    if (phase >= 9) {
        phase = 8 - (phase & 7);
    }
    return phase;
}

s32 evtGetSolarPhase(void) {
    return datGameState->world.phase;
}

void evtSetSolarPhase(s32 phase) {
    datGameState->world.phase = phase & 0xf;
    datGameState->world.phaseTimer = 0;
}

void evtEnableSolarPhaseAdvance(void) {
    datGameState->world.flags = datGameState->world.flags | 1;
}

void evtDisableSolarPhaseAdvance(void) {
    datGameState->world.flags = datGameState->world.flags & 0xfe;
}

void evtSetSolarOverlayFullyVisible(void) {
    datGameState->world.flags = datGameState->world.flags | 2;
    evtSolarOverlayAlpha = 1.0f;
    evtBeginSolarOverlayFadeIn(0);
}

void evtSetSolarOverlayFullyTransparent(void) {
    datGameState->world.flags = datGameState->world.flags & 0xfd;
    evtSolarOverlayAlpha = 0.0f;
}

void evtEnableSolarOverlayAlpha(void) {
    datGameState->world.flags = datGameState->world.flags | 2;
}

void evtDisableSolarOverlayAlpha(void) {
    datGameState->world.flags = datGameState->world.flags & 0xfd;
}

s32 evtUpdateSolarOverlayFade(KwlnTask *task) {
    DatWorldState *state;
    SolarOverlayWork *overlay;
    f32 alpha;
    f32 f;

    f = 0.0f;
    if ((fileMenuTaskExists() != 0) || (fldIsUpdateInhibited() != 0)) {
        return 0;
    }
    overlay = (SolarOverlayWork *)dds3GetProcessData(task);
    state = &datGameState->world;
    alpha = evtSolarOverlayAlpha;
    if ((state->flags & 2) != 0) {
        if (alpha < 1.0f) {
            alpha += 0.1f;
            if (alpha > 1.0f) {
                alpha = 1.0f;
            }
            evtSolarOverlayAlpha = alpha;
        }
    } else {
        if (alpha > f) {
            alpha -= 0.1f;
            if (alpha < f) {
                alpha = f;
            }
            evtSolarOverlayAlpha = alpha;
        }
    }
    /* Two jal sites in retail: the DDS1 twin issues extra draw calls in the first arm, removed here. The flags are re-read from the global; the cached state copy does not match. */
    if ((datGameState->world.flags & 2) != 0) {
        evtAdvanceSolarOverlayFadeAndDraw(0, 0, 1, (s32)(alpha * 128.0f), overlay, 0x53);
    } else if (alpha > 0.0f) {
        evtAdvanceSolarOverlayFadeAndDraw(0, 0, 1, (s32)(alpha * 128.0f), overlay, 0x53);
    }
    return 0;
}

extern s32 D_004371FC;
extern void fldConsumePrimarySceneFlag(void);
extern void fldConsumeSecondarySceneFlag(void);
extern void fldConsumeFieldTransitionFlag(void);
extern void fldConsumeSceneCommandFlag(void);

/* Advance the enabled solar clock and expire its associated field controls. */
void evtAdvanceSolarClockAndExpireControls(f32 travelDistance) {
    if (datGameState->world.flags & 1) {
        datGameState->world.phaseTimer += travelDistance;
        if (datGameState->world.phaseTimer > 4500.0f) {
            datGameState->world.phaseTimer = 0;
            evtSetSolarPhase((datGameState->world.phase + 1) & 0xF);
            D_004371FC = 30;
            if (evtGetMirroredSolarPhase() == 0) {
                fldConsumePrimarySceneFlag();
                fldConsumeSecondarySceneFlag();
            }
            if (datGameState->world.fieldFlags & 4) {
                datGameState->world.fieldTransitionCycleCount++;
                if (datGameState->world.fieldTransitionCycleCount >= 6U) {
                    fldConsumeFieldTransitionFlag();
                    datGameState->world.fieldTransitionCycleCount = 0;
                }
            }
            if (datGameState->world.fieldFlags & 8) {
                datGameState->world.overlayFlag++;
                if (datGameState->world.overlayFlag >= 6) {
                    fldConsumeSceneCommandFlag();
                    datGameState->world.overlayFlag = 0;
                }
            }
            if (mdlFlagTest(0x802)) {
                datGameState->world.secondaryPhase++;
                if (datGameState->world.secondaryPhase >= 81) {
                    mdlFlagClear(0x802);
                    datGameState->world.secondaryPhase = 0;
                }
            }
            datGameState->world.specialEncounterAttemptCount = 0;
        }
    }
}



s32 evtCreateSolarOverlayWork(KwlnTask *task) {
    SolarOverlayWork *overlay;

    overlay = sdfChipAllocDirect(0x104);
    evtInitializeVisualData(overlay);
    evtLoadSolarNoiseSprite(&overlay->noiseSprite);
    dds3SetProcessData(task, (u32)overlay);
    return (s32)evtUpdateSolarOverlayFade;
}

void evtFreeSolarOverlayWork(KwlnTask *task) {
    SolarOverlayWork *overlay;

    overlay = (SolarOverlayWork *)dds3GetProcessData(task);
    evtReleaseSolarNoiseSprite(&overlay->noiseSprite);
    sdfChipFreeDirect(overlay);
    evtSolarOverlayTask = 0;
}

/* One scheduler task owns the overlay; a second start leaves its state intact. */
void evtEnsureSolarOverlayTaskAndResetPhase(void) {
    if (evtSolarOverlayTask != 0) {
        return;
    }
    evtSolarOverlayTask = dds3StartProcess(D_00437208, 0x2B0B, 1, 1, evtCreateSolarOverlayWork, evtFreeSolarOverlayWork, 0);
    evtEnableSolarPhaseAdvance();
    evtSetSolarOverlayFullyTransparent();
    evtSetSolarPhase(0);
    datGameState->world.secondaryPhase = 0;
    datGameState->world.overlayFlag = 0;
    datGameState->world.fieldTransitionCycleCount = 0;
}

void evtStopSolarOverlay(void) {
    if (evtSolarOverlayTask != 0) {
        dds3KillProcess(evtSolarOverlayTask, 1);
    }
}

INCLUDE_SDATA(const s32, "game/code_00242CB8", evtSolarOverlayAlpha);

INCLUDE_SDATA(const s32, "game/code_00242CB8", D_004371FC);

INCLUDE_SDATA(const s32, "game/code_00242CB8", evtSolarOverlayTask);

INCLUDE_SDATA(const s32, "game/code_00242CB8", D_00437208);

