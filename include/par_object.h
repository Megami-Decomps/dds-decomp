#ifndef PAR_OBJECT_H
#define PAR_OBJECT_H

#include "common.h"
#include "eff.h"
#include "par_color_ramp.h"

/* Common emitter header. Radial-only fields belong to ParBurstEmitter's tail. */
typedef struct ParObj {
    f32 origin[4];                /* 0x00 */
    f32 billboardScale;           /* 0x10: radial packet's base scale */
    f32 billboardScaleY;          /* 0x14 */
    u8 pad18[8];
    s32 particleCount;            /* 0x20 */
    s32 lifetimeFrames;           /* 0x24 */
    s32 billboardCloneMarker;     /* 0x28: PAR_BILLBOARD_CLONE_FROM_RESOURCE */
    s16 billboardMode;            /* 0x2C */
    u8 pad2E[2];
    ParKindState kindState;       /* 0x30 */
    ParColorRamp colorRamp;       /* 0x48 */
    u8 pad8C[8];
    f32 billboardScaleJitter;     /* 0x94 */
    f32 initialPhaseJitter;       /* 0x98 */
    u8 pad9C[8];
    u32 restartStepCount;         /* 0xA4 */
    u8 padA8[8];
    f32 matrix[16];               /* 0xB0 */
    u8 padF0[4];
    BillObj *billboard;           /* 0xF4 */
    EffectBufferTail *buffer;     /* 0xF8 */
    u32 pendingRestartSteps;      /* 0xFC: count, not a pointer */
    f32 sourceMatrix[16];         /* 0x100 */
    u16 dispatchIndex;            /* 0x140: object dispatch, distinct from kind */
    u16 restartFlag;              /* 0x142 */
    u8 pad144[0xC];
} ParObj;

#endif /* PAR_OBJECT_H */
