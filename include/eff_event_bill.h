#ifndef DDS_EFF_EVENT_BILL_H
#define DDS_EFF_EVENT_BILL_H

#include "common.h"

struct SdfMemBlock;

/* Shared billboard-emitter parameters copied into each live set. Creation and
 * parameter copies clamp startDelaySpread, fadeIn, and fadeOut to at least
 * one; motionDelaySpread is consumed unchanged. */
typedef struct EffEventBillParams {
    f32 position[4];
    u32 particleCount;
    s32 duration;
    s32 startDelaySpread;
    s32 motionDelaySpread;
    f32 spawnRadius;
    u32 color;
    s16 drawMode; /* Forwarded to the billboard mode setter in direct draw. */
    u8 pad2A[2];
    s32 fadeIn;
    s32 fadeOut;
    u8 repeat;
    u8 pad35[3];
    f32 baseScale;
    f32 scaleRandomness;
    f32 scaleAmplitudeX;
    f32 scaleAmplitudeY;
    f32 scalePhaseStep;
    f32 angularSpeed;
    f32 verticalSpeed;
    f32 verticalSpeedRandomness;
    f32 lateralSpeed;
    f32 lateralSpeedRandomness;
    f32 accelerationPercent; /* Speeds multiply by 1 + this / 100 per step. */
    f32 swayPhaseStep;
    f32 swayAmplitude;
    f32 swayAmplitudeStep;
    f32 swayRandomness;
    s32 colorStartAge; /* Batched path fades primary texture RGB from black. */
    s32 colorTransitionFrames;
} EffEventBillParams;

typedef struct EffEventBillParticle {
    f32 position[3];
    u8 pad0C[4];
    f32 motion[3]; /* X/Z direction; Y accumulates vertical offset. */
    u8 pad1C[4];
    s32 age;
    s32 motionDelay;
    f32 baseScale;
    f32 scalePhaseX;
    f32 scalePhaseStepX;
    f32 scaleAmplitudeX;
    f32 scalePhaseY;
    f32 scalePhaseStepY;
    f32 scaleAmplitudeY;
    f32 rotation;
    f32 angularSpeed;
    f32 swayAmplitude;
    f32 swayPhase;
    f32 verticalSpeed;
    f32 lateralSpeed;
    u8 pad5C[4];
} EffEventBillParticle;

/* Particle storage is followed by this owner in the same allocation. */
typedef struct EffEventBillSet {
    EffEventBillParams head;
    EffEventBillParticle *particles;
    u8 flag;
    u8 pad81[3];
    struct SdfMemBlock *allocationHandle;
} EffEventBillSet;

typedef char EffEventBillParams_size[(sizeof(EffEventBillParams) == 0x7C) ? 1 : -1];
typedef char EffEventBillParticle_size[(sizeof(EffEventBillParticle) == 0x60) ? 1 : -1];
typedef char EffEventBillSet_size[(sizeof(EffEventBillSet) == 0x88) ? 1 : -1];
typedef char EffEventBillParams_count_offset[((u32)&((EffEventBillParams *)0)->particleCount == 0x10) ? 1 : -1];
typedef char EffEventBillParams_fade_offset[((u32)&((EffEventBillParams *)0)->fadeIn == 0x2C) ? 1 : -1];
typedef char EffEventBillParams_scale_offset[((u32)&((EffEventBillParams *)0)->baseScale == 0x38) ? 1 : -1];
typedef char EffEventBillParams_color_age_offset[((u32)&((EffEventBillParams *)0)->colorStartAge == 0x74) ? 1 : -1];
typedef char EffEventBillSet_particles_offset[((u32)&((EffEventBillSet *)0)->particles == 0x7C) ? 1 : -1];
typedef char EffEventBillSet_flag_offset[((u32)&((EffEventBillSet *)0)->flag == 0x80) ? 1 : -1];
typedef char EffEventBillSet_allocation_offset[((u32)&((EffEventBillSet *)0)->allocationHandle == 0x84) ? 1 : -1];

#endif
