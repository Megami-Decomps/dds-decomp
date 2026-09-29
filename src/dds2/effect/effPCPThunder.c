#include "common.h"

extern u64 effParamTableGetBlock(u64, u64);

extern u32 effMiscRand(void *state);

extern u8 D_003AA868[];

/* 12-byte randomized fragment (see func_001656B8). */
typedef struct {
    u32 unk00;    /* 0x00 random value modulo work param */
    u32 unk04;    /* 0x04 random value modulo work param, plus 1 */
    u32 color08;  /* 0x08 always grey 0x80808080 */
} EffThunderFrag; /* 0x0C */

/* 20-byte thunder element (see func_00166B38/func_00167070): randomized on
 * setup (direction floats plus moduli), then counted down while active. */
typedef struct {
    f32 f00;      /* 0x00 (float rand - 0.5) * 2 */
    f32 f04;      /* 0x04 */
    f32 f08;      /* 0x08 */
    u32 cnt0C;    /* 0x0C random modulus, then decremented */
    u32 cnt10;    /* 0x10 random modulus + 1, then decremented */
} EffThunderCell; /* 0x14 */

/* 0x20-byte sub-element holding a handle released by func_0015B8B8. */
typedef struct {
    u8 pad00[0x1C]; /* 0x00 */
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
    u32 fragmentBase; /* 0x54: fragment array base */
    u32 unk58;      /* 0x58 settable param */
    u32 unk5C;      /* 0x5C handle released by func_0015B8B8 */
    u32 unk60;      /* 0x60 handle released by func_0015B8B8/func_002D0918 */
    u32 unk64;      /* 0x64 handle released by func_002D0918 */
    s32 subElementCount; /* 0x68 */
    u8 pad6C[0x38]; /* 0x6C */
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

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016B160);

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

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016BC90);

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

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016C6F0);

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

void func_0016D298(s32 arg0) {
    func_001648C0(*(u32 *)(arg0 + 0x60), *(u32 *)(arg0 + 0x40),
                                *(u32 *)(arg0 + 0x48), *(u32 *)(arg0 + 0x50));
}

void func_0016D2C0(s32 arg0) {
    func_001649E0(*(u32 *)(arg0 + 0x60), *(u32 *)(arg0 + 0x40),
                                *(u32 *)(arg0 + 0x48), *(u32 *)(arg0 + 0x50));
}

void func_0016D2E8(s32 arg0) {
    func_00164848(*(u32 *)(arg0 + 0x60), *(u32 *)(arg0 + 0x40),
                                *(u32 *)(arg0 + 0x48), *(u32 *)(arg0 + 0x50));
}

void effThunderRandomizeFrag(EffPCPThunderWorkB *work, s32 index) {
    EffThunderFrag *frag = (EffThunderFrag *)(work->fragmentBase + index * 12);

    frag->unk00 = effMiscRand(&D_003AA868) % work->fragmentModulusA;
    frag->unk04 = effMiscRand(&D_003AA868) % work->fragmentModulusB + 1;
    frag->color08 = 0x80808080;
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
    EffThunderFrag *frag = (EffThunderFrag *)(work->fragmentBase + index * 12);

    frag->unk00 = effMiscRand(&D_003AA868) % work->fragmentModulusA;
    frag->unk04 = effMiscRand(&D_003AA868) % work->fragmentModulusB + 1;
    frag->color08 = 0x80808080;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016DE30);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016E468);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016E5B8);

void func_0016E748(s32 arg0) {
    func_001634A8(*(u32 *)(arg0 + 0x50));
    func_003297C8(*(u32 *)(arg0 + 0x54));
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016E778);

void func_0016E788(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x4c) = arg1;
}

void effThunderRandomizeCell(EffPCPThunderWorkB *work, s32 index) {
    EffThunderCell *cell = work->cells + index;
    f32 v;

    v = func_00341240(&D_003AA868) - 0.5f;
    cell->f00 = v + v;
    v = func_00341240(&D_003AA868) - 0.5f;
    cell->f04 = v + v;
    v = func_00341240(&D_003AA868) - 0.5f;
    cell->f08 = v + v;
    cell->cnt0C = effMiscRand(&D_003AA868) % work->cellModulusA;
    cell->cnt10 = effMiscRand(&D_003AA868) % work->cellModulusB + 1;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016E870);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_0016ECC8);
