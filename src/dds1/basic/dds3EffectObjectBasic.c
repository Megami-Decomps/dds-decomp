#include "common.h"
#include "pcp_vu0.h"

typedef struct {
    void *objectHandle; /* 0x0 passed to func_00111840, returned by effObjGetObjectHandle */
    u32 flags;   /* 0x4 effect flag bits */
    s32 unk8;    /* 0x8 cleared ^6 by func_001158B8 */
    void *bill;   /* 0xC passed to func_00151E60/billSetKind1Entry */
    u32 unk10;   /* 0x10 cleared by func_00115BB8 */
    u32 unk14;   /* 0x14 cleared by func_00115BB8 */
    void *unk18; /* 0x18 cleared by func_00115BB8 */
    u8 pad1C[4]; /* 0x1C */
    void *unk20; /* 0x20 set to the owner by func_00115BB8 */
    u16 unk24;   /* 0x24 */
    u16 unk26;   /* 0x26 kind copied from +0xF */
    void *unk28; /* 0x28 vector passed to func_00190130 */
    void *unk2C; /* 0x2C node handle owned by func_00115318 */
} EffectData; /* 0x30 bytes */

typedef struct {
    u8 pad0[0x60];
    f32 parameter;
} EffectParameters;

typedef struct {
    u8 pad0[4];              /* 0x0 */
    u32 unk4;                /* 0x4 world counter copied by func_00114A78 */
    u8 pad8[7];              /* 0x8 */
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
void *func_001150F0();
void func_00115398(void);
/* Old-style (K&R) callee: callers pass (kind, value) positionally. */
void *func_00115478();
EffectData *func_001158B8(EffectObj *obj);
void *func_002D0918(void *arg);
void *func_002EB028(void *arg0, u32 *arg1, s32 arg2);

extern void *func_0014FE28(void);

extern void *func_0014FE48(void);

extern void *func_0014FD20(void);

extern void *billCreateFromResource(s32 arg0, s32 arg1);

extern void *billCreateIndexed(s32 arg0, u32 arg1);

extern void *func_00151E60(void *arg);

extern s32 func_002D9E58(void *param, s32 id);

extern void effEventReleaseNode(void *node);

extern void *func_00190130(void *bill, u32 id, void *vec);

extern EffectObj *func_00110880(s32 kind);

extern void dds3EnsureSlotData(void *obj);

extern void effObjSetInnerFirstVec(void *obj, void *vec);

extern void effObjSetInnerSecondVec(void *obj, void *vec);

extern void effObjInnerVecBackup(void *params);

/* Release the effect's dependent resources before clearing its data handle. */
void effObjReleaseObjectData(EffectObj *obj) {
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

EffectObj *func_00114A78(u32 arg0, void *arg1, void *arg2) {
    EffectObj *obj;
    EffectData *data;

    obj = func_00110880(7);
    if (obj == NULL) {
        return NULL;
    }
    obj->unk4 = arg0;
    dds3EnsureSlotData(obj);
    effObjSetInnerFirstVec(obj, arg1);
    effObjSetInnerSecondVec(obj, arg2);
    effObjInnerVecBackup(obj->params);
    data = obj->data;
    data->flags = 0;
    data->unk8 = 0;
    data->bill = NULL;
    data->unk20 = NULL;
    data->unk24 = 0;
    data->unk26 = 0;
    return obj;
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00114B18);

/* Resolve the object's billboard and forward its vector and extra argument. */
void func_00114BF0(EffectObj *obj, void *vec, s32 extra) {
    void *handle;

    handle = func_00151E60(obj->data->bill);
    func_00114B18(handle, vec, extra);
}

/* Create an indexed billboard of kind one and dispatch it. */
void effObjCreateIndexedKindOne(u32 billId, void *vec, s32 extra) {
    void *handle;

    handle = billCreateIndexed(1, billId);
    func_00114B18(handle, vec, extra);
}

/* Create a resource-backed billboard of kind one and dispatch it. */
void effObjCreateResourceKindOne(s32 billId, void *vec, s32 extra) {
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

void effObjCreateIndexedKindZero(u32 billId, void *vec, s32 extra) {
    void *handle;

    handle = billCreateIndexed(0, billId);
    func_00114CE8(handle, vec, extra);
}

void effObjCreateResourceKindZero(s32 billId, void *vec, s32 extra) {
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

void *func_001152B0(void *arg0, void *arg1, void *arg2) {
    u32 local;
    void *result;
    void *result2;

    local = 0;
    result = func_002EB028(arg0, &local, 0);
    result2 = func_001150F0(local, arg1, arg2);
    func_002D0918(result);
    return result2;
}

void func_00115318(EffectObj *obj, u32 arg1) {
    EffectData *data;

    data = obj->data;
    if (data->unk8 != 5) {
        return;
    }
    if (data->unk28 == NULL) {
        return;
    }
    if (data->bill == NULL) {
        return;
    }
    if (data->unk2C != NULL) {
        effEventReleaseNode(data->unk2C);
        data->unk2C = NULL;
    }
    data->unk2C = func_00190130(data->bill, arg1 & 0xFFFF, data->unk28);
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115398);

void func_00115460(void) {
    func_00115398();
}

INCLUDE_ASM(const s32, "basic/dds3EffectObjectBasic", func_00115478);

void func_00115840(void) {
    func_00115478();
}

void *func_00115858(s32 kind, void *arg1) {
    u32 local;
    void *result;
    void *result2;

    local = 0;
    result = func_002EB028(arg1, &local, 0);
    result2 = func_00115478(kind, local);
    func_002D0918(result);
    return result2;
}

EffectData *func_001158B8(EffectObj *obj) {
    EffectData *data;

    data = NULL;
    if (obj == NULL) {
        return data;
    }
    if (obj->unkF != 7) {
        return data;
    }
    data = obj->data;
    if (data->unk8 != 6) {
        data = NULL;
    }
    return data;
}

s32 func_001158F0(EffectObj *obj) {
    if (func_001158B8(obj) == NULL) {
        return 0;
    }
    VU0_LOAD_VF(vf10, (u8 *)obj->params + 0x40);
    return 1;
}

/* Return the object's scalar parameter as an integer when data is present. */
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

s32 func_00115BB8(EffectObj *arg0, EffectObj *arg1) {
    EffectData *data;
    u8 kind;

    kind = arg1->unkF;
    if (kind < 4 || (kind >= 10 && kind != 0x11)) {
        return 0;
    }
    data = arg0->data;
    data->unk18 = NULL;
    data->unk20 = arg1;
    data->unk24 = 0;
    data->flags |= 4;
    data->flags &= ~8;
    data->unk10 = 0;
    data->unk26 = arg1->unkF;
    data->unk14 = 0;
    return 1;
}

/* Billboard payload: state word and the SDF parameter block it was built from. */
typedef struct {
    u8 pad0[8];   /* 0x0 */
    u32 unk8;     /* 0x8 */
    void *unkC;   /* 0xC */
} BillPayload;

typedef struct {
    u8 pad0[0x18]; /* 0x0 */
    void *unk18;   /* 0x18 */
} BillParam;

void func_00115C20(EffectObj *obj) {
    EffectData *data;
    EffectObj *owner;
    BillPayload *bill;

    data = obj->data;
    if (data->flags & 8) {
        owner = data->unk20;
        if (owner->unkF == 5) {
            bill = owner->data->bill;
            if (bill->unk8 != 0) {
                return;
            }
            func_002D9E58(((BillParam *)bill->unkC)->unk18, data->unk24);
        }
    }
}

s32 func_00115C80(EffectObj *obj, EffectObj *arg1, s32 arg2) {
    EffectData *data;

    if (func_00115BB8(obj, arg1) == 0) {
        return 1;
    }
    data = obj->data;
    data->unk24 = arg2;
    data->flags |= 8;
    return 1;
}

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

