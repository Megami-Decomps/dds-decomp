#include "common.h"
#include "pcp_vu0.h"

extern u64 effParamTableGetBlock(u64, u64);

typedef struct {
    u8 pad00[0x24];
    u32 unk24;
    u32 resource28;
    u32 resource2C;
} EffPCPBossWork;

extern void func_0018D210();

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_0018C190);

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_0018C288);

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_0018C4E8);

void func_0018C618(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    temp_v1 = effParamTableGetBlock(arg0, 1);
    func_0018C4E8(temp_v0, temp_v1);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_0018C660);

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_0018C788);

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_0018C820);

void func_0018D210(dst, src)
    void *dst;
    void *src;
{
    PCP_COPY_VECTOR(dst, src);
}

void func_0018D220(arg0, arg1)
    s32 arg0;
    u32 arg1;
{
    *(u32 *)(arg0 + 0xA0) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_0018D228);

void func_0018D2C8(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;
    u64 temp_v2;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    temp_v1 = effParamTableGetBlock(arg0, 1);
    temp_v2 = effParamTableGetBlock(arg0, 2);
    func_0018D228(temp_v0, temp_v1, temp_v2);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_0018D330);

void effPCPBossFree(EffPCPBossWork *work) {
    func_0016A620(work->resource2C);
    func_0016A620(work->resource28);
    func_00328E48(work);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_0018D3E8);

void effPCPBossSetParameterVector(u8 *work, void *src) {
    PCP_COPY_VECTOR(work + 0x10, src);
}

void effPCPBossSetParameter(EffPCPBossWork *work, u32 value) {
    work->unk24 = value;
}

u32 func_0018D540(void) {
    return 0;
}

u32 func_0018D548(void) {
    return 0;
}

u32 func_0018D550(void) {
    return 0;
}

void func_0018D558(void) {
}

void func_0018D560(void) {
}

void func_0018D568(void) {
}

u32 func_0018D570(void) {
    return 0;
}

u32 func_0018D578(void) {
    return 0;
}

u32 func_0018D580(void) {
    return 0;
}

void func_0018D588(void) {
}

void func_0018D590(void) {
}

void func_0018D598(void) {
}

void func_0018D5A0(void) {
}

u32 func_0018D5A8(void) {
    return 0;
}

u32 func_0018D5B0(void) {
    return 0;
}

u32 func_0018D5B8(void) {
    return 0;
}

void func_0018D5C0(void) {
}

void func_0018D5C8(void) {
}

void func_0018D5D0(void) {
}

u32 func_0018D5D8(void) {
    return 0;
}

u32 func_0018D5E0(void) {
    return 0;
}

u32 func_0018D5E8(void) {
    return 0;
}

void func_0018D5F0(void) {
}

void func_0018D5F8(void) {
}

void func_0018D600(void) {
}

u32 func_0018D608(void) {
    return 0;
}

u32 func_0018D610(void) {
    return 0;
}

u32 func_0018D618(void) {
    return 0;
}

void func_0018D620(void) {
}

void func_0018D628(void) {
}

void func_0018D630(void) {
}

void func_0018D638(void) {
}

u32 func_0018D640(void) {
    return 0;
}

u32 func_0018D648(void) {
    return 0;
}

u32 func_0018D650(void) {
    return 0;
}

void func_0018D658(void) {
}

void func_0018D660(void) {
}

void func_0018D668(void) {
}

void func_0018D670(void) {
}

u32 func_0018D678(void) {
    return 0;
}

s32 func_0018D680(void) {
    return 0;
}

s32 func_0018D688(void) {
    return 0;
}

void func_0018D690(void) {
}

void func_0018D698(void) {
}

void func_0018D6A0(void) {
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_0018D6A8);

void func_0018D710(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    temp_v1 = effParamTableGetBlock(arg0, 1);
    func_0018D6A8(temp_v0, temp_v1);
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_0018D758);

void func_0018D7D0(void) {
    func_0018C788();
}

void func_0018D7E8(void) {
    func_0018C820();
}

void func_0018D800(void) {
    func_0018D210();
}

void func_0018D818(void) {
    func_0018D220();
}

INCLUDE_ASM(const s32, "effect/effPCPBoss", func_0018D830);

void func_0018DA38(u64 arg0) {
    u64 temp_v0;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    func_0018D830(temp_v0);
}

void func_0018DA58(void) {
    func_0018D830();
}
