#include "common.h"
#include "dat_state.h"
#include "scr.h"


/* The first word is the solarnoise.spr handle; the work allocation is 0x104 bytes. */
typedef struct SolarOverlayWork {
    u32 noiseSprite;
    u8 pad04[0x100];
} SolarOverlayWork;

extern s32 kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);

void *sdfAllocSizeClassBlock(s32 size);

void evtInitializeVisualData(s32 arg0);
void evtLoadSolarNoiseSprite(u32 *sprite);

void kwlnTaskSetUserValue(s32 arg0, void *arg1);

void func_00101A80(s32 arg0, s32 arg1);


char *scrReadStringParameter(s32 idx);

void scrSetIntegerReturnValue(s32 value);

extern char D_003ACAE0[];

u32 evtUpdateSolarOverlayFade(s32 task);
extern s32 fileMenuTaskExists(void);
extern s32 func_0011E278(void);
extern void evtAdvanceSolarOverlayFadeAndDraw(s32 a0, s32 a1, s32 a2, s32 a3, u32 overlay, s32 a5);
extern void fldSelectDisplayBuffer(u32 row);
extern void func_00129900(s32 arg);
extern void fldSubmitFrameQuad(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7);
extern u32 kwlnDrawControlFlags;
extern s32 scrGetCommandTimer(void);
s32 evtPreloadBgm(s32 id);
s32 evtIsBgmLoaded(s32 id);

void evtBeginSolarOverlayFadeIn(s32 arg0);

extern u32 evtSolarOverlayTask;

extern char D_003BBDC8[];

extern s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

u32 kwlnTaskGetUserValue(s32 task);

extern f32 evtSolarOverlayAlpha; /* solar overlay alpha, interpolated toward 0 or 1 */


extern s64 kwlnTaskIsRegistered(u64);

s32 scrReadIntParameter(s32 idx);
INCLUDE_ASM(const s32, "game/code_00228058", func_00228058);

extern char D_003ACA78[];
extern s32 evtFindTaskResourceEntryByKey(u32 id, s32 key);
extern s32 evtCreateTaskWithValue(s32 taskId, s32 value);
void evtPrintDeveloperConsoleMessage(const char *fmt, ...);

INCLUDE_RODATA(const s32, "game/code_00228058", D_003ACA78);

u32 evtCreateTextureEntryChildTask(void) {
    ScrData *work;
    s32 id;
    s32 entry;
    s32 task;

    work = scrGetCurrentContext();
    if (work == NULL) {
        return 1;
    }
    if (work->task == NULL) {
        func_003003F0(D_003ACA78);
        return 1;
    }
    id = scrReadIntParameter(0);
    entry = evtFindTaskResourceEntryByKey(id, scrReadIntParameter(1));
    if (entry == 0) {
        evtPrintDeveloperConsoleMessage("TEX_BE not fount RID = %d\n", scrReadIntParameter(1));
        return 1;
    }
    task = evtCreateTaskWithValue(0x2AFE, entry);
    func_00101A80((s32)work->task, task);
    scrSetIntegerReturnValue(task);
    return 1;
}

u32 evtOpcodeCreateWorldChildTask(void) {
    ScrData *work;
    s32 task;

    work = scrGetCurrentContext();
    if (work == NULL) {
        return 1;
    }
    if (work->task == NULL) {
        func_003003F0(D_003ACAE0);
        return 1;
    }
    task = evtCreateTask(0x2afe, scrReadStringParameter(0));
    func_00101A80((s32)work->task, task);
    scrSetIntegerReturnValue(task);
    return 1;
}

u32 evtOpcodeSetTaskContextFlag(void) {
    u64 task;
    s64 registered;

    task = scrReadIntParameter(0);
    registered = kwlnTaskIsRegistered(task);
    if (registered != 0) {
        evtSetContextFlag(task);
    }
    return 1;
}

u32 evtOpcodeClearTaskContextFlag(void) {
    u64 task;
    s64 registered;

    task = scrReadIntParameter(0);
    registered = kwlnTaskIsRegistered(task);
    if (registered != 0) {
        evtClearContextFlag(task);
    }
    return 1;
}

u32 evtOpcodeDestroyTask(void) {
    u64 task;
    s64 registered;

    task = scrReadIntParameter(0);
    registered = kwlnTaskIsRegistered(task);
    if (registered != 0) {
        evtDestroyTaskHierarchy(task);
    }
    return 1;
}

u32 evtOpcodeSetFadeTarget(void) {
    u64 area;
    u64 target;
    u64 duration;

    fldSetSwayMode(0);
    fldSetSkyDrawState(0x80);
    area = scrReadIntParameter(0);
    target = scrReadIntParameter(1);
    duration = scrReadIntParameter(2);
    fldSetFadeTarget(area, target, duration);
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00228058", D_003ACAE0);

u32 evtSetSkyAlpha(void) {
    s64 alpha;

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
    u64 id;
    u64 fade;

    id = scrReadIntParameter(0);
    fade = scrReadIntParameter(1);
    evtQueueValidatedBgmSoundCode(id, fade);
    return 1;
}

u32 func_00228520(void) {
    func_002E9708();
    btlAdvanceTitleStateWithAudioCleanupTask();
    return 1;
}

u32 func_00228548(void) {
    func_002E9730();
    btlAdvanceTitleStateWithAudioCleanupTask();
    return 1;
}

u32 evtOpcodeSetBgmVolumePan(void) {
    u64 id;
    u64 fade;

    id = scrReadIntParameter(0);
    fade = scrReadIntParameter(1);
    evtSetBgmVolumePan(id, fade);
    return 1;
}

u32 evtCommandStartBgmWithFade(void) {
    u64 id;
    u64 fade;

    id = scrReadIntParameter(0);
    fade = scrReadIntParameter(1);
    evtStartBgmBySoundIdAndFade(id, fade);
    return 1;
}

u32 evtOpcodeRefreshTaskData(void) {
    u64 task;
    u64 value;

    task = scrReadIntParameter(0);
    value = scrReadIntParameter(1);
    evtRefreshTaskData(task, value);
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


#define SOLAR_PHASE_ADVANCE_FLAG 1
#define SOLAR_ALPHA_ENABLED_FLAG 2

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
    datGameState->world.phaseTimer = 0.0f;
}

void evtEnableSolarPhaseAdvance(void) {
    datGameState->world.flags = datGameState->world.flags | SOLAR_PHASE_ADVANCE_FLAG;
}

void evtDisableSolarPhaseAdvance(void) {
    datGameState->world.flags = datGameState->world.flags & ~SOLAR_PHASE_ADVANCE_FLAG;
}

void evtSetSolarOverlayFullyVisible(void) {
    datGameState->world.flags = datGameState->world.flags | SOLAR_ALPHA_ENABLED_FLAG;
    evtSolarOverlayAlpha = 1.0f;
    evtBeginSolarOverlayFadeIn(0);
}

void evtSetSolarOverlayFullyTransparent(void) {
    datGameState->world.flags = datGameState->world.flags & ~SOLAR_ALPHA_ENABLED_FLAG;
    evtSolarOverlayAlpha = 0.0f;
}

void evtEnableSolarOverlayAlpha(void) {
    datGameState->world.flags = datGameState->world.flags | SOLAR_ALPHA_ENABLED_FLAG;
}

void evtDisableSolarOverlayAlpha(void) {
    datGameState->world.flags = datGameState->world.flags & ~SOLAR_ALPHA_ENABLED_FLAG;
}

u32 evtUpdateSolarOverlayFade(s32 task) {
    DatWorldState *state;
    u32 overlay;
    f32 alpha;
    f32 f;

    f = 0.0f;
    if ((fileMenuTaskExists() != 0) || (func_0011E278() != 0)) {
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
    if ((datGameState->world.flags & 2) != 0) {
        evtAdvanceSolarOverlayFadeAndDraw(0, 0, 1, (s32)(alpha * 128.0f), overlay, 0x53);
        fldSelectDisplayBuffer(0x53);
        func_00129900(0);
        fldSubmitFrameQuad(1, 0, 0x80, 3, 0, 0, 1, 1);
    } else if (alpha > 0.0f) {
        evtAdvanceSolarOverlayFadeAndDraw(0, 0, 1, (s32)(alpha * 128.0f), overlay, 0x53);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00228058", func_00228930);

void *evtCreateSolarOverlayWork(s32 owner) {
    SolarOverlayWork *overlay;

    overlay = sdfAllocSizeClassBlock(0x104);
    evtInitializeVisualData((s32)overlay);
    evtLoadSolarNoiseSprite(&overlay->noiseSprite);
    kwlnTaskSetUserValue(owner, overlay);
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
void evtStartSolarOverlay(void) {
    if (evtSolarOverlayTask != 0) {
        return;
    }
    evtSolarOverlayTask = kwlnTaskCreate((s32)D_003BBDC8, 0x2b0b, 1, 1, (s32)evtCreateSolarOverlayWork, (s32)evtFreeSolarOverlayWork, 0);
    evtEnableSolarPhaseAdvance();
    evtSetSolarOverlayFullyTransparent();
    evtSetSolarPhase(0);
    datGameState->world.secondaryPhase = 0;
}

void evtStopSolarOverlay(void) {
    if (evtSolarOverlayTask != 0) {
        kwlnTaskDestroyWithHierarchy(evtSolarOverlayTask, 1);
    }
}

INCLUDE_SDATA(const s32, "game/code_00228058", evtSolarOverlayAlpha);

INCLUDE_SDATA(const s32, "game/code_00228058", D_003BBDBC);

INCLUDE_SDATA(const s32, "game/code_00228058", evtSolarOverlayTask);

INCLUDE_SDATA(const s32, "game/code_00228058", D_003BBDC8);

