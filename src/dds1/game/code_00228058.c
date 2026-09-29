#include "common.h"

/* Event-command work prefix; the +0xE4 key is also used by field task lookup. */
typedef struct EvtCommandWork {
    u8 pad00[0xE4];
    s32 taskKey;
} EvtCommandWork;

/* The first word is the solarnoise.spr handle; the work allocation is 0x104 bytes. */
typedef struct SolarOverlayWork {
    u32 noiseSprite;
    u8 pad04[0x100];
} SolarOverlayWork;

extern s32 kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);

void *func_002CFEB8(s32 size);

void evtInitializeVisualData(s32 arg0);
void evtLoadSolarNoiseSprite(u32 *sprite);

void func_00101A68(s32 arg0, void *arg1);

void func_00101A80(s32 arg0, s32 arg1);

EvtCommandWork *func_0010D6A0(void);

char *func_0010D5A8(s32 idx);

void func_0010D5F0(s32 value);

extern char D_003ACAE0[];

void func_002287C0(void);

void func_0022AB00(s32 arg0);

extern u32 D_003BBDC0;

extern char D_003BBDC8[];

extern s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

extern u64 func_00101A70(void);

extern f32 D_003BBDB8; /* solar overlay alpha, interpolated toward 0 or 1 */

typedef struct SolarWorldState SolarWorldState;
extern SolarWorldState *D_003BAA00;

extern s64 kwlnTaskIsRegistered(u64);

extern u64 func_0010D428(u64);

INCLUDE_ASM(const s32, "game/code_00228058", func_00228058);

INCLUDE_RODATA(const s32, "game/code_00228058", D_003ACA78);

INCLUDE_ASM(const s32, "game/code_00228058", func_002280B0);

u32 evtOpcodeCreateTaskForCurrentWork(void) {
    EvtCommandWork *work;
    s32 task;

    work = func_0010D6A0();
    if (work == NULL) {
        return 1;
    }
    if (work->taskKey == 0) {
        func_003003F0(D_003ACAE0);
        return 1;
    }
    task = evtCreateTask(0x2afe, func_0010D5A8(0));
    func_00101A80(work->taskKey, task);
    func_0010D5F0(task);
    return 1;
}

u32 evtOpcodeEnableCurrentContextFlag(void) {
    u64 task;
    s64 registered;

    task = func_0010D428(0);
    registered = kwlnTaskIsRegistered(task);
    if (registered != 0) {
        evtSetContextFlag(task);
    }
    return 1;
}

u32 evtOpcodeDisableCurrentContextFlag(void) {
    u64 task;
    s64 registered;

    task = func_0010D428(0);
    registered = kwlnTaskIsRegistered(task);
    if (registered != 0) {
        evtClearContextFlag(task);
    }
    return 1;
}

u32 evtOpcodeDestroyRegisteredTask(void) {
    u64 task;
    s64 registered;

    task = func_0010D428(0);
    registered = kwlnTaskIsRegistered(task);
    if (registered != 0) {
        evtDestroyTaskHierarchy(task);
    }
    return 1;
}

u32 evtOpcodeSetFieldFadeTarget(void) {
    u64 area;
    u64 target;
    u64 duration;

    func_00132B60(0);
    func_00132B70(0x80);
    area = func_0010D428(0);
    target = func_0010D428(1);
    duration = func_0010D428(2);
    fldSetFadeTarget(area, target, duration);
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00228058", D_003ACAE0);

u32 evtSetSkyAlpha(void) {
    s64 alpha;

    alpha = func_0010D428(1);
    if (alpha < -255) {
        func_0010AC10("warning : SET_SKY_A alpha < -255\n");
        alpha = -255;
    }
    if (alpha > 255) {
        func_0010AC10("warning : SET_SKY_A alpha > 255\n");
        alpha = 255;
    }
    evtBeginSkyParameterTransition(func_0010D428(0), alpha);
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

INCLUDE_ASM(const s32, "game/code_00228058", func_00228408);

u32 evtOpcodePlayBgm(void) {
    u64 id;
    u64 fade;

    id = func_0010D428(0);
    fade = func_0010D428(1);
    evtPlayBgm(id, fade);
    return 1;
}

u32 evtOpcodeTransitionBgm(void) {
    u64 id;
    u64 fade;

    id = func_0010D428(0);
    fade = func_0010D428(1);
    evtTransitionBgm(id, fade);
    return 1;
}

u32 func_002284E0(void) {
    u64 id;
    u64 fade;

    id = func_0010D428(0);
    fade = func_0010D428(1);
    func_00241A50(id, fade);
    return 1;
}

u32 func_00228520(void) {
    func_002E9708();
    func_001F3448();
    return 1;
}

u32 func_00228548(void) {
    func_002E9730();
    func_001F3448();
    return 1;
}

u32 evtOpcodeSetBgmVolumePan(void) {
    u64 id;
    u64 fade;

    id = func_0010D428(0);
    fade = func_0010D428(1);
    evtSetBgmVolumePan(id, fade);
    return 1;
}

u32 func_002285B0(void) {
    u64 id;
    u64 fade;

    id = func_0010D428(0);
    fade = func_0010D428(1);
    func_00241AE8(id, fade);
    return 1;
}

u32 evtOpcodeRefreshTaskData(void) {
    u64 task;
    u64 value;

    task = func_0010D428(0);
    value = func_0010D428(1);
    evtRefreshTaskData(task, value);
    return 1;
}

u32 evtOpcodeInitializeEffectSoundChannel(void) {
    effInitCh72Id();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00228058", func_00228650);

struct SolarWorldState {
    u8 pad00[0xA40];
    u8 flags;          /* 0xA40 */
    u8 phase;          /* 0xA41 */
    u8 secondaryPhase; /* 0xA42 */
    u8 padA43;
    f32 phaseTimer;    /* 0xA44: elapsed phase time, advanced as float in func_00228930 */
};

#define SOLAR_PHASE_ADVANCE_FLAG 1
#define SOLAR_ALPHA_ENABLED_FLAG 2

s32 evtGetMirroredSolarPhase(void) {
    s32 phase;

    phase = D_003BAA00->phase;
    if (phase >= 9) {
        phase = 8 - (phase & 7);
    }
    return phase;
}

u8 evtGetSolarPhase(void) {
    return D_003BAA00->phase;
}

void evtSetSolarPhase(u8 phase) {
    D_003BAA00->phase = phase & 0xf;
    D_003BAA00->phaseTimer = 0.0f;
}

void evtEnableSolarPhaseAdvance(void) {
    D_003BAA00->flags = D_003BAA00->flags | SOLAR_PHASE_ADVANCE_FLAG;
}

void evtDisableSolarPhaseAdvance(void) {
    D_003BAA00->flags = D_003BAA00->flags & ~SOLAR_PHASE_ADVANCE_FLAG;
}

void evtSetSolarOverlayFullyVisible(void) {
    D_003BAA00->flags = D_003BAA00->flags | SOLAR_ALPHA_ENABLED_FLAG;
    D_003BBDB8 = 1.0f;
    func_0022AB00(0);
}

void evtSetSolarOverlayFullyTransparent(void) {
    D_003BAA00->flags = D_003BAA00->flags & ~SOLAR_ALPHA_ENABLED_FLAG;
    D_003BBDB8 = 0.0f;
}

void evtEnableSolarOverlayAlpha(void) {
    D_003BAA00->flags = D_003BAA00->flags | SOLAR_ALPHA_ENABLED_FLAG;
}

void evtDisableSolarOverlayAlpha(void) {
    D_003BAA00->flags = D_003BAA00->flags & ~SOLAR_ALPHA_ENABLED_FLAG;
}

INCLUDE_ASM(const s32, "game/code_00228058", func_002287C0);

INCLUDE_ASM(const s32, "game/code_00228058", func_00228930);

void *evtCreateSolarOverlayWork(s32 owner) {
    SolarOverlayWork *overlay;

    overlay = func_002CFEB8(0x104);
    evtInitializeVisualData((s32)overlay);
    evtLoadSolarNoiseSprite(&overlay->noiseSprite);
    func_00101A68(owner, overlay);
    return (void *)func_002287C0;
}

void evtFreeSolarOverlayWork(void) {
    u64 overlay;

    overlay = func_00101A70();
    evtReleaseSolarNoiseSprite(overlay);
    func_002CFF98(overlay);
    D_003BBDC0 = 0;
}

void evtStartSolarOverlay(void) {
    if (D_003BBDC0 != 0) {
        return;
    }
    D_003BBDC0 = kwlnTaskCreate((s32)D_003BBDC8, 0x2b0b, 1, 1, (s32)evtCreateSolarOverlayWork, (s32)evtFreeSolarOverlayWork, 0);
    evtEnableSolarPhaseAdvance();
    evtSetSolarOverlayFullyTransparent();
    evtSetSolarPhase(0);
    D_003BAA00->secondaryPhase = 0;
}

void evtStopSolarOverlay(void) {
    if (D_003BBDC0 != 0) {
        kwlnTaskDestroyWithHierarchy(D_003BBDC0, 1);
    }
}

INCLUDE_SDATA(const s32, "game/code_00228058", D_003BBDB8);

INCLUDE_SDATA(const s32, "game/code_00228058", D_003BBDBC);

INCLUDE_SDATA(const s32, "game/code_00228058", D_003BBDC0);

INCLUDE_SDATA(const s32, "game/code_00228058", D_003BBDC8);

