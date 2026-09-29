#include "common.h"

extern s64 func_0011B938(void);
extern void func_0011B940(void);

extern u32 D_003BAAAC;

extern s32 D_003BAA00;
typedef struct EvtScaledValue {
    u32 unk0;
    u32 flags;
    f32 base;
    f32 scaled;
} EvtScaledValue;

typedef struct SdfRuntime {
    u8 pad00[0x34];
    u32 firstTick;
    u32 secondTick;
    u8 pad3C[0xA20];
    u32 updateMode;
} SdfRuntime;

typedef struct SdfPackedValue {
    u8 pad00[0xE];
    u16 flagsAndValue;
} SdfPackedValue;


void func_001184A8(u32 arg0, u32 arg1, u32 arg2, u8 arg3);
INCLUDE_ASM(const s32, "game/code_00117438", func_00117438);

INCLUDE_ASM(const s32, "game/code_00117438", func_001174C0);

void func_00117568(u32 *arg0, u32 arg1) {
    *arg0 = arg1;
}

u32 func_00117570(u32 *arg0) {
    return *arg0;
}

void func_00117578(float multiplier, EvtScaledValue *value) {
    value->scaled = multiplier * value->base;
}

float func_00117588(EvtScaledValue *value) {
    return value->scaled / value->base;
}

void func_001175A8(EvtScaledValue *value) {
    value->flags = value->flags | 8;
}

void func_001175B8(EvtScaledValue *value) {
    value->flags = value->flags & 0xfffffff7;
}

void func_001175D0(EvtScaledValue *value) {
    value->flags = value->flags | 0x20;
}

void func_001175E0(EvtScaledValue *value) {
    value->flags = value->flags & 0xffffffdf;
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

s32 sdfBumpTickCounters(void) {
    SdfRuntime *runtime;

    runtime = (SdfRuntime *)D_003BAA00;
    runtime->firstTick += 1;
    runtime->secondTick += 1;
    return 0;
}

void func_001177A8(void) {
    scrClearProcessGlobals();
    func_0021F4B8();
    ((SdfRuntime *)D_003BAA00)->updateMode = 8;
    func_0011A238();
    func_00120C08(0);
    func_002CC7D8();
    func_002CCDC8();
    evtUpdateFlaggedEntries();
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

void sdfFirePendingCallback(void) {
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

void sdfDispatchCmd(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    func_001184A8(arg0, arg1, arg2, (u8)arg3);
}

INCLUDE_ASM(const s32, "game/code_00117438", func_00118570);

void func_00118620(u32 arg0, u32 arg1, u32 arg2, u8 arg3) {
    evtRunContext(10, arg1, arg2, arg0, arg3);
}

void func_00118648(u32 arg0, u32 arg1, u32 arg2, u8 arg3) {
    evtRunContext(7, arg1, arg2, arg0, arg3);
}

void sdfDispatchSubCmd(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    func_00118648(arg0, arg1, arg2, (u8)arg3);
}

INCLUDE_ASM(const s32, "game/code_00117438", func_00118688);

INCLUDE_ASM(const s32, "game/code_00117438", func_001189A0);

INCLUDE_ASM(const s32, "game/code_00117438", func_00118D70);

INCLUDE_ASM(const s32, "game/code_00117438", func_00118DD8);

INCLUDE_ASM(const s32, "game/code_00117438", func_00118E38);

void func_00119000(SdfPackedValue *item, u16 value) {
    item->flagsAndValue = (item->flagsAndValue & 0x8000) | (value & 0x7fff);
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

