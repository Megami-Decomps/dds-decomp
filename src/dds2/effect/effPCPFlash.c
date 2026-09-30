#include "common.h"
#include "pcp_vu0.h"

extern u64 effParamTableGetBlock(u64, u64);

/* Per-effect work areas. Only the fields touched by the matched spawn,
   teardown and accumulator helpers are known; the update bodies are still
   assembly. Each work area belongs to the effect whose initializer is noted. */
typedef struct PcpFlashParticle10 {
    u32 color;
    s32 age;
    f32 scale;
    u8 pad0C[0x04];
} PcpFlashParticle10;

typedef struct PcpFlashWork1 PcpFlashWork1;

/* func_0016A088 */
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

extern s32 func_00177E90(s32 base, s32 index);
extern f32 *func_00177B60(u32 handle, s32 index);
extern f32 D_003B1230[];
extern f32 D_003B1260[];
extern f32 D_003B1240[];
extern f32 D_003B1290[];
extern f32 func_003407A0(f32 angle);
extern f32 sdfSinPoly(f32 angle);

extern s32 func_00195A30(s32 color, s32 param);

/* Three consecutive packed colors returned by func_00177E90. */
typedef struct PcpFlashColorSlot {
    s32 first;
    s32 second;
    s32 third;
} PcpFlashColorSlot;

typedef struct PcpFlashWork2 PcpFlashWork2;
typedef struct PcpFlashRotatingParticle PcpFlashRotatingParticle;

/* func_0016A6C0 */
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

/* func_0016AFF0 */
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
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 maxScale;
    f32 unk3C;
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

/* func_0016B800 */
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

/* Particle elements. Only the fields touched by the matched accumulators are
   known; each struct's size is the element stride used to index its array. */
typedef struct PcpFlashPtc10 PcpFlashPtc10;

struct PcpFlashPtc10 {
    u32 color;
    s32 age;
    f32 accumulator;
    f32 unk0C;
};

typedef struct PcpFlashWork5 PcpFlashWork5;

/* func_0016C0E8 */
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

/* func_0016C9F0 */
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

/* func_0016D2A8 */
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

/* func_0016D940 */
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

/* func_0016E290 */
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
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 maxScale;
    f32 unk44;
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

/* func_0016EB00 */
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

extern void func_00336768(void *orientation, f32 angle);

void func_00171E00(u64 arg0) {
    u64 effectParams;

    effectParams = effParamTableGetBlock(arg0, 0);
    func_00171CE0(effectParams);
}

void func_00171E20(void) {
    func_00171CE0();
}

void func_00171E38(PcpFlashWork1 *work) {
    func_00177CA0(work->resourceHandle);
    func_003297C8(work->ownedBuffer);
}

void func_00171E68(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00171E78(PcpFlashWork1 *work, u32 value) {
    work->unk38 = value;
}

void func_00171E80(PcpFlashWork1 *work, f32 value)
{
    work->unk3C = value;
}

/* Two vertices inherit color B, while the third receives color A with full alpha. */
void effWriteFlashColorSlot(PcpFlashWork1 *work, s32 index, s32 param)
{
    PcpFlashColorSlot *slot;
    s32 colorA;
    s32 colorB;

    slot = (PcpFlashColorSlot *)func_00177E90(work->resourceHandle, index);
    colorA = work->colorA & 0xFFFFFF;
    colorB = work->colorB & 0xFFFFFF;
    slot->first = func_00195A30(colorB, param);
    slot->second = func_00195A30(colorB, param);
    slot->third = func_00195A30(colorA | 0xFF000000, param);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00171F28);

extern void func_00171F28();
extern u8 D_0037F680[];
extern u8 D_0037F690[];
extern s32 effBlendColor(s32, s32, f32);
extern void func_001727A0(PcpFlashWork2 *, s32, void *);
extern void func_00177CD0(void *);

typedef struct PcpFlashHandle {
    u8 pad00[0x40];
    f32 origin[3];
    u8 pad4C[0x10];
    f32 unk5C;
} PcpFlashHandle;

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

    VU0_LOAD_VF($vf10, D_0037F680);
    VU0_LOAD_VF($vf11, D_0037F690);
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
    fadeParam = work->unk38;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;
        s32 color;
        f32 blend;

        if (part->age == 0) {
            func_00171F28(work, index, axis);
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
                func_00171F28(work, index, axis);
                if (part->age < half) {
                    blend = (f32)age / (f32)half;
                } else {
                    blend = (f32)(lifetime - age) / (f32)half;
                }
                color = func_00195A30(effBlendColor(0, part->color, blend), fadeParam);
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
    handle->unk5C = work->unk3C;
    func_00177CD0(handle);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00172318);

void func_001724B0(u64 arg0) {
    u64 effectParams;

    effectParams = effParamTableGetBlock(arg0, 0);
    func_00172318(effectParams);
}

void func_001724D0(void) {
    func_00172318();
}

void func_001724E8(PcpFlashWork2 *work) {
    func_00177880(work->resourceHandle);
    func_003297C8(work->ownedBuffer);
}

void func_00172518(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00172528(PcpFlashWork2 *work, u32 value) {
    work->unk48 = value;
}

void func_00172530(PcpFlashWork2 *work, f32 value)
{
    work->unk4C = value;
}

extern s32 func_00177B78(s32 base);

typedef struct PcpFlashColorSlot5 {
    s32 color[5];
} PcpFlashColorSlot5;

/* Alternate the center alpha arrangement for successive streak vertices. */
void effFlashColorSlot5Set(PcpFlashWork2 *work, s32 flag, s32 param) {
    PcpFlashColorSlot5 *slot;
    s32 colorA;
    s32 colorB;

    slot = (PcpFlashColorSlot5 *)func_00177B78(work->resourceHandle);
    colorA = work->colorA & 0xFFFFFF;
    colorB = work->colorB & 0xFFFFFF;
    slot->color[0] = func_00195A30(colorB, param);
    slot->color[1] = func_00195A30(colorB, param);
    if (flag & 1) {
        slot->color[2] = func_00195A30(0x80000000, param);
        slot->color[3] = func_00195A30(colorA | 0xFF000000, param);
        slot->color[4] = func_00195A30(0x80000000, param);
    } else {
        slot->color[2] = func_00195A30(0xFF000000, param);
        slot->color[3] = func_00195A30(colorA | 0x40000000, param);
        slot->color[4] = func_00195A30(0xFF000000, param);
    }
}

extern f32 func_00341240(void *state);
extern u8 D_003AA868[];

void effFlashSpawnRotatingParticle(PcpFlashWork2 *work, s32 index, void *orientation) {
    PcpFlashRotatingParticle *part = work->parts + index;
    f32 direction[4];
    f32 factor;
    f32 scale;

    direction[0] = (func_00341240(D_003AA868) - 0.5f) * 2.0f;
    direction[1] = (func_00341240(D_003AA868) - 0.5f) * 2.0f;
    direction[2] = (func_00341240(D_003AA868) - 0.5f) * 2.0f;
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
    factor = func_00341240(D_003AA868) * 0.3f + 0.7f;
    scale = work->maxScale * factor;
    part->unk28 = scale;
    part->scale = scale;
    factor = (func_00341240(D_003AA868) * 0.5f + 0.5f) * 0.5f;
    part->unk20 = work->unk2C * factor;
    part->unk24 = work->unk30 * factor;
    part->angle = work->unk38 * ((func_00341240(D_003AA868) - 0.5f) * 2.0f);
}

#define VEC3_SPLAT(v, x) ((v)[0] = (x), (v)[1] = (x), (v)[2] = (x))

/* vu0 routine: the four corner offsets of a rotating particle's billboard around its scaled position */
void func_001727A0(PcpFlashWork2 *work, s32 index, void *view)
{
    PcpFlashRotatingParticle *part = &work->parts[index];
    f32 *quad = func_00177B60(work->resourceHandle, index);
    f32 center[4];
    f32 scale[4];
    f32 across[4];
    f32 up[4];
    f32 ratio;
    f32 size;
    f32 acrossLen;
    f32 upLen;

    size = part->scale;
    ratio = size / part->unk28;
    VEC3_SPLAT(scale, size);
    acrossLen = part->unk24 * ratio;
    VEC3_SPLAT(across, acrossLen);
    upLen = part->unk20 * ratio;
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
    VU0_LOAD_VF(vf10, D_003B1230);
    VU0_LOAD_VF(vf11, up);
    VU0_STORE_VF(vf10, quad + 12);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 8);
    VU0_SUB(vf10, vf10, vf11);
    VU0_SUB(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, quad + 16);
    VU0_LOAD_VF(vf10, D_003B1230);
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
    func_00336768(orientation, part->angle);
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

extern u32 effMiscRand(void *);
extern s32 effBlendColor(s32, s32, f32);
extern void func_001778B0(void *);
extern u8 D_003AA868[];
extern u8 D_0037F680[];
extern u8 D_0037F690[];

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

    VU0_LOAD_VF($vf10, D_0037F680);
    VU0_LOAD_VF($vf11, D_0037F690);
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
    fadeParam = work->unk48;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;
        s32 color;
        f32 blend;

        if (part->age == 0) {
            effFlashSpawnRotatingParticle(work, index, &axis);
            func_001727A0(work, index, &axis);
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
                    part->age = ~(effMiscRand(D_003AA868) % range);
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
                func_001727A0(work, index, &axis);
                if (part->age < half) {
                    blend = (f32)age / (f32)half;
                } else {
                    blend = (f32)(lifetime - age) / (f32)half;
                }
                color = func_00195A30(effBlendColor(0, part->color, blend), fadeParam);
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
    func_001778B0(handle);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00172C48);

void func_00172E68(u64 arg0) {
    u64 effectParams;

    effectParams = effParamTableGetBlock(arg0, 0);
    func_00172C48(effectParams);
}

void func_00172E88(void) {
    func_00172C48();
}

void func_00172EA0(PcpFlashWork3 *work) {
    func_00177880(work->resourceHandle);
    func_003297C8(work->ownedBuffer);
}

void func_00172ED0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00172EE0(PcpFlashWork3 *work, u32 value) {
    work->unk50 = value;
}

void func_00172EE8(PcpFlashWork3 *work, f32 value)
{
    work->unk54 = value;
}

void func_00172EF0(PcpFlashWork3 *work, s32 flag, s32 param) {
    PcpFlashColorSlot5 *slot;
    s32 colorA;
    s32 colorB;

    slot = (PcpFlashColorSlot5 *)func_00177B78(work->resourceHandle);
    colorA = work->colorA & 0xFFFFFF;
    colorB = work->colorB & 0xFFFFFF;
    slot->color[0] = func_00195A30(colorB, param);
    slot->color[1] = func_00195A30(colorB, param);
    if (flag & 1) {
        slot->color[2] = func_00195A30(0x80000000, param);
        slot->color[3] = func_00195A30(colorA | 0xFF000000, param);
        slot->color[4] = func_00195A30(0x80000000, param);
    } else {
        slot->color[2] = func_00195A30(0xFF000000, param);
        slot->color[3] = func_00195A30(colorA | 0x40000000, param);
        slot->color[4] = func_00195A30(0xFF000000, param);
    }
}

#define VEC3_SPLAT(v, x) ((v)[0] = (x), (v)[1] = (x), (v)[2] = (x))

/* vu0 routine: billboard corner offsets for a scaling particle on an arc, built from a normalised direction and its perpendicular */
void func_00172FE0(PcpFlashWork3 *work, s32 index)
{
    PcpFlashPtc14 *part = &work->parts[index];
    f32 *quad = func_00177B60(work->resourceHandle, index);
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
    ratio = size / part->unk0C;
    VEC3_SPLAT(scaleA, size);
    widthB = work->unk30 * ratio;
    widthC = work->unk2C * ratio;
    VEC3_SPLAT(scaleB, widthB);
    VEC3_SPLAT(scaleC, widthC);
    unit[0] = func_003407A0(part->accumulator);
    unit[1] = 0;
    sinv = sdfSinPoly(part->accumulator);
    unit[2] = sinv;
    offset[0] = unit[0] * work->unk34;
    offset[1] = 0;
    offset[2] = sinv * work->unk34;
    height = work->unk3C;
    D_003B1240[0] = unit[0] * height;
    D_003B1240[1] = height + -1.0f;
    D_003B1240[2] = sinv * height;
    VU0_LOAD_VF(vf10, D_003B1240);
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

void func_001731C8(PcpFlashWork3 *work, s32 index) {
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
    PcpFlashHandle *handle;

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
            func_00172FE0(work, index);
            func_00172EF0(work, index, 0);
            part->color = 0x80808080;
        } else {
            if (part->age >= lifetime) {
                if (restart != 0) {
                    part->age = ~(effMiscRand(D_003AA868) % range);
                }
                color = 0;
                func_00172EF0(work, index, color);
            } else if (part->age > 0) {
                if (ramp == 0) {
                    part->scale = maxScale;
                } else {
                    part->scale = (maxScale * (f32)part->age) / (f32)ramp;
                    if (maxScale < part->scale) {
                        part->scale = maxScale;
                    }
                }
                func_001731C8(work, index);
                func_00172FE0(work, index);
                if (part->age < half) {
                    blend = (f32)age / (f32)half;
                } else {
                    blend = (f32)(lifetime - age) / (f32)half;
                }
                color = func_00195A30(effBlendColor(0, part->color, blend), fadeParam);
                func_00172EF0(work, index, color);
            }
        }
        part->age = part->age + 1;
    }
    handle = (PcpFlashHandle *)work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->unk4C = work->unk4C + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->unk5C = work->unk54;
    func_001778B0(handle);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00173458);

void func_00173690(u64 arg0) {
    u64 effectParams;

    effectParams = effParamTableGetBlock(arg0, 0);
    func_00173458(effectParams);
}

void func_001736B0(void) {
    func_00173458();
}

void func_001736C8(PcpFlashWork4 *work) {
    func_00177FA8(work->resourceHandle);
    func_003297C8(work->ownedBuffer);
}

void func_001736F8(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00173708(PcpFlashWork4 *work, u32 value) {
    work->unk58 = value;
}

void func_00173710(PcpFlashWork4 *work, f32 value)
{
    work->unk5C = value;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00173718);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00173808);

void func_00173A78(PcpFlashWork4 *work, s32 index) {
    PcpFlashPtc1C *part;

    part = &work->parts[index];
    part->accumulator += work->increment;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00173AA0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00173D40);

void func_00173F90(u64 arg0) {
    u64 effectParams;

    effectParams = effParamTableGetBlock(arg0, 0);
    func_00173D40(effectParams);
}

void func_00173FB0(void) {
    func_00173D40();
}

void func_00173FC8(PcpFlashWork5 *work) {
    func_00177880(work->resourceHandle);
    func_003297C8(work->ownedBuffer);
}

void func_00173FF8(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00174008(PcpFlashWork5 *work, u32 value) {
    work->unk60 = value;
}

void func_00174010(PcpFlashWork5 *work, f32 value)
{
    work->unk64 = value;
}

void func_00174018(PcpFlashWork5 *work, s32 flag, s32 param) {
    PcpFlashColorSlot5 *slot;
    s32 colorA;
    s32 colorB;

    slot = (PcpFlashColorSlot5 *)func_00177B78(work->resourceHandle);
    colorA = work->colorA & 0xFFFFFF;
    colorB = work->colorB & 0xFFFFFF;
    slot->color[0] = func_00195A30(colorB, param);
    slot->color[1] = func_00195A30(colorB, param);
    if (flag & 1) {
        slot->color[2] = func_00195A30(0x80000000, param);
        slot->color[3] = func_00195A30(colorA | 0xFF000000, param);
        slot->color[4] = func_00195A30(0x80000000, param);
    } else {
        slot->color[2] = func_00195A30(0xFF000000, param);
        slot->color[3] = func_00195A30(colorA | 0x40000000, param);
        slot->color[4] = func_00195A30(0xFF000000, param);
    }
}

#define VEC3_SPLAT(v, x) ((v)[0] = (x), (v)[1] = (x), (v)[2] = (x))

/* vu0 routine: billboard corner offsets for a particle on an arc, built from a normalised direction and its perpendicular */
void func_00174108(PcpFlashWork5 *work, s32 index)
{
    PcpFlashPtc10 *part = &work->parts[index];
    f32 *quad = func_00177B60(work->resourceHandle, index);
    f32 offset[4];
    f32 unit[4];
    f32 scaleA[4];
    f32 scaleB[4];
    f32 scaleC[4];
    f32 sinv;
    f32 height;

    VEC3_SPLAT(scaleA, work->unk6C);
    VEC3_SPLAT(scaleB, work->unk74);
    VEC3_SPLAT(scaleC, work->unk70);
    unit[0] = func_003407A0(part->accumulator);
    unit[1] = 0;
    sinv = sdfSinPoly(part->accumulator);
    unit[2] = sinv;
    offset[0] = unit[0] * work->unk68;
    offset[1] = 0;
    offset[2] = sinv * work->unk68;
    height = part->unk0C;
    D_003B1260[0] = unit[0] * height;
    D_003B1260[1] = height + -1.0f;
    D_003B1260[2] = sinv * height;
    VU0_LOAD_VF(vf10, D_003B1260);
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

void func_001742D0(PcpFlashWork5 *work, s32 index) {
    PcpFlashPtc10 *part;

    part = &work->parts[index];
    part->accumulator += work->increment;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001742F0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00174648);

void func_00174808(u64 arg0) {
    u64 effectParams;

    effectParams = effParamTableGetBlock(arg0, 0);
    func_00174648(effectParams);
}

void func_00174828(void) {
    func_00174648();
}

void func_00174840(PcpFlashWork6 *work) {
    func_00177880(work->resourceHandle);
    func_003297C8(work->ownedBuffer);
}

void func_00174870(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00174880(PcpFlashWork6 *work, u32 value) {
    work->unk54 = value;
}

void func_00174888(PcpFlashWork6 *work, f32 value)
{
    work->unk58 = value;
}

void func_00174890(PcpFlashWork6 *work, s32 flag, s32 param) {
    PcpFlashColorSlot5 *slot;
    s32 colorA;
    s32 colorB;

    slot = (PcpFlashColorSlot5 *)func_00177B78(work->resourceHandle);
    colorA = work->colorA & 0xFFFFFF;
    colorB = work->colorB & 0xFFFFFF;
    slot->color[0] = func_00195A30(colorB, param);
    slot->color[1] = func_00195A30(colorB, param);
    if (flag & 1) {
        slot->color[2] = func_00195A30(0x80000000, param);
        slot->color[3] = func_00195A30(colorA | 0xFF000000, param);
        slot->color[4] = func_00195A30(0x80000000, param);
    } else {
        slot->color[2] = func_00195A30(0xFF000000, param);
        slot->color[3] = func_00195A30(colorA | 0x40000000, param);
        slot->color[4] = func_00195A30(0xFF000000, param);
    }
}

extern f32 func_00341240(void *state);
extern u8 D_003AA868[];

void effFlashSpawnParticle6(PcpFlashWork6 *work, s32 index, void *orientation) {
    PcpFlashPtc20A *part = work->parts + index;
    f32 factor;
    f32 scale;

    part->accumulator = func_00341240(D_003AA868) * 6.2831853f;
    factor = func_00341240(D_003AA868) * 0.3f + 0.7f;
    scale = work->unk40 * factor;
    part->unk1C = scale;
    part->unk0C = scale;
    factor = (func_00341240(D_003AA868) * 0.5f + 0.5f) * 0.5f;
    part->unk14 = work->unk38 * factor;
    part->unk18 = work->unk3C * factor;
    part->increment = work->unk44 * ((func_00341240(D_003AA868) - 0.5f) * 2.0f);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00174A58);

void effFlashAdvanceOrbitPhase(PcpFlashWork6 *work, s32 index, void *orientation) {
    PcpFlashPtc20A *part;

    part = &work->parts[index];
    part->accumulator += part->increment;
}

extern void func_00174A58(void *, s32, void *);

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

    VU0_LOAD_VF($vf10, D_0037F680);
    VU0_LOAD_VF($vf11, D_0037F690);
    __asm__ volatile(".set noreorder\n\tvsub.xyzw $vf10, $vf10, $vf11\n\t.set reorder");
    VU0_STORE_VF($vf10, &axis);
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
            func_00174A58(work, index, &axis);
            func_00174890(work, index, 0);
            if (ramp == 0) {
                part->unk0C = maxScale;
            } else {
                part->unk0C = 0.0f;
            }
            part->color = 0x80808080;
        } else {
            if (part->age >= lifetime) {
                if (restart != 0) {
                    part->age = ~(effMiscRand(D_003AA868) % range);
                }
                color = 0;
                func_00174890(work, index, color);
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
                effFlashAdvanceOrbitPhase(work, index, &axis);
                func_00174A58(work, index, &axis);
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
                color = func_00195A30(effBlendColor(0, part->color, blend), fadeParam);
                func_00174890(work, index, color);
            }
        }
        part->age = part->age + 1;
    }
    handle = (PcpFlashHandle *)work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->unk50 = work->unk50 + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->unk5C = work->unk58;
    func_001778B0(handle);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00174F00);

void func_00175038(u64 arg0) {
    u64 effectParams;

    effectParams = effParamTableGetBlock(arg0, 0);
    func_00174F00(effectParams);
}

void func_00175058(void) {
    func_00174F00();
}

void func_00175070(PcpFlashWork7 *work) {
    func_00177CA0(work->resourceHandle);
    func_003297C8(work->ownedBuffer);
}

void func_001750A0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_001750B0(PcpFlashWork7 *work, u32 value) {
    work->unk44 = value;
}

void func_001750B8(PcpFlashWork7 *work, f32 value)
{
    work->unk48 = value;
}

void func_001750C0(PcpFlashWork7 *work, s32 index, s32 param)
{
    PcpFlashColorSlot *slot;
    s32 colorA;
    s32 colorB;

    slot = (PcpFlashColorSlot *)func_00177E90(work->resourceHandle, index);
    colorA = work->colorA & 0xFFFFFF;
    colorB = work->colorB & 0xFFFFFF;
    slot->first = func_00195A30(colorB, param);
    slot->second = func_00195A30(colorB, param);
    slot->third = func_00195A30(colorA | 0xFF000000, param);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00175160);

extern void func_00175160();
extern void func_00177CD0(void *);

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

    VU0_LOAD_VF($vf10, D_0037F680);
    VU0_LOAD_VF($vf11, D_0037F690);
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
            func_001750C0(work, index, 0);
        } else {
            if (age == 0) {
                func_00175160(work, index, &axis);
                func_001750C0(work, index, 0);
                part->accumulator = startA;
                part->unk0C = startB;
                part->color = 0x80808080;
            } else if (age > 0) {
                f32 speed = part->unk0C;
                s32 remain;
                f32 blend;

                part->unk0C = speed * decay;
                part->accumulator = part->accumulator + speed;
                func_00175160(work, index, &axis);
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
                func_001750C0(work, index, func_00195A30(effBlendColor(0, part->color, blend), fadeParam));
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
    handle->unk5C = work->unk48;
    if (active != 0) {
        func_00177CD0(handle);
    }
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00175598);

void func_00175770(u64 arg0) {
    u64 effectParams;

    effectParams = effParamTableGetBlock(arg0, 0);
    func_00175598(effectParams);
}

void func_00175790(void) {
    func_00175598();
}

void func_001757A8(PcpFlashWork8 *work) {
    func_00177FA8(work->resourceHandle);
    func_003297C8(work->ownedBuffer);
}

void func_001757D8(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_001757E8(PcpFlashWork8 *work, u32 value) {
    work->unkD8 = value;
}

void func_001757F0(PcpFlashWork8 *work, f32 value)
{
    work->unkDC = value;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001757F8);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001758E8);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001759C0);

void func_00175BE8(PcpFlashWork8 *work, s32 index, void *orientation) {
    PcpFlashPtc20B *part;

    part = &work->parts[index];
    part->accumulator += part->increment;
}

extern void func_001758E8();
extern void func_001759C0();

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

    VU0_LOAD_VF($vf10, D_0037F680);
    VU0_LOAD_VF($vf11, D_0037F690);
    __asm__ volatile(".set noreorder\n\tvsub.xyzw $vf10, $vf10, $vf11\n\t.set reorder");
    VU0_STORE_VF($vf10, &axis);
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
            func_001757F8(work, index, 0);
        } else {
            if (age == 0) {
                func_001758E8(work, index, &axis);
                func_001759C0(work, index, &axis);
                func_001757F8(work, index, 0);
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
                func_00175BE8(work, index, &axis);
                func_001759C0(work, index, &axis);
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
                func_001757F8(work, index, func_00195A30(effBlendColor(0, part->color, blend), fadeParam));
            }
            if (age == lifetime && restart != 0) {
                part->age = ~(effMiscRand(D_003AA868) % range);
                func_001757F8(work, index, 0);
            } else {
                part->age = part->age + 1;
            }
        }
    }
    handle = (PcpFlashHandle *)work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->unkD4 = work->unkD4 + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->unk5C = work->unkDC;
    func_00177FD8(handle);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00175EE8);

void func_00176118(u64 arg0) {
    u64 effectParams;

    effectParams = effParamTableGetBlock(arg0, 0);
    func_00175EE8(effectParams);
}

void func_00176138(void) {
    func_00175EE8();
}

void func_00176150(PcpFlashWork9 *work) {
    func_00177880(work->resourceHandle);
    func_003297C8(work->ownedBuffer);
}

void func_00176180(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00176190(PcpFlashWork9 *work, u32 value) {
    work->unk58 = value;
}

void func_00176198(PcpFlashWork9 *work, f32 value)
{
    work->unk5C = value;
}

void func_001761A0(PcpFlashWork9 *work, s32 flag, s32 param) {
    PcpFlashColorSlot5 *slot;
    s32 colorA;
    s32 colorB;

    slot = (PcpFlashColorSlot5 *)func_00177B78(work->resourceHandle);
    colorA = work->colorA & 0xFFFFFF;
    colorB = work->colorB & 0xFFFFFF;
    slot->color[0] = func_00195A30(colorB, param);
    slot->color[1] = func_00195A30(colorB, param);
    if (flag & 1) {
        slot->color[2] = func_00195A30(0x80000000, param);
        slot->color[3] = func_00195A30(colorA | 0xFF000000, param);
        slot->color[4] = func_00195A30(0x80000000, param);
    } else {
        slot->color[2] = func_00195A30(0xFF000000, param);
        slot->color[3] = func_00195A30(colorA | 0x40000000, param);
        slot->color[4] = func_00195A30(0xFF000000, param);
    }
}

#define VEC3_SPLAT(v, x) ((v)[0] = (x), (v)[1] = (x), (v)[2] = (x))

/* vu0 routine: billboard corner offsets for a scaling particle on an arc, built from a normalised direction and its perpendicular */
void func_00176290(PcpFlashWork9 *work, s32 index)
{
    PcpFlashPtc14 *part = &work->parts[index];
    f32 *quad = func_00177B60(work->resourceHandle, index);
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
    ratio = size / part->unk0C;
    VEC3_SPLAT(scaleA, size);
    widthB = work->unk38 * ratio;
    widthC = work->unk34 * ratio;
    VEC3_SPLAT(scaleB, widthB);
    VEC3_SPLAT(scaleC, widthC);
    unit[0] = func_003407A0(part->accumulator);
    unit[1] = 0;
    sinv = sdfSinPoly(part->accumulator);
    unit[2] = sinv;
    offset[0] = unit[0] * work->unk3C;
    offset[1] = 0;
    offset[2] = sinv * work->unk3C;
    height = work->unk44;
    D_003B1290[0] = unit[0] * height;
    D_003B1290[1] = height + -1.0f;
    D_003B1290[2] = sinv * height;
    VU0_LOAD_VF(vf10, D_003B1290);
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

void func_00176478(PcpFlashWork9 *work, s32 index) {
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
    fadeParam = work->unk58;
    for (index = 0; index < count; index++, part++) {
        s32 age = part->age;

        if (lifetime < age) {
            func_001761A0(work, index, 0);
        } else {
            if (age == 0) {
                part->unk0C = maxScale;
                if (ramp == 0) {
                    part->scale = maxScale;
                } else {
                    part->scale = 0.0f;
                }
                func_00176290(work, index);
                func_001761A0(work, index, 0);
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
                func_00176478(work, index);
                func_00176290(work, index);
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
                func_001761A0(work, index, func_00195A30(effBlendColor(0, part->color, blend), fadeParam));
            }
            if (age == lifetime && restart != 0) {
                part->age = ~(effMiscRand(D_003AA868) % range);
                func_001761A0(work, index, 0);
            } else {
                part->age = part->age + 1;
            }
        }
    }
    handle = (PcpFlashHandle *)work->resourceHandle;
    handle->origin[0] = work->origin[0];
    work->unk54 = work->unk54 + 1;
    handle->origin[1] = work->origin[1];
    handle->origin[2] = work->origin[2];
    handle->unk5C = work->unk5C;
    func_001778B0(handle);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00176758);

void func_00176898(u64 arg0) {
    u64 effectParams;

    effectParams = effParamTableGetBlock(arg0, 0);
    func_00176758(effectParams);
}

void func_001768B8(void) {
    func_00176758();
}

void func_001768D0(PcpFlashWork10 *work) {
    func_00177CA0(work->resourceHandle);
    func_003297C8(work->ownedBuffer);
}

void func_00176900(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00176910(PcpFlashWork10 *work, u32 value) {
    work->unk48 = value;
}

void func_00176918(PcpFlashWork10 *work, f32 value)
{
    work->unk4C = value;
}

void func_00176920(PcpFlashWork10 *work, s32 index, s32 param)
{
    PcpFlashColorSlot *slot;
    s32 colorA;
    s32 colorB;

    slot = (PcpFlashColorSlot *)func_00177E90(work->resourceHandle, index);
    colorA = work->colorA & 0xFFFFFF;
    colorB = work->colorB & 0xFFFFFF;
    slot->first = func_00195A30(colorB, param);
    slot->second = func_00195A30(colorB, param);
    slot->third = func_00195A30(colorA | 0xFF000000, param);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001769C0);

extern void func_001769C0();
extern void func_00177CD0(void *);

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

    VU0_LOAD_VF($vf10, D_0037F680);
    VU0_LOAD_VF($vf11, D_0037F690);
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
            func_00176920(work, index, 0);
        } else {
            if (age == 0) {
                func_001769C0(work, index, axis);
                func_00176920(work, index, 0);
                part->accumulator = startA;
                part->unk0C = startB;
                part->color = 0x80808080;
            } else if (age > 0) {
                f32 speed = part->unk0C;
                s32 remain;
                f32 blend;

                part->unk0C = speed * decay;
                part->accumulator = part->accumulator + speed;
                func_001769C0(work, index, axis);
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
                func_00176920(work, index, func_00195A30(effBlendColor(0, part->color, blend), fadeParam));
            }
            if (age == lifetime && restart != 0) {
                part->age = 0;
            } else {
                part->age = part->age + 1;
            }
        }
    }
    scale = work->unk38 * work->unk4C;
    handle = (PcpFlashHandle *)work->resourceHandle;
    work->updateCount = work->updateCount + 1;
    handle->origin[0] = work->origin[0] + axis[0] * scale;
    handle->origin[1] = work->origin[1] + axis[1] * scale;
    handle->origin[2] = work->origin[2] + axis[2] * scale;
    handle->unk5C = work->unk4C;
    if (active != 0) {
        func_00177CD0(handle);
    }
}
