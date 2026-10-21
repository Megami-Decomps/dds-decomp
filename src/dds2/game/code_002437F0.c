#include "sdf_gs_register_submit_api.h"
#include "common.h"
#include "sdf_trig.h"
#include "eff_random.h"
#include "eff_resource_slots.h"
#include "evt_solar.h"


extern void evtDrawSolarNoiseLayerAtCenteredScale(s32 x, s32 y, s32 z, s32 alpha, s32 layer,
                                                   s32 mode, s32 contextHandle, s32 color, f32 scale);


extern char D_00437210[];


void uiDrawUniformColorRect(s32 x, s32 y, s32 z, s32 width, s32 height, s32 angle, s32 object);

void uiDrawActiveSurfaceRegion(s32 object);

void sdfDispatchSurfaceWithPreparedTexturePacket(s32 object);

void sdfSubmitGsAlphaOneRegisterPacket(s32 property, s32 object);

void evtDrawSolarNoiseLayerAtConfiguredScale(s32 x, s32 y, s32 z, s32 alpha, s32 layer, s32 mode,
                                             s32 contextHandle, s32 color);
extern s32 D_003C90DC[];
extern s32 evtSolarNoiseLayerPlacements[][3];
extern SolarOverlayShape evtSolarOverlayShapes[];
extern void sdfDrawColoredTriangleFan(s32 *points, u32 tail, u32 *colors, s32 count,
                                      s32 useFirstColor, s32 surfaceIndex);
extern void evtPrepareSolarOverlayTestState(s32 renderHandle);
extern void effDrawTextureSlotWithBlendedCornerColors(s32 x, s32 y, s32 depth, u32 blend, s32 flags,
                                                       s32 textureSetHandle, s32 slotIndex, s32 layer);





void evtLoadSolarNoiseSprite(struct EffectSlotSet **sprite) {
    *sprite = effLoadIndexedResource(D_00437210, "solarnoise.spr", 0);
}

void evtReleaseSolarNoiseSprite(struct EffectSlotSet **sprite) {
    effDestroyResourceSlotSet(*sprite);
}

void evtInitializeSolarOverlay(s32 renderHandle) {
    sdfSubmitGsTestOneRegisterPacket(0x30000, renderHandle);
    uiDrawUniformColorRect(0, 0, 0, 0x2000, 0xE00, 0, renderHandle);
    uiDrawActiveSurfaceRegion(renderHandle);
    sdfSubmitGsTestOneRegisterPacket(0x30000, renderHandle);
}

void evtFinalizeSolarOverlay(s32 renderHandle) {
    sdfDispatchSurfaceWithPreparedTexturePacket(renderHandle);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, renderHandle);
    sdfSubmitGsTestOneRegisterPacket(0x50000, renderHandle);
}

void evtPrepareSolarOverlayTestState(s32 renderHandle) {
    sdfSubmitGsAlphaOneRegisterPacket(0x44, renderHandle);
    sdfSubmitGsTestOneRegisterPacket(0x30000, renderHandle);
    uiDrawUniformColorRect(0, 0, 0, 0x2000, 0xE00, 0, renderHandle);
    sdfSubmitGsTestOneRegisterPacket(0x5100DL, renderHandle);
}

void evtDrawSolarNoiseLayerAtConfiguredScale(s32 x, s32 y, s32 z, s32 alpha, s32 layer, s32 mode,
    s32 contextHandle, s32 color) {
    {
        SolarSpriteContext *context = *(SolarSpriteContext **)contextHandle;
        s32 spriteOffset = layer * sizeof(SolarSpriteLayer);
        s32 scalePercent = evtSolarNoiseLayerPlacements[layer][2];
        SolarSpriteLayer *sprite =
            (SolarSpriteLayer *)(spriteOffset + (s32)context->layers);

        sprite->drawWidth = (s32)((f32)(sprite->width * scalePercent) / 100.0f) << 4;
        sprite->drawHeight = (s32)((f32)(sprite->height * evtSolarNoiseLayerPlacements[layer][2]) / 100.0f) << 3;
        effDrawTextureSlotWithBlendedCornerColors((x + evtSolarNoiseLayerPlacements[layer][0]) << 4,
                      (y + evtSolarNoiseLayerPlacements[layer][1]) << 3, z,
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
void evtDrawSolarNoiseLayerAtCenteredScale(s32 x, s32 y, s32 z, s32 alpha, s32 layer, s32 mode,
    s32 contextHandle, s32 color, f32 scale) {
    {
        SolarSpriteContext *context = *(SolarSpriteContext **)contextHandle;
        s32 spriteOffset = layer * sizeof(SolarSpriteLayer);
        s32 scalePercent = evtSolarNoiseLayerPlacements[layer][2];
        SolarSpriteLayer *sprite =
            (SolarSpriteLayer *)(spriteOffset + (s32)context->layers);

        sprite->drawWidth =
            (s32)((scale * (f32)sprite->width * (f32)scalePercent) / 100.0f) << 4;
        sprite->drawHeight =
            (s32)((scale * (f32)sprite->height * (f32)evtSolarNoiseLayerPlacements[layer][2]) / 100.0f) << 3;
        effDrawTextureSlotWithBlendedCornerColors(((x + evtSolarNoiseLayerPlacements[layer][0]) << 4) - (sprite->drawWidth / 2),
                      ((y + evtSolarNoiseLayerPlacements[layer][1]) << 3) - (sprite->drawHeight / 2), z,
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
void evtDrawSolarOverlayMarker(s32 unused0, s32 unused1, s32 alpha, s32 intensity, s32 object) {
    s32 positions[28] = {
        47, 48, 76, 48, 72, 63, 63, 75, 51, 79, 38, 75, 29, 64,
        27, 48, 29, 32, 38, 20, 51, 16, 63, 20, 72, 31, 76, 48,
    };
    u32 colors[14];
    s32 i;
    s32 color = 0x335072;

    colors[0] = ((s32)((f32)intensity * 0.2f) << 24) | color;
    for (i = 1; i < 14; i++) {
        colors[i] = color;
    }
    sdfDrawColoredTriangleFan(positions, alpha, colors, 14, 0, object);
}

/* Scale a selected overlay shape's alpha values and submit it between GS states. */
void evtDrawSolarOverlayShapeWithScaledAlpha(s32 unused0, s32 unused1, s32 alpha, s32 alphaScale,
    s32 entryIndex, s32 object) {
    u32 colors[4];
    s32 i;
    s32 tableIndex;
    u32 color;

    if (entryIndex == 0) {
        return;
    }

    sdfSubmitGsAlphaOneRegisterPacket(0x48, object);
    tableIndex = entryIndex - 1;
    for (i = 0; i < evtSolarOverlayShapes[tableIndex].count; i++) {
        color = evtSolarOverlayShapes[tableIndex].colors[i];
        colors[i] = (color & 0xFFFFFF) |
            ((s32)((f32)((s32)((color & 0xFF000000) >> 24) * alphaScale) * 0.0078125f) << 24);
    }
    sdfDrawColoredTriangleFan(evtSolarOverlayShapes[tableIndex].positions, alpha, colors,
                  evtSolarOverlayShapes[tableIndex].count, 0, object);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, object);
}

void evtDrawIndexedSolarOverlayLayers(s32 x, s32 y, s32 unused2, s32 alpha, s32 entryCount,
    s32 contextHandle, s32 renderHandle) {
    s32 n;
    u32 zeroColor;
    s32 *p;

    if (entryCount == 0) {
        return;
    }
    n = entryCount;
    evtInitializeSolarOverlay(renderHandle);
    zeroColor = 0;
    if (n < 8) {
        p = D_003C90DC + entryCount * 8;
        while (n < 8) {
            sdfDrawColoredTriangleFan((s32 *)p[-1], 0xFF, &zeroColor, p[0], 1, renderHandle);
            n++;
            p += 8;
        }
    }
    evtFinalizeSolarOverlay(renderHandle);
    evtDrawSolarNoiseLayerAtConfiguredScale(x, y, 0, alpha, 0xA, 0x20, contextHandle, renderHandle);
    evtPrepareSolarOverlayTestState(renderHandle);
}

void evtDrawPartialSolarOverlay(s32 x, s32 y, s32 unused2, s32 alpha, s32 entryCount,
    s32 contextHandle, s32 renderHandle) {
    s32 n;
    u32 zeroColor;
    s32 *p;

    if (entryCount == 0) {
        return;
    }
    n = entryCount;
    evtInitializeSolarOverlay(renderHandle);
    zeroColor = 0;
    if (n < 8) {
        p = D_003C90DC + entryCount * 8;
        while (n < 8) {
            sdfDrawColoredTriangleFan((s32 *)p[-1], 0xFF, &zeroColor, p[0], 1, renderHandle);
            n++;
            p += 8;
        }
    }
    evtFinalizeSolarOverlay(renderHandle);
    evtDrawSolarNoiseLayerAtConfiguredScale(x, y, 0, alpha, 0x10, 0x20, contextHandle, renderHandle);
    evtPrepareSolarOverlayTestState(renderHandle);
}

/* Draw the selected solar-noise layer; intermediate layers also receive layer 9. */
void evtDrawSolarLayerPair(s32 x, s32 y, s32 z, s32 width, s32 layer, s32 context, s32 color) {
    evtDrawSolarNoiseLayerAtConfiguredScale(x, y, z, width, layer, 0, context, color);
    if (layer != 0 && layer != 4 && layer != 8) {
        evtDrawSolarNoiseLayerAtConfiguredScale(x, y, z, width, 9, 0, context, color);
    }
}

/* Layer lifetimes differ, but both reset their activation byte on expiry. */
s32 evtAdvanceSolarShortLayerTimer(SolarNoiseLayer *timer) {
    s32 nextAge;

    nextAge = (u16)timer->age + 1;
    timer->age = nextAge;
    if ((f32)(s16)nextAge > 60.0f) {
        timer->age = 0;
        timer->active = 0;
    }
    return timer->active;
}

s32 evtAdvanceSolarLongLayerTimer(SolarNoiseLayer *timer) {
    s32 nextAge;

    nextAge = (u16)timer->age + 1;
    timer->age = nextAge;
    if ((f32)(s16)nextAge > 80.0f) {
        timer->age = 0;
        timer->active = 0;
    }
    return timer->active;
}


void evtAdvanceShortSolarNoiseLayers(SolarNoiseState *state) {
    u8 phase = evtGetMirroredSolarPhase();
    s32 spawnLimit = (s32)((f32)phase * 10.0f * 0.125f + 5.0f);
    SolarNoiseLayer *layer;
    s8 *active;
    s32 i;
    f32 angle;

    state->spawnAge++;
    if ((s16)state->spawnAge > state->spawnInterval) {
        state->spawnAge = 0;
        if (state->activeCount < 10 && state->activeCount < spawnLimit) {
            state->activeCount++;
            for (i = 0; i < 10; i++) {
                if (state->layers[i].active == 0) {
                    angle = sdfGetRandFloat(0) * 6.2831852f;
                    state->layers[i].x = (s32)((f32)state->centerX +
                        (f32)state->radiusX * sdfCos(angle));
                    state->layers[i].y = (s32)((f32)state->centerY +
                        (f32)state->radiusY * sdfSinPoly(angle));
                    state->layers[i].age = 0;
                    state->layers[i].active = 1;
                    state->layers[i].scale = (s32)(sdfGetRandFloat(0) * 100.0f + 100.0f);
                    break;
                }
            }
        }
    }
    layer = state->layers;
    active = &layer->active;
    for (i = 9; i >= 0; i--, layer++, active += sizeof(*layer)) {
        if (*active != 0 &&
            evtAdvanceSolarShortLayerTimer(layer) == 0) {
            state->activeCount--;
        }
    }
}

INCLUDE_SDATA(const s32, "game/code_002437F0", D_00437210);

void evtAdvanceLongSolarNoiseLayers(SolarNoiseState *state) {
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
                                sdfGetRand(0) % spawnCounts[phase - 2];
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
                evtAdvanceSolarLongLayerTimer(layer) == 0) {
                state->activeCount--;
            }
        }
    }
}

const char D_004221C8[16] = "FREE CAMERA";
static SolarNoiseDebugData evtSolarNoiseDebugData
    __attribute__((section(".sdata"))) = {(const char *)0x004221C8, 0, "%s"};

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
            evtDrawSolarNoiseLayerAtCenteredScale(layerX, layerY, z, (s32)value, 15, 0, context, color, scale);
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
            evtDrawSolarNoiseLayerAtCenteredScale(layerX, layerY, z, (s32)value, 15, 0, context, color, scale);
        }
    }
}

/* Activate the first inactive solar point. */
void evtActivateNextSolarPoint(SolarOverlayWork *overlay) {
    SolarPoint *points = overlay->state.points;
    s32 i;

    for (i = 0; i < 8; i++) {
        if (points[i].active == 0) {
            points[i].active = 1;
            break;
        }
    }
}

/* Deactivate the last active solar point. */
void evtDeactivateLastSolarPoint(SolarOverlayWork *overlay) {
    SolarPoint *points = overlay->state.points;
    s32 i;

    for (i = 7; i >= 0; i--) {
        if (points[i].active == 1) {
            points[i].active = 0;
            break;
        }
    }
}

void evtSetSolarPointActiveCount(SolarOverlayWork *overlay, u32 desiredCount) {
    SolarPoint *point;
    u8 *active;
    u32 activeCount;
    s32 i;

    activeCount = 0;
    point = overlay->state.points;
    active = &point->active;
    for (i = 0; i < 8; i++, active += sizeof(*point)) {
        if (*active == 1) {
            activeCount++;
        }
    }
    while (activeCount != desiredCount) {
        if (activeCount < desiredCount) {
            activeCount++;
            evtActivateNextSolarPoint(overlay);
        } else if (desiredCount < activeCount) {
            activeCount--;
            evtDeactivateLastSolarPoint(overlay);
        }
    }
}

/* Each active point restarts with a randomized duration near 120-150 frames. */
void evtUpdateSolarPointTimers(SolarOverlayWork *overlay) {
    SolarPoint *point = overlay->state.points;
    s32 i;
    for (i = 7; i >= 0; i--, point++) {
        if (point->active != 0) {
            s32 age = point->age + 1;
            point->age = age;
            if ((s16)age > point->duration) {
                point->age = 0;
                point->duration = (s16)(sdfGetRandFloat(0) * 30.0f + 120.0f);
            }
        } else {
            point->age = 0;
            point->duration = 0;
        }
    }
}

/* Pulse the eight solar-point fans: the first pass scales each fan's alpha
 * by its timer's half-period sine plus a phase term, the second by its
 * third-period sine. */
void evtDrawSolarPointTransitionShapes(s32 unused0, s32 unused1, s32 alpha, s32 alphaScale, s32 phase,
                                       SolarOverlayWork *overlay, s32 object) {
    u32 colors[8];
    SolarPoint *points = overlay->state.points;
    f32 base;
    f32 pulse;
    f32 t;
    s32 i;
    s32 j;

    evtUpdateSolarPointTimers(overlay);
    base = (f32)phase * 0.3f * 0.125f + 0.1f;
    sdfSubmitGsAlphaOneRegisterPacket(0x48, object);
    for (i = 0; i < 8; i++) {
        if (points[i].duration == 0) {
            break;
        }
        t = (f32)(s16)points[i].age / ((f32)points[i].duration * 0.5f);
        pulse = sdfSinPoly(t * 3.14159265f);

        if (pulse < 0.0f) {
            pulse = -pulse;
        }
        pulse = base + pulse * 0.4f;
        for (j = 0; j < evtSolarOverlayShapes[i].pointCount; j++) {
            colors[j] = (evtSolarOverlayShapes[i].pointColors[j] & 0xFFFFFF) |
                ((s32)((f32)(s32)((evtSolarOverlayShapes[i].pointColors[j] & 0xFF000000) >> 24) * pulse * (f32)alphaScale * 0.0078125f) << 24);
        }
        sdfDrawColoredTriangleFan(evtSolarOverlayShapes[i].pointPositions, alpha, colors, evtSolarOverlayShapes[i].pointCount, 0, object);
    }
    sdfSubmitGsAlphaOneRegisterPacket(0x48, object);
    for (i = 0; i < 8; i++) {
        if (points[i].duration == 0) {
            break;
        }
        t = (f32)(s16)points[i].age / ((f32)points[i].duration / 3.0f);
        pulse = sdfSinPoly(t * 3.14159265f);

        if (pulse < 0.0f) {
            pulse = -pulse;
        }
        pulse = pulse * 0.4f;
        for (j = 0; j < evtSolarOverlayShapes[i].count; j++) {
            colors[j] = (evtSolarOverlayShapes[i].colors[j] & 0xFFFFFF) |
                ((s32)((f32)(s32)((evtSolarOverlayShapes[i].colors[j] & 0xFF000000) >> 24) * pulse * (f32)alphaScale * 0.0078125f) << 24);
        }
        sdfDrawColoredTriangleFan(evtSolarOverlayShapes[i].positions, alpha, colors, evtSolarOverlayShapes[i].count, 0, object);
    }
    sdfSubmitGsAlphaOneRegisterPacket(0x44, object);
}

INCLUDE_RODATA(const s32, "game/code_002437F0", D_004221D8);

INCLUDE_RODATA(const s32, "game/code_002437F0", D_004221E8);

INCLUDE_RODATA(const s32, "game/code_002437F0", D_00422238);

INCLUDE_RODATA(const s32, "game/code_002437F0", D_00422308);

