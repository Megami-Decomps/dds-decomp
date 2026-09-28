#include "common.h"

extern u64 dds3GetWorldSecondaryObject(void);

extern s32 func_00110C70(u64, u64, u64);

extern u32 D_00435DA0;

typedef struct EffectObjectData {
    u32 word00;
    u32 word04;
    u32 word08;
    u32 word0C;
    s32 activeId;
    u32 word14;
    u32 word18;
    u32 pendingValue;
    u32 timer;
} EffectObjectData;

typedef struct EffectObject {
    u8 pad00[0x18];
    EffectObjectData *data;
} EffectObject;

u32 func_00113308(EffectObject *object) {
    return object->data->word00;
}

u32 func_00113318(EffectObject *object) {
    return object->data->word08;
}

void func_00113328(EffectObject *object, u32 value) {
    object->data->word14 = value;
}

INCLUDE_ASM(const s32, "game/code_00113308", func_00113338);

INCLUDE_ASM(const s32, "game/code_00113308", func_00113408);

INCLUDE_ASM(const s32, "game/code_00113308", func_00113560);

void func_00113660(EffectObject *object, u32 value) {
    EffectObjectData *data;

    data = object->data;
    dds3SetObjectFlags(object, 0x2000);
    data->pendingValue = value;
    data->timer = 0;
}

void func_001136A0(EffectObject *object) {
    EffectObjectData *data;

    data = object->data;
    data->timer = 0x1e;
    data->pendingValue = 0;
}

INCLUDE_ASM(const s32, "game/code_00113308", func_001136B8);

void func_00113760(u32 arg0) {
    s32 *piVar1;

    func_00113CD0();
    effObjFreeInner(arg0);
    piVar1 = *(s32 **)((s32)arg0 + 0x18);
    if (*piVar1 != -1) {
        *piVar1 = -1;
    }
    if (piVar1[2] != 0) {
        func_0023CD98(piVar1[2]);
        piVar1[2] = 0;
    }
    func_00111D68(arg0);
    func_00111A68(piVar1[3]);
    func_00328E48(*(u32 *)((s32)arg0 + 0x18));
}

INCLUDE_ASM(const s32, "game/code_00113308", func_001137D8);

INCLUDE_ASM(const s32, "game/code_00113308", func_00113AB0);

void func_00113CD0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x18);
    if (*(s32 *)(temp_v0 + 0x10) != -1) {
        func_00116958(arg0, 10);
        *(u32 *)(temp_v0 + 0x10) = 0xffffffff;
    }
}

INCLUDE_ASM(const s32, "game/code_00113308", func_00113D18);

u32 func_00113F00(s32 arg0) {
    return **(u32 **)(arg0 + 0x18);
}

INCLUDE_ASM(const s32, "game/code_00113308", func_00113F10);

void func_00113FD0(void) {
    func_00110B50();
}

void func_00113FE8(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 0xc) = arg1;
}

void func_00113FF8(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 4) = arg1;
}

u32 func_00114008(u64 arg0) {
    s32 temp_v0;
    u64 temp_v1;

    temp_v1 = dds3GetWorldSecondaryObject();
    temp_v0 = func_00110C70(temp_v1, arg0, 6);
    return *(u32 *)(*(s32 *)(temp_v0 + 0x18) + 4);
}

void func_00114048(s32 arg0, u32 arg1) {
    *(u32 *)(*(s32 *)(arg0 + 0x18) + 8) = arg1;
}

u32 func_00114058(s32 arg0) {
    return *(u32 *)(*(s32 *)(arg0 + 0x18) + 8);
}

void func_00114068(u32 arg0) {
    D_00435DA0 = arg0;
}

INCLUDE_ASM(const s32, "game/code_00113308", func_00114070);

void func_00114110(s32 arg0) {
    u32 *puVar1;

    effObjFreeInner();
    puVar1 = *(u32 **)(arg0 + 0x18);
    func_00111A68(*puVar1);
    func_00328E48(puVar1);
}

INCLUDE_RODATA(const s32, "game/code_00113308", D_004128A0);

INCLUDE_RODATA(const s32, "game/code_00113308", D_004128B0);

INCLUDE_ASM(const s32, "game/code_00113308", func_00114150);

INCLUDE_ASM(const s32, "game/code_00113308", func_00114428);

INCLUDE_ASM(const s32, "game/code_00113308", func_00114618);

INCLUDE_ASM(const s32, "game/code_00113308", func_00114640);

INCLUDE_ASM(const s32, "game/code_00113308", func_00114770);

INCLUDE_SDATA(const s32, "game/code_00113308", D_00435DA0);

