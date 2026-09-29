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

/* Particle elements. Only the fields touched by the matched accumulators are
   known; each struct's size is the element stride used to index its array. */
typedef struct PcpFlashPtc10 PcpFlashPtc10;
typedef struct PcpFlashPtc14 PcpFlashPtc14;
typedef struct PcpFlashPtc1C PcpFlashPtc1C;
typedef struct PcpFlashPtc20A PcpFlashPtc20A;
typedef struct PcpFlashPtc20B PcpFlashPtc20B;

struct PcpFlashPtc10 {
    u8 pad00[0x08];
    f32 unk08;
    u8 pad0C[0x04];
};

struct PcpFlashPtc14 {
    u8 pad00[0x10];
    f32 unk10;
};

struct PcpFlashPtc1C {
    u8 pad00[0x18];
    f32 unk18;
};

struct PcpFlashPtc20A {
    u8 pad00[0x08];
    f32 unk08;
    f32 unk0C;
    f32 unk10;
    f32 unk14;
    f32 unk18;
    f32 unk1C;
};

struct PcpFlashPtc20B {
    u8 pad00[0x08];
    f32 unk08;
    f32 unk0C;
    u8 pad10[0x08];
    f32 unk18;
    f32 unk1C;
};

/* Per-effect work areas. Only the fields touched by the matched spawn,
   teardown and accumulator helpers are known; the update bodies are still
   assembly. Each work area belongs to the effect whose initializer is noted. */
typedef struct PcpFlashWork1 PcpFlashWork1;
typedef struct PcpFlashWork2 PcpFlashWork2;
typedef struct PcpFlashWork3 PcpFlashWork3;
typedef struct PcpFlashWork4 PcpFlashWork4;
typedef struct PcpFlashWork5 PcpFlashWork5;
typedef struct PcpFlashWork6 PcpFlashWork6;
typedef struct PcpFlashWork7 PcpFlashWork7;
typedef struct PcpFlashWork8 PcpFlashWork8;
typedef struct PcpFlashWork9 PcpFlashWork9;
typedef struct PcpFlashWork10 PcpFlashWork10;

/* func_0016A088 */
struct PcpFlashWork1 {
    u8 pad00[0x20];
    u32 colorA;
    u32 colorB;
    u8 pad28[0x10];
    u32 unk38;
    f32 unk3C;
    u32 unk40;
    u32 unk44;
};

/* func_0016A6C0 */
struct PcpFlashWork2 {
    u8 pad00[0x10];
    u32 unk10;
    u8 pad14[0x0C];
    u32 unk20;
    u32 colorA;
    u32 colorB;
    u8 pad2C[0x10];
    u32 unk3C;
    u8 *unk40;
    u32 unk44;
    u32 unk48;
    f32 unk4C;
    u32 unk50;
    u32 unk54;
};

/* func_0016AFF0 */
struct PcpFlashWork3 {
    u8 pad00[0x24];
    u32 colorA;
    u32 colorB;
    u8 pad2C[0x14];
    f32 particleIncrement;
    u32 unk44;
    PcpFlashPtc14 *parts;
    u32 unk4C;
    u32 unk50;
    f32 unk54;
    u32 unk58;
    u32 unk5C;
};

/* func_0016B800 */
struct PcpFlashWork4 {
    u8 pad00[0x28];
    u32 unk28;
    u32 unk2C;
    u8 pad30[0x14];
    f32 unk44;
    u32 unk48;
    u32 unk4C;
    PcpFlashPtc1C *parts;
    u32 unk54;
    u32 unk58;
    f32 unk5C;
    u32 unk60;
    u32 unk64;
};

/* func_0016C0E8 */
struct PcpFlashWork5 {
    u8 pad00[0x28];
    u32 colorA;
    u32 colorB;
    u8 pad30[0x20];
    f32 unk50;
    u32 unk54;
    PcpFlashPtc10 *parts;
    u32 unk5C;
    u32 unk60;
    f32 unk64;
    u8 pad68[0x10];
    u32 unk78;
    u32 unk7C;
};

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
    u32 unk5C;
    u32 unk60;
};

/* func_0016D2A8 */
struct PcpFlashWork7 {
    u8 pad00[0x24];
    u32 colorA;
    u32 colorB;
    u8 pad2C[0x18];
    u32 unk44;
    f32 unk48;
    u32 unk4C;
    u32 unk50;
};

/* func_0016D940 */
struct PcpFlashWork8 {
    u8 pad00[0x28];
    u32 unk28;
    u32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    f32 unk3C;
    u8 pad40[0x90];
    PcpFlashPtc20B *parts;
    u32 unkD4;
    u32 unkD8;
    f32 unkDC;
    u32 unkE0;
    u32 unkE4;
};

/* func_0016E290 */
struct PcpFlashWork9 {
    u8 pad00[0x2C];
    u32 colorA;
    u32 colorB;
    u8 pad34[0x14];
    f32 unk48;
    u32 unk4C;
    PcpFlashPtc14 *parts;
    u32 unk54;
    u32 unk58;
    f32 unk5C;
    u32 unk60;
    u32 unk64;
};

/* func_0016EB00 */
struct PcpFlashWork10 {
    u8 pad00[0x24];
    u32 colorA;
    u32 colorB;
    u8 pad2C[0x1C];
    u32 unk48;
    f32 unk4C;
    u32 unk50;
    u32 unk54;
};

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
    func_00170048(work->unk44);
    func_002D0918(work->unk40);
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

    slot = func_00170238(work->unk44, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    *(s32 *)(slot + 0) = func_0018DDF8(rgb2, param);
    *(s32 *)(slot + 4) = func_0018DDF8(rgb2, param);
    *(s32 *)(slot + 8) = func_0018DDF8(rgb1 | 0xFF000000, param);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016A2D0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", effFlashUpdateWork1);

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
    func_0016FC28(work->unk54);
    func_002D0918(work->unk50);
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

    slot = func_0016FF20(work->unk54, index);
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

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016A9D0);

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

INCLUDE_ASM(const s32, "effect/effPCPFlash", effFlashUpdateStreak);

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
    func_0016FC28(work->unk5C);
    func_002D0918(work->unk58);
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

    slot = func_0016FF20(work->unk5C, index);
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
    part->unk10 += work->particleIncrement;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", effFlashUpdateWork3);

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
    func_00170350(work->unk64);
    func_002D0918(work->unk60);
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
    part->unk18 += work->unk44;
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
    func_0016FC28(work->unk7C);
    func_002D0918(work->unk78);
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

    slot = func_0016FF20(work->unk7C, index);
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
    part->unk08 += work->unk50;
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
    func_0016FC28(work->unk60);
    func_002D0918(work->unk5C);
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

    slot = func_0016FF20(work->unk60, index);
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

INCLUDE_ASM(const s32, "effect/effPCPFlash", effFlashSpawnParticle6);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016CE00);

void func_0016CFC0(PcpFlashWork6 *work, s32 index)
{
    PcpFlashPtc20A *part;

    part = &work->parts[index];
    part->unk10 += part->unk08;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", effFlashUpdateWork6);

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
    func_00170048(work->unk50);
    func_002D0918(work->unk4C);
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

    slot = func_00170238(work->unk50, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    *(s32 *)(slot + 0) = func_0018DDF8(rgb2, param);
    *(s32 *)(slot + 4) = func_0018DDF8(rgb2, param);
    *(s32 *)(slot + 8) = func_0018DDF8(rgb1 | 0xFF000000, param);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016D508);

INCLUDE_ASM(const s32, "effect/effPCPFlash", effFlashUpdateWork7);

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
    func_00170350(work->unkE4);
    func_002D0918(work->unkE0);
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

void func_0016DF90(PcpFlashWork8 *work, s32 index)
{
    PcpFlashPtc20B *part;

    part = &work->parts[index];
    part->unk18 += part->unk08;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", effFlashUpdateWork8);

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
    func_0016FC28(work->unk64);
    func_002D0918(work->unk60);
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

    slot = func_0016FF20(work->unk64, index);
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
    part->unk10 += work->unk48;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", effFlashUpdateWork9);

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
    func_00170048(work->unk54);
    func_002D0918(work->unk50);
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

    slot = func_00170238(work->unk54, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    *(s32 *)(slot + 0) = func_0018DDF8(rgb2, param);
    *(s32 *)(slot + 4) = func_0018DDF8(rgb2, param);
    *(s32 *)(slot + 8) = func_0018DDF8(rgb1 | 0xFF000000, param);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_0016ED68);

INCLUDE_ASM(const s32, "effect/effPCPFlash", effFlashUpdateWork10);
