#include "common.h"

extern s64 kwlnTaskIsRegistered(u64);

extern u64 scrReadIntParameter(u64);

extern s32 D_00435DD0;

extern u32 D_004371F8;

extern u32 D_00437200;

extern u64 kwlnTaskGetUserValue(void);

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

void func_00243430(void);

void func_002457B8(s32 arg0);

extern s32 kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00242CB8);

INCLUDE_RODATA(const s32, "game/code_00242CB8", D_00421FE8);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00242D10);

/* Event-command work prefix; the +0xE4 key selects the parent field task. */
typedef struct {
    u8 pad00[0xE4];
    u32 taskKey;
} EvtCommandWork;

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

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00243068);

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

INCLUDE_ASM(const s32, "game/code_00242CB8", func_002432A0);

typedef struct {
    u8 pad00[0xA40];
    u8 flags;          /* 0xA40 */
    u8 phase;          /* 0xA41 */
    u8 padA42;          /* 0xA42 */
    u8 overlayFlag;     /* 0xA43: cleared separately from solar phase */
    u32 phaseTimer;    /* 0xA44: cleared on phase change */
    u32 padA48;
    u32 unkA4C;        /* 0xA4C: cleared when the overlay starts */
} SolarWorldState;

void evtClearSolarOverlayControl(void) {
    ((SolarWorldState *)D_00435DD0)->overlayFlag = 0;
}

void func_00243320(void) {
    ((SolarWorldState *)D_00435DD0)->unkA4C = 0;
}

/* Fold phases 9-15 back toward zero for the symmetric solar animation. */
s32 evtGetMirroredSolarPhase(void) {
    s32 phase;

    phase = ((SolarWorldState *)D_00435DD0)->phase;
    if (phase >= 9) {
        phase = 8 - (phase & 7);
    }
    return phase;
}

u8 evtGetSolarPhase(void) {
    return ((SolarWorldState *)D_00435DD0)->phase;
}

void evtSetSolarPhase(u8 phase) {
    ((SolarWorldState *)D_00435DD0)->phase = phase & 0xf;
    ((SolarWorldState *)D_00435DD0)->phaseTimer = 0;
}

void evtEnableSolarPhaseAdvance(void) {
    ((SolarWorldState *)D_00435DD0)->flags = ((SolarWorldState *)D_00435DD0)->flags | 1;
}

void evtDisableSolarPhaseAdvance(void) {
    ((SolarWorldState *)D_00435DD0)->flags = ((SolarWorldState *)D_00435DD0)->flags & 0xfe;
}

void evtSetSolarOverlayFullyVisible(void) {
    ((SolarWorldState *)D_00435DD0)->flags = ((SolarWorldState *)D_00435DD0)->flags | 2;
    *(f32 *)&D_004371F8 = 1.0f;
    func_002457B8(0);
}

void evtSetSolarOverlayFullyTransparent(void) {
    ((SolarWorldState *)D_00435DD0)->flags = ((SolarWorldState *)D_00435DD0)->flags & 0xfd;
    D_004371F8 = 0;
}

void evtEnableSolarOverlayAlpha(void) {
    ((SolarWorldState *)D_00435DD0)->flags = ((SolarWorldState *)D_00435DD0)->flags | 2;
}

void evtDisableSolarOverlayAlpha(void) {
    ((SolarWorldState *)D_00435DD0)->flags = ((SolarWorldState *)D_00435DD0)->flags & 0xfd;
}

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00243430);

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

void evtFreeSolarOverlayWork(void) {
    u64 overlay;

    overlay = kwlnTaskGetUserValue();
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
    ((SolarWorldState *)D_00435DD0)->padA42 = 0;
    ((SolarWorldState *)D_00435DD0)->overlayFlag = 0;
    ((SolarWorldState *)D_00435DD0)->unkA4C = 0;
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

