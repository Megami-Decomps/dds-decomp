#include "common.h"

/* Packed effect parameter-set accessor (see game/code_001624D0). */
extern void *func_00163258(void *data, s32 index);
extern void func_001632E0(void *work);
extern void func_00163E10(void *work);

extern void func_0015B8B8(u32 handle);
extern void func_0015CC58(u32 param0, u32 param1, u32 param2, u32 param3);
extern void func_0015CCD0(u32 param0, u32 param1, u32 param2, u32 param3);
extern void func_0015CDF0(u32 param0, u32 param1, u32 param2, u32 param3);
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
    f32 unk1C;      /* 0x1C scaled from unk54 */
    f32 unk20;      /* 0x20 scaled from unk58 */
    u8 unk24[0x2C]; /* 0x24 */
    u32 unk50;      /* 0x50 settable param */
    f32 unk54;      /* 0x54 scale source for unk1C */
    f32 unk58;      /* 0x58 scale source for unk20 */
    u32 unk5C;      /* 0x5C handle released by func_0015B8B8 */
    u32 unk60;      /* 0x60 handle released by func_002D0918 */
} EffPCPThunderWork;

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
    s32 unk18;      /* 0x18 element count */
    u8 unk1C[0x08]; /* 0x1C */
    u32 unk24;      /* 0x24 random modulus */
    u32 unk28;      /* 0x28 random modulus */
    u8 unk2C[0x04]; /* 0x2C */
    u32 unk30;      /* 0x30 random modulus */
    u32 unk34;      /* 0x34 random modulus */
    u8 unk38[0x08]; /* 0x38 */
    u32 unk40;      /* 0x40 */
    u8 unk44[0x04]; /* 0x44 */
    EffThunderCell *unk48; /* 0x48 thunder element array */
    u32 unk4C;      /* 0x4C settable param */
    u32 unk50;      /* 0x50 settable param */
    u32 unk54;      /* 0x54 fragment array base */
    u32 unk58;      /* 0x58 settable param */
    u32 unk5C;      /* 0x5C handle released by func_0015B8B8 */
    u32 unk60;      /* 0x60 handle released by func_0015B8B8/func_002D0918 */
    u32 unk64;      /* 0x64 handle released by func_002D0918 */
    s32 unk68;      /* 0x68 sub-element count */
    u8 pad6C[0x38]; /* 0x6C */
    EffThunderSub *unkA4; /* 0xA4 sub-element array */
    u32 unkA8;      /* 0xA8 settable param */
    u32 unkAC;      /* 0xAC handle released by func_002D0918 */
} EffPCPThunderWorkB;

void func_001634A0(void *data) {
    void *work;

    work = func_00163258(data, 0);
    func_001632E0(work);
}
void func_001634C0(void *work) {
    func_001632E0(work);
}

void func_001634D8(EffPCPThunderWork *work) {
    func_0015B8B8(work->unk5C);
    func_002D0918(work->unk60);
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00163508);

void func_00163518(EffPCPThunderWork *work, u32 value) {
    work->unk50 = value;
}

void func_00163520(f32 value, EffPCPThunderWork *work) {
    work->unk1C = work->unk54 * value;
    work->unk20 = work->unk58 * value;
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

    work = func_00163258(data, 0);
    func_00163E10(work);
}

void func_00164020(void *work) {
    func_00163E10(work);
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00164038);

void func_00164048(EffPCPThunderWork *work, u32 value) {
    work->unk50 = value;
}

void func_00164050(f32 value, EffPCPThunderWork *work) {
    work->unk1C = work->unk54 * value;
    work->unk20 = work->unk58 * value;
}

u32 func_00164070(u32 arg0) {
    return arg0;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00164078);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_001642B0);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_001645A0);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_001646F8);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00164838);

void func_00164A30(EffPCPThunderWorkB *work) {
    s32 count = work->unk68;
    s32 i = 0;

    if (count > 0) {
        do {
            func_0015B8B8(work->unkA4[i].handle1C);
            i++;
        } while (i < count);
    }
    func_002D0918(work->unkAC);
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00164A98);

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

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00165600);

void func_00165630(EffPCPThunderWorkB *work, u32 value) {
    work->unk58 = value;
}

u32 func_00165638(u32 arg0) {
    return arg0;
}

void func_00165640(EffPCPThunderWorkB *work) {
    func_0015CCD0(work->unk60, work->unk40, work->unk48, work->unk50);
}

void func_00165668(EffPCPThunderWorkB *work) {
    func_0015CDF0(work->unk60, work->unk40, work->unk48, work->unk50);
}

void func_00165690(EffPCPThunderWorkB *work) {
    func_0015CC58(work->unk60, work->unk40, work->unk48, work->unk50);
}

void func_001656B8(EffPCPThunderWorkB *work, s32 index) {
    EffThunderFrag *frag = (EffThunderFrag *)(work->unk54 + index * 12);

    frag->unk00 = effMiscRand(&D_0034DF38) % work->unk30;
    frag->unk04 = effMiscRand(&D_0034DF38) % work->unk34 + 1;
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

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00166100);

void func_00166130(EffPCPThunderWorkB *work, u32 value) {
    work->unk58 = value;
}

void func_00166138(EffPCPThunderWorkB *work, s32 index) {
    EffThunderFrag *frag = (EffThunderFrag *)(work->unk54 + index * 12);

    frag->unk00 = effMiscRand(&D_0034DF38) % work->unk30;
    frag->unk04 = effMiscRand(&D_0034DF38) % work->unk34 + 1;
    frag->color08 = 0x80808080;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_001661D8);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00166810);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00166960);

void func_00166AF0(EffPCPThunderWorkB *work) {
    func_0015B8B8(work->unk50);
    func_002D0918(work->unk54);
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00166B20);

void func_00166B30(EffPCPThunderWorkB *work, u32 value) {
    work->unk4C = value;
}

void func_00166B38(EffPCPThunderWorkB *work, s32 index) {
    EffThunderCell *cell = work->unk48 + index;
    f32 v;

    v = func_002E8398(&D_0034DF38) - 0.5f;
    cell->f00 = v + v;
    v = func_002E8398(&D_0034DF38) - 0.5f;
    cell->f04 = v + v;
    v = func_002E8398(&D_0034DF38) - 0.5f;
    cell->f08 = v + v;
    cell->cnt0C = effMiscRand(&D_0034DF38) % work->unk24;
    cell->cnt10 = effMiscRand(&D_0034DF38) % work->unk28 + 1;
}

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00166C18);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00167070);
