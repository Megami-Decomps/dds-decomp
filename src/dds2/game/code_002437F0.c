#include "common.h"

u32 func_00304030(void *arg0, const char *arg1, s32 arg2);

extern u32 D_00437210[];

void func_003054E8(u32 sprite);

void func_00308380(s32 property, s32 object);

void func_00308808(s32 x, s32 y, s32 z, s32 width, s32 height, s32 angle, s32 object);

void func_00308DB0(s32 object);

void func_00308E60(s32 object);

void func_00308478(s32 property, s32 object);

void func_00243958(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);

typedef struct SolarPoint {
    u16 age;
    s16 duration;
    u8 active;
    u8 pad05;
} SolarPoint;

f32 func_00341240(s32 seed);

void func_002437F0(u32 *arg0) {
    *arg0 = func_00304030(D_00437210, "solarnoise.spr", 0);
}

void releaseSolarNoiseSprite(u32 *sprite) {
    func_003054E8(*sprite);
}

void initializeSolarOverlay(s32 object) {
    func_00308380(0x30000, object);
    func_00308808(0, 0, 0, 0x2000, 0xE00, 0, object);
    func_00308DB0(object);
    func_00308380(0x30000, object);
}

void finalizeSolarOverlay(s32 object) {
    func_00308E60(object);
    func_00308478(0x44, object);
    func_00308380(0x50000, object);
}

INCLUDE_ASM(const s32, "game/code_002437F0", func_002438F0);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00243958);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00243AD8);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00243C68);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00243DB8);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00243EE8);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00243FD8);

void drawSolarLayerPair(s32 x, s32 y, s32 z, s32 width, s32 layer, s32 context, s32 color) {
    func_00243958(x, y, z, width, layer, 0, context, color);
    if (layer != 0 && layer != 4 && layer != 8) {
        func_00243958(x, y, z, width, 9, 0, context, color);
    }
}

s32 func_00244178(s32 arg0) {
    s32 v;

    v = *(u16 *)(arg0 + 4) + 1;
    *(u16 *)(arg0 + 4) = v;
    if ((f32)(s16)v > 60.0f) {
        *(u16 *)(arg0 + 4) = 0;
        *(u8 *)(arg0 + 6) = 0;
    }
    return *(s8 *)(arg0 + 6);
}

s32 func_002441B8(s32 arg0) {
    s32 v;

    v = *(u16 *)(arg0 + 4) + 1;
    *(u16 *)(arg0 + 4) = v;
    if ((f32)(s16)v > 80.0f) {
        *(u16 *)(arg0 + 4) = 0;
        *(u8 *)(arg0 + 6) = 0;
    }
    return *(s8 *)(arg0 + 6);
}

INCLUDE_ASM(const s32, "game/code_002437F0", func_002441F8);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00244408);

INCLUDE_ASM(const s32, "game/code_002437F0", func_002446C8);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00244828);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00244988);

INCLUDE_ASM(const s32, "game/code_002437F0", func_002449E0);

INCLUDE_ASM(const s32, "game/code_002437F0", func_00244A38);

void updateSolarPointTimers(s32 object) {
    SolarPoint *point = (SolarPoint *)(object + 0xC);
    s32 i;
    for (i = 7; i >= 0; i--, point++) {
        if (point->active != 0) {
            s32 age = point->age + 1;
            point->age = age;
            if ((s16)age > point->duration) {
                point->age = 0;
                point->duration = (s16)(func_00341240(0) * 30.0f + 120.0f);
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

