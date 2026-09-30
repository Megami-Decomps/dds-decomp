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

void func_00228CA0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);

u32 effLoadIndexedResource(void *arg0, const char *arg1, s32 arg2);
void effDestroyResourceSlotSet(u32 sprite);
void func_002C1430(s32 object);
void func_002C0A48(s32 property, s32 object);
void func_002C0950(s32 property, s32 object);
void func_002C0DD8(s32 x, s32 y, s32 z, s32 width, s32 height, s32 angle, s32 object);
void func_002C1380(s32 object);
f32 func_002E8398(s32 seed);

extern u32 D_003BBDD0[];

void evtLoadSolarNoiseSprite(u32 *sprite) {
    *sprite = effLoadIndexedResource(D_003BBDD0, "solarnoise.spr", 0);
}

void evtReleaseSolarNoiseSprite(u32 *sprite) {
    effDestroyResourceSlotSet(*sprite);
}

void evtInitializeSolarOverlay(s32 object) {
    func_002C0950(0x30000, object);
    func_002C0DD8(0, 0, 0, 0x2000, 0xE00, 0, object);
    func_002C1380(object);
    func_002C0950(0x30000, object);
}

void evtFinalizeSolarOverlay(s32 object) {
    func_002C1430(object);
    func_002C0A48(0x44, object);
    func_002C0950(0x50000, object);
}

INCLUDE_ASM(const s32, "game/code_00228B38", func_00228C38);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00228CA0);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00228E20);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00228FB0);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229100);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229230);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229320);

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

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229CD0);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229D28);

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
                point->duration = (s16)(func_002E8398(0) * 30.0f + 120.0f);
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

