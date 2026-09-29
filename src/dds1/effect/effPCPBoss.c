#include "common.h"
#include "pcp_vu0.h"

/* Packed effect parameter-set accessor (see game/code_001624D0). */
extern void *effParamTableGetBlock(void *data, s32 index);
extern void func_00184890(void *work0, void *work1);
extern void func_00184B30(void *work);
extern void func_00184BC8(void *work);
extern void func_001855B8();
extern void func_001855D0(void *work0, void *work1, void *work2);
extern void func_00185A50(void *work0, void *work1);
extern void func_00185BD8(void *work);

extern void func_001629F0(u32 handle);
extern void func_002CFF98(void *work);

/* Boss effect work: settable param plus two handles released on free. */
typedef struct {
    u8 unk00[0x24]; /* 0x00 */
    u32 unk24;      /* 0x24 settable param */
    u32 resource28;  /* 0x28 released by func_001629F0 */
    u32 resource2C;  /* 0x2C released by func_001629F0 */
} EffPCPBossWork;

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_00184538);

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_00184630);

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_00184890);

void func_001849C0(void *data) {
    void *work0;
    void *work1;

    work0 = effParamTableGetBlock(data, 0);
    work1 = effParamTableGetBlock(data, 1);
    func_00184890(work0, work1);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_00184A08);

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_00184B30);

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_00184BC8);

void func_001855B8(dst, src)
void *dst;
void *src;
{
    PCP_COPY_VECTOR(dst, src);
}

void func_001855C8(u8 *work, s32 value) {
    *(s32 *)(work + 0xA0) = value;
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_001855D0);

void func_00185670(void *data) {
    void *work0;
    void *work1;
    void *work2;

    work0 = effParamTableGetBlock(data, 0);
    work1 = effParamTableGetBlock(data, 1);
    work2 = effParamTableGetBlock(data, 2);
    func_001855D0(work0, work1, work2);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_001856D8);

void effPCPBossFree(EffPCPBossWork *work) {
    func_001629F0(work->resource2C);
    func_001629F0(work->resource28);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_00185790);

void func_001858C8(void *work, void *src) {
    PCP_COPY_VECTOR((u8 *)work + 0x10, src);
}

void effPCPBossSetParameter(EffPCPBossWork *work, u32 value) {
    work->unk24 = value;
}

u32 func_001858E8(void) {
    return 0;
}

u32 func_001858F0(void) {
    return 0;
}

u32 func_001858F8(void) {
    return 0;
}

void func_00185900(void) {
}

void func_00185908(void) {
}

void func_00185910(void) {
}

u32 func_00185918(void) {
    return 0;
}

u32 func_00185920(void) {
    return 0;
}

u32 func_00185928(void) {
    return 0;
}

void func_00185930(void) {
}

void func_00185938(void) {
}

void func_00185940(void) {
}

void func_00185948(void) {
}

void func_00185950(void) {
}

void func_00185958(void) {
}

u32 func_00185960(void) {
    return 0;
}

void func_00185968(void) {
}

void func_00185970(void) {
}

void func_00185978(void) {
}

void func_00185980(void) {
}

void func_00185988(void) {
}

void func_00185990(void) {
}

void func_00185998(void) {
}

void func_001859A0(void) {
}

void func_001859A8(void) {
}

u32 func_001859B0(void) {
    return 0;
}

u32 func_001859B8(void) {
    return 0;
}

u32 func_001859C0(void) {
    return 0;
}

void func_001859C8(void) {
}

void func_001859D0(void) {
}

void func_001859D8(void) {
}

void func_001859E0(void) {
}

u32 func_001859E8(void) {
    return 0;
}

u32 func_001859F0(void) {
    return 0;
}

u32 func_001859F8(void) {
    return 0;
}

void func_00185A00(void) {
}

void func_00185A08(void) {
}

void func_00185A10(void) {
}

void func_00185A18(void) {
}

u32 func_00185A20(void) {
    return 0;
}

s32 func_00185A28(void) {
    return 0;
}

s32 func_00185A30(void) {
    return 0;
}

void func_00185A38(void) {
}

void func_00185A40(void) {
}

void func_00185A48(void) {
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_00185A50);

void func_00185AB8(void *data) {
    void *work0;
    void *work1;

    work0 = effParamTableGetBlock(data, 0);
    work1 = effParamTableGetBlock(data, 1);
    func_00185A50(work0, work1);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_00185B00);

void func_00185B78(void *work) {
    func_00184B30(work);
}

void func_00185B90(void *work) {
    func_00184BC8(work);
}

void func_00185BA8(void *work) {
    func_001855B8(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_00185BC0);

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_00185BD8);

void func_00185DE0(void *data) {
    void *work;

    work = effParamTableGetBlock(data, 0);
    func_00185BD8(work);
}

void func_00185E00(void *work) {
    func_00185BD8(work);
}
