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

/*
 * Work shared by the 0x1634A0/0x164000 effect pair (identical observed
 * layout): a float pair scaled per frame plus two resource handles.
 */
typedef struct {
    u8 unk00[0x1C]; /* 0x00 */
    f32 unk1C;      /* 0x1C scaled from unk54 */
    f32 unk20;      /* 0x20 scaled from unk58 */
    u8 unk24[0x2C]; /* 0x24 */
    u32 unk50;      /* 0x50 settable param */
    f32 unk54;      /* 0x54 scale source for unk1C */
    f32 unk58;      /* 0x58 scale source for unk20 */
    u32 unk5C;      /* 0x5C handle released by func_0015B8B8 */
    u32 unk60;      /* 0x60 handle released by func_002D0918 */
} EffPCPThunderWork;

/* Work for the remaining thunder effects (u32 params and handles only). */
typedef struct {
    u8 unk00[0x40]; /* 0x00 */
    u32 unk40;      /* 0x40 */
    u8 unk44[0x04]; /* 0x44 */
    u32 unk48;      /* 0x48 */
    u32 unk4C;      /* 0x4C settable param */
    u32 unk50;      /* 0x50 settable param */
    u32 unk54;      /* 0x54 */
    u32 unk58;      /* 0x58 settable param */
    u32 unk5C;      /* 0x5C handle released by func_0015B8B8 */
    u32 unk60;      /* 0x60 handle released by func_0015B8B8/func_002D0918 */
    u32 unk64;      /* 0x64 handle released by func_002D0918 */
    u8 unk68[0x40]; /* 0x68 */
    u32 unkA8;      /* 0xA8 settable param */
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

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00164A30);

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

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_001656B8);

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

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00166138);

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

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00166B38);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00166C18);

INCLUDE_ASM(const s32, "effect/effPCPThunder", func_00167070);
