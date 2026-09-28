#include "common.h"

extern u64 func_00111838(u64);

extern void *objGetSlot(void *arg0, s32 index);

void func_001113F0(void *arg0, void *arg1);

void func_001118C0(void *arg0, void *arg1);

extern s32 func_00112AB0(void);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111A68);

void func_00111B30(u32 arg0, u32 arg1) {
    u32 *puVar1;

    puVar1 = (u32 *)func_00112AB0();
    *puVar1 = *puVar1 | arg1;
}

void func_00111B60(u32 arg0, u32 arg1) {
    u32 *puVar1;

    puVar1 = (u32 *)func_00112AB0();
    *puVar1 = *puVar1 & ~arg1;
}

u8 func_00111B98(u32 arg0, u32 arg1) {
    u32 *puVar1;

    puVar1 = (u32 *)func_00112AB0();
    return (*puVar1 & arg1) != 0;
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111BC8);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", objGetExtData);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", objSetSlotByKind);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", objExchangeSlot);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", objGetSlot);

u32 objGetUnk04(void) {
    s32 temp_v0;

    temp_v0 = func_00112AB0();
    return *(u32 *)(temp_v0 + 4);
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", objGetUnk0C);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111D68);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00111E00);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112058);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112168);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112230);

s32 objInvokeSlot5Handler(void *arg0) {
    void *v;

    v = objGetSlot(arg0, 5);
    if (v == NULL) {
        return 0;
    }
    func_001113F0(v, arg0);
    return 1;
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112328);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112518);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_00112978);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", func_001129C8);

s32 objInvokeSlot1Handler(void *arg0, void *arg1) {
    void *v;

    v = objGetSlot(arg0, 1);
    if (v == NULL) {
        return 0;
    }
    func_001118C0(v, arg1);
    return 1;
}

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", objRunSlot1Handlers);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", objReleaseSlot1Data);

INCLUDE_ASM(const s32, "basic/dds3ObjectBase", objGetSlot1Data);

INCLUDE_SDATA(const s32, "basic/dds3ObjectBase", D_00435D98);

