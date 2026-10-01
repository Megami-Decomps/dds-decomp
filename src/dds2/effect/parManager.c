#include "common.h"

#include "ee_mmi.h"

#include "pcp_vu0.h"

#include "eff.h"

enum {
    PAR_BURST_RANDOM_AXIS = 1
};

typedef struct {
    u8 pad[0x30];    /* 0x0 */
    u16 kind;        /* 0x30 */
    u8 pad32[6];     /* 0x32 */
    f32 scale;       /* 0x38 */
    u8 pad3C[0x68];  /* 0x3C */
    u32 restartStepCount; /* 0xA4 copied to pendingRestartSteps on restart */
    u8 padA8[0x54];  /* 0xA8 */
    void *pendingRestartSteps; /* 0xFC native updater treats this word as a count, not a pointer */
    u8 pad100[0x40]; /* 0x100 */
    u16 dispatchIndex; /* 0x140 selects D_0034E250/D_0034E258/D_0034E2F0 */
    u16 restartFlag; /* 0x142 read by parGetRestartFlag, set to 1 by parRestartKind */
    u8 pad144[0x30]; /* 0x144 */
    void *child;      /* 0x174 released by parReleaseObject */
} ParObj;

typedef struct {
    u8 pad[0xC];
    void *resource; /* 0xC released by effParReleaseNodeResource */
} ParNode;

typedef struct {
    u8 pad[4];   /* 0x0 native renderer reads a point-buffer address */
    u16 pointCount; /* 0x4 number of billboard points emitted from this slot */
    u8 pad6[10]; /* 0x6 includes packed color at +0x8 and billboard scale at +0xC */
} ParSlot; /* 0x10 bytes */

typedef struct {
    u8 pad[4];
    ParSlot *slots;
} ParTable;

extern void (*D_003AAC20[])();

/* Particle dispatch entry (0xC bytes): command func selected by the
   u16 at +0x140. */
typedef struct {
    void *(*func)(); /* 0x0 */
    u32 unk4;        /* 0x4 */
    u32 unk8;        /* 0x8 */
} ParDispatch; /* 0xC bytes */

extern ParDispatch D_003AAB80[];

extern ParDispatch D_003AAB88[];

/* Release a non-null child, then the object's resources and chip block. */
void parReleaseObject(ParObj *obj) {
    s32 child;

    child = (s32)obj->child;
    if (child != 0) {
        func_003297C8(child);
    }
    effDestroyResources(obj);
    sdfReleaseChipBlock(obj);
}

typedef struct {
    u8 pad00[4];
    void *records;
} ParBurstBuffer;

typedef struct {
    f32 origin[4];
    f32 billboardScale;
    u8 pad14[0x10];
    s32 lifetimeFrames;
    u8 pad28[8];
    u8 kindState[0x64];
    f32 billboardScaleJitter;
    f32 initialPhaseJitter;
    u8 pad9C[0x14];
    f32 matrix[16];
    u8 padF0[8];
    ParBurstBuffer *buffer;
    u8 padFC[0x54];
    u8 axisMode;
    u8 pad151[3];
    u32 spawnDelayFrames;
    f32 initialRadius;
    u32 rotationStepDegrees; /* native lwc1 reads float bits; retain the existing u32 view */
    f32 targetRadius;
    f32 initialRadiusJitter;
    f32 targetRadiusJitter;
} ParBurstEmitter;

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
extern u8 D_003AA868[];
extern u8 D_00451F20[];
extern void parDispatchKindInit(void *, u32);

/* Initialize one 64-byte particle record and its kind-specific state. Negative
   ages delay activation; radial distance changes toward a jittered target over
   lifetimeFrames. RNG calls and the pre-transform length measurement stay in
   their original order. */
void func_00160B98(ParBurstEmitter *effect, u32 particleIndex) {
    ParBurstPacket *packet = (ParBurstPacket *)effect->buffer->records;
    f32 direction[4];
    f32 initialRadius;
    f32 jitterFactor;
    f32 initialRadiusLength;

    packet += particleIndex;
    packet->color = 0;
    packet->age = -(effMiscRand(D_00451F20) % (effect->spawnDelayFrames + 1));
    initialRadius = effect->initialRadius;
    jitterFactor = effect->initialRadiusJitter;
    direction[0] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    direction[1] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    direction[2] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, direction);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, direction);
    /* Each component gets a separate jitter sample, not one shared radius draw. */
    packet->radialOffset[0] = initialRadius * (effMiscRandUnitFloat(D_003AA868) * jitterFactor + (1.0f - jitterFactor)) * direction[0];
    packet->radialOffset[1] = initialRadius * (effMiscRandUnitFloat(D_003AA868) * jitterFactor + (1.0f - jitterFactor)) * direction[1];
    packet->radialOffset[2] = initialRadius * (effMiscRandUnitFloat(D_003AA868) * jitterFactor + (1.0f - jitterFactor)) * direction[2];
    VU0_LOAD_VF(vf10, packet->radialOffset);
    VU0_LENGTH_VF10(initialRadiusLength);
    VU0_LOAD_MATRIX(effect->matrix);
    VU0_LOAD_VF(vf10, packet->radialOffset);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF_UNCLOBBERED(vf10, packet->radialOffset);
    packet->position[0] = packet->radialOffset[0] + effect->origin[0];
    packet->position[1] = packet->radialOffset[1] + effect->origin[1];
    packet->position[2] = packet->radialOffset[2] + effect->origin[2];
    if (effect->axisMode == PAR_BURST_RANDOM_AXIS) {
        direction[0] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
        direction[1] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
        direction[2] = (effMiscRandUnitFloat(D_003AA868) - 0.5f) * 2.0f;
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
    packet->radiusStep = (effect->targetRadius * (effMiscRandUnitFloat(D_003AA868) * jitterFactor +
                                    (1.0f - jitterFactor)) - initialRadiusLength) /
                   effect->lifetimeFrames;
    jitterFactor = effect->billboardScaleJitter;
    packet->billboardScale = effect->billboardScale * (effMiscRandUnitFloat(D_003AA868) * jitterFactor +
                                         (1.0f - jitterFactor));
    jitterFactor = effect->initialPhaseJitter;
    if (jitterFactor != 0) {
        packet->initialPhaseRadians = (effMiscRandUnitFloat(D_003AA868) * jitterFactor + (1.0f - jitterFactor)) *
                       (3.14159265f * 2.0f);
    } else {
        packet->initialPhaseRadians = 0;
    }
    parDispatchKindInit(effect->kindState, particleIndex);
}

INCLUDE_ASM(const s32, "effect/parManager", func_00160EF8);

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

void func_001612D8(ParColorRamp *p, s32 frames) {
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

INCLUDE_ASM(const s32, "effect/parManager", func_001616A8);

INCLUDE_ASM(const s32, "effect/parManager", func_001617F8);

/* Release the allocation handle stored in this node, not the node itself. */
void effParReleaseNodeResource(ParNode *node) {
    func_003297C8(node->resource);
}

INCLUDE_ASM(const s32, "effect/parManager", func_001618E0);

INCLUDE_ASM(const s32, "effect/parManager", func_00161958);

INCLUDE_ASM(const s32, "effect/parManager", func_00161A10);

/* Disable this slot's billboard points by clearing its count, not a bit flag. */
void parClearSlotFlag(ParTable *table, s32 slotIndex) {
    table->slots[slotIndex].pointCount = 0;
}

INCLUDE_ASM(const s32, "effect/parManager", func_00161B38);

INCLUDE_ASM(const s32, "effect/parManager", func_00161D08);

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

    createdObject = D_003AAB80[dispatchIndex].func(creationData);
    createdObject->dispatchIndex = dispatchIndex;
}

/* Invoke the selected command with the existing empty argument list. */
void parDispatchByKind(ParObj *obj) {
    D_003AAB88[obj->dispatchIndex].func();
}

INCLUDE_ASM(const s32, "effect/parManager", func_00161FE8);

/* Create another object of the same dispatch index; the existing call forwards
   no explicit arguments and this interface does not return the new object. */
void parCloneKind(ParObj *obj) {
    ParObj *createdObject;

    createdObject = D_003AAB80[obj->dispatchIndex].func();
    createdObject->dispatchIndex = obj->dispatchIndex;
}

/* Reissue the callback, reload the native updater's repeat count, and arm restart. */
void parRestartKind(ParObj *obj) {
    D_003AAC20[obj->dispatchIndex]();
    obj->pendingRestartSteps = (void *)obj->restartStepCount;
    obj->restartFlag = 1;
}

extern void (*D_003AAC58[])(ParObj *, f32);

/* Call the selected entry with factor, additionally scale kinds 2..4, then restart. */
void parScaleAndRestartKind(ParObj *obj, f32 factor) {
    D_003AAC58[obj->dispatchIndex](obj, factor);
    switch (obj->kind) {
    case 2:
        obj->scale *= factor;
        break;
    case 3:
        obj->scale *= factor;
        break;
    case 4:
        obj->scale *= factor;
        break;
    }
    parRestartKind(obj);
}

/* Return the full 16-bit restart state without converting it to a boolean. */
u16 parGetRestartFlag(ParObj *obj) {
    return obj->restartFlag;
}

/* Copy one 16-byte vector with the existing EE/VU copy primitive. */
void func_001622D8(void *destination, void *source) {
    PCP_COPY_VECTOR(destination, source);
}

/* vu0 routine: effect+0xB0 = matrix * effect+0x100 via sdfComposeVuMatrixFromRegisters */
void parComposeEffectTransformMatrices(u8 *effect, void *matrix) {
    VU0_LOAD_MATRIX(matrix);
    VU0_LOAD_MATRIX_B(effect + 0x100);
    sdfComposeVuMatrixFromRegisters();
    VU0_STORE_MATRIX(effect + 0xB0);
}
