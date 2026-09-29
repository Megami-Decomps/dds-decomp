#include "common.h"

typedef struct {
    void *objectHandle; /* 0x0 passed to func_00111840, returned by effObjGetObjectHandle */
    u32 flags;   /* 0x4 effect flag bits */
    u32 unk8;    /* 0x8 cleared ^6 by func_001158B8 */
    void *bill;   /* 0xC passed to func_00151E60/billSetKind1Entry */
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
    u8 pad[0xF];             /* 0x0 */
    u8 unkF;                 /* 0xF kind checked ==7 by func_001158B8 */
    u8 pad10[8];             /* 0x10 */
    EffectData *data;         /* 0x18 */
    EffectParameters *params; /* 0x1C vector base read by func_001158F0/effObjGetIntParam */
} EffectObj;

void func_001143D8(void *arg);
void effObjFreeInner(void *arg);
void func_00111840(void *arg);
void func_002CFF98(void *arg);
/* Dispatchers take (bill handle, 16-byte vector, extra); the vector is
   loaded with lqc2 and the extra is forwarded to func_00114A78. */
void func_00114B18(void *arg0, void *vec, s32 arg2);
void billSetKind1Entry(void *arg);
void func_00114CE8(void *arg0, void *vec, s32 arg2);
void func_00114E90(void *arg0, void *vec, s32 arg2);
void func_00114FE0(void *arg0, void *vec, s32 arg2);
void func_001150F0(void);
void func_00115398(void);
/* Old-style (K&R) callee: callers pass (object, value) positionally. */
void func_00115478();
EffectData *func_001158B8(EffectObj *obj);
void *func_002D0918(void *arg);
void *func_002EB028(void *arg0, u32 *arg1, s32 arg2);

extern void *func_0014FE28(void);

extern void *func_0014FE48(void);

extern void *func_0014FD20(void);

extern void *billCreateFromResource(s32 arg0, s32 arg1);

extern void *billCreateIndexed(s32 arg0, u32 arg1);

extern void *func_00151E60(void *arg);

void func_00114570(EffectObj *obj) {
    EffectData *data;

    data = obj->data;
    func_001143D8(data);
    effObjFreeInner(obj);
    func_00111840(data->objectHandle);
    func_002CFF98(obj->data);
    obj->data = NULL;
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001145C0);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114988);

void *effObjGetObjectHandle(EffectObj *obj) {
    return obj->data->objectHandle;
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114A78);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114B18);

void func_00114BF0(EffectObj *obj, void *vec, s32 extra) {
    void *handle;

    handle = func_00151E60(obj->data->bill);
    func_00114B18(handle, vec, extra);
}

void func_00114C38(u32 billId, void *vec, s32 extra) {
    void *handle;

    handle = billCreateIndexed(1, billId);
    func_00114B18(handle, vec, extra);
}

void func_00114C80(s32 billId, void *vec, s32 extra) {
    void *handle;

    handle = billCreateFromResource(1, billId);
    func_00114B18(handle, vec, extra);
}

void func_00114CC8(EffectObj *obj) {
    billSetKind1Entry(obj->data->bill);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114CE8);

void func_00114DB8(EffectObj *obj, void *vec, s32 extra) {
    void *handle;

    handle = func_00151E60(obj->data->bill);
    func_00114CE8(handle, vec, extra);
}

void func_00114E00(u32 billId, void *vec, s32 extra) {
    void *handle;

    handle = billCreateIndexed(0, billId);
    func_00114CE8(handle, vec, extra);
}

void func_00114E48(s32 billId, void *vec, s32 extra) {
    void *handle;

    handle = billCreateFromResource(0, billId);
    func_00114CE8(handle, vec, extra);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114E90);

void func_00114F60(u32 arg0, void *vec, s32 arg2) {
    void *handle;

    handle = func_0014FD20();
    func_00114E90(handle, vec, arg2);
}

void func_00114FA0(u32 arg0, void *vec, s32 arg2) {
    void *handle;

    handle = func_0014FE48();
    func_00114E90(handle, vec, arg2);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114FE0);

void func_001150B0(u32 arg0, void *vec, s32 arg2) {
    void *handle;

    handle = func_0014FE28();
    func_00114FE0(handle, vec, arg2);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001150F0);

void func_00115298(void) {
    func_001150F0();
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001152B0);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115318);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115398);

void func_00115460(void) {
    func_00115398();
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115478);

void func_00115840(void) {
    func_00115478();
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115858);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001158B8);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001158F0);

s32 effObjGetIntParam(EffectObj *obj) {
    EffectParameters *parameters;

    if (func_001158B8(obj) == NULL) {
        return 0;
    }
    parameters = obj->params;
    return (s32)parameters->parameter;
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115970);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_001159E8);

void effObjSetFlags(EffectObj *obj, s32 flags) {
    obj->data->flags |= flags;
}

void effObjClearFlags(EffectObj *obj, s32 flags) {
    obj->data->flags &= ~flags;
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115BB8);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115C20);

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115C80);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F7D0);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F800);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F828);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F878);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F890);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F8A8);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F8C0);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F8D0);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F8E8);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F900);

INCLUDE_RODATA(const s32, "basic/dds3EffectObjectBasic", D_0039F918);

