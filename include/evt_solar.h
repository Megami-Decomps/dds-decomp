#ifndef EVT_SOLAR_H
#define EVT_SOLAR_H

#include "common.h"
#include "kwln.h"

/* Sprite resource layer stride used by the paired solar render helpers. */
typedef struct SolarSpriteLayer {
    u8 pad00[0x0C];
    s32 drawWidth;
    s32 drawHeight;
    u8 pad14[0x68];
    s32 width;
    s32 height;
    u8 pad84[0x1C];
} SolarSpriteLayer;

/* Debug text bundle initialized with the solar-noise camera label and "%s". */
typedef struct SolarNoiseDebugData {
    const char *label;
    u32 opaque04;
    char format[4];
} SolarNoiseDebugData;

/* Resource context with the sprite-layer array pointer at +0x18. */
typedef struct SolarSpriteContext {
    u8 opaque00[0x18];
    SolarSpriteLayer *layers;
} SolarSpriteContext;

/* Indexed triangle-fan table view: position/color arrays and their count. */
typedef struct SolarOverlayShape {
    u8 opaque00[0x0C];
    s32 *positions;
    u32 *colors;
    s32 count;
    u8 opaque18[8];
} SolarOverlayShape;

typedef char SolarNoiseDebugDataFormatOffsetAssert[
    ((u32)&((SolarNoiseDebugData *)0)->format == 8) ? 1 : -1];
typedef char SolarSpriteContextLayersOffsetAssert[
    ((u32)&((SolarSpriteContext *)0)->layers == 0x18) ? 1 : -1];
typedef char SolarOverlayShapeDrawFieldsOffsetAssert[
    ((u32)&((SolarOverlayShape *)0)->positions == 0x0C &&
     (u32)&((SolarOverlayShape *)0)->colors == 0x10 &&
     (u32)&((SolarOverlayShape *)0)->count == 0x14) ? 1 : -1];

typedef char SolarSpriteLayer_size_must_be_0xA0[
    (sizeof(SolarSpriteLayer) == 0xA0) ? 1 : -1];
typedef char SolarSpriteLayer_drawWidth_offset_check[
    ((u32)&((SolarSpriteLayer *)0)->drawWidth == 0x0C) ? 1 : -1];
typedef char SolarSpriteLayer_drawHeight_offset_check[
    ((u32)&((SolarSpriteLayer *)0)->drawHeight == 0x10) ? 1 : -1];
typedef char SolarSpriteLayer_width_offset_check[
    ((u32)&((SolarSpriteLayer *)0)->width == 0x7C) ? 1 : -1];
typedef char SolarSpriteLayer_height_offset_check[
    ((u32)&((SolarSpriteLayer *)0)->height == 0x80) ? 1 : -1];

typedef struct SolarNoiseLayer {
    s16 x;
    s16 y;
    s16 age;
    s8 active;
    u8 scale;
} SolarNoiseLayer;

typedef struct SolarNoiseState {
    s16 centerX;
    s16 centerY;
    s16 radiusX;
    s16 radiusY;
    u16 spawnAge;
    s16 spawnInterval;
    u16 activeCount;
    u16 unk0E;
    SolarNoiseLayer layers[10];
} SolarNoiseState;

typedef struct SolarPoint {
    u16 age;
    s16 duration;
    u8 active;
    u8 pad05;
} SolarPoint;

/* The noise sprite has its own resource owner; animation state starts at +4. */
typedef struct SolarOverlayState {
    u32 flags;
    s32 transitionTimer;
    SolarPoint points[8];
    SolarNoiseState firstNoise;
    SolarNoiseState secondNoise;
    u8 solarPhase;
    u8 padF9[3];
    s32 pulseFrame; /* 0xFC: overlay +0x100 cycles through 0..120 in the frame draw. */
} SolarOverlayState;

struct EffectSlotSet;

typedef struct SolarOverlayWork {
    struct EffectSlotSet *noiseSprite;
    SolarOverlayState state;
} SolarOverlayWork;

typedef char SolarPoint_size_must_be_0x06[(sizeof(SolarPoint) == 0x06) ? 1 : -1];
typedef char SolarNoiseLayer_size_must_be_0x08[(sizeof(SolarNoiseLayer) == 0x08) ? 1 : -1];
typedef char SolarNoiseState_size_must_be_0x60[(sizeof(SolarNoiseState) == 0x60) ? 1 : -1];
typedef char SolarOverlayState_size_must_be_0x100[(sizeof(SolarOverlayState) == 0x100) ? 1 : -1];
typedef char SolarOverlayWork_size_must_be_0x104[(sizeof(SolarOverlayWork) == 0x104) ? 1 : -1];
typedef char SolarOverlayState_pulseFrame_offset_check[
    ((u32)&((SolarOverlayState *)0)->pulseFrame == 0xFC) ? 1 : -1];

s32 evtGetSolarPhase(void);
s32 evtGetMirroredSolarPhase(void);
void evtInitializeVisualData(SolarOverlayWork *overlay);
void evtUpdateSolarPhaseTransition(SolarOverlayWork *overlay);
void evtDrawFadingSolarOverlayFrame(s32 x, s32 y, s32 z, s32 alpha,
    s32 mirroredPhase, SolarOverlayWork *overlay, s32 renderContext);
void evtAdvanceSolarOverlayFadeAndDraw(s32 x, s32 y, s32 z, s32 alpha,
    SolarOverlayWork *overlay, s32 renderContext);
void evtActivateNextSolarPoint(SolarOverlayWork *overlay);
void evtDeactivateLastSolarPoint(SolarOverlayWork *overlay);
void evtSetSolarPointActiveCount(SolarOverlayWork *overlay, u32 desiredCount);
void evtUpdateSolarPointTimers(SolarOverlayWork *overlay);
void evtLoadSolarNoiseSprite(struct EffectSlotSet **sprite);
void evtReleaseSolarNoiseSprite(struct EffectSlotSet **sprite);

extern KwlnTask *evtSolarOverlayTask;
s32 evtCreateSolarOverlayWork(KwlnTask *task);
s32 evtUpdateSolarOverlayFade(KwlnTask *task);
void evtFreeSolarOverlayWork(KwlnTask *task);

#endif /* EVT_SOLAR_H */
