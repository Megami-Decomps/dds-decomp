#include "common.h"
#include "pcp_vu0.h"

extern u64 effParamTableGetBlock(u64, u64);

extern u32 effMiscRand(void *state);

extern u8 D_003AA868[];

#define EFF_THUNDER_FRAGMENT_GREY 0x80808080

/* Two independently sampled ranges and a constant greyscale color. */
typedef struct {
    u32 firstRandom;  /* 0x00: modulo fragmentModulusA */
    u32 secondRandom; /* 0x04: modulo fragmentModulusB, plus one */
    u32 color08;      /* 0x08: 0x80808080 */
} EffThunderFrag; /* 0x0C */

/* Three randomized direction components and two active countdowns. */
typedef struct {
    f32 directionX; /* 0x00: random value in [-1, 1] */
    f32 directionY; /* 0x04 */
    f32 directionZ; /* 0x08 */
    u32 cnt0C;      /* 0x0C random modulus, then decremented */
    u32 cnt10;      /* 0x10 random modulus + 1, then decremented */
} EffThunderCell; /* 0x14 */

/* 0x20-byte sub-element holding a handle released by func_0015B8B8. */
typedef struct {
    u8 pad00[0x04]; /* 0x00 */
    f32 f04;        /* 0x04 */
    f32 f08;        /* 0x08 */
    f32 f0C;        /* 0x0C */
    f32 f10;        /* 0x10 */
    f32 f14;        /* 0x14 */
    f32 f18;        /* 0x18 */
    u32 handle1C;   /* 0x1C released by func_0015B8B8 */
} EffThunderSub; /* 0x20 */

/* Work for the remaining thunder effects (u32 params and handles only). */
typedef struct {
    u128 quad00;      /* 0x00 copied as one quadword on spawn */
    u8 pad10[0x08];   /* 0x10 */
    s32 elementCount; /* 0x18: thunder element count */
    u8 unk1C[0x08]; /* 0x1C */
    u32 cellModulusA; /* 0x24 */
    u32 cellModulusB; /* 0x28 */
    u8 unk2C[0x04];   /* 0x2C */
    u32 fragmentModulusA; /* 0x30 */
    u32 fragmentModulusB; /* 0x34 */
    u8 unk38[0x08]; /* 0x38 */
    u32 unk40;      /* 0x40 */
    u8 unk44[0x04]; /* 0x44 */
    EffThunderCell *cells; /* 0x48: thunder element array */
    u32 unk4C;      /* 0x4C settable param */
    u32 unk50;      /* 0x50 settable param */
    union {
        u32 resource;
        EffThunderFrag *fragments;
    } fragmentData; /* 0x54: array pointer / released handle */
    u32 unk58;      /* 0x58 settable param */
    u32 unk5C;      /* 0x5C handle released by func_0015B8B8 */
    u32 unk60;      /* 0x60 handle released by func_0015B8B8/func_002D0918 */
    u32 unk64;      /* 0x64 handle released by func_002D0918 */
    s32 subElementCount; /* 0x68 */
    u8 pad6C[0x14]; /* 0x6C */
    f32 unk80;
    f32 unk84;
    f32 unk88;
    f32 unk8C;
    f32 unk90;
    u8 pad94[0x04];
    f32 unk98;
    f32 unk9C;
    u8 padA0[0x04];
    EffThunderSub *subElements; /* 0xA4 */
    u32 unkA8;      /* 0xA8 settable param */
    u32 unkAC;      /* 0xAC handle released by func_002D0918 */
} EffPCPThunderWorkB;

typedef struct EffThunderScale {
    u8 pad00[0x1C];
    f32 scaledA; /* 0x1C */
    f32 scaledB; /* 0x20 */
    u8 pad24[0x30];
    f32 sourceA; /* 0x54 */
    f32 sourceB; /* 0x58 */
} EffThunderScale;

extern f32 func_00341240(void *state);

void func_0016B0F8(u64 arg0) {
    u64 temp_v0;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    func_0016AF38(temp_v0);
}

void func_0016B118(void) {
    func_0016AF38();
}

void func_0016B130(EffPCPThunderWorkB *work) {
    func_001634A8(work->unk5C);
    func_003297C8(work->unk60);
}

void func_0016B160(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0016B170(EffPCPThunderWorkB *work, u32 value) {
    work->unk50 = value;
}

void func_0016B178(float scale, EffThunderScale *work) {
    work->scaledA = work->sourceA * scale;
    work->scaledB = work->sourceB * scale;
}

u32 func_0016B198(u32 arg0) {
    return arg0;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016B1A0);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016B3D8);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016B750);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016B928);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016BA68);

void func_0016BC28(EffPCPThunderWorkB *work) {
    func_001634A8(work->unk5C);
    func_003297C8(work->unk60);
}

void func_0016BC58(u64 arg0) {
    u64 temp_v0;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    func_0016BA68(temp_v0);
}

void func_0016BC78(void) {
    func_0016BA68();
}

void func_0016BC90(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0016BCA0(EffPCPThunderWorkB *work, u32 value) {
    work->unk50 = value;
}

void func_0016BCA8(float scale, EffThunderScale *work) {
    work->scaledA = work->sourceA * scale;
    work->scaledB = work->sourceB * scale;
}

u32 func_0016BCC8(u32 arg0) {
    return arg0;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016BCD0);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016BF08);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016C1F8);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016C350);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016C490);

INCLUDE_ASM(const s32, "effect/effPCPThunder", effThunderDestroySubs);

void func_0016C6F0(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0016C700(EffPCPThunderWorkB *work, u32 value) {
    work->unkA8 = value;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016C708);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016C800);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016CD68);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016D070);

void func_0016D228(EffPCPThunderWorkB *work) {
    func_001634A8(work->unk60);
    func_003297C8(work->unk64);
}

void func_0016D258(u8 *p, void *src) {
    __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(p + 0x10));
    __asm__ volatile(".set noreorder\n\tlqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(src));
    __asm__ volatile(".set noreorder\n\tsqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(p + 0x10) : "memory");
    __asm__ volatile(".set noreorder\n\tvsub.xyzw $vf11, $vf11, $vf10\n\t.set reorder");
    __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(p));
    __asm__ volatile(".set noreorder\n\tvadd.xyzw $vf10, $vf10, $vf11\n\t.set reorder");
    __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(p) : "memory");
}

void func_0016D288(EffPCPThunderWorkB *work, u32 value) {
    work->unk58 = value;
}

u32 func_0016D290(u32 arg0) {
    return arg0;
}

void func_0016D298(EffPCPThunderWorkB *work) {
    func_001648C0(work->unk60, work->unk40, work->cells, work->unk50);
}

void func_0016D2C0(EffPCPThunderWorkB *work) {
    func_001649E0(work->unk60, work->unk40, work->cells, work->unk50);
}

void func_0016D2E8(EffPCPThunderWorkB *work) {
    func_00164848(work->unk60, work->unk40, work->cells, work->unk50);
}

/* Sample per-fragment timing values; the color is a fixed neutral grey. */
void effThunderRandomizeFrag(EffPCPThunderWorkB *work, s32 index) {
    EffThunderFrag *frag = &work->fragmentData.fragments[index];

    frag->firstRandom = effMiscRand(&D_003AA868) % work->fragmentModulusA;
    frag->secondRandom = effMiscRand(&D_003AA868) % work->fragmentModulusB + 1;
    frag->color08 = EFF_THUNDER_FRAGMENT_GREY;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016D3B0);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016D9D8);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016DB28);

void func_0016DD20(EffPCPThunderWorkB *work) {
    func_001634A8(work->unk5C);
    func_001634A8(work->unk60);
    func_003297C8(work->unk64);
}

void func_0016DD58(u8 *p, void *src) {
    __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(p + 0x10));
    __asm__ volatile(".set noreorder\n\tlqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(src));
    __asm__ volatile(".set noreorder\n\tsqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(p + 0x10) : "memory");
    __asm__ volatile(".set noreorder\n\tvsub.xyzw $vf11, $vf11, $vf10\n\t.set reorder");
    __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(p));
    __asm__ volatile(".set noreorder\n\tvadd.xyzw $vf10, $vf10, $vf11\n\t.set reorder");
    __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(p) : "memory");
}

void func_0016DD88(EffPCPThunderWorkB *work, u32 value) {
    work->unk58 = value;
}

void effThunderRandomizeFrag2(EffPCPThunderWorkB *work, s32 index) {
    EffThunderFrag *frag = &work->fragmentData.fragments[index];

    frag->firstRandom = effMiscRand(&D_003AA868) % work->fragmentModulusA;
    frag->secondRandom = effMiscRand(&D_003AA868) % work->fragmentModulusB + 1;
    frag->color08 = EFF_THUNDER_FRAGMENT_GREY;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016DE30);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016E468);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016E5B8);

void func_0016E748(EffPCPThunderWorkB *work) {
    func_001634A8(work->unk50);
    func_003297C8(work->fragmentData.resource);
}

void func_0016E778(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0016E788(EffPCPThunderWorkB *work, u32 value) {
    work->unk4C = value;
}

/* Spread all three direction components over [-1, 1] before sampling lifetimes. */
void effThunderRandomizeCell(EffPCPThunderWorkB *work, s32 index) {
    EffThunderCell *cell = work->cells + index;
    f32 v;

    v = func_00341240(&D_003AA868) - 0.5f;
    cell->directionX = v + v;
    v = func_00341240(&D_003AA868) - 0.5f;
    cell->directionY = v + v;
    v = func_00341240(&D_003AA868) - 0.5f;
    cell->directionZ = v + v;
    cell->cnt0C = effMiscRand(&D_003AA868) % work->cellModulusA;
    cell->cnt10 = effMiscRand(&D_003AA868) % work->cellModulusB + 1;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016E870);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016ECC8);
