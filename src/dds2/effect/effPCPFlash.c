#include "common.h"
#include "pcp_vu0.h"

extern u64 effParamTableGetBlock(u64, u64);

/* Per-effect work areas. Only the fields touched by the matched spawn,
   teardown and accumulator helpers are known; the update bodies are still
   assembly. Each work area belongs to the effect whose initializer is noted. */
typedef struct PcpFlashWork1 PcpFlashWork1;

/* func_0016A088 */
struct PcpFlashWork1 {
    u8 pad00[0x20];
    u32 colorA;
    u32 colorB;
    u8 pad28[0x10];
    u32 unk38;
    f32 unk3C;
    u32 ownedBuffer;
    u32 resourceHandle;
};

extern s32 func_00177E90(s32 base, s32 index);

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
    u32 unk24;
    u32 unk28;
    u8 pad2C[0x08];
    f32 maxScale;
    u8 pad38[0x08];
    PcpFlashRotatingParticle *parts;
    s32 updateCount;
    u32 unk48;
    f32 unk4C;
    u32 ownedBuffer;
    u32 resourceHandle;
};

typedef struct PcpFlashPtc14 PcpFlashPtc14;

struct PcpFlashPtc14 {
    u8 pad00[0x10];
    f32 accumulator;
};

typedef struct PcpFlashWork3 PcpFlashWork3;

/* func_0016AFF0 */
struct PcpFlashWork3 {
    u8 pad00[0x24];
    u32 colorA;
    u32 colorB;
    u8 pad2C[0x14];
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
    u8 pad00[0x18];
    f32 accumulator;
};

typedef struct PcpFlashWork4 PcpFlashWork4;

/* func_0016B800 */
struct PcpFlashWork4 {
    u8 pad00[0x28];
    u32 colorA;
    u32 colorB;
    u8 pad30[0x14];
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
    u8 pad00[0x08];
    f32 accumulator;
    u8 pad0C[0x04];
};

typedef struct PcpFlashWork5 PcpFlashWork5;

/* func_0016C0E8 */
struct PcpFlashWork5 {
    u8 pad00[0x28];
    u32 colorA;
    u32 colorB;
    u8 pad30[0x20];
    f32 increment;
    u32 unk54;
    PcpFlashPtc10 *parts;
    u32 unk5C;
    u32 unk60;
    f32 unk64;
    u8 pad68[0x10];
    u32 ownedBuffer;
    u32 resourceHandle;
};

typedef struct PcpFlashPtc20A PcpFlashPtc20A;

struct PcpFlashPtc20A {
    u8 pad00[0x08];
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
    u8 pad00[0x30];
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
    u8 pad00[0x24];
    u32 colorA;
    u32 colorB;
    u8 pad2C[0x18];
    u32 unk44;
    f32 unk48;
    u32 ownedBuffer;
    u32 resourceHandle;
};

typedef struct PcpFlashPtc20B PcpFlashPtc20B;

struct PcpFlashPtc20B {
    u8 pad00[0x08];
    f32 increment;
    f32 unk0C;
    u8 pad10[0x08];
    f32 accumulator;
    f32 unk1C;
};

typedef struct PcpFlashWork8 PcpFlashWork8;

/* func_0016D940 */
struct PcpFlashWork8 {
    u8 pad00[0x28];
    u32 colorA;
    u32 colorB;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    u8 pad40[0x90];
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
    u8 pad00[0x2C];
    u32 colorA;
    u32 colorB;
    u8 pad34[0x14];
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
    u8 pad00[0x24];
    u32 colorA;
    u32 colorB;
    u8 pad2C[0x1C];
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
    u8 pad1C[0x10];
};

typedef struct PcpFlashRotationWork {
    u8 pad00[0x40];
    PcpFlashRotatingParticle *parts;
} PcpFlashRotationWork;

extern void func_00336768(void *orientation, f32 angle);

void func_00171E00(u64 arg0) {
    u64 temp_v0;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    func_00171CE0(temp_v0);
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

void func_00171E88(PcpFlashWork1 *work, s32 index, s32 param)
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

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001720A8);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00172318);

void func_001724B0(u64 arg0) {
    u64 temp_v0;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    func_00172318(temp_v0);
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

void func_00172538(PcpFlashWork2 *work, s32 flag, s32 param) {
    PcpFlashColorSlot5 *slot;
    s32 colorA;
    s32 colorB;

    slot = (PcpFlashColorSlot5 *)func_00177B78(work->resourceHandle);
    colorA = work->unk24 & 0xFFFFFF;
    colorB = work->unk28 & 0xFFFFFF;
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

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00172628);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001727A0);

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

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001729B0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00172C48);

void func_00172E68(u64 arg0) {
    u64 temp_v0;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    func_00172C48(temp_v0);
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

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00172FE0);

void func_001731C8(PcpFlashWork3 *work, s32 index) {
    PcpFlashPtc14 *part;

    part = &work->parts[index];
    part->accumulator += work->increment;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001731F0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00173458);

void func_00173690(u64 arg0) {
    u64 temp_v0;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    func_00173458(temp_v0);
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
    u64 temp_v0;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    func_00173D40(temp_v0);
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

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00174108);

void func_001742D0(PcpFlashWork5 *work, s32 index) {
    PcpFlashPtc10 *part;

    part = &work->parts[index];
    part->accumulator += work->increment;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001742F0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00174648);

void func_00174808(u64 arg0) {
    u64 temp_v0;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    func_00174648(temp_v0);
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

void func_00174980(PcpFlashWork6 *work, s32 index) {
    PcpFlashPtc20A *part = work->parts + index;
    f32 v;
    f32 size;

    part->accumulator = func_00341240(D_003AA868) * 6.2831853f;
    v = func_00341240(D_003AA868) * 0.3f + 0.7f;
    size = work->unk40 * v;
    part->unk1C = size;
    part->unk0C = size;
    v = (func_00341240(D_003AA868) * 0.5f + 0.5f) * 0.5f;
    part->unk14 = work->unk38 * v;
    part->unk18 = work->unk3C * v;
    part->increment = work->unk44 * ((func_00341240(D_003AA868) - 0.5f) * 2.0f);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00174A58);

void func_00174C18(PcpFlashWork6 *work, s32 index) {
    PcpFlashPtc20A *part;

    part = &work->parts[index];
    part->accumulator += part->increment;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00174C38);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00174F00);

void func_00175038(u64 arg0) {
    u64 temp_v0;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    func_00174F00(temp_v0);
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

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001752F8);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00175598);

void func_00175770(u64 arg0) {
    u64 temp_v0;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    func_00175598(temp_v0);
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

void func_00175BE8(PcpFlashWork8 *work, s32 index) {
    PcpFlashPtc20B *part;

    part = &work->parts[index];
    part->accumulator += part->increment;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00175C08);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00175EE8);

void func_00176118(u64 arg0) {
    u64 temp_v0;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    func_00175EE8(temp_v0);
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

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00176290);

void func_00176478(PcpFlashWork9 *work, s32 index) {
    PcpFlashPtc14 *part;

    part = &work->parts[index];
    part->accumulator += work->increment;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001764A0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00176758);

void func_00176898(u64 arg0) {
    u64 temp_v0;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    func_00176758(temp_v0);
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

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00176B58);
