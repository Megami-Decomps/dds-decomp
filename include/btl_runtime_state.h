#ifndef BTL_RUNTIME_STATE_H
#define BTL_RUNTIME_STATE_H

#include "common.h"
#include "eff_ref_obj.h"

/* The battle's runtime state, as the overlay effects read it. */
#ifdef VERSION_DDS1
typedef struct BattleRuntimeState {
    s32 counter;
    u16 state;
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
    u8 unk_3C[4];
    RefObj *resource;
    void *request;
    void *handle;
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
    u16 state;
    u8 fadeMode; /* selects the initial overlay alpha in btlInitFadeColors */
    s8 pending;
    s8 active;
    u8 pad09[3];
    u32 options;
    u32 color10;
    u32 color14;
    s32 particlesPerRow;
    s32 particleRowCount;
    s32 unk20;
    s32 unk24;
    f32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    BtlFadeParticle *ownedData;
    void *resource;
    void *handle;
} BattleRuntimeState;
#endif

#endif
