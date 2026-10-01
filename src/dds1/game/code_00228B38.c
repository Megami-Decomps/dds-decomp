#include "common.h"
typedef struct SolarPoint {
    u16 age;
    s16 duration;
    u8 active;
    u8 pad05;
} SolarPoint;

typedef struct {
    u8 pad00[4];
    u16 age;    /* 0x04 */
    s8 active;  /* 0x06 */
} SolarLayerTimer;

typedef struct SolarFlagEntry {
    u8 pad[4];
    u8 flag;   /* 0x04 */
    u8 unk05;  /* 0x05 */
} SolarFlagEntry;

void func_00228CA0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);
extern void func_002CAAC8(s32, s32, s32 *, s32, s32, s32);
extern s32 D_0036832C[];

u32 effLoadIndexedResource(void *arg0, const char *arg1, s32 arg2);
void effDestroyResourceSlotSet(u32 sprite);
void sdfDispatchSurfaceWithPreparedTexturePacket(s32 object);
void sdfSubmitGsAlphaOneRegisterPacket(s32 property, s32 object);
void sdfSubmitGsTestOneRegisterPacket(s32 property, s32 object);
void func_002C0DD8(s32 x, s32 y, s32 z, s32 width, s32 height, s32 angle, s32 object);
void uiDrawActiveSurfaceRegion(s32 object);
f32 effMiscRandUnitFloat(s32 seed);

extern u32 D_003BBDD0[];

void evtLoadSolarNoiseSprite(u32 *sprite) {
    *sprite = effLoadIndexedResource(D_003BBDD0, "solarnoise.spr", 0);
}

void evtReleaseSolarNoiseSprite(u32 *sprite) {
    effDestroyResourceSlotSet(*sprite);
}

void evtInitializeSolarOverlay(s32 object) {
    sdfSubmitGsTestOneRegisterPacket(0x30000, object);
    func_002C0DD8(0, 0, 0, 0x2000, 0xE00, 0, object);
    uiDrawActiveSurfaceRegion(object);
    sdfSubmitGsTestOneRegisterPacket(0x30000, object);
}

void evtFinalizeSolarOverlay(s32 object) {
    sdfDispatchSurfaceWithPreparedTexturePacket(object);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, object);
    sdfSubmitGsTestOneRegisterPacket(0x50000, object);
}

INCLUDE_ASM(const s32, "game/code_00228B38", func_00228C38);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00228CA0);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00228E20);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00228FB0);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229100);

/* Indexed solar-table pass, then the shared layer setup with the 0xA rate. */
void func_00229230(s32 a0, s32 a1, s32 a2, s32 a3, s32 t0, s32 t1, s32 t2, s32 t3) {
    s32 n;
    s32 tmp;
    s32 *p;
    if (t0 == 0) {
        return;
    }
    n = t0;
    evtInitializeSolarOverlay(t2);
    tmp = 0;
    if (n < 8) {
        p = (s32 *)D_0036832C + t0 * 8;
        while (n < 8) {
            func_002CAAC8(p[-1], 0xFF, &tmp, p[0], 1, t2);
            n++;
            p += 8;
        }
    }
    evtFinalizeSolarOverlay(t2);
    func_00228CA0(a0, a1, 0, a3, 0xA, 0x20, t1, t2);
    func_00228C38(t2);
}

/* Indexed solar-table pass, then the shared layer setup with the 0x10 rate. */
void func_00229320(s32 a0, s32 a1, s32 a2, s32 a3, s32 t0, s32 t1, s32 t2, s32 t3) {
    s32 n;
    s32 tmp;
    s32 *p;
    if (t0 == 0) {
        return;
    }
    n = t0;
    evtInitializeSolarOverlay(t2);
    tmp = 0;
    if (n < 8) {
        p = (s32 *)D_0036832C + t0 * 8;
        while (n < 8) {
            func_002CAAC8(p[-1], 0xFF, &tmp, p[0], 1, t2);
            n++;
            p += 8;
        }
    }
    evtFinalizeSolarOverlay(t2);
    func_00228CA0(a0, a1, 0, a3, 0x10, 0x20, t1, t2);
    func_00228C38(t2);
}

/* Draw the selected solar-noise layer; intermediate layers also receive layer 9. */
void evtDrawSolarLayerPair(s32 x, s32 y, s32 z, s32 width, s32 layer, s32 context, s32 color) {
    func_00228CA0(x, y, z, width, layer, 0, context, color);
    if (layer != 0 && layer != 4 && layer != 8) {
        func_00228CA0(x, y, z, width, 9, 0, context, color);
    }
}

/* Layer lifetimes differ, but both reset their activation byte on expiry. */
s32 evtAdvanceSolarShortLayerTimer(SolarLayerTimer *timer) {
    s32 age;

    age = timer->age + 1;
    timer->age = age;
    if ((f32)(s16)age > 60.0f) {
        timer->age = 0;
        timer->active = 0;
    }
    return timer->active;
}

s32 evtAdvanceSolarLongLayerTimer(SolarLayerTimer *timer) {
    s32 age;

    age = timer->age + 1;
    timer->age = age;
    if ((f32)(s16)age > 80.0f) {
        timer->age = 0;
        timer->active = 0;
    }
    return timer->active;
}

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229540);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229750);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229A10);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229B70);

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
loop:
    if (--n < 0) {
        return;
    }
    if (tab[n].flag != 1) {
        goto loop;
    }
    tab[n].flag = 0;
}

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229D80);

/* Each active point restarts with a randomized duration near 120-150 frames. */
void evtUpdateSolarPointTimers(s32 object) {
    SolarPoint *point = (SolarPoint *)(object + 0xC);
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

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229ED8);

INCLUDE_RODATA(const s32, "game/code_00228B38", D_003ACBF8);

INCLUDE_RODATA(const s32, "game/code_00228B38", D_003ACC68);

INCLUDE_RODATA(const s32, "game/code_00228B38", D_003ACC78);

INCLUDE_RODATA(const s32, "game/code_00228B38", D_003ACC88);

INCLUDE_SDATA(const s32, "game/code_00228B38", D_003BBDD0);

INCLUDE_SDATA(const s32, "game/code_00228B38", D_003BBDD8);

