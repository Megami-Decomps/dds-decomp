#include "common.h"
#include "dat_state.h"
#include "scr.h"
#include "kwln.h"
#include "evt_world.h"
#include "evt_solar.h"

extern s32 kwlnTaskIsRegistered(KwlnTask *);
extern void evtSetContextFlag(KwlnTask *);
extern void evtClearContextFlag(KwlnTask *);
extern void fldSetFadeTarget(s32, s32, s32);
extern void evtBeginSkyParameterTransition(s32, s32);

extern s32 scrReadIntParameter(s32 idx);



extern f32 evtSolarOverlayAlpha;

extern u32 kwlnDrawControlFlags;

extern s32 scrGetCommandTimer(void);


u32 kwlnTaskGetUserValue(KwlnTask *task);

extern char D_00437208[];

extern KwlnTask *kwlnTaskCreate(const char *, u32, s32, s32, TaskUpdate, TaskDestroy, u32);

void func_00101968(KwlnTask *parent, KwlnTask *child);


char *scrReadStringParameter(s32 idx);

void scrSetIntegerReturnValue(s32 value);

extern char D_00422050[];

void *sdfAllocSizeClassBlock(s32 size);
void sdfReleaseChipBlock(void *memory);


void kwlnTaskSetUserValue(KwlnTask *task, u32 value);


void evtBeginSolarOverlayFadeIn(s32 arg0);

extern s32 kwlnTaskDestroyWithHierarchy(KwlnTask *task, s32 delayTicks);
extern s32 fileMenuTaskExists(void);
extern u32 func_001200E0(void);
void func_0035B6E0(const char *fmt, ...);
void evtPrintDeveloperConsoleMessage(const char *fmt, ...);
extern char D_00422030[];
extern char D_00421FE8[];
void *evtFindTaskResourceEntryByKey(u32, s32);
KwlnTask *evtCreateTaskWithValue(s32, struct SdfTex *);
KwlnTask *evtCreateTask(s32, const char *);
s32 evtPreloadBgm(s32 id);
s32 evtIsBgmLoaded(s32 id);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00242CB8);

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
        func_0035B6E0(D_00421FE8);
        return 1;
    }
    id = scrReadIntParameter(0);
    entry = evtFindTaskResourceEntryByKey(id, scrReadIntParameter(1));
    if (entry == 0) {
        evtPrintDeveloperConsoleMessage(D_00422030, scrReadIntParameter(1));
        return 1;
    }
    task = evtCreateTaskWithValue(0x2AFE, entry);
    func_00101968(work->task, task);
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
        func_0035B6E0(D_00422050);
        return 1;
    }
    childTask = evtCreateTask(0x2afe, scrReadStringParameter(0));
    func_00101968(work->task, childTask);
    scrSetIntegerReturnValue((s32)childTask);
    return 1;
}

u32 evtOpcodeSetTaskContextFlag(void) {
    u32 task;
    s32 registered;

    task = scrReadIntParameter(0);
    registered = kwlnTaskIsRegistered((KwlnTask *)task);
    if (registered != 0) {
        evtSetContextFlag((KwlnTask *)task);
    }
    return 1;
}

u32 evtOpcodeClearTaskContextFlag(void) {
    u32 task;
    s32 registered;

    task = scrReadIntParameter(0);
    registered = kwlnTaskIsRegistered((KwlnTask *)task);
    if (registered != 0) {
        evtClearContextFlag((KwlnTask *)task);
    }
    return 1;
}

u32 evtOpcodeDestroyTask(void) {
    u32 task;
    s32 registered;

    task = scrReadIntParameter(0);
    registered = kwlnTaskIsRegistered((KwlnTask *)task);
    if (registered != 0) {
        evtDestroyTaskHierarchy(task);
    }
    return 1;
}

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

u32 evtShowSky(void) {
    evtSetSkyOverlayEnabled(1);
    return 1;
}

u32 evtHideSky(void) {
    evtSetSkyOverlayEnabled(0);
    return 1;
}

u32 evtCommandAwaitBgmPreload(void) {
    s32 id;

    /* One parameter call per arm; retail has two call sites and sharing one changes the code. */
    if (scrGetCommandTimer() == 0) {
        evtPreloadBgm(scrReadIntParameter(0));
    } else {
        id = scrReadIntParameter(0);
        if (evtIsBgmLoaded(id) != 0) {
            return 1;
        }
    }
    return 0;
}

u32 evtOpcodePlayBgm(void) {
    u64 id;
    u64 fade;

    id = scrReadIntParameter(0);
    fade = scrReadIntParameter(1);
    evtPlayBgm(id, fade);
    return 1;
}

u32 evtOpcodeTransitionBgm(void) {
    u64 id;
    u64 fade;

    id = scrReadIntParameter(0);
    fade = scrReadIntParameter(1);
    evtTransitionBgm(id, fade);
    return 1;
}

u32 evtCommandQueueBgmWithFade(void) {
    u64 first;
    u64 second;

    first = scrReadIntParameter(0);
    second = scrReadIntParameter(1);
    evtQueueValidatedBgmSoundCode(first, second);
    return 1;
}

u32 func_00243180(void) {
    func_003425B0();
    return 1;
}

u32 func_002431A0(void) {
    func_003425D8();
    return 1;
}

u32 evtOpcodeSetBgmVolumePan(void) {
    u64 volume;
    u64 pan;

    volume = scrReadIntParameter(0);
    pan = scrReadIntParameter(1);
    evtSetBgmVolumePan(volume, pan);
    return 1;
}

u32 evtCommandStartBgmWithFade(void) {
    u64 first;
    u64 second;

    first = scrReadIntParameter(0);
    second = scrReadIntParameter(1);
    evtStartBgmBySoundIdAndFade(first, second);
    return 1;
}

u32 evtOpcodeRefreshTaskData(void) {
    u64 task;
    u64 value;

    task = scrReadIntParameter(0);
    value = scrReadIntParameter(1);
    evtRefreshTaskEffectTexture(task, value);
    return 1;
}

u32 evtOpcodeInitializeEffectSoundChannel(void) {
    effInitCh72Id();
    return 1;
}

u32 evtCommandSetDrawFlagWhileWaiting(void) {
    if (scrReadIntParameter(0) <= 0) {
        kwlnDrawControlFlags |= 0x2000000;
        return 1;
    }
    if (scrGetCommandTimer() < scrReadIntParameter(0)) {
        kwlnDrawControlFlags |= 0x2000000;
        return 0;
    }
    return 1;
}



void evtClearSolarOverlayControl(void) {
    datGameState->world.overlayFlag = 0;
}

void func_00243320(void) {
    datGameState->world.unkA4C = 0;
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

void evtSetSolarPhase(u8 phase) {
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
    if ((fileMenuTaskExists() != 0) || (func_001200E0() != 0)) {
        return 0;
    }
    overlay = (SolarOverlayWork *)kwlnTaskGetUserValue(task);
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
extern s32 mdlFlagTest(s32);
extern void mdlFlagClear(s32);

/* Advance the enabled solar clock and expire its associated field controls. */
void func_00243568(f32 delta) {
    if (datGameState->world.flags & 1) {
        datGameState->world.phaseTimer += delta;
        if (datGameState->world.phaseTimer > 4500.0f) {
            datGameState->world.phaseTimer = 0;
            evtSetSolarPhase((datGameState->world.phase + 1) & 0xF);
            D_004371FC = 30;
            if (evtGetMirroredSolarPhase() == 0) {
                fldConsumePrimarySceneFlag();
                fldConsumeSecondarySceneFlag();
            }
            if (datGameState->world.fieldFlags & 4) {
                datGameState->world.unkA4C++;
                if (datGameState->world.unkA4C >= 6U) {
                    fldConsumeFieldTransitionFlag();
                    datGameState->world.unkA4C = 0;
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
            datGameState->world.unkA48 = 0;
        }
    }
}



s32 evtCreateSolarOverlayWork(KwlnTask *task) {
    SolarOverlayWork *overlay;

    overlay = sdfAllocSizeClassBlock(0x104);
    evtInitializeVisualData(overlay);
    evtLoadSolarNoiseSprite(&overlay->noiseSprite);
    kwlnTaskSetUserValue(task, (u32)overlay);
    return (s32)evtUpdateSolarOverlayFade;
}

void evtFreeSolarOverlayWork(KwlnTask *task) {
    SolarOverlayWork *overlay;

    overlay = (SolarOverlayWork *)kwlnTaskGetUserValue(task);
    evtReleaseSolarNoiseSprite(&overlay->noiseSprite);
    sdfReleaseChipBlock(overlay);
    evtSolarOverlayTask = 0;
}

/* One scheduler task owns the overlay; a second start leaves its state intact. */
void evtEnsureSolarOverlayTaskAndResetPhase(void) {
    if (evtSolarOverlayTask != 0) {
        return;
    }
    evtSolarOverlayTask = kwlnTaskCreate(D_00437208, 0x2B0B, 1, 1, evtCreateSolarOverlayWork, evtFreeSolarOverlayWork, 0);
    evtEnableSolarPhaseAdvance();
    evtSetSolarOverlayFullyTransparent();
    evtSetSolarPhase(0);
    datGameState->world.secondaryPhase = 0;
    datGameState->world.overlayFlag = 0;
    datGameState->world.unkA4C = 0;
}

void evtStopSolarOverlay(void) {
    if (evtSolarOverlayTask != 0) {
        kwlnTaskDestroyWithHierarchy(evtSolarOverlayTask, 1);
    }
}

INCLUDE_SDATA(const s32, "game/code_00242CB8", evtSolarOverlayAlpha);

INCLUDE_SDATA(const s32, "game/code_00242CB8", D_004371FC);

INCLUDE_SDATA(const s32, "game/code_00242CB8", evtSolarOverlayTask);

INCLUDE_SDATA(const s32, "game/code_00242CB8", D_00437208);

