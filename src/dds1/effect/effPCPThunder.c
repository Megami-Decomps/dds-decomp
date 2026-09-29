#include "common.h"
#include "pcp_vu0.h"

/* Packed effect parameter-set accessor (see game/code_001624D0). */
extern void *effParamTableGetBlock(void *data, s32 index);
extern void func_001632E0(void *work);
extern void func_00163E10(void *work);

extern void func_0015B8B8(u32 handle);
extern void func_0015CC58(u32 param0, u32 param1, void *cells, u32 param3);
extern void func_0015CCD0(u32 param0, u32 param1, void *cells, u32 param3);
extern void func_0015CDF0(u32 param0, u32 param1, void *cells, u32 param3);
extern void func_002D0918(u32 handle);
extern u32 effMiscRand(void *state);
extern f32 func_002E8398(void *state);
extern u8 D_0034DF38[];

/*
 * Work shared by the 0x1634A0/0x164000 effect pair (identical observed
 * layout): a quadword copied on spawn plus a float pair scaled per frame,
 * plus two resource handles.
 */
typedef struct {
    u128 quad00;      /* 0x00 copied as one quadword on spawn */
    u8 pad10[0x0C];   /* 0x10 */
    f32 scaledFirst; /* 0x1C scaled from baseFirst */
    f32 scaledSecond; /* 0x20 scaled from baseSecond */
    u8 unk24[0x2C]; /* 0x24 */
    u32 unk50;      /* 0x50 settable param */
    f32 baseFirst;   /* 0x54 */
    f32 baseSecond;  /* 0x58 */
    u32 unk5C;      /* 0x5C handle released by func_0015B8B8 */
    u32 unk60;      /* 0x60 handle released by func_002D0918 */
} EffPCPThunderWork;

/* 12-byte randomized fragment (see effThunderRandomizeFrag). */
typedef struct {
    u32 unk00;    /* 0x00 random value modulo work param */
    u32 unk04;    /* 0x04 random value modulo work param, plus 1 */
    u32 color08;  /* 0x08 always grey 0x80808080 */
} EffThunderFrag; /* 0x0C */

/* 20-byte thunder element (see effThunderRandomizeCell/func_00167070): randomized on
 * setup (direction floats plus moduli), then counted down while active. */
typedef struct {
    f32 directionX; /* 0x00 randomized to [-1, 1] */
    f32 directionY; /* 0x04 randomized to [-1, 1] */
    f32 directionZ; /* 0x08 randomized to [-1, 1] */
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
    s32 elementCount; /* 0x18 */
    u8 unk1C[0x08]; /* 0x1C */
    u32 cellFirstRange; /* 0x24 random modulus */
    u32 cellSecondRange; /* 0x28 random modulus */
    u8 unk2C[0x04]; /* 0x2C */
    u32 fragmentFirstRange; /* 0x30 random modulus */
    u32 fragmentSecondRange; /* 0x34 random modulus */
    u8 unk38[0x08]; /* 0x38 */
    u32 unk40;      /* 0x40 */
    u8 unk44[0x04]; /* 0x44 */
    EffThunderCell *cells; /* 0x48 thunder element array */
    u32 unk4C;      /* 0x4C settable param */
    u32 unk50;      /* 0x50 settable param */
    union {
        u32 resource;
        EffThunderFrag *fragments;
    } fragmentData; /* 0x54: fragment array / released handle */
    u32 unk58;      /* 0x58 settable param */
    u32 unk5C;      /* 0x5C handle released by func_0015B8B8 */
    u32 unk60;      /* 0x60 handle released by func_0015B8B8/func_002D0918 */
    u32 unk64;      /* 0x64 handle released by func_002D0918 */
    s32 subCount;    /* 0x68 */
    u8 pad6C[0x38]; /* 0x6C */
    EffThunderSub *subs; /* 0xA4 sub-element array */
    u32 unkA8;      /* 0xA8 settable param */
    u32 unkAC;      /* 0xAC handle released by func_002D0918 */
} EffPCPThunderWorkB;

void func_001634A0(void *data) {
    void *work;

    work = effParamTableGetBlock(data, 0);
    func_001632E0(work);
}
void func_001634C0(void *work) {
    func_001632E0(work);
}

void func_001634D8(EffPCPThunderWork *work) {
    func_0015B8B8(work->unk5C);
    func_002D0918(work->unk60);
}

void func_00163508(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00163518(EffPCPThunderWork *work, u32 value) {
    work->unk50 = value;
}

void func_00163520(f32 value, EffPCPThunderWork *work) {
    work->scaledFirst = work->baseFirst * value;
    work->scaledSecond = work->baseSecond * value;
}

u32 func_00163540(u32 arg0) {
    return arg0;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00163548);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00163780);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00163AF8);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00163CD0);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00163E10);

void func_00163FD0(EffPCPThunderWork *work) {
    func_0015B8B8(work->unk5C);
    func_002D0918(work->unk60);
}

void func_00164000(void *data) {
    void *work;

    work = effParamTableGetBlock(data, 0);
    func_00163E10(work);
}

void func_00164020(void *work) {
    func_00163E10(work);
}

void func_00164038(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00164048(EffPCPThunderWork *work, u32 value) {
    work->unk50 = value;
}

void func_00164050(f32 value, EffPCPThunderWork *work) {
    work->scaledFirst = work->baseFirst * value;
    work->scaledSecond = work->baseSecond * value;
}

u32 func_00164070(u32 arg0) {
    return arg0;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00164078);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_001642B0);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_001645A0);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_001646F8);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00164838);

void effThunderDestroySubs(EffPCPThunderWorkB *work) {
    s32 count = work->subCount;
    s32 i = 0;

    if (count > 0) {
        do {
            func_0015B8B8(work->subs[i].handle1C);
            i++;
        } while (i < count);
    }
    func_002D0918(work->unkAC);
}

void func_00164A98(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00164AA8(EffPCPThunderWorkB *work, u32 value) {
    work->unkA8 = value;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00164AB0);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00164BA8);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00165110);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00165418);

void func_001655D0(EffPCPThunderWorkB *work) {
    func_0015B8B8(work->unk60);
    func_002D0918(work->unk64);
}

void func_00165600(u8 *p, void *src) {
    __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(p + 0x10));
    __asm__ volatile(".set noreorder\n\tlqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(src));
    __asm__ volatile(".set noreorder\n\tsqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(p + 0x10) : "memory");
    __asm__ volatile(".set noreorder\n\tvsub.xyzw $vf11, $vf11, $vf10\n\t.set reorder");
    __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(p));
    __asm__ volatile(".set noreorder\n\tvadd.xyzw $vf10, $vf10, $vf11\n\t.set reorder");
    __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(p) : "memory");
}

void func_00165630(EffPCPThunderWorkB *work, u32 value) {
    work->unk58 = value;
}

u32 func_00165638(u32 arg0) {
    return arg0;
}

void func_00165640(EffPCPThunderWorkB *work) {
    func_0015CCD0(work->unk60, work->unk40, work->cells, work->unk50);
}

void func_00165668(EffPCPThunderWorkB *work) {
    func_0015CDF0(work->unk60, work->unk40, work->cells, work->unk50);
}

void func_00165690(EffPCPThunderWorkB *work) {
    func_0015CC58(work->unk60, work->unk40, work->cells, work->unk50);
}

void effThunderRandomizeFrag(EffPCPThunderWorkB *work, s32 index) {
    EffThunderFrag *frag = &work->fragmentData.fragments[index];

    frag->unk00 = effMiscRand(&D_0034DF38) % work->fragmentFirstRange;
    frag->unk04 = effMiscRand(&D_0034DF38) % work->fragmentSecondRange + 1;
    frag->color08 = 0x80808080;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00165758);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00165D80);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00165ED0);

void func_001660C8(EffPCPThunderWorkB *work) {
    func_0015B8B8(work->unk5C);
    func_0015B8B8(work->unk60);
    func_002D0918(work->unk64);
}

void func_00166100(u8 *p, void *src) {
    __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(p + 0x10));
    __asm__ volatile(".set noreorder\n\tlqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(src));
    __asm__ volatile(".set noreorder\n\tsqc2 $vf11, 0(%0)\n\t.set reorder" : : "r"(p + 0x10) : "memory");
    __asm__ volatile(".set noreorder\n\tvsub.xyzw $vf11, $vf11, $vf10\n\t.set reorder");
    __asm__ volatile(".set noreorder\n\tlqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(p));
    __asm__ volatile(".set noreorder\n\tvadd.xyzw $vf10, $vf10, $vf11\n\t.set reorder");
    __asm__ volatile(".set noreorder\n\tsqc2 $vf10, 0(%0)\n\t.set reorder" : : "r"(p) : "memory");
}

void func_00166130(EffPCPThunderWorkB *work, u32 value) {
    work->unk58 = value;
}

void effThunderRandomizeFrag2(EffPCPThunderWorkB *work, s32 index) {
    EffThunderFrag *frag = &work->fragmentData.fragments[index];

    frag->unk00 = effMiscRand(&D_0034DF38) % work->fragmentFirstRange;
    frag->unk04 = effMiscRand(&D_0034DF38) % work->fragmentSecondRange + 1;
    frag->color08 = 0x80808080;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_001661D8);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00166810);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00166960);

void func_00166AF0(EffPCPThunderWorkB *work) {
    func_0015B8B8(work->unk50);
    func_002D0918(work->fragmentData.resource);
}

void func_00166B20(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00166B30(EffPCPThunderWorkB *work, u32 value) {
    work->unk4C = value;
}

void effThunderRandomizeCell(EffPCPThunderWorkB *work, s32 index) {
    EffThunderCell *cell = work->cells + index;
    f32 v;

    v = func_002E8398(&D_0034DF38) - 0.5f;
    cell->directionX = v + v;
    v = func_002E8398(&D_0034DF38) - 0.5f;
    cell->directionY = v + v;
    v = func_002E8398(&D_0034DF38) - 0.5f;
    cell->directionZ = v + v;
    cell->cnt0C = effMiscRand(&D_0034DF38) % work->cellFirstRange;
    cell->cnt10 = effMiscRand(&D_0034DF38) % work->cellSecondRange + 1;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00166C18);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00167070);
