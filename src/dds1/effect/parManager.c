#include "common.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"

enum {
    PAR_BURST_RANDOM_AXIS = 1
};

/* Colour-ramp setup for a particle emitter: masks the three colours to 24 bits, derives the fade-in/out
   steps from the alpha byte, then the per-channel (colour0 -> colour1 -> colour2) steps per frame. */
typedef struct ParColorRamp {
    s32 mode;          /* 0x00: 1 = no ramp, 2 = ramp color0->color1 only */
    u32 color0;        /* 0x04 */
    u32 color1;        /* 0x08 */
    u32 color2;        /* 0x0C */
    u32 alpha;         /* 0x10 */
    s32 fadeInFrames;  /* 0x14 */
    s32 fadeOutFrames; /* 0x18 */
    s32 frames;        /* 0x1C */
    s32 rampFrames;    /* 0x20 */
    f32 r01;           /* 0x24 per-channel step color0 -> color1 */
    f32 r12;           /* 0x28 per-channel step color1 -> color2 */
    f32 g01;           /* 0x2C */
    f32 g12;           /* 0x30 */
    f32 b01;           /* 0x34 */
    f32 b12;           /* 0x38 */
    u32 fadeIn;        /* 0x3C */
    u32 fadeOut;       /* 0x40 */
} ParColorRamp;

typedef struct {
    u128 *points;
    u16 pointCount;
    u8 pad06[2];
    u32 color;
    f32 billboardScale;
} ParSlot; /* 0x10 */

/* The slot table and its allocation owner are one record, not two views.
 * The native allocator returns this header after the point/slot arrays. */
typedef struct {
    u16 slotCount;
    u16 pointCapacity;
    ParSlot *slots;
    void *billboardRef;
    u32 resource;
} ParTable; /* 0x10 */

/* Kind 1 uses a point-history table; kinds 2..4 use the same word as a
 * floating-point scale. Only those scaled kinds reach the scale accessor. */
typedef struct {
    u16 kind;
    u8 pad02[6];
    union {
        ParTable *table;
        f32 scale;
    } value;
    u8 pad0C[0xC];
} ParKindState; /* 0x18 */

/* Record contents depend on the emitter; radial records are ParBurstPacket. */
typedef struct {
    u32 allocation;
    void *records;
} ParBuffer; /* 0x08 */

/* Common emitter header. Radial-only fields belong to ParBurstEmitter's tail. */
typedef struct {
    f32 origin[4];                /* 0x00 */
    f32 billboardScale;           /* 0x10: radial packet's base scale */
    u8 pad14[0xC];
    s32 particleCount;            /* 0x20 */
    s32 lifetimeFrames;           /* 0x24 */
    u8 pad28[8];
    ParKindState kindState;       /* 0x30 */
    ParColorRamp colorRamp;       /* 0x48 */
    u8 pad8C[8];
    f32 billboardScaleJitter;     /* 0x94 */
    f32 initialPhaseJitter;       /* 0x98 */
    u8 pad9C[8];
    u32 restartStepCount;         /* 0xA4 */
    u8 padA8[8];
    f32 matrix[16];               /* 0xB0 */
    u8 padF0[8];
    ParBuffer *buffer;            /* 0xF8 */
    u32 pendingRestartSteps;      /* 0xFC: count, not a pointer */
    f32 sourceMatrix[16];         /* 0x100 */
    u16 dispatchIndex;            /* 0x140: object dispatch, distinct from kind */
    u16 restartFlag;              /* 0x142 */
    u8 pad144[0xC];
} ParObj; /* 0x150 */

typedef struct {
    ParObj head;
    u8 axisMode;                 /* 0x150 */
    u8 loop;                     /* 0x151: rearm expired particles */
    u8 pad152[2];
    u32 spawnDelayFrames;        /* 0x154 */
    f32 initialRadius;           /* 0x158 */
    f32 rotationStepDegrees;     /* 0x15C: native updater converts to radians */
    f32 targetRadius;
    f32 initialRadiusJitter;
    f32 targetRadiusJitter;
    u8 pad16C[8];
    u32 childHandle;             /* 0x174: additional owned allocation */
    f32 previousOrigin[3];       /* 0x178: native updater caches origin here */
} ParBurstEmitter; /* 0x184 */


/* Particle dispatch entry (0xC bytes): command func selected by the
   u16 at +0x140. */
typedef struct {
    void *(*func)(); /* 0x0 */
    u32 unk4;        /* 0x4 */
    u32 unk8;        /* 0x8 */
} ParDispatch; /* 0xC bytes */

extern ParDispatch parKindConstructorEntries[];
extern ParDispatch D_0034E258[];
extern void (*D_0034E2F0[])();

void func_002D0918(u32 handle);
void effDestroyResources(void *arg);
void sdfReleaseChipBlock(void *arg);
extern void sdfComposeVuMatrixFromRegisters(void);

/* Release the radial emitter's extra allocation, shared resources, and block. */
void parReleaseObject(ParBurstEmitter *obj) {
    if (obj->childHandle != 0) {
        func_002D0918(obj->childHandle);
    }
    effDestroyResources(obj);
    sdfReleaseChipBlock(obj);
}


/* The native updater rotates radialOffset and rebuilds position = origin +
   radialOffset; it does not integrate this vector as a velocity. */
typedef struct {
    f32 position[4];
    f32 radialOffset[3];
    u8 pad1C[4];
    s32 age;
    u32 color;
    f32 billboardScale;
    f32 initialPhaseRadians; /* initialized as a full-turn phase; later use is not established */
    f32 rotationAxis[3];
    f32 radiusStep;
} ParBurstPacket;

extern s32 effMiscRand(void *);
extern f32 effMiscRandUnitFloat(void *);
extern u8 D_0034DF38[];
extern u8 effEmitterDelayRandomState[];

/* Initialize one 64-byte particle record and its kind-specific state. Negative
   ages delay activation; radial distance changes toward a jittered target over
   lifetimeFrames. RNG calls and the pre-transform length measurement stay in
   their original order. */
void parInitializeRadialParticle(ParBurstEmitter *effect, u32 particleIndex) {
    ParBurstPacket *packet = effect->head.buffer->records;
    f32 direction[4];
    f32 initialRadius;
    f32 jitterFactor;
    f32 initialRadiusLength;

    packet += particleIndex;
    packet->color = 0;
    packet->age = -(effMiscRand(effEmitterDelayRandomState) % (effect->spawnDelayFrames + 1));
    initialRadius = effect->initialRadius;
    jitterFactor = effect->initialRadiusJitter;
    direction[0] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    direction[1] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    direction[2] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, direction);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, direction);
    /* Each component gets a separate jitter sample, not one shared radius draw. */
    packet->radialOffset[0] = initialRadius * (effMiscRandUnitFloat(D_0034DF38) * jitterFactor + (1.0f - jitterFactor)) * direction[0];
    packet->radialOffset[1] = initialRadius * (effMiscRandUnitFloat(D_0034DF38) * jitterFactor + (1.0f - jitterFactor)) * direction[1];
    packet->radialOffset[2] = initialRadius * (effMiscRandUnitFloat(D_0034DF38) * jitterFactor + (1.0f - jitterFactor)) * direction[2];
    VU0_LOAD_VF(vf10, packet->radialOffset);
    VU0_LENGTH_VF10(initialRadiusLength);
    VU0_LOAD_MATRIX(effect->head.matrix);
    VU0_LOAD_VF(vf10, packet->radialOffset);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF_UNCLOBBERED(vf10, packet->radialOffset);
    packet->position[0] = packet->radialOffset[0] + effect->head.origin[0];
    packet->position[1] = packet->radialOffset[1] + effect->head.origin[1];
    packet->position[2] = packet->radialOffset[2] + effect->head.origin[2];
    if (effect->axisMode == PAR_BURST_RANDOM_AXIS) {
        direction[0] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
        direction[1] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
        direction[2] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
        VU0_LOAD_VF(vf10, direction);
        VU0_NORMALIZE_VF10();
        VU0_STORE_VF(vf10, direction);
        packet->rotationAxis[0] = direction[0];
        packet->rotationAxis[1] = direction[1];
        packet->rotationAxis[2] = direction[2];
    } else {
        packet->rotationAxis[0] = 0;
        packet->rotationAxis[1] = -1.0f;
        packet->rotationAxis[2] = 0;
    }
    jitterFactor = effect->targetRadiusJitter;
    packet->radiusStep = (effect->targetRadius * (effMiscRandUnitFloat(D_0034DF38) * jitterFactor +
                                    (1.0f - jitterFactor)) - initialRadiusLength) /
                   effect->head.lifetimeFrames;
    jitterFactor = effect->head.billboardScaleJitter;
    packet->billboardScale = effect->head.billboardScale * (effMiscRandUnitFloat(D_0034DF38) * jitterFactor +
                                         (1.0f - jitterFactor));
    jitterFactor = effect->head.initialPhaseJitter;
    if (jitterFactor != 0) {
        packet->initialPhaseRadians = (effMiscRandUnitFloat(D_0034DF38) * jitterFactor + (1.0f - jitterFactor)) *
                       (3.14159265f * 2.0f);
    } else {
        packet->initialPhaseRadians = 0;
    }
    parDispatchKindInit(&effect->head.kindState, particleIndex);
}

INCLUDE_ASM(const s32, "effect/parManager", func_00159308);


void parInitColorRamp(ParColorRamp *p, s32 frames) {
    u32 alpha;
    f32 span;
    u32 a, b;

    p->color0 &= 0xFFFFFF;
    p->color1 &= 0xFFFFFF;
    p->color2 &= 0xFFFFFF;
    alpha = p->alpha << 24;
    p->fadeIn = alpha;
    if (p->fadeInFrames > 0) {
        p->fadeIn = alpha / p->fadeInFrames;
    }
    p->fadeOut = alpha;
    if (p->fadeOutFrames > 0) {
        p->fadeOut = alpha / p->fadeOutFrames;
    }
    p->frames = frames;
    if (p->mode == 1) {
        return;
    }
    if (p->mode == 2) {
        p->rampFrames = frames;
    } else {
        p->rampFrames = frames >> 1;
    }
    span = p->rampFrames;
    if (span == 0) {
        span = 1.0f;
    }
    a = p->color1;
    b = p->color0;
    p->r01 = ((f32)(a & 0xFF) - (f32)(b & 0xFF)) / span;
    p->g01 = ((f32)((a >> 8) & 0xFF) - (f32)((b >> 8) & 0xFF)) / span;
    p->b01 = ((f32)((a >> 16) & 0xFF) - (f32)((b >> 16) & 0xFF)) / span;
    if (p->mode == 2) {
        return;
    }
    a = p->color2;
    b = p->color1;
    p->r12 = ((f32)(a & 0xFF) - (f32)(b & 0xFF)) / span;
    p->g12 = ((f32)((a >> 8) & 0xFF) - (f32)((b >> 8) & 0xFF)) / span;
    p->b12 = ((f32)((a >> 16) & 0xFF) - (f32)((b >> 16) & 0xFF)) / span;
}

INCLUDE_ASM(const s32, "effect/parManager", func_00159AB8);

INCLUDE_ASM(const s32, "effect/parManager", func_00159C08);

/* Release the slot table's allocation handle, not a separate node object. */
void effParReleaseNodeResource(ParTable *table) {
    func_002D0918(table->resource);
}

INCLUDE_ASM(const s32, "effect/parManager", func_00159CF0);

INCLUDE_ASM(const s32, "effect/parManager", func_00159D68);

INCLUDE_ASM(const s32, "effect/parManager", func_00159E20);

/* Disable this slot's billboard points by clearing its count, not a bit flag. */
void parClearSlotFlag(ParTable *table, s32 slotIndex) {
    table->slots[slotIndex].pointCount = 0;
}

INCLUDE_ASM(const s32, "effect/parManager", func_00159F48);

INCLUDE_ASM(const s32, "effect/parManager", func_0015A118);

/* vu0 routine: modulate two RGBA8888 colours, (a/128 * b/128) * 128 per channel */
u32 effParModulateColors(u32 colorA, u32 colorB) {
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit = 0x3C000000;
    color1[0] = colorA;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = colorB;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    return blended[0];
}

/* Create through the selected dispatch entry and record its index; returns void. */
void parCreateIndexed(s32 dispatchIndex, void *creationData) {
    ParObj *createdObject;

    createdObject = parKindConstructorEntries[dispatchIndex].func(creationData);
    createdObject->dispatchIndex = dispatchIndex;
}

/* Invoke the selected command with the existing empty argument list. */
void parDispatchByKind(ParObj *obj) {
    D_0034E258[obj->dispatchIndex].func();
}

INCLUDE_ASM(const s32, "effect/parManager", func_0015A3F8);

/* Create another object of the same dispatch index; the existing call forwards
   no explicit arguments and this interface does not return the new object. */
void parCloneKind(ParObj *obj) {
    ParObj *createdObject;

    createdObject = parKindConstructorEntries[obj->dispatchIndex].func();
    createdObject->dispatchIndex = obj->dispatchIndex;
}

/* Reissue the callback, reload the native updater's repeat count, and arm restart. */

void parRestartKind(ParObj *obj) {
    D_0034E2F0[obj->dispatchIndex]();
    obj->pendingRestartSteps = obj->restartStepCount;
    obj->restartFlag = 1;
}

extern void (*D_0034E328[])(ParObj *, f32);

/* Call the selected entry with factor, additionally scale kinds 2..4, then restart. */
void parScaleAndRestartKind(ParObj *obj, f32 factor) {
    D_0034E328[obj->dispatchIndex](obj, factor);
    switch (obj->kindState.kind) {
    case 2:
        obj->kindState.value.scale *= factor;
        break;
    case 3:
        obj->kindState.value.scale *= factor;
        break;
    case 4:
        obj->kindState.value.scale *= factor;
        break;
    }
    parRestartKind(obj);
}

/* Return the full 16-bit restart state without converting it to a boolean. */
u16 parGetRestartFlag(ParObj *obj) {
    return obj->restartFlag;
}

/* Copy one 16-byte vector with the existing EE/VU copy primitive. */
void parCopyVectorB(void *destination, void *source) {
    PCP_COPY_VECTOR(destination, source);
}

/* vu0 routine: compose the supplied matrix with the emitter's source matrix. */
void parComposeEffectTransformMatrices(ParObj *effect, void *matrix) {
    VU0_LOAD_MATRIX(matrix);
    VU0_LOAD_MATRIX_B(effect->sourceMatrix);
    sdfComposeVuMatrixFromRegisters();
    VU0_STORE_MATRIX(effect->matrix);
}
