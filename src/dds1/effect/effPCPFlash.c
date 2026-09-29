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

extern void func_002DD8B8(void *orientation, f32 angle);


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
    u32 unk38;
    f32 unk3C;
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
    f32 unk2C;
    f32 unk30;
    f32 maxScale;
    f32 unk38;
    u8 pad3C[0x04];
    PcpFlashRotatingParticle *parts;
    s32 updateCount;
    u32 unk48;
    f32 unk4C;
    u32 ownedBuffer;
    u32 resourceHandle;
};

typedef struct PcpFlashPtc14 PcpFlashPtc14;

struct PcpFlashPtc14 {
    u32 color;
    s32 age;
    f32 scale;
    f32 unk0C;
    f32 accumulator;
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
    u8 pad2C[0x0C];
    f32 maxScale;
    u8 pad3C[0x04];
    f32 increment;
    u32 unk44;
    PcpFlashPtc14 *parts;
    u32 unk4C;
    u32 unk50;
    f32 unk54;
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
    u32 unk58;
    f32 unk5C;
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
    u32 unk60;
    f32 unk64;
    f32 unk68;
    f32 unk6C;
    f32 unk70;
    f32 unk74;
    u32 ownedBuffer;
    u32 resourceHandle;
};

typedef struct PcpFlashPtc20A PcpFlashPtc20A;

struct PcpFlashPtc20A {
    u32 color;
    s32 age;
    f32 increment;
    f32 unk0C;
    f32 accumulator;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
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
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    u8 pad48[0x04];
    PcpFlashPtc20A *parts;
    u32 unk50;
    u32 unk54;
    f32 unk58;
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
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    u8 pad38[0x04];
    PcpFlashPtc10 *parts;
    s32 updateCount;
    u32 unk44;
    f32 unk48;
    u32 ownedBuffer;
    u32 resourceHandle;
};

typedef struct PcpFlashPtc20B PcpFlashPtc20B;

struct PcpFlashPtc20B {
    u32 color;
    s32 age;
    f32 increment;
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 accumulator;
    f32 unk1C;
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
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    u8 pad4C[0x84];
    PcpFlashPtc20B *parts;
    u32 unkD4;
    u32 unkD8;
    f32 unkDC;
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
    u8 pad34[0x0C];
    f32 maxScale;
    u8 pad44[0x04];
    f32 increment;
    u32 unk4C;
    PcpFlashPtc14 *parts;
    u32 unk54;
    u32 unk58;
    f32 unk5C;
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
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    u8 pad3C[0x04];
    PcpFlashPtc10 *parts;
    s32 updateCount;
    u32 unk48;
    f32 unk4C;
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
    f32 unk20;
    f32 unk24;
    f32 unk28;
};

typedef struct PcpFlashRotationWork {
    u8 pad00[0x40];
    PcpFlashRotatingParticle *parts;
} PcpFlashRotationWork;

typedef struct PcpFlashHandle1 {
    u8 pad00[0x40];
    f32 origin[3];
    u8 pad4C[0x10];
    f32 unk5C;
} PcpFlashHandle1;

typedef struct PcpFlashColorSlot5 {
    s32 color[5];
} PcpFlashColorSlot5;

typedef struct PcpFlashHandle {
    u8 pad00[0x40];
    f32 origin[3];
    u8 pad4C[0x10];
    f32 unk5C;
} PcpFlashHandle;

typedef struct PcpFlashHandle3 {
    u8 pad00[0x40];
    f32 origin[3];
    u8 pad4C[0x10];
    f32 unk5C;
} PcpFlashHandle3;

typedef struct PcpFlashHandle6 {
    u8 pad00[0x40];
    f32 origin[3];
    u8 pad4C[0x10];
    f32 unk5C;
} PcpFlashHandle6;

typedef struct PcpFlashHandle7 {
    u8 pad00[0x40];
    f32 origin[3];
    u8 pad4C[0x10];
    f32 unk5C;
} PcpFlashHandle7;

typedef struct PcpFlashHandle8 {
    u8 pad00[0x40];
    f32 origin[3];
    u8 pad4C[0x10];
    f32 unk5C;
} PcpFlashHandle8;

typedef struct PcpFlashHandle9 {
    u8 pad00[0x40];
    f32 origin[3];
    u8 pad4C[0x10];
    f32 unk5C;
} PcpFlashHandle9;

typedef struct PcpFlashHandle10 {
    u8 pad00[0x40];
    f32 origin[3];
    u8 pad4C[0x10];
    f32 unk5C;
} PcpFlashHandle10;

extern u8 D_00324680[];

extern u8 D_00324690[];

extern s32 effBlendColor(s32, s32, f32);

extern void func_0016A2D0();

extern void func_00170078(void *);


extern void func_0016AB48(void *, s32, void *);

extern void func_0016FC58(void *);

extern void func_0016B388(void *, s32);

/* Particle elements. Only the fields touched by the matched accumulators are
   known; each struct's size is the element stride used to index its array. */
typedef struct PcpFlashPtc10 PcpFlashPtc10;

extern void func_0016D508();

extern void func_0016E638();

extern void func_0016ED68();

void func_0016A1A8(void *data)
{
    func_0016A088(effParamTableGetBlock(data, 0));
}

void func_0016A1C8(void)
{
    func_0016A088();
}

void func_0016A1E0(PcpFlashWork1 *work)
{
    func_00170048(work->resourceHandle);
    func_002D0918(work->ownedBuffer);
}

void func_0016A210(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0016A220(PcpFlashWork1 *work, u32 value)
{
    work->unk38 = value;
}

void func_0016A228(PcpFlashWork1 *work, f32 value)
{
    work->unk3C = value;
}

void effWriteFlashColorSlot(PcpFlashWork1 *work, s32 index, s32 param)
{
    s32 slot;
    s32 rgb1;
    s32 rgb2;

    slot = func_00170238(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    *(s32 *)(slot + 0) = func_0018DDF8(rgb2, param);
    *(s32 *)(slot + 4) = func_0018DDF8(rgb2, param);
    *(s32 *)(slot + 8) = func_0018DDF8(rgb1 | 0xFF000000, param);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016A2D0);

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
    PcpFlashHandle1 *handle;

    __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(D_00324680));
    __asm__ volatile(".set noreorder\n\tlqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(D_00324690));
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
    __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(axis) : "memory");
    lifetime = work->lifetime;
    count = work->particleCount;
    part = work->parts;
    half = lifetime >> 1;
    ramp = work->rampTime;
    maxScale = work->maxScale;
    restart = work->restartRandomly;
    fadeParam = work->unk38;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;
        s32 color;
        f32 blend;

        if (part->age == 0) {
            func_0016A2D0(work, index, axis);
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
                func_0016A2D0(work, index, axis);
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
    handle = (PcpFlashHandle1 *)work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->updateCount = work->updateCount + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->unk5C = work->unk3C;
    func_00170078(handle);
}

extern s32 func_002D03F8(s32 size);
extern void *sdfResourceRetainAddress(s32 allocation);
extern void *memcpy(void *dst, const void *src, u32 n);
extern u32 effMiscRand(void *state);
extern s32 func_0016FB08();

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016A6C0);

void func_0016A858(void *data)
{
    func_0016A6C0(effParamTableGetBlock(data, 0));
}

void func_0016A878(void)
{
    func_0016A6C0();
}

void func_0016A890(PcpFlashWork2 *work)
{
    func_0016FC28(work->resourceHandle);
    func_002D0918(work->ownedBuffer);
}

void func_0016A8C0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0016A8D0(PcpFlashWork2 *work, u32 value)
{
    work->unk48 = value;
}

void func_0016A8D8(PcpFlashWork2 *work, f32 value)
{
    work->unk4C = value;
}

extern s32 func_0016FF20(s32 handle, s32 index);

void effFlashColorSlot5Set(PcpFlashWork2 *work, s32 index, s32 param)
{
    s32 slot;
    s32 rgb1;
    s32 rgb2;

    slot = func_0016FF20(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    *(s32 *)(slot + 0) = func_0018DDF8(rgb2, param);
    *(s32 *)(slot + 4) = func_0018DDF8(rgb2, param);
    if (index & 1) {
        *(s32 *)(slot + 8) = func_0018DDF8(0x80000000, param);
        *(s32 *)(slot + 12) = func_0018DDF8(rgb1 | 0xFF000000, param);
        *(s32 *)(slot + 16) = func_0018DDF8(0x80000000, param);
    } else {
        *(s32 *)(slot + 8) = func_0018DDF8(0xFF000000, param);
        *(s32 *)(slot + 12) = func_0018DDF8(rgb1 | 0x40000000, param);
        *(s32 *)(slot + 16) = func_0018DDF8(0xFF000000, param);
    }
}

extern f32 func_002E8398(void *state);
extern u8 D_0034DF38[];

void func_0016A9D0(PcpFlashWork2 *work, s32 index, void *orientation) {
    PcpFlashRotatingParticle *part = work->parts + index;
    f32 v[4];
    f32 r;
    f32 size;

    v[0] = (func_002E8398(D_0034DF38) - 0.5f) * 2.0f;
    v[1] = (func_002E8398(D_0034DF38) - 0.5f) * 2.0f;
    v[2] = (func_002E8398(D_0034DF38) - 0.5f) * 2.0f;
    __asm__ volatile(".set noreorder
	lqc2 $vf10, 0(%0)
	.set reorder" : : "r"(v));
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
	.set reorder" : : "r"(v) : "memory");
    part->position[0] = v[0];
    part->position[1] = v[1];
    part->position[2] = v[2];
    r = func_002E8398(D_0034DF38) * 0.3f + 0.7f;
    size = work->maxScale * r;
    part->unk28 = size;
    part->scale = size;
    r = (func_002E8398(D_0034DF38) * 0.5f + 0.5f) * 0.5f;
    part->unk20 = work->unk2C * r;
    part->unk24 = work->unk30 * r;
    part->angle = work->unk38 * ((func_002E8398(D_0034DF38) - 0.5f) * 2.0f);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016AB48);

void effRotateFlashParticlePosition(void *work, s32 index, void *orientation)
{
    u8 *part = *(u8 **)((u8 *)work + 0x40) + index * 0x2C;
    f32 position[4];

    position[0] = *(f32 *)(part + 0x10);
    position[1] = *(f32 *)(part + 0x14);
    position[2] = *(f32 *)(part + 0x18);
    func_002DD8B8(orientation, *(f32 *)(part + 0x08));
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf10, 0(%0)\n"
        "vmulax.xyzw ACC, vf28, vf10x\n"
        "vmadday.xyzw ACC, vf29, vf10y\n"
        "vmaddz.xyzw vf10, vf30, vf10z\n"
        "sqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(position) : "memory");
    *(f32 *)(part + 0x10) = position[0];
    *(f32 *)(part + 0x14) = position[1];
    *(f32 *)(part + 0x18) = position[2];
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

    __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(D_00324680));
    __asm__ volatile(".set noreorder\n\tlqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(D_00324690));
    __asm__ volatile(".set noreorder\n\tvsub.xyzw $vf10, $vf10, $vf11\n\t.set reorder");
    __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(&axis) : "memory");
    lifetime = work->lifetime;
    count = work->particleCount;
    part = work->parts;
    half = lifetime >> 1;
    ramp = work->rampTime;
    maxScale = work->maxScale;
    restart = work->restartRandomly;
    range = work->randomRange;
    fadeParam = work->unk48;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;
        s32 color;
        f32 blend;

        if (part->age == 0) {
            func_0016A9D0(work, index, &axis);
            func_0016AB48(work, index, &axis);
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
                func_0016AB48(work, index, &axis);
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
    handle->unk5C = work->unk4C;
    func_0016FC58(handle);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016AFF0);

void func_0016B210(void *data)
{
    func_0016AFF0(effParamTableGetBlock(data, 0));
}

void func_0016B230(void)
{
    func_0016AFF0();
}

void func_0016B248(PcpFlashWork3 *work)
{
    func_0016FC28(work->resourceHandle);
    func_002D0918(work->ownedBuffer);
}

void func_0016B278(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0016B288(PcpFlashWork3 *work, u32 value)
{
    work->unk50 = value;
}

void func_0016B290(PcpFlashWork3 *work, f32 value)
{
    work->unk54 = value;
}

void func_0016B298(PcpFlashWork3 *work, s32 index, s32 param)
{
    s32 slot;
    s32 rgb1;
    s32 rgb2;

    slot = func_0016FF20(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    *(s32 *)(slot + 0) = func_0018DDF8(rgb2, param);
    *(s32 *)(slot + 4) = func_0018DDF8(rgb2, param);
    if (index & 1) {
        *(s32 *)(slot + 8) = func_0018DDF8(0x80000000, param);
        *(s32 *)(slot + 12) = func_0018DDF8(rgb1 | 0xFF000000, param);
        *(s32 *)(slot + 16) = func_0018DDF8(0x80000000, param);
    } else {
        *(s32 *)(slot + 8) = func_0018DDF8(0xFF000000, param);
        *(s32 *)(slot + 12) = func_0018DDF8(rgb1 | 0x40000000, param);
        *(s32 *)(slot + 16) = func_0018DDF8(0xFF000000, param);
    }
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016B388);

void func_0016B570(PcpFlashWork3 *work, s32 index)
{
    PcpFlashPtc14 *part;

    part = &work->parts[index];
    part->accumulator += work->increment;
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
    PcpFlashHandle3 *handle;

    lifetime = work->lifetime;
    count = work->particleCount;
    part = work->parts;
    half = lifetime >> 1;
    ramp = work->rampTime;
    maxScale = work->maxScale;
    restart = work->restartRandomly;
    range = work->randomRange;
    fadeParam = work->unk50;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;
        s32 color;
        f32 blend;

        if (part->age == 0) {
            part->unk0C = maxScale;
            if (ramp == 0) {
                part->scale = maxScale;
            } else {
                part->scale = 0.0f;
            }
            func_0016B388(work, index);
            func_0016B298(work, index, 0);
            part->color = 0x80808080;
        } else {
            if (part->age >= lifetime) {
                if (restart != 0) {
                    part->age = ~(effMiscRand(D_0034DF38) % range);
                }
                color = 0;
                func_0016B298(work, index, color);
            } else if (part->age > 0) {
                if (ramp == 0) {
                    part->scale = maxScale;
                } else {
                    part->scale = (maxScale * (f32)part->age) / (f32)ramp;
                    if (maxScale < part->scale) {
                        part->scale = maxScale;
                    }
                }
                func_0016B570(work, index);
                func_0016B388(work, index);
                if (part->age < half) {
                    blend = (f32)age / (f32)half;
                } else {
                    blend = (f32)(lifetime - age) / (f32)half;
                }
                color = func_0018DDF8(effBlendColor(0, part->color, blend), fadeParam);
                func_0016B298(work, index, color);
            }
        }
        part->age = part->age + 1;
    }
    handle = (PcpFlashHandle3 *)work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->unk4C = work->unk4C + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->unk5C = work->unk54;
    func_0016FC58(handle);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016B800);

void func_0016BA38(void *data)
{
    func_0016B800(effParamTableGetBlock(data, 0));
}

void func_0016BA58(void)
{
    func_0016B800();
}

void func_0016BA70(PcpFlashWork4 *work)
{
    func_00170350(work->resourceHandle);
    func_002D0918(work->ownedBuffer);
}

void func_0016BAA0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0016BAB0(PcpFlashWork4 *work, u32 value)
{
    work->unk58 = value;
}

void func_0016BAB8(PcpFlashWork4 *work, f32 value)
{
    work->unk5C = value;
}

extern s32 func_00170548(s32 handle, s32 index);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016BAC0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016BBB0);

void func_0016BE20(PcpFlashWork4 *work, s32 index)
{
    PcpFlashPtc1C *part;

    part = &work->parts[index];
    part->accumulator += work->increment;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016BE48);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016C0E8);

void func_0016C338(void *data)
{
    func_0016C0E8(effParamTableGetBlock(data, 0));
}

void func_0016C358(void)
{
    func_0016C0E8();
}

void func_0016C370(PcpFlashWork5 *work)
{
    func_0016FC28(work->resourceHandle);
    func_002D0918(work->ownedBuffer);
}

void func_0016C3A0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0016C3B0(PcpFlashWork5 *work, u32 value)
{
    work->unk60 = value;
}

void func_0016C3B8(PcpFlashWork5 *work, f32 value)
{
    work->unk64 = value;
}

void func_0016C3C0(PcpFlashWork5 *work, s32 index, s32 param)
{
    s32 slot;
    s32 rgb1;
    s32 rgb2;

    slot = func_0016FF20(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    *(s32 *)(slot + 0) = func_0018DDF8(rgb2, param);
    *(s32 *)(slot + 4) = func_0018DDF8(rgb2, param);
    if (index & 1) {
        *(s32 *)(slot + 8) = func_0018DDF8(0x80000000, param);
        *(s32 *)(slot + 12) = func_0018DDF8(rgb1 | 0xFF000000, param);
        *(s32 *)(slot + 16) = func_0018DDF8(0x80000000, param);
    } else {
        *(s32 *)(slot + 8) = func_0018DDF8(0xFF000000, param);
        *(s32 *)(slot + 12) = func_0018DDF8(rgb1 | 0x40000000, param);
        *(s32 *)(slot + 16) = func_0018DDF8(0xFF000000, param);
    }
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016C4B0);

void func_0016C678(PcpFlashWork5 *work, s32 index)
{
    PcpFlashPtc10 *part;

    part = &work->parts[index];
    part->accumulator += work->increment;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016C698);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016C9F0);

void func_0016CBB0(void *data)
{
    func_0016C9F0(effParamTableGetBlock(data, 0));
}

void func_0016CBD0(void)
{
    func_0016C9F0();
}

void func_0016CBE8(PcpFlashWork6 *work)
{
    func_0016FC28(work->resourceHandle);
    func_002D0918(work->ownedBuffer);
}

void func_0016CC18(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0016CC28(PcpFlashWork6 *work, u32 value)
{
    work->unk54 = value;
}

void func_0016CC30(PcpFlashWork6 *work, f32 value)
{
    work->unk58 = value;
}

void func_0016CC38(PcpFlashWork6 *work, s32 index, s32 param)
{
    s32 slot;
    s32 rgb1;
    s32 rgb2;

    slot = func_0016FF20(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    *(s32 *)(slot + 0) = func_0018DDF8(rgb2, param);
    *(s32 *)(slot + 4) = func_0018DDF8(rgb2, param);
    if (index & 1) {
        *(s32 *)(slot + 8) = func_0018DDF8(0x80000000, param);
        *(s32 *)(slot + 12) = func_0018DDF8(rgb1 | 0xFF000000, param);
        *(s32 *)(slot + 16) = func_0018DDF8(0x80000000, param);
    } else {
        *(s32 *)(slot + 8) = func_0018DDF8(0xFF000000, param);
        *(s32 *)(slot + 12) = func_0018DDF8(rgb1 | 0x40000000, param);
        *(s32 *)(slot + 16) = func_0018DDF8(0xFF000000, param);
    }
}

extern f32 func_002E8398(void *state);

void effFlashSpawnParticle6(PcpFlashWork6 *work, s32 index, void *orientation) {
    PcpFlashPtc20A *part = work->parts + index;
    f32 v;
    f32 size;

    part->accumulator = func_002E8398(D_0034DF38) * 6.2831853f;
    v = func_002E8398(D_0034DF38) * 0.3f + 0.7f;
    size = work->unk40 * v;
    part->unk1C = size;
    part->unk0C = size;
    v = (func_002E8398(D_0034DF38) * 0.5f + 0.5f) * 0.5f;
    part->unk14 = work->unk38 * v;
    part->unk18 = work->unk3C * v;
    part->increment = work->unk44 * ((func_002E8398(D_0034DF38) - 0.5f) * 2.0f);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016CE00);

void func_0016CFC0(PcpFlashWork6 *work, s32 index, void *orientation)
{
    PcpFlashPtc20A *part;

    part = &work->parts[index];
    part->accumulator += part->increment;
}

extern void func_0016CE00(void *, s32, void *);

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
    PcpFlashHandle6 *handle;

    __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(D_00324680));
    __asm__ volatile(".set noreorder\n\tlqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(D_00324690));
    __asm__ volatile(".set noreorder\n\tvsub.xyzw $vf10, $vf10, $vf11\n\t.set reorder");
    __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(&axis) : "memory");
    count = work->particleCount;
    part = work->parts;
    lifetime = work->lifetime;
    ramp = work->rampTime;
    maxScale = work->unk40;
    fadeIn = work->fadeInTime;
    fadeOut = work->fadeOutTime;
    restart = work->restartRandomly;
    range = work->randomRange;
    fadeParam = work->unk54;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;
        s32 color;
        f32 blend;

        if (part->age == 0) {
            effFlashSpawnParticle6(work, index, &axis);
            func_0016CE00(work, index, &axis);
            func_0016CC38(work, index, 0);
            if (ramp == 0) {
                part->unk0C = maxScale;
            } else {
                part->unk0C = 0.0f;
            }
            part->color = 0x80808080;
        } else {
            if (part->age >= lifetime) {
                if (restart != 0) {
                    part->age = ~(effMiscRand(D_0034DF38) % range);
                }
                color = 0;
                func_0016CC38(work, index, color);
            } else if (part->age > 0) {
                s32 remain;

                if (ramp == 0) {
                    part->unk0C = maxScale;
                } else {
                    part->unk0C = (maxScale * (f32)part->age) / (f32)ramp;
                    if (maxScale < part->unk0C) {
                        part->unk0C = maxScale;
                    }
                }
                func_0016CFC0(work, index, &axis);
                func_0016CE00(work, index, &axis);
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
                func_0016CC38(work, index, color);
            }
        }
        part->age = part->age + 1;
    }
    handle = (PcpFlashHandle6 *)work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->unk50 = work->unk50 + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->unk5C = work->unk58;
    func_0016FC58(handle);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016D2A8);

void func_0016D3E0(void *data)
{
    func_0016D2A8(effParamTableGetBlock(data, 0));
}

void func_0016D400(void)
{
    func_0016D2A8();
}

void func_0016D418(PcpFlashWork7 *work)
{
    func_00170048(work->resourceHandle);
    func_002D0918(work->ownedBuffer);
}

void func_0016D448(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0016D458(PcpFlashWork7 *work, u32 value)
{
    work->unk44 = value;
}

void func_0016D460(PcpFlashWork7 *work, f32 value)
{
    work->unk48 = value;
}

void func_0016D468(PcpFlashWork7 *work, s32 index, s32 param)
{
    s32 slot;
    s32 rgb1;
    s32 rgb2;

    slot = func_00170238(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    *(s32 *)(slot + 0) = func_0018DDF8(rgb2, param);
    *(s32 *)(slot + 4) = func_0018DDF8(rgb2, param);
    *(s32 *)(slot + 8) = func_0018DDF8(rgb1 | 0xFF000000, param);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016D508);

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
    PcpFlashHandle7 *handle;

    __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(D_00324680));
    __asm__ volatile(".set noreorder\n\tlqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(D_00324690));
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
    __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(&axis) : "memory");
    count = work->particleCount;
    part = work->parts;
    lifetime = work->lifetime;
    startA = work->unk2C;
    startB = work->unk30;
    decay = work->unk34;
    fadeIn = work->fadeInTime;
    fadeOut = work->fadeOutTime;
    restart = work->restartRandomly;
    fadeParam = work->unk44;
    active = 0;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;

        if (lifetime < age) {
            func_0016D468(work, index, 0);
        } else {
            if (age == 0) {
                func_0016D508(work, index, &axis);
                func_0016D468(work, index, 0);
                part->accumulator = startA;
                part->unk0C = startB;
                part->color = 0x80808080;
            } else if (age > 0) {
                f32 speed = part->unk0C;
                s32 remain;
                f32 blend;

                part->unk0C = speed * decay;
                part->accumulator = part->accumulator + speed;
                func_0016D508(work, index, &axis);
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
                func_0016D468(work, index, func_0018DDF8(effBlendColor(0, part->color, blend), fadeParam));
            }
            if (age == lifetime && restart != 0) {
                part->age = 0;
            } else {
                part->age = part->age + 1;
            }
        }
    }
    handle = (PcpFlashHandle7 *)work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->updateCount = work->updateCount + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->unk5C = work->unk48;
    if (active != 0) {
        func_00170078(handle);
    }
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016D940);

void func_0016DB18(void *data)
{
    func_0016D940(effParamTableGetBlock(data, 0));
}

void func_0016DB38(void)
{
    func_0016D940();
}

void func_0016DB50(PcpFlashWork8 *work)
{
    func_00170350(work->resourceHandle);
    func_002D0918(work->ownedBuffer);
}

void func_0016DB80(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0016DB90(PcpFlashWork8 *work, u32 value)
{
    work->unkD8 = value;
}

void func_0016DB98(PcpFlashWork8 *work, f32 value)
{
    work->unkDC = value;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016DBA0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016DC90);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016DD68);

void func_0016DF90(PcpFlashWork8 *work, s32 index, void *orientation)
{
    PcpFlashPtc20B *part;

    part = &work->parts[index];
    part->accumulator += part->increment;
}

extern void func_0016DC90(void *, s32, void *);
extern void func_0016DD68(void *, s32, void *);
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
    PcpFlashHandle8 *handle;

    __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(D_00324680));
    __asm__ volatile(".set noreorder\n\tlqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(D_00324690));
    __asm__ volatile(".set noreorder\n\tvsub.xyzw $vf10, $vf10, $vf11\n\t.set reorder");
    __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(&axis) : "memory");
    count = work->particleCount;
    part = work->parts;
    lifetime = work->lifetime;
    maxScale = work->unk38;
    fadeIn = work->fadeInTime;
    fadeOut = work->fadeOutTime;
    startA = work->unk40;
    startB = work->unk44;
    decay = work->unk48;
    restart = work->restartRandomly;
    range = work->randomRange;
    fadeParam = work->unkD8;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;

        if (lifetime < age) {
            func_0016DBA0(work, index, 0);
        } else {
            if (age == 0) {
                func_0016DC90(work, index, &axis);
                func_0016DD68(work, index, &axis);
                func_0016DBA0(work, index, 0);
                part->unk0C = maxScale;
                part->unk10 = startA;
                part->unk14 = startB;
                part->color = 0x80808080;
            } else if (age > 0) {
                f32 speed = part->unk14;
                s32 remain;
                f32 blend;

                part->unk14 = speed * decay;
                part->unk10 = part->unk10 + speed;
                func_0016DF90(work, index, &axis);
                func_0016DD68(work, index, &axis);
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
    handle = (PcpFlashHandle8 *)work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->unkD4 = work->unkD4 + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->unk5C = work->unkDC;
    func_00170380(handle);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016E290);

void func_0016E4C0(void *data)
{
    func_0016E290(effParamTableGetBlock(data, 0));
}

void func_0016E4E0(void)
{
    func_0016E290();
}

void func_0016E4F8(PcpFlashWork9 *work)
{
    func_0016FC28(work->resourceHandle);
    func_002D0918(work->ownedBuffer);
}

void func_0016E528(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0016E538(PcpFlashWork9 *work, u32 value)
{
    work->unk58 = value;
}

void func_0016E540(PcpFlashWork9 *work, f32 value)
{
    work->unk5C = value;
}

void func_0016E548(PcpFlashWork9 *work, s32 index, s32 param)
{
    s32 slot;
    s32 rgb1;
    s32 rgb2;

    slot = func_0016FF20(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    *(s32 *)(slot + 0) = func_0018DDF8(rgb2, param);
    *(s32 *)(slot + 4) = func_0018DDF8(rgb2, param);
    if (index & 1) {
        *(s32 *)(slot + 8) = func_0018DDF8(0x80000000, param);
        *(s32 *)(slot + 12) = func_0018DDF8(rgb1 | 0xFF000000, param);
        *(s32 *)(slot + 16) = func_0018DDF8(0x80000000, param);
    } else {
        *(s32 *)(slot + 8) = func_0018DDF8(0xFF000000, param);
        *(s32 *)(slot + 12) = func_0018DDF8(rgb1 | 0x40000000, param);
        *(s32 *)(slot + 16) = func_0018DDF8(0xFF000000, param);
    }
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016E638);

void func_0016E820(PcpFlashWork9 *work, s32 index)
{
    PcpFlashPtc14 *part;

    part = &work->parts[index];
    part->accumulator += work->increment;
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
    PcpFlashHandle9 *handle;

    count = work->particleCount;
    part = work->parts;
    lifetime = work->lifetime;
    fadeIn = work->fadeInTime;
    fadeOut = work->fadeOutTime;
    ramp = work->rampTime;
    maxScale = work->maxScale;
    restart = work->restartRandomly;
    range = work->randomRange;
    fadeParam = work->unk58;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;

        if (lifetime < age) {
            func_0016E548(work, index, 0);
        } else {
            if (age == 0) {
                part->unk0C = maxScale;
                if (ramp == 0) {
                    part->scale = maxScale;
                } else {
                    part->scale = 0.0f;
                }
                func_0016E638(work, index);
                func_0016E548(work, index, 0);
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
                func_0016E820(work, index);
                func_0016E638(work, index);
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
                func_0016E548(work, index, func_0018DDF8(effBlendColor(0, part->color, blend), fadeParam));
            }
            if (age == lifetime && restart != 0) {
                part->age = ~(effMiscRand(D_0034DF38) % range);
                func_0016E548(work, index, 0);
            } else {
                part->age = part->age + 1;
            }
        }
    }
    handle = (PcpFlashHandle9 *)work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->unk54 = work->unk54 + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->unk5C = work->unk5C;
    func_0016FC58(handle);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016EB00);

void func_0016EC40(void *data)
{
    func_0016EB00(effParamTableGetBlock(data, 0));
}

void func_0016EC60(void)
{
    func_0016EB00();
}

void func_0016EC78(PcpFlashWork10 *work)
{
    func_00170048(work->resourceHandle);
    func_002D0918(work->ownedBuffer);
}

void func_0016ECA8(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0016ECB8(PcpFlashWork10 *work, u32 value)
{
    work->unk48 = value;
}

void func_0016ECC0(PcpFlashWork10 *work, f32 value)
{
    work->unk4C = value;
}

void func_0016ECC8(PcpFlashWork10 *work, s32 index, s32 param)
{
    s32 slot;
    s32 rgb1;
    s32 rgb2;

    slot = func_00170238(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    *(s32 *)(slot + 0) = func_0018DDF8(rgb2, param);
    *(s32 *)(slot + 4) = func_0018DDF8(rgb2, param);
    *(s32 *)(slot + 8) = func_0018DDF8(rgb1 | 0xFF000000, param);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016ED68);

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
    PcpFlashHandle10 *handle;

    __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(D_00324680));
    __asm__ volatile(".set noreorder\n\tlqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(D_00324690));
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
    __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(axis) : "memory");
    count = work->particleCount;
    part = work->parts;
    lifetime = work->lifetime;
    startA = work->unk2C;
    startB = work->unk30;
    decay = work->unk34;
    fadeIn = work->fadeInTime;
    fadeOut = work->fadeOutTime;
    restart = work->restartRandomly;
    fadeParam = work->unk48;
    active = 0;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;

        if (lifetime < age) {
            func_0016ECC8(work, index, 0);
        } else {
            if (age == 0) {
                func_0016ED68(work, index, axis);
                func_0016ECC8(work, index, 0);
                part->accumulator = startA;
                part->unk0C = startB;
                part->color = 0x80808080;
            } else if (age > 0) {
                f32 speed = part->unk0C;
                s32 remain;
                f32 blend;

                part->unk0C = speed * decay;
                part->accumulator = part->accumulator + speed;
                func_0016ED68(work, index, axis);
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
                func_0016ECC8(work, index, func_0018DDF8(effBlendColor(0, part->color, blend), fadeParam));
            }
            if (age == lifetime && restart != 0) {
                part->age = 0;
            } else {
                part->age = part->age + 1;
            }
        }
    }
    scale = work->unk38 * work->unk4C;
    handle = (PcpFlashHandle10 *)work->resourceHandle;
    work->updateCount = work->updateCount + 1;
    handle->origin[0] = work->origin[0] + axis[0] * scale;
    handle->origin[1] = work->origin[1] + axis[1] * scale;
    handle->origin[2] = work->origin[2] + axis[2] * scale;
    handle->unk5C = work->unk4C;
    if (active != 0) {
        func_00170078(handle);
    }
}
