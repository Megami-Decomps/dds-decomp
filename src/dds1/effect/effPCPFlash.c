#include "common.h"
#include "pcp_vu0.h"

extern void *effParamTableGetBlock(void *data, s32 index);

extern void func_00170048(u32 res);
extern void func_00170350(u32 res);
extern void func_0016FC28(u32 res);
extern void func_002D0918(u32 res);
extern s32 func_00170238(s32 base, s32 index);
extern s32 func_0018DDF8(s32 color, s32 param);
extern u8 D_0034DF38[];
extern f32 *func_0016FF08(u32 handle, s32 index);
extern f32 D_00354900[];
extern f32 D_00354910[];
extern f32 D_00354960[];
extern f32 D_00354930[];
extern f32 D_00354940[];
extern f32 D_00354950[];
extern f32 D_003548F0[];
extern f32 D_00354970[];
extern f32 *func_00170220(u32 handle, s32 index);
extern f32 *func_00170538(u32 handle, s32 index);
extern f32 func_002E78F8(f32 angle);
extern f32 sdfSinPoly(f32 angle);

extern void func_002DD8B8(f32 angle, void *orientation);


/* Effect initializers implemented in assembly below. Each is entered both with
   and without spawn arguments, so they are declared unchecked. */
extern void func_0016A088();
extern void func_0016A6C0();
extern void func_0016AFF0();
extern void func_0016B800();
extern void func_0016C0E8();
extern void func_0016C9F0();
extern void func_0016D2A8();
extern void func_0016D940();
extern void func_0016E290();
extern void func_0016EB00();

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

typedef struct PcpFlashWork2 PcpFlashWork2;

typedef struct PcpFlashRotatingParticle PcpFlashRotatingParticle;

struct PcpFlashWork2 {
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

typedef struct PcpFlashPtc10 PcpFlashPtc10;

struct PcpFlashPtc10 {
    u32 color;
    s32 age;
    f32 accumulator;
    f32 unk0C;
};

typedef struct PcpFlashWork5 PcpFlashWork5;

struct PcpFlashWork5 {
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
    PcpFlashPtc10 *parts;
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
    u8 pad48[0x04];
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
    u8 pad38[0x04];
    PcpFlashPtc10 *parts;
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
    u8 pad4C[0x84];
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
    u8 pad3C[0x04];
    PcpFlashPtc10 *parts;
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

typedef struct PcpFlashRotationWork {
    u8 pad00[0x40];
    PcpFlashRotatingParticle *parts;
} PcpFlashRotationWork;

typedef struct PcpFlashColorSlot5 {
    s32 color[5];
} PcpFlashColorSlot5;

typedef struct PcpFlashHandle {
    u8 pad00[0x40];
    f32 origin[3];
    u8 pad4C[0x10];
    f32 renderScale;
} PcpFlashHandle;

extern u8 D_00324680[];

extern u8 D_00324690[];

extern s32 effBlendColor(s32, s32, f32);

extern void effFlashTrianglePulseWriteCorners();

extern void func_00170078(void *);


extern void effFlashBillboardQuad(PcpFlashWork2 *, s32, void *);

extern void func_0016FC58(void *);

extern void effFlashArcQuadScaling(PcpFlashWork3 *, s32);

/* Particle elements. Only the fields touched by the matched accumulators are
   known; each struct's size is the element stride used to index its array. */
typedef struct PcpFlashPtc10 PcpFlashPtc10;

extern void effFlashRotatedTriangle(PcpFlashWork7 *, s32, void *);

extern void effFlashArcQuadScalingB(PcpFlashWork9 *, s32);

extern void effFlashRotatedTriangleB(PcpFlashWork10 *, s32, void *);

void effFlashTrianglePulseSpawnFromTable(void *data)
{
    func_0016A088(effParamTableGetBlock(data, 0));
}

void func_0016A1C8(void)
{
    func_0016A088();
}

void effFlashTrianglePulseDestroy(PcpFlashWork1 *work)
{
    func_00170048(work->resourceHandle);
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

    slot = (PcpFlashColorSlot *)func_00170238(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    slot->first = func_0018DDF8(rgb2, param);
    slot->second = func_0018DDF8(rgb2, param);
    slot->third = func_0018DDF8(rgb1 | 0xFF000000, param);
}

#define VEC3_SPLAT(v, x) ((v)[0] = (x), (v)[1] = (x), (v)[2] = (x))

/* vu0 routine: a triangle of corner offsets for a flash particle, two of them turned around the view axis by index * step */
void effFlashTrianglePulseWriteCorners(PcpFlashWork1 *work, s32 index, void *view)
{
    PcpFlashParticle10 *part = &work->parts[index];
    f32 *quad = func_00170220(work->resourceHandle, index);
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
    func_002DD8B8(angle, view);
    VU0_LOAD_VF(vf10, base);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_LOAD_VF(vf11, size);
    VU0_MUL(vf10, vf10, vf11);
    VU0_MOVE_VF(vf11, vf10);
    VU0_LOAD_VF(vf10, D_003548F0);
    VU0_STORE_VF(vf10, quad + 8);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad);
    func_002DD8B8(angle + step, view);
    VU0_LOAD_VF(vf10, base);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_LOAD_VF(vf11, size);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_003548F0);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 4);
}
#undef VEC3_SPLAT

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

    VU0_LOAD_VF($vf10, D_00324680);
    VU0_LOAD_VF($vf11, D_00324690);
    __asm__ volatile(".set noreorder\n\tvsub.xyzw $vf10, $vf10, $vf11\n\t.set reorder");
    __asm__ volatile(
        ".set noreorder\n\t"
        "vmul.xyz $vf2, $vf10, $vf10\n\t"
        "vmulax.w ACC, $vf0, $vf2x\n\t"
        "vmadday.w ACC, $vf0, $vf2y\n\t"
        "vmaddz.w $vf2, $vf0, $vf2z\n\t"
        "vrsqrt Q, $vf0w, $vf2w\n\t"
        "vwaitq\n\t"
        "vmulq.xyz $vf10, $vf10, Q\n\t"
        ".set reorder");
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
                color = func_0018DDF8(effBlendColor(0, part->color, blend), fadeParam);
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

void effFlashRotatingStreakDestroy(PcpFlashWork2 *work)
{
    func_0016FC28(work->resourceHandle);
    func_002D0918(work->ownedBuffer);
}

void effFlashRotatingStreakCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effFlashRotatingStreakSetColorParam(PcpFlashWork2 *work, u32 value)
{
    work->colorParam = value;
}

void effFlashRotatingStreakSetRenderScale(PcpFlashWork2 *work, f32 value)
{
    work->renderScale = value;
}

extern s32 func_0016FF20(s32 handle, s32 index);

void effFlashColorSlot5Set(PcpFlashWork2 *work, s32 index, s32 param)
{
    PcpFlashColorSlot5 *slot;
    s32 rgb1;
    s32 rgb2;

    slot = (PcpFlashColorSlot5 *)func_0016FF20(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    slot->color[0] = func_0018DDF8(rgb2, param);
    slot->color[1] = func_0018DDF8(rgb2, param);
    if (index & 1) {
        slot->color[2] = func_0018DDF8(0x80000000, param);
        slot->color[3] = func_0018DDF8(rgb1 | 0xFF000000, param);
        slot->color[4] = func_0018DDF8(0x80000000, param);
    } else {
        slot->color[2] = func_0018DDF8(0xFF000000, param);
        slot->color[3] = func_0018DDF8(rgb1 | 0x40000000, param);
        slot->color[4] = func_0018DDF8(0xFF000000, param);
    }
}

extern f32 func_002E8398(void *state);
extern u8 D_0034DF38[];

void effFlashSpawnRotatingParticle(PcpFlashWork2 *work, s32 index, void *orientation) {
    PcpFlashRotatingParticle *part = work->parts + index;
    f32 direction[4];
    f32 factor;
    f32 scale;

    direction[0] = (func_002E8398(D_0034DF38) - 0.5f) * 2.0f;
    direction[1] = (func_002E8398(D_0034DF38) - 0.5f) * 2.0f;
    direction[2] = (func_002E8398(D_0034DF38) - 0.5f) * 2.0f;
    __asm__ volatile(".set noreorder
	lqc2 $vf10, 0(%0)
	.set reorder" : : "r"(direction));
    __asm__ volatile(".set noreorder
	lqc2 $vf11, 0(%0)
	.set reorder" : : "r"(orientation));
    __asm__ volatile(".set noreorder
	vopmula.xyz ACC, $vf10, $vf11
	vopmsub.xyz $vf10, $vf11, $vf10
	.set reorder");
    __asm__ volatile(
        ".set noreorder
	"
        "vmul.xyz $vf2, $vf10, $vf10
	"
        "vmulax.w ACC, $vf0, $vf2x
	"
        "vmadday.w ACC, $vf0, $vf2y
	"
        "vmaddz.w $vf2, $vf0, $vf2z
	"
        "vrsqrt Q, $vf0w, $vf2w
	"
        "vwaitq
	"
        "vmulq.xyz $vf10, $vf10, Q
	"
        ".set reorder");
    __asm__ volatile(".set noreorder
	sqc2 $vf10, 0(%0)
	.set reorder" : : "r"(direction) : "memory");
    part->position[0] = direction[0];
    part->position[1] = direction[1];
    part->position[2] = direction[2];
    factor = func_002E8398(D_0034DF38) * 0.3f + 0.7f;
    scale = work->maxScale * factor;
    part->initialScale = scale;
    part->scale = scale;
    factor = (func_002E8398(D_0034DF38) * 0.5f + 0.5f) * 0.5f;
    part->upSpan = work->upSpan * factor;
    part->acrossSpan = work->acrossSpan * factor;
    part->angle = work->initialAngleSpread * ((func_002E8398(D_0034DF38) - 0.5f) * 2.0f);
}

#define VEC3_SPLAT(v, x) ((v)[0] = (x), (v)[1] = (x), (v)[2] = (x))

/* vu0 routine: the four corner offsets of a rotating particle's billboard around its scaled position */
void effFlashBillboardQuad(PcpFlashWork2 *work, s32 index, void *view)
{
    PcpFlashRotatingParticle *part = &work->parts[index];
    f32 *quad = func_0016FF08(work->resourceHandle, index);
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
#undef VEC3_SPLAT

void effRotateFlashParticlePosition(PcpFlashRotationWork *work, s32 index, void *orientation)
{
    PcpFlashRotatingParticle *part = &work->parts[index];
    f32 position[4];

    position[0] = part->position[0];
    position[1] = part->position[1];
    position[2] = part->position[2];
    func_002DD8B8(part->angle, orientation);
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf10, 0(%0)\n"
        "vmulax.xyzw ACC, vf28, vf10x\n"
        "vmadday.xyzw ACC, vf29, vf10y\n"
        "vmaddz.xyzw vf10, vf30, vf10z\n"
        "sqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(position) : "memory");
    part->position[0] = position[0];
    part->position[1] = position[1];
    part->position[2] = position[2];
}

void effFlashUpdateStreak(PcpFlashWork2 *work) {
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

    VU0_LOAD_VF($vf10, D_00324680);
    VU0_LOAD_VF($vf11, D_00324690);
    __asm__ volatile(".set noreorder\n\tvsub.xyzw $vf10, $vf10, $vf11\n\t.set reorder");
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
                effRotateFlashParticlePosition((PcpFlashRotationWork *)work, index, &axis);
                effFlashBillboardQuad(work, index, &axis);
                if (part->age < half) {
                    blend = (f32)age / (f32)half;
                } else {
                    blend = (f32)(lifetime - age) / (f32)half;
                }
                color = func_0018DDF8(effBlendColor(0, part->color, blend), fadeParam);
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

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016AFF0);

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
    func_0016FC28(work->resourceHandle);
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

    slot = (PcpFlashColorSlot5 *)func_0016FF20(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    slot->color[0] = func_0018DDF8(rgb2, param);
    slot->color[1] = func_0018DDF8(rgb2, param);
    if (index & 1) {
        slot->color[2] = func_0018DDF8(0x80000000, param);
        slot->color[3] = func_0018DDF8(rgb1 | 0xFF000000, param);
        slot->color[4] = func_0018DDF8(0x80000000, param);
    } else {
        slot->color[2] = func_0018DDF8(0xFF000000, param);
        slot->color[3] = func_0018DDF8(rgb1 | 0x40000000, param);
        slot->color[4] = func_0018DDF8(0xFF000000, param);
    }
}

#define VEC3_SPLAT(v, x) ((v)[0] = (x), (v)[1] = (x), (v)[2] = (x))

/* vu0 routine: billboard corner offsets for a scaling particle on an arc, built from a normalised direction and its perpendicular */
void effFlashArcQuadScaling(PcpFlashWork3 *work, s32 index)
{
    PcpFlashPtc14 *part = &work->parts[index];
    f32 *quad = func_0016FF08(work->resourceHandle, index);
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
    unit[0] = func_002E78F8(part->angle);
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
#undef VEC3_SPLAT

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
                color = func_0018DDF8(effBlendColor(0, part->color, blend), fadeParam);
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

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016B800);

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

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016BE48);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016C0E8);

void effFlashOrbitArcSpawnFromTable(void *data)
{
    func_0016C0E8(effParamTableGetBlock(data, 0));
}

void func_0016C358(void)
{
    func_0016C0E8();
}

void effFlashOrbitArcDestroy(PcpFlashWork5 *work)
{
    func_0016FC28(work->resourceHandle);
    func_002D0918(work->ownedBuffer);
}

void effFlashOrbitArcCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effFlashOrbitArcSetColorParam(PcpFlashWork5 *work, u32 value)
{
    work->colorParam = value;
}

void effFlashOrbitArcSetRenderScale(PcpFlashWork5 *work, f32 value)
{
    work->renderScale = value;
}

void effFlashOrbitArcSetParticleColors(PcpFlashWork5 *work, s32 index, s32 param)
{
    PcpFlashColorSlot5 *slot;
    s32 rgb1;
    s32 rgb2;

    slot = (PcpFlashColorSlot5 *)func_0016FF20(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    slot->color[0] = func_0018DDF8(rgb2, param);
    slot->color[1] = func_0018DDF8(rgb2, param);
    if (index & 1) {
        slot->color[2] = func_0018DDF8(0x80000000, param);
        slot->color[3] = func_0018DDF8(rgb1 | 0xFF000000, param);
        slot->color[4] = func_0018DDF8(0x80000000, param);
    } else {
        slot->color[2] = func_0018DDF8(0xFF000000, param);
        slot->color[3] = func_0018DDF8(rgb1 | 0x40000000, param);
        slot->color[4] = func_0018DDF8(0xFF000000, param);
    }
}

#define VEC3_SPLAT(v, x) ((v)[0] = (x), (v)[1] = (x), (v)[2] = (x))

/* vu0 routine: billboard corner offsets for a particle on an arc, built from a normalised direction and its perpendicular */
void effFlashArcQuad(PcpFlashWork5 *work, s32 index)
{
    PcpFlashPtc10 *part = &work->parts[index];
    f32 *quad = func_0016FF08(work->resourceHandle, index);
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
    unit[0] = func_002E78F8(part->accumulator);
    unit[1] = 0;
    sinv = sdfSinPoly(part->accumulator);
    unit[2] = sinv;
    offset[0] = unit[0] * work->orbitRadius;
    offset[1] = 0;
    offset[2] = sinv * work->orbitRadius;
    height = part->unk0C;
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
#undef VEC3_SPLAT

void effFlashOrbitArcAdvanceAngle(PcpFlashWork5 *work, s32 index)
{
    PcpFlashPtc10 *part;

    part = &work->parts[index];
    part->accumulator += work->increment;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016C698);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016C9F0);

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
    func_0016FC28(work->resourceHandle);
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

    slot = (PcpFlashColorSlot5 *)func_0016FF20(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    slot->color[0] = func_0018DDF8(rgb2, param);
    slot->color[1] = func_0018DDF8(rgb2, param);
    if (index & 1) {
        slot->color[2] = func_0018DDF8(0x80000000, param);
        slot->color[3] = func_0018DDF8(rgb1 | 0xFF000000, param);
        slot->color[4] = func_0018DDF8(0x80000000, param);
    } else {
        slot->color[2] = func_0018DDF8(0xFF000000, param);
        slot->color[3] = func_0018DDF8(rgb1 | 0x40000000, param);
        slot->color[4] = func_0018DDF8(0xFF000000, param);
    }
}

extern f32 func_002E8398(void *state);

void effFlashSpawnParticle6(PcpFlashWork6 *work, s32 index, void *orientation) {
    PcpFlashPtc20A *part = work->parts + index;
    f32 factor;
    f32 scale;

    part->angle = func_002E8398(D_0034DF38) * 6.2831853f;
    factor = func_002E8398(D_0034DF38) * 0.3f + 0.7f;
    scale = work->maxScale * factor;
    part->initialScale = scale;
    part->scale = scale;
    factor = (func_002E8398(D_0034DF38) * 0.5f + 0.5f) * 0.5f;
    part->upSpan = work->upSpan * factor;
    part->acrossSpan = work->acrossSpan * factor;
    part->increment = work->angularSpread * ((func_002E8398(D_0034DF38) - 0.5f) * 2.0f);
}

#define VEC3_SPLAT(v, x) ((v)[0] = (x), (v)[1] = (x), (v)[2] = (x))

/* vu0 routine: corner offsets of a flash particle's billboard, turned around the view axis by the particle's angle */
void effFlashRotatedQuad(PcpFlashWork6 *work, s32 index, void *view)
{
    PcpFlashPtc20A *part = &work->parts[index];
    f32 *quad = func_0016FF08(work->resourceHandle, index);
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
    func_002DD8B8(part->angle, view);
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
#undef VEC3_SPLAT

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

    VU0_LOAD_VF($vf10, D_00324680);
    VU0_LOAD_VF($vf11, D_00324690);
    __asm__ volatile(".set noreorder\n\tvsub.xyzw $vf10, $vf10, $vf11\n\t.set reorder");
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
                color = func_0018DDF8(effBlendColor(0, part->color, blend), fadeParam);
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

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016D2A8);

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
    func_00170048(work->resourceHandle);
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

    slot = (PcpFlashColorSlot *)func_00170238(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    slot->first = func_0018DDF8(rgb2, param);
    slot->second = func_0018DDF8(rgb2, param);
    slot->third = func_0018DDF8(rgb1 | 0xFF000000, param);
}

#define VEC3_SPLAT(v, x) ((v)[0] = (x), (v)[1] = (x), (v)[2] = (x))

/* vu0 routine: a triangle of corner offsets for a flash particle, two of them turned around the view axis by index * step */
void effFlashRotatedTriangle(PcpFlashWork7 *work, s32 index, void *view)
{
    PcpFlashPtc10 *part = &work->parts[index];
    f32 *quad = func_00170220(work->resourceHandle, index);
    f32 base[4];
    f32 size[4];
    f32 step;
    f32 angle;
    f32 radius;

    step = 3.14159265f * 2.0f / (f32)(u32)work->particleCount;
    radius = part->accumulator;
    VEC3_SPLAT(size, radius);
    angle = step * (f32)index;
    func_002DD8B8(angle, view);
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
    func_002DD8B8(angle + step, view);
    VU0_LOAD_VF(vf10, base);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_LOAD_VF(vf11, size);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_00354950);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 4);
}
#undef VEC3_SPLAT

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
    PcpFlashPtc10 *part;
    PcpFlashHandle *handle;

    VU0_LOAD_VF($vf10, D_00324680);
    VU0_LOAD_VF($vf11, D_00324690);
    __asm__ volatile(".set noreorder\n\tvsub.xyzw $vf10, $vf10, $vf11\n\t.set reorder");
    __asm__ volatile(
        ".set noreorder\n\t"
        "vmul.xyz $vf2, $vf10, $vf10\n\t"
        "vmulax.w ACC, $vf0, $vf2x\n\t"
        "vmadday.w ACC, $vf0, $vf2y\n\t"
        "vmaddz.w $vf2, $vf0, $vf2z\n\t"
        "vrsqrt Q, $vf0w, $vf2w\n\t"
        "vwaitq\n\t"
        "vmulq.xyz $vf10, $vf10, Q\n\t"
        ".set reorder");
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
                part->unk0C = startB;
                part->color = 0x80808080;
            } else if (age > 0) {
                f32 speed = part->unk0C;
                s32 remain;
                f32 blend;

                part->unk0C = speed * decay;
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
                effFlashRadialTriangleSetParticleColors(work, index, func_0018DDF8(effBlendColor(0, part->color, blend), fadeParam));
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

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016D940);

void effFlashRadialStripSpawnFromTable(void *data)
{
    func_0016D940(effParamTableGetBlock(data, 0));
}

void func_0016DB38(void)
{
    func_0016D940();
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

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016DC90);

#define VEC3_SPLAT(v, x) ((v)[0] = (x), (v)[1] = (x), (v)[2] = (x))

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
    func_002DD8B8(part->angle, view);
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
#undef VEC3_SPLAT

void effFlashRadialStripAdvanceAngle(PcpFlashWork8 *work, s32 index, void *orientation)
{
    PcpFlashPtc20B *part;

    part = &work->parts[index];
    part->angle += part->increment;
}

extern void func_0016DC90(void *, s32, void *);
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

    VU0_LOAD_VF($vf10, D_00324680);
    VU0_LOAD_VF($vf11, D_00324690);
    __asm__ volatile(".set noreorder\n\tvsub.xyzw $vf10, $vf10, $vf11\n\t.set reorder");
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
                func_0016DC90(work, index, &axis);
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
                func_0016DBA0(work, index, func_0018DDF8(effBlendColor(0, part->color, blend), fadeParam));
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

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016E290);

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
    func_0016FC28(work->resourceHandle);
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

    slot = (PcpFlashColorSlot5 *)func_0016FF20(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    slot->color[0] = func_0018DDF8(rgb2, param);
    slot->color[1] = func_0018DDF8(rgb2, param);
    if (index & 1) {
        slot->color[2] = func_0018DDF8(0x80000000, param);
        slot->color[3] = func_0018DDF8(rgb1 | 0xFF000000, param);
        slot->color[4] = func_0018DDF8(0x80000000, param);
    } else {
        slot->color[2] = func_0018DDF8(0xFF000000, param);
        slot->color[3] = func_0018DDF8(rgb1 | 0x40000000, param);
        slot->color[4] = func_0018DDF8(0xFF000000, param);
    }
}

#define VEC3_SPLAT(v, x) ((v)[0] = (x), (v)[1] = (x), (v)[2] = (x))

/* vu0 routine: billboard corner offsets for a scaling particle on an arc, built from a normalised direction and its perpendicular */
void effFlashArcQuadScalingB(PcpFlashWork9 *work, s32 index)
{
    PcpFlashPtc14 *part = &work->parts[index];
    f32 *quad = func_0016FF08(work->resourceHandle, index);
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
    unit[0] = func_002E78F8(part->angle);
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
#undef VEC3_SPLAT

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
                effFlashFadingOrbitSetParticleColors(work, index, func_0018DDF8(effBlendColor(0, part->color, blend), fadeParam));
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

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016EB00);

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
    func_00170048(work->resourceHandle);
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

    slot = (PcpFlashColorSlot *)func_00170238(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    slot->first = func_0018DDF8(rgb2, param);
    slot->second = func_0018DDF8(rgb2, param);
    slot->third = func_0018DDF8(rgb1 | 0xFF000000, param);
}

#define VEC3_SPLAT(v, x) ((v)[0] = (x), (v)[1] = (x), (v)[2] = (x))

/* vu0 routine: a triangle of corner offsets for a flash particle, two of them turned around the view axis by index * step */
void effFlashRotatedTriangleB(PcpFlashWork10 *work, s32 index, void *view)
{
    PcpFlashPtc10 *part = &work->parts[index];
    f32 *quad = func_00170220(work->resourceHandle, index);
    f32 base[4];
    f32 size[4];
    f32 step;
    f32 angle;
    f32 radius;

    step = 3.14159265f * 2.0f / (f32)(u32)work->particleCount;
    radius = part->accumulator;
    VEC3_SPLAT(size, radius);
    angle = step * (f32)index;
    func_002DD8B8(angle, view);
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
    func_002DD8B8(angle + step, view);
    VU0_LOAD_VF(vf10, base);
    VU0_ROTATE_VEC(vf10, vf10);
    VU0_LOAD_VF(vf11, size);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_00354970);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 4);
}
#undef VEC3_SPLAT

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
    PcpFlashPtc10 *part;
    PcpFlashHandle *handle;

    VU0_LOAD_VF($vf10, D_00324680);
    VU0_LOAD_VF($vf11, D_00324690);
    __asm__ volatile(".set noreorder\n\tvsub.xyzw $vf10, $vf10, $vf11\n\t.set reorder");
    __asm__ volatile(
        ".set noreorder\n\t"
        "vmul.xyz $vf2, $vf10, $vf10\n\t"
        "vmulax.w ACC, $vf0, $vf2x\n\t"
        "vmadday.w ACC, $vf0, $vf2y\n\t"
        "vmaddz.w $vf2, $vf0, $vf2z\n\t"
        "vrsqrt Q, $vf0w, $vf2w\n\t"
        "vwaitq\n\t"
        "vmulq.xyz $vf10, $vf10, Q\n\t"
        ".set reorder");
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
                part->unk0C = startB;
                part->color = 0x80808080;
            } else if (age > 0) {
                f32 speed = part->unk0C;
                s32 remain;
                f32 blend;

                part->unk0C = speed * decay;
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
                effFlashOffsetRadialTriangleSetParticleColors(work, index, func_0018DDF8(effBlendColor(0, part->color, blend), fadeParam));
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
