#include "common.h"

extern u64 func_0016AEB0(u64, u64);

extern u32 func_0017AD10(u32);

/* Shared resource handed between scatter effects. func_00173018 creates it,
   func_001730B8 takes a reference, func_00173068 releases it. */
typedef struct PcpScatterRes PcpScatterRes;

struct PcpScatterRes {
    u32 unk00;
    s32 refCount;
};

extern void *func_00328D68(s32 size);

extern u32 func_0032C138(u32 resId);

extern PcpScatterRes *func_0017AC70(u32 resId);

typedef struct PcpScatterWork4 PcpScatterWork4;

/* func_001730D0 */
struct PcpScatterWork4 {
    u8 pad00[0x40];
    s128 unk40;
    u8 pad50[0x12C];
    f32 unk17C;
    u32 unk180;
    u32 unk184;
    u32 unk188;
};

extern PcpScatterWork4 *func_0017AD28(void *param0, void *param1);

extern void func_0017DCA8(u32 param0, u32 param1);

typedef struct PcpScatterWork5 PcpScatterWork5;

/* func_00173B48 */
struct PcpScatterWork5 {
    u8 pad00[0x40];
    s128 unk40;
    u8 pad50[0x130];
    f32 unk180;
    u32 unk184;
    u32 unk188;
    u32 unk18C;
    u32 unk190;
};

typedef struct PcpScatterWork6 PcpScatterWork6;

/* func_00174680 */
struct PcpScatterWork6 {
    u8 pad00[0x40];
    s128 unk40;
    u8 pad50[0x138];
    f32 unk188;
    u32 unk18C;
    u32 unk190;
    u32 unk194;
    u32 unk198;
};

void func_001787E0(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;
    u64 temp_v2;

    temp_v0 = func_0016AEB0(arg0, 0);
    temp_v1 = func_0016AEB0(arg0, 1);
    temp_v2 = func_0016AEB0(arg0, 2);
    func_001784F8(temp_v0, temp_v1, temp_v2);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00178848);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00178938);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001789C0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00178B80);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00179168);

void func_00179178(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x7c) = arg1;
}

void func_00179180(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x3c) = *(float *)(arg1 + 0x3c) * arg0;
    *(float *)(arg1 + 0x4c) = *(float *)(arg1 + 0x4c) * arg0;
    *(float *)(arg1 + 0x50) = *(float *)(arg1 + 0x50) * arg0;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001791A8);

void func_00179438(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;
    u64 temp_v2;

    temp_v0 = func_0016AEB0(arg0, 0);
    temp_v1 = func_0016AEB0(arg0, 1);
    temp_v2 = func_0016AEB0(arg0, 2);
    func_001791A8(temp_v0, temp_v1, temp_v2);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001794A0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00179590);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00179618);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00179780);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00179D78);

void func_00179D88(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x6c) = arg1;
}

void func_00179D90(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x40) = *(float *)(arg1 + 0x40) * arg0;
    *(float *)(arg1 + 0x44) = *(float *)(arg1 + 0x44) * arg0;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00179DB0);

void func_0017A058(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;
    u64 temp_v2;

    temp_v0 = func_0016AEB0(arg0, 0);
    temp_v1 = func_0016AEB0(arg0, 1);
    temp_v2 = func_0016AEB0(arg0, 2);
    func_00179DB0(temp_v0, temp_v1, temp_v2);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017A0C0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017A1C0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017A248);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017A340);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017A8A0);

void func_0017A8B0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x54) = arg1;
}

void func_0017A8B8(void) {
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017A8C0);

void func_0017A9C8(s32 arg0) {
    if (*(s32 *)(arg0 + 0x30) != 0) {
        func_0017ACC0(*(s32 *)(arg0 + 0x30));
    }
    func_00333918(*(u32 *)(arg0 + 0x28));
    func_003297C8(*(u32 *)(arg0 + 0x2c));
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017AA08);

void func_0017ABE0(s32 arg0, u32 arg1) {
    u32 temp_v0;

    temp_v0 = func_0017AC70(arg1);
    *(u32 *)(arg0 + 0x30) = temp_v0;
}

void func_0017AC10(s32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = func_0017AD10(*(u32 *)(arg1 + 0x30));
    *(u32 *)(arg0 + 0x30) = temp_v0;
}

s32 func_0017AC40(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 0x20) + arg1 * 0x60;
}

s32 func_0017AC58(s32 arg0, s32 arg1) {
    return *(s32 *)(arg0 + 0x24) + arg1 * 0x18;
}

PcpScatterRes *func_0017AC70(u32 resId)
{
    PcpScatterRes *res;

    res = func_00328D68(8);
    res->unk00 = func_0032C138(resId);
    res->refCount = 1;
    return res;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017ACC0);

u32 func_0017AD10(u32 arg0) {
    *(s32 *)((s32)arg0 + 4) = *(s32 *)((s32)arg0 + 4) + 1;
    return arg0;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017AD28);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017AF40);

PcpScatterWork4 *func_0017AF88(PcpScatterWork4 *work) {
    PcpScatterWork4 *child;

    child = func_0017AD28(&work->unk40, NULL);
    func_0017DCA8(child->unk184, work->unk184);
    return child;
}

void func_0017AFD0(s32 arg0) {
    func_0017D9E0(*(u32 *)(arg0 + 0x184));
    func_003297C8(*(u32 *)(arg0 + 0x188));
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017B000);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017B390);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017B520);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017B718);

void func_0017B730(PcpScatterWork4 *work, f32 value)
{
    work->unk17C = value;
}

void func_0017B738(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x180) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017B740);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017B7A0);

void func_0017B9D0(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0016AEB0(arg0, 0);
    temp_v1 = func_0016AEB0(arg0, 1);
    func_0017B7A0(temp_v0, temp_v1);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017BA18);

void func_0017BA60(s32 arg0) {
    func_0017D9E0(*(u32 *)(arg0 + 0x18c));
    func_003297C8(*(u32 *)(arg0 + 400));
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017BA90);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017BE08);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017BFA8);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017C250);

void func_0017C268(PcpScatterWork5 *work, f32 value)
{
    work->unk180 = value;
}

void func_0017C270(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x184) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017C278);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017C2D8);

void func_0017C4D8(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0016AEB0(arg0, 0);
    temp_v1 = func_0016AEB0(arg0, 1);
    func_0017C2D8(temp_v0, temp_v1);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017C520);

void func_0017C568(s32 arg0) {
    func_0017D9E0(*(u32 *)(arg0 + 0x194));
    func_003297C8(*(u32 *)(arg0 + 0x198));
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017C598);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017C988);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017CB28);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017CE00);

void func_0017CE18(PcpScatterWork6 *work, f32 value)
{
    work->unk188 = value;
}

void func_0017CE20(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18c) = arg1;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017CE28);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017CE88);

void func_0017D078(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0016AEB0(arg0, 0);
    temp_v1 = func_0016AEB0(arg0, 1);
    func_0017CE88(temp_v0, temp_v1);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017D0C0);

void func_0017D108(s32 arg0) {
    func_0017D9E0(*(u32 *)(arg0 + 0x134));
    func_003297C8(*(u32 *)(arg0 + 0x138));
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017D138);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017D3D8);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_0017D560);
