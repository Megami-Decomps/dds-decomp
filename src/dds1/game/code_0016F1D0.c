#include "common.h"

typedef struct {
    u8 pad0[8];
    f32 value;
    u8 padC[4];
} EffectVectorRecord;

typedef struct {
    u8 pad0[0x50];
    f32 increment;
    u32 value54;
    EffectVectorRecord *vectors;
    u8 pad5C[4];
    u8 *records;
    u8 *indices;
    u32 handle68;
    u32 handle6C;
    u8 pad70[8];
    u32 handle78;
    u32 handle7C;
} EffectRecordGroup;

extern u64 effParamTableGetBlock(u64, u64);

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_0016F1D0);

void func_0016F420(u64 arg0) {
    u64 temp_v0;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    func_0016F1D0(temp_v0);
}

void func_0016F440(void) {
    func_0016F1D0();
}

void func_0016F458(EffectRecordGroup *group) {
    func_001705A0(group->handle7C);
    func_002D0918(group->handle78);
}

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_0016F488);

void func_0016F498(EffectRecordGroup *group, u32 records) {
    group->records = (u8 *)records;
}

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_0016F4A0);

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_0016F4A8);

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_0016F4D8);

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_0016F5C8);

void func_0016F790(EffectRecordGroup *group, s32 index) {
    EffectVectorRecord *record;

    record = &group->vectors[index];
    record->value = record->value + group->increment;
}

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_0016F7B0);

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_0016FB08);

void func_0016FC28(EffectRecordGroup *group) {
    func_002DAA68(group->handle68);
    func_002D0918(group->handle6C);
}

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_0016FC58);

s32 func_0016FF08(EffectRecordGroup *group, s32 index) {
    return (s32)group->records + index * 0x50;
}

s32 func_0016FF20(EffectRecordGroup *group, s32 index) {
    return (s32)group->indices + index * 0x14;
}

void func_0016FF38(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x50) = arg1;
}

void func_0016FF40(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x54) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_0016FF48);

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_0016FF50);

void func_00170048(EffectRecordGroup *group) {
    func_002DAA68(group->handle68);
    func_002D0918(group->handle6C);
}

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_00170078);

s32 func_00170220(EffectRecordGroup *group, s32 index) {
    return (s32)group->records + index * 0x30;
}

s32 func_00170238(EffectRecordGroup *group, s32 index) {
    return (s32)group->indices + index * 0xc;
}

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_00170250);

void func_00170350(EffectRecordGroup *group) {
    func_002DAA68(group->handle68);
    func_002D0918(group->handle6C);
}

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_00170380);

s32 func_00170538(EffectRecordGroup *group, s32 index) {
    return (s32)group->records + index * 0x40;
}

s32 func_00170548(EffectRecordGroup *group, s32 index) {
    return (s32)group->indices + index * 0x10;
}

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_00170558);

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_001705A0);

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_001705B8);

s32 func_00170858(EffectRecordGroup *group, s32 index) {
    return (s32)group->records + index * 0x50;
}

s32 func_00170870(EffectRecordGroup *group, s32 index) {
    return (s32)group->indices + index * 0x14;
}

void func_00170888(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x50) = arg1;
}

void func_00170890(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x54) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_00170898);

INCLUDE_ASM(const s32, "game/code_0016F1D0", func_001708A0);
