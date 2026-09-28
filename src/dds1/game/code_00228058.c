#include "common.h"

extern s32 kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);

void *func_002CFEB8(s32 size);

void evtInitializeVisualData(s32 arg0);

void func_00101A68(s32 arg0, void *arg1);

void func_00101A80(s32 arg0, s32 arg1);

s32 func_0010D6A0(void);

char *func_0010D5A8(s32 idx);

void func_0010D5F0(s32 value);

extern char D_003ACAE0[];

void func_002287C0(void);

void func_0022AB00(s32 arg0);

extern u32 D_003BBDC0;

extern char D_003BBDC8[];

extern s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

extern u64 func_00101A70(void);

extern u32 D_003BBDB8;

extern s32 D_003BAA00;

extern s64 kwlnTaskIsRegistered(u64);

extern u64 func_0010D428(u64);

INCLUDE_ASM(const s32, "game/code_00228058", func_00228058);

INCLUDE_RODATA(const s32, "game/code_00228058", D_003ACA78);

INCLUDE_ASM(const s32, "game/code_00228058", func_002280B0);

u32 func_00228180(void) {
    s32 v0;
    s32 v1;

    v0 = func_0010D6A0();
    if (v0 == 0) {
        return 1;
    }
    if (*(s32 *)(v0 + 0xe4) == 0) {
        func_003003F0(D_003ACAE0);
        return 1;
    }
    v1 = evtCreateTask(0x2afe, func_0010D5A8(0));
    func_00101A80(*(s32 *)(v0 + 0xe4), v1);
    func_0010D5F0(v1);
    return 1;
}

u32 func_00228210(void) {
    u64 temp_v0;
    s64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = kwlnTaskIsRegistered(temp_v0);
    if (temp_v1 != 0) {
        func_00235088(temp_v0);
    }
    return 1;
}

u32 func_00228258(void) {
    u64 temp_v0;
    s64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = kwlnTaskIsRegistered(temp_v0);
    if (temp_v1 != 0) {
        func_002350B0(temp_v0);
    }
    return 1;
}

u32 func_002282A0(void) {
    u64 temp_v0;
    s64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = kwlnTaskIsRegistered(temp_v0);
    if (temp_v1 != 0) {
        func_002350E0(temp_v0);
    }
    return 1;
}

u32 func_002282E8(void) {
    u64 temp_v0;
    u64 temp_v1;
    u64 temp_v2;

    func_00132B60(0);
    func_00132B70(0x80);
    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    temp_v2 = func_0010D428(2);
    fldSetFadeTarget(temp_v0, temp_v1, temp_v2);
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
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    evtPlayBgm(temp_v0, temp_v1);
    return 1;
}

u32 evtOpcodeTransitionBgm(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    evtTransitionBgm(temp_v0, temp_v1);
    return 1;
}

u32 func_002284E0(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    func_00241A50(temp_v0, temp_v1);
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
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    evtSetBgmVolumePan(temp_v0, temp_v1);
    return 1;
}

u32 func_002285B0(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    func_00241AE8(temp_v0, temp_v1);
    return 1;
}

u32 func_002285F0(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    evtRefreshTaskData(temp_v0, temp_v1);
    return 1;
}

u32 func_00228630(void) {
    effInitCh72Id();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00228058", func_00228650);

typedef struct {
    u8 pad00[0xA40];
    u8 flags;          /* 0xA40 */
    u8 phase;          /* 0xA41 */
    u8 secondaryPhase; /* 0xA42 */
    u8 padA43;
    u32 phaseCounter;  /* 0xA44 */
} SolarWorldState;

s32 evtGetMirroredSolarPhase(void) {
    s32 phase;

    phase = ((SolarWorldState *)D_003BAA00)->phase;
    if (phase >= 9) {
        phase = 8 - (phase & 7);
    }
    return phase;
}

u8 evtGetSolarPhase(void) {
    return ((SolarWorldState *)D_003BAA00)->phase;
}

void evtSetSolarPhase(u8 phase) {
    ((SolarWorldState *)D_003BAA00)->phase = phase & 0xf;
    ((SolarWorldState *)D_003BAA00)->phaseCounter = 0;
}

void func_00228710(void) {
    ((SolarWorldState *)D_003BAA00)->flags = ((SolarWorldState *)D_003BAA00)->flags | 1;
}

void func_00228728(void) {
    ((SolarWorldState *)D_003BAA00)->flags = ((SolarWorldState *)D_003BAA00)->flags & 0xfe;
}

void func_00228740(void) {
    ((SolarWorldState *)D_003BAA00)->flags = ((SolarWorldState *)D_003BAA00)->flags | 2;
    *(f32 *)&D_003BBDB8 = 1.0f;
    func_0022AB00(0);
}

void func_00228778(void) {
    ((SolarWorldState *)D_003BAA00)->flags = ((SolarWorldState *)D_003BAA00)->flags & 0xfd;
    D_003BBDB8 = 0;
}

void func_00228790(void) {
    ((SolarWorldState *)D_003BAA00)->flags = ((SolarWorldState *)D_003BAA00)->flags | 2;
}

void func_002287A8(void) {
    ((SolarWorldState *)D_003BAA00)->flags = ((SolarWorldState *)D_003BAA00)->flags & 0xfd;
}

INCLUDE_ASM(const s32, "game/code_00228058", func_002287C0);

INCLUDE_ASM(const s32, "game/code_00228058", func_00228930);

void *func_00228A00(s32 owner) {
    s32 overlay;

    overlay = (s32)func_002CFEB8(0x104);
    evtInitializeVisualData(overlay);
    evtLoadSolarNoiseSprite((u32 *)overlay);
    func_00101A68(owner, (void *)overlay);
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
    D_003BBDC0 = kwlnTaskCreate((s32)D_003BBDC8, 0x2b0b, 1, 1, (s32)func_00228A00, (s32)evtFreeSolarOverlayWork, 0);
    func_00228710();
    func_00228778();
    evtSetSolarPhase(0);
    ((SolarWorldState *)D_003BAA00)->secondaryPhase = 0;
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

