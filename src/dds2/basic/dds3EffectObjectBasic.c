#include "common.h"

extern u64 func_001579C8(void);

extern u64 func_001579E8(void);

extern u64 func_001578C0(void);

extern u64 billCreateFromResource(u64, u64);

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
    u8 pad0[0x60];
    f32 parameter;
} EffectParameters;

typedef struct {
    u8 pad[0xF];       /* 0x0 */
    u8 unkF;           /* 0xF kind checked ==7 by func_001158B8 */
    u8 pad10[8];       /* 0x10 */
    EffectData *data; /* 0x18 */
    EffectParameters *params; /* 0x1C vector base; scalar parameter at +0x60 */
} EffectObj;

EffectData *func_00115B20(EffectObj *obj);

/* Release the effect's dependent resources before clearing its data handle. */
void effObjReleaseObjectData(u32 object) {
    u32 *data;
    s32 objectAddress;

    objectAddress = (s32)object;
    data = *(u32 **)(objectAddress + 0x18);
    func_00114640(data);
    effObjFreeInner(object);
    func_00111A68(*data);
    func_00328E48(*(u32 *)(objectAddress + 0x18));
    *(u32 *)(objectAddress + 0x18) = 0;
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114828);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114BF0);

u32 effObjGetObjectHandle(EffectObj *obj) {
    return (u32)obj->data->objectHandle;
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114CE0);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114D80);

/* Resolve the object's billboard and forward its two draw arguments. */
void func_00114E58(EffectObj *obj, u64 arg1, u64 arg2) {
    u64 bill;

    bill = func_00159A50((u32)obj->data->bill);
    func_00114D80(bill, arg1, arg2);
}
/* Create an indexed billboard of kind one and dispatch it. */
void effObjCreateIndexedKindOne(u64 billId, u64 arg1, u64 arg2) {
    u64 bill;

    bill = billCreateIndexed(1, billId);
    func_00114D80(bill, arg1, arg2);
}
/* Create a resource-backed billboard of kind one and dispatch it. */
void effObjCreateResourceKindOne(u64 resourceId, u64 arg1, u64 arg2) {
    u64 bill;

    bill = billCreateFromResource(1, resourceId);
    func_00114D80(bill, arg1, arg2);
}

/* Select the billboard object's kind-one entry. */
void func_00114F30(EffectObj *obj) {
    billSetKind1Entry((u32)obj->data->bill);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114F50);

/* Resolve the object's billboard and dispatch through the second handler. */
void func_00115020(EffectObj *obj, u64 arg1, u64 arg2) {
    u64 bill;

    bill = func_00159A50((u32)obj->data->bill);
    func_00114F50(bill, arg1, arg2);
}

/* Create an indexed billboard of kind zero for the second handler. */
void effObjCreateIndexedKindZero(u64 billId, u64 arg1, u64 arg2) {
    u64 bill;

    bill = billCreateIndexed(0, billId);
    func_00114F50(bill, arg1, arg2);
}

/* Create a resource-backed billboard of kind zero for the second handler. */
void effObjCreateResourceKindZero(u64 resourceId, u64 arg1, u64 arg2) {
    u64 bill;

    bill = billCreateFromResource(0, resourceId);
    func_00114F50(bill, arg1, arg2);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001150F8);

/* Dispatch a newly allocated handle from the first parameter source. */
void func_001151C8(u64 arg0, u64 arg1, u64 arg2) {
    u64 handle;

    handle = func_001578C0();
    func_001150F8(handle, arg1, arg2);
}

/* Dispatch a newly allocated handle from the second parameter source. */
void func_00115208(u64 arg0, u64 arg1, u64 arg2) {
    u64 handle;

    handle = func_001579E8();
    func_001150F8(handle, arg1, arg2);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115248);

/* Dispatch a newly allocated handle from the third parameter source. */
void func_00115318(u64 arg0, u64 arg1, u64 arg2) {
    u64 handle;

    handle = func_001579C8();
    func_00115248(handle, arg1, arg2);
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

/* Return the object's scalar parameter as an integer when data is present. */
s32 effObjGetIntParam(EffectObj *obj) {
    EffectParameters *parameters;

    if (func_00115B20(obj) == NULL) {
        return 0;
    }
    parameters = obj->params;
    return (s32)parameters->parameter;
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

