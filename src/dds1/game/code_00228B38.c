#include "common.h"
typedef struct SolarPoint {
    u16 age;
    s16 duration;
    u8 active;
    u8 pad05;
} SolarPoint;

void func_00228CA0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);

u32 func_002BC8F0(void *arg0, const char *arg1, s32 arg2);
void func_002BDD60(u32 sprite);
void func_002C1430(s32 object);
void func_002C0A48(s32 property, s32 object);
void func_002C0950(s32 property, s32 object);
void func_002C0DD8(s32 x, s32 y, s32 z, s32 width, s32 height, s32 angle, s32 object);
void func_002C1380(s32 object);
f32 func_002E8398(s32 seed);

extern u32 D_003BBDD0[];

void func_00228B38(u32 *arg0) {
    *arg0 = func_002BC8F0(D_003BBDD0, "solarnoise.spr", 0);
}

void releaseSolarNoiseSprite(u32 *sprite) {
    func_002BDD60(*sprite);
}

void initializeSolarOverlay(s32 object) {
    func_002C0950(0x30000, object);
    func_002C0DD8(0, 0, 0, 0x2000, 0xE00, 0, object);
    func_002C1380(object);
    func_002C0950(0x30000, object);
}

void finalizeSolarOverlay(s32 object) {
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

void drawSolarLayerPair(s32 x, s32 y, s32 z, s32 width, s32 layer, s32 context, s32 color) {
    func_00228CA0(x, y, z, width, layer, 0, context, color);
    if (layer != 0 && layer != 4 && layer != 8) {
        func_00228CA0(x, y, z, width, 9, 0, context, color);
    }
}

s32 func_002294C0(s32 arg0) {
    s32 v;

    v = *(u16 *)(arg0 + 4) + 1;
    *(u16 *)(arg0 + 4) = v;
    if ((f32)(s16)v > 60.0f) {
        *(u16 *)(arg0 + 4) = 0;
        *(u8 *)(arg0 + 6) = 0;
    }
    return *(s8 *)(arg0 + 6);
}

s32 func_00229500(s32 arg0) {
    s32 v;

    v = *(u16 *)(arg0 + 4) + 1;
    *(u16 *)(arg0 + 4) = v;
    if ((f32)(s16)v > 80.0f) {
        *(u16 *)(arg0 + 4) = 0;
        *(u8 *)(arg0 + 6) = 0;
    }
    return *(s8 *)(arg0 + 6);
}

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229540);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229750);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229A10);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229B70);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229CD0);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229D28);

INCLUDE_ASM(const s32, "game/code_00228B38", func_00229D80);

void updateSolarPointTimers(s32 object) {
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

