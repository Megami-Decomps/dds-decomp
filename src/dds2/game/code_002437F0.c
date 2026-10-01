#include "common.h"

u32 effLoadIndexedResource(void *resourceTable, const char *fileName, s32 index);

extern u32 D_00437210[];

void effDestroyResourceSlotSet(u32 sprite);

void sdfSubmitGsTestOneRegisterPacket(s32 property, s32 object);

void func_00308808(s32 x, s32 y, s32 z, s32 width, s32 height, s32 angle, s32 object);

void uiDrawActiveSurfaceRegion(s32 object);

void sdfDispatchSurfaceWithPreparedTexturePacket(s32 object);

void sdfSubmitGsAlphaOneRegisterPacket(s32 property, s32 object);

void func_00243958(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);

typedef struct SolarPoint {
    u16 age;
    s16 duration;
    u8 active;
    u8 pad05;
} SolarPoint;

typedef struct SolarLayerTimer {
    u8 pad00[4];
    u16 age;
    union {
        u8 byte;
        s8 signedByte;
    } active;
} SolarLayerTimer;

typedef struct SolarOverlayWork {
    u8 pad00[0xC];
    SolarPoint points[8];
} SolarOverlayWork;

typedef struct SolarFlagEntry {
    u8 pad[4];
    u8 flag;   /* 0x04 */
    u8 unk05;  /* 0x05 */
} SolarFlagEntry;

f32 effMiscRandUnitFloat(s32 seed);

void evtLoadSolarNoiseSprite(u32 *sprite) {
    *sprite = effLoadIndexedResource(D_00437210, "solarnoise.spr", 0);
}

void evtReleaseSolarNoiseSprite(u32 *sprite) {
    effDestroyResourceSlotSet(*sprite);
}

void evtInitializeSolarOverlay(s32 object) {
    sdfSubmitGsTestOneRegisterPacket(0x30000, object);
    func_00308808(0, 0, 0, 0x2000, 0xE00, 0, object);
    uiDrawActiveSurfaceRegion(object);
    sdfSubmitGsTestOneRegisterPacket(0x30000, object);
}

void evtFinalizeSolarOverlay(s32 object) {
    sdfDispatchSurfaceWithPreparedTexturePacket(object);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, object);
    sdfSubmitGsTestOneRegisterPacket(0x50000, object);
}

INCLUDE_ASM(const s32, "game/code_002437F0", func_002438F0);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00243958);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00243AD8);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00243C68);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00243DB8);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00243EE8);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00243FD8);

/* Draw the selected solar-noise layer; intermediate layers also receive layer 9. */
void evtDrawSolarLayerPair(s32 x, s32 y, s32 z, s32 width, s32 layer, s32 context, s32 color) {
    func_00243958(x, y, z, width, layer, 0, context, color);
    if (layer != 0 && layer != 4 && layer != 8) {
        func_00243958(x, y, z, width, 9, 0, context, color);
    }
}

/* Layer lifetimes differ, but both reset their activation byte on expiry. */
s32 evtAdvanceSolarShortLayerTimer(SolarLayerTimer *timer) {
    s32 nextAge;

    nextAge = timer->age + 1;
    timer->age = nextAge;
    if ((f32)(s16)nextAge > 60.0f) {
        timer->age = 0;
        timer->active.byte = 0;
    }
    return timer->active.signedByte;
}

s32 evtAdvanceSolarLongLayerTimer(SolarLayerTimer *timer) {
    s32 nextAge;

    nextAge = timer->age + 1;
    timer->age = nextAge;
    if ((f32)(s16)nextAge > 80.0f) {
        timer->age = 0;
        timer->active.byte = 0;
    }
    return timer->active.signedByte;
}

INCLUDE_ASM(const s32, "game/code_002437F0", func_002441F8);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00244408);

INCLUDE_ASM(const s32, "game/code_002437F0", func_002446C8);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00244828);

void evtSetFirstUnflaggedSolarEntry(s32 base, s32 n) {
    u8 *p = (u8 *)(base + 0xC);
    SolarFlagEntry *tab = (SolarFlagEntry *)(base + 0xC);
    s32 i;
    (void)n;
    i = 0;
    if (p[4] != 0) {
        goto scan;
    }
    p[4] = 1;
    return;
    /* Likewise fused ++i head test with a goto back-edge to the test. */
scan:
    if (++i < 8) {
        if (tab[i].flag != 0) {
            goto scan;
        }
        tab[i].flag = 1;
    }
}

void evtConsumeFlaggedSolarEntry(s32 base, s32 n) {
    u8 *p = (u8 *)(base + 0x36);
    SolarFlagEntry *tab = (SolarFlagEntry *)(base + 0xC);
    n = 7;
    if (p[4] == 1) {
        p[4] = 0;
        return;
    }
    /* The adjust is fused into the head test so the 7 init survives, and the
       goto targets the test so each pass adjusts once. Plain for/while forms
       fold the init and walk the table instead of indexing it. */
loop:
    if (--n < 0) {
        return;
    }
    if (tab[n].flag != 1) {
        goto loop;
    }
    tab[n].flag = 0;
}

INCLUDE_ASM(const s32, "game/code_002437F0", func_00244A38);

/* Each active point restarts with a randomized duration near 120-150 frames. */
void evtUpdateSolarPointTimers(SolarOverlayWork *overlay) {
    SolarPoint *point = overlay->points;
    s32 i;
    for (i = 7; i >= 0; i--, point++) {
        if (point->active != 0) {
            s32 age = point->age + 1;
            point->age = age;
            if ((s16)age > point->duration) {
                point->age = 0;
                point->duration = (s16)(effMiscRandUnitFloat(0) * 30.0f + 120.0f);
            }
        } else {
            point->age = 0;
            point->duration = 0;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002437F0", func_00244B90);

INCLUDE_RODATA(const s32, "game/code_002437F0", D_00422168);

INCLUDE_RODATA(const s32, "game/code_002437F0", D_004221D8);

INCLUDE_RODATA(const s32, "game/code_002437F0", D_004221E8);

INCLUDE_RODATA(const s32, "game/code_002437F0", D_00422238);

INCLUDE_RODATA(const s32, "game/code_002437F0", D_00422308);

INCLUDE_SDATA(const s32, "game/code_002437F0", D_00437210);

INCLUDE_SDATA(const s32, "game/code_002437F0", D_00437218);

