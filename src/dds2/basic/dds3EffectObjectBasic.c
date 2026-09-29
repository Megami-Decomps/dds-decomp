#include "common.h"

extern u64 func_001579C8(void);

extern u64 func_001579E8(void);

extern u64 func_001578C0(void);

extern u64 func_001599F8(u64, u64);

extern u64 billCreateIndexed(u64, u64);

extern u64 func_00159A50(u32);

typedef struct {
    void *objectHandle; /* 0x0 returned by effObjGetObjectHandle */
    u32 flags; /* 0x4 effect flag bits */
    u32 unk8;    /* 0x8 cleared ^6 by func_001158B8 */
    void *bill; /* 0xC bill object */
    u32 unk10;   /* 0x10 cleared by func_00115BB8 */
    u32 unk14;   /* 0x14 cleared by func_00115BB8 */
    void *unk18; /* 0x18 cleared by func_00115BB8 */
    u8 pad1C[4]; /* 0x1C */
    void *unk20; /* 0x20 set to the owner by func_00115BB8 */
    u16 unk24;   /* 0x24 */
    u16 unk26;   /* 0x26 kind copied from +0xF */
} EffectData; /* 0x28 bytes */

typedef struct {
    u8 pad[0xF];       /* 0x0 */
    u8 unkF;           /* 0xF kind checked ==7 by func_001158B8 */
    u8 pad10[8];       /* 0x10 */
    EffectData *data; /* 0x18 */
    void *params; /* 0x1C vector base */
} EffectObj;

EffectData *func_00115B20(EffectObj *obj);

void func_001147D8(u32 arg0) {
    u32 *puVar1;
    s32 temp_v0;

    temp_v0 = (s32)arg0;
    puVar1 = *(u32 **)(temp_v0 + 0x18);
    func_00114640(puVar1);
    effObjFreeInner(arg0);
    func_00111A68(*puVar1);
    func_00328E48(*(u32 *)(temp_v0 + 0x18));
    *(u32 *)(temp_v0 + 0x18) = 0;
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114828);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114BF0);

u32 effObjGetObjectHandle(EffectObj *obj) {
    return (u32)obj->data->objectHandle;
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114CE0);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114D80);

void func_00114E58(EffectObj *obj, u64 arg1, u64 arg2) {
    u64 bill;

    bill = func_00159A50((u32)obj->data->bill);
    func_00114D80(bill, arg1, arg2);
}

void func_00114EA0(u64 arg0, u64 arg1, u64 arg2) {
    u64 temp_v0;

    temp_v0 = billCreateIndexed(1, arg0);
    func_00114D80(temp_v0, arg1, arg2);
}

void func_00114EE8(u64 arg0, u64 arg1, u64 arg2) {
    u64 temp_v0;

    temp_v0 = func_001599F8(1, arg0);
    func_00114D80(temp_v0, arg1, arg2);
}

void func_00114F30(EffectObj *obj) {
    func_00159DF0((u32)obj->data->bill);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114F50);

void func_00115020(EffectObj *obj, u64 arg1, u64 arg2) {
    u64 bill;

    bill = func_00159A50((u32)obj->data->bill);
    func_00114F50(bill, arg1, arg2);
}

void func_00115068(u64 arg0, u64 arg1, u64 arg2) {
    u64 temp_v0;

    temp_v0 = billCreateIndexed(0, arg0);
    func_00114F50(temp_v0, arg1, arg2);
}

void func_001150B0(u64 arg0, u64 arg1, u64 arg2) {
    u64 temp_v0;

    temp_v0 = func_001599F8(0, arg0);
    func_00114F50(temp_v0, arg1, arg2);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001150F8);

void func_001151C8(u64 arg0, u64 arg1, u64 arg2) {
    u64 temp_v0;

    temp_v0 = func_001578C0();
    func_001150F8(temp_v0, arg1, arg2);
}

void func_00115208(u64 arg0, u64 arg1, u64 arg2) {
    u64 temp_v0;

    temp_v0 = func_001579E8();
    func_001150F8(temp_v0, arg1, arg2);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115248);

void func_00115318(u64 arg0, u64 arg1, u64 arg2) {
    u64 temp_v0;

    temp_v0 = func_001579C8();
    func_00115248(temp_v0, arg1, arg2);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115358);

void func_00115500(void) {
    func_00115358();
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115518);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115580);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115600);

void func_001156C8(void) {
    func_00115600();
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001156E0);

void func_00115AA8(void) {
    func_001156E0();
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115AC0);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115B20);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115B58);

s32 effObjGetIntParam(EffectObj *obj) {
    void *p;

    if (func_00115B20(obj) == NULL) {
        return 0;
    }
    p = obj->params;
    return (s32)(*(f32 *)((u8 *)p + 0x60));
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115BD8);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115C50);

void effObjSetFlags(EffectObj *obj, u32 flags) {
    obj->data->flags = obj->data->flags | flags;
}

void effObjClearFlags(EffectObj *obj, u32 flags) {
    obj->data->flags = obj->data->flags & ~flags;
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115E20);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115E88);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115EE8);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412950);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412980);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_004129A8);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_004129F8);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412A10);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412A28);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412A40);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412A50);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412A68);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412A80);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_00412A98);

