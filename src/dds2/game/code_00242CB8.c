#include "common.h"
#include "dat_state.h"
#include "scr.h"
#include "kwln.h"
#include "evt_world.h"

extern s32 kwlnTaskIsRegistered(KwlnTask *);
extern void evtSetContextFlag(KwlnTask *);
extern void evtClearContextFlag(KwlnTask *);
extern void fldSetFadeTarget(s32, s32, s32);
extern void evtBeginSkyParameterTransition(s32, s32);

extern s32 scrReadIntParameter(s32 idx);



extern f32 evtSolarOverlayAlpha;

extern u32 kwlnDrawControlFlags;

extern s32 scrGetCommandTimer(void);

extern u32 evtSolarOverlayTask;

u32 kwlnTaskGetUserValue(s32 task);

extern char D_00437208[];

extern s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

void func_00101968(KwlnTask *parent, KwlnTask *child);


char *scrReadStringParameter(s32 idx);

void scrSetIntegerReturnValue(s32 value);

extern char D_00422050[];

void *sdfAllocSizeClassBlock(s32 size);

void evtInitializeVisualData(s32 arg0);

void kwlnTaskSetUserValue(s32 arg0, void *arg1);

u32 evtUpdateSolarOverlayFade(s32 task);

void evtBeginSolarOverlayFadeIn(s32 arg0);

extern s32 kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);
extern s32 fileMenuTaskExists(void);
extern u32 func_001200E0(void);
extern void evtAdvanceSolarOverlayFadeAndDraw(s32 a0, s32 a1, s32 a2, s32 a3, u32 overlay, s32 a5);
void func_0035B6E0(const char *fmt, ...);
void evtPrintDeveloperConsoleMessage(const char *fmt, ...);
extern char D_00422030[];
extern char D_00421FE8[];
s32 evtFindTaskResourceEntryByKey(u32 id, s32 key);
KwlnTask *evtCreateTaskWithValue(s32, struct SdfTex *);
KwlnTask *evtCreateTask(s32, const char *);
s32 evtPreloadBgm(s32 id);
s32 evtIsBgmLoaded(s32 id);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00242CB8);

u32 evtCreateTextureEntryChildTask(void) {
    ScrData *work;
    s32 id;
    s32 entry;
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
    task = evtCreateTaskWithValue(0x2AFE, (struct SdfTex *)entry);
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

u8 evtGetSolarPhase(void) {
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

u32 evtUpdateSolarOverlayFade(s32 task) {
    DatWorldState *state;
    u32 overlay;
    f32 alpha;
    f32 f;

    f = 0.0f;
    if ((fileMenuTaskExists() != 0) || (func_001200E0() != 0)) {
        return 0;
    }
    overlay = kwlnTaskGetUserValue(task);
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

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00243568);

/* The first word holds the solar-noise sprite; total work size is 0x104. */
typedef struct {
    u32 noiseSprite;
    u8 pad04[0x100];
} SolarOverlayWork;

void *evtCreateSolarOverlayWork(s32 task) {
    SolarOverlayWork *overlay;

    overlay = sdfAllocSizeClassBlock(0x104);
    evtInitializeVisualData((s32)overlay);
    evtLoadSolarNoiseSprite(&overlay->noiseSprite);
    kwlnTaskSetUserValue(task, overlay);
    return (void *)evtUpdateSolarOverlayFade;
}

void evtFreeSolarOverlayWork(s32 task) {
    u32 overlay;

    overlay = kwlnTaskGetUserValue(task);
    evtReleaseSolarNoiseSprite(overlay);
    sdfReleaseChipBlock(overlay);
    evtSolarOverlayTask = 0;
}

/* One scheduler task owns the overlay; a second start leaves its state intact. */
void evtEnsureSolarOverlayTaskAndResetPhase(void) {
    if (evtSolarOverlayTask != 0) {
        return;
    }
    evtSolarOverlayTask = kwlnTaskCreate((s32)D_00437208, 0x2B0B, 1, 1, (s32)evtCreateSolarOverlayWork, (s32)evtFreeSolarOverlayWork, 0);
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

