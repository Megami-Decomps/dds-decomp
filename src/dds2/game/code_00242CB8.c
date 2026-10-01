#include "common.h"

extern s64 kwlnTaskIsRegistered(u64);

extern s32 scrReadIntParameter(s32 idx);

/* Event-command work prefix; the +0xE4 key selects the parent field task. */
typedef struct {
    u8 pad00[0xE4];
    u32 taskKey;
} EvtCommandWork;


extern f32 D_004371F8;

extern u32 D_00435CD4;

extern s32 scrGetCommandTimer(void);

extern u32 D_00437200;

u32 kwlnTaskGetUserValue(s32 task);

extern char D_00437208[];

extern s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

void func_00101968(s32 arg0, s32 arg1);

s32 func_0010D8C8(void);

char *scrReadStringParameter(s32 idx);

void scrSetIntegerReturnValue(s32 value);

extern char D_00422050[];

void *func_00328D68(s32 size);

void evtInitializeVisualData(s32 arg0);

void kwlnTaskSetUserValue(s32 arg0, void *arg1);

u32 func_00243430(s32 task);

void func_002457B8(s32 arg0);

extern s32 kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);
extern s32 fileMenuTaskExists(void);
extern s32 func_001200E0(void);
extern void func_00245618(s32 a0, s32 a1, s32 a2, s32 a3, u32 overlay, s32 a5);
void func_0035B6E0(const char *fmt, ...);
void func_0010AE38(const char *fmt, ...);
extern char D_00422030[];
extern char D_00421FE8[];
s32 evtFindTaskResourceEntryByKey(u32 id, s32 key);
s32 evtCreateTaskWithValue(s32 taskId, s32 value);
s32 evtPreloadBgm(s32 id);
s32 evtIsBgmLoaded(s32 id);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00242CB8);

u32 func_00242D10(void) {
    EvtCommandWork *work;
    s32 id;
    s32 entry;
    s32 task;

    work = (EvtCommandWork *)func_0010D8C8();
    if (work == NULL) {
        return 1;
    }
    if (work->taskKey == 0) {
        func_0035B6E0(D_00421FE8);
        return 1;
    }
    id = scrReadIntParameter(0);
    entry = evtFindTaskResourceEntryByKey(id, scrReadIntParameter(1));
    if (entry == 0) {
        func_0010AE38(D_00422030, scrReadIntParameter(1));
        return 1;
    }
    task = evtCreateTaskWithValue(0x2AFE, entry);
    func_00101968(work->taskKey, task);
    scrSetIntegerReturnValue(task);
    return 1;
}

u32 evtOpcodeCreateWorldChildTask(void) {
    EvtCommandWork *work;
    s32 childTask;

    work = (EvtCommandWork *)func_0010D8C8();
    if (work == NULL) {
        return 1;
    }
    if (work->taskKey == 0) {
        func_0035B6E0(D_00422050);
        return 1;
    }
    childTask = evtCreateTask(0x2afe, scrReadStringParameter(0));
    func_00101968(work->taskKey, childTask);
    scrSetIntegerReturnValue(childTask);
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

INCLUDE_RODATA(const s32, "game/code_00242CB8", D_00421FE8);

INCLUDE_RODATA(const s32, "game/code_00242CB8", D_00422030);

INCLUDE_RODATA(const s32, "game/code_00242CB8", D_00422050);

u32 evtSetSkyAlpha(void) {
    s64 alpha;

    alpha = scrReadIntParameter(1);
    if (alpha < -255) {
        func_0010AE38("warning : SET_SKY_A alpha < -255\n");
        alpha = -255;
    }
    if (alpha > 255) {
        func_0010AE38("warning : SET_SKY_A alpha > 255\n");
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

u32 func_00243068(void) {
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

u32 func_00243140(void) {
    u64 first;
    u64 second;

    first = scrReadIntParameter(0);
    second = scrReadIntParameter(1);
    func_0025CE68(first, second);
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

u32 func_00243200(void) {
    u64 first;
    u64 second;

    first = scrReadIntParameter(0);
    second = scrReadIntParameter(1);
    func_0025CF00(first, second);
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

u32 func_002432A0(void) {
    if (scrReadIntParameter(0) <= 0) {
        D_00435CD4 |= 0x2000000;
        return 1;
    }
    if (scrGetCommandTimer() < scrReadIntParameter(0)) {
        D_00435CD4 |= 0x2000000;
        return 0;
    }
    return 1;
}

typedef struct {
    u8 pad00[0xA40];
    u8 flags;          /* 0xA40 */
    u8 phase;          /* 0xA41 */
    u8 padA42;          /* 0xA42 */
    u8 overlayFlag;     /* 0xA43: cleared separately from solar phase */
    f32 phaseTimer;    /* 0xA44: cleared on phase change */
    u32 padA48;
    u32 unkA4C;        /* 0xA4C: cleared when the overlay starts */
    u32 padA50;
    u32 padA54;
    u32 extFlags;      /* 0xA58 */
} SolarWorldState;

extern SolarWorldState *D_00435DD0;

void evtClearSolarOverlayControl(void) {
    D_00435DD0->overlayFlag = 0;
}

void func_00243320(void) {
    D_00435DD0->unkA4C = 0;
}

/* Fold phases 9-15 back toward zero for the symmetric solar animation. */
s32 evtGetMirroredSolarPhase(void) {
    s32 phase;

    phase = D_00435DD0->phase;
    if (phase >= 9) {
        phase = 8 - (phase & 7);
    }
    return phase;
}

u8 evtGetSolarPhase(void) {
    return D_00435DD0->phase;
}

void evtSetSolarPhase(u8 phase) {
    D_00435DD0->phase = phase & 0xf;
    D_00435DD0->phaseTimer = 0;
}

void evtEnableSolarPhaseAdvance(void) {
    D_00435DD0->flags = D_00435DD0->flags | 1;
}

void evtDisableSolarPhaseAdvance(void) {
    D_00435DD0->flags = D_00435DD0->flags & 0xfe;
}

void evtSetSolarOverlayFullyVisible(void) {
    D_00435DD0->flags = D_00435DD0->flags | 2;
    D_004371F8 = 1.0f;
    func_002457B8(0);
}

void evtSetSolarOverlayFullyTransparent(void) {
    D_00435DD0->flags = D_00435DD0->flags & 0xfd;
    D_004371F8 = 0.0f;
}

void evtEnableSolarOverlayAlpha(void) {
    D_00435DD0->flags = D_00435DD0->flags | 2;
}

void evtDisableSolarOverlayAlpha(void) {
    D_00435DD0->flags = D_00435DD0->flags & 0xfd;
}

u32 func_00243430(s32 task) {
    SolarWorldState *state;
    u32 overlay;
    f32 alpha;
    f32 f;

    f = 0.0f;
    if ((fileMenuTaskExists() != 0) || (func_001200E0() != 0)) {
        return 0;
    }
    overlay = kwlnTaskGetUserValue(task);
    state = D_00435DD0;
    alpha = D_004371F8;
    if ((state->flags & 2) != 0) {
        if (alpha < 1.0f) {
            alpha += 0.1f;
            if (alpha > 1.0f) {
                alpha = 1.0f;
            }
            D_004371F8 = alpha;
        }
    } else {
        if (alpha > f) {
            alpha -= 0.1f;
            if (alpha < f) {
                alpha = f;
            }
            D_004371F8 = alpha;
        }
    }
    /* Two jal sites in retail: the DDS1 twin issues extra draw calls in the first arm, removed here. The flags are re-read from the global; the cached state copy does not match. */
    if ((D_00435DD0->flags & 2) != 0) {
        func_00245618(0, 0, 1, (s32)(alpha * 128.0f), overlay, 0x53);
    } else if (alpha > 0.0f) {
        func_00245618(0, 0, 1, (s32)(alpha * 128.0f), overlay, 0x53);
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

    overlay = func_00328D68(0x104);
    evtInitializeVisualData((s32)overlay);
    evtLoadSolarNoiseSprite(&overlay->noiseSprite);
    kwlnTaskSetUserValue(task, overlay);
    return (void *)func_00243430;
}

void evtFreeSolarOverlayWork(s32 task) {
    u32 overlay;

    overlay = kwlnTaskGetUserValue(task);
    evtReleaseSolarNoiseSprite(overlay);
    sdfReleaseChipBlock(overlay);
    D_00437200 = 0;
}

/* One scheduler task owns the overlay; a second start leaves its state intact. */
void func_00243740(void) {
    if (D_00437200 != 0) {
        return;
    }
    D_00437200 = kwlnTaskCreate((s32)D_00437208, 0x2B0B, 1, 1, (s32)evtCreateSolarOverlayWork, (s32)evtFreeSolarOverlayWork, 0);
    evtEnableSolarPhaseAdvance();
    evtSetSolarOverlayFullyTransparent();
    evtSetSolarPhase(0);
    D_00435DD0->padA42 = 0;
    D_00435DD0->overlayFlag = 0;
    D_00435DD0->unkA4C = 0;
}

void evtStopSolarOverlay(void) {
    if (D_00437200 != 0) {
        kwlnTaskDestroyWithHierarchy(D_00437200, 1);
    }
}

INCLUDE_SDATA(const s32, "game/code_00242CB8", D_004371F8);

INCLUDE_SDATA(const s32, "game/code_00242CB8", D_004371FC);

INCLUDE_SDATA(const s32, "game/code_00242CB8", D_00437200);

INCLUDE_SDATA(const s32, "game/code_00242CB8", D_00437208);

