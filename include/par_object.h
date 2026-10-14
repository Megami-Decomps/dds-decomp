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
    f32 scaleEnd;                /* 0x8C: rendered scale after interpolation. */
    f32 spinDegrees;             /* 0x90: converted to radians by the client update. */
    f32 billboardScaleJitter;     /* 0x94 */
    f32 initialPhaseJitter;       /* 0x98 */
    u8 alternateSpin;            /* 0x9C: selects the sign of particle rotation. */
    u8 pad9D[7];
    u32 restartStepCount;         /* 0xA4 */
    u8 padA8[8];
    f32 matrix[16];               /* 0xB0 */
    u32 parameterWord;           /* 0xF0: set through parSetObjectParameterWord. */
    BillObj *billboard;           /* 0xF4 */
    EffectBufferTail *buffer;     /* 0xF8 */
    u32 pendingRestartSteps;      /* 0xFC: count, not a pointer */
    f32 sourceMatrix[16];         /* 0x100 */
    u16 dispatchIndex;            /* 0x140: object dispatch, distinct from kind */
    u16 restartFlag;              /* 0x142 */
    u8 pad144[0xC];
} ParObj;

typedef char ParObj_size_must_be_0x150[(sizeof(ParObj) == 0x150) ? 1 : -1];
typedef char ParObj_scaleEnd_offset_must_be_0x8C[
    ((u32)&((ParObj *)0)->scaleEnd == 0x8C) ? 1 : -1];
typedef char ParObj_spinDegrees_offset_must_be_0x90[
    ((u32)&((ParObj *)0)->spinDegrees == 0x90) ? 1 : -1];
typedef char ParObj_alternateSpin_offset_must_be_0x9C[
    ((u32)&((ParObj *)0)->alternateSpin == 0x9C) ? 1 : -1];
typedef char ParObj_parameterWord_offset_must_be_0xF0[
    ((u32)&((ParObj *)0)->parameterWord == 0xF0) ? 1 : -1];

#endif /* PAR_OBJECT_H */
