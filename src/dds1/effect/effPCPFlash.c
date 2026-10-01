#include "common.h"
#include "eff.h"
#include "pcp_vu0.h"

extern void *effParamTableGetBlock(void *data, s32 index);

extern void effReleaseRecordPoolResourceAndBuffer(u32 res);
extern void func_00170350(u32 res);
extern void effReleaseRecordGroupAssetAndHandle(u32 res);
extern void func_002D0918(u32 res);
extern s32 effGetGroupIndexRecord(s32 base, s32 index);
extern s32 effMultiplyPackedColors(s32 color, s32 param);
extern u8 D_0034DF38[];
extern f32 *effGetIndexedEffectGroupRecord(u32 handle, s32 index);
extern f32 D_00354900[];
extern f32 D_00354910[];
extern f32 D_00354960[];
extern f32 D_00354930[];
extern f32 D_00354940[];
extern f32 D_00354950[];
extern f32 D_003548F0[];
extern f32 D_00354970[];
extern f32 *effGetGroupRecordByIndex(u32 handle, s32 index);
extern f32 *func_00170538(u32 handle, s32 index);
extern f32 sdfEvaluateCosineViaSinePhaseShift(f32 angle);
extern f32 sdfSinPoly(f32 angle);

extern void sdfBuildVuRotationFromAxisAngle(f32 angle, void *orientation);


/* Effect initializers implemented in assembly below. Each is entered both with
   and without spawn arguments, so they are declared unchecked. */
extern void effFlashRecordCreate();
extern void func_0016A6C0();
extern void *func_0016C0E8();

typedef struct PcpFlashParticle10 {
    u32 color;
    s32 age;
    f32 scale;
    u8 pad0C[0x04];
} PcpFlashParticle10;

typedef struct PcpFlashWork1 PcpFlashWork1;

struct PcpFlashWork1 {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    s32 rampTime;
    u32 colorA;
    u32 colorB;
    f32 maxScale;
    u8 pad2C[0x04];
    struct PcpFlashParticle10 *parts;
    s32 updateCount;
    u32 colorParam;
    f32 renderScale;
    u32 ownedBuffer;
    u32 resourceHandle;
};

typedef struct PcpFlashColorSlot {
    s32 first;
    s32 second;
    s32 third;
} PcpFlashColorSlot;

typedef struct PcpFlashStreakWork PcpFlashStreakWork;

typedef struct PcpFlashRotatingParticle PcpFlashRotatingParticle;

/* Spawn, rotation and draw passes share this 0x58-byte streak work.
   The position-rotation routine uses the same parts pointer at 0x40. */
struct PcpFlashStreakWork {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    s32 rampTime;
    u32 randomRange;
    u32 colorA;
    u32 colorB;
    f32 upSpan;
    f32 acrossSpan;
    f32 maxScale;
    f32 initialAngleSpread;
    u8 pad3C[0x04];
    PcpFlashRotatingParticle *parts;
    s32 updateCount;
    u32 colorParam;
    f32 renderScale;
    u32 ownedBuffer;
    u32 resourceHandle;
};

typedef struct PcpFlashPtc14 PcpFlashPtc14;

struct PcpFlashPtc14 {
    u32 color;
    s32 age;
    f32 scale;
    f32 initialScale;
    f32 angle;
};

typedef struct PcpFlashWork3 PcpFlashWork3;

struct PcpFlashWork3 {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    s32 rampTime;
    u32 randomRange;
    u32 colorA;
    u32 colorB;
    f32 upSpan;
    f32 acrossSpan;
    f32 orbitRadius;
    f32 maxScale;
    f32 tilt;
    f32 increment;
    u32 unk44;
    PcpFlashPtc14 *parts;
    u32 updateCount;
    u32 colorParam;
    f32 renderScale;
    u32 ownedBuffer;
    u32 resourceHandle;
};

typedef struct PcpFlashPtc1C PcpFlashPtc1C;

struct PcpFlashPtc1C {
    u32 color;
    s32 age;
    f32 unk08;
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 accumulator;
};

typedef struct PcpFlashWork4 PcpFlashWork4;

struct PcpFlashWork4 {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    u32 randomRange;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 unk30;
    f32 unk34;
    u8 pad38[0x04];
    f32 unk3C;
    u8 pad40[0x04];
    f32 increment;
    u32 unk48;
    u32 unk4C;
    PcpFlashPtc1C *parts;
    u32 unk54;
    u32 colorParam;
    f32 renderScale;
    u32 ownedBuffer;
    u32 resourceHandle;
};

/* Shared 0x10-byte motion state: accumulator is an angle for orbit arcs and
   a radius for radial triangles; stepSpeed is radial speed or arc height. */
typedef struct PcpFlashMotionParticle PcpFlashMotionParticle;

struct PcpFlashMotionParticle {
    u32 color;
    s32 age;
    f32 accumulator;
    f32 stepSpeed; /* 0x0C: multiplied by decay each step, then added to
                       * accumulator; also read as the particle's height */
};

typedef struct PcpFlashOrbitArcWork PcpFlashOrbitArcWork;

/* The orbit constructor and renderer share this 0x80-byte work record.
   The 0x58-byte copied parameters are followed by the motion-particle array
   pointer and draw/resource state. */
struct PcpFlashOrbitArcWork {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    u32 randomRange;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 increment;
    u32 unk54;
    PcpFlashMotionParticle *parts;
    u32 unk5C;
    u32 colorParam;
    f32 renderScale;
    f32 orbitRadius;
    f32 normalSpan;
    f32 upSpan;
    f32 acrossSpan;
    u32 ownedBuffer;
    u32 resourceHandle;
};

typedef struct PcpFlashPtc20A PcpFlashPtc20A;

struct PcpFlashPtc20A {
    u32 color;
    s32 age;
    f32 increment;
    f32 scale;
    f32 angle;
    f32 upSpan;
    f32 acrossSpan;
    f32 initialScale;
};

typedef struct PcpFlashWork6 PcpFlashWork6;

struct PcpFlashWork6 {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    s32 rampTime;
    u32 randomRange;
    u8 pad24[0x04];
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 upSpan;
    f32 acrossSpan;
    f32 maxScale;
    f32 angularSpread;
    u32 unk48;
    PcpFlashPtc20A *parts;
    u32 updateCount;
    u32 colorParam;
    f32 renderScale;
    u32 ownedBuffer;
    u32 resourceHandle;
};

typedef struct PcpFlashWork7 PcpFlashWork7;

struct PcpFlashWork7 {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 initialRadius;
    f32 initialRadialSpeed;
    f32 radialDamping;
    u32 unk38;
    PcpFlashMotionParticle *parts;
    s32 updateCount;
    u32 colorParam;
    f32 renderScale;
    u32 ownedBuffer;
    u32 resourceHandle;
};

typedef struct PcpFlashPtc20B PcpFlashPtc20B;

struct PcpFlashPtc20B {
    u32 color;
    s32 age;
    f32 increment;
    f32 thickness;
    f32 radius;
    f32 radialSpeed;
    f32 angle;
    f32 span;
};

typedef struct PcpFlashWork8 PcpFlashWork8;

struct PcpFlashWork8 {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    u32 randomRange;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 unk30;
    f32 unk34;
    f32 maxScale;
    f32 unk3C;
    f32 initialRadius;
    f32 initialRadialSpeed;
    f32 radialDamping;
    u32 unk4C;
    u8 pad50[0x80];
    PcpFlashPtc20B *parts;
    u32 updateCount;
    u32 colorParam;
    f32 renderScale;
    u32 ownedBuffer;
    u32 resourceHandle;
};

typedef struct PcpFlashWork9 PcpFlashWork9;

struct PcpFlashWork9 {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    s32 rampTime;
    u32 randomRange;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 upSpan;
    f32 acrossSpan;
    f32 orbitRadius;
    f32 maxScale;
    f32 tilt;
    f32 increment;
    u32 unk4C;
    PcpFlashPtc14 *parts;
    u32 updateCount;
    u32 colorParam;
    f32 renderScale;
    u32 ownedBuffer;
    u32 resourceHandle;
};

typedef struct PcpFlashWork10 PcpFlashWork10;

struct PcpFlashWork10 {
    f32 origin[3];
    u8 pad0C[0x04];
    s32 particleCount;
    u8 restartRandomly;
    u8 pad15[0x03];
    s32 lifetime;
    s32 fadeInTime;
    s32 fadeOutTime;
    u32 colorA;
    u32 colorB;
    f32 initialRadius;
    f32 initialRadialSpeed;
    f32 radialDamping;
    f32 originOffset;
    u32 unk3C;
    PcpFlashMotionParticle *parts;
    s32 updateCount;
    u32 colorParam;
    f32 renderScale;
    u32 ownedBuffer;
    u32 resourceHandle;
};

struct PcpFlashRotatingParticle {
    u32 color;
    s32 age;
    f32 angle;
    f32 scale;
    f32 position[3];
    f32 unk1C;
    f32 upSpan;
    f32 acrossSpan;
    f32 initialScale;
};


typedef struct PcpFlashColorSlot5 {
    s32 color[5];
} PcpFlashColorSlot5;

/* The constructors and update passes share this draw record: origin at 0x40,
   a write-only control word at 0x50, and render scale at 0x5C. */
typedef struct PcpFlashHandle {
    u8 pad00[0x40];
    f32 origin[3];
    u8 pad4C[4];
    u32 unk50;
    u8 pad54[8];
    f32 renderScale;
} PcpFlashHandle;


extern u8 sdfViewEyeVector[];

extern u8 sdfViewTargetVector[];

extern s32 effBlendColor(s32, s32, f32);

extern void effFlashTrianglePulseWriteCorners();

extern void func_00170078(void *);


extern void effFlashBillboardQuad(PcpFlashStreakWork *, s32, void *);

extern void func_0016FC58(void *);

extern void effFlashArcQuadScaling(PcpFlashWork3 *, s32);


extern void effFlashRotatedTriangle(PcpFlashWork7 *, s32, void *);

extern void effFlashArcQuadScalingB(PcpFlashWork9 *, s32);

extern void effFlashRotatedTriangleB(PcpFlashWork10 *, s32, void *);

void effFlashTrianglePulseSpawnFromTable(void *data)
{
    effFlashRecordCreate(effParamTableGetBlock(data, 0));
}

void func_0016A1C8(void)
{
    effFlashRecordCreate();
}

void effFlashTrianglePulseDestroy(PcpFlashWork1 *work)
{
    effReleaseRecordPoolResourceAndBuffer(work->resourceHandle);
    func_002D0918(work->ownedBuffer);
}

void effFlashTrianglePulseCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effFlashTrianglePulseSetColorParam(PcpFlashWork1 *work, u32 value)
{
    work->colorParam = value;
}

void effFlashTrianglePulseSetRenderScale(PcpFlashWork1 *work, f32 value)
{
    work->renderScale = value;
}

void effWriteFlashColorSlot(PcpFlashWork1 *work, s32 index, s32 param)
{
    PcpFlashColorSlot *slot;
    s32 rgb1;
    s32 rgb2;

    slot = (PcpFlashColorSlot *)effGetGroupIndexRecord(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    slot->first = effMultiplyPackedColors(rgb2, param);
    slot->second = effMultiplyPackedColors(rgb2, param);
    slot->third = effMultiplyPackedColors(rgb1 | 0xFF000000, param);
}

/* vu0 routine: a triangle of corner offsets for a flash particle, two of them turned around the view axis by index * step */
void effFlashTrianglePulseWriteCorners(PcpFlashWork1 *work, s32 index, void *view)
{
    PcpFlashParticle10 *part = &work->parts[index];
    f32 *quad = effGetGroupRecordByIndex(work->resourceHandle, index);
    f32 base[4];
    f32 size[4];
    f32 step;
    f32 angle;
    f32 radius;

    step = 3.14159265f * 2.0f / (f32)(u32)work->particleCount;
    radius = part->scale;
    VEC3_SPLAT(size, radius);
    angle = step * (f32)index;
    base[0] = 0;
    base[1] = 1.0f;
    base[2] = 0;
    VU0_LOAD_VF(vf10, base);
    VU0_LOAD_VF(vf11, view);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, base);
    sdfBuildVuRotationFromAxisAngle(angle, view);
    VU0_LOAD_VF(vf10, base);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_LOAD_VF(vf11, size);
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, D_003548F0);
    VU0_STORE_VF(vf10, quad + 8);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad);
    sdfBuildVuRotationFromAxisAngle(angle + step, view);
    VU0_LOAD_VF(vf10, base);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_LOAD_VF(vf11, size);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_003548F0);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 4);
}
void effFlashUpdateWork1(PcpFlashWork1 *work) {
    f32 axis[4];
    s32 index;
    s32 lifetime;
    s32 count;
    s32 half;
    s32 ramp;
    f32 maxScale;
    s32 restart;
    s32 fadeParam;
    PcpFlashParticle10 *part;
    PcpFlashHandle *handle;

    VU0_LOAD_VF($vf10, sdfViewEyeVector);
    VU0_LOAD_VF($vf11, sdfViewTargetVector);
        VU0_SUB(vf10, vf10, vf11);;
        VU0_NORMALIZE_VF10();;
    VU0_STORE_VF($vf10, axis);
    lifetime = work->lifetime;
    count = work->particleCount;
    part = work->parts;
    half = lifetime >> 1;
    ramp = work->rampTime;
    maxScale = work->maxScale;
    restart = work->restartRandomly;
    fadeParam = work->colorParam;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;
        s32 color;
        f32 blend;

        if (part->age == 0) {
            effFlashTrianglePulseWriteCorners(work, index, axis);
            effWriteFlashColorSlot(work, index, 0);
            if (ramp == 0) {
                part->scale = maxScale;
            } else {
                part->scale = 0.0f;
            }
            part->color = 0x80808080;
        } else {
            if (part->age >= lifetime) {
                if (restart != 0) {
                    part->age = 0;
                }
                color = 0;
                effWriteFlashColorSlot(work, index, color);
            } else if (part->age > 0) {
                if (ramp == 0) {
                    part->scale = maxScale;
                } else {
                    part->scale = (maxScale * (f32)part->age) / (f32)ramp;
                    if (maxScale < part->scale) {
                        part->scale = maxScale;
                    }
                }
                effFlashTrianglePulseWriteCorners(work, index, axis);
                if (part->age < half) {
                    blend = (f32)age / (f32)half;
                } else {
                    blend = (f32)(lifetime - age) / (f32)half;
                }
                color = effMultiplyPackedColors(effBlendColor(0, part->color, blend), fadeParam);
                effWriteFlashColorSlot(work, index, color);
            }
        }
        part->age = part->age + 1;
    }
    handle = (PcpFlashHandle *)work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->updateCount = work->updateCount + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->renderScale = work->renderScale;
    func_00170078(handle);
}

extern s32 func_002D03F8(s32 size);
extern void *sdfResourceRetainAddress(s32 allocation);
extern void *memcpy(void *dst, const void *src, u32 n);
extern u32 effMiscRand(void *state);
extern s32 func_0016FB08();

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016A6C0);

void effFlashRotatingStreakSpawnFromTable(void *data)
{
    func_0016A6C0(effParamTableGetBlock(data, 0));
}

void func_0016A878(void)
{
    func_0016A6C0();
}

void effFlashRotatingStreakDestroy(PcpFlashStreakWork *work)
{
    effReleaseRecordGroupAssetAndHandle(work->resourceHandle);
    func_002D0918(work->ownedBuffer);
}

void effFlashRotatingStreakCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effFlashRotatingStreakSetColorParam(PcpFlashStreakWork *work, u32 value)
{
    work->colorParam = value;
}

void effFlashRotatingStreakSetRenderScale(PcpFlashStreakWork *work, f32 value)
{
    work->renderScale = value;
}

extern s32 effGetIndexedEffectGroupIndexEntry(s32 handle, s32 index);

void effFlashColorSlot5Set(PcpFlashStreakWork *work, s32 index, s32 param)
{
    PcpFlashColorSlot5 *slot;
    s32 rgb1;
    s32 rgb2;

    slot = (PcpFlashColorSlot5 *)effGetIndexedEffectGroupIndexEntry(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    slot->color[0] = effMultiplyPackedColors(rgb2, param);
    slot->color[1] = effMultiplyPackedColors(rgb2, param);
    if (index & 1) {
        slot->color[2] = effMultiplyPackedColors(0x80000000, param);
        slot->color[3] = effMultiplyPackedColors(rgb1 | 0xFF000000, param);
        slot->color[4] = effMultiplyPackedColors(0x80000000, param);
    } else {
        slot->color[2] = effMultiplyPackedColors(0xFF000000, param);
        slot->color[3] = effMultiplyPackedColors(rgb1 | 0x40000000, param);
        slot->color[4] = effMultiplyPackedColors(0xFF000000, param);
    }
}

extern f32 effMiscRandUnitFloat(void *state);
extern u8 D_0034DF38[];

void effFlashSpawnRotatingParticle(PcpFlashStreakWork *work, s32 index, void *orientation) {
    PcpFlashRotatingParticle *part = work->parts + index;
    f32 direction[4];
    f32 factor;
    f32 scale;

    direction[0] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    direction[1] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    direction[2] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    /* Two plain quadword loads, no memory clobber: retail keeps `direction`
     * and `orientation` CSE'd across them. */
    VU0_LOAD_VF(vf10, direction);
    VU0_LOAD_VF(vf11, orientation);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
        VU0_NORMALIZE_VF10();;
    VU0_STORE_VF(vf10, direction);
    part->position[0] = direction[0];
    part->position[1] = direction[1];
    part->position[2] = direction[2];
    factor = effMiscRandUnitFloat(D_0034DF38) * 0.3f + 0.7f;
    scale = work->maxScale * factor;
    part->initialScale = scale;
    part->scale = scale;
    factor = (effMiscRandUnitFloat(D_0034DF38) * 0.5f + 0.5f) * 0.5f;
    part->upSpan = work->upSpan * factor;
    part->acrossSpan = work->acrossSpan * factor;
    part->angle = work->initialAngleSpread * ((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f);
}

/* vu0 routine: the four corner offsets of a rotating particle's billboard around its scaled position */
void effFlashBillboardQuad(PcpFlashStreakWork *work, s32 index, void *view)
{
    PcpFlashRotatingParticle *part = &work->parts[index];
    f32 *quad = effGetIndexedEffectGroupRecord(work->resourceHandle, index);
    f32 center[4];
    f32 scale[4];
    f32 across[4];
    f32 up[4];
    f32 ratio;
    f32 size;
    f32 acrossLen;
    f32 upLen;

    size = part->scale;
    ratio = size / part->initialScale;
    VEC3_SPLAT(scale, size);
    acrossLen = part->acrossSpan * ratio;
    VEC3_SPLAT(across, acrossLen);
    upLen = part->upSpan * ratio;
    VEC3_SPLAT(up, upLen);
    center[0] = part->position[0];
    center[1] = part->position[1];
    center[2] = part->position[2];
    VU0_LOAD_VF(vf10, center);
    VU0_LOAD_VF(vf11, view);
    VU0_MOVE_VF(vf12, vf10);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, across);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, across);
    VU0_LOAD_VF(vf10, up);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, up);
    VU0_MOVE_VF(vf10, vf12);
    VU0_LOAD_VF(vf11, scale);
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf10, D_00354900);
    VU0_LOAD_VF(vf11, up);
    VU0_STORE_VF(vf10, quad + 12);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 8);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 16);
    VU0_LOAD_VF(vf10, D_00354900);
    VU0_LOAD_VF(vf11, across);
    VU0_ADD(vf10, vf10, vf12);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 4);
}
void effRotateFlashParticlePosition(PcpFlashStreakWork *work, s32 index, void *orientation)
{
    PcpFlashRotatingParticle *part = &work->parts[index];
    f32 position[4];

    position[0] = part->position[0];
    position[1] = part->position[1];
    position[2] = part->position[2];
    sdfBuildVuRotationFromAxisAngle(part->angle, orientation);
    VU0_LOAD_VF(vf10, position);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_STORE_VF(vf10, position);
    part->position[0] = position[0];
    part->position[1] = position[1];
    part->position[2] = position[2];
}

void effFlashUpdateStreak(PcpFlashStreakWork *work) {
    s128 axis;
    s32 index;
    s32 lifetime;
    s32 count;
    s32 half;
    s32 ramp;
    f32 maxScale;
    s32 restart;
    s32 fadeParam;
    u32 range;
    PcpFlashRotatingParticle *part;
    PcpFlashHandle *handle;

    VU0_LOAD_VF($vf10, sdfViewEyeVector);
    VU0_LOAD_VF($vf11, sdfViewTargetVector);
        VU0_SUB(vf10, vf10, vf11);;
    VU0_STORE_VF($vf10, &axis);
    lifetime = work->lifetime;
    count = work->particleCount;
    part = work->parts;
    half = lifetime >> 1;
    ramp = work->rampTime;
    maxScale = work->maxScale;
    restart = work->restartRandomly;
    range = work->randomRange;
    fadeParam = work->colorParam;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;
        s32 color;
        f32 blend;

        if (part->age == 0) {
            effFlashSpawnRotatingParticle(work, index, &axis);
            effFlashBillboardQuad(work, index, &axis);
            effFlashColorSlot5Set(work, index, 0);
            if (ramp == 0) {
                part->scale = maxScale;
            } else {
                part->scale = 0.0f;
            }
            part->color = 0x80808080;
        } else {
            if (part->age >= lifetime) {
                if (restart != 0) {
                    part->age = ~(effMiscRand(D_0034DF38) % range);
                }
                color = 0;
                effFlashColorSlot5Set(work, index, color);
            } else if (part->age > 0) {
                if (ramp == 0) {
                    part->scale = maxScale;
                } else {
                    part->scale = (maxScale * (f32)part->age) / (f32)ramp;
                    if (maxScale < part->scale) {
                        part->scale = maxScale;
                    }
                }
                effRotateFlashParticlePosition(work, index, &axis);
                effFlashBillboardQuad(work, index, &axis);
                if (part->age < half) {
                    blend = (f32)age / (f32)half;
                } else {
                    blend = (f32)(lifetime - age) / (f32)half;
                }
                color = effMultiplyPackedColors(effBlendColor(0, part->color, blend), fadeParam);
                effFlashColorSlot5Set(work, index, color);
            }
        }
        part->age = part->age + 1;
    }
    handle = (PcpFlashHandle *)work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->updateCount = work->updateCount + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->renderScale = work->renderScale;
    func_0016FC58(handle);
}

PcpFlashWork3 *func_0016AFF0(src)
    PcpFlashWork3 *src;
{
    u32 handle = func_002D03F8(src->particleCount * sizeof(PcpFlashPtc14) + sizeof(PcpFlashWork3));
    PcpFlashWork3 *work = (PcpFlashWork3 *)sdfResourceRetainAddress(handle);
    PcpFlashHandle *record;
    f32 angle;
    f32 step;
    u32 range;
    u32 i;

    memcpy(work, src, 0x48);
    work->parts = (PcpFlashPtc14 *)(work + 1);
    work->ownedBuffer = handle;
    work->colorParam = 0x80808080;
    work->updateCount = 0;
    work->renderScale = 1.0f;
    if (work->randomRange == 0) {
        work->randomRange = 1;
    }
    angle = -3.14159265f / 2.0f;
    record = (PcpFlashHandle *)func_0016FB08(work->particleCount);
    record->renderScale = 1.0f;
    record->unk50 = work->unk44;
    work->resourceHandle = (u32)record;
    step = 3.14159265f * 2.0f / (f32)(u32)work->particleCount;
    range = work->randomRange;
    for (i = 0; i < (u32)work->particleCount; i++) {
        work->parts[i].age = -(effMiscRand(D_0034DF38) % range);
        work->parts[i].angle = angle;
        angle += step;
    }
    return work;
}

void effFlashOrbitScalingSpawnFromTable(void *data)
{
    func_0016AFF0(effParamTableGetBlock(data, 0));
}

void func_0016B230(void)
{
    func_0016AFF0();
}

void effFlashOrbitScalingDestroy(PcpFlashWork3 *work)
{
    effReleaseRecordGroupAssetAndHandle(work->resourceHandle);
    func_002D0918(work->ownedBuffer);
}

void effFlashOrbitScalingCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effFlashOrbitScalingSetColorParam(PcpFlashWork3 *work, u32 value)
{
    work->colorParam = value;
}

void effFlashOrbitScalingSetRenderScale(PcpFlashWork3 *work, f32 value)
{
    work->renderScale = value;
}

void effFlashOrbitScalingSetParticleColors(PcpFlashWork3 *work, s32 index, s32 param)
{
    PcpFlashColorSlot5 *slot;
    s32 rgb1;
    s32 rgb2;

    slot = (PcpFlashColorSlot5 *)effGetIndexedEffectGroupIndexEntry(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    slot->color[0] = effMultiplyPackedColors(rgb2, param);
    slot->color[1] = effMultiplyPackedColors(rgb2, param);
    if (index & 1) {
        slot->color[2] = effMultiplyPackedColors(0x80000000, param);
        slot->color[3] = effMultiplyPackedColors(rgb1 | 0xFF000000, param);
        slot->color[4] = effMultiplyPackedColors(0x80000000, param);
    } else {
        slot->color[2] = effMultiplyPackedColors(0xFF000000, param);
        slot->color[3] = effMultiplyPackedColors(rgb1 | 0x40000000, param);
        slot->color[4] = effMultiplyPackedColors(0xFF000000, param);
    }
}

/* vu0 routine: billboard corner offsets for a scaling particle on an arc, built from a normalised direction and its perpendicular */
void effFlashArcQuadScaling(PcpFlashWork3 *work, s32 index)
{
    PcpFlashPtc14 *part = &work->parts[index];
    f32 *quad = effGetIndexedEffectGroupRecord(work->resourceHandle, index);
    f32 offset[4];
    f32 unit[4];
    f32 scaleA[4];
    f32 scaleB[4];
    f32 scaleC[4];
    f32 size;
    f32 ratio;
    f32 sinv;
    f32 height;
    f32 widthB;
    f32 widthC;

    size = part->scale;
    ratio = size / part->initialScale;
    VEC3_SPLAT(scaleA, size);
    widthB = work->acrossSpan * ratio;
    widthC = work->upSpan * ratio;
    VEC3_SPLAT(scaleB, widthB);
    VEC3_SPLAT(scaleC, widthC);
    unit[0] = sdfEvaluateCosineViaSinePhaseShift(part->angle);
    unit[1] = 0;
    sinv = sdfSinPoly(part->angle);
    unit[2] = sinv;
    offset[0] = unit[0] * work->orbitRadius;
    offset[1] = 0;
    offset[2] = sinv * work->orbitRadius;
    height = work->tilt;
    D_00354910[0] = unit[0] * height;
    D_00354910[1] = height + -1.0f;
    D_00354910[2] = sinv * height;
    VU0_LOAD_VF(vf10, D_00354910);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf11, vf10);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf10, unit);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, scaleB);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, scaleB);
    VU0_LOAD_VF(vf10, scaleC);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, scaleC);
    VU0_MOVE_VF(vf10, vf12);
    VU0_LOAD_VF(vf11, scaleA);
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf10, offset);
    VU0_LOAD_VF(vf11, scaleC);
    VU0_STORE_VF(vf10, quad + 12);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 8);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 16);
    VU0_LOAD_VF(vf10, offset);
    VU0_LOAD_VF(vf11, scaleB);
    VU0_ADD(vf10, vf10, vf12);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 4);
}
void effFlashOrbitScalingAdvanceAngle(PcpFlashWork3 *work, s32 index)
{
    PcpFlashPtc14 *part;

    part = &work->parts[index];
    part->angle += work->increment;
}

void effFlashUpdateWork3(PcpFlashWork3 *work) {
    s32 index;
    s32 lifetime;
    s32 count;
    s32 half;
    s32 ramp;
    f32 maxScale;
    s32 restart;
    s32 fadeParam;
    u32 range;
    PcpFlashPtc14 *part;
    PcpFlashHandle *handle;

    lifetime = work->lifetime;
    count = work->particleCount;
    part = work->parts;
    half = lifetime >> 1;
    ramp = work->rampTime;
    maxScale = work->maxScale;
    restart = work->restartRandomly;
    range = work->randomRange;
    fadeParam = work->colorParam;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;
        s32 color;
        f32 blend;

        if (part->age == 0) {
            part->initialScale = maxScale;
            if (ramp == 0) {
                part->scale = maxScale;
            } else {
                part->scale = 0.0f;
            }
            effFlashArcQuadScaling(work, index);
            effFlashOrbitScalingSetParticleColors(work, index, 0);
            part->color = 0x80808080;
        } else {
            if (part->age >= lifetime) {
                if (restart != 0) {
                    part->age = ~(effMiscRand(D_0034DF38) % range);
                }
                color = 0;
                effFlashOrbitScalingSetParticleColors(work, index, color);
            } else if (part->age > 0) {
                if (ramp == 0) {
                    part->scale = maxScale;
                } else {
                    part->scale = (maxScale * (f32)part->age) / (f32)ramp;
                    if (maxScale < part->scale) {
                        part->scale = maxScale;
                    }
                }
                effFlashOrbitScalingAdvanceAngle(work, index);
                effFlashArcQuadScaling(work, index);
                if (part->age < half) {
                    blend = (f32)age / (f32)half;
                } else {
                    blend = (f32)(lifetime - age) / (f32)half;
                }
                color = effMultiplyPackedColors(effBlendColor(0, part->color, blend), fadeParam);
                effFlashOrbitScalingSetParticleColors(work, index, color);
            }
        }
        part->age = part->age + 1;
    }
    handle = (PcpFlashHandle *)work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->updateCount = work->updateCount + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->renderScale = work->renderScale;
    func_0016FC58(handle);
}

extern s32 effRecordPoolCreate();

/* Clone the 0x50-byte parameter block, then spread the particles evenly around the orbit from -pi/2 with random negative start ages (two handle slots per particle). */
PcpFlashWork4 *func_0016B800(src)
    PcpFlashWork4 *src;
{
    u32 handle = func_002D03F8(src->particleCount * sizeof(PcpFlashPtc1C) + sizeof(PcpFlashWork4));
    PcpFlashWork4 *work = (PcpFlashWork4 *)sdfResourceRetainAddress(handle);
    PcpFlashHandle *record;
    f32 angle;
    f32 step;
    u32 range;
    u32 i;

    memcpy(work, src, 0x50);
    work->parts = (PcpFlashPtc1C *)(work + 1);
    work->ownedBuffer = handle;
    work->colorParam = 0x80808080;
    work->unk54 = 0;
    work->renderScale = 1.0f;
    if (work->randomRange == 0) {
        work->randomRange = 1;
    }
    angle = -3.14159265f / 2.0f;
    record = (PcpFlashHandle *)effRecordPoolCreate(work->particleCount * 2);
    record->renderScale = 1.0f;
    record->unk50 = work->unk4C;
    work->resourceHandle = (u32)record;
    step = 3.14159265f * 2.0f / (f32)(u32)work->particleCount;
    range = work->randomRange;
    for (i = 0; i < (u32)work->particleCount; i++) {
        work->parts[i].age = -(effMiscRand(D_0034DF38) % range);
        work->parts[i].accumulator = angle;
        angle += step;
    }
    return work;
}

void effFlashAccumulatingParticleSpawnFromTable(void *data)
{
    func_0016B800(effParamTableGetBlock(data, 0));
}

void func_0016BA58(void)
{
    func_0016B800();
}

void effFlashAccumulatingParticleDestroy(PcpFlashWork4 *work)
{
    func_00170350(work->resourceHandle);
    func_002D0918(work->ownedBuffer);
}

void effFlashAccumulatingParticleCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effFlashAccumulatingParticleSetColorParam(PcpFlashWork4 *work, u32 value)
{
    work->colorParam = value;
}

void effFlashAccumulatingParticleSetRenderScale(PcpFlashWork4 *work, f32 value)
{
    work->renderScale = value;
}

extern s32 func_00170548(s32 handle, s32 index);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016BAC0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016BBB0);

void effFlashAccumulatingParticleAdvance(PcpFlashWork4 *work, s32 index)
{
    PcpFlashPtc1C *part;

    part = &work->parts[index];
    part->accumulator += work->increment;
}

extern void func_0016BAC0(PcpFlashWork4 *, s32, s32);
extern void func_0016BBB0(PcpFlashWork4 *, s32);

void effFlashAccumulatingParticleUpdate(PcpFlashWork4 *work)
{
    s32 index;
    s32 count;
    PcpFlashPtc1C *part;
    s32 lifetime;
    s32 fadeIn;
    s32 fadeOut;
    u32 randomRange;
    s32 restart;
    s32 fadeParam;
    PcpFlashHandle *handle;

    count = work->particleCount;
    part = work->parts;
    lifetime = work->lifetime;
    fadeIn = work->fadeInTime;
    fadeOut = work->fadeOutTime;
    restart = work->restartRandomly;
    randomRange = work->randomRange;
    fadeParam = work->colorParam;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;
        s32 color;
        f32 factor;

        if (age == 0) {
            func_0016BBB0(work, index);
            func_0016BAC0(work, index, 0);
            part->color = 0x80808080;
            factor = effMiscRandUnitFloat(D_0034DF38) * 0.5f + 0.5f;
            part->unk08 = work->unk3C * factor;
            factor = effMiscRandUnitFloat(D_0034DF38) * 0.7f + 0.3f;
            part->unk0C = work->unk30 * factor;
            part->unk10 = work->unk34 * factor;
            part->unk14 = 0;
        } else if (age >= lifetime) {
            if (restart != 0) {
                part->age = ~(effMiscRand(D_0034DF38) % randomRange);
            }
            color = 0;
            func_0016BAC0(work, index, color);
        } else if (age > 0) {
            effFlashAccumulatingParticleAdvance(work, index);
            func_0016BBB0(work, index);
            if (part->age < fadeIn && fadeIn != 0) {
                factor = (f32)part->age / (f32)fadeIn;
            } else if (fadeOut >= lifetime - part->age && fadeOut != 0) {
                factor = (f32)(lifetime - part->age) / (f32)fadeOut;
            } else {
                factor = 1.0f;
            }
            color = effMultiplyPackedColors(effBlendColor(0, part->color, factor), fadeParam);
            func_0016BAC0(work, index, color);
        }
        part->age = part->age + 1;
    }
    handle = (PcpFlashHandle *)work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->unk54 = work->unk54 + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->renderScale = work->renderScale;
    func_00170380(handle);
}

/* The orbit-arc particles follow the same work header used by its draw and
   angle-update passes; this is an allocation container, not a second view. */
typedef struct PcpFlashOrbitArcBlock {
    PcpFlashOrbitArcWork header;
    PcpFlashMotionParticle parts[1];
} PcpFlashOrbitArcBlock;

void *func_0016C0E8(source)
PcpFlashOrbitArcWork *source;
{
    u32 handle;
    PcpFlashOrbitArcBlock *block;
    PcpFlashOrbitArcWork *ring;
    PcpFlashHandle *record;
    f32 angle;
    f32 step;
    u32 spread;
    u32 i;

    handle = func_002D03F8((u32)source->particleCount * 16 + 0x80);
    block = (PcpFlashOrbitArcBlock *)sdfResourceRetainAddress(handle);
    ring = &block->header;
    memcpy(ring, source, 0x58);
    ring->parts = block->parts;
    ring->ownedBuffer = handle;
    ring->colorParam = 0x80808080;
    ring->orbitRadius = ring->unk38;
    ring->upSpan = ring->unk30;
    ring->acrossSpan = ring->unk34;
    ring->unk5C = 0;
    ring->renderScale = 1.0f;
    if (ring->randomRange == 0) {
        ring->randomRange = 1;
    }
    angle = EFFECT_RING_START_ANGLE;
    record = (PcpFlashHandle *)func_0016FB08(ring->particleCount);
    record->renderScale = 1.0f;
    record->unk50 = ring->unk54;
    ring->resourceHandle = (u32)record;
    step = EFFECT_RING_FULL_TURN / (u32)ring->particleCount;
    spread = ring->randomRange;
    for (i = 0; i < (u32)ring->particleCount; i++) {
        ring->parts[i].age = -(effMiscRand(D_0034DF38) % spread);
        ring->parts[i].accumulator = angle;
        angle += step;
    }
    return ring;
}

void effFlashOrbitArcSpawnFromTable(void *data)
{
    func_0016C0E8(effParamTableGetBlock(data, 0));
}

void func_0016C358(void)
{
    func_0016C0E8();
}

void effFlashOrbitArcDestroy(PcpFlashOrbitArcWork *work)
{
    effReleaseRecordGroupAssetAndHandle(work->resourceHandle);
    func_002D0918(work->ownedBuffer);
}

void effFlashOrbitArcCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effFlashOrbitArcSetColorParam(PcpFlashOrbitArcWork *work, u32 value)
{
    work->colorParam = value;
}

void effFlashOrbitArcSetRenderScale(PcpFlashOrbitArcWork *work, f32 value)
{
    work->renderScale = value;
}

void effFlashOrbitArcSetParticleColors(PcpFlashOrbitArcWork *work, s32 index, s32 param)
{
    PcpFlashColorSlot5 *slot;
    s32 rgb1;
    s32 rgb2;

    slot = (PcpFlashColorSlot5 *)effGetIndexedEffectGroupIndexEntry(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    slot->color[0] = effMultiplyPackedColors(rgb2, param);
    slot->color[1] = effMultiplyPackedColors(rgb2, param);
    if (index & 1) {
        slot->color[2] = effMultiplyPackedColors(0x80000000, param);
        slot->color[3] = effMultiplyPackedColors(rgb1 | 0xFF000000, param);
        slot->color[4] = effMultiplyPackedColors(0x80000000, param);
    } else {
        slot->color[2] = effMultiplyPackedColors(0xFF000000, param);
        slot->color[3] = effMultiplyPackedColors(rgb1 | 0x40000000, param);
        slot->color[4] = effMultiplyPackedColors(0xFF000000, param);
    }
}

/* vu0 routine: billboard corner offsets for a particle on an arc, built from a normalised direction and its perpendicular */
void effFlashArcQuad(PcpFlashOrbitArcWork *work, s32 index)
{
    PcpFlashMotionParticle *part = &work->parts[index];
    f32 *quad = effGetIndexedEffectGroupRecord(work->resourceHandle, index);
    f32 offset[4];
    f32 unit[4];
    f32 scaleA[4];
    f32 scaleB[4];
    f32 scaleC[4];
    f32 sinv;
    f32 height;

    VEC3_SPLAT(scaleA, work->normalSpan);
    VEC3_SPLAT(scaleB, work->acrossSpan);
    VEC3_SPLAT(scaleC, work->upSpan);
    unit[0] = sdfEvaluateCosineViaSinePhaseShift(part->accumulator);
    unit[1] = 0;
    sinv = sdfSinPoly(part->accumulator);
    unit[2] = sinv;
    offset[0] = unit[0] * work->orbitRadius;
    offset[1] = 0;
    offset[2] = sinv * work->orbitRadius;
    height = part->stepSpeed;
    D_00354930[0] = unit[0] * height;
    D_00354930[1] = height + -1.0f;
    D_00354930[2] = sinv * height;
    VU0_LOAD_VF(vf10, D_00354930);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf11, vf10);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf10, unit);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, scaleB);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, scaleB);
    VU0_LOAD_VF(vf10, scaleC);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, scaleC);
    VU0_MOVE_VF(vf10, vf12);
    VU0_LOAD_VF(vf11, scaleA);
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf10, offset);
    VU0_LOAD_VF(vf11, scaleC);
    VU0_STORE_VF(vf10, quad + 12);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 8);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 16);
    VU0_LOAD_VF(vf10, offset);
    VU0_LOAD_VF(vf11, scaleB);
    VU0_ADD(vf10, vf10, vf12);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 4);
}
void effFlashOrbitArcAdvanceAngle(PcpFlashOrbitArcWork *work, s32 index)
{
    PcpFlashMotionParticle *part;

    part = &work->parts[index];
    part->accumulator += work->increment;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016C698);

PcpFlashWork6 *func_0016C9F0(src)
    PcpFlashWork6 *src;
{
    u32 handle = func_002D03F8(src->particleCount * sizeof(PcpFlashPtc20A) + sizeof(PcpFlashWork6));
    PcpFlashWork6 *work = (PcpFlashWork6 *)sdfResourceRetainAddress(handle);
    PcpFlashHandle *record;
    u32 range;
    u32 i;

    memcpy(work, src, 0x4C);
    work->parts = (PcpFlashPtc20A *)(work + 1);
    work->ownedBuffer = handle;
    work->colorParam = 0x80808080;
    work->renderScale = 1.0f;
    work->updateCount = 0;
    if (work->randomRange == 0) {
        work->randomRange = 1;
    }
    record = (PcpFlashHandle *)func_0016FB08(work->particleCount);
    work->resourceHandle = (u32)record;
    record->unk50 = work->unk48;
    range = work->randomRange;
    for (i = 0; i < work->particleCount; i++) {
        work->parts[i].age = -(effMiscRand(D_0034DF38) % range);
    }
    return work;
}

void effFlashRotatingQuadSpawnFromTable(void *data)
{
    func_0016C9F0(effParamTableGetBlock(data, 0));
}

void func_0016CBD0(void)
{
    func_0016C9F0();
}

void effFlashRotatingQuadDestroy(PcpFlashWork6 *work)
{
    effReleaseRecordGroupAssetAndHandle(work->resourceHandle);
    func_002D0918(work->ownedBuffer);
}

void effFlashRotatingQuadCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effFlashRotatingQuadSetColorParam(PcpFlashWork6 *work, u32 value)
{
    work->colorParam = value;
}

void effFlashRotatingQuadSetRenderScale(PcpFlashWork6 *work, f32 value)
{
    work->renderScale = value;
}

void effFlashRotatingQuadSetParticleColors(PcpFlashWork6 *work, s32 index, s32 param)
{
    PcpFlashColorSlot5 *slot;
    s32 rgb1;
    s32 rgb2;

    slot = (PcpFlashColorSlot5 *)effGetIndexedEffectGroupIndexEntry(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    slot->color[0] = effMultiplyPackedColors(rgb2, param);
    slot->color[1] = effMultiplyPackedColors(rgb2, param);
    if (index & 1) {
        slot->color[2] = effMultiplyPackedColors(0x80000000, param);
        slot->color[3] = effMultiplyPackedColors(rgb1 | 0xFF000000, param);
        slot->color[4] = effMultiplyPackedColors(0x80000000, param);
    } else {
        slot->color[2] = effMultiplyPackedColors(0xFF000000, param);
        slot->color[3] = effMultiplyPackedColors(rgb1 | 0x40000000, param);
        slot->color[4] = effMultiplyPackedColors(0xFF000000, param);
    }
}

extern f32 effMiscRandUnitFloat(void *state);

void effFlashSpawnParticle6(PcpFlashWork6 *work, s32 index, void *orientation) {
    PcpFlashPtc20A *part = work->parts + index;
    f32 factor;
    f32 scale;

    part->angle = effMiscRandUnitFloat(D_0034DF38) * 6.2831853f;
    factor = effMiscRandUnitFloat(D_0034DF38) * 0.3f + 0.7f;
    scale = work->maxScale * factor;
    part->initialScale = scale;
    part->scale = scale;
    factor = (effMiscRandUnitFloat(D_0034DF38) * 0.5f + 0.5f) * 0.5f;
    part->upSpan = work->upSpan * factor;
    part->acrossSpan = work->acrossSpan * factor;
    part->increment = work->angularSpread * ((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f);
}

/* vu0 routine: corner offsets of a flash particle's billboard, turned around the view axis by the particle's angle */
void effFlashRotatedQuad(PcpFlashWork6 *work, s32 index, void *view)
{
    PcpFlashPtc20A *part = &work->parts[index];
    f32 *quad = effGetIndexedEffectGroupRecord(work->resourceHandle, index);
    f32 base[4];
    f32 scale[4];
    f32 across[4];
    f32 up[4];
    f32 ratio;
    f32 size;
    f32 acrossLen;
    f32 upLen;

    size = part->scale;
    ratio = size / part->initialScale;
    VEC3_SPLAT(scale, size);
    acrossLen = part->acrossSpan * ratio;
    VEC3_SPLAT(across, acrossLen);
    upLen = part->upSpan * ratio;
    VEC3_SPLAT(up, upLen);
    sdfBuildVuRotationFromAxisAngle(part->angle, view);
    base[0] = 0;
    base[1] = 1.0f;
    base[2] = 0;
    VU0_LOAD_VF(vf10, base);
    VU0_LOAD_VF(vf11, view);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_LOAD_VF(vf11, view);
    VU0_MOVE_VF(vf12, vf10);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, across);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, across);
    VU0_LOAD_VF(vf10, up);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, up);
    VU0_MOVE_VF(vf10, vf12);
    VU0_LOAD_VF(vf11, scale);
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf10, D_00354940);
    VU0_LOAD_VF(vf11, up);
    VU0_STORE_VF(vf10, quad + 12);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 8);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 16);
    VU0_LOAD_VF(vf10, D_00354940);
    VU0_LOAD_VF(vf11, across);
    VU0_ADD(vf10, vf10, vf12);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 4);
}
void effFlashAdvanceOrbitPhase(PcpFlashWork6 *work, s32 index, void *orientation)
{
    PcpFlashPtc20A *part;

    part = &work->parts[index];
    part->angle += part->increment;
}

void effFlashUpdateWork6(PcpFlashWork6 *work) {
    s128 axis;
    s32 index;
    s32 lifetime;
    s32 count;
    s32 ramp;
    s32 fadeIn;
    s32 fadeOut;
    f32 maxScale;
    s32 restart;
    s32 fadeParam;
    u32 range;
    PcpFlashPtc20A *part;
    PcpFlashHandle *handle;

    VU0_LOAD_VF($vf10, sdfViewEyeVector);
    VU0_LOAD_VF($vf11, sdfViewTargetVector);
        VU0_SUB(vf10, vf10, vf11);;
    VU0_STORE_VF($vf10, &axis);
    count = work->particleCount;
    part = work->parts;
    lifetime = work->lifetime;
    ramp = work->rampTime;
    maxScale = work->maxScale;
    fadeIn = work->fadeInTime;
    fadeOut = work->fadeOutTime;
    restart = work->restartRandomly;
    range = work->randomRange;
    fadeParam = work->colorParam;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;
        s32 color;
        f32 blend;

        if (part->age == 0) {
            effFlashSpawnParticle6(work, index, &axis);
            effFlashRotatedQuad(work, index, &axis);
            effFlashRotatingQuadSetParticleColors(work, index, 0);
            if (ramp == 0) {
                part->scale = maxScale;
            } else {
                part->scale = 0.0f;
            }
            part->color = 0x80808080;
        } else {
            if (part->age >= lifetime) {
                if (restart != 0) {
                    part->age = ~(effMiscRand(D_0034DF38) % range);
                }
                color = 0;
                effFlashRotatingQuadSetParticleColors(work, index, color);
            } else if (part->age > 0) {
                s32 remain;

                if (ramp == 0) {
                    part->scale = maxScale;
                } else {
                    part->scale = (maxScale * (f32)part->age) / (f32)ramp;
                    if (maxScale < part->scale) {
                        part->scale = maxScale;
                    }
                }
                effFlashAdvanceOrbitPhase(work, index, &axis);
                effFlashRotatedQuad(work, index, &axis);
                age = part->age;
                if (age < fadeIn && fadeIn != 0) {
                    blend = (f32)age / (f32)fadeIn;
                } else {
                    remain = lifetime - age;
                    if (fadeOut >= remain && fadeOut != 0) {
                        blend = (f32)remain / (f32)fadeOut;
                    } else {
                        blend = 1.0f;
                    }
                }
                color = effMultiplyPackedColors(effBlendColor(0, part->color, blend), fadeParam);
                effFlashRotatingQuadSetParticleColors(work, index, color);
            }
        }
        part->age = part->age + 1;
    }
    handle = (PcpFlashHandle *)work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->updateCount = work->updateCount + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->renderScale = work->renderScale;
    func_0016FC58(handle);
}

extern PcpFlashHandle *effRecordPoolCreateTriad(s32 count);

PcpFlashWork7 *func_0016D2A8(src)
    PcpFlashWork7 *src;
{
    u32 handle = func_002D03F8(src->particleCount * sizeof(PcpFlashMotionParticle) + sizeof(PcpFlashWork7));
    PcpFlashWork7 *work = (PcpFlashWork7 *)sdfResourceRetainAddress(handle);
    PcpFlashHandle *record;
    u32 i;

    memcpy(work, src, 0x3C);
    work->parts = (PcpFlashMotionParticle *)(work + 1);
    work->colorParam = 0x80808080;
    work->ownedBuffer = handle;
    work->renderScale = 1.0f;
    work->updateCount = 0;
    record = effRecordPoolCreateTriad(work->particleCount);
    work->resourceHandle = (u32)record;
    record->unk50 = work->unk38;
    for (i = 0; i < work->particleCount; i++) {
        work->parts[i].age = 0;
    }
    return work;
}

void effFlashRadialTriangleSpawnFromTable(void *data)
{
    func_0016D2A8(effParamTableGetBlock(data, 0));
}

void func_0016D400(void)
{
    func_0016D2A8();
}

void effFlashRadialTriangleDestroy(PcpFlashWork7 *work)
{
    effReleaseRecordPoolResourceAndBuffer(work->resourceHandle);
    func_002D0918(work->ownedBuffer);
}

void effFlashRadialTriangleCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effFlashRadialTriangleSetColorParam(PcpFlashWork7 *work, u32 value)
{
    work->colorParam = value;
}

void effFlashRadialTriangleSetRenderScale(PcpFlashWork7 *work, f32 value)
{
    work->renderScale = value;
}

void effFlashRadialTriangleSetParticleColors(PcpFlashWork7 *work, s32 index, s32 param)
{
    PcpFlashColorSlot *slot;
    s32 rgb1;
    s32 rgb2;

    slot = (PcpFlashColorSlot *)effGetGroupIndexRecord(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    slot->first = effMultiplyPackedColors(rgb2, param);
    slot->second = effMultiplyPackedColors(rgb2, param);
    slot->third = effMultiplyPackedColors(rgb1 | 0xFF000000, param);
}

/* vu0 routine: a triangle of corner offsets for a flash particle, two of them turned around the view axis by index * step */
void effFlashRotatedTriangle(PcpFlashWork7 *work, s32 index, void *view)
{
    PcpFlashMotionParticle *part = &work->parts[index];
    f32 *quad = effGetGroupRecordByIndex(work->resourceHandle, index);
    f32 base[4];
    f32 size[4];
    f32 step;
    f32 angle;
    f32 radius;

    step = 3.14159265f * 2.0f / (f32)(u32)work->particleCount;
    radius = part->accumulator;
    VEC3_SPLAT(size, radius);
    angle = step * (f32)index;
    sdfBuildVuRotationFromAxisAngle(angle, view);
    base[0] = 0;
    base[1] = 1.0f;
    base[2] = 0;
    VU0_LOAD_VF(vf10, base);
    VU0_LOAD_VF(vf11, view);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, base);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_LOAD_VF(vf11, size);
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, D_00354950);
    VU0_STORE_VF(vf10, quad + 8);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad);
    sdfBuildVuRotationFromAxisAngle(angle + step, view);
    VU0_LOAD_VF(vf10, base);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_LOAD_VF(vf11, size);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_00354950);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 4);
}
void effFlashUpdateWork7(PcpFlashWork7 *work) {
    s128 axis;
    s32 restart;
    s32 fadeParam;
    s32 index;
    s32 active;
    s32 count;
    s32 lifetime;
    s32 fadeIn;
    s32 fadeOut;
    f32 startA;
    f32 startB;
    f32 decay;
    PcpFlashMotionParticle *part;
    PcpFlashHandle *handle;

    VU0_LOAD_VF($vf10, sdfViewEyeVector);
    VU0_LOAD_VF($vf11, sdfViewTargetVector);
        VU0_SUB(vf10, vf10, vf11);;
        VU0_NORMALIZE_VF10();;
    VU0_STORE_VF($vf10, &axis);
    count = work->particleCount;
    part = work->parts;
    lifetime = work->lifetime;
    startA = work->initialRadius;
    startB = work->initialRadialSpeed;
    decay = work->radialDamping;
    fadeIn = work->fadeInTime;
    fadeOut = work->fadeOutTime;
    restart = work->restartRandomly;
    fadeParam = work->colorParam;
    active = 0;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;

        if (lifetime < age) {
            effFlashRadialTriangleSetParticleColors(work, index, 0);
        } else {
            if (age == 0) {
                effFlashRotatedTriangle(work, index, &axis);
                effFlashRadialTriangleSetParticleColors(work, index, 0);
                part->accumulator = startA;
                part->stepSpeed = startB;
                part->color = 0x80808080;
            } else if (age > 0) {
                f32 speed = part->stepSpeed;
                s32 remain;
                f32 blend;

                part->stepSpeed = speed * decay;
                part->accumulator = part->accumulator + speed;
                effFlashRotatedTriangle(work, index, &axis);
                if (age < fadeIn && fadeIn != 0) {
                    blend = (f32)age / (f32)fadeIn;
                } else {
                    remain = lifetime - age;
                    if (fadeOut >= remain && fadeOut != 0) {
                        blend = (f32)remain / (f32)fadeOut;
                    } else {
                        blend = 1.0f;
                    }
                }
                active++;
                effFlashRadialTriangleSetParticleColors(work, index, effMultiplyPackedColors(effBlendColor(0, part->color, blend), fadeParam));
            }
            if (age == lifetime && restart != 0) {
                part->age = 0;
            } else {
                part->age = part->age + 1;
            }
        }
    }
    handle = (PcpFlashHandle *)work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->updateCount = work->updateCount + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->renderScale = work->renderScale;
    if (active != 0) {
        func_00170078(handle);
    }
}

/* Clone the 0xD0-byte parameter block, then give every particle a random negative start age (two handle slots per particle). */
PcpFlashWork8 *effFlashRadialStripCreate(src)
    PcpFlashWork8 *src;
{
    u32 handle = func_002D03F8(src->particleCount * sizeof(PcpFlashPtc20B) + sizeof(PcpFlashWork8));
    PcpFlashWork8 *work = (PcpFlashWork8 *)sdfResourceRetainAddress(handle);
    PcpFlashHandle *record;
    u32 range;
    u32 i;

    memcpy(work, src, 0xD0);
    work->parts = (PcpFlashPtc20B *)(work + 1);
    work->ownedBuffer = handle;
    work->colorParam = 0x80808080;
    work->updateCount = 0;
    work->renderScale = 1.0f;
    if (work->randomRange == 0) {
        work->randomRange = 1;
    }
    record = (PcpFlashHandle *)effRecordPoolCreate(work->particleCount * 2);
    record->renderScale = 1.0f;
    record->unk50 = work->unk4C;
    work->resourceHandle = (u32)record;
    range = work->randomRange;
    for (i = 0; i < work->particleCount; i++) {
        work->parts[i].age = -(effMiscRand(D_0034DF38) % range);
    }
    return work;
}

void effFlashRadialStripSpawnFromTable(void *data)
{
    effFlashRadialStripCreate(effParamTableGetBlock(data, 0));
}

void func_0016DB38(void)
{
    effFlashRadialStripCreate();
}

void effFlashRadialStripDestroy(PcpFlashWork8 *work)
{
    func_00170350(work->resourceHandle);
    func_002D0918(work->ownedBuffer);
}

void effFlashRadialStripCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effFlashRadialStripSetColorParam(PcpFlashWork8 *work, u32 value)
{
    work->colorParam = value;
}

void effFlashRadialStripSetRenderScale(PcpFlashWork8 *work, f32 value)
{
    work->renderScale = value;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016DBA0);

void effFlashSpawnStripParticle(PcpFlashWork8 *work, s32 index, void *orientation) {
    PcpFlashPtc20B *part = work->parts + index;
    f32 factor;

    part->angle = effMiscRandUnitFloat(D_0034DF38) * 6.2831853f;
    factor = effMiscRandUnitFloat(D_0034DF38) * 0.3f + 0.7f;
    part->thickness = work->maxScale * factor;
    factor = work->unk34;
    part->span = work->unk30 * (effMiscRandUnitFloat(D_0034DF38) * factor + (1.0f - factor));
    part->increment = work->unk3C * ((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f);
}

/* vu0 routine: two quads of corner offsets for a flash particle (a strip and its mirror), turned around the view axis by the particle's angle */
void effFlashRotatedStripPair(PcpFlashWork8 *work, s32 index, void *view)
{
    PcpFlashPtc20B *part = &work->parts[index];
    f32 *quad = func_00170538(work->resourceHandle, index * 2);
    f32 base[4];
    f32 size[4];
    f32 spare[4];
    f32 middle[4];
    f32 outer[4];
    f32 inner[4];
    f32 center;
    f32 outerEdge;
    f32 innerEdge;
    f32 span;
    f32 *mirror;

    center = part->radius;
    VEC3_SPLAT(middle, center);
    outerEdge = center + part->thickness;
    VEC3_SPLAT(outer, outerEdge);
    innerEdge = center - part->thickness;
    VEC3_SPLAT(inner, innerEdge);
    span = part->span;
    VEC3_SPLAT(size, span);
    sdfBuildVuRotationFromAxisAngle(part->angle, view);
    base[0] = 0;
    base[1] = 1.0f;
    base[2] = 0;
    VU0_LOAD_VF(vf10, base);
    VU0_LOAD_VF(vf11, view);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_MOVE_VF(vf12, vf10);
    /* retail multiplies the (never written) `spare` slot here and stores it back */
    VU0_LOAD_VF(vf11, spare);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, spare);
    VU0_MOVE_VF(vf10, vf12);
    VU0_LOAD_VF(vf11, view);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, size);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, size);
    VU0_MOVE_VF(vf10, vf12);
    VU0_LOAD_VF(vf11, outer);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, outer);
    VU0_MOVE_VF(vf10, vf12);
    VU0_LOAD_VF(vf11, inner);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, inner);
    VU0_MOVE_VF(vf10, vf12);
    VU0_LOAD_VF(vf11, middle);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, size);
    VU0_STORE_VF(vf10, quad + 8);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 4);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 12);
    VU0_LOAD_VF(vf10, outer);
    VU0_STORE_VF(vf10, quad);
    mirror = func_00170538(work->resourceHandle, index * 2 + 1);
    PCP_COPY_VECTOR(mirror + 8, quad + 8);
    PCP_COPY_VECTOR(mirror + 4, quad + 4);
    PCP_COPY_VECTOR(mirror + 12, quad + 12);
    VU0_LOAD_VF(vf10, inner);
    VU0_STORE_VF(vf10, mirror);
}
void effFlashRadialStripAdvanceAngle(PcpFlashWork8 *work, s32 index, void *orientation)
{
    PcpFlashPtc20B *part;

    part = &work->parts[index];
    part->angle += part->increment;
}

extern void func_0016DBA0(void *, s32, s32);

void effFlashUpdateWork8(PcpFlashWork8 *work) {
    s128 axis;
    s32 index;
    s32 count;
    s32 lifetime;
    f32 maxScale;
    s32 fadeIn;
    s32 fadeOut;
    f32 startA;
    f32 startB;
    f32 decay;
    s32 restart;
    u32 range;
    s32 fadeParam;
    PcpFlashPtc20B *part;
    PcpFlashHandle *handle;

    VU0_LOAD_VF($vf10, sdfViewEyeVector);
    VU0_LOAD_VF($vf11, sdfViewTargetVector);
        VU0_SUB(vf10, vf10, vf11);;
    VU0_STORE_VF($vf10, &axis);
    count = work->particleCount;
    part = work->parts;
    lifetime = work->lifetime;
    maxScale = work->maxScale;
    fadeIn = work->fadeInTime;
    fadeOut = work->fadeOutTime;
    startA = work->initialRadius;
    startB = work->initialRadialSpeed;
    decay = work->radialDamping;
    restart = work->restartRandomly;
    range = work->randomRange;
    fadeParam = work->colorParam;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;

        if (lifetime < age) {
            func_0016DBA0(work, index, 0);
        } else {
            if (age == 0) {
                effFlashSpawnStripParticle(work, index, &axis);
                effFlashRotatedStripPair(work, index, &axis);
                func_0016DBA0(work, index, 0);
                part->thickness = maxScale;
                part->radius = startA;
                part->radialSpeed = startB;
                part->color = 0x80808080;
            } else if (age > 0) {
                f32 speed = part->radialSpeed;
                s32 remain;
                f32 blend;

                part->radialSpeed = speed * decay;
                part->radius = part->radius + speed;
                effFlashRadialStripAdvanceAngle(work, index, &axis);
                effFlashRotatedStripPair(work, index, &axis);
                age = part->age;
                if (age < fadeIn && fadeIn != 0) {
                    blend = (f32)age / (f32)fadeIn;
                } else {
                    remain = lifetime - age;
                    if (fadeOut >= remain && fadeOut != 0) {
                        blend = (f32)remain / (f32)fadeOut;
                    } else {
                        blend = 1.0f;
                    }
                }
                func_0016DBA0(work, index, effMultiplyPackedColors(effBlendColor(0, part->color, blend), fadeParam));
            }
            if (age == lifetime && restart != 0) {
                part->age = ~(effMiscRand(D_0034DF38) % range);
                func_0016DBA0(work, index, 0);
            } else {
                part->age = part->age + 1;
            }
        }
    }
    handle = (PcpFlashHandle *)work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->updateCount = work->updateCount + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->renderScale = work->renderScale;
    func_00170380(handle);
}

/* Clone the 0x50-byte parameter block, then spread the particles evenly around the orbit from -pi/2 with random negative start ages. */
PcpFlashWork9 *func_0016E290(src)
    PcpFlashWork9 *src;
{
    u32 handle = func_002D03F8(src->particleCount * sizeof(PcpFlashPtc14) + sizeof(PcpFlashWork9));
    PcpFlashWork9 *work = (PcpFlashWork9 *)sdfResourceRetainAddress(handle);
    PcpFlashHandle *record;
    f32 angle;
    f32 step;
    u32 range;
    u32 i;

    memcpy(work, src, 0x50);
    work->parts = (PcpFlashPtc14 *)(work + 1);
    work->ownedBuffer = handle;
    work->colorParam = 0x80808080;
    work->updateCount = 0;
    work->renderScale = 1.0f;
    if (work->randomRange == 0) {
        work->randomRange = 1;
    }
    angle = -3.14159265f / 2.0f;
    record = (PcpFlashHandle *)func_0016FB08(work->particleCount);
    record->renderScale = 1.0f;
    record->unk50 = work->unk4C;
    work->resourceHandle = (u32)record;
    step = 3.14159265f * 2.0f / (f32)(u32)work->particleCount;
    range = work->randomRange;
    for (i = 0; i < (u32)work->particleCount; i++) {
        work->parts[i].age = -(effMiscRand(D_0034DF38) % range);
        work->parts[i].angle = angle;
        angle += step;
    }
    return work;
}

void effFlashFadingOrbitSpawnFromTable(void *data)
{
    func_0016E290(effParamTableGetBlock(data, 0));
}

void func_0016E4E0(void)
{
    func_0016E290();
}

void effFlashFadingOrbitDestroy(PcpFlashWork9 *work)
{
    effReleaseRecordGroupAssetAndHandle(work->resourceHandle);
    func_002D0918(work->ownedBuffer);
}

void effFlashFadingOrbitCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effFlashFadingOrbitSetColorParam(PcpFlashWork9 *work, u32 value)
{
    work->colorParam = value;
}

void effFlashFadingOrbitSetRenderScale(PcpFlashWork9 *work, f32 value)
{
    work->renderScale = value;
}

void effFlashFadingOrbitSetParticleColors(PcpFlashWork9 *work, s32 index, s32 param)
{
    PcpFlashColorSlot5 *slot;
    s32 rgb1;
    s32 rgb2;

    slot = (PcpFlashColorSlot5 *)effGetIndexedEffectGroupIndexEntry(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    slot->color[0] = effMultiplyPackedColors(rgb2, param);
    slot->color[1] = effMultiplyPackedColors(rgb2, param);
    if (index & 1) {
        slot->color[2] = effMultiplyPackedColors(0x80000000, param);
        slot->color[3] = effMultiplyPackedColors(rgb1 | 0xFF000000, param);
        slot->color[4] = effMultiplyPackedColors(0x80000000, param);
    } else {
        slot->color[2] = effMultiplyPackedColors(0xFF000000, param);
        slot->color[3] = effMultiplyPackedColors(rgb1 | 0x40000000, param);
        slot->color[4] = effMultiplyPackedColors(0xFF000000, param);
    }
}

/* vu0 routine: billboard corner offsets for a scaling particle on an arc, built from a normalised direction and its perpendicular */
void effFlashArcQuadScalingB(PcpFlashWork9 *work, s32 index)
{
    PcpFlashPtc14 *part = &work->parts[index];
    f32 *quad = effGetIndexedEffectGroupRecord(work->resourceHandle, index);
    f32 offset[4];
    f32 unit[4];
    f32 scaleA[4];
    f32 scaleB[4];
    f32 scaleC[4];
    f32 size;
    f32 ratio;
    f32 sinv;
    f32 height;
    f32 widthB;
    f32 widthC;

    size = part->scale;
    ratio = size / part->initialScale;
    VEC3_SPLAT(scaleA, size);
    widthB = work->acrossSpan * ratio;
    widthC = work->upSpan * ratio;
    VEC3_SPLAT(scaleB, widthB);
    VEC3_SPLAT(scaleC, widthC);
    unit[0] = sdfEvaluateCosineViaSinePhaseShift(part->angle);
    unit[1] = 0;
    sinv = sdfSinPoly(part->angle);
    unit[2] = sinv;
    offset[0] = unit[0] * work->orbitRadius;
    offset[1] = 0;
    offset[2] = sinv * work->orbitRadius;
    height = work->tilt;
    D_00354960[0] = unit[0] * height;
    D_00354960[1] = height + -1.0f;
    D_00354960[2] = sinv * height;
    VU0_LOAD_VF(vf10, D_00354960);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf11, vf10);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf10, unit);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, scaleB);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, scaleB);
    VU0_LOAD_VF(vf10, scaleC);
    VU0_MUL(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, scaleC);
    VU0_MOVE_VF(vf10, vf12);
    VU0_LOAD_VF(vf11, scaleA);
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf12, vf10);
    VU0_LOAD_VF(vf10, offset);
    VU0_LOAD_VF(vf11, scaleC);
    VU0_STORE_VF(vf10, quad + 12);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 8);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 16);
    VU0_LOAD_VF(vf10, offset);
    VU0_LOAD_VF(vf11, scaleB);
    VU0_ADD(vf10, vf10, vf12);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 4);
}
void effFlashFadingOrbitAdvanceAngle(PcpFlashWork9 *work, s32 index)
{
    PcpFlashPtc14 *part;

    part = &work->parts[index];
    part->angle += work->increment;
}

void effFlashUpdateWork9(PcpFlashWork9 *work) {
    s32 restart;
    s32 fadeParam;
    s32 count;
    s32 index;
    s32 lifetime;
    s32 fadeIn;
    s32 fadeOut;
    s32 ramp;
    f32 maxScale;
    u32 range;
    PcpFlashPtc14 *part;
    PcpFlashHandle *handle;

    count = work->particleCount;
    part = work->parts;
    lifetime = work->lifetime;
    fadeIn = work->fadeInTime;
    fadeOut = work->fadeOutTime;
    ramp = work->rampTime;
    maxScale = work->maxScale;
    restart = work->restartRandomly;
    range = work->randomRange;
    fadeParam = work->colorParam;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;

        if (lifetime < age) {
            effFlashFadingOrbitSetParticleColors(work, index, 0);
        } else {
            if (age == 0) {
                part->initialScale = maxScale;
                if (ramp == 0) {
                    part->scale = maxScale;
                } else {
                    part->scale = 0.0f;
                }
                effFlashArcQuadScalingB(work, index);
                effFlashFadingOrbitSetParticleColors(work, index, 0);
                part->color = 0x80808080;
            } else if (age > 0) {
                s32 remain;
                f32 blend;

                if (ramp == 0) {
                    part->scale = maxScale;
                } else {
                    part->scale = (maxScale * (f32)age) / (f32)ramp;
                    if (maxScale < part->scale) {
                        part->scale = maxScale;
                    }
                }
                effFlashFadingOrbitAdvanceAngle(work, index);
                effFlashArcQuadScalingB(work, index);
                if (age < fadeIn && fadeIn != 0) {
                    blend = (f32)age / (f32)fadeIn;
                } else {
                    remain = lifetime - age;
                    if (fadeOut >= remain && fadeOut != 0) {
                        blend = (f32)remain / (f32)fadeOut;
                    } else {
                        blend = 1.0f;
                    }
                }
                effFlashFadingOrbitSetParticleColors(work, index, effMultiplyPackedColors(effBlendColor(0, part->color, blend), fadeParam));
            }
            if (age == lifetime && restart != 0) {
                part->age = ~(effMiscRand(D_0034DF38) % range);
                effFlashFadingOrbitSetParticleColors(work, index, 0);
            } else {
                part->age = part->age + 1;
            }
        }
    }
    handle = (PcpFlashHandle *)work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->updateCount = work->updateCount + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->renderScale = work->renderScale;
    func_0016FC58(handle);
}

PcpFlashWork10 *func_0016EB00(src)
    PcpFlashWork10 *src;
{
    u32 handle = func_002D03F8(src->particleCount * sizeof(PcpFlashMotionParticle) + sizeof(PcpFlashWork10));
    PcpFlashWork10 *work = (PcpFlashWork10 *)sdfResourceRetainAddress(handle);
    PcpFlashHandle *record;
    u32 i;

    memcpy(work, src, 0x40);
    work->parts = (PcpFlashMotionParticle *)(work + 1);
    work->colorParam = 0x80808080;
    work->ownedBuffer = handle;
    work->renderScale = 1.0f;
    work->updateCount = 0;
    record = effRecordPoolCreateTriad(work->particleCount);
    work->resourceHandle = (u32)record;
    record->unk50 = work->unk3C;
    for (i = 0; i < work->particleCount; i++) {
        work->parts[i].age = 0;
    }
    return work;
}

void effFlashOffsetRadialTriangleSpawnFromTable(void *data)
{
    func_0016EB00(effParamTableGetBlock(data, 0));
}

void func_0016EC60(void)
{
    func_0016EB00();
}

void effFlashOffsetRadialTriangleDestroy(PcpFlashWork10 *work)
{
    effReleaseRecordPoolResourceAndBuffer(work->resourceHandle);
    func_002D0918(work->ownedBuffer);
}

void effFlashOffsetRadialTriangleCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effFlashOffsetRadialTriangleSetColorParam(PcpFlashWork10 *work, u32 value)
{
    work->colorParam = value;
}

void effFlashOffsetRadialTriangleSetRenderScale(PcpFlashWork10 *work, f32 value)
{
    work->renderScale = value;
}

void effFlashOffsetRadialTriangleSetParticleColors(PcpFlashWork10 *work, s32 index, s32 param)
{
    PcpFlashColorSlot *slot;
    s32 rgb1;
    s32 rgb2;

    slot = (PcpFlashColorSlot *)effGetGroupIndexRecord(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    slot->first = effMultiplyPackedColors(rgb2, param);
    slot->second = effMultiplyPackedColors(rgb2, param);
    slot->third = effMultiplyPackedColors(rgb1 | 0xFF000000, param);
}

/* vu0 routine: a triangle of corner offsets for a flash particle, two of them turned around the view axis by index * step */
void effFlashRotatedTriangleB(PcpFlashWork10 *work, s32 index, void *view)
{
    PcpFlashMotionParticle *part = &work->parts[index];
    f32 *quad = effGetGroupRecordByIndex(work->resourceHandle, index);
    f32 base[4];
    f32 size[4];
    f32 step;
    f32 angle;
    f32 radius;

    step = 3.14159265f * 2.0f / (f32)(u32)work->particleCount;
    radius = part->accumulator;
    VEC3_SPLAT(size, radius);
    angle = step * (f32)index;
    sdfBuildVuRotationFromAxisAngle(angle, view);
    base[0] = 0;
    base[1] = 1.0f;
    base[2] = 0;
    VU0_LOAD_VF(vf10, base);
    VU0_LOAD_VF(vf11, view);
    VU0_CROSS_XYZ(vf10, vf10, vf11);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, base);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_LOAD_VF(vf11, size);
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, D_00354970);
    VU0_STORE_VF(vf10, quad + 8);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad);
    sdfBuildVuRotationFromAxisAngle(angle + step, view);
    VU0_LOAD_VF(vf10, base);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_LOAD_VF(vf11, size);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_00354970);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 4);
}
void effFlashUpdateWork10(PcpFlashWork10 *work) {
    f32 axis[4];
    s32 restart;
    s32 fadeParam;
    s32 index;
    s32 active;
    s32 count;
    s32 lifetime;
    s32 fadeIn;
    s32 fadeOut;
    f32 startA;
    f32 startB;
    f32 decay;
    f32 scale;
    PcpFlashMotionParticle *part;
    PcpFlashHandle *handle;

    VU0_LOAD_VF($vf10, sdfViewEyeVector);
    VU0_LOAD_VF($vf11, sdfViewTargetVector);
        VU0_SUB(vf10, vf10, vf11);;
        VU0_NORMALIZE_VF10();;
    VU0_STORE_VF($vf10, axis);
    count = work->particleCount;
    part = work->parts;
    lifetime = work->lifetime;
    startA = work->initialRadius;
    startB = work->initialRadialSpeed;
    decay = work->radialDamping;
    fadeIn = work->fadeInTime;
    fadeOut = work->fadeOutTime;
    restart = work->restartRandomly;
    fadeParam = work->colorParam;
    active = 0;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;

        if (lifetime < age) {
            effFlashOffsetRadialTriangleSetParticleColors(work, index, 0);
        } else {
            if (age == 0) {
                effFlashRotatedTriangleB(work, index, axis);
                effFlashOffsetRadialTriangleSetParticleColors(work, index, 0);
                part->accumulator = startA;
                part->stepSpeed = startB;
                part->color = 0x80808080;
            } else if (age > 0) {
                f32 speed = part->stepSpeed;
                s32 remain;
                f32 blend;

                part->stepSpeed = speed * decay;
                part->accumulator = part->accumulator + speed;
                effFlashRotatedTriangleB(work, index, axis);
                if (age < fadeIn && fadeIn != 0) {
                    blend = (f32)age / (f32)fadeIn;
                } else {
                    remain = lifetime - age;
                    if (fadeOut >= remain && fadeOut != 0) {
                        blend = (f32)remain / (f32)fadeOut;
                    } else {
                        blend = 1.0f;
                    }
                }
                active++;
                effFlashOffsetRadialTriangleSetParticleColors(work, index, effMultiplyPackedColors(effBlendColor(0, part->color, blend), fadeParam));
            }
            if (age == lifetime && restart != 0) {
                part->age = 0;
            } else {
                part->age = part->age + 1;
            }
        }
    }
    scale = work->originOffset * work->renderScale;
    handle = (PcpFlashHandle *)work->resourceHandle;
    work->updateCount = work->updateCount + 1;
    handle->origin[0] = work->origin[0] + axis[0] * scale;
    handle->origin[1] = work->origin[1] + axis[1] * scale;
    handle->origin[2] = work->origin[2] + axis[2] * scale;
    handle->renderScale = work->renderScale;
    if (active != 0) {
        func_00170078(handle);
    }
}
