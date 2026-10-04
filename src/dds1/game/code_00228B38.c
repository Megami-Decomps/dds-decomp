#include "common.h"
typedef struct SolarNoiseLayer {
    s16 x;
    s16 y;
    s16 age;
    s8 active;
    u8 scale;
} SolarNoiseLayer;

typedef struct SolarNoiseState {
    u8 pad00[8];
    u16 spawnAge;
    s16 spawnInterval;
    u16 activeCount;
    u16 pad0E;
    SolarNoiseLayer layers[10];
} SolarNoiseState;

typedef struct SolarNoiseDebugData {
    const char *label;
    u32 unused;
    char format[4];
} SolarNoiseDebugData;

typedef struct SolarSpriteLayer {
    u8 pad00[0xC];
    s32 drawWidth;
    s32 drawHeight;
    u8 pad14[0x68];
    s32 width;
    s32 height;
    u8 pad84[0x1C];
} SolarSpriteLayer;

typedef struct SolarSpriteContext {
    u8 pad00[0x18];
    SolarSpriteLayer *layers;
} SolarSpriteContext;

typedef struct SolarOverlayShape {
    s32 *primaryPositions;
    u32 *primaryColors;
    s32 primaryCount;
    s32 *positions;
    u32 *colors;
    s32 count;
    s32 *tailPositions;
    s32 tailCount;
} SolarOverlayShape;

extern f32 sdfSinPoly(f32 angle);
extern void func_00228E20(s32, s32, s32, s32, s32, s32, s32, s32, f32);

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

void func_00228CA0(s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_002CAAC8(s32, s32, s32 *, s32, s32, s32);
extern void func_002BF4E0(s32, s32, s32, u32, s32, s32, s32, s32);
extern s32 D_0036832C[];
extern s32 D_00367EE0[][3];
extern SolarOverlayShape D_00368310[];

u32 effLoadIndexedResource(void *arg0, const char *arg1, s32 arg2);
void effDestroyResourceSlotSet(u32 sprite);
void sdfDispatchSurfaceWithPreparedTexturePacket(s32 object);
void sdfSubmitGsAlphaOneRegisterPacket(s32 property, s32 object);
/* The definition uses legacy K&R parameters. */
void sdfSubmitGsTestOneRegisterPacket();
void uiDrawUniformColorRect(s32 x, s32 y, s32 z, s32 width, s32 height, s32 angle, s32 object);
void uiDrawActiveSurfaceRegion(s32 object);
f32 effMiscRandUnitFloat(s32 seed);
u32 effMiscRand(s32 seed);

extern u32 D_003BBDD0[];

void evtLoadSolarNoiseSprite(u32 *sprite) {
    *sprite = effLoadIndexedResource(D_003BBDD0, "solarnoise.spr", 0);
}

void evtReleaseSolarNoiseSprite(u32 *sprite) {
    effDestroyResourceSlotSet(*sprite);
}

void evtInitializeSolarOverlay(s32 object) {
    sdfSubmitGsTestOneRegisterPacket(0x30000, object);
    uiDrawUniformColorRect(0, 0, 0, 0x2000, 0xE00, 0, object);
    uiDrawActiveSurfaceRegion(object);
    sdfSubmitGsTestOneRegisterPacket(0x30000, object);
}

void evtFinalizeSolarOverlay(s32 object) {
    sdfDispatchSurfaceWithPreparedTexturePacket(object);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, object);
    sdfSubmitGsTestOneRegisterPacket(0x50000, object);
}

void evtPrepareSolarOverlayTestState(s32 object) {
    sdfSubmitGsAlphaOneRegisterPacket(0x44, object);
    sdfSubmitGsTestOneRegisterPacket(0x30000, object);
    uiDrawUniformColorRect(0, 0, 0, 0x2000, 0xE00, 0, object);
    sdfSubmitGsTestOneRegisterPacket(0x5100DL, object);
}

void func_00228CA0(s32 x, s32 y, s32 z, s32 alpha, s32 layer, s32 mode,
    s32 contextHandle, s32 color) {
    {
        SolarSpriteContext *context = *(SolarSpriteContext **)contextHandle;
        s32 spriteOffset = layer * sizeof(SolarSpriteLayer);
        s32 scalePercent = D_00367EE0[layer][2];
        SolarSpriteLayer *sprite =
            (SolarSpriteLayer *)(spriteOffset + (s32)context->layers);

        sprite->drawWidth = (s32)((f32)(sprite->width * scalePercent) / 100.0f) << 4;
        sprite->drawHeight = (s32)((f32)(sprite->height * D_00367EE0[layer][2]) / 100.0f) << 3;
        func_002BF4E0((x + D_00367EE0[layer][0]) << 4,
                      (y + D_00367EE0[layer][1]) << 3, z,
                      (u32)((f32)(alpha << 8) * 0.0078125f), mode, (s32)context, layer, color);
    }

    {
        SolarSpriteContext *restoredContext = *(SolarSpriteContext **)contextHandle;
        s32 restoredOffset = layer * sizeof(SolarSpriteLayer);
        SolarSpriteLayer *restoredSprite =
            (SolarSpriteLayer *)(restoredOffset + (s32)restoredContext->layers);

        restoredSprite->drawWidth = restoredSprite->width << 4;
        restoredSprite->drawHeight = restoredSprite->height << 3;
    }
}

/* Draw a scaled sprite around its center, then restore its native dimensions. */
void func_00228E20(s32 x, s32 y, s32 z, s32 alpha, s32 layer, s32 mode,
    s32 contextHandle, s32 color, f32 scale) {
    {
        SolarSpriteContext *context = *(SolarSpriteContext **)contextHandle;
        s32 spriteOffset = layer * sizeof(SolarSpriteLayer);
        s32 scalePercent = D_00367EE0[layer][2];
        SolarSpriteLayer *sprite =
            (SolarSpriteLayer *)(spriteOffset + (s32)context->layers);

        sprite->drawWidth =
            (s32)((scale * (f32)sprite->width * (f32)scalePercent) / 100.0f) << 4;
        sprite->drawHeight =
            (s32)((scale * (f32)sprite->height * (f32)D_00367EE0[layer][2]) / 100.0f) << 3;
        func_002BF4E0(((x + D_00367EE0[layer][0]) << 4) - (sprite->drawWidth / 2),
                      ((y + D_00367EE0[layer][1]) << 3) - (sprite->drawHeight / 2), z,
                      (u32)((f32)(alpha << 8) * 0.0078125f), mode, (s32)context, layer, color);
    }

    {
        SolarSpriteContext *restoredContext = *(SolarSpriteContext **)contextHandle;
        s32 restoredOffset = layer * sizeof(SolarSpriteLayer);
        SolarSpriteLayer *restoredSprite =
            (SolarSpriteLayer *)(restoredOffset + (s32)restoredContext->layers);

        restoredSprite->drawWidth = restoredSprite->width << 4;
        restoredSprite->drawHeight = restoredSprite->height << 3;
    }
}

/* Submit the fixed fourteen-point solar marker with a scaled leading alpha. */
void func_00228FB0(s32 unused0, s32 unused1, s32 alpha, s32 intensity, s32 object) {
    s32 positions[28] = {
        47, 48, 76, 48, 72, 63, 63, 75, 51, 79, 38, 75, 29, 64,
        27, 48, 29, 32, 38, 20, 51, 16, 63, 20, 72, 31, 76, 48,
    };
    s32 colors[14];
    s32 i;
    s32 color = 0x335072;

    colors[0] = ((s32)((f32)intensity * 0.2f) << 24) | color;
    for (i = 1; i < 14; i++) {
        colors[i] = color;
    }
    func_002CAAC8((s32)positions, alpha, colors, 14, 0, object);
}

/* Scale a selected overlay shape's alpha values and submit it between GS states. */
void func_00229100(s32 unused0, s32 unused1, s32 alpha, s32 alphaScale,
    s32 entryIndex, s32 object) {
    s32 colors[4];
    s32 i;
    s32 tableIndex;
    u32 color;

    if (entryIndex == 0) {
        return;
    }

    sdfSubmitGsAlphaOneRegisterPacket(0x48, object);
    tableIndex = entryIndex - 1;
    for (i = 0; i < D_00368310[tableIndex].count; i++) {
        color = D_00368310[tableIndex].colors[i];
        colors[i] = (color & 0xFFFFFF) |
            ((s32)((f32)((s32)((color & 0xFF000000) >> 24) * alphaScale) * 0.0078125f) << 24);
    }
    func_002CAAC8((s32)D_00368310[tableIndex].positions, alpha, colors,
                  D_00368310[tableIndex].count, 0, object);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, object);
}

/* Indexed solar-table pass, then the shared layer setup with the 0xA rate. */
void evtDrawIndexedSolarOverlayLayers(s32 a0, s32 a1, s32 a2, s32 a3, s32 t0, s32 t1, s32 t2, s32 t3) {
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
    evtPrepareSolarOverlayTestState(t2);
}

/* Indexed solar-table pass, then the shared layer setup with the 0x10 rate. */
void evtDrawPartialSolarOverlay(s32 a0, s32 a1, s32 a2, s32 a3, s32 t0, s32 t1, s32 t2, s32 t3) {
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
    evtPrepareSolarOverlayTestState(t2);
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

INCLUDE_SDATA(const s32, "game/code_00228B38", D_003BBDD0);

void func_00229750(SolarNoiseState *state) {
    u8 phase = evtGetMirroredSolarPhase();
    u16 spawns[15][3] = {
        {57, 62, 50}, {50, 66, 20}, {52, 62, 50}, {52, 60, 100},
        {45, 58, 50}, {41, 50, 50}, {41, 50, 100}, {42, 42, 50},
        {46, 39, 50}, {50, 32, 50}, {50, 32, 100}, {60, 32, 50},
        {67, 38, 50}, {65, 40, 100}, {71, 45, 50},
    };

    {
        u8 spawnCounts[8] = {3, 5, 6, 8, 10, 12, 13, 15};
        SolarNoiseLayer *layer;
        s8 *active;
        s32 i;

        if (phase >= 2) {
            s32 spawnLimit = (s32)((f32)phase * 3.0f * 0.125f + 1.0f);
            state->spawnAge++;
            if ((s16)state->spawnAge > state->spawnInterval) {
                state->spawnAge = 0;
                if (state->activeCount < 10 && state->activeCount < spawnLimit) {
                    state->activeCount++;
                    for (i = 0; i < 10; i++) {
                        if (state->layers[i].active == 0) {
                            s32 spawnIndex =
                                effMiscRand(0) % spawnCounts[phase - 2];
                            state->layers[i].x = spawns[spawnIndex][0];
                            state->layers[i].y = spawns[spawnIndex][1];
                            state->layers[i].age = 0;
                            state->layers[i].active = 1;
                            state->layers[i].scale = spawns[spawnIndex][2];
                            break;
                        }
                    }
                }
            }
        }

        layer = state->layers;
        active = &layer->active;
        for (i = 9; i >= 0; i--, layer++, active += sizeof(*layer)) {
            if (*active != 0 &&
                evtAdvanceSolarLongLayerTimer((SolarLayerTimer *)layer) == 0) {
                state->activeCount--;
            }
        }
    }
}

const char D_003ACC58[16] = "FREE CAMERA";
static SolarNoiseDebugData evtSolarNoiseDebugData
    __attribute__((section(".sdata"))) = {(const char *)0x003ACC58, 0, "%s"};

void evtDrawShortSolarNoiseLayers(s32 x, s32 y, s32 z, s32 width, SolarNoiseState *state, s32 context, s32 color) {
    SolarNoiseLayer *layer = state->layers;
    s32 i;
    s32 layerX;
    s32 layerY;
    f32 t;
    f32 value;
    f32 scale;

    for (i = 9; i >= 0; i--, layer++) {
        if (layer->active != 0) {
            layerX = layer->x + x;
            layerY = layer->y + y;
            value = (f32)layer->age;
            t = value / 60.0f;
            t = sdfSinPoly(t * 3.14159265f);
            value = (f32)width * 0.15f;
            value *= t;
            scale = (f32)layer->scale / 100.0f;
            scale *= t;
            func_00228E20(layerX, layerY, z, (s32)value, 15, 0, context, color, scale);
        }
    }
}

void evtDrawLongSolarNoiseLayers(s32 x, s32 y, s32 z, s32 width, SolarNoiseState *state, s32 context, s32 color) {
    SolarNoiseLayer *layer = state->layers;
    s32 i;
    s32 layerX;
    s32 layerY;
    f32 t;
    f32 value;
    f32 scale;

    for (i = 9; i >= 0; i--, layer++) {
        if (layer->active != 0) {
            layerX = layer->x + x;
            layerY = layer->y + y;
            value = (f32)layer->age;
            t = value / 80.0f;
            t = sdfSinPoly(t * 3.14159265f);
            value = (f32)width * 0.6f;
            value *= t;
            scale = (f32)layer->scale / 100.0f;
            scale *= t;
            func_00228E20(layerX, layerY, z, (s32)value, 15, 0, context, color, scale);
        }
    }
}

/* Activate the first inactive solar point. */
void evtActivateNextSolarPoint(s32 object) {
    SolarPoint *points = (SolarPoint *)(object + 0xC);
    s32 i;

    for (i = 0; i < 8; i++) {
        if (points[i].active == 0) {
            points[i].active = 1;
            break;
        }
    }
}

/* Deactivate the last active solar point. */
void evtDeactivateLastSolarPoint(s32 object) {
    SolarPoint *points = (SolarPoint *)(object + 0xC);
    s32 i;

    for (i = 7; i >= 0; i--) {
        if (points[i].active == 1) {
            points[i].active = 0;
            break;
        }
    }
}

void evtSetSolarPointActiveCount(s32 object, u32 desiredCount) {
    SolarPoint *point;
    u8 *active;
    u32 activeCount;
    s32 i;

    activeCount = 0;
    point = (SolarPoint *)(object + 0xC);
    active = &point->active;
    for (i = 0; i < 8; i++, active += sizeof(*point)) {
        if (*active == 1) {
            activeCount++;
        }
    }
    while (activeCount != desiredCount) {
        if (activeCount < desiredCount) {
            activeCount++;
            evtActivateNextSolarPoint(object);
        } else if (desiredCount < activeCount) {
            activeCount--;
            evtDeactivateLastSolarPoint(object);
        }
    }
}

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

INCLUDE_RODATA(const s32, "game/code_00228B38", D_003ACC68);

INCLUDE_RODATA(const s32, "game/code_00228B38", D_003ACC78);

INCLUDE_RODATA(const s32, "game/code_00228B38", D_003ACC88);

