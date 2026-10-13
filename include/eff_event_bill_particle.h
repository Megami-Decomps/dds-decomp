#ifndef DDS_EFF_EVENT_BILL_PARTICLE_H
#define DDS_EFF_EVENT_BILL_PARTICLE_H

#include "common.h"

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

typedef char EffEventBillParticle_size[(sizeof(EffEventBillParticle) == 0x60) ? 1 : -1];
typedef char EffEventBillParticle_position_offset[(((u32)&((EffEventBillParticle *)0)->position) == 0x00) ? 1 : -1];
typedef char EffEventBillParticle_motion_offset[(((u32)&((EffEventBillParticle *)0)->motion) == 0x10) ? 1 : -1];
typedef char EffEventBillParticle_age_offset[(((u32)&((EffEventBillParticle *)0)->age) == 0x20) ? 1 : -1];

#endif
