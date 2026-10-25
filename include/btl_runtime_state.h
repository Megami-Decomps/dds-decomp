#ifndef BTL_RUNTIME_STATE_H
#define BTL_RUNTIME_STATE_H

#include "common.h"
#include "eff_ref_obj.h"

typedef u16 BtlRuntimeFadeState;
typedef enum BtlRuntimeFadeStateValue {
    BTL_RUNTIME_FADE_IDLE = 0,
    BTL_RUNTIME_FADE_SETUP = 1,
    BTL_RUNTIME_FADE_IN = 2,
    BTL_RUNTIME_FADE_OUT = 3,
    BTL_RUNTIME_FADE_CLEANUP = 4,
} BtlRuntimeFadeStateValue;

/* Fade work is pending at zero and the current step is complete at one. */
typedef s32 BtlRuntimeFadeStepResult;
enum {
    BTL_RUNTIME_FADE_STEP_IN_PROGRESS = 0,
    BTL_RUNTIME_FADE_STEP_COMPLETE = 1,
};

/* The battle's runtime state, as the overlay effects read it. */
#ifdef VERSION_DDS1
typedef struct BattleRuntimeState {
    s32 counter;
    BtlRuntimeFadeState state;
    u8 fadeMode; /* selects the initial overlay alpha in btlInitFadeColors */
    s8 pending;
    s8 active;
    u8 pad09[3];
    u32 options;
    u32 color10;
    u32 color14;
    u32 color18;
    u32 color1C;
    f32 phase;      /* 0x20: overlay sweep, advanced each frame by phaseSpeed */
    f32 phaseSpeed; /* 0x24: decays toward -0.02 */
    s32 gridWidth;
    s32 gridHeight;
    s32 cellWidth;
    s32 cellHeight;
    u8 *ownedData;
    u8 fadeOrderCursor;
    u8 pad3D[3];
    RefObj *resource;
    u8 *frameBuffer;
    struct sdfMemHandle_t *frameBufferAllocation;
} BattleRuntimeState;
#else
/* Native 32-byte overlay particle, initialized at 0x22D040 and drawn at 0x22D8A8. */
typedef struct BtlFadeParticle {
    f32 direction[3];
    f32 rotationAngle;
    f32 rotationSpeed;
    f32 displacement;
    f32 displacementSpeed;
    u8 alpha;
    u8 age;
    u8 delay;
    u8 pad1F;
} BtlFadeParticle;

typedef struct BattleRuntimeState {
    s32 counter; /* Active encounter updates since initialization. */
    BtlRuntimeFadeState state;
    u8 fadeMode; /* selects the initial overlay alpha in btlInitFadeColors */
    s8 pending;
    s8 active;
    u8 pad09[3];
    u32 options;
    u32 color10;
    u32 color14;
    s32 particlesPerRow;
    s32 particleRowCount;
    s32 particleGridHalfCellWidth;
    s32 particleGridHalfCellHeight;
    f32 waveRadius;
    f32 waveRadiusSpeed;
    f32 particleActivationRadius;
    f32 particleActivationRadiusStep;
    BtlFadeParticle *ownedData;
    u8 *frameBuffer;
    struct sdfMemHandle_t *frameBufferAllocation;
} BattleRuntimeState;
#endif

#endif
