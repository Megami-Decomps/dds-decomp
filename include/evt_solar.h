#ifndef EVT_SOLAR_H
#define EVT_SOLAR_H

#include "common.h"
#include "kwln.h"

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

/* The noise sprite has a separate handle; all animation state starts at +4. */
typedef struct SolarOverlayState {
    u32 flags;
    s32 transitionTimer;
    SolarPoint points[8];
    SolarNoiseState firstNoise;
    SolarNoiseState secondNoise;
    u8 solarPhase;
    u8 padF9[7];
} SolarOverlayState;

typedef struct SolarOverlayWork {
    u32 noiseSprite;
    SolarOverlayState state;
} SolarOverlayWork;

typedef char SolarPoint_size_must_be_0x06[(sizeof(SolarPoint) == 0x06) ? 1 : -1];
typedef char SolarNoiseLayer_size_must_be_0x08[(sizeof(SolarNoiseLayer) == 0x08) ? 1 : -1];
typedef char SolarNoiseState_size_must_be_0x60[(sizeof(SolarNoiseState) == 0x60) ? 1 : -1];
typedef char SolarOverlayState_size_must_be_0x100[(sizeof(SolarOverlayState) == 0x100) ? 1 : -1];
typedef char SolarOverlayWork_size_must_be_0x104[(sizeof(SolarOverlayWork) == 0x104) ? 1 : -1];

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
void evtLoadSolarNoiseSprite(u32 *sprite);
void evtReleaseSolarNoiseSprite(u32 *sprite);

extern KwlnTask *evtSolarOverlayTask;
s32 evtCreateSolarOverlayWork(KwlnTask *task);
s32 evtUpdateSolarOverlayFade(KwlnTask *task);
void evtFreeSolarOverlayWork(KwlnTask *task);

#endif /* EVT_SOLAR_H */
