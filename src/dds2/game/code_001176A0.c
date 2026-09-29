#include "common.h"

extern u32 D_00435E80;

extern s32 D_00435DD0;

extern s64 func_0011D588(void);

typedef struct EvtScaledValue {
    u32 value;
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

#define SDF_PACKED_FLAG 0x8000
#define SDF_PACKED_VALUE_MASK 0x7FFF

extern void func_0011D590(void);

INCLUDE_ASM(const s32, "game/code_001176A0", func_001176A0);

INCLUDE_ASM(const s32, "game/code_001176A0", func_00117728);

void func_001177D0(u32 *destination, u32 value) {
    *destination = value;
}

u32 func_001177D8(u32 *source) {
    return *source;
}

void evtScaleValueByMultiplier(float multiplier, EvtScaledValue *value) {
    value->scaled = multiplier * value->base;
}

float evtGetValueScaleFactor(EvtScaledValue *value) {
    return value->scaled / value->base;
}

void func_00117810(EvtScaledValue *value) {
    value->flags = value->flags | 8;
}

void func_00117820(EvtScaledValue *value) {
    value->flags = value->flags & 0xfffffff7;
}

void func_00117838(EvtScaledValue *value) {
    value->flags = value->flags | 0x20;
}

void func_00117848(EvtScaledValue *value) {
    value->flags = value->flags & 0xffffffdf;
}

INCLUDE_ASM(const s32, "game/code_001176A0", evtSpawnActionObj11);

u32 func_001178B0(EvtScaledValue *value) {
    return value->value18;
}

u32 func_001178B8(EvtScaledValue *value) {
    return value->value18;
}

/* Load the vector at the slot's +0x18 pointer into VF10. */
void func_001178C0(EvtScaledValue *value) {
    void *vec = (void *)func_001178B8(value);

    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf10, 0(%0)\n"
        ".set reorder"
        :
        : "r"(vec)
        : "memory"
    );
}

/* Same, from the second quadword at +0x18. */
void func_001178E0(EvtScaledValue *value) {
    void *vec = (void *)((u8 *)func_001178B8(value) + 0x10);

    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf10, 0(%0)\n"
        ".set reorder"
        :
        : "r"(vec)
        : "memory"
    );
}

INCLUDE_ASM(const s32, "game/code_001176A0", sdfCreateRuntimeTask);

INCLUDE_ASM(const s32, "game/code_001176A0", sdfDestroyRuntimeTask);

s32 sdfBumpTickCounters(void) {
    SdfRuntime *runtime;

    runtime = (SdfRuntime *)D_00435DD0;
    runtime->firstTick += 1;
    runtime->secondTick += 1;
    return 0;
}

void func_00117A10(void) {
    scrClearProcessGlobals();
    func_0023A028();
    ((SdfRuntime *)D_00435DD0)->updateMode = 8;
    func_0011AB38();
    func_00122B58(0);
    ptyClearProfileRecords();
    func_0026CE90();
    ptyRebuildAllProfiles();
    evtUpdateFlaggedEntries();
    dds3ForEachEntry();
    func_0011D438();
    func_001B7900();
}

void func_00117A80(void) {
}

INCLUDE_ASM(const s32, "game/code_001176A0", func_00117A88);

INCLUDE_ASM(const s32, "game/code_001176A0", func_00118008);

INCLUDE_ASM(const s32, "game/code_001176A0", func_00118598);

void func_00118680(void) {
    dds3WorkInit(D_00435E80);
}

void sdfFirePendingCallback(void) {
    if (D_00435E80 == 0) {
        return;
    }
    func_0011D590();
}

u8 func_001186C8(s64 expected) {
    s64 current;

    current = func_0011D588();
    return current == expected;
}

INCLUDE_ASM(const s32, "game/code_001176A0", func_001186F8);

INCLUDE_ASM(const s32, "game/code_001176A0", func_00118798);

INCLUDE_ASM(const s32, "game/code_001176A0", sdfResetChannels);

INCLUDE_ASM(const s32, "game/code_001176A0", func_001188F0);

INCLUDE_ASM(const s32, "game/code_001176A0", func_001189D0);

INCLUDE_ASM(const s32, "game/code_001176A0", func_00118AB0);

INCLUDE_ASM(const s32, "game/code_001176A0", sdfDispatchCmd);

INCLUDE_ASM(const s32, "game/code_001176A0", sdfDispatchUnitScriptDefault9);

void func_00118C58(u32 context, u32 first, u32 second, u8 flags) {
    evtRunContext(10, first, second, context, flags);
}

void func_00118C80(u32 context, u32 first, u32 second, u8 flags) {
    evtRunContext(7, first, second, context, flags);
}

void sdfDispatchSubCmd(u32 context, u32 first, u32 second, u32 flags) {
    func_00118C80(context, first, second, (u8)flags);
}

INCLUDE_ASM(const s32, "game/code_001176A0", func_00118CC0);

INCLUDE_ASM(const s32, "game/code_001176A0", func_00118D60);

INCLUDE_ASM(const s32, "game/code_001176A0", func_001190B0);

INCLUDE_ASM(const s32, "game/code_001176A0", func_00119480);

INCLUDE_ASM(const s32, "game/code_001176A0", sdfQueryChannelBits);

INCLUDE_ASM(const s32, "game/code_001176A0", func_00119548);

void sdfSetPackedValuePreservingFlag(SdfPackedValue *item, u16 value) {
    item->flagsAndValue = (item->flagsAndValue & SDF_PACKED_FLAG) | (value & SDF_PACKED_VALUE_MASK);
}

INCLUDE_ASM(const s32, "game/code_001176A0", func_00119728);

INCLUDE_SDATA(const s32, "game/code_001176A0", D_00435DB0);

INCLUDE_SDATA(const s32, "game/code_001176A0", D_00435DB8);

INCLUDE_SDATA(const s32, "game/code_001176A0", D_00435DBC);

INCLUDE_SDATA(const s32, "game/code_001176A0", D_00435DC0);

INCLUDE_SDATA(const s32, "game/code_001176A0", D_00435DC4);

INCLUDE_SDATA(const s32, "game/code_001176A0", D_00435DC5);

INCLUDE_SDATA(const s32, "game/code_001176A0", D_00435DD0);

INCLUDE_SDATA(const s32, "game/code_001176A0", D_00435DD4);

