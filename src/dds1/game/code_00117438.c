#include "common.h"

extern s64 func_0011B938(void);
extern void func_0011B940(void);

extern u32 D_003BAAAC;

extern s32 D_003BAA00;

void func_001184A8(u32 arg0, u32 arg1, u32 arg2, u8 arg3);
INCLUDE_ASM(const s32, "game/code_00117438", func_00117438);

INCLUDE_ASM(const s32, "game/code_00117438", func_001174C0);

void func_00117568(u32 *arg0, u32 arg1) {
    *arg0 = arg1;
}

u32 func_00117570(u32 *arg0) {
    return *arg0;
}

void func_00117578(float arg0, s32 arg1) {
    *(float *)(arg1 + 0xc) = arg0 * *(float *)(arg1 + 8);
}

float func_00117588(s32 arg0) {
    return *(float *)(arg0 + 0xc) / *(float *)(arg0 + 8);
}

void func_001175A8(s32 arg0) {
    *(u32 *)(arg0 + 4) = *(u32 *)(arg0 + 4) | 8;
}

void func_001175B8(s32 arg0) {
    *(u32 *)(arg0 + 4) = *(u32 *)(arg0 + 4) & 0xfffffff7;
}

void func_001175D0(s32 arg0) {
    *(u32 *)(arg0 + 4) = *(u32 *)(arg0 + 4) | 0x20;
}

void func_001175E0(s32 arg0) {
    *(u32 *)(arg0 + 4) = *(u32 *)(arg0 + 4) & 0xffffffdf;
}

INCLUDE_ASM(const s32, "game/code_00117438", func_001175F8);

u32 func_00117648(s32 arg0) {
    return *(u32 *)(arg0 + 0x18);
}

u32 func_00117650(s32 arg0) {
    return *(u32 *)(arg0 + 0x18);
}

INCLUDE_ASM(const s32, "game/code_00117438", func_00117658);

INCLUDE_ASM(const s32, "game/code_00117438", func_00117678);

INCLUDE_ASM(const s32, "game/code_00117438", func_001176A0);

INCLUDE_ASM(const s32, "game/code_00117438", func_00117730);

s32 sysBumpTickCounters(void) {
    s32 base;

    base = D_003BAA00;
    *(u32 *)(base + 0x34) += 1;
    *(u32 *)(base + 0x38) += 1;
    return 0;
}

void func_001177A8(void) {
    func_0010BBE0();
    func_0021F4B8();
    *(u32 *)(D_003BAA00 + 0xa5c) = 8;
    func_0011A238();
    func_00120C08(0);
    func_002CC7D8();
    func_002CCDC8();
    eventUpdateFlaggedEntries();
    dds3ForEachEntry();
    func_001ACCF0();
}

void func_00117808(void) {
}

INCLUDE_ASM(const s32, "game/code_00117438", func_00117810);

INCLUDE_ASM(const s32, "game/code_00117438", func_00117C48);

INCLUDE_ASM(const s32, "game/code_00117438", func_00118020);

void func_001180F8(void) {
    dds3WorkInit(D_003BAAAC);
}

void sysFirePendingCallback(void) {
    if (D_003BAAAC == 0) {
        return;
    }
    func_0011B940();
}

u8 func_00118140(s64 arg0) {
    s64 temp_v0;

    temp_v0 = func_0011B938();
    return temp_v0 == arg0;
}

INCLUDE_ASM(const s32, "game/code_00117438", func_00118170);

INCLUDE_ASM(const s32, "game/code_00117438", func_00118210);

INCLUDE_ASM(const s32, "game/code_00117438", func_00118310);

INCLUDE_ASM(const s32, "game/code_00117438", func_00118368);

INCLUDE_ASM(const s32, "game/code_00117438", func_00118408);

INCLUDE_ASM(const s32, "game/code_00117438", func_001184A8);

void sysDispatchCmd(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    func_001184A8(arg0, arg1, arg2, (u8)arg3);
}

INCLUDE_ASM(const s32, "game/code_00117438", func_00118570);

void func_00118620(u32 arg0, u32 arg1, u32 arg2, u8 arg3) {
    eventRunContext(10, arg1, arg2, arg0, arg3);
}

void func_00118648(u32 arg0, u32 arg1, u32 arg2, u8 arg3) {
    eventRunContext(7, arg1, arg2, arg0, arg3);
}

void sysDispatchSubCmd(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    func_00118648(arg0, arg1, arg2, (u8)arg3);
}

INCLUDE_ASM(const s32, "game/code_00117438", func_00118688);

INCLUDE_ASM(const s32, "game/code_00117438", func_001189A0);

INCLUDE_ASM(const s32, "game/code_00117438", func_00118D70);

INCLUDE_ASM(const s32, "game/code_00117438", func_00118DD8);

INCLUDE_ASM(const s32, "game/code_00117438", func_00118E38);

void func_00119000(s32 arg0, u16 arg1) {
    *(u16 *)(arg0 + 0xe) = (*(u16 *)(arg0 + 0xe) & 0x8000) | (arg1 & 0x7fff);
}

INCLUDE_ASM(const s32, "game/code_00117438", func_00119018);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BA9E0);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BA9E8);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BA9E9);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BA9EA);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BA9EC);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BA9F0);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BA9F4);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BA9F8);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BA9F9);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BAA00);

INCLUDE_SDATA(const s32, "game/code_00117438", D_003BAA04);

