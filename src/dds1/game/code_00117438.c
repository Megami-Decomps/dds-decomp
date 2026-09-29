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
    u8 pad10[8];
    u32 value18;
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

void func_00117568(u32 *value, u32 newValue) {
    *value = newValue;
}

u32 func_00117570(u32 *value) {
    return *value;
}

void evtScaleValueByMultiplier(float multiplier, EvtScaledValue *value) {
    value->scaled = multiplier * value->base;
}

float evtGetValueScaleFactor(EvtScaledValue *value) {
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

typedef struct ActionObj {
    u8 unk0[4];   /* 0x0 */
    s32 unk4;     /* 0x4 */
    s32 unk8;     /* 0x8 */
    u8 unkC[0xC]; /* 0xC */
    void *unk18;  /* 0x18 */
} ActionObj;

extern ActionObj *func_00110880();

ActionObj *evtSpawnActionObj11(s32 a, s32 b, s32 c) {
    ActionObj *obj = func_00110880(0x11);

    obj->unk18 = (void *)b;
    obj->unk4 = a;
    obj->unk8 = c;
    return obj;
}

u32 func_00117648(EvtScaledValue *value) {
    return value->value18;
}

u32 func_00117650(EvtScaledValue *value) {
    return value->value18;
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
    ptyInitRuntime();
    func_00120C08(0);
    func_002CC7D8();
    ptyRebuildAllProfiles();
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

u8 func_00118140(s64 expectedValue) {
    s64 currentValue;

    currentValue = func_0011B938();
    return currentValue == expectedValue;
}

INCLUDE_ASM(const s32, "game/code_00117438", func_00118170);

INCLUDE_ASM(const s32, "game/code_00117438", func_00118210);

extern u8 D_0032A6F0[];
extern void func_00118210();

void sdfResetChannels(void) {
    u8 *entry;
    u32 i;

    sdfFirePendingCallback();
    entry = D_0032A6F0;
    for (i = 0; i < 8; i++) {
        func_00118210(entry);
        entry += 0xA4;
    }
    func_001180F8();
}

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

extern u8 *D_003BAA50;
extern u32 func_001189A0(s32 index, s32 arg1, SdfPackedValue *packed);

u32 sdfQueryChannelBits(s32 index, s32 arg1, SdfPackedValue *packed) {
    u32 result;

    if (D_003BAA50[index * 0x38 + 0x24] != 2) {
        return 0;
    }
    result = func_001189A0(index, arg1, packed);
    if ((result & (packed->flagsAndValue & 0x7FFF)) == 0) {
        result = 0;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_00117438", func_00118E38);

void sdfSetPackedValuePreservingFlag(SdfPackedValue *item, u16 value) {
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

