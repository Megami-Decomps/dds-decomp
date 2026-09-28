#include "common.h"

extern u32 D_00435E80;

extern s32 D_00435DD0;

extern s64 func_0011D588(void);

typedef struct WorldSlotData {
    u32 value;
    u32 flags;
    f32 scale;
    f32 scaledValue;
    u8 pad10[8];
    u32 value18;
} WorldSlotData;

INCLUDE_ASM(const s32, "game/code_001176A0", func_001176A0);

INCLUDE_ASM(const s32, "game/code_001176A0", func_00117728);

void func_001177D0(u32 *arg0, u32 arg1) {
    *arg0 = arg1;
}

u32 func_001177D8(u32 *arg0) {
    return *arg0;
}

void func_001177E0(float value, WorldSlotData *slot) {
    slot->scaledValue = value * slot->scale;
}

float func_001177F0(WorldSlotData *slot) {
    return slot->scaledValue / slot->scale;
}

void func_00117810(WorldSlotData *slot) {
    slot->flags = slot->flags | 8;
}

void func_00117820(WorldSlotData *slot) {
    slot->flags = slot->flags & 0xfffffff7;
}

void func_00117838(WorldSlotData *slot) {
    slot->flags = slot->flags | 0x20;
}

void func_00117848(WorldSlotData *slot) {
    slot->flags = slot->flags & 0xffffffdf;
}

INCLUDE_ASM(const s32, "game/code_001176A0", func_00117860);

u32 func_001178B0(s32 arg0) {
    return *(u32 *)(arg0 + 0x18);
}

u32 func_001178B8(s32 arg0) {
    return *(u32 *)(arg0 + 0x18);
}

INCLUDE_ASM(const s32, "game/code_001176A0", func_001178C0);

INCLUDE_ASM(const s32, "game/code_001176A0", func_001178E0);

INCLUDE_ASM(const s32, "game/code_001176A0", func_00117908);

INCLUDE_ASM(const s32, "game/code_001176A0", func_00117998);

s32 sdfBumpTickCounters(void) {
    s32 base;

    base = D_00435DD0;
    *(u32 *)(base + 0x34) += 1;
    *(u32 *)(base + 0x38) += 1;
    return 0;
}

void func_00117A10(void) {
    scrClearProcessGlobals();
    func_0023A028();
    *(u32 *)(D_00435DD0 + 0xa5c) = 8;
    func_0011AB38();
    func_00122B58(0);
    func_00313C40();
    func_0026CE90();
    func_00314200();
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

INCLUDE_ASM(const s32, "game/code_001176A0", sdfFirePendingCallback);

u8 func_001186C8(s64 arg0) {
    s64 temp_v0;

    temp_v0 = func_0011D588();
    return temp_v0 == arg0;
}

INCLUDE_ASM(const s32, "game/code_001176A0", func_001186F8);

INCLUDE_ASM(const s32, "game/code_001176A0", func_00118798);

INCLUDE_ASM(const s32, "game/code_001176A0", func_00118898);

INCLUDE_ASM(const s32, "game/code_001176A0", func_001188F0);

INCLUDE_ASM(const s32, "game/code_001176A0", func_001189D0);

INCLUDE_ASM(const s32, "game/code_001176A0", func_00118AB0);

INCLUDE_ASM(const s32, "game/code_001176A0", sdfDispatchCmd);

INCLUDE_ASM(const s32, "game/code_001176A0", func_00118BA8);

void func_00118C58(u32 arg0, u32 arg1, u32 arg2, u8 arg3) {
    evtRunContext(10, arg1, arg2, arg0, arg3);
}

void func_00118C80(u32 arg0, u32 arg1, u32 arg2, u8 arg3) {
    evtRunContext(7, arg1, arg2, arg0, arg3);
}

INCLUDE_ASM(const s32, "game/code_001176A0", sdfDispatchSubCmd);

INCLUDE_ASM(const s32, "game/code_001176A0", func_00118CC0);

INCLUDE_ASM(const s32, "game/code_001176A0", func_00118D60);

INCLUDE_ASM(const s32, "game/code_001176A0", func_001190B0);

INCLUDE_ASM(const s32, "game/code_001176A0", func_00119480);

INCLUDE_ASM(const s32, "game/code_001176A0", func_001194E8);

INCLUDE_ASM(const s32, "game/code_001176A0", func_00119548);

void func_00119710(s32 arg0, u16 arg1) {
    *(u16 *)(arg0 + 0xe) = (*(u16 *)(arg0 + 0xe) & 0x8000) | (arg1 & 0x7fff);
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

